#include <stdio.h>
#include <string.h>
#include <gw_system.h>
#include "menu.h"
#include "game.h"
#include "pixel_font.h"

uint16_t disabled_color = BYTE_SWAP ? 0xf49c : 0x9cf4;
uint16_t menu_bg_color = BYTE_SWAP ? 0xf8bd : 0xbdf8;
uint16_t menu_bar_line_color = BYTE_SWAP ? 0xf49c : 0x9cf4;
bool menu_show = true;
bool menu_update = true;
uint8_t menu_index = 0;
uint32_t menu_time = 0;
extern const uint8_t img_start[] asm("_binary_menu_raw_start");
extern const uint8_t img_end[]   asm("_binary_menu_raw_end");

uint16_t menu_get_color(uint16_t color) 
{
	if (BYTE_SWAP) {
		return __builtin_bswap16(color);
	}
	
	return color;
}

void menu_display_pixel_font_pixel(unsigned short *framebuffer, int x_pos, int y_pos, uint16_t color) 
{
	
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 2; j++) {
			framebuffer[x_pos + i + y_pos + (j * GW_SCREEN_WIDTH)] = color;
		}
	}
	
}

void menu_display_text(unsigned short *framebuffer, const char *text, int y_start, int x_start, uint16_t text_fg_color, uint16_t text_bg_color) 
{
	
	uint8_t pixel_font_data = 0;
    uint8_t pixel_font_pos = 0;
    int x_pos = 0;
    int y_pos = 0;
    int text_pos = 0;
	
	for (int i = 0; text[i] != '\0'; i++) {
			
		pixel_font_pos = (int)text[i];
	
		for (int j = 0; j < 8; j++) {
		
			pixel_font_data = pixel_font_get_data((pixel_font_pos * 8) + j);
		
			for (int k = 7; k >= 0; k--) {

				x_pos = x_start + (text_pos * 8) + (7 - k);
				y_pos = (y_start * GW_SCREEN_WIDTH) + ((j * 2) * GW_SCREEN_WIDTH);
					
				menu_display_pixel_font_pixel(framebuffer, x_pos, y_pos, ((pixel_font_data >> k) & 1) ? text_fg_color : text_bg_color);
				
			}
		
		}
		
		text_pos++;
	}
	
}

void menu_display_arrow(unsigned short *framebuffer, int y_start, int x_start, uint16_t fg_color, uint16_t bg_color, bool flip) 
{
	uint8_t arrow_icon_8x15[15][8] = {
		{1,0,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0},
		{1,1,1,0,0,0,0,0},
		{1,1,1,1,0,0,0,0},
		{0,1,1,1,1,0,0,0},
		{0,0,1,1,1,1,0,0},
		{0,0,0,1,1,1,1,0},
		{0,0,0,0,1,1,1,1},
		{0,0,0,1,1,1,1,0},
		{0,0,1,1,1,1,0,0},
		{0,1,1,1,1,0,0,0},
		{1,1,1,1,0,0,0,0},
		{1,1,1,0,0,0,0,0},
		{1,1,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0}
	};
	
	int fb_pos = (y_start * GW_SCREEN_WIDTH) + x_start;

	for (int y = 0; y < 15; y++) {

		for (int x = 0; x < 8; x++) {
			
			int src_x = flip ? (7 - x) : x; 

			framebuffer[fb_pos] = (arrow_icon_8x15[y][src_x] == 1) ? fg_color : bg_color;
			
			fb_pos++;
		}
		
		fb_pos = fb_pos + GW_SCREEN_WIDTH - 8;
	}
}

int menu_get_game_offset(uint8_t index)
{
    int offset = 0;

    for (uint8_t i = 0; i < index; i++) {
        offset += games[i].img_width * games[i].img_height;
    }

    return offset;
}

void menu_display_top_section(unsigned short *framebuffer, uint16_t bar_color, Game game)
{
	
	int text_width = 8 * 19;
    int text_height = 16;	
	int y_start = 3;
    int x_start = (GW_SCREEN_WIDTH - text_width) / 2;
    
    int fb_pos = 0;
	
	for (int y = 0; y < 23; y++) {
		
        for (int x = 0; x < GW_SCREEN_WIDTH; x++) {
			
			if (!(y >= y_start
				&& x >= x_start
				&& y < y_start + text_height
				&& x < x_start + text_width)) {
			
					framebuffer[fb_pos] = (y == 22) ? menu_bar_line_color : bar_color;
			
			}
			
			fb_pos++;
			
		}
		
	}

	menu_display_text(framebuffer, "Select Game & Watch", y_start, x_start, game.text_color, bar_color);
	
}

void menu_display_bottom_section(unsigned short *framebuffer, uint16_t bar_color, Game game)
{
	
    int text_height = 16;
	int text_y_start = (GW_SCREEN_HEIGHT - 19);
    
	int text1_width = 8 * strlen(game.name);
    int text1_x_start = 3;
    
    int text2_width = 8 * strlen(game.year);
    int text2_x_start = GW_SCREEN_WIDTH - text2_width - 3;
    
    int y_start = GW_SCREEN_HEIGHT - 23;
    
    int fb_pos = y_start * GW_SCREEN_WIDTH;
	
	for (int y = y_start; y < GW_SCREEN_HEIGHT; y++) {
		
        for (int x = 0; x < GW_SCREEN_WIDTH; x++) {
			
			if (!
					(
						(
							y >= text_y_start
							&& x >= text1_x_start
							&& y < text_y_start + text_height
							&& x < text1_x_start + text1_width
						)
						||
						(
							y >= text_y_start
							&& x >= text2_x_start
							&& y < text_y_start + text_height
							&& x < text2_x_start + text2_width
						)
					
					)
				) {
			
					framebuffer[fb_pos] = (y == y_start) ? menu_bar_line_color : bar_color;
			
			}
			
			fb_pos++;
			
		}
		
	}

	menu_display_text(framebuffer, game.name, text_y_start, text1_x_start, game.text_color, bar_color);		

	menu_display_text(framebuffer, game.year, text_y_start, text2_x_start, game.text_color, bar_color);
	
}

void menu_display_item(unsigned short *framebuffer, int index)
{

	int offset = menu_get_game_offset(index);
	Game game = games[index];
	
	int arrow_y = (GW_SCREEN_HEIGHT / 2) - 7;
	int arrow1_x = 11;
	int arrow2_x = GW_SCREEN_WIDTH - 11 - 8;
	const uint16_t *img = (const uint16_t *)img_start + offset;
	int mid_pixel_index = game.color_line * game.img_width + (game.img_width / 2);
	uint16_t bar_color = menu_get_color(img[mid_pixel_index]);
	uint16_t px_color = 0;
	int img_pos = 0;
	int img_y = (GW_SCREEN_HEIGHT - game.img_height) / 2;
    int img_x = (GW_SCREEN_WIDTH - game.img_width) / 2;
	int fb_pos = 23 * GW_SCREEN_WIDTH;
	
	for (int y = 23; y < GW_SCREEN_HEIGHT - 23; y++) {
		
        for (int x = 0; x < GW_SCREEN_WIDTH; x++) {

			if (
					!(
						(y >= arrow_y
						&& y < arrow_y + 15
						&& x >= arrow1_x
						&& x < arrow1_x + 8)
						||
						(y >= arrow_y
						&& y < arrow_y + 15
						&& x >= arrow2_x
						&& x < arrow2_x + 8)
					)
				) {


					if (y >= img_y 
						&& x >= img_x 
						&& y < (img_y + game.img_height)
						&& x < (img_x + game.img_width)) {
							
							px_color = menu_get_color(img[img_pos]);
							img_pos++;
							

					}
					else {
						px_color = menu_bg_color;
					}
			
					framebuffer[fb_pos] = px_color;
					
			}
			
			fb_pos++;
			
		}
    }
    
    //uint16_t bar_color = 0xff00;
	
	menu_display_arrow(framebuffer, arrow_y, arrow1_x, index == 0 ? disabled_color : bar_color, menu_bg_color, true);
	
	menu_display_arrow(framebuffer, arrow_y, arrow2_x, (index == game_count - 1) ? disabled_color : bar_color, menu_bg_color, false);
	
	menu_display_top_section(framebuffer, bar_color, game);
    
	menu_display_bottom_section(framebuffer, bar_color, game);
	
} 
