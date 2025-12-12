#ifndef VOLUME_H
#define VOLUME_H

#include "driver/i2s_std.h"

extern uint16_t volume ;
extern uint8_t volume_level;
extern uint32_t volume_time;
extern uint16_t volume_display_count;
void volume_read();
void volume_set(bool is_up);
void volume_display(unsigned short *framebuffer);
void volume_menu_tone(i2s_chan_handle_t i2s_handle, int volume_level);

#endif


