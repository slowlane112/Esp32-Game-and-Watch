#include <stdint.h>
#include "game.h"
#include "button.h"

Game games[] = {
    { "Ball", "1980", 232, 156, 2, 0xffff},
    { "Flagman", "1980", 232, 156, 2, 0xffff },
    { "Vermin", "1980", 232, 156, 2, 0x0000 },
    { "Fire", "1980", 232, 156, 2, 0xffff },
    { "Judge", "1980", 232, 156, 2, 0xffff },
    { "Manhole", "1981", 232, 156, 2, 0xffff },
    { "Helmet", "1981", 232, 154, 2, 0xffff },
    { "Lion", "1981", 232, 156, 2, 0xffff },
    { "Parachute", "1981", 256, 154, 2, 0xffff },
    { "Octopus", "1981", 256, 154, 2, 0xffff },
    { "Popeye", "1981", 256, 154, 2, 0xffff },
    { "Chef", "1981", 256, 154, 2, 0xffff },
    { "Mickey Mouse", "1981", 256, 154, 2, 0xffff },
    { "Egg", "1981", 256, 154, 2, 0xffff },
    { "Fire", "1981", 256, 154, 2, 0xffff },
    { "Turtle Bridge", "1982", 256, 154, 2, 0x0000 },
    { "Fire Attack", "1982", 256, 154, 2, 0xffff },
    { "Snoopy Tennis", "1982", 256, 154, 2, 0x0000 },
    { "Mario's Cement Factory", "1983", 256, 154, 10, 0xffff },
    { "Manhole", "1983", 256, 154, 10, 0xffff },
    { "Tropical Fish", "1985", 256, 154, 10, 0xffff },
    { "Mario the Juggler", "1991", 256, 155, 10, 0x0000 },  
    
};

int game_count = sizeof(games) / sizeof(games[0]);

unsigned char *ROM_DATA;
unsigned int ROM_DATA_LENGTH;

void game_load(int index)
{

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

    if (index == 0) {
        ROM_DATA = (unsigned char *)gnwball_gw_start;
        ROM_DATA_LENGTH = gnwball_gw_end - gnwball_gw_start;
        button_control_type = 2;
		button_control_mode = 0;       
    }
    else if (index == 1) { 
        ROM_DATA = (unsigned char *)gnwflagman_gw_start;
        ROM_DATA_LENGTH = gnwflagman_gw_end - gnwflagman_gw_start;
		button_control_type = 4;
		button_control_mode = 3;
    }
    else if (index == 2) {
        ROM_DATA = (unsigned char *)gnwvermin_gw_start;
        ROM_DATA_LENGTH = gnwvermin_gw_end - gnwvermin_gw_start;
        button_control_type = 2;
		button_control_mode = 0;
    }
    else if (index == 3) {
        ROM_DATA = (unsigned char *)gnwfiresilver_gw_start;
        ROM_DATA_LENGTH = gnwfiresilver_gw_end - gnwfiresilver_gw_start;
        button_control_type = 2;
		button_control_mode = 0;  
    }
    else if (index == 4) {
        ROM_DATA = (unsigned char *)gnwjudge_green_gw_start;
        ROM_DATA_LENGTH = gnwjudge_green_gw_end - gnwjudge_green_gw_start;
		button_control_type = 4;
		button_control_mode = 0;     
    }
    else if (index == 5) {
        ROM_DATA = (unsigned char *)gnwmanhole_gold_gw_start;
        ROM_DATA_LENGTH = gnwmanhole_gold_gw_end - gnwmanhole_gold_gw_start;
		button_control_type = 4;
		button_control_mode = 0;  
    }
    else if (index == 6) {
        ROM_DATA = (unsigned char *)gnwhelmet_cn17_gw_start;
        ROM_DATA_LENGTH = gnwhelmet_cn17_gw_end - gnwhelmet_cn17_gw_start;
        button_control_type = 2;
		button_control_mode = 0;   
    }
    else if (index == 7) {
        ROM_DATA = (unsigned char *)gnwlion_gw_start;
        ROM_DATA_LENGTH = gnwlion_gw_end - gnwlion_gw_start;
		button_control_type = 4;
		button_control_mode = 3;
    }
    else if (index == 8) {
        ROM_DATA = (unsigned char *)gnwparachute_gw_start;
        ROM_DATA_LENGTH = gnwparachute_gw_end - gnwparachute_gw_start;
        button_control_type = 2;
		button_control_mode = 0;
    }
    else if (index == 9) {
        ROM_DATA = (unsigned char *)gnwoctopus_gw_start;
        ROM_DATA_LENGTH = gnwoctopus_gw_end - gnwoctopus_gw_start;
        button_control_type = 2;
		button_control_mode = 0;
    }
    else if (index == 10) {
        ROM_DATA = (unsigned char *)gnwpopeye_wide_gw_start;
        ROM_DATA_LENGTH = gnwpopeye_wide_gw_end - gnwpopeye_wide_gw_start;
        button_control_type = 2;
		button_control_mode = 0;  
    }
    else if (index == 11) {
        ROM_DATA = (unsigned char *)gnwchef_gw_start;
        ROM_DATA_LENGTH = gnwchef_gw_end - gnwchef_gw_start;
        button_control_type = 2;
		button_control_mode = 0;
    }
    else if (index == 12) {
        ROM_DATA = (unsigned char *)gnwmickey_wide_gw_start;
        ROM_DATA_LENGTH = gnwmickey_wide_gw_end - gnwmickey_wide_gw_start;
		button_control_type = 4;
		button_control_mode = 0;
    }
    else if (index == 13) {
        ROM_DATA = (unsigned char *)gnwegg_gw_start;
        ROM_DATA_LENGTH = gnwegg_gw_end - gnwegg_gw_start;
		button_control_type = 4;
		button_control_mode = 0;
    }
    else if (index == 14) {
        ROM_DATA = (unsigned char *)gnwfirewidescreen_gw_start;
        ROM_DATA_LENGTH = gnwfirewidescreen_gw_end -gnwfirewidescreen_gw_start;
        button_control_type = 2;
		button_control_mode = 0;
    }
    else if (index == 15) {
        ROM_DATA = (unsigned char *)gnwturtlebridge_gw_start;
        ROM_DATA_LENGTH = gnwturtlebridge_gw_end - gnwturtlebridge_gw_start;
        button_control_type = 2;
		button_control_mode = 0;  
    }
    else if (index == 16) {
        ROM_DATA = (unsigned char *)gnwfireattack_gw_start;
        ROM_DATA_LENGTH = gnwfireattack_gw_end - gnwfireattack_gw_start;
		button_control_type = 4;
		button_control_mode = 0;
    }
    else if (index == 17) {
        ROM_DATA = (unsigned char *)gnwsnoopytennis_gw_start;
        ROM_DATA_LENGTH = gnwsnoopytennis_gw_end - gnwsnoopytennis_gw_start;
		button_control_type = 4;
		button_control_mode = 1;
    }
    else if (index == 18) {
        ROM_DATA = (unsigned char *)gnwmario_cement_wide_gw_start;
        ROM_DATA_LENGTH = gnwmario_cement_wide_gw_end - gnwmario_cement_wide_gw_start;
		button_control_type = 4;
		button_control_mode = 2; 
    }
    else if (index == 19) {
        ROM_DATA = (unsigned char *)gnwmanhole_wide_gw_start;
        ROM_DATA_LENGTH = gnwmanhole_wide_gw_end - gnwmanhole_wide_gw_start;
		button_control_type = 4;
		button_control_mode = 0; 
    }
    else if (index == 20) {
        ROM_DATA = (unsigned char *)gnwtropicalfish_gw_start;
        ROM_DATA_LENGTH = gnwtropicalfish_gw_end - gnwtropicalfish_gw_start;
		button_control_type = 2;
		button_control_mode = 1;
    }
    else if (index == 21) {
        ROM_DATA = (unsigned char *)gnwmariojuggler_gw_start;
        ROM_DATA_LENGTH = gnwmariojuggler_gw_end - gnwmariojuggler_gw_start;
        button_control_type = 2;
		button_control_mode = 0;  
    }

}
    
