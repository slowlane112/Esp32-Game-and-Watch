#ifndef BUTTON_H
#define BUTTON_H

extern uint8_t button_control_type;
extern uint8_t button_control_mode;
extern uint8_t button_start_delay;
uint8_t button_get_menu_buttons(char key);
unsigned int button_get_two_buttons(char key);
unsigned int button_get_dpad_buttons(char key);
unsigned int button_get_4direction_buttons(char key);
unsigned int button_get_bjack_buttons(char key);

#endif
