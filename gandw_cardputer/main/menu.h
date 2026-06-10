#ifndef GANDW_MENU_H
#define GANDW_MENU_H

#include <stdbool.h>

extern bool menu_show;
extern bool menu_update;
extern uint8_t menu_index;
void menu_display_item(unsigned short *framebuffer, int item_number);
void menu_buttons_process(void);

#endif

