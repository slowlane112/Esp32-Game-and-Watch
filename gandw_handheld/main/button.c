#include "driver/gpio.h"
#include "esp_timer.h"
#include <gw_system.h>
#include "button.h"
#include "volume.h"
#include "audio.h"
#include "menu.h"

// buttons
#define BUTTON_GAME_A		GPIO_NUM_14
#define BUTTON_GAME_B		GPIO_NUM_5
#define BUTTON_TIME			GPIO_NUM_3
#define BUTTON_ALARM		GPIO_NUM_7
#define BUTTON_ACL			GPIO_NUM_8

#if defined(MODEL_SINGLE_SCREEN_2_BUTTON) || defined(MODEL_SINGLE_SCREEN_4_BUTTON) || defined(MODEL_MULTI_SCREEN_2_BUTTON)
	#define BUTTON_LEFT			GPIO_NUM_2
	#define BUTTON_RIGHT		GPIO_NUM_1
#endif

#if defined(MODEL_SINGLE_SCREEN_4_BUTTON)
	#define BUTTON_LEFT_2		GPIO_NUM_17
	#define BUTTON_RIGHT_2		GPIO_NUM_18
#endif

#if defined(MODEL_MULTI_SCREEN_DPAD)
	#define BUTTON_LEFT				GPIO_NUM_41
	#define BUTTON_RIGHT			GPIO_NUM_40
	#define BUTTON_UP				GPIO_NUM_42
	#define BUTTON_DOWN				GPIO_NUM_2
	#define BUTTON_JUMP				GPIO_NUM_1
#endif

// group buttons

#if defined(MODEL_SINGLE_SCREEN_2_BUTTON) || defined(MODEL_MULTI_SCREEN_2_BUTTON)
	static const int left_group_buttons[]  = { BUTTON_LEFT };
	static const int right_group_buttons[] = { BUTTON_RIGHT };
#elif defined(MODEL_SINGLE_SCREEN_4_BUTTON)
	static const int left_group_buttons[]  = { BUTTON_LEFT, BUTTON_LEFT_2 };
	static const int right_group_buttons[] = { BUTTON_RIGHT, BUTTON_RIGHT_2 };
#elif defined(MODEL_MULTI_SCREEN_DPAD)
	static const int left_group_buttons[]  = { BUTTON_LEFT };
	static const int right_group_buttons[] = { BUTTON_RIGHT };	
#endif

#if defined(MODEL_MULTI_SCREEN_DPAD)
	static const int start_button = GPIO_NUM_1;
	static const int reset_button_1 = BUTTON_LEFT;
	static const int reset_button_2 = BUTTON_JUMP;
#else
	static const int start_button = BUTTON_GAME_A;
	static const int reset_button_1 = BUTTON_LEFT;
	static const int reset_button_2 = BUTTON_RIGHT;	
#endif

button_control_type_t button_control_type = BUTTON_CONTROL_TYPE_NONE;
button_control_mode_t button_control_mode = BUTTON_CONTROL_MODE_NONE;

uint8_t button_start_delay = 63;
int64_t button_reset_timer = 0;

void button_init(void) {

	esp_rom_gpio_pad_select_gpio(BUTTON_GAME_B);
	gpio_set_direction(BUTTON_GAME_B, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON_GAME_B, GPIO_PULLUP_ONLY);
	
	esp_rom_gpio_pad_select_gpio(BUTTON_TIME);
	gpio_set_direction(BUTTON_TIME, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON_TIME, GPIO_PULLUP_ONLY);

	esp_rom_gpio_pad_select_gpio(BUTTON_GAME_A);
	gpio_set_direction(BUTTON_GAME_A, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON_GAME_A, GPIO_PULLUP_ONLY);

	esp_rom_gpio_pad_select_gpio(BUTTON_ALARM);
	gpio_set_direction(BUTTON_ALARM, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON_ALARM, GPIO_PULLUP_ONLY);

	esp_rom_gpio_pad_select_gpio(BUTTON_ACL);
	gpio_set_direction(BUTTON_ACL, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON_ACL, GPIO_PULLUP_ONLY);

#if defined(MODEL_SINGLE_SCREEN_2_BUTTON) || defined(MODEL_SINGLE_SCREEN_4_BUTTON) || defined(MODEL_MULTI_SCREEN_2_BUTTON)
	esp_rom_gpio_pad_select_gpio(BUTTON_LEFT);
	gpio_set_direction(BUTTON_LEFT, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON_LEFT, GPIO_PULLUP_ONLY);

	esp_rom_gpio_pad_select_gpio(BUTTON_RIGHT);
	gpio_set_direction(BUTTON_RIGHT, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON_RIGHT, GPIO_PULLUP_ONLY);
#endif
	
#if defined(MODEL_SINGLE_SCREEN_4_BUTTON)	
	esp_rom_gpio_pad_select_gpio(BUTTON_LEFT_2);
	gpio_set_direction(BUTTON_LEFT_2, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON_LEFT_2, GPIO_PULLUP_ONLY);

	esp_rom_gpio_pad_select_gpio(BUTTON_RIGHT_2);
	gpio_set_direction(BUTTON_RIGHT_2, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON_RIGHT_2, GPIO_PULLUP_ONLY);
#endif

#if defined(MODEL_MULTI_SCREEN_DPAD)

	esp_rom_gpio_pad_select_gpio(BUTTON_LEFT);
	gpio_set_direction(BUTTON_LEFT, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON_LEFT, GPIO_PULLUP_ONLY);

	esp_rom_gpio_pad_select_gpio(BUTTON_RIGHT);
	gpio_set_direction(BUTTON_RIGHT, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON_RIGHT, GPIO_PULLUP_ONLY);
	
	esp_rom_gpio_pad_select_gpio(BUTTON_UP);
	gpio_set_direction(BUTTON_UP, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON_UP, GPIO_PULLUP_ONLY);

	esp_rom_gpio_pad_select_gpio(BUTTON_DOWN);
	gpio_set_direction(BUTTON_DOWN, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON_DOWN, GPIO_PULLUP_ONLY);
	
	esp_rom_gpio_pad_select_gpio(BUTTON_JUMP);
	gpio_set_direction(BUTTON_JUMP, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON_JUMP, GPIO_PULLUP_ONLY);

#endif

}

static bool process_reset_buttons(void) {
	
	if (gpio_get_level(reset_button_1) == 0 && gpio_get_level(reset_button_2) == 0) { 
			
		if (button_reset_timer == 0) {
			button_reset_timer = esp_timer_get_time();
		}
		else {
			if (esp_timer_get_time() - button_reset_timer > 2000000) {
				menu_show = true;
			}
		}
		
		return true;
	}
	else {
		button_reset_timer = 0;
	}
	
	return false;
}

static bool left_group_pressed(void)
{
	for (int i = 0; i < sizeof(left_group_buttons)/sizeof(int); i++)
		if (gpio_get_level(left_group_buttons[i]) == 0)
			return true;

	return false;
}

static bool right_group_pressed(void)
{
	for (int i = 0; i < sizeof(right_group_buttons)/sizeof(int); i++)
		if (gpio_get_level(right_group_buttons[i]) == 0)
			return true;

	return false;
}

uint8_t button_get_menu_buttons(void)
{
	
	if (button_start_delay == 0) {
	
		if (gpio_get_level(BUTTON_TIME) == 0) { 

			if (right_group_pressed()) { 
				volume_set(true); // volume up
				volume_menu_tone(audio_handle);
				menu_update = true;
				button_start_delay = 7;
			}
			else if (left_group_pressed()) {
				volume_set(false); // volume down
				volume_menu_tone(audio_handle);
				menu_update = true;
				button_start_delay = 7;
			}

		}
		else if (gpio_get_level(reset_button_1) == 0 && gpio_get_level(reset_button_2) == 0) {
			button_start_delay = 31;
			return 0;
		}
		else if (right_group_pressed()) { 
			button_start_delay = 7;
			return 1;
		}
		else if (left_group_pressed()) {
			button_start_delay = 7;
			return 2;
		}
		else if (gpio_get_level(start_button) == 0) { 
			return 3;
		}
		
	}
		
	return 0;
}

// Four buttons

#if defined(MODEL_SINGLE_SCREEN_4_BUTTON)

	static unsigned int button_get_four_default_buttons(void) 
	{
		
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

	static unsigned int button_get_four_snoopytennis_buttons(void) 
	{
		
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

	static unsigned int button_get_four_marioscementfactory_buttons(void) {
		
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

	static unsigned int button_get_four_flagman_lion_buttons(void) {
		
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

	static unsigned int button_get_four_buttons(void)
	{
		
		uint32_t hw_buttons = 0;
		
		if (button_start_delay == 0) {
			
			if (process_reset_buttons()) {
				return 0;
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
				if (button_control_mode == BUTTON_CONTROL_MODE_SNOOPY_TENNIS) {
					hw_buttons = button_get_four_snoopytennis_buttons();
				}
				else if (button_control_mode == BUTTON_CONTROL_MODE_MARIOS_CEMENT_FACTORY) {
					hw_buttons = button_get_four_marioscementfactory_buttons();
				}
				else if (button_control_mode == BUTTON_CONTROL_MODE_FLAGMAN_LION) {
					hw_buttons = button_get_four_flagman_lion_buttons();
				}
				else {
					hw_buttons = button_get_four_default_buttons();
				}
			}
			
		}

		return hw_buttons;
		
	}
	
#endif

// Two buttons

#if defined(MODEL_SINGLE_SCREEN_2_BUTTON) || defined(MODEL_SINGLE_SCREEN_4_BUTTON) || defined(MODEL_MULTI_SCREEN_2_BUTTON)

	static unsigned int button_get_two_default_buttons(void) 
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

	static unsigned int button_get_two_tropicalfish_buttons(void) 
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

	static unsigned int button_get_two_buttons(void)
	{
		
		uint32_t hw_buttons = 0;
		
		if (button_start_delay == 0) {
			
			if (process_reset_buttons()) {
				return 0;
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
			else if (right_group_pressed()) {
				hw_buttons |= GW_BUTTON_RIGHT;
			}
			else if (left_group_pressed()) {
					hw_buttons |= GW_BUTTON_LEFT;
			}
			else if (gpio_get_level(BUTTON_ALARM) == 0) {
				hw_buttons |= GW_BUTTON_B + GW_BUTTON_GAME;
			}
			else if (gpio_get_level(BUTTON_ACL) == 0) {
				gw_system_reset();
			}
			else {
				if (button_control_mode == BUTTON_CONTROL_MODE_TROPICAL_FISH) {
					hw_buttons = button_get_two_tropicalfish_buttons();
				}
				else {
					hw_buttons = button_get_two_default_buttons();
				}
			}
			
		}

		return hw_buttons;
	}

#endif

// Dpad

#if defined(MODEL_MULTI_SCREEN_DPAD)

	static unsigned int button_get_dpad_buttons(void)
	{
		
		uint32_t hw_buttons = 0;
		
		if (button_start_delay == 0) {
			
			if (process_reset_buttons()) {
				return 0;
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
			else if (gpio_get_level(BUTTON_UP) == 0) { 
				hw_buttons |= GW_BUTTON_UP;
			}
			else if (gpio_get_level(BUTTON_DOWN) == 0) {
					hw_buttons |= GW_BUTTON_DOWN;
			}
			else if (gpio_get_level(BUTTON_JUMP) == 0) {
					hw_buttons |= GW_BUTTON_A;
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
			
		}

		return hw_buttons;
	}
	
#endif

unsigned int gw_get_buttons()
{
	uint32_t hw_buttons = 0;
	
#if defined(MODEL_SINGLE_SCREEN_4_BUTTON)
	if (button_control_type == BUTTON_CONTROL_TYPE_FOUR_BUTTON) {
		hw_buttons = button_get_four_buttons();
	}
	else {
		hw_buttons = button_get_two_buttons();
	}
#elif (defined(MODEL_SINGLE_SCREEN_2_BUTTON) || defined(MODEL_MULTI_SCREEN_2_BUTTON))
	hw_buttons = button_get_two_buttons();
	
#elif defined(MODEL_MULTI_SCREEN_DPAD)
	hw_buttons = button_get_dpad_buttons();
#endif

	return hw_buttons;
}
