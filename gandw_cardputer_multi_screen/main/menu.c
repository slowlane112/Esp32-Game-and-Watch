#include <stdio.h>
#include <string.h>
#include <gw_system.h>
#include "menu.h"
#include "game.h"
#include "pixel_font.h"

uint16_t disabled_color = BYTE_SWAP ? 0xf49c : 0x9cf4;
uint16_t menu_bg_color = BYTE_SWAP ? 0xf8bd : 0xbdf8;
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
	
	int item = 0;
    int offset = 0;

    for (uint8_t i = 0; i < game_count; i++) {
		
		if (index == item) {
			return offset;
		}
		
        offset += games[i].img_unit_width * games[i].img_unit_height;
        item ++;
        
        if (index == item) {
			return offset;
		}
		
		offset += games[i].img_box_width * games[i].img_box_height;
        item ++;
    }

    return 0;
}

void menu_display_top_section(unsigned short *framebuffer, uint16_t box_bg_color, Game game)
{
	
	int text_width = 8 * 19;
    int text_height = 16;	
	int y_start = RENDER_HEIGHT + 3;
    int x_start = (GW_SCREEN_WIDTH - text_width) / 2;
    
    int fb_pos = RENDER_HEIGHT * GW_SCREEN_WIDTH;
	
	for (int y = RENDER_HEIGHT; y < RENDER_HEIGHT + 23; y++) {
		
        for (int x = 0; x < GW_SCREEN_WIDTH; x++) {
			
			if (!(y >= y_start
				&& x >= x_start
				&& y < y_start + text_height
				&& x < x_start + text_width)) {
			
					framebuffer[fb_pos] = box_bg_color;
			
			}
			
			fb_pos++;
			
		}
		
	}

	menu_display_text(framebuffer, "Select Game & Watch", y_start, x_start, menu_get_color(game.text_color), box_bg_color);
	
}

void menu_display_bottom_section(unsigned short *framebuffer, uint16_t box_bg_color, Game game)
{
	
    int text_height = 16;
	int text_y_start = RENDER_HEIGHT + (RENDER_HEIGHT - 19);
    
	int text1_width = 8 * strlen(game.name);
    int text1_x_start = 3;
    
    int text2_width = 8 * strlen(game.year);
    int text2_x_start = GW_SCREEN_WIDTH - text2_width - 3;
    
    int y_start = RENDER_HEIGHT + (RENDER_HEIGHT - 23);
    
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
			
					framebuffer[fb_pos] = box_bg_color;
			}
			
			fb_pos++;
			
		}
		
	}

	menu_display_text(framebuffer, game.name, text_y_start, text1_x_start, menu_get_color(game.text_color), box_bg_color);		

	menu_display_text(framebuffer, game.year, text_y_start, text2_x_start, menu_get_color(game.text_color), box_bg_color);

	
}


void menu_display_item(unsigned short *framebuffer, int index)
{

	Game game = games[index];

	// start of unit image in menu.raw
	int unit_offset = menu_get_game_offset((index * 2));
	const uint16_t *unit_img = (const uint16_t *)img_start + unit_offset;
	
	// start of box image in menu.raw
	int box_offset = menu_get_game_offset((index * 2) + 1);
	const uint16_t *box_img = (const uint16_t *)img_start + box_offset;

	uint16_t box_bg_color = menu_get_color(game.box_bg_color);
	uint16_t box_border_color = menu_get_color(game.box_border_color);
	uint16_t text_color = menu_get_color(game.text_color);
	
	uint16_t px_color = 0;

	// render top screen
	
	int unit_img_pos = 0;
	int unit_img_y = (RENDER_HEIGHT - game.img_unit_height) / 2;
    int unit_img_x = (GW_SCREEN_WIDTH - game.img_unit_width) / 2;
    
	int fb_pos = 0;
	
    
    for (int y = 0; y < RENDER_HEIGHT; y++) {
		
		for (int x = 0; x < GW_SCREEN_WIDTH; x++) {
			
			if (y >= unit_img_y 
				&& x >= unit_img_x 
				&& y < (unit_img_y + game.img_unit_height)
				&& x < (unit_img_x + game.img_unit_width)) {
					
					px_color = menu_get_color(unit_img[unit_img_pos]);
					unit_img_pos++;
					

			}
			else {
				px_color = menu_bg_color;
			}
	
			framebuffer[fb_pos] = px_color;
		
			fb_pos++;	
		}
		
	}
    
    // render bottom screen
    
	int box_img_pos = 0;
	int box_img_y = RENDER_HEIGHT + ((RENDER_HEIGHT - game.img_box_height) / 2);
    int box_img_x = (GW_SCREEN_WIDTH - game.img_box_width) / 2;
    
    // arrow position
	int arrow_y = RENDER_HEIGHT + ((RENDER_HEIGHT / 2) - 7);
	int arrow1_x = 6;
	int arrow2_x = GW_SCREEN_WIDTH - 6 - 8;
    
	fb_pos = (RENDER_HEIGHT + 23) * GW_SCREEN_WIDTH;
	
	for (int y = RENDER_HEIGHT + 23; y < GW_SCREEN_HEIGHT - 23; y++) {
		
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

					if (y >= box_img_y 
						&& x >= box_img_x 
						&& y < (box_img_y + game.img_box_height)
						&& x < (box_img_x + game.img_box_width)) {
							
							if (y == box_img_y 
								|| y == (box_img_y + game.img_box_height - 1)
								|| x == box_img_x
								|| x == (box_img_x + game.img_box_width - 1)
								
							)
							{
								// border
								px_color = box_border_color;

							}
							else {
							
								px_color = menu_get_color(box_img[box_img_pos]);
							
							}
							
							box_img_pos++;
							

					}
					else {
						px_color = box_bg_color; 
					}
			
					framebuffer[fb_pos] = px_color;
					
			}
			
			fb_pos++;
			
		}
    }
	
	menu_display_arrow(framebuffer, arrow_y, arrow1_x, index == 0 ? disabled_color : text_color, box_bg_color, true);
	
	menu_display_arrow(framebuffer, arrow_y, arrow2_x, (index == game_count - 1) ? disabled_color : text_color, box_bg_color, false);
	
	menu_display_top_section(framebuffer, box_bg_color, game);
    
	menu_display_bottom_section(framebuffer, box_bg_color, game);
	
} 
