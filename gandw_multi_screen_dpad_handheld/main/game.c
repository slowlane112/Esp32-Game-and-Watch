#include <stdint.h>
#include "game.h"
#include "button.h"

Game games[] = {
    { "Donkey Kong", "1982", 160, 200, 270, 154, 2, 0x0000, 0xf345, 0x9c8c},
    { "Green House", "1982", 160, 200, 270, 154, 2, 0x0000, 0x03ac, 0xac6b },
    { "Donkey Kong II", "1983", 160, 200, 270, 154, 2, 0xffff, 0x6163, 0x9c2b},
    { "Bomb Sweeper", "1987", 160, 200, 271, 154, 2, 0x04cd, 0xee4f, 0xb3e8},
    { "Gold Cliff", "1988", 160, 200, 271, 154, 2, 0x0000, 0x6d78, 0xe5cb},
    { "Zelda", "1989", 160, 200, 268, 154, 2, 0xd126, 0xc449, 0xd126}
};

int game_count = sizeof(games) / sizeof(games[0]);

unsigned char *ROM_DATA;
unsigned int ROM_DATA_LENGTH;

void game_load(int index)
{

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

}
    
