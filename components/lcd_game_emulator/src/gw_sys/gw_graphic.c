/*

This program implements the graphics rendering for the different variation
of SM510 family microcomputer.

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program. If not, see <http://www.gnu.org/licenses/>.

__author__ = "bzhxx"
__contact__ = "https://github.com/bzhxx"
__license__ = "GPLv3"

*/
#include <string.h>
#include "gw_type_defs.h"
#include "gw_graphic.h"
#include "gw_system.h"
#include "gw_romloader.h"
#include "sm510.h"


#include "esp_heap_caps.h"

//  RGB565 16bits pixel format to 32bits pixel format
/*
			RRRRRGGGGGGBBBBB
			RGB16 bits to 32 bits
			RRRRRGGGGGGBBBBBRRRRRGGGGGGBBBBB
			00000111111000001111100000011111
		0x     ...0...7...e...0...f...8...1....f
*/
#define GW_RGB565_32BITS 0x07E0F81F

// RGB565 16bits pixel format mask:  per color
#define GW_MASK_RGB565_R 0xF800
#define GW_MASK_RGB565_G 0x07E0
#define GW_MASK_RGB565_B 0x001F

bool process_oil_panic = false;

#define IS_ROM(s) (memcmp(gw_head.rom_signature, s, 8) == 0)

static uint16 *gw_graphic_framebuffer = 0;

static uint16 *source_mixer = 0;

static uint8 SEG_TRANSPARENT_COLOR = 0;
static uint16 SEG_TRANSPARENT_COLOR_16_BIT = 0;
#define SEG_WHITE_COLOR 0xff
#define SEG_BLACK_COLOR 0x0
#define SEG_WHITE_COLOR_16_BIT 0xffff
#define SEG_BLACK_COLOR_16_BIT 0x0000

unsigned int gw_segments_16bit_size = 0;
uint16 *gw_segments_16bit;

/*** Basic Pixel functions ***/
/*
  RGB multiplicative blending between background and segment 
  bg: background RGB565 16bits pixel format
  sg: segment is 8bits Green of RGB565 16 pixel format
*/

static inline uint16 rgb_multiply_8bits_non_byte_swap(uint32 bg, uint32 sg)
{

	// Separate each colors
	uint32 bg_r = (bg & GW_MASK_RGB565_R) >> 11;
	uint32 bg_g = (bg & GW_MASK_RGB565_G) >> 5;
	uint32 bg_b = (bg & GW_MASK_RGB565_B);

	// RGB multiply and normalize
	bg_r = (bg_r * sg) >> 8;
	bg_g = (bg_g * sg) >> 8;
	bg_b = (bg_b * sg) >> 8;

	// return in RGB565 format
	return (uint16)(bg_r << 11) | (bg_g << 5) | bg_b;
}

static inline uint16_t rgb_multiply_8bits_byte_swap(uint32_t bg, uint32_t sg)
{

	// swap bg back
	uint16_t unswapped_bg = __builtin_bswap16((uint16_t)bg);

	/* Separate each colors */
	uint32_t bg_r = (unswapped_bg & GW_MASK_RGB565_R) >> 11;
	uint32_t bg_g = (unswapped_bg & GW_MASK_RGB565_G) >> 5;
	uint32_t bg_b = (unswapped_bg & GW_MASK_RGB565_B);

	/* RGB multiply and normalize */
	bg_r = (bg_r * sg) >> 8;
	bg_g = (bg_g * sg) >> 8;
	bg_b = (bg_b * sg) >> 8;

	// return the pixel in byte swapped format
	return
		((bg_g & 0x07) << 13)	|	// G[2:0] -> bits 15:13
		((bg_b & 0x1F) << 8)	|	// B[4:0] -> bits 12:8
		((bg_r & 0x1F) << 3)	|	// R[4:0] -> bits 7:3
		((bg_g >> 3) & 0x07);		// G[5:3] -> bits 2:0

}

static inline uint16 rgb_multiply_8bits(uint32 bg, uint32 sg)
{
	if (BYTE_SWAP) {
		return rgb_multiply_8bits_byte_swap(bg, sg);
	}
	
	return rgb_multiply_8bits_non_byte_swap(bg, sg);
	
}

/* Preprocess segments into final 16 bit format */
void gw_preprocess_segments(void) {
	
	if (gw_head.flags & FLAG_RENDERING_LCD_INVERTED)
	{
		SEG_TRANSPARENT_COLOR = SEG_BLACK_COLOR;
		SEG_TRANSPARENT_COLOR_16_BIT = SEG_BLACK_COLOR_16_BIT;
	}
	else
	{
		SEG_TRANSPARENT_COLOR = SEG_WHITE_COLOR;
		SEG_TRANSPARENT_COLOR_16_BIT = SEG_WHITE_COLOR_16_BIT;
	}
	
	int seg_pixel_count = gw_head.segments_pixel_size / sizeof(unsigned char);

	if (gw_head.flags & FLAG_SEGMENTS_4BITS) {
		seg_pixel_count = seg_pixel_count * 2;
	}
	else if  (gw_head.flags & FLAG_SEGMENTS_2BITS) {
		seg_pixel_count = seg_pixel_count * 4;
	}

	gw_segments_16bit_size = seg_pixel_count * sizeof(uint16);
	gw_segments_16bit = malloc(gw_segments_16bit_size);
	size_t seg_count = gw_head.segments_width_size / sizeof(unsigned short);

	for (size_t s = 0; s < seg_count; s++) {
	
		unsigned short segment_width = gw_segments_width[s];
		unsigned short segment_height = gw_segments_height[s];
	
		if (segment_width > 0 && segment_height > 0) {
			
			unsigned short segment_x = gw_segments_x[s];
			unsigned short segment_y = gw_segments_y[s];
			unsigned int segment_offset = gw_segments_offset[s];
			
			uint32 segment = gw_segments_offset[s];
			uint8 cur_pixel = 0;
			uint8 cur_pixelr = 0;
			int idx = 0;
		
			if (gw_head.flags & FLAG_SEGMENTS_4BITS) {
				idx = segment & 0x1;
				segment = segment >> 1;
			}
			else if (gw_head.flags & FLAG_SEGMENTS_2BITS) {
				idx = segment & 0x3;
				segment = segment >> 2;
			}
			
			uint8 *pixel;
			pixel = &gw_segments[segment];
			
			for (int y = segment_y; y < segment_y + segment_height; y++) {
				
				for (int x = segment_x; x < segment_x + segment_width; x++) {
				
					if (gw_head.flags & FLAG_SEGMENTS_4BITS) {
						
						if ((idx & 0x1) == 0)
							cur_pixel = pixel[idx >> 1] & 0xF0;
						else
							cur_pixel = pixel[idx >> 1] << 4;

						cur_pixel |= cur_pixel >> 4;
					
					}
					else if (gw_head.flags & FLAG_SEGMENTS_2BITS) {
						cur_pixelr = (pixel[idx >> 2] >> 2*(idx & 0x3)) &0x3;
					}
					else {
						cur_pixel = pixel[idx];
					}
					
					idx++;
					
					if (gw_head.flags & FLAG_SEGMENTS_2BITS) {
						cur_pixel = cur_pixelr | cur_pixelr << 2 | cur_pixelr << 4 | cur_pixelr << 6;
					}

					if (cur_pixel != SEG_TRANSPARENT_COLOR) {
						if (gw_head.flags & FLAG_SEGMENTS_2BITS) {
							if ( cur_pixel == 0 ) cur_pixel = 39;
						}
						gw_segments_16bit[segment_offset] = rgb_multiply_8bits_non_byte_swap(gw_background[y * GW_SCREEN_WIDTH + x], cur_pixel);
					}
					else {
						gw_segments_16bit[segment_offset] = SEG_TRANSPARENT_COLOR_16_BIT;
					}

					segment_offset++;
				
				}
				
			}
			
		}
		
	}
	
}

void gw_byte_swap(void) {
	
	for (int i = 0; i < (gw_head.background_pixel_size / 2); i++) {
		gw_background[i] = __builtin_bswap16(gw_background[i]);
	}

	for (int i = 0; i < gw_segments_16bit_size / sizeof(uint16); i++) {
		gw_segments_16bit[i] = __builtin_bswap16(gw_segments_16bit[i]);
	}
	
}

/************************ Generic function to display a segment *****************/
void (*update_segment)(uint8 segment_nb, bool segment_state);
void (*original_update_segment)(uint8 segment_nb, bool segment_state);
/* Segment to framebuffer graphic function */
/* segments are stored from 0 to 255 in memory*/

/* Deviation 16bits segment resolution function */
__attribute__((optimize("unroll-loops"))) static inline void update_segment_16bits(uint8 segment_nb, bool segment_state)
{
	if (!segment_state) {
		return;
	}

	uint16_t *segment_ptr = &gw_segments_16bit[gw_segments_offset[segment_nb]];
	uint16_t *line_ptr = &gw_graphic_framebuffer[gw_segments_y[segment_nb] * GW_SCREEN_WIDTH + gw_segments_x[segment_nb]];

	int width  = gw_segments_width[segment_nb];
	int height = gw_segments_height[segment_nb];

	for (int h = 0; h < height; h++) {

		uint16_t *frame_ptr = line_ptr;

		for (int x = 0; x < width; x++) {

			uint16_t color = *segment_ptr++;

			if (color != SEG_TRANSPARENT_COLOR_16_BIT) {
				*frame_ptr = color;
			}

			frame_ptr++;
		}

		line_ptr += GW_SCREEN_WIDTH;
	}
}

/* Deviation 2bits segment resolution function */
__attribute__((optimize("unroll-loops"))) static inline void update_segment_2bits(uint8 segment_nb, bool segment_state)
{

	/* segment first pixel corner (up/left) */
	uint32 segment = gw_segments_offset[segment_nb];

	uint8 cur_pixel,cur_pixelr;
	int idx = 0;

	/* nothing to do for this segment */
	if (segment_state == 0)
		return;

	/* 4bits data size access */
	idx = segment & 0x3;
	segment = segment >> 2;

	uint8 *pixel;
	pixel = &gw_segments[segment];

	/* get segment coordinates */
	uint16 segments_x = gw_segments_x[segment_nb];
	uint16 segments_y = gw_segments_y[segment_nb];
	uint16 segments_width = gw_segments_width[segment_nb];
	uint16 segments_height = gw_segments_height[segment_nb];

	for (int line = segments_y; line < segments_height + segments_y; line++)
	{
		for (int x = segments_x; x < segments_width + segments_x; x++)
		{

			cur_pixelr = (pixel[idx >> 2] >> 2*(idx & 0x3)) &0x3;

			idx++;

			cur_pixel = cur_pixelr | cur_pixelr << 2 | cur_pixelr << 4 | cur_pixelr << 6;


			/* Check if there something to mix
			if the segment pixel is transparent nothing to do. */
			if (cur_pixel != SEG_TRANSPARENT_COLOR) {
			
			// change black color to get transparency effect
			if ( cur_pixel == 0 ) cur_pixel = 39;
			gw_graphic_framebuffer[line * GW_SCREEN_WIDTH + x] = rgb_multiply_8bits(source_mixer[line * GW_SCREEN_WIDTH + x], cur_pixel);
			}

		}
	}
}

/* Deviation 4bits segment resolution function */
__attribute__((optimize("unroll-loops"))) static inline void update_segment_4bits(uint8 segment_nb, bool segment_state)
{

	/* segment first pixel corner (up/left) */
	uint32 segment = gw_segments_offset[segment_nb];

	uint8 cur_pixel;
	int idx = 0;

	/* nothing to do for this segment */
	if (segment_state == 0)
		return;

	/* 4bits data size access */
	idx = segment & 0x1;
	segment = segment >> 1;

	uint8 *pixel;
	pixel = &gw_segments[segment];

	/* get segment coordinates */
	uint16 segments_x = gw_segments_x[segment_nb];
	uint16 segments_y = gw_segments_y[segment_nb];
	uint16 segments_width = gw_segments_width[segment_nb];
	uint16 segments_height = gw_segments_height[segment_nb];

	for (int line = segments_y; line < segments_height + segments_y; line++)
	{
		for (int x = segments_x; x < segments_width + segments_x; x++)
		{

			if ((idx & 0x1) == 0)
				cur_pixel = pixel[idx >> 1] & 0xF0;
			else
				cur_pixel = pixel[idx >> 1] << 4;

			cur_pixel |= cur_pixel >> 4;
			idx++;

			/* Check if there something to mix
			if the segment pixel is transparent nothing to do. */
			if (cur_pixel != SEG_TRANSPARENT_COLOR)
				gw_graphic_framebuffer[line * GW_SCREEN_WIDTH + x] = rgb_multiply_8bits(source_mixer[line * GW_SCREEN_WIDTH + x], cur_pixel);
		}
	}
}

/* Deviation 8bits segment resolution function */
__attribute__((optimize("unroll-loops"))) static inline void update_segment_8bits(uint8 segment_nb, bool segment_state)
{

	/* segment first pixel corner (up/left) */
	uint32 segment = gw_segments_offset[segment_nb];

	uint8 cur_pixel;
	int idx = 0;

	/* nothing to do for this segment */
	if (segment_state == 0)
		return;

	uint8 *pixel;
	pixel = &gw_segments[segment];

	/* get segment coordinates */
	uint16 segments_x = gw_segments_x[segment_nb];
	uint16 segments_y = gw_segments_y[segment_nb];
	uint16 segments_width = gw_segments_width[segment_nb];
	uint16 segments_height = gw_segments_height[segment_nb];

	for (int line = segments_y; line < segments_height + segments_y; line++)
	{
		for (int x = segments_x; x < segments_width + segments_x; x++)
		{

			cur_pixel = pixel[idx];
			idx++;

			/* Check if there something to mix
				if the segment pixel is black, it's considered transparent.
				it prevents from mixing 2 overlapping segments
				'gw_background' is the colour sheet. segment colour is white. */
			if (cur_pixel != SEG_TRANSPARENT_COLOR)
				gw_graphic_framebuffer[line * GW_SCREEN_WIDTH + x] = rgb_multiply_8bits(source_mixer[line * GW_SCREEN_WIDTH + x], cur_pixel);
		}
	}
}

/* Oil Panic processing 
 * 
 * The player is made up of multiple segments, and timing issues can cause incorrect segments to appear briefly during movement.
 * This code verifies each player segment before rendering and ensures that only the segments corresponding to the player's torso position are displayed.
 */

const uint8 oil_panic_segments[] = {40, 46, 50, 59, 123, 104, 112, 52, 100, 128, 60, 106, 114, 55, 44, 48, 56}; 

bool oil_panic_segment_values[] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}; 

bool oil_panic_is_process_segment(uint8 value)
{
	for (size_t i = 0; i < 17; i++) {
		if (oil_panic_segments[i] == value) {
			return true;
		}
	}
	return false;
}

void oil_panic_set_segment(uint8 segment, bool value)
{
	for (int i = 0; i < 17; i++) {
		if (oil_panic_segments[i] == segment) {
			oil_panic_segment_values[i] = value;
			return;
		}
	}
}

bool oil_panic_get_segment(uint8 segment)
{
	for (int i = 0; i < 17; i++) {
		if (oil_panic_segments[i] == segment) {
			return oil_panic_segment_values[i];
		}
	}
	return false;
}

bool oil_panic_any_segments_true(uint8 seg1, uint8 seg2, uint8 seg3)
{
	for (int i = 0; i < 17; i++) {
		uint8 seg = oil_panic_segments[i];

		if (seg == seg1 || seg == seg2 || seg == seg3) {
			if (oil_panic_segment_values[i] == true) {
				return true;
			}
		}
	}
	return false;
}

__attribute__((optimize("unroll-loops"))) static inline void oil_panic_update_segment(uint8 segment_nb, bool segment_state)
{
	if (!oil_panic_is_process_segment(segment_nb)) {
		original_update_segment(segment_nb, segment_state); // not player segment, render as normal
	}
	else {
		oil_panic_set_segment(segment_nb, segment_state);
	}
}

static inline void oil_panic_complete_processing()
{
	
	if (oil_panic_get_segment(40) && oil_panic_get_segment(123)) { // start up screen - show all
		for (int i = 0; i < 17; i++) {
			original_update_segment(oil_panic_segments[i], true);
		}
	}
	else {
	
		if (oil_panic_get_segment(40)) {
			original_update_segment(40, true); // torso left balcony 
		}
		else if (oil_panic_get_segment(46)) {
			original_update_segment(46, true); // torso left
			original_update_segment(104, true); // bucket left
			original_update_segment(100, oil_panic_any_segments_true(100, 128, 60)); // left bucket level top
			original_update_segment(106, oil_panic_any_segments_true(106, 114, 55)); // left bucket level middle
			original_update_segment(44, oil_panic_any_segments_true(44, 48, 56)); // left bucket level bottom
		}
		else if (oil_panic_get_segment(50)) {
			original_update_segment(50, true); // torso middle
			original_update_segment(112, true); // bucket middle
			original_update_segment(128, oil_panic_any_segments_true(100, 128, 60));  // middle bucket level top
			original_update_segment(114, oil_panic_any_segments_true(106, 114, 55));  // middle bucket level middle
			original_update_segment(48, oil_panic_any_segments_true(44, 48, 56)); // middle bucket level bottom
		}
		else if (oil_panic_get_segment(59)) {
			original_update_segment(59, true); // torso right
			original_update_segment(52, true); // bucket right
			original_update_segment(60, oil_panic_any_segments_true(100, 128, 60)); // right bucket level top
			original_update_segment(55, oil_panic_any_segments_true(106, 114, 55)); // right bucket level middle
			original_update_segment(56, oil_panic_any_segments_true(44, 48, 56)); // right bucket level bottom
		}
		else if (oil_panic_get_segment(123)) {
			original_update_segment(123, true); // torso right balcony
		}
	
	}
	
}

/* Oil Panic processing end */

/* Specific functions to pool segments status */

/* Flicker filter enable flag */
static bool deflicker_enabled = false;

/* SM510 RAM based LCD controller */
__attribute__((optimize("unroll-loops"))) inline void gw_gfx_sm510_rendering(uint16 *framebuffer)
{
	/*
#SM51X series: output to x.y.z, where:
	# x = group a/b/bs/c (0/1/2/3)
	# y = segment 1-16 (0-15)
	# z = common H1-H4 (0-3)
			
#segment position   = 64*x + 4*y + z

	CPU RAM display 
	0X50.. 0X5F : c1..c16
	0X60.. 0X6F : a1..a16
	0X70.. 0X7F : b1..b16
*/

	uint8 HxA, HxB, HxC; // RAM segment 4bits value H4..H1

	uint8 segment_position;
	uint8 segment_state;

	gw_graphic_framebuffer = framebuffer;

	if (gw_head.flags & FLAG_RENDERING_LCD_INVERTED)
	{
		memset(framebuffer, 0, GW_SCREEN_WIDTH * GW_SCREEN_HEIGHT * 2);
		SEG_TRANSPARENT_COLOR = SEG_BLACK_COLOR;
		SEG_TRANSPARENT_COLOR_16_BIT = SEG_BLACK_COLOR_16_BIT;
		source_mixer = gw_background;
	}
	else
	{
		memcpy(framebuffer, gw_background, GW_SCREEN_WIDTH * GW_SCREEN_HEIGHT * 2);
		SEG_TRANSPARENT_COLOR = SEG_WHITE_COLOR;
		SEG_TRANSPARENT_COLOR_16_BIT = SEG_WHITE_COLOR_16_BIT;
		source_mixer = framebuffer;
	}

	//scan group a1..a16,b1..b16,c11..c16
	for (int seg_y = 0; seg_y < NB_SEGS_ROW; seg_y++)
	{

		if (deflicker_enabled)
		{
			HxA = gw_ram_state[ADD_SEGA_BASE + seg_y] & 0xf;
			HxB = gw_ram_state[ADD_SEGB_BASE + seg_y] & 0xf;
			HxC = gw_ram_state[ADD_SEGC_BASE + seg_y] & 0xf;
		}
		else
		{
			HxA = gw_ram[ADD_SEGA_BASE + seg_y] & 0xf;
			HxB = gw_ram[ADD_SEGB_BASE + seg_y] & 0xf;
			HxC = gw_ram[ADD_SEGC_BASE + seg_y] & 0xf;
		}

		//scan H1..H4
		for (int seg_z = 0; seg_z < NB_SEGS_COL; seg_z++)
		{

			segment_position = 4 * seg_y + seg_z;

			//segment a
			segment_state = m_bc || !m_bp ? 0 : (HxA & (1 << seg_z)) != 0;
			update_segment(segment_position, segment_state);

			//segment b
			segment_state = m_bc || !m_bp ? 0 : (HxB & (1 << seg_z)) != 0;
			update_segment(segment_position + 64, segment_state);

			//segment c
			segment_state = m_bc || !m_bp ? 0 : (HxC & (1 << seg_z)) != 0;
			update_segment(segment_position + 192, segment_state);
		}
	}

	/* bs1,b2 output from L/X and Y regs */

	//scan H1..H4 BS segment
	for (int seg_z = 0; seg_z < NB_SEGS_COL; seg_z++)
	{

		/* Blinking mask using m_y and m_div as timer */
		uint8 blink = (m_div & 0x4000) ? m_y : 0;

		/* bs1 is derived from ml */
		uint8 seg = (m_l & ~blink);
		segment_state = (m_bc || !m_bp) ? 0 : seg;

		update_segment(128 + seg_z, ((segment_state & (1 << seg_z)) != 0));

		/* bs2 is derived from mx */
		seg = (m_x & ~blink);
		segment_state = (m_bc || !m_bp) ? 0 : seg;

		update_segment(132 + seg_z, ((segment_state & (1 << seg_z)) != 0));
	}
	
	if (process_oil_panic) {
		oil_panic_complete_processing();
	}

}

/* SM500 I/O based LCD controller */
__attribute__((optimize("unroll-loops"))) inline void gw_gfx_sm500_rendering(uint16 *framebuffer)
{
	/*
# SM500/SM5A series: output to x.y.z, where:
	# x = O group (0-*)
	# y = O segment 1-4 (0-3)
	# z = common H1/H2 (0/1)
			
#segment position  = 8*x + 2*y + z    
	 56 segments
	m_ox[9];  W' latch, max 9
	m_o[9];    W latch
*/
	uint8 seg;
	
	gw_graphic_framebuffer = framebuffer;

	if (gw_head.flags & FLAG_RENDERING_LCD_INVERTED)
	{
		memset(framebuffer, 0, GW_SCREEN_WIDTH * GW_SCREEN_HEIGHT * 2);
		SEG_TRANSPARENT_COLOR = SEG_BLACK_COLOR;
		SEG_TRANSPARENT_COLOR_16_BIT = SEG_BLACK_COLOR_16_BIT;
		source_mixer = gw_background;
	}
	else
	{
		memcpy(framebuffer, gw_background, GW_SCREEN_WIDTH * GW_SCREEN_HEIGHT * 2);
		SEG_TRANSPARENT_COLOR = SEG_WHITE_COLOR;
		SEG_TRANSPARENT_COLOR_16_BIT = SEG_WHITE_COLOR_16_BIT;
		source_mixer = framebuffer;
	}

	// 2 columns z
	for (int h = 0; h < 2; h++)
	{
		// number of W,W' output registers (m_o_pins= 7 or 9)
		for (int o = 0; o < m_o_pins; o++)
		{
			// 4 segments per group
			// check if LCD deflicker is activated
			if (deflicker_enabled)
				seg = h ? m_ox_state[o] : m_o_state[o];
			else
				seg = h ? m_ox[o] : m_o[o];

			// 8x+2y+z with x=o, y=2,4,6,8, z=h (72 segments max.)
			update_segment(8 * o + 0 + h, m_bp ? ((seg & 0x1) != 0) : 0); // 0,1 8,9 16,17 24,25 32,33 40,41 48,49 56,57 64,65
			update_segment(8 * o + 2 + h, m_bp ? ((seg & 0x2) != 0) : 0); // 2,3
			update_segment(8 * o + 4 + h, m_bp ? ((seg & 0x4) != 0) : 0); // 4,5
			update_segment(8 * o + 6 + h, m_bp ? ((seg & 0x8) != 0) : 0); // 6,7
		}
	}
	
}

void gw_gfx_init()
{

	/* init LCD deflicker level */
	// for emulated cpus side
	flag_lcd_deflicker_level = (gw_head.flags & FLAG_LCD_DEFLICKER_MASK) >> 6;

	// fix for some roms by changing flag_lcd_deflicker_level - printf("[%.8s]\n", gw_head.rom_signature);
	if IS_ROM("nw_tfish") {
		// change from value set in lcd game shrinker
		flag_lcd_deflicker_level = 0;
	}
	else if (IS_ROM("gnw_chef") || IS_ROM("_flagman")) {
		// change from value set in lcd game shrinker
		flag_lcd_deflicker_level = 1;
	}
	else if (IS_ROM("_octopus") || IS_ROM("gnw_ball")  || IS_ROM("nw_judge")
				|| IS_ROM("manholeg") || IS_ROM("w_helmet") || IS_ROM("gnw_lion")
				|| IS_ROM("w_pchute") || IS_ROM("w_mmouse") || IS_ROM(" gnw_egg")
				|| IS_ROM("gnw_fire")) {
		// flag level 2 rom that need to also update segments in sm500_op_trs subroutine
		flag_lcd_deflicker_level = 3;
	}
	else if IS_ROM("w_popeye") {
		// flag level 2 rom that need to also update segments in sm500_op_rtn1
		flag_lcd_deflicker_level = 4;
	}
	else if IS_ROM("nw_fires") {
		// flag level 2 rom that need to also update segments in sm500_op_atbp
		flag_lcd_deflicker_level = 5;
	}

	// for segments rendering side
	deflicker_enabled = (flag_lcd_deflicker_level != 0);
	
	if (IS_ROM("w_opanic")) {
		// special segment processing for oil panic
		process_oil_panic = true;
	}

	/* determine which API to use for segments rendering */
	
#if defined(MODEL_CARDPUTER_MULTI_SCREEN)

	update_segment = update_segment_8bits;

	if (gw_head.flags & FLAG_SEGMENTS_4BITS) {
		update_segment = update_segment_4bits;
	}
	
	if (gw_head.flags & FLAG_SEGMENTS_2BITS) {
		update_segment = update_segment_2bits;
	}
	
#else
	update_segment = update_segment_16bits;
#endif
	
	if (process_oil_panic) {
		original_update_segment = update_segment;
		update_segment = oil_panic_update_segment;
	}

}
