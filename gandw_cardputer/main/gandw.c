#include <gw_system.h>
#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "nvs_flash.h"
#include "button.h"
#include "keyboard.h"
#include "game.h"
#include "volume.h"
#include "battery.h"
#include "menu.h"


// LCD
#define LCD_PIXEL_CLOCK_HZ  (80 * 1000 * 1000)
#define LCD_CMD_BITS        8
#define LCD_PARAM_BITS      8
#define LCD_HOST       		SPI2_HOST
#define LCD_MOSI   			35
#define LCD_SCLK    		36
#define LCD_CS     			37
#define LCD_DC     			34
#define LCD_RST    			33
#define LCD_BLK     		38
#define LCD_MISO    		-1

// audio
#define AUD_I2S_BCK         41
#define AUD_I2S_WS          43
#define AUD_I2S_DATA        42

i2s_chan_handle_t i2s_audio_handle;
volatile bool lcd_transfer_in_progress = false;

static bool on_color_trans_done(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *event_data, void *user_ctx) {
    lcd_transfer_in_progress = false;
    return false;
}

unsigned int gw_get_buttons()
{
    uint32_t hw_buttons = 0;
    
    char key = keyboard_get_key();
    
    if (key == ';') { 
		volume_set(true); // volume up
	}
	else if (key == '.') { 
		volume_set(false); // volume down
	}
	else {
    
		if (button_control_type == 4) {
			hw_buttons = button_get_four_buttons(key);
		}
		else if (button_control_type == 5) {
			hw_buttons = button_get_five_buttons(key);
		}
		else {
			hw_buttons = button_get_two_buttons(key);
		}
		
	}

    return hw_buttons;
}

i2s_chan_handle_t setup_audio_i2s() {

    i2s_chan_handle_t new_i2s_audio_handle;
    
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);

    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &new_i2s_audio_handle, NULL));

    i2s_std_config_t i2s_config = {
        .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(GW_SYS_FREQ),
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .mclk = GPIO_NUM_NC,
            .bclk = AUD_I2S_BCK,    // CLK
            .ws = AUD_I2S_WS,       // LRC
            .dout = AUD_I2S_DATA,   // DIN
            .din = GPIO_NUM_NC,  
        }
    };

    ESP_ERROR_CHECK(i2s_channel_init_std_mode(new_i2s_audio_handle, &i2s_config));
    ESP_ERROR_CHECK(i2s_channel_enable(new_i2s_audio_handle));

    return new_i2s_audio_handle;

}

esp_lcd_panel_handle_t setup_lcd_spi() {

	esp_lcd_panel_handle_t spi_lcd_handle = NULL;
    esp_lcd_panel_io_handle_t io_handle = NULL;

    gpio_config_t bk_gpio_config = {
        .pin_bit_mask = 1ULL << LCD_BLK,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    gpio_set_level(LCD_BLK, 1);

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
        .cs_gpio_num = LCD_CS,
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
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,
    };

    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &spi_lcd_handle));

	esp_lcd_panel_set_gap(spi_lcd_handle, 40, 53);

    ESP_ERROR_CHECK(esp_lcd_panel_reset(spi_lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(spi_lcd_handle));
    esp_lcd_panel_swap_xy(spi_lcd_handle, true);
	esp_lcd_panel_mirror(spi_lcd_handle, true, false);
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(spi_lcd_handle, true));
    esp_lcd_panel_invert_color(spi_lcd_handle, true);

    return spi_lcd_handle;    
}

uint8_t read_selected_game() {
    uint8_t selected_game = 0;
    nvs_handle_t my_handle;
    ESP_ERROR_CHECK(nvs_open("gandw", NVS_READWRITE, &my_handle));
    nvs_get_u8(my_handle, "selected_game", &selected_game);
    nvs_close(my_handle);

    if (selected_game > game_count - 1) {
        selected_game = 0;
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

void set_menu(bool is_up) {

    uint32_t current_time = esp_timer_get_time() / 1000;

    if (current_time - menu_time > 200) {
        if (is_up) {
            if (menu_index < game_count - 1) {
                menu_index++;
            }
        }
        else {
            if (menu_index > 0) {
                menu_index--;
            }
        }

        menu_time = current_time;
		menu_update = true;
    }
}

void check_menu_buttons()
{
	char key = keyboard_get_key();
	uint8_t menu_button = button_get_menu_buttons(key);
    if (menu_button == 1) { 
        set_menu(true);
    }
    else if (menu_button == 2) {
        set_menu(false);
    }
    else if (menu_button == 3) {
		save_selected_game(menu_index);
        menu_show = false;
    }
}

void display_volume_battery(unsigned short *framebuffer)
{

    if (volume_display_count > 0) {
        volume_display(framebuffer);
    }

    if (battery_display_count > 0) {
        battery_display(framebuffer);
    }

}

void app_main(void)
{

    // memory for sound and screen

    uint16_t *framebuffer = (uint16_t *)heap_caps_malloc(GW_SCREEN_WIDTH * GW_SCREEN_HEIGHT * sizeof(uint16_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);

    uint16_t audio_buffer[GW_AUDIO_BUFFER_LENGTH];

    // sound
    i2s_audio_handle = setup_audio_i2s();

    // screen
    esp_lcd_panel_handle_t spi_lcd_handle = setup_lcd_spi();


    // keyboard
    keyboard_setup();


    // flash
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        printf("NVS Error\n");
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }
    
    
    volume_read();

    battery_get_level();
    
	menu_index = read_selected_game();

	menu_update = true;
	
	while (menu_show) {
		
		check_menu_buttons();
		
		if (volume_display_count > 0) {
			volume_display_count--;
			if (volume_display_count == 0) {
				menu_update = true;
			}
		}

		if (battery_display_count > 0) {
			battery_display_count--;
			if (battery_display_count == 0) {
				menu_update = true;
			}
		}
		
		if (button_start_delay > 0) {
			button_start_delay--;
		}
			
		if (menu_update && !lcd_transfer_in_progress) {
			
			menu_display_item(framebuffer, menu_index);
			
			if (volume_display_count > 0 || battery_display_count > 0) {
				display_volume_battery(framebuffer);
			}
	
			esp_lcd_panel_draw_bitmap(spi_lcd_handle, 0, 0, GW_SCREEN_WIDTH, GW_SCREEN_HEIGHT, framebuffer);
			
			menu_update = false;
			
		}
		
		vTaskDelay(pdMS_TO_TICKS(10)); 
		
	}
	
	game_load(menu_index);

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
		
		if (button_start_delay > 0) {
			button_start_delay--;
		}

		gw_system_run(GW_SYSTEM_CYCLES);

		// lcd
		if (!lcd_transfer_in_progress) {

			lcd_transfer_in_progress = true;

			gw_system_blit(framebuffer);

			if (volume_display_count > 0 || battery_display_count > 0) {
				display_volume_battery(framebuffer);
			}

			esp_lcd_panel_draw_bitmap(spi_lcd_handle, 0, 0, GW_SCREEN_WIDTH, GW_SCREEN_HEIGHT, framebuffer);
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


