#include <stdint.h>
#include "game.h"
#include "button.h"

Game games[] = {
    { "Oil Panic", "1982", 160, 200, 270, 154, 2, 0x0000, 0xef9d, 0xa4ac},
    { "Pinball", "1983", 160, 200, 270, 154, 2, 0xffff, 0x0000, 0x93a8 },
    { "Safe Buster", "1988", 160, 200, 271, 154, 2, 0x0000, 0xd532, 0xfde8}
};

int game_count = sizeof(games) / sizeof(games[0]);

unsigned char *ROM_DATA;
unsigned int ROM_DATA_LENGTH;

void game_load(int index)
{

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

}
    
