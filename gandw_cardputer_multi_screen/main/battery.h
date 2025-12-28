#ifndef BATTERY_H
#define BATTERY_H

extern uint8_t battery_level;
extern uint16_t battery_display_count;
void battery_get_level();
void battery_display(unsigned short *framebuffer);

#endif

