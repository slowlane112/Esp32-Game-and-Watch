#include <stdint.h>
#include <gw_system.h>
#include "driver/gpio.h"
#include "button.h"
#include "volume.h"
#include "menu.h"
#include "driver/i2s_std.h"
#include "esp_timer.h"
#include "esp_system.h"

// buttons
#define BUTTON_GAME_A       GPIO_NUM_14
#define BUTTON_GAME_B       GPIO_NUM_5
#define BUTTON_TIME         GPIO_NUM_3
#define BUTTON_LEFT         GPIO_NUM_2
#define BUTTON_RIGHT        GPIO_NUM_1
#define BUTTON_ALARM        GPIO_NUM_7
#define BUTTON_ACL          GPIO_NUM_8

uint8_t button_control_type = 2;
uint8_t button_control_mode = 0;
uint8_t button_start_delay = 63;
int64_t button_reset_timer = 0;
extern i2s_chan_handle_t i2s_audio_handle;

void button_setup() {

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
    
}

uint8_t button_get_menu_buttons()
{
	
	if (button_start_delay == 0) {
	
		if (gpio_get_level(BUTTON_TIME) == 0) { 

			if (gpio_get_level(BUTTON_RIGHT) == 0) { 
				volume_set(true); // volume up
				volume_menu_tone(i2s_audio_handle, volume_level);
				menu_update = true;
				button_start_delay = 7;
			}
			else if (gpio_get_level(BUTTON_LEFT) == 0) {
				volume_set(false); // volume down
				volume_menu_tone(i2s_audio_handle, volume_level);
				menu_update = true;
				button_start_delay = 7;
			}

		}
		else if (gpio_get_level(BUTTON_RIGHT) == 0 && gpio_get_level(BUTTON_LEFT) == 0) {
			button_start_delay = 31;
			return 0;
		}
		else if (gpio_get_level(BUTTON_RIGHT) == 0) { 
			button_start_delay = 7;
			return 1;
		}
		else if (gpio_get_level(BUTTON_LEFT) == 0) {
			button_start_delay = 7;
			return 2;
		}
		else if (gpio_get_level(BUTTON_GAME_A) == 0) { 
			return 3;
		}
		
	}
		
    return 0;
}

unsigned int button_get_two_default_buttons() 
{
	
	uint32_t hw_buttons = 0;
	
	if (gpio_get_level(BUTTON_GAME_A) == 0) { 
        hw_buttons |= GW_BUTTON_GAME;
    }
    else if (gpio_get_level(BUTTON_GAME_B) == 0) {
        hw_buttons |= GW_BUTTON_TIME;
    }
	
	return hw_buttons;
        
}

unsigned int button_get_two_tropicalfish_buttons() 
{
	
	uint32_t hw_buttons = 0;
	
	if (gpio_get_level(BUTTON_GAME_A) == 0) { 
        hw_buttons |= GW_BUTTON_TIME;
    }
    else if (gpio_get_level(BUTTON_GAME_B) == 0) {
        hw_buttons |= GW_BUTTON_GAME;
    }
	
	return hw_buttons;
        
}

unsigned int button_get_two_buttons()
{
	
    uint32_t hw_buttons = 0;
    
    if (button_start_delay == 0) {
		
		if (gpio_get_level(BUTTON_RIGHT) == 0 && gpio_get_level(BUTTON_LEFT) == 0) { 
		
			if (button_reset_timer == 0) {
				button_reset_timer = esp_timer_get_time();
			}
			else {
				if (esp_timer_get_time() - button_reset_timer > 2000000) {
					menu_show = true;
				}
			}
			
			return 0;
		}
		else {
			button_reset_timer = 0;
		}

		if (gpio_get_level(BUTTON_TIME) == 0) {
			
			if (gpio_get_level(BUTTON_RIGHT) == 0) { 
				volume_set(true); // volume up
			}
			else if (gpio_get_level(BUTTON_LEFT) == 0) {
				volume_set(false); // volume down
			}
			else {
				hw_buttons |= GW_BUTTON_B + GW_BUTTON_TIME;
			}
			
		}
		else if (gpio_get_level(BUTTON_RIGHT) == 0) {
			hw_buttons |= GW_BUTTON_RIGHT;
		}
		else if (gpio_get_level(BUTTON_LEFT) == 0) {
				hw_buttons |= GW_BUTTON_LEFT;
		}
		else if (gpio_get_level(BUTTON_ALARM) == 0) {
			hw_buttons |= GW_BUTTON_B + GW_BUTTON_GAME;
		}
		else if (gpio_get_level(BUTTON_ACL) == 0) {
			gw_system_reset();
		}
		else {
			if (button_control_mode == 1) {
				hw_buttons = button_get_two_tropicalfish_buttons();
			}
			else {
				hw_buttons = button_get_two_default_buttons();
			}
		}
		
	}

    return hw_buttons;
}
