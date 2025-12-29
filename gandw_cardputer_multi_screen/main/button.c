#include <stdint.h>
#include <gw_system.h>
#include "driver/i2s_std.h"
#include "esp_timer.h"
#include "button.h"
#include "volume.h"
#include "menu.h"
#include "audio.h"

uint8_t button_control_type = 2;
uint8_t button_control_mode = 0;
uint8_t button_start_delay = 63;
int64_t button_reset_timer = 0;

uint8_t button_get_menu_buttons(char key)
{
	
	if (button_start_delay == 0) {
		
		if (key == ';') { 
			volume_set(true); // volume up
			volume_menu_tone(i2s_audio_handle, volume_level);
			menu_update = true;
			button_start_delay = 7;
		}
		else if (key == '.') { 
			volume_set(false); // volume down
			volume_menu_tone(i2s_audio_handle, volume_level);
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

void process_reset_key(char key) {
	
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

unsigned int button_get_two_buttons(char key)
{
	
	/*  button_control_type = 2
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

	if (key == '1') { 
        hw_buttons |= GW_BUTTON_GAME;
    }
    else if (key == '2') {
        hw_buttons |= GW_BUTTON_TIME;
    }
	else if (key == '3') { 
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

    return hw_buttons;
}

unsigned int button_get_dpad_buttons(char key)
{
	
	/*  button_control_type = 5
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

unsigned int button_get_4direction_default_buttons(char key) {
	
	// button_control_mode = 0
	
	uint32_t hw_buttons = 0;
	
	if (key == '1') { 
        hw_buttons |= GW_BUTTON_GAME;
    }
    else if (key == '2') {
        hw_buttons |= GW_BUTTON_TIME;
    }
	
	return hw_buttons;
        
}

unsigned int button_get_4direction_squish_buttons(char key) {
	
	// button_control_mode = 1
	
	uint32_t hw_buttons = 0;
	
	if (key == '1') { 
        hw_buttons |= GW_BUTTON_TIME;
    }
    else if (key == '2') {
        hw_buttons |= GW_BUTTON_GAME;
    }
	
	return hw_buttons;
        
}

unsigned int button_get_4direction_buttons(char key)
{
	
	/*
	 *  button_control_type = 3
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
		
		if (button_control_mode == 1) {
			hw_buttons = button_get_4direction_squish_buttons(key);
		}
		else {
			hw_buttons = button_get_4direction_default_buttons(key);
		}
		
	}
	
    return hw_buttons;
}

unsigned int button_get_bjack_buttons(char key)
{
	
	/*  button_control_type = 4
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
