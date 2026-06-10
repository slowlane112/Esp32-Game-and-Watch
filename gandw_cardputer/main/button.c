#include "esp_timer.h"
#include <gw_system.h>
#include "button.h"
#include "volume.h"
#include "menu.h"
#include "audio.h"
#include "keyboard.h"

button_control_type_t button_control_type = BUTTON_CONTROL_TYPE_NONE;
button_control_mode_t button_control_mode = BUTTON_CONTROL_MODE_NONE;
uint8_t button_start_delay = 63;
static int64_t button_reset_timer = 0;

uint8_t button_get_menu_buttons(char key)
{
	
	if (button_start_delay == 0) {
		
		if (key == ';') { 
			volume_set(true); // volume up
			volume_menu_tone(i2s_audio_handle);
			menu_update = true;
			button_start_delay = 7;
		}
		else if (key == '.') { 
			volume_set(false); // volume down
			volume_menu_tone(i2s_audio_handle);
			menu_update = true;
			button_start_delay = 7;
		}
		else if (key == 0x87) { 
			button_start_delay = 31;
			return 0;
		}
		else if (key == '/' || key == 0x20) { 
			button_start_delay = 7;
			return 1;
		}
		else if (key == ',' || key == 0x80) { 
			button_start_delay = 7;
			return 2;
		}
		else if (key == 0x86) { 
			return 3;
		}
		
	}
		
	return 0;
}

static void process_reset_key(char key) {
	
	if (key == 0x87) { 
		
		if (button_reset_timer == 0) {
			button_reset_timer = esp_timer_get_time();
		}
		else {
			if (esp_timer_get_time() - button_reset_timer > 2000000) {
				menu_show = true;
			}
		}

	}
	else {
		button_reset_timer = 0;
	}
}

static unsigned int button_get_two_default_buttons(char key) {
	
	uint32_t hw_buttons = 0;
	
	if (key == '1') { 
		hw_buttons |= GW_BUTTON_GAME;
	}
	else if (key == '2') {
		hw_buttons |= GW_BUTTON_TIME;
	}
	
	return hw_buttons;
		
}

static unsigned int button_get_two_tropicalfish_buttons(char key) {
	
	uint32_t hw_buttons = 0;
	
	if (key == '1') { 
		hw_buttons |= GW_BUTTON_TIME;
	}
	else if (key == '2') {
		hw_buttons |= GW_BUTTON_GAME;
	}
	
	return hw_buttons;
		
}

static unsigned int button_get_four_default_buttons(char key) {
	
	uint32_t hw_buttons = 0;
	
	if (key == 0x20) {
		hw_buttons |= GW_BUTTON_RIGHT + GW_BUTTON_DOWN;
	}
	else if (key == 0x80) {
		hw_buttons |= GW_BUTTON_LEFT + GW_BUTTON_DOWN;
	}
	else if (key == 0x86) { 
		hw_buttons |= GW_BUTTON_RIGHT + GW_BUTTON_UP;
	}
	else if (key == 0x85) {
		hw_buttons |= GW_BUTTON_LEFT + GW_BUTTON_UP;
	}
	
	return hw_buttons;
		
}

static unsigned int button_get_four_snoopytennis_buttons(char key) {
	
	uint32_t hw_buttons = 0;
	
	if (key == 0x20) {
		hw_buttons |= GW_BUTTON_DOWN;
	}
	else if (key == 0x80) {
		hw_buttons |= GW_BUTTON_A;
	}
	else if (key == 0x86) { 
		hw_buttons |= GW_BUTTON_UP;
	}
	else if (key == 0x85) {
		hw_buttons |= GW_BUTTON_A;
	}
	
	return hw_buttons;
		
}

static unsigned int button_get_four_marioscementfactory_buttons(char key) {
	
	uint32_t hw_buttons = 0;
	
	if (key == 0x82) {
		hw_buttons |= GW_BUTTON_RIGHT;
	}
	else if (key == 0x80) {
		hw_buttons |= GW_BUTTON_LEFT;
	}
	else if (key == 0x20) { 
		hw_buttons |= GW_BUTTON_A;
	}
	
	return hw_buttons;
		
}

static unsigned int button_get_four_flagman_lion_buttons(char key) {
	
	uint32_t hw_buttons = 0;
	
	if (key == 0x20) {
		hw_buttons |= GW_BUTTON_B;
	}
	else if (key == 0x80) {
		hw_buttons |= GW_BUTTON_DOWN;
	}
	else if (key == 0x86) { 
		hw_buttons |= GW_BUTTON_A;
	}
	else if (key == 0x85) {
		hw_buttons |= GW_BUTTON_UP;
	}
	
	return hw_buttons;
		
}

static unsigned int button_get_four_buttons(char key)
{
	
	/*
	 *  1			Game A
	 *  2			Game B
	 *  3			Time
	 *  4			Alarm
	 *  5			ACL
	 *  crtl 0x80	Left Down
	 *  fn 0x85		Left Up
	 *  space 0x20  Right Down
	 *  enter 0x86	Right Up
	 */
	 
	uint32_t hw_buttons = 0;
	
	process_reset_key(key);

	if (key == '3') { 
		hw_buttons |= GW_BUTTON_B + GW_BUTTON_TIME;
	}
	else if (key == '1') { 
		hw_buttons |= GW_BUTTON_GAME;
	}
	else if (key == '2') {
		hw_buttons |= GW_BUTTON_TIME;
	}
	else if (key == '4') {
		hw_buttons |= GW_BUTTON_B + GW_BUTTON_GAME;
	}
	else if (key == '5') {
		gw_system_reset();
	}
	else {
		if (button_control_mode == BUTTON_CONTROL_MODE_SNOOPY_TENNIS) {
			hw_buttons = button_get_four_snoopytennis_buttons(key);
		}
		else if (button_control_mode == BUTTON_CONTROL_MODE_MARIOS_CEMENT_FACTORY) {
			hw_buttons = button_get_four_marioscementfactory_buttons(key);
		}
		else if (button_control_mode == BUTTON_CONTROL_MODE_FLAGMAN_LION) {
			hw_buttons = button_get_four_flagman_lion_buttons(key);
		}
		else {
			hw_buttons = button_get_four_default_buttons(key);
		}
	}

	return hw_buttons;
}

static unsigned int button_get_two_buttons(char key)
{
	
	/*
	 *  1			Game A
	 *  2			Game B
	 *  3			Time
	 *  4			Alarm
	 *  5			ACL
	 *  crtl 0x80	Left
	 *  space 0x20	Right
	 */
	
	uint32_t hw_buttons = 0;
	
	process_reset_key(key);

	if (key == '3') { 
		hw_buttons |= GW_BUTTON_B + GW_BUTTON_TIME;
	}
	else if (key == 0x20) { 
		hw_buttons |= GW_BUTTON_RIGHT;
	}
	else if (key == 0x80) {
			hw_buttons |= GW_BUTTON_LEFT;
	}
	else if (key == '4') {
		hw_buttons |= GW_BUTTON_B + GW_BUTTON_GAME;
	}
	else if (key == '5') {
		gw_system_reset();
	}
	else {
		if (button_control_mode == BUTTON_CONTROL_MODE_TROPICAL_FISH) {
			hw_buttons = button_get_two_tropicalfish_buttons(key);
		}
		else {
			hw_buttons = button_get_two_default_buttons(key);
		}
	}

	return hw_buttons;
}

static unsigned int button_get_dpad_buttons(char key)
{
	
	/*
	 *  1			Game A
	 *  2			Game B
	 *  3			Time
	 *  4			Alarm
	 *  5			ACL
	 *  crtl 0x80	Left
	 *  alt 0x81	Right
	 *  Aa 0x84		Up
	 *  opt 0x82	Down
	 * 	space 0x20	Button
	 */
	
	uint32_t hw_buttons = 0;
	
	process_reset_key(key);

	if (key == '1') { 
		hw_buttons |= GW_BUTTON_GAME;
	}
	else if (key == '2') {
		hw_buttons |= GW_BUTTON_TIME;
	}
	else if (key == '3') { 
		hw_buttons |= GW_BUTTON_B + GW_BUTTON_TIME;
	}
	else if (key == '4') {
		hw_buttons |= GW_BUTTON_B + GW_BUTTON_GAME;
	}
	else if (key == '5') {
		gw_system_reset();
	}
	else if (key == 0x80) {
		hw_buttons |= GW_BUTTON_LEFT;
	}
	else if (key == 0x81) {
		hw_buttons |= GW_BUTTON_RIGHT;
	}
	else if (key == 0x84) {
		hw_buttons |= GW_BUTTON_UP;
	}
	else if (key == 0x82) {
		hw_buttons |= GW_BUTTON_DOWN;
	}
	else if (key == 0x20) {
		hw_buttons |= GW_BUTTON_A;
	}
	
	return hw_buttons;
}

static unsigned int button_get_multi_direction_default_buttons(char key) {
	
	uint32_t hw_buttons = 0;
	
	if (key == '1') { 
		hw_buttons |= GW_BUTTON_GAME;
	}
	else if (key == '2') {
		hw_buttons |= GW_BUTTON_TIME;
	}
	
	return hw_buttons;
		
}

static unsigned int button_get_multi_direction_squish_buttons(char key) {
	
	
	uint32_t hw_buttons = 0;
	
	if (key == '1') { 
		hw_buttons |= GW_BUTTON_TIME;
	}
	else if (key == '2') {
		hw_buttons |= GW_BUTTON_GAME;
	}
	
	return hw_buttons;
		
}

static unsigned int button_get_multi_direction_buttons(char key)
{
	
	/*
	 *  1			Game A
	 *  2			Game B
	 *  3			Time
	 *  4			Alarm
	 *  5			ACL
	 *  Fn 0x85		Up
	 *  crtl 0x80	Down
	 *  /			Left
	 *  space 0x20	Right
	 */
	
	uint32_t hw_buttons = 0;
	
	process_reset_key(key);

	if (key == '3') { 
		hw_buttons |= GW_BUTTON_B + GW_BUTTON_TIME;
	}
	else if (key == '4') {
		hw_buttons |= GW_BUTTON_B + GW_BUTTON_GAME;
	}
	else if (key == '5') {
		gw_system_reset();
	}
	else if (key == '/') {
		hw_buttons |= GW_BUTTON_LEFT;
	}
	else if (key == 0x20) {
		hw_buttons |= GW_BUTTON_RIGHT;
	}
	else if (key == 0x85) {
		hw_buttons |= GW_BUTTON_UP;
	}
	else if (key == 0x80) {
		hw_buttons |= GW_BUTTON_DOWN;
	}
	else {
		
		if (button_control_mode == BUTTON_CONTROL_MODE_SQUISH) {
			hw_buttons = button_get_multi_direction_squish_buttons(key);
		}
		else {
			hw_buttons = button_get_multi_direction_default_buttons(key);
		}
		
	}
	
	return hw_buttons;
}

static unsigned int button_get_blackjack_buttons(char key)
{
	
	/*  
	 *  1			Game A
	 *  2			Game B
	 *  3			Time
	 *  4			Alarm
	 *  5			ACL
	 *  crtl 0x80	Bet x10 / Hit
	 *  fn 0x85		Double Down
	 *  space 0x20  Bet x1 / Stand
	 *  enter 0x86	Enter
	 */
	 
	uint32_t hw_buttons = 0;
	
	process_reset_key(key);

	if (key == '3') { 
		hw_buttons |= GW_BUTTON_B + GW_BUTTON_TIME;
	}
	else if (key == '1') { 
		hw_buttons |= GW_BUTTON_GAME;
	}
	else if (key == '2') {
		hw_buttons |= GW_BUTTON_TIME;
	}
	else if (key == '4') {
		hw_buttons |= GW_BUTTON_B + GW_BUTTON_GAME;
	}
	else if (key == '5') {
		gw_system_reset();
	}
	if (key == 0x20) {
		hw_buttons |= GW_BUTTON_B; // Bet x1 / Stand
	}
	else if (key == 0x80) {
		hw_buttons |= GW_BUTTON_DOWN; // Bet x10 / Hit
	}
	else if (key == 0x86) { 
		hw_buttons |= GW_BUTTON_A; // Enter
	}
	else if (key == 0x85) { 
		hw_buttons |= GW_BUTTON_UP; // Double Down
	}

	return hw_buttons;
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
	
		if (button_control_type == BUTTON_CONTROL_TYPE_FOUR_BUTTON) {
			hw_buttons = button_get_four_buttons(key);
		}
		else if (button_control_type == BUTTON_CONTROL_TYPE_DPAD) {
			hw_buttons = button_get_dpad_buttons(key);
		}
		else if (button_control_type == BUTTON_CONTROL_TYPE_MULTI_DIRECTION) {
			hw_buttons = button_get_multi_direction_buttons(key);
		}
		else if (button_control_type == BUTTON_CONTROL_TYPE_BLACKJACK) {
			hw_buttons = button_get_blackjack_buttons(key);
		}		
		else {
			hw_buttons = button_get_two_buttons(key);
		}
		
	}

	return hw_buttons;
}
