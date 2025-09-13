#include <stdint.h>
#include "gandw_buttons.h"
#include "gandw_keyboard.h"
#include "gandw_games.h"
#include "esp_err.h"
#include "nvs_flash.h"
		
uint8_t start_delay = 127;
unsigned char *ROM_DATA;
unsigned int ROM_DATA_LENGTH;
		
uint8_t read_selected_game() {
    uint8_t selected_game = 1; // gnwparachute
    nvs_handle_t my_handle;
    ESP_ERROR_CHECK(nvs_open("gandw", NVS_READWRITE, &my_handle));
    nvs_get_u8(my_handle, "selected_game", &selected_game);
    nvs_close(my_handle);

    if (selected_game < 1 || selected_game > 26) {
        selected_game = 1;
    }

    return selected_game;
}

void save_selected_game(uint8_t selected_game) {
    nvs_handle_t my_handle;
    ESP_ERROR_CHECK(nvs_open("gandw", NVS_READWRITE, &my_handle));
    ESP_ERROR_CHECK(nvs_set_u8(my_handle, "selected_game", selected_game));
    ESP_ERROR_CHECK(nvs_commit(my_handle));
    nvs_close(my_handle);
}


void load_game() {

    char key = get_key();

	extern const uint8_t gnwball_gw_start[] asm("_binary_Game___Watch_Ball_gw_start");
	extern const uint8_t gnwball_gw_end[]   asm("_binary_Game___Watch_Ball_gw_end");

	extern const uint8_t gnwchef_gw_start[] asm("_binary_Game___Watch_Chef_gw_start");
	extern const uint8_t gnwchef_gw_end[]   asm("_binary_Game___Watch_Chef_gw_end");

	extern const uint8_t gnwegg_gw_start[] asm("_binary_Game___Watch_Egg_gw_start");
	extern const uint8_t gnwegg_gw_end[]   asm("_binary_Game___Watch_Egg_gw_end");

	extern const uint8_t gnwfireattack_gw_start[] asm("_binary_Game___Watch_Fire_Attack_gw_start");
	extern const uint8_t gnwfireattack_gw_end[]   asm("_binary_Game___Watch_Fire_Attack_gw_end");

	extern const uint8_t gnwfiresilver_gw_start[] asm("_binary_Game___Watch_Fire__Silver__gw_start");
	extern const uint8_t gnwfiresilver_gw_end[]   asm("_binary_Game___Watch_Fire__Silver__gw_end");

	extern const uint8_t gnwfirewidescreen_gw_start[] asm("_binary_Game___Watch_Fire__Wide_Screen__gw_start");
	extern const uint8_t gnwfirewidescreen_gw_end[]   asm("_binary_Game___Watch_Fire__Wide_Screen__gw_end");

	extern const uint8_t gnwflagman_gw_start[] asm("_binary_Game___Watch_Flagman_gw_start");
	extern const uint8_t gnwflagman_gw_end[]   asm("_binary_Game___Watch_Flagman_gw_end");

	extern const uint8_t gnwhelmet_cn17_gw_start[] asm("_binary_Game___Watch_Helmet__CN_17_version__gw_start");
	extern const uint8_t gnwhelmet_cn17_gw_end[]   asm("_binary_Game___Watch_Helmet__CN_17_version__gw_end");

	extern const uint8_t gnwjudge_green_gw_start[] asm("_binary_Game___Watch_Judge__green_version__gw_start");
	extern const uint8_t gnwjudge_green_gw_end[]   asm("_binary_Game___Watch_Judge__green_version__gw_end");

	extern const uint8_t gnwlion_gw_start[] asm("_binary_Game___Watch_Lion_gw_start");
	extern const uint8_t gnwlion_gw_end[]   asm("_binary_Game___Watch_Lion_gw_end");

	extern const uint8_t gnwmanhole_gold_gw_start[] asm("_binary_Game___Watch_Manhole__Gold__gw_start");
	extern const uint8_t gnwmanhole_gold_gw_end[]   asm("_binary_Game___Watch_Manhole__Gold__gw_end");

	extern const uint8_t gnwmanhole_wide_gw_start[] asm("_binary_Game___Watch_Manhole__New_Wide_Screen__gw_start");
	extern const uint8_t gnwmanhole_wide_gw_end[]   asm("_binary_Game___Watch_Manhole__New_Wide_Screen__gw_end");

	extern const uint8_t gnwmario_cement_wide_gw_start[] asm("_binary_Game___Watch_Mario_s_Cement_Factory__New_Wide_Screen__gw_start");
	extern const uint8_t gnwmario_cement_wide_gw_end[]   asm("_binary_Game___Watch_Mario_s_Cement_Factory__New_Wide_Screen__gw_end");

	extern const uint8_t gnwmariojuggler_gw_start[] asm("_binary_Game___Watch_Mario_The_Juggler_gw_start");
	extern const uint8_t gnwmariojuggler_gw_end[]   asm("_binary_Game___Watch_Mario_The_Juggler_gw_end");

	extern const uint8_t gnwmickey_wide_gw_start[] asm("_binary_Game___Watch_Mickey_Mouse__Wide_Screen__gw_start");
	extern const uint8_t gnwmickey_wide_gw_end[]   asm("_binary_Game___Watch_Mickey_Mouse__Wide_Screen__gw_end");

	extern const uint8_t gnwoctopus_gw_start[] asm("_binary_Game___Watch_Octopus_gw_start");
	extern const uint8_t gnwoctopus_gw_end[]   asm("_binary_Game___Watch_Octopus_gw_end");

	extern const uint8_t gnwparachute_gw_start[] asm("_binary_Game___Watch_Parachute_gw_start");
	extern const uint8_t gnwparachute_gw_end[]   asm("_binary_Game___Watch_Parachute_gw_end");

	extern const uint8_t gnwpopeye_wide_gw_start[] asm("_binary_Game___Watch_Popeye__Wide_Screen__gw_start");
	extern const uint8_t gnwpopeye_wide_gw_end[]   asm("_binary_Game___Watch_Popeye__Wide_Screen__gw_end");

	extern const uint8_t gnwsnoopytennis_gw_start[] asm("_binary_Game___Watch_Snoopy_Tennis_gw_start");
	extern const uint8_t gnwsnoopytennis_gw_end[]   asm("_binary_Game___Watch_Snoopy_Tennis_gw_end");

	extern const uint8_t gnwtropicalfish_gw_start[] asm("_binary_Game___Watch_Tropical_Fish_gw_start");
	extern const uint8_t gnwtropicalfish_gw_end[]   asm("_binary_Game___Watch_Tropical_Fish_gw_end");

	extern const uint8_t gnwturtlebridge_gw_start[] asm("_binary_Game___Watch_Turtle_Bridge_gw_start");
	extern const uint8_t gnwturtlebridge_gw_end[]   asm("_binary_Game___Watch_Turtle_Bridge_gw_end");

	extern const uint8_t gnwvermin_gw_start[] asm("_binary_Game___Watch_Vermin_gw_start");
	extern const uint8_t gnwvermin_gw_end[]   asm("_binary_Game___Watch_Vermin_gw_end");
	
	extern const uint8_t gnwdonkeykongjr_gw_start[] asm("_binary_Game___Watch_Donkey_Kong_Jr___New_Wide_Screen__gw_start");
	extern const uint8_t gnwdonkeykongjr_gw_end[] asm("_binary_Game___Watch_Donkey_Kong_Jr___New_Wide_Screen__gw_end");	
    
    extern const uint8_t gnwdballoonfight_gw_start[] asm("_binary_Game___Watch_Balloon_Fight__Crystal_Screen__gw_start");
    extern const uint8_t gnwdballoonfight_gw_end[] asm("_binary_Game___Watch_Balloon_Fight__Crystal_Screen__gw_end");
    
    extern const uint8_t gnwclimber_gw_start[] asm("_binary_Game___Watch_Climber__Crystal_Screen__gw_start");
    extern const uint8_t gnwclimber_gw_end[] asm("_binary_Game___Watch_Climber__Crystal_Screen__gw_end");

	extern const uint8_t gnwsupermariobros_gw_start[] asm("_binary_Game___Watch_Super_Mario_Bros___Crystal_Screen__gw_start");
    extern const uint8_t gnwsupermariobros_gw_end[] asm("_binary_Game___Watch_Super_Mario_Bros___Crystal_Screen__gw_end");

    
    uint8_t selected_game = 0;


    if (key == '1') { 
        selected_game = 1; // gnwparachute
    }
    else if (key == '2') { 
        selected_game = 2; // gnwoctopus
    }
    else if (key == '3') { 
        selected_game = 3; // gnwfirewidescreen
    }
    else if (key == '4') { 
        selected_game = 4; // gnwchef
    }
    else if (key == '5') { 
        selected_game = 5; // gnwpopeye
    }
    else if (key == '6') { 
        selected_game = 6; // gnwturtlebridge
    }
    else if (key == '7') { 
        selected_game = 7; // gnwvermin
    }
    else if (key == '8') { 
        selected_game = 8; // gnwball
    }
    else if (key == '9') { 
        selected_game = 9; // gnwfiresilver
    }
    else if (key == '0') { 
        selected_game = 10; // gnwhelmet
    }
	else if (key == 'Q') { 
        selected_game = 11; // gnwmariothejuggler
    }
    else if (key == 'W') { 
        selected_game = 12; // gnwtropicalfish
    }
    else if (key == 'E') { 
        selected_game = 13; // gnwegg
    }
    else if (key == 'R') { 
        selected_game = 14; // gnwfireattack
    }
    else if (key == 'T') { 
        selected_game = 15; // gnwflagman
    }
    else if (key == 'Y') { 
        selected_game = 16; // gnwjudge
    }
    else if (key == 'U') { 
        selected_game = 17; // gnwlion
    }
    else if (key == 'I') { 
        selected_game = 18; // gnwmanhole
    }
    else if (key == 'O') { 
        selected_game = 19; // gnwmanholegold
    }
    else if (key == 'P') { 
        selected_game = 20; // gnwmarioscementfactory
    }
    else if (key == 'A') { 
        selected_game = 21; // gnwmickeymouse
    }
    else if (key == 'S') { 
        selected_game = 22; // gnwsnoopytennis
    }
	else if (key == 'D') {
        selected_game = 23; // gnwdonkeykongjr
    }
    else if (key == 'F') { 
        selected_game = 24; // gnwdballoonfight
    }
    else if (key == 'G') { 
        selected_game = 25; // gnwclimber
    }
    else if (key == 'H') { 
        selected_game = 26; // gnwsupermariobros
    }

    if (selected_game == 0) {
        selected_game = read_selected_game();
        // no game selected, no start_delay
        start_delay = 0;
    }

    save_selected_game(selected_game);


    if (selected_game == 1) {
        ROM_DATA = (unsigned char *)gnwparachute_gw_start;
        ROM_DATA_LENGTH = gnwparachute_gw_end - gnwparachute_gw_start;
        control_button_type = 2;
		control_button_mode = 0;
    }
    else if (selected_game == 2) { 
        ROM_DATA = (unsigned char *)gnwoctopus_gw_start;
        ROM_DATA_LENGTH = gnwoctopus_gw_end - gnwoctopus_gw_start;
        control_button_type = 2;
		control_button_mode = 0;
    }
    else if (selected_game == 3) {
        ROM_DATA = (unsigned char *)gnwfirewidescreen_gw_start;
        ROM_DATA_LENGTH = gnwfirewidescreen_gw_end -gnwfirewidescreen_gw_start;
        control_button_type = 2;
		control_button_mode = 0;
    }
    else if (selected_game == 4) {
        ROM_DATA = (unsigned char *)gnwchef_gw_start;
        ROM_DATA_LENGTH = gnwchef_gw_end - gnwchef_gw_start;
        control_button_type = 2;
		control_button_mode = 0;
    }
    else if (selected_game == 5) {
        ROM_DATA = (unsigned char *)gnwpopeye_wide_gw_start;
        ROM_DATA_LENGTH = gnwpopeye_wide_gw_end - gnwpopeye_wide_gw_start;
        control_button_type = 2;
		control_button_mode = 0;        
    }
    else if (selected_game == 6) {
        ROM_DATA = (unsigned char *)gnwturtlebridge_gw_start;
        ROM_DATA_LENGTH = gnwturtlebridge_gw_end - gnwturtlebridge_gw_start;
        control_button_type = 2;
		control_button_mode = 0;        
    }
    else if (selected_game == 7) {
        ROM_DATA = (unsigned char *)gnwvermin_gw_start;
        ROM_DATA_LENGTH = gnwvermin_gw_end - gnwvermin_gw_start;
        control_button_type = 2;
		control_button_mode = 0;        
    }
    else if (selected_game == 8) {
        ROM_DATA = (unsigned char *)gnwball_gw_start;
        ROM_DATA_LENGTH = gnwball_gw_end - gnwball_gw_start;
        control_button_type = 2;
		control_button_mode = 0;        
    }
    else if (selected_game == 9) {
        ROM_DATA = (unsigned char *)gnwfiresilver_gw_start;
        ROM_DATA_LENGTH = gnwfiresilver_gw_end - gnwfiresilver_gw_start;
        control_button_type = 2;
		control_button_mode = 0;        
    }
    else if (selected_game == 10) {
        ROM_DATA = (unsigned char *)gnwhelmet_cn17_gw_start;
        ROM_DATA_LENGTH = gnwhelmet_cn17_gw_end - gnwhelmet_cn17_gw_start;
        control_button_type = 2;
		control_button_mode = 0;        
    }
    else if (selected_game == 11) {
        ROM_DATA = (unsigned char *)gnwmariojuggler_gw_start;
        ROM_DATA_LENGTH = gnwmariojuggler_gw_end - gnwmariojuggler_gw_start;
        control_button_type = 2;
		control_button_mode = 0;        
    }
    else if (selected_game == 12) {
        ROM_DATA = (unsigned char *)gnwtropicalfish_gw_start;
        ROM_DATA_LENGTH = gnwtropicalfish_gw_end - gnwtropicalfish_gw_start;
		control_button_type = 2;
		control_button_mode = 1;
    }
    else if (selected_game == 13) {
        ROM_DATA = (unsigned char *)gnwegg_gw_start;
        ROM_DATA_LENGTH = gnwegg_gw_end - gnwegg_gw_start;
		control_button_type = 4;
		control_button_mode = 0;
    }
    else if (selected_game == 14) { 
        ROM_DATA = (unsigned char *)gnwfireattack_gw_start;
        ROM_DATA_LENGTH = gnwfireattack_gw_end - gnwfireattack_gw_start;
		control_button_type = 4;
		control_button_mode = 0;
    }
    else if (selected_game == 15) {
        ROM_DATA = (unsigned char *)gnwflagman_gw_start;
        ROM_DATA_LENGTH = gnwflagman_gw_end - gnwflagman_gw_start;
		control_button_type = 4;
		control_button_mode = 3;
    }
    else if (selected_game == 16) {
        ROM_DATA = (unsigned char *)gnwjudge_green_gw_start;
        ROM_DATA_LENGTH = gnwjudge_green_gw_end - gnwjudge_green_gw_start;
		control_button_type = 4;
		control_button_mode = 0;        
    }
    else if (selected_game == 17) {
        ROM_DATA = (unsigned char *)gnwlion_gw_start;
        ROM_DATA_LENGTH = gnwlion_gw_end - gnwlion_gw_start;
		control_button_type = 4;
		control_button_mode = 3;
    }
    else if (selected_game == 18) {
        ROM_DATA = (unsigned char *)gnwmanhole_wide_gw_start;
        ROM_DATA_LENGTH = gnwmanhole_wide_gw_end - gnwmanhole_wide_gw_start;
		control_button_type = 4;
		control_button_mode = 0;        
    }
    else if (selected_game == 19) {
        ROM_DATA = (unsigned char *)gnwmanhole_gold_gw_start;
        ROM_DATA_LENGTH = gnwmanhole_gold_gw_end - gnwmanhole_gold_gw_start;
		control_button_type = 4;
		control_button_mode = 0;        
    }
    else if (selected_game == 20) {
        ROM_DATA = (unsigned char *)gnwmario_cement_wide_gw_start;
        ROM_DATA_LENGTH = gnwmario_cement_wide_gw_end - gnwmario_cement_wide_gw_start;
		control_button_type = 4;
		control_button_mode = 2;        
    }
    else if (selected_game == 21) {
        ROM_DATA = (unsigned char *)gnwmickey_wide_gw_start;
        ROM_DATA_LENGTH = gnwmickey_wide_gw_end - gnwmickey_wide_gw_start;
		control_button_type = 4;
		control_button_mode = 0;
    }
    else if (selected_game == 22) {
        ROM_DATA = (unsigned char *)gnwsnoopytennis_gw_start;
        ROM_DATA_LENGTH = gnwsnoopytennis_gw_end - gnwsnoopytennis_gw_start;
		control_button_type = 4;
		control_button_mode = 1;
    }
    else if (selected_game == 23) {
        ROM_DATA = (unsigned char *)gnwdonkeykongjr_gw_start;
        ROM_DATA_LENGTH = gnwdonkeykongjr_gw_end - gnwdonkeykongjr_gw_start;
		control_button_type = 5;
		control_button_mode = 0;
    }
    else if (selected_game == 24) {
        ROM_DATA = (unsigned char *)gnwdballoonfight_gw_start;
        ROM_DATA_LENGTH = gnwdballoonfight_gw_end - gnwdballoonfight_gw_start;
		control_button_type = 5;
		control_button_mode = 0;
    }
    else if (selected_game == 25) {
        ROM_DATA = (unsigned char *)gnwclimber_gw_start;
        ROM_DATA_LENGTH = gnwclimber_gw_end - gnwclimber_gw_start;
		control_button_type = 5;
		control_button_mode = 0;
    }
    else { // 26
        ROM_DATA = (unsigned char *)gnwsupermariobros_gw_start;
        ROM_DATA_LENGTH = gnwsupermariobros_gw_end - gnwsupermariobros_gw_start;
		control_button_type = 5;
		control_button_mode = 0;
    }
    
}

