#include <stdint.h>
#include "game.h"
#include "button.h"

Game games[] = {
    { "Oil Panic", "1982", 104, 129, 139, 79, 2, 0x0000, 0xef9d, 0xa4ac },
    { "Donkey Kong", "1982", 103, 129, 139, 79, 2, 0x0000, 0xf345, 0x9c8c },
    { "Mickey & Donald", "1982", 104, 129, 139, 79, 2, 0x0000, 0xd804, 0x9383 },
    { "Green House", "1982", 103, 129, 139, 79, 2, 0x0000, 0x03ac, 0xac6b },
    { "Donkey Kong II", "1983", 103, 129, 139, 79, 2, 0xffff, 0x6163, 0x9c2b },
    { "Pinball", "1983", 103, 129, 139, 79, 2, 0xffff, 0x0000, 0x93a8 },
    { "Black Jack", "1985", 103, 129, 139, 79, 2, 0xffff, 0x8883, 0x33d6 },
    { "Squish", "1986", 103, 129, 139, 79, 2, 0xffff, 0x6d14, 0xfde8 },
    { "Bomb Sweeper", "1987", 103, 129, 139, 79, 2, 0x04cd, 0xee4f, 0xb3e8 },
    { "Safe Buster", "1988", 103, 129, 139, 79, 2, 0x0000, 0xd532, 0xfde8 },
    { "Gold Cliff", "1988", 103, 129, 139, 79, 2, 0x0000, 0x6d78, 0xe5cb },
    { "Zelda", "1989", 103, 129, 137, 79, 2, 0xd126, 0xc449, 0xd126 }
};

int game_count = sizeof(games) / sizeof(games[0]);

unsigned char *ROM_DATA;
unsigned int ROM_DATA_LENGTH;

void game_load(int index)
{
	
	extern const uint8_t gnwoilpanic_gw_start[] asm("_binary_gnw_opanic_gw_start");
	extern const uint8_t gnwoilpanic_gw_end[]   asm("_binary_gnw_opanic_gw_end");
	
	extern const uint8_t gnwdonkeykong_gw_start[] asm("_binary_gnw_dkong_gw_start");
	extern const uint8_t gnwdonkeykong_gw_end[]   asm("_binary_gnw_dkong_gw_end");
	
	extern const uint8_t gnwmickeydonald_gw_start[] asm("_binary_gnw_mickdon_gw_start");
	extern const uint8_t gnwmickeydonald_gw_end[]   asm("_binary_gnw_mickdon_gw_end");
	
	extern const uint8_t gnwgreenhouse_gw_start[] asm("_binary_gnw_ghouse_gw_start");
	extern const uint8_t gnwgreenhouse_gw_end[]   asm("_binary_gnw_ghouse_gw_end");

	extern const uint8_t gnwdonkeykong2_gw_start[] asm("_binary_gnw_dkong2_gw_start");
	extern const uint8_t gnwdonkeykong2_gw_end[]   asm("_binary_gnw_dkong2_gw_end");
	
	extern const uint8_t gnwpinball_gw_start[] asm("_binary_gnw_pinball_gw_start");
	extern const uint8_t gnwpinball_gw_end[]   asm("_binary_gnw_pinball_gw_end");
	
	extern const uint8_t gnwblackjack_gw_start[] asm("_binary_gnw_bjack_gw_start");
	extern const uint8_t gnwblackjack_gw_end[]   asm("_binary_gnw_bjack_gw_end");

	extern const uint8_t gnwsquish_gw_start[] asm("_binary_gnw_squish_gw_start");
	extern const uint8_t gnwsquish_gw_end[]   asm("_binary_gnw_squish_gw_end");

	extern const uint8_t gnwsafebuster_gw_start[] asm("_binary_gnw_sbuster_gw_start");
	extern const uint8_t gnwsafebuster_gw_end[]   asm("_binary_gnw_sbuster_gw_end");

	extern const uint8_t gnwbombsweeper_gw_start[] asm("_binary_gnw_bsweep_gw_start");
	extern const uint8_t gnwbombsweeper_gw_end[]   asm("_binary_gnw_bsweep_gw_end");

	extern const uint8_t gnwgoldcliff_gw_start[] asm("_binary_gnw_gcliff_gw_start");
	extern const uint8_t gnwgoldcliff_gw_end[]   asm("_binary_gnw_gcliff_gw_end");

	extern const uint8_t zelda_gw_start[] asm("_binary_gnw_zelda_gw_start");
	extern const uint8_t zelda_gw_end[]   asm("_binary_gnw_zelda_gw_end");


	if (index == 0) {
        ROM_DATA = (unsigned char *)gnwoilpanic_gw_start;
        ROM_DATA_LENGTH = gnwoilpanic_gw_end - gnwoilpanic_gw_start;
		button_control_type = 2;
		button_control_mode = 0; 
    }
    else if (index == 1) {
        ROM_DATA = (unsigned char *)gnwdonkeykong_gw_start;
        ROM_DATA_LENGTH = gnwdonkeykong_gw_end - gnwdonkeykong_gw_start;
        button_control_type = 5;
		button_control_mode = 0; 
    }
    else if (index == 2) {
        ROM_DATA = (unsigned char *)gnwmickeydonald_gw_start;
        ROM_DATA_LENGTH = gnwmickeydonald_gw_end - gnwmickeydonald_gw_start;
		button_control_type = 3;
		button_control_mode = 0; 
    }
    else if (index == 3) {
        ROM_DATA = (unsigned char *)gnwgreenhouse_gw_start;
        ROM_DATA_LENGTH = gnwgreenhouse_gw_end - gnwgreenhouse_gw_start;
        button_control_type = 5;
		button_control_mode = 0; 
    }
    else if (index == 4) {
        ROM_DATA = (unsigned char *)gnwdonkeykong2_gw_start;
        ROM_DATA_LENGTH = gnwdonkeykong2_gw_end - gnwdonkeykong2_gw_start;
        button_control_type = 5;
		button_control_mode = 0; 
    }
    else if (index == 5) {
        ROM_DATA = (unsigned char *)gnwpinball_gw_start;
        ROM_DATA_LENGTH = gnwpinball_gw_end - gnwpinball_gw_start;
		button_control_type = 2;
		button_control_mode = 0; 
    }
    else if (index == 6) {
        ROM_DATA = (unsigned char *)gnwblackjack_gw_start;
        ROM_DATA_LENGTH = gnwblackjack_gw_end - gnwblackjack_gw_start;
		button_control_type = 4;
		button_control_mode = 0; 
    }
    else if (index == 7) {
        ROM_DATA = (unsigned char *)gnwsquish_gw_start;
        ROM_DATA_LENGTH = gnwsquish_gw_end - gnwsquish_gw_start;
		button_control_type = 3;
		button_control_mode = 1; 
    }
    else if (index == 8) {
        ROM_DATA = (unsigned char *)gnwbombsweeper_gw_start;
        ROM_DATA_LENGTH = gnwbombsweeper_gw_end - gnwbombsweeper_gw_start;
        button_control_type = 5;
		button_control_mode = 0; 
    }
    else if (index == 9) {
        ROM_DATA = (unsigned char *)gnwsafebuster_gw_start;
        ROM_DATA_LENGTH = gnwsafebuster_gw_end - gnwsafebuster_gw_start;
        button_control_type = 2;
		button_control_mode = 0; 
    }
    else if (index == 10) {
        ROM_DATA = (unsigned char *)gnwgoldcliff_gw_start;
        ROM_DATA_LENGTH = gnwgoldcliff_gw_end - gnwgoldcliff_gw_start;
        button_control_type = 5;
		button_control_mode = 0; 
    }
    else if (index == 11) {
        ROM_DATA = (unsigned char *)zelda_gw_start;
        ROM_DATA_LENGTH = zelda_gw_end - zelda_gw_start;
        button_control_type = 5;
		button_control_mode = 0; 
    }


}
    
