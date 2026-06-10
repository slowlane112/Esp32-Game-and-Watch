#ifndef DISPLAY_H
#define DISPLAY_H

#include "esp_lcd_panel_ops.h"

#define RENDER_HEIGHT		135
#define RENDER_PADDING		0
#define DISPLAY2_WIDTH		320
#define DISPLAY2_HEIGHT		240

extern esp_lcd_panel_handle_t display_1_handle;
extern esp_lcd_panel_handle_t display_2_handle;
extern bool display_transfer_in_progress;
void display_render_screen(unsigned short *framebuffer);
void display_blank_screen(unsigned short *framebuffer);
void display_setup_top_screen(unsigned short *framebuffer);
void display_init(void);

#endif


