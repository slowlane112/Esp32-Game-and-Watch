
#ifndef GANDW_BATTERY_H
#define GANDW_BATTERY_H

extern uint8_t battery_level;
extern uint16_t battery_display_count;

void get_battery_level();

void display_battery(unsigned short *framebuffer);

#endif // GANDW_BATTERY_H


