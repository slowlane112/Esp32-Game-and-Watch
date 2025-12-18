#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_intr_alloc.h"
#include "i2c.h"

// ###### Cardputer adv ###### 

#define ADV_I2C_PORT            I2C_NUM_0
#define ADV_SDA_PIN             8
#define ADV_SCL_PIN             9
#define ADV_INT_PIN             11
#define ADV_TCA8418_ADDR        0x34 
#define ADV_REG_CFG             0x01
#define ADV_REG_INT_STAT        0x02
#define ADV_REG_KEY_LCK_EC      0x03
#define ADV_REG_KEY_EVENT       0x04
#define ADV_REG_INT_EN          0x07
#define ADV_REG_KP_GPIO1        0x1D
#define ADV_REG_KP_GPIO2        0x1E
#define ADV_REG_KP_GPIO3        0x1F
#define ADV_TCA8418_I2C_SPEED   400000
#define ADV_I2C_MASTER_TIMEOUT  50
#define ADV_NUM_ROWS            8
#define ADV_NUM_COLS            9

static uint8_t adv_key_map_state[ADV_NUM_ROWS][ADV_NUM_COLS] = {{0}};
static QueueHandle_t adv_key_event_queue = NULL;
static char adv_key_map[ADV_NUM_ROWS][ADV_NUM_COLS] = {
	
    [0][0] = '`',	
    [4][0] = '1',
    [2][1] = '2',
    [6][1] = '3',
    [4][2] = '4',
    [0][3] = '5',
    [6][3] = '6',
    [2][4] = '7',
    [0][5] = '8',
    [4][5] = '9',
    [2][6] = '0',
    [6][6] = '_',
    [4][7] = '=',
    [0][8] = 0x87, // Backspace
    
    [1][0] = 0x88, // Tab
    [5][0] = 'Q',
    [3][1] = 'W',	
    [7][1] = 'E',
    [5][2] = 'R',	
    [1][3] = 'T',
    [7][3] = 'Y',	
    [3][4] = 'U',
    [1][5] = 'I',	
    [5][5] = 'O',
    [3][6] = 'P',	
    [7][6] = '[',
    [5][7] = ']',	
    [1][8] = '\\',
    
	[2][0] = 0x85, // Fn
    [6][0] = 0x84, // Aa
    [4][1] = 'A',	
    [0][2] = 'S',
    [6][2] = 'D',	
    [2][3] = 'F',
    [0][4] = 'G',	
    [4][4] = 'H',
    [2][5] = 'J',	
    [6][5] = 'K',
    [4][6] = 'L',	
    [0][7] = ';',
    [6][7] = '\'',	
    [2][8] = 0x86, // Enter
    
    [3][0] = 0x80, // Ctrl
	[7][0] = 0x82, // opt
    [5][1] = 0x81, // Alt
    [1][2] = 'Z',     
    [7][2] = 'X',
    [3][3] = 'C',
    [1][4] = 'V',
    [5][4] = 'B',
    [3][5] = 'N',
    [7][5] = 'M',
    [5][6] = ',',
    [1][7] = '.',
    [7][7] = '/',
    [3][8] = 0x20 // Space
    
};
    
static esp_err_t adv_tca8418_write_reg(uint8_t reg_addr, uint8_t data) {
    uint8_t write_buf[2] = {reg_addr, data};
    return i2c_master_write_to_device(
        ADV_I2C_PORT, 
        ADV_TCA8418_ADDR, 
        write_buf, 2, 
        ADV_I2C_MASTER_TIMEOUT / portTICK_PERIOD_MS
    );
}

static esp_err_t adv_tca8418_read_mult_reg(uint8_t reg_addr, uint8_t *data_buf, size_t len) {
    return i2c_master_write_read_device(
        ADV_I2C_PORT, 
        ADV_TCA8418_ADDR,
        &reg_addr, 1, 
        data_buf, len, 
        ADV_I2C_MASTER_TIMEOUT / portTICK_PERIOD_MS
    );
}

static void adv_keycode_to_rc(uint8_t keycode, int *r, int *c) {
    if (keycode == 0) { *r = -1; *c = -1; return; }
    // Keycode = 1 + (Column Index * 8) + Row Index
    *r = (keycode - 1) % 8; // row index 0-7
    *c = (keycode - 1) / 8; // column index 0-9
}

static void adv_tca8418_event_task(void *arg) {
    uint32_t event_val;
    uint8_t key_event_byte;

    while (1) {
        if (xQueueReceive(adv_key_event_queue, &event_val, portMAX_DELAY) == pdTRUE) {
            
            uint8_t count_and_lck;

            while (adv_tca8418_read_mult_reg(ADV_REG_KEY_LCK_EC, &count_and_lck, 1) == ESP_OK) {
                uint8_t event_count = count_and_lck & 0x0F;

                if (event_count == 0) {
                    adv_tca8418_write_reg(ADV_REG_INT_STAT, 0xFF); 
                    break;
                }

                if (adv_tca8418_read_mult_reg(ADV_REG_KEY_EVENT, &key_event_byte, 1) == ESP_OK) {
                    
                    bool is_pressed = (key_event_byte & 0x80) ? true : false;
                    uint8_t key_code = key_event_byte & 0x7F;

                    int r, c;
                    adv_keycode_to_rc(key_code, &r, &c);

                    if (r >= 0 && r < ADV_NUM_ROWS && c >= 0 && c < ADV_NUM_COLS) {
                        adv_key_map_state[r][c] = is_pressed ? 1 : 0;
                        //printf("FIFO Event: R=%d, C=%d, State=%s\n", r, c, is_pressed ? "PRESS" : "RELEASE");
                    }
                }
            }
        }
    }
}

static void IRAM_ATTR adv_tca8418_isr_handler(void *arg) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(adv_key_event_queue, &(uint32_t){1}, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

void adv_keyboard_setup() 
{
    
    adv_tca8418_write_reg(ADV_REG_CFG, 0x01);       
    
    adv_tca8418_write_reg(ADV_REG_KP_GPIO1, 0xFF);
    adv_tca8418_write_reg(ADV_REG_KP_GPIO2, 0xFF);
    adv_tca8418_write_reg(ADV_REG_KP_GPIO3, 0x03);
    adv_tca8418_write_reg(ADV_REG_INT_EN, 0x02);
    adv_tca8418_write_reg(ADV_REG_INT_STAT, 0xFF);

    adv_key_event_queue = xQueueCreate(10, sizeof(uint32_t)); 
    gpio_set_direction(ADV_INT_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(ADV_INT_PIN, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(ADV_INT_PIN, GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(ADV_INT_PIN, adv_tca8418_isr_handler, NULL);

    xTaskCreate(adv_tca8418_event_task, "adv_tca_evt_task", 4096, NULL, 10, NULL);
    
}


// ###### Cardputer Original ###### 

// 74HC138 decoder
#define ORG_GPIO_74HC138_A0    (GPIO_NUM_8)
#define ORG_GPIO_74HC138_A1    (GPIO_NUM_9)
#define ORG_GPIO_74HC138_A2    (GPIO_NUM_11)

// 7 columns
#define ORG_NUM_COLUMNS 7
const gpio_num_t column_gpios[ORG_NUM_COLUMNS] = {
    GPIO_NUM_13,
    GPIO_NUM_15,
    GPIO_NUM_3,
    GPIO_NUM_4,
    GPIO_NUM_5,
    GPIO_NUM_6,
    GPIO_NUM_7
};

const char org_keyboard_map[8][7] = {
    {0x82, 'Z', 'C', 'B', 'M', '.', 0x20},  // opt, ..., Space
    {0x84, 'S', 'F', 'H', 'K', ';', 0x86},  // Aa, ..., Enter
    {'Q',  'E', 'T', 'U', 'O', '[', '\\'},
    {'1',  '3', '5', '7', '9', '-', 0x87},  // ..., Backspace
    {0x80, 0x81, 'X', 'V', 'N', ',', '/'},  // Ctrl, Alt, ...
    {0x85, 'A', 'D', 'G', 'J', 'L', '\''},  // Fn, ...
    {0x88, 'W', 'R', 'Y', 'I', 'P', ']'},   // Tab, ...
    {'`',  '2', '4', '6', '8', '0', '='}
};

void org_keyboard_setup() {

	// 74HC138 inputs
    gpio_config_t out_conf = {
        .pin_bit_mask = ((1ULL << ORG_GPIO_74HC138_A0) | (1ULL << ORG_GPIO_74HC138_A1) | (1ULL << ORG_GPIO_74HC138_A2)),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&out_conf);

    // Columns
    gpio_config_t in_conf = {
        .pin_bit_mask = 0,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    for (int i = 0; i < ORG_NUM_COLUMNS; i++) {
        in_conf.pin_bit_mask |= (1ULL << column_gpios[i]);
    }
    gpio_config(&in_conf);

}

void keyboard_setup() {
	
	if (is_cardputer_adv) 
	{
		adv_keyboard_setup();
	}
	else 
	{
		org_keyboard_setup();
	}
	
}

char keyboard_get_key() {
	if (is_cardputer_adv) 
	{
		for (int r = 0; r < ADV_NUM_ROWS; r++) {
			for (int c = 0; c < ADV_NUM_COLS; c++) {
				if (adv_key_map_state[r][c] == 1) {
					//printf("Key Pressed NOW (Polling): Row=%d, Col=%d %c\n", r, c, adv_key_map[r][c]);
					return adv_key_map[r][c]; 
				}
			}
		}
	}
	else
	{
		for (int row = 0; row < 8; row++) {
			// Set row 74HC138 inputs 
			gpio_set_level(ORG_GPIO_74HC138_A0, (row & 0x01) ? 1 : 0);
			gpio_set_level(ORG_GPIO_74HC138_A1, (row & 0x02) ? 1 : 0);
			gpio_set_level(ORG_GPIO_74HC138_A2, (row & 0x04) ? 1 : 0);

			vTaskDelay(pdMS_TO_TICKS(1));

			// Read the column inputs
			for (int col = 0; col < ORG_NUM_COLUMNS; col++) {
				if (gpio_get_level(column_gpios[col]) == 0) {
					return org_keyboard_map[row][col];
				}
			}
		}
	}

    return '0';
}
