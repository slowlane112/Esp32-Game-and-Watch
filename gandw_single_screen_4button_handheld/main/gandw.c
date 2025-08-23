#include <stdio.h>
#include <gw_system.h>
#include <gw_romloader.h>
#include "driver/gpio.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_ili9341.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2s_std.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "nvs_flash.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"

#define DISPLAY_HEIGHT      240
#define RENDER_HEIGHT       240
#define RENDER_PADDING      0

// battery
#define BATT_ADC_UNIT        ADC_UNIT_2
#define BATT_ADC_CHANNEL     ADC_CHANNEL_5
#define BATT_ADC_ATTEN       ADC_ATTEN_DB_12
#define BATT_ADC_WIDTH       ADC_BITWIDTH_DEFAULT

// buttons
#define BUTTON_GAME_A       GPIO_NUM_14
#define BUTTON_GAME_B       GPIO_NUM_5
#define BUTTON_TIME         GPIO_NUM_3
#define BUTTON_LEFT         GPIO_NUM_2
#define BUTTON_RIGHT        GPIO_NUM_1
#define BUTTON_ALARM        GPIO_NUM_7
#define BUTTON_ACL          GPIO_NUM_8
#define BUTTON_LEFT_2       GPIO_NUM_17
#define BUTTON_RIGHT_2      GPIO_NUM_18

// LCD
#define LCD_PIXEL_CLOCK_HZ  (60 * 1000 * 1000)
#define LCD_CMD_BITS        8
#define LCD_PARAM_BITS      8
#define LCD_HOST            SPI2_HOST
#define LCD_SCLK            12
#define LCD_MOSI            11
#define LCD_MISO            -1
#define LCD_DC              9
#define LCD_RST             13

// audio
#define AUD_I2S_BCK         10
#define AUD_I2S_WS          4
#define AUD_I2S_DATA        6

uint16_t volume = 2000;
uint8_t volume_level = 2;
static uint32_t volume_time = 0;
uint16_t volume_display_count = 0;

uint8_t battery_level = 0;
uint16_t battery_display_count = 0;

uint8_t start_delay = 127;

uint8_t control_button_mode = 0;

unsigned char *ROM_DATA;
unsigned int ROM_DATA_LENGTH;

volatile bool lcd_transfer_in_progress = false;

static bool on_color_trans_done(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *event_data, void *user_ctx) {
    lcd_transfer_in_progress = false;
    return false;
}

void read_volume() {
    nvs_handle_t my_handle;
    ESP_ERROR_CHECK(nvs_open("gandw", NVS_READWRITE, &my_handle));
    nvs_get_u8(my_handle, "volume_level", &volume_level);
    nvs_close(my_handle);

    if (volume_level > 10) {
        volume_level = 4;
    }

    volume = 10000 * (volume_level / 10.0);
}

void save_volume() {
    nvs_handle_t my_handle;
    ESP_ERROR_CHECK(nvs_open("gandw", NVS_READWRITE, &my_handle));
    ESP_ERROR_CHECK(nvs_set_u8(my_handle, "volume_level", volume_level));
    ESP_ERROR_CHECK(nvs_commit(my_handle));
    nvs_close(my_handle);
}

void set_volume(bool is_up) {

    uint32_t current_time = esp_timer_get_time() / 1000;

    if (current_time - volume_time > 200) {
        if (is_up) {
            if (volume_level < 10) {
                volume_level++;
            }
        }
        else {
            if (volume_level > 0) {
                volume_level--;
            }
        }

        volume = 10000 * (volume_level / 10.0);
        volume_time = current_time;
        volume_display_count = 255;
        save_volume();

    }
}


unsigned int gw_get_default_buttons() {
	
	uint32_t hw_buttons = 0;
	
	if (gpio_get_level(BUTTON_RIGHT) == 0) { 
		hw_buttons |= GW_BUTTON_RIGHT + GW_BUTTON_DOWN;
	}
	else if (gpio_get_level(BUTTON_LEFT) == 0) {
			hw_buttons |= GW_BUTTON_LEFT + GW_BUTTON_DOWN;
	}
	else if (gpio_get_level(BUTTON_RIGHT_2) == 0) { 
		hw_buttons |= GW_BUTTON_RIGHT + GW_BUTTON_UP;
	}
	else if (gpio_get_level(BUTTON_LEFT_2) == 0) {
			hw_buttons |= GW_BUTTON_LEFT + GW_BUTTON_UP;
	}
	
	return hw_buttons;
        
}

unsigned int gw_get_snoopytennis_buttons() {
	
	uint32_t hw_buttons = 0;
	
	if (gpio_get_level(BUTTON_RIGHT) == 0) { 
		hw_buttons |= GW_BUTTON_DOWN;
	}
	else if (gpio_get_level(BUTTON_LEFT) == 0) {
			hw_buttons |= GW_BUTTON_A;
	}
	else if (gpio_get_level(BUTTON_RIGHT_2) == 0) { 
		hw_buttons |= GW_BUTTON_UP;
	}
	else if (gpio_get_level(BUTTON_LEFT_2) == 0) {
			hw_buttons |= GW_BUTTON_A;
	}
	
	return hw_buttons;
        
}

unsigned int gw_get_marioscementfactory_buttons() {
	
	uint32_t hw_buttons = 0;
	
	if (gpio_get_level(BUTTON_RIGHT) == 0) { 
		hw_buttons |= GW_BUTTON_RIGHT;
	}
	else if (gpio_get_level(BUTTON_LEFT) == 0) {
			hw_buttons |= GW_BUTTON_LEFT;
	}
	else if (gpio_get_level(BUTTON_RIGHT_2) == 0) { 
		hw_buttons |= GW_BUTTON_A;
	}
	else if (gpio_get_level(BUTTON_LEFT_2) == 0) {
			hw_buttons |= GW_BUTTON_A;
	}
	
	return hw_buttons;
        
}

unsigned int gw_get_flagman_lion_buttons() {
	
	uint32_t hw_buttons = 0;
	
	if (gpio_get_level(BUTTON_RIGHT) == 0) { 
		hw_buttons |= GW_BUTTON_B;
	}
	else if (gpio_get_level(BUTTON_LEFT) == 0) {
			hw_buttons |= GW_BUTTON_DOWN;
	}
	else if (gpio_get_level(BUTTON_RIGHT_2) == 0) { 
		hw_buttons |= GW_BUTTON_A;
	}
	else if (gpio_get_level(BUTTON_LEFT_2) == 0) {
			hw_buttons |= GW_BUTTON_UP;
	}
	
	return hw_buttons;
        
}

unsigned int gw_get_buttons()
{
    uint32_t hw_buttons = 0;

    if (start_delay == 0) {

        if (gpio_get_level(BUTTON_TIME) == 0) { 

            if (gpio_get_level(BUTTON_RIGHT) == 0) { 
                set_volume(true); // volume up
            }
            else if (gpio_get_level(BUTTON_LEFT) == 0) {
                set_volume(false); // volume down
            }
            else {
                hw_buttons |= GW_BUTTON_B + GW_BUTTON_TIME;
            }

        }
        else if (gpio_get_level(BUTTON_GAME_A) == 0) { 
            hw_buttons |= GW_BUTTON_GAME;
        }
        else if (gpio_get_level(BUTTON_GAME_B) == 0) {
            hw_buttons |= GW_BUTTON_TIME;
        }
        else if (gpio_get_level(BUTTON_ALARM) == 0) {
            hw_buttons |= GW_BUTTON_B + GW_BUTTON_GAME;
        }
        else if (gpio_get_level(BUTTON_ACL) == 0) {
            gw_system_reset();
        }
        else {
			if (control_button_mode == 1) {
				hw_buttons = gw_get_snoopytennis_buttons();
			}
			else if (control_button_mode == 2) {
				hw_buttons = gw_get_marioscementfactory_buttons();
			}
			else if (control_button_mode == 3) {
				hw_buttons = gw_get_flagman_lion_buttons();
			}
			else {
				hw_buttons = gw_get_default_buttons();
			}
		}

    }

    return hw_buttons;
}



i2s_chan_handle_t setup_audio_i2s() {

    i2s_chan_handle_t i2s_audio_handle;
    
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);

    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &i2s_audio_handle, NULL));

    i2s_std_config_t i2s_config = {
        .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(GW_SYS_FREQ),
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .mclk = GPIO_NUM_NC,
            .bclk = AUD_I2S_BCK,    // Max98357 CLK
            .ws = AUD_I2S_WS,       // Max98357 LRC
            .dout = AUD_I2S_DATA,   // Max98357 DIN
            .din = GPIO_NUM_NC,  
        }
    };

    ESP_ERROR_CHECK(i2s_channel_init_std_mode(i2s_audio_handle, &i2s_config));
    ESP_ERROR_CHECK(i2s_channel_enable(i2s_audio_handle));

    return i2s_audio_handle;

}

esp_lcd_panel_handle_t setup_lcd_spi() {

    esp_lcd_panel_handle_t spi_lcd_handle = NULL;
    esp_lcd_panel_io_handle_t io_handle = NULL;

    spi_bus_config_t buscfg = {
        .sclk_io_num = LCD_SCLK,
        .mosi_io_num = LCD_MOSI,
        .miso_io_num = LCD_MISO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = GW_SCREEN_WIDTH * 80 * sizeof(uint16_t),
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = LCD_DC,
        .cs_gpio_num = -1,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10,
        .on_color_trans_done = on_color_trans_done
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
        .bits_per_pixel = 16,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_ili9341(io_handle, &panel_config, &spi_lcd_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(spi_lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(spi_lcd_handle));
    esp_lcd_panel_swap_xy(spi_lcd_handle, true);
    esp_lcd_panel_mirror(spi_lcd_handle, true, true);
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(spi_lcd_handle, true));

    return spi_lcd_handle;
}

void setup_buttons() {

    esp_rom_gpio_pad_select_gpio(BUTTON_TIME);
    gpio_set_direction(BUTTON_TIME, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_TIME, GPIO_PULLUP_ONLY);

    esp_rom_gpio_pad_select_gpio(BUTTON_GAME_A);
    gpio_set_direction(BUTTON_GAME_A, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GAME_A, GPIO_PULLUP_ONLY);

    esp_rom_gpio_pad_select_gpio(BUTTON_GAME_B);
    gpio_set_direction(BUTTON_GAME_B, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GAME_B, GPIO_PULLUP_ONLY);

    esp_rom_gpio_pad_select_gpio(BUTTON_ALARM);
    gpio_set_direction(BUTTON_ALARM, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_ALARM, GPIO_PULLUP_ONLY);

    esp_rom_gpio_pad_select_gpio(BUTTON_ACL);
    gpio_set_direction(BUTTON_ACL, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_ACL, GPIO_PULLUP_ONLY);

    esp_rom_gpio_pad_select_gpio(BUTTON_LEFT);
    gpio_set_direction(BUTTON_LEFT, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_LEFT, GPIO_PULLUP_ONLY);

    esp_rom_gpio_pad_select_gpio(BUTTON_RIGHT);
    gpio_set_direction(BUTTON_RIGHT, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_RIGHT, GPIO_PULLUP_ONLY);
    
    esp_rom_gpio_pad_select_gpio(BUTTON_LEFT_2);
    gpio_set_direction(BUTTON_LEFT_2, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_LEFT_2, GPIO_PULLUP_ONLY);

    esp_rom_gpio_pad_select_gpio(BUTTON_RIGHT_2);
    gpio_set_direction(BUTTON_RIGHT_2, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_RIGHT_2, GPIO_PULLUP_ONLY);

}

uint8_t read_selected_game() {
    uint8_t selected_game = 1; // gnwegg
    nvs_handle_t my_handle;
    ESP_ERROR_CHECK(nvs_open("gandw", NVS_READWRITE, &my_handle));
    nvs_get_u8(my_handle, "selected_game", &selected_game);
    nvs_close(my_handle);

    if (selected_game < 1 || selected_game > 10) {
        selected_game = 1;
    }

    return selected_game;
}

void save_selected_game(uint8_t selected_game) {
    nvs_handle_t my_handle;
    ESP_ERROR_CHECK(nvs_open("gandw", NVS_READWRITE, &my_handle));
    ESP_ERROR_CHECK(nvs_set_u8(my_handle, "selected_game", selected_game));
    ESP_ERROR_CHECK(nvs_commit(my_handle));
    nvs_close(my_handle);
}

void load_game() {

    uint8_t gs1 = gpio_get_level(BUTTON_GAME_A);
    uint8_t gs2 = gpio_get_level(BUTTON_GAME_B);
    uint8_t gs3 = gpio_get_level(BUTTON_TIME);
    uint8_t gs4 = gpio_get_level(BUTTON_LEFT);
    uint8_t gs5 = gpio_get_level(BUTTON_RIGHT);

	extern const uint8_t gnwegg_gw_start[] asm("_binary_gnwegg_gw_start");
    extern const uint8_t gnwegg_gw_end[] asm("_binary_gnwegg_gw_end");
	extern const uint8_t gnwfireattack_gw_start[] asm("_binary_gnwfireattack_gw_start");
    extern const uint8_t gnwfireattack_gw_end[] asm("_binary_gnwfireattack_gw_end");
    extern const uint8_t gnwflagman_gw_start[] asm("_binary_gnwflagman_gw_start");
    extern const uint8_t gnwflagman_gw_end[] asm("_binary_gnwflagman_gw_end");
    extern const uint8_t gnwjudge_gw_start[] asm("_binary_gnwjudge_gw_start");
    extern const uint8_t gnwjudge_gw_end[] asm("_binary_gnwjudge_gw_end");
    extern const uint8_t gnwlion_gw_start[] asm("_binary_gnwlion_gw_start");
    extern const uint8_t gnwlion_gw_end[] asm("_binary_gnwlion_gw_end");
    extern const uint8_t gnwmanhole_gw_start[] asm("_binary_gnwmanhole_gw_start");
    extern const uint8_t gnwmanhole_gw_end[] asm("_binary_gnwmanhole_gw_end");
    extern const uint8_t gnwmanholegold_gw_start[] asm("_binary_gnwmanholegold_gw_start");
    extern const uint8_t gnwmanholegold_gw_end[] asm("_binary_gnwmanholegold_gw_end");
    extern const uint8_t gnwmarioscementfactory_gw_start[] asm("_binary_gnwmarioscementfactory_gw_start");
    extern const uint8_t gnwmarioscementfactory_gw_end[] asm("_binary_gnwmarioscementfactory_gw_end");
    extern const uint8_t gnwmickeymouse_gw_start[] asm("_binary_gnwmickeymouse_gw_start");
    extern const uint8_t gnwmickeymouse_gw_end[] asm("_binary_gnwmickeymouse_gw_end");
    extern const uint8_t gnwsnoopytennis_gw_start[] asm("_binary_gnwsnoopytennis_gw_start");
    extern const uint8_t gnwsnoopytennis_gw_end[] asm("_binary_gnwsnoopytennis_gw_end");

    uint8_t selected_game = 0;

	if (gs1 == 0 && gs2 == 1 && gs3 == 1) { // 100
        selected_game = 1; // gnwegg
    }
    else if (gs1 == 1 && gs2 == 0 && gs3 == 1) { // 010
        selected_game = 2; // gnwfireattack
    }
    else if (gs1 == 1 && gs2 == 1 && gs3 == 0) { // 001
        selected_game = 3; // gnwflagman
    }
    else if (gs1 == 0 && gs2 == 0 && gs3 == 1) { // 110
        selected_game = 4; // gnwjudge
    }
    else if (gs1 == 1 && gs2 == 0 && gs3 == 0) { // 011
        selected_game = 5; // gnwlion
    }
    else if (gs1 == 0 && gs2 == 1 && gs3 == 0) { // 101
        selected_game = 6; // gnwmanhole
    }
    else if (gs1 == 0 && gs2 == 0 && gs3 == 0) { // 111
        selected_game = 7; // gnwmanholegold
    }
    else if (gs4 == 0 && gs5 == 1) { // left
        selected_game = 8; // gnwmarioscementfactory
    }
    else if (gs4 == 1 && gs5 == 0) { // right
        selected_game = 9; // gnwmickeymouse
    }
    else if (gs4 == 0 && gs5 == 0) { // left and right
        selected_game = 10; // gnwsnoopytennis
    }


    if (selected_game == 0) {
        selected_game = read_selected_game();
        // no game selected, no start_delay
        start_delay = 0;
    }

    save_selected_game(selected_game);
    
    control_button_mode = 0;

    if (selected_game == 1) {
        ROM_DATA = (unsigned char *)gnwegg_gw_start;
        ROM_DATA_LENGTH = gnwegg_gw_end - gnwegg_gw_start;
    }
    else if (selected_game == 2) { 
        ROM_DATA = (unsigned char *)gnwfireattack_gw_start;
        ROM_DATA_LENGTH = gnwfireattack_gw_end - gnwfireattack_gw_start;
    }
    else if (selected_game == 3) {
        ROM_DATA = (unsigned char *)gnwflagman_gw_start;
        ROM_DATA_LENGTH = gnwflagman_gw_end - gnwflagman_gw_start;
        control_button_mode = 3;
    }
    else if (selected_game == 4) {
        ROM_DATA = (unsigned char *)gnwjudge_gw_start;
        ROM_DATA_LENGTH = gnwjudge_gw_end - gnwjudge_gw_start;
    }
    else if (selected_game == 5) {
        ROM_DATA = (unsigned char *)gnwlion_gw_start;
        ROM_DATA_LENGTH = gnwlion_gw_end - gnwlion_gw_start;
        control_button_mode = 3;
    }
    else if (selected_game == 6) {
        ROM_DATA = (unsigned char *)gnwmanhole_gw_start;
        ROM_DATA_LENGTH = gnwmanhole_gw_end - gnwmanhole_gw_start;
    }
    else if (selected_game == 7) {
        ROM_DATA = (unsigned char *)gnwmanholegold_gw_start;
        ROM_DATA_LENGTH = gnwmanholegold_gw_end - gnwmanholegold_gw_start;
    }
    else if (selected_game == 8) {
        ROM_DATA = (unsigned char *)gnwmarioscementfactory_gw_start;
        ROM_DATA_LENGTH = gnwmarioscementfactory_gw_end - gnwmarioscementfactory_gw_start;
        control_button_mode = 2;
    }
    else if (selected_game == 9) {
        ROM_DATA = (unsigned char *)gnwmickeymouse_gw_start;
        ROM_DATA_LENGTH = gnwmickeymouse_gw_end - gnwmickeymouse_gw_start;
    }
    else { // 10
        ROM_DATA = (unsigned char *)gnwsnoopytennis_gw_start;
        ROM_DATA_LENGTH = gnwsnoopytennis_gw_end - gnwsnoopytennis_gw_start;
        control_button_mode = 1;
    }
}

void display_battery(unsigned short *framebuffer)
{
    uint8_t seg_count = 0;
    bool show_segment = false;
    uint8_t seg_number = 0;
    uint16_t pixel_colour = 0;
    uint16_t index = 0;
    uint16_t level_colour = 0xe007; // green: 0x07e0

    if (battery_level == 2 || battery_level == 3) {
        level_colour = 0xe0ff; // yellow: 0xffe0
    }
    else if (battery_level == 1) {
        level_colour = 0x00f8; // yellow: 0xf800
    }

    uint16_t batteryIcon_8x20[] = {
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
        0xffff,0xffff,0xffff,0x0000,0x0000,0xffff,0xffff,0xffff,
        0xffff,0xffff,0xffff,0x0000,0x0000,0xffff,0xffff,0xffff,
        0xffff,0x7294,0x0000,0x0000,0x0000,0x0000,0x7294,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x7294,0x0000,0x0000,0x0000,0x0000,0x7294,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff
    };


    uint16_t x_end = GW_SCREEN_WIDTH - 2 - 35;
    uint16_t x_start = x_end - 8;

    uint16_t pixel_pos = 0;
    for (uint8_t y = 3; y < 23; y++) {
        for (uint16_t x = x_start; x < x_end; x++) {
            index = y * GW_SCREEN_WIDTH + x;
            framebuffer[index] = batteryIcon_8x20[pixel_pos];

            pixel_pos++;
        }
    }

    x_end = GW_SCREEN_WIDTH - 2;
    x_start = x_end - 35;

    for (uint8_t y = 3; y < 23; y++) {
        seg_count = 0;
        seg_number = 0;
        for (uint16_t x = x_start; x < x_end; x++) {
            index = y * GW_SCREEN_WIDTH + x;
            seg_count++;
            if ((seg_count == 5 && show_segment) || (seg_count == 2 && !show_segment)) {
                seg_count = 0;
                show_segment = !show_segment;

                if (show_segment) {
                    seg_number++;
                }
            }

            pixel_colour = 0x9ad6;

            if ((y > 3 && y < 22) && show_segment) {
            
                if (seg_count == 0 || seg_count == 4 || y == 4 || y == 21) {
                    pixel_colour = 0x2c63;
                }
                else {
                
                    if (battery_level >= seg_number) {
                        pixel_colour = level_colour;
                    }
                    else {
                        pixel_colour = 0x518c;
                    }

                }
            }
           
            framebuffer[index] = pixel_colour;

        }
    }
}

uint16_t get_volume_segment_colour(uint8_t seg_number) {

    if (seg_number > 3 && seg_number < 8) { //4, 5, 6, 7
        return 0xe0ff; // yellow: 0xffe0
    }
    else if (seg_number > 7) { // 8, 9, 10
        return 0x00f8; // yellow: 0xf800
    }
    else {
        return 0xe007; // green: 0x07e0
    }

}

void display_volume(unsigned short *framebuffer)
{
    uint8_t seg_count = 0;
    bool show_segment = false;
    uint8_t seg_number = 0;
    uint16_t pixel_colour = 0;
    uint16_t index = 0;

    uint16_t speakerIcon_13x20[] = {
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x7294,0x0000,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x7294,0x0000,0x0000,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x7294,0x0000,0x0000,0x0000,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x7294,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x7294,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0x7294,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x7294,0x0000,0x0000,0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x7294,0x0000,0x0000,0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0x7294,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x7294,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x7294,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x7294,0x0000,0x0000,0x0000,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x7294,0x0000,0x0000,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x7294,0x0000,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff
    };

    uint16_t pixel_pos = 0;
    for (uint8_t y = 3; y < 23; y++) {
        for (uint8_t x = 2; x < 15; x++) {
            index = y * GW_SCREEN_WIDTH + x;
            framebuffer[index] = speakerIcon_13x20[pixel_pos];

            pixel_pos++;
        }
    }

    for (uint8_t y = 3; y < 23; y++) {
        seg_count = 0;
        seg_number = 0;
        for (uint8_t x = 15; x < 85; x++) {
            index = y * GW_SCREEN_WIDTH + x;
            seg_count++;
            if ((seg_count == 5 && show_segment) || (seg_count == 2 && !show_segment)) {
                seg_count = 0;
                show_segment = !show_segment;

                if (show_segment) {
                    seg_number++;
                }
            }

            pixel_colour = 0x9ad6;

            if ((y > 3 && y < 22) && show_segment) {

                if (seg_count == 0 || seg_count == 4 || y == 4 || y == 21) {
                    pixel_colour = 0x2c63;
                }
                else {
                
                    if (volume_level >= seg_number) {
                        pixel_colour = get_volume_segment_colour(seg_number);
                    }
                    else {
                        pixel_colour = 0x518c;
                    }

                }
            }

            framebuffer[index] = pixel_colour;

        }
    }
}

void display_volume_battery(unsigned short *framebuffer)
{

    if (volume_display_count > 0) {
        display_volume(framebuffer);
    }

    if (battery_display_count > 0) {
        display_battery(framebuffer);
    }

}

void get_battery_level() {

    esp_err_t ret;
    
    // --- ADC ONESHOT CONFIGURATION ---
    adc_oneshot_unit_handle_t adc_handle;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = BATT_ADC_UNIT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ret = adc_oneshot_new_unit(&init_config, &adc_handle);
    if (ret != ESP_OK) {
        return;
    }

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = BATT_ADC_WIDTH,
        .atten = BATT_ADC_ATTEN,
    };
    ret = adc_oneshot_config_channel(adc_handle, BATT_ADC_CHANNEL, &config);
    if (ret != ESP_OK) {
        adc_oneshot_del_unit(adc_handle);
        return;
    }

    // --- ADC CALIBRATION ---
    adc_cali_handle_t adc_cali_handle = NULL;
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = BATT_ADC_UNIT,
        .atten = BATT_ADC_ATTEN,
        .bitwidth = BATT_ADC_WIDTH,
    };
    ret = adc_cali_create_scheme_curve_fitting(&cali_config, &adc_cali_handle);
    if (ret != ESP_OK) {
        adc_oneshot_del_unit(adc_handle);
        return;
    }

    int total_adc_reading = 0;
    for (int i = 0; i < 10; i++) {
        int raw_adc_reading = 0;
        ret = adc_oneshot_read(adc_handle, BATT_ADC_CHANNEL, &raw_adc_reading);
        if (ret != ESP_OK) {
            adc_cali_delete_scheme_curve_fitting(adc_cali_handle);
            adc_oneshot_del_unit(adc_handle);
            return;
        }
        total_adc_reading += raw_adc_reading;
    }

    int averaged_adc_reading = total_adc_reading / 10.0f;

    // Convert averaged ADC reading to voltage (mV)
    int voltage_at_adc_pin_mv = 0;
    ret = adc_cali_raw_to_voltage(adc_cali_handle, averaged_adc_reading, &voltage_at_adc_pin_mv);
    if (ret != ESP_OK) {
        adc_cali_delete_scheme_curve_fitting(adc_cali_handle);
        adc_oneshot_del_unit(adc_handle);
        return;
    }

    // (R1 + R2) / R2: (4.7 + 10) / 10 = 1.47 or 0.00147
    // max: 4.2v, min: 3.0v

    //testing    
    //voltage_at_adc_pin_mv = 2800;
    
    float battery_voltage_level = ((float)voltage_at_adc_pin_mv * 0.00147) - 3.0f;
    float battery_percentage = 0.0f;

    if (battery_voltage_level >= 1.2f) {
        battery_percentage = 100.0f;
    }
    else if (battery_voltage_level <= 0.0f) {
        battery_percentage = 0.0f;
    }
    else {
        battery_percentage = (battery_voltage_level / 1.2f) * 100.0f;
    }

    if (battery_percentage > 100.0f) battery_percentage = 100.0f;
    if (battery_percentage < 0.0f) battery_percentage = 0.0f;

    if (battery_percentage >= 80.0f) {
        battery_level = 5;
    }
    else if (battery_percentage >= 60.0f) {
        battery_level = 4;
    }
    else if (battery_percentage >= 40.0f) {
        battery_level = 3;
    }
    else if (battery_percentage >= 20.0f) {
        battery_level = 2;
    }
    else if (battery_percentage >= 5.0f) {
        battery_level = 1;
    }
    else {
        battery_level = 0;
    }

    battery_display_count = 255;

    // --- Cleanup ---
    adc_cali_delete_scheme_curve_fitting(adc_cali_handle);
    adc_oneshot_del_unit(adc_handle);

}

void app_main(void)
{

    // memory for sound and screen

    uint16_t *framebuffer = (uint16_t *)heap_caps_malloc(GW_SCREEN_WIDTH * GW_SCREEN_HEIGHT * sizeof(uint16_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);

    uint16_t audio_buffer[GW_AUDIO_BUFFER_LENGTH];

    // sound
    i2s_chan_handle_t i2s_audio_handle = setup_audio_i2s();

    // screen
    esp_lcd_panel_handle_t spi_lcd_handle = setup_lcd_spi();


    // buttons
    setup_buttons();


    // flash
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        printf("NVS Error\n");
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    read_volume();

    get_battery_level();

    // Load ROM
    load_game();

    gw_system_romload();

    gw_system_sound_init();
    gw_system_config();
    gw_system_start();
    gw_system_reset();
    
    
    while (true) {

        if (volume_display_count > 0) {
            volume_display_count--;
        }

        if (battery_display_count > 0) {
            battery_display_count--;
        }

        if (start_delay > 0) {
            start_delay--;
        }

        gw_system_run(GW_SYSTEM_CYCLES);

        // lcd
        if (!lcd_transfer_in_progress) {

            lcd_transfer_in_progress = true;

            gw_system_blit(framebuffer);

            if (volume_display_count > 0 || battery_display_count > 0) {
                display_volume_battery(framebuffer);
            }

            esp_lcd_panel_draw_bitmap(spi_lcd_handle, 0, RENDER_PADDING, GW_SCREEN_WIDTH, RENDER_HEIGHT + RENDER_PADDING, framebuffer);
        }

        // audio
        for (size_t i = 0; i < GW_AUDIO_BUFFER_LENGTH; i++)
        {
            audio_buffer[i] = (gw_audio_buffer[i] > 0) ? volume : 0;
        }

        size_t bytes_written;

        i2s_channel_write(i2s_audio_handle, audio_buffer, sizeof(audio_buffer), &bytes_written, portMAX_DELAY);

        gw_audio_buffer_copied = true;

    }

}


