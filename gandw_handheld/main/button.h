#ifndef BUTTON_H
#define BUTTON_H

typedef enum : uint8_t {
	BUTTON_CONTROL_TYPE_NONE = 0,
	BUTTON_CONTROL_TYPE_TWO_BUTTON = 1,
	BUTTON_CONTROL_TYPE_FOUR_BUTTON = 2
} button_control_type_t;

typedef enum : uint8_t {
	BUTTON_CONTROL_MODE_NONE = 0,
	BUTTON_CONTROL_MODE_TROPICAL_FISH = 1,
	BUTTON_CONTROL_MODE_SNOOPY_TENNIS = 2,
	BUTTON_CONTROL_MODE_MARIOS_CEMENT_FACTORY = 3,
	BUTTON_CONTROL_MODE_FLAGMAN_LION = 4
} button_control_mode_t;

extern button_control_type_t button_control_type;
extern button_control_mode_t button_control_mode;

extern uint8_t button_start_delay;
void button_init(void);
uint8_t button_get_menu_buttons(void);

#endif
