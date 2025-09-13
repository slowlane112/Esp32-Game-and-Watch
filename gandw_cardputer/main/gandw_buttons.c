#include <stdint.h>
#include <gw_system.h>
#include "gandw_buttons.h"

uint8_t control_button_type = 2;
uint8_t control_button_mode = 0;

unsigned int gw_get_two_default_buttons(char key) {
	
	uint32_t hw_buttons = 0;
	
	if (key == '1') { 
        hw_buttons |= GW_BUTTON_GAME;
    }
    else if (key == '2') {
        hw_buttons |= GW_BUTTON_TIME;
    }
	
	return hw_buttons;
        
}

unsigned int gw_get_two_tropicalfish_buttons(char key) {
	
	uint32_t hw_buttons = 0;
	
	if (key == '1') { 
        hw_buttons |= GW_BUTTON_TIME;
    }
    else if (key == '2') {
        hw_buttons |= GW_BUTTON_GAME;
    }
	
	return hw_buttons;
        
}


unsigned int gw_get_four_default_buttons(char key) {
	
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

unsigned int gw_get_four_snoopytennis_buttons(char key) {
	
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

unsigned int gw_get_four_marioscementfactory_buttons(char key) {
	
	uint32_t hw_buttons = 0;
	
	if (key == 0x20) {
		hw_buttons |= GW_BUTTON_RIGHT;
	}
	else if (key == 0x80) {
		hw_buttons |= GW_BUTTON_LEFT;
	}
	else if (key == 0x86) { 
		hw_buttons |= GW_BUTTON_A;
	}
	else if (key == 0x85) {
		hw_buttons |= GW_BUTTON_A;
	}
	
	return hw_buttons;
        
}

unsigned int gw_get_four_flagman_lion_buttons(char key) {
	
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

unsigned int gw_get_four_buttons(char key)
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
		if (control_button_mode == 1) {
			hw_buttons = gw_get_four_snoopytennis_buttons(key);
		}
		else if (control_button_mode == 2) {
			hw_buttons = gw_get_four_marioscementfactory_buttons(key);
		}
		else if (control_button_mode == 3) {
			hw_buttons = gw_get_four_flagman_lion_buttons(key);
		}
		else {
			hw_buttons = gw_get_four_default_buttons(key);
		}
	}

    return hw_buttons;
}


unsigned int gw_get_two_buttons(char key)
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
		if (control_button_mode == 1) {
			hw_buttons = gw_get_two_tropicalfish_buttons(key);
		}
		else {
			hw_buttons = gw_get_two_default_buttons(key);
		}
	}

    return hw_buttons;
}

unsigned int gw_get_five_buttons(char key)
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
