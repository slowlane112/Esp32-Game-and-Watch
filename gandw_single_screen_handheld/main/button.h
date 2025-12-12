#ifndef BUTTON_H
#define BUTTON_H

extern uint8_t button_control_type;
extern uint8_t button_control_mode;
extern uint8_t button_start_delay;
void button_setup();
uint8_t button_get_menu_buttons();
unsigned int button_get_four_buttons();
unsigned int button_get_two_buttons();

#endif
