#ifndef GANDW_VOLUME_H
#define GANDW_VOLUME_H

extern uint16_t volume ;
extern uint8_t volume_level;
extern uint32_t volume_time;
extern uint16_t volume_display_count;

void read_volume();

void set_volume(bool is_up);

void display_volume(unsigned short *framebuffer);


#endif // GANDW_VOLUME_H


