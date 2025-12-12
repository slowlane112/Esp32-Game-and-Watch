#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "keyboard.h"

// 74HC138 decoder
#define GPIO_74HC138_A0    (GPIO_NUM_8)
#define GPIO_74HC138_A1    (GPIO_NUM_9)
#define GPIO_74HC138_A2    (GPIO_NUM_11)

// 7 columns
#define NUM_COLUMNS 7
const gpio_num_t column_gpios[NUM_COLUMNS] = {
    GPIO_NUM_13,
    GPIO_NUM_15,
    GPIO_NUM_3,
    GPIO_NUM_4,
    GPIO_NUM_5,
    GPIO_NUM_6,
    GPIO_NUM_7
};


const char keyboard_map[8][7] = {
    {0x82, 'Z', 'C', 'B', 'M', '.', 0x20},  // opt, ..., Space
    {0x84, 'S', 'F', 'H', 'K', ';', 0x86},  // Aa, ..., Enter
    {'Q',  'E', 'T', 'U', 'O', '[', '\\'},
    {'1',  '3', '5', '7', '9', '-', 0x87},  // ..., Backspace
    {0x80, 0x81, 'X', 'V', 'N', ',', '/'},  // Ctrl, Alt, ...
    {0x85, 'A', 'D', 'G', 'J', 'L', '\''},  // Fn, ...
    {0x88, 'W', 'R', 'Y', 'I', 'P', ']'},   // Tab, ...
    {'`',  '2', '4', '6', '8', '0', '='}
};

void keyboard_setup() {

	// 74HC138 inputs
    gpio_config_t out_conf = {
        .pin_bit_mask = ((1ULL << GPIO_74HC138_A0) | (1ULL << GPIO_74HC138_A1) | (1ULL << GPIO_74HC138_A2)),
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
    for (int i = 0; i < NUM_COLUMNS; i++) {
        in_conf.pin_bit_mask |= (1ULL << column_gpios[i]);
    }
    gpio_config(&in_conf);

}


char keyboard_get_key()
{
	
	// loop each row and check if column is selected
	
	for (int row = 0; row < 8; row++) {
		// Set row 74HC138 inputs 
		gpio_set_level(GPIO_74HC138_A0, (row & 0x01) ? 1 : 0);
		gpio_set_level(GPIO_74HC138_A1, (row & 0x02) ? 1 : 0);
		gpio_set_level(GPIO_74HC138_A2, (row & 0x04) ? 1 : 0);

		vTaskDelay(pdMS_TO_TICKS(1));

		// Read the column inputs
		for (int col = 0; col < NUM_COLUMNS; col++) {
			if (gpio_get_level(column_gpios[col]) == 0) {
				return keyboard_map[row][col];
			}
		}
	}
    
    return '\0';
}

