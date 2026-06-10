#ifndef BATTERY_H
#define BATTERY_H

extern uint16_t battery_display_count;
void battery_get_level(void);
void battery_display(unsigned short *framebuffer);

#endif

