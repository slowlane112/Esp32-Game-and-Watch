#include <stdint.h>
#include "game.h"
#include "button.h"

Game games[] = {
    { "Ball", "1980", 232, 156, 2, 0xffff},
    { "Vermin", "1980", 232, 156, 2, 0x0000 },
    { "Fire", "1980", 232, 156, 2, 0xffff },
    { "Helmet", "1981", 232, 154, 2, 0xffff },
    { "Parachute", "1981", 256, 154, 2, 0xffff },
    { "Octopus", "1981", 256, 154, 2, 0xffff },
    { "Popeye", "1981", 256, 154, 2, 0xffff },
    { "Chef", "1981", 256, 154, 2, 0xffff },
    { "Fire", "1981", 256, 154, 2, 0xffff },
    { "Turtle Bridge", "1982", 256, 154, 2, 0x0000 },
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
        button_control_type = 2;
		button_control_mode = 0;        
    }
    else if (index == 1) {
        ROM_DATA = (unsigned char *)gnwvermin_gw_start;
        ROM_DATA_LENGTH = gnwvermin_gw_end - gnwvermin_gw_start;
        button_control_type = 2;
		button_control_mode = 0;
    }
    else if (index == 2) {
        ROM_DATA = (unsigned char *)gnwfiresilver_gw_start;
        ROM_DATA_LENGTH = gnwfiresilver_gw_end - gnwfiresilver_gw_start;
        button_control_type = 2;
		button_control_mode = 0;  
    }
    else if (index == 3) {
        ROM_DATA = (unsigned char *)gnwhelmet_cn17_gw_start;
        ROM_DATA_LENGTH = gnwhelmet_cn17_gw_end - gnwhelmet_cn17_gw_start;
        button_control_type = 2;
		button_control_mode = 0;   
    }
    else if (index == 4) {
        ROM_DATA = (unsigned char *)gnwparachute_gw_start;
        ROM_DATA_LENGTH = gnwparachute_gw_end - gnwparachute_gw_start;
        button_control_type = 2;
		button_control_mode = 0;
    }
    else if (index == 5) {
        ROM_DATA = (unsigned char *)gnwoctopus_gw_start;
        ROM_DATA_LENGTH = gnwoctopus_gw_end - gnwoctopus_gw_start;
        button_control_type = 2;
		button_control_mode = 0;
    }
    else if (index == 6) {
        ROM_DATA = (unsigned char *)gnwpopeye_wide_gw_start;
        ROM_DATA_LENGTH = gnwpopeye_wide_gw_end - gnwpopeye_wide_gw_start;
        button_control_type = 2;
		button_control_mode = 0;  
    }
    else if (index == 7) {
        ROM_DATA = (unsigned char *)gnwchef_gw_start;
        ROM_DATA_LENGTH = gnwchef_gw_end - gnwchef_gw_start;
        button_control_type = 2;
		button_control_mode = 0;
    }
    else if (index == 8) {
        ROM_DATA = (unsigned char *)gnwfirewidescreen_gw_start;
        ROM_DATA_LENGTH = gnwfirewidescreen_gw_end -gnwfirewidescreen_gw_start;
        button_control_type = 2;
		button_control_mode = 0;
    }
    else if (index == 9) {
        ROM_DATA = (unsigned char *)gnwturtlebridge_gw_start;
        ROM_DATA_LENGTH = gnwturtlebridge_gw_end - gnwturtlebridge_gw_start;
        button_control_type = 2;
		button_control_mode = 0;  
    }
    else if (index == 10) {
        ROM_DATA = (unsigned char *)gnwtropicalfish_gw_start;
        ROM_DATA_LENGTH = gnwtropicalfish_gw_end - gnwtropicalfish_gw_start;
		button_control_type = 2;
		button_control_mode = 1;
    }
    else if (index == 11) {
        ROM_DATA = (unsigned char *)gnwmariojuggler_gw_start;
        ROM_DATA_LENGTH = gnwmariojuggler_gw_end - gnwmariojuggler_gw_start;
        button_control_type = 2;
		button_control_mode = 0;  
    }

}
    
