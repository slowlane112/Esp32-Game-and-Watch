#include <stdint.h>
#include "game.h"
#include "button.h"
#include "nvs.h"

Game games[] = {
	
#if defined(MODEL_SINGLE_SCREEN_2_BUTTON)

	{ .name="Ball", .year="1980", .img_unit_width=232, .img_unit_height=156, .color_line=2, .text_color=0xffff },
	{ .name="Vermin", .year="1980", .img_unit_width=232, .img_unit_height=156, .color_line=2, .text_color=0x0000 },
	{ .name="Fire", .year="1980", .img_unit_width=232, .img_unit_height=156, .color_line=2, .text_color=0xffff },
	{ .name="Helmet", .year="1981", .img_unit_width=232, .img_unit_height=154, .color_line=2, .text_color=0xffff },
	{ .name="Parachute", .year="1981", .img_unit_width=256, .img_unit_height=154, .color_line=2, .text_color=0xffff },
	{ .name="Octopus", .year="1981", .img_unit_width=256, .img_unit_height=154, .color_line=2, .text_color=0xffff },
	{ .name="Popeye", .year="1981", .img_unit_width=256, .img_unit_height=154, .color_line=2, .text_color=0xffff },
	{ .name="Chef", .year="1981", .img_unit_width=256, .img_unit_height=154, .color_line=2, .text_color=0xffff },
	{ .name="Fire", .year="1981", .img_unit_width=256, .img_unit_height=154, .color_line=2, .text_color=0xffff },
	{ .name="Turtle Bridge", .year="1982", .img_unit_width=256, .img_unit_height=154, .color_line=2, .text_color=0x0000 },
	{ .name="Tropical Fish", .year="1985", .img_unit_width=256, .img_unit_height=154, .color_line=10, .text_color=0xffff },
	{ .name="Mario the Juggler", .year="1991", .img_unit_width=256, .img_unit_height=155, .color_line=10, .text_color=0x0000 }
	
#elif defined(MODEL_SINGLE_SCREEN_4_BUTTON)

	{ .name="Ball", .year="1980", .img_unit_width=232, .img_unit_height=156, .color_line=2, .text_color=0xffff },
	{ .name="Flagman", .year="1980", .img_unit_width=232, .img_unit_height=156, .color_line=2, .text_color=0xffff },
	{ .name="Vermin", .year="1980", .img_unit_width=232, .img_unit_height=156, .color_line=2, .text_color=0x0000 },
	{ .name="Fire", .year="1980", .img_unit_width=232, .img_unit_height=156, .color_line=2, .text_color=0xffff },
	{ .name="Judge", .year="1980", .img_unit_width=232, .img_unit_height=156, .color_line=2, .text_color=0xffff },
	{ .name="Manhole", .year="1981", .img_unit_width=232, .img_unit_height=156, .color_line=2, .text_color=0xffff },
	{ .name="Helmet", .year="1981", .img_unit_width=232, .img_unit_height=154, .color_line=2, .text_color=0xffff },
	{ .name="Lion", .year="1981", .img_unit_width=232, .img_unit_height=156, .color_line=2, .text_color=0xffff },
	{ .name="Parachute", .year="1981", .img_unit_width=256, .img_unit_height=154, .color_line=2, .text_color=0xffff },
	{ .name="Octopus", .year="1981", .img_unit_width=256, .img_unit_height=154, .color_line=2, .text_color=0xffff },
	{ .name="Popeye", .year="1981", .img_unit_width=256, .img_unit_height=154, .color_line=2, .text_color=0xffff },
	{ .name="Chef", .year="1981", .img_unit_width=256, .img_unit_height=154, .color_line=2, .text_color=0xffff },
	{ .name="Mickey Mouse", .year="1981", .img_unit_width=256, .img_unit_height=154, .color_line=2, .text_color=0xffff },
	{ .name="Egg", .year="1981", .img_unit_width=256, .img_unit_height=154, .color_line=2, .text_color=0xffff },
	{ .name="Fire", .year="1981", .img_unit_width=256, .img_unit_height=154, .color_line=2, .text_color=0xffff },
	{ .name="Turtle Bridge", .year="1982", .img_unit_width=256, .img_unit_height=154, .color_line=2, .text_color=0x0000 },
	{ .name="Fire Attack", .year="1982", .img_unit_width=256, .img_unit_height=154, .color_line=2, .text_color=0xffff },
	{ .name="Snoopy Tennis", .year="1982", .img_unit_width=256, .img_unit_height=154, .color_line=2, .text_color=0x0000 },
	{ .name="Mario's Cement Factory", .year="1983", .img_unit_width=256, .img_unit_height=154, .color_line=10, .text_color=0xffff },
	{ .name="Manhole", .year="1983", .img_unit_width=256, .img_unit_height=154, .color_line=10, .text_color=0xffff },
	{ .name="Tropical Fish", .year="1985", .img_unit_width=256, .img_unit_height=154, .color_line=10, .text_color=0xffff },
	{ .name="Mario the Juggler", .year="1991", .img_unit_width=256, .img_unit_height=155, .color_line=10, .text_color=0x0000 }

#elif defined(MODEL_MULTI_SCREEN_2_BUTTON)

	{ .name="Oil Panic", .year="1982", .img_unit_width=160, .img_unit_height=200, .img_box_width=270, .img_box_height=154, .color_line=2, .text_color=0x0000, .box_bg_color=0xef9d, .box_border_color=0xa4ac },
	{ .name="Pinball", .year="1983", .img_unit_width=160, .img_unit_height=200, .img_box_width=270, .img_box_height=154, .color_line=2, .text_color=0xffff, .box_bg_color=0x0000, .box_border_color=0x93a8 },
	{ .name="Safe Buster", .year="1988", .img_unit_width=160, .img_unit_height=200, .img_box_width=271, .img_box_height=154, .color_line=2, .text_color=0x0000, .box_bg_color=0xd532, .box_border_color=0xfde8 }

#elif defined(MODEL_MULTI_SCREEN_DPAD)    

	{ .name="Donkey Kong", .year="1982", .img_unit_width=160, .img_unit_height=200, .img_box_width=270, .img_box_height=154, .color_line=2, .text_color=0x0000, .box_bg_color=0xf345, .box_border_color=0x9c8c },
	{ .name="Green House", .year="1982", .img_unit_width=160, .img_unit_height=200, .img_box_width=270, .img_box_height=154, .color_line=2, .text_color=0x0000, .box_bg_color=0x03ac, .box_border_color=0xac6b },
	{ .name="Donkey Kong II", .year="1983", .img_unit_width=160, .img_unit_height=200, .img_box_width=270, .img_box_height=154, .color_line=2, .text_color=0xffff, .box_bg_color=0x6163, .box_border_color=0x9c2b },
	{ .name="Bomb Sweeper", .year="1987", .img_unit_width=160, .img_unit_height=200, .img_box_width=271, .img_box_height=154, .color_line=2, .text_color=0x04cd, .box_bg_color=0xee4f, .box_border_color=0xb3e8 },
	{ .name="Gold Cliff", .year="1988", .img_unit_width=160, .img_unit_height=200, .img_box_width=271, .img_box_height=154, .color_line=2, .text_color=0x0000, .box_bg_color=0x6d78, .box_border_color=0xe5cb },
	{ .name="Zelda", .year="1989", .img_unit_width=160, .img_unit_height=200, .img_box_width=268, .img_box_height=154, .color_line=2, .text_color=0xd126, .box_bg_color=0xc449, .box_border_color=0xd126 }

#endif

};

int game_count = sizeof(games) / sizeof(games[0]);

unsigned char *ROM_DATA;
unsigned int ROM_DATA_LENGTH;

void game_load(int index)
{

	button_control_type = BUTTON_CONTROL_TYPE_NONE;
	button_control_mode = BUTTON_CONTROL_MODE_NONE; 

#if defined(MODEL_SINGLE_SCREEN_2_BUTTON)

	extern const uint8_t gnwball_gw_start[] asm("_binary_Game___Watch_Ball_gw_start");
	extern const uint8_t gnwball_gw_end[]   asm("_binary_Game___Watch_Ball_gw_end");

	extern const uint8_t gnwvermin_gw_start[] asm("_binary_Game___Watch_Vermin_gw_start");
	extern const uint8_t gnwvermin_gw_end[]   asm("_binary_Game___Watch_Vermin_gw_end");

	extern const uint8_t gnwfiresilver_gw_start[] asm("_binary_Game___Watch_Fire__Silver__gw_start");
	extern const uint8_t gnwfiresilver_gw_end[]   asm("_binary_Game___Watch_Fire__Silver__gw_end");

	extern const uint8_t gnwhelmet_cn17_gw_start[] asm("_binary_Game___Watch_Helmet__CN_17_version__gw_start");
	extern const uint8_t gnwhelmet_cn17_gw_end[]   asm("_binary_Game___Watch_Helmet__CN_17_version__gw_end");

	extern const uint8_t gnwparachute_gw_start[] asm("_binary_Game___Watch_Parachute_gw_start");
	extern const uint8_t gnwparachute_gw_end[]   asm("_binary_Game___Watch_Parachute_gw_end");

	extern const uint8_t gnwoctopus_gw_start[] asm("_binary_Game___Watch_Octopus_gw_start");
	extern const uint8_t gnwoctopus_gw_end[]   asm("_binary_Game___Watch_Octopus_gw_end");

	extern const uint8_t gnwpopeye_wide_gw_start[] asm("_binary_Game___Watch_Popeye__Wide_Screen__gw_start");
	extern const uint8_t gnwpopeye_wide_gw_end[]   asm("_binary_Game___Watch_Popeye__Wide_Screen__gw_end");

	extern const uint8_t gnwchef_gw_start[] asm("_binary_Game___Watch_Chef_gw_start");
	extern const uint8_t gnwchef_gw_end[]   asm("_binary_Game___Watch_Chef_gw_end");

	extern const uint8_t gnwfirewidescreen_gw_start[] asm("_binary_Game___Watch_Fire__Wide_Screen__gw_start");
	extern const uint8_t gnwfirewidescreen_gw_end[]   asm("_binary_Game___Watch_Fire__Wide_Screen__gw_end");

	extern const uint8_t gnwturtlebridge_gw_start[] asm("_binary_Game___Watch_Turtle_Bridge_gw_start");
	extern const uint8_t gnwturtlebridge_gw_end[]   asm("_binary_Game___Watch_Turtle_Bridge_gw_end");

	extern const uint8_t gnwtropicalfish_gw_start[] asm("_binary_Game___Watch_Tropical_Fish_gw_start");
	extern const uint8_t gnwtropicalfish_gw_end[]   asm("_binary_Game___Watch_Tropical_Fish_gw_end");

	extern const uint8_t gnwmariojuggler_gw_start[] asm("_binary_Game___Watch_Mario_The_Juggler_gw_start");
	extern const uint8_t gnwmariojuggler_gw_end[]   asm("_binary_Game___Watch_Mario_The_Juggler_gw_end");

	if (index == 0) {
		ROM_DATA = (unsigned char *)gnwball_gw_start;
		ROM_DATA_LENGTH = gnwball_gw_end - gnwball_gw_start;
	}
	else if (index == 1) {
		ROM_DATA = (unsigned char *)gnwvermin_gw_start;
		ROM_DATA_LENGTH = gnwvermin_gw_end - gnwvermin_gw_start;
	}
	else if (index == 2) {
		ROM_DATA = (unsigned char *)gnwfiresilver_gw_start;
		ROM_DATA_LENGTH = gnwfiresilver_gw_end - gnwfiresilver_gw_start;
	}
	else if (index == 3) {
		ROM_DATA = (unsigned char *)gnwhelmet_cn17_gw_start;
		ROM_DATA_LENGTH = gnwhelmet_cn17_gw_end - gnwhelmet_cn17_gw_start;
	}
	else if (index == 4) {
		ROM_DATA = (unsigned char *)gnwparachute_gw_start;
		ROM_DATA_LENGTH = gnwparachute_gw_end - gnwparachute_gw_start;
	}
	else if (index == 5) {
		ROM_DATA = (unsigned char *)gnwoctopus_gw_start;
		ROM_DATA_LENGTH = gnwoctopus_gw_end - gnwoctopus_gw_start;
	}
	else if (index == 6) {
		ROM_DATA = (unsigned char *)gnwpopeye_wide_gw_start;
		ROM_DATA_LENGTH = gnwpopeye_wide_gw_end - gnwpopeye_wide_gw_start;
	}
	else if (index == 7) {
		ROM_DATA = (unsigned char *)gnwchef_gw_start;
		ROM_DATA_LENGTH = gnwchef_gw_end - gnwchef_gw_start;
	}
	else if (index == 8) {
		ROM_DATA = (unsigned char *)gnwfirewidescreen_gw_start;
		ROM_DATA_LENGTH = gnwfirewidescreen_gw_end -gnwfirewidescreen_gw_start;
	}
	else if (index == 9) {
		ROM_DATA = (unsigned char *)gnwturtlebridge_gw_start;
		ROM_DATA_LENGTH = gnwturtlebridge_gw_end - gnwturtlebridge_gw_start;
	}
	else if (index == 10) {
		ROM_DATA = (unsigned char *)gnwtropicalfish_gw_start;
		ROM_DATA_LENGTH = gnwtropicalfish_gw_end - gnwtropicalfish_gw_start;
		button_control_mode = BUTTON_CONTROL_MODE_TROPICAL_FISH;
	}
	else if (index == 11) {
		ROM_DATA = (unsigned char *)gnwmariojuggler_gw_start;
		ROM_DATA_LENGTH = gnwmariojuggler_gw_end - gnwmariojuggler_gw_start;
	}

#elif defined(MODEL_SINGLE_SCREEN_4_BUTTON)

	extern const uint8_t gnwball_gw_start[] asm("_binary_Game___Watch_Ball_gw_start");
	extern const uint8_t gnwball_gw_end[]   asm("_binary_Game___Watch_Ball_gw_end");

	extern const uint8_t gnwflagman_gw_start[] asm("_binary_Game___Watch_Flagman_gw_start");
	extern const uint8_t gnwflagman_gw_end[]   asm("_binary_Game___Watch_Flagman_gw_end");

	extern const uint8_t gnwvermin_gw_start[] asm("_binary_Game___Watch_Vermin_gw_start");
	extern const uint8_t gnwvermin_gw_end[]   asm("_binary_Game___Watch_Vermin_gw_end");

	extern const uint8_t gnwfiresilver_gw_start[] asm("_binary_Game___Watch_Fire__Silver__gw_start");
	extern const uint8_t gnwfiresilver_gw_end[]   asm("_binary_Game___Watch_Fire__Silver__gw_end");

	extern const uint8_t gnwjudge_green_gw_start[] asm("_binary_Game___Watch_Judge__green_version__gw_start");
	extern const uint8_t gnwjudge_green_gw_end[]   asm("_binary_Game___Watch_Judge__green_version__gw_end");

	extern const uint8_t gnwmanhole_gold_gw_start[] asm("_binary_Game___Watch_Manhole__Gold__gw_start");
	extern const uint8_t gnwmanhole_gold_gw_end[]   asm("_binary_Game___Watch_Manhole__Gold__gw_end");

	extern const uint8_t gnwhelmet_cn17_gw_start[] asm("_binary_Game___Watch_Helmet__CN_17_version__gw_start");
	extern const uint8_t gnwhelmet_cn17_gw_end[]   asm("_binary_Game___Watch_Helmet__CN_17_version__gw_end");

	extern const uint8_t gnwlion_gw_start[] asm("_binary_Game___Watch_Lion_gw_start");
	extern const uint8_t gnwlion_gw_end[]   asm("_binary_Game___Watch_Lion_gw_end");

	extern const uint8_t gnwparachute_gw_start[] asm("_binary_Game___Watch_Parachute_gw_start");
	extern const uint8_t gnwparachute_gw_end[]   asm("_binary_Game___Watch_Parachute_gw_end");

	extern const uint8_t gnwoctopus_gw_start[] asm("_binary_Game___Watch_Octopus_gw_start");
	extern const uint8_t gnwoctopus_gw_end[]   asm("_binary_Game___Watch_Octopus_gw_end");

	extern const uint8_t gnwpopeye_wide_gw_start[] asm("_binary_Game___Watch_Popeye__Wide_Screen__gw_start");
	extern const uint8_t gnwpopeye_wide_gw_end[]   asm("_binary_Game___Watch_Popeye__Wide_Screen__gw_end");

	extern const uint8_t gnwchef_gw_start[] asm("_binary_Game___Watch_Chef_gw_start");
	extern const uint8_t gnwchef_gw_end[]   asm("_binary_Game___Watch_Chef_gw_end");

	extern const uint8_t gnwmickey_wide_gw_start[] asm("_binary_Game___Watch_Mickey_Mouse__Wide_Screen__gw_start");
	extern const uint8_t gnwmickey_wide_gw_end[]   asm("_binary_Game___Watch_Mickey_Mouse__Wide_Screen__gw_end");

	extern const uint8_t gnwegg_gw_start[] asm("_binary_Game___Watch_Egg_gw_start");
	extern const uint8_t gnwegg_gw_end[]   asm("_binary_Game___Watch_Egg_gw_end");

	extern const uint8_t gnwfirewidescreen_gw_start[] asm("_binary_Game___Watch_Fire__Wide_Screen__gw_start");
	extern const uint8_t gnwfirewidescreen_gw_end[]   asm("_binary_Game___Watch_Fire__Wide_Screen__gw_end");

	extern const uint8_t gnwturtlebridge_gw_start[] asm("_binary_Game___Watch_Turtle_Bridge_gw_start");
	extern const uint8_t gnwturtlebridge_gw_end[]   asm("_binary_Game___Watch_Turtle_Bridge_gw_end");

	extern const uint8_t gnwfireattack_gw_start[] asm("_binary_Game___Watch_Fire_Attack_gw_start");
	extern const uint8_t gnwfireattack_gw_end[]   asm("_binary_Game___Watch_Fire_Attack_gw_end");

	extern const uint8_t gnwsnoopytennis_gw_start[] asm("_binary_Game___Watch_Snoopy_Tennis_gw_start");
	extern const uint8_t gnwsnoopytennis_gw_end[]   asm("_binary_Game___Watch_Snoopy_Tennis_gw_end");

	extern const uint8_t gnwmario_cement_wide_gw_start[] asm("_binary_Game___Watch_Mario_s_Cement_Factory__New_Wide_Screen__gw_start");
	extern const uint8_t gnwmario_cement_wide_gw_end[]   asm("_binary_Game___Watch_Mario_s_Cement_Factory__New_Wide_Screen__gw_end");

	extern const uint8_t gnwmanhole_wide_gw_start[] asm("_binary_Game___Watch_Manhole__New_Wide_Screen__gw_start");
	extern const uint8_t gnwmanhole_wide_gw_end[]   asm("_binary_Game___Watch_Manhole__New_Wide_Screen__gw_end");

	extern const uint8_t gnwtropicalfish_gw_start[] asm("_binary_Game___Watch_Tropical_Fish_gw_start");
	extern const uint8_t gnwtropicalfish_gw_end[]   asm("_binary_Game___Watch_Tropical_Fish_gw_end");

	extern const uint8_t gnwmariojuggler_gw_start[] asm("_binary_Game___Watch_Mario_The_Juggler_gw_start");
	extern const uint8_t gnwmariojuggler_gw_end[]   asm("_binary_Game___Watch_Mario_The_Juggler_gw_end");

	button_control_type = BUTTON_CONTROL_TYPE_TWO_BUTTON;

	if (index == 0) {
		ROM_DATA = (unsigned char *)gnwball_gw_start;
		ROM_DATA_LENGTH = gnwball_gw_end - gnwball_gw_start;
	}
	else if (index == 1) { 
		ROM_DATA = (unsigned char *)gnwflagman_gw_start;
		ROM_DATA_LENGTH = gnwflagman_gw_end - gnwflagman_gw_start;
		button_control_type = BUTTON_CONTROL_TYPE_FOUR_BUTTON;
		button_control_mode = BUTTON_CONTROL_MODE_FLAGMAN_LION;
	}
	else if (index == 2) {
		ROM_DATA = (unsigned char *)gnwvermin_gw_start;
		ROM_DATA_LENGTH = gnwvermin_gw_end - gnwvermin_gw_start;
	}
	else if (index == 3) {
		ROM_DATA = (unsigned char *)gnwfiresilver_gw_start;
		ROM_DATA_LENGTH = gnwfiresilver_gw_end - gnwfiresilver_gw_start;
	}
	else if (index == 4) {
		ROM_DATA = (unsigned char *)gnwjudge_green_gw_start;
		ROM_DATA_LENGTH = gnwjudge_green_gw_end - gnwjudge_green_gw_start;
		button_control_type = BUTTON_CONTROL_TYPE_FOUR_BUTTON;
	}
	else if (index == 5) {
		ROM_DATA = (unsigned char *)gnwmanhole_gold_gw_start;
		ROM_DATA_LENGTH = gnwmanhole_gold_gw_end - gnwmanhole_gold_gw_start;
		button_control_type = BUTTON_CONTROL_TYPE_FOUR_BUTTON;
	}
	else if (index == 6) {
		ROM_DATA = (unsigned char *)gnwhelmet_cn17_gw_start;
		ROM_DATA_LENGTH = gnwhelmet_cn17_gw_end - gnwhelmet_cn17_gw_start;
	}
	else if (index == 7) {
		ROM_DATA = (unsigned char *)gnwlion_gw_start;
		ROM_DATA_LENGTH = gnwlion_gw_end - gnwlion_gw_start;
		button_control_type = BUTTON_CONTROL_TYPE_FOUR_BUTTON;
		button_control_mode = BUTTON_CONTROL_MODE_FLAGMAN_LION;
	}
	else if (index == 8) {
		ROM_DATA = (unsigned char *)gnwparachute_gw_start;
		ROM_DATA_LENGTH = gnwparachute_gw_end - gnwparachute_gw_start;
	}
	else if (index == 9) {
		ROM_DATA = (unsigned char *)gnwoctopus_gw_start;
		ROM_DATA_LENGTH = gnwoctopus_gw_end - gnwoctopus_gw_start;
	}
	else if (index == 10) {
		ROM_DATA = (unsigned char *)gnwpopeye_wide_gw_start;
		ROM_DATA_LENGTH = gnwpopeye_wide_gw_end - gnwpopeye_wide_gw_start;
	}
	else if (index == 11) {
		ROM_DATA = (unsigned char *)gnwchef_gw_start;
		ROM_DATA_LENGTH = gnwchef_gw_end - gnwchef_gw_start;
	}
	else if (index == 12) {
		ROM_DATA = (unsigned char *)gnwmickey_wide_gw_start;
		ROM_DATA_LENGTH = gnwmickey_wide_gw_end - gnwmickey_wide_gw_start;
		button_control_type = BUTTON_CONTROL_TYPE_FOUR_BUTTON;
	}
	else if (index == 13) {
		ROM_DATA = (unsigned char *)gnwegg_gw_start;
		ROM_DATA_LENGTH = gnwegg_gw_end - gnwegg_gw_start;
		button_control_type = BUTTON_CONTROL_TYPE_FOUR_BUTTON;
	}
	else if (index == 14) {
		ROM_DATA = (unsigned char *)gnwfirewidescreen_gw_start;
		ROM_DATA_LENGTH = gnwfirewidescreen_gw_end -gnwfirewidescreen_gw_start;
	}
	else if (index == 15) {
		ROM_DATA = (unsigned char *)gnwturtlebridge_gw_start;
		ROM_DATA_LENGTH = gnwturtlebridge_gw_end - gnwturtlebridge_gw_start;
	}
	else if (index == 16) {
		ROM_DATA = (unsigned char *)gnwfireattack_gw_start;
		ROM_DATA_LENGTH = gnwfireattack_gw_end - gnwfireattack_gw_start;
		button_control_type = BUTTON_CONTROL_TYPE_FOUR_BUTTON;
	}
	else if (index == 17) {
		ROM_DATA = (unsigned char *)gnwsnoopytennis_gw_start;
		ROM_DATA_LENGTH = gnwsnoopytennis_gw_end - gnwsnoopytennis_gw_start;
		button_control_type = BUTTON_CONTROL_TYPE_FOUR_BUTTON;
		button_control_mode = BUTTON_CONTROL_MODE_SNOOPY_TENNIS;
	}
	else if (index == 18) {
		ROM_DATA = (unsigned char *)gnwmario_cement_wide_gw_start;
		ROM_DATA_LENGTH = gnwmario_cement_wide_gw_end - gnwmario_cement_wide_gw_start;
		button_control_type = BUTTON_CONTROL_TYPE_FOUR_BUTTON;
		button_control_mode = BUTTON_CONTROL_MODE_MARIOS_CEMENT_FACTORY; 
	}
	else if (index == 19) {
		ROM_DATA = (unsigned char *)gnwmanhole_wide_gw_start;
		ROM_DATA_LENGTH = gnwmanhole_wide_gw_end - gnwmanhole_wide_gw_start;
		button_control_type = BUTTON_CONTROL_TYPE_FOUR_BUTTON;
	}
	else if (index == 20) {
		ROM_DATA = (unsigned char *)gnwtropicalfish_gw_start;
		ROM_DATA_LENGTH = gnwtropicalfish_gw_end - gnwtropicalfish_gw_start;
		button_control_mode = BUTTON_CONTROL_MODE_TROPICAL_FISH;
	}
	else if (index == 21) {
		ROM_DATA = (unsigned char *)gnwmariojuggler_gw_start;
		ROM_DATA_LENGTH = gnwmariojuggler_gw_end - gnwmariojuggler_gw_start;
	}
#elif defined(MODEL_MULTI_SCREEN_2_BUTTON)	
	extern const uint8_t gnwoilpanic_gw_start[] asm("_binary_Game___Watch_Oil_Panic_gw_start");
	extern const uint8_t gnwoilpanic_gw_end[]   asm("_binary_Game___Watch_Oil_Panic_gw_end");

	extern const uint8_t gnwpinball_gw_start[] asm("_binary_Game___Watch_Pinball_gw_start");
	extern const uint8_t gnwpinball_gw_end[]   asm("_binary_Game___Watch_Pinball_gw_end");

	extern const uint8_t gnwsafebuster_gw_start[] asm("_binary_Game___Watch_Safe_Buster_gw_start");
	extern const uint8_t gnwsafebuster_gw_end[]   asm("_binary_Game___Watch_Safe_Buster_gw_end");

	if (index == 0) {
		ROM_DATA = (unsigned char *)gnwoilpanic_gw_start;
		ROM_DATA_LENGTH = gnwoilpanic_gw_end - gnwoilpanic_gw_start;
	}
	else if (index == 1) {
		ROM_DATA = (unsigned char *)gnwpinball_gw_start;
		ROM_DATA_LENGTH = gnwpinball_gw_end - gnwpinball_gw_start;
	}
	else if (index == 2) {
		ROM_DATA = (unsigned char *)gnwsafebuster_gw_start;
		ROM_DATA_LENGTH = gnwsafebuster_gw_end - gnwsafebuster_gw_start;
	}
	
#elif defined(MODEL_MULTI_SCREEN_DPAD)
	
	extern const uint8_t gnwdonkeykong_gw_start[] asm("_binary_Game___Watch_Donkey_Kong_gw_start");
	extern const uint8_t gnwdonkeykong_gw_end[]   asm("_binary_Game___Watch_Donkey_Kong_gw_end");

	extern const uint8_t gnwgreenhouse_gw_start[] asm("_binary_Game___Watch_Green_House_gw_start");
	extern const uint8_t gnwgreenhouse_gw_end[]   asm("_binary_Game___Watch_Green_House_gw_end");

	extern const uint8_t gnwdonkeykong2_gw_start[] asm("_binary_Game___Watch_Donkey_Kong_II_gw_start");
	extern const uint8_t gnwdonkeykong2_gw_end[]   asm("_binary_Game___Watch_Donkey_Kong_II_gw_end");

	extern const uint8_t gnwbombsweeper_gw_start[] asm("_binary_Game___Watch_Bomb_Sweeper_gw_start");
	extern const uint8_t gnwbombsweeper_gw_end[]   asm("_binary_Game___Watch_Bomb_Sweeper_gw_end");

	extern const uint8_t gnwgoldcliff_gw_start[] asm("_binary_Game___Watch_Gold_Cliff_gw_start");
	extern const uint8_t gnwgoldcliff_gw_end[]   asm("_binary_Game___Watch_Gold_Cliff_gw_end");

	extern const uint8_t zelda_gw_start[] asm("_binary_Game___Watch_Zelda_gw_start");
	extern const uint8_t zelda_gw_end[]   asm("_binary_Game___Watch_Zelda_gw_end");

	if (index == 0) {
		ROM_DATA = (unsigned char *)gnwdonkeykong_gw_start;
		ROM_DATA_LENGTH = gnwdonkeykong_gw_end - gnwdonkeykong_gw_start;
	}
	else if (index == 1) {
		ROM_DATA = (unsigned char *)gnwgreenhouse_gw_start;
		ROM_DATA_LENGTH = gnwgreenhouse_gw_end - gnwgreenhouse_gw_start;
	}
	else if (index == 2) {
		ROM_DATA = (unsigned char *)gnwdonkeykong2_gw_start;
		ROM_DATA_LENGTH = gnwdonkeykong2_gw_end - gnwdonkeykong2_gw_start;
	}
	else if (index == 3) {
		ROM_DATA = (unsigned char *)gnwbombsweeper_gw_start;
		ROM_DATA_LENGTH = gnwbombsweeper_gw_end - gnwbombsweeper_gw_start;
	}
	else if (index == 4) {
		ROM_DATA = (unsigned char *)gnwgoldcliff_gw_start;
		ROM_DATA_LENGTH = gnwgoldcliff_gw_end - gnwgoldcliff_gw_start;
	}
	else if (index == 5) {
		ROM_DATA = (unsigned char *)zelda_gw_start;
		ROM_DATA_LENGTH = zelda_gw_end - zelda_gw_start;
	}
	
#endif

}

uint8_t game_get_selected(void) {
	uint8_t selected_game = nvs_get_value("selected_game", 0);
	if (selected_game > game_count - 1) {
		selected_game = 0;
	}
	return selected_game;
}

void game_save_selected(uint8_t selected_game) {
	nvs_set_value("selected_game", selected_game);
}
