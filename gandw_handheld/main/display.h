#ifndef DISPLAY_H
#define DISPLAY_H

#include "esp_lcd_panel_ops.h"

#define DISPLAY_HEIGHT	240

#if defined(MODEL_MULTI_SCREEN_2_BUTTON) || defined(MODEL_MULTI_SCREEN_DPAD)
	#define RENDER_HEIGHT	210
	#define RENDER_PADDING	15
#else
	#define RENDER_HEIGHT	240
	#define RENDER_PADDING	0
#endif

extern esp_lcd_panel_handle_t display_1_handle;
extern esp_lcd_panel_handle_t display_2_handle;
extern bool display_transfer_in_progress;
void display_render_screen(unsigned short *framebuffer);
void display_blank_screen(unsigned short *framebuffer);
void display_init(void);

#endif


