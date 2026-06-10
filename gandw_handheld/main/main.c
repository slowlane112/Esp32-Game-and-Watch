#include "freertos/FreeRTOS.h"
#include <gw_system.h>
#include "button.h"
#include "nvs.h"
#include "menu.h"
#include "game.h"
#include "battery.h"
#include "volume.h"
#include "display.h"
#include "audio.h"

static void display_volume_battery(unsigned short *framebuffer)
{
	if (volume_display_count > 0) {
		volume_display(framebuffer);
	}
	if (battery_display_count > 0) {
		battery_display(framebuffer);
	}
}

static void reset_system(unsigned short *framebuffer)
{
	display_blank_screen(framebuffer);
	esp_restart();
}

static void state_menu(unsigned short *framebuffer) {
	
	menu_index = game_get_selected();
	menu_update = true;
	
	while (menu_show) {
		
		menu_buttons_process();
		
		if (volume_display_count > 0) {
			volume_display_count--;
			if (volume_display_count == 0) {
				menu_update = true;
			}
		}

		if (battery_display_count > 0) {
			battery_display_count--;
			if (battery_display_count == 0) {
				menu_update = true;
			}
		}
		
		if (button_start_delay > 0) {
			button_start_delay--;
		}
			
		if (menu_update && !display_transfer_in_progress) {
			
			menu_display_item(framebuffer, menu_index);
			
			if (volume_display_count > 0 || battery_display_count > 0) {
				display_volume_battery(framebuffer);
			}

			display_render_screen(framebuffer);
			
			menu_update = false;
			
		}
		
		vTaskDelay(pdMS_TO_TICKS(8)); 
		
	}
	
}

static void state_game(unsigned short *framebuffer, uint16_t *audio_buffer) {
	
	size_t audio_buffer_size = GW_AUDIO_BUFFER_LENGTH * sizeof(uint16_t);
	
	button_start_delay = 63;
	
	game_load(menu_index);

	gw_system_romload();
	gw_system_sound_init();
	gw_system_config();
	gw_system_start();
	gw_system_reset();
	
	while (!menu_show) {

		if (volume_display_count > 0) {
			volume_display_count--;
		}

		if (battery_display_count > 0) {
			battery_display_count--;
		}
		
		if (button_start_delay > 0) {
			button_start_delay--;
		}

		gw_system_run(GW_SYSTEM_CYCLES);
		

		// lcd
		if (!display_transfer_in_progress) {

			display_transfer_in_progress = true;

			gw_system_blit(framebuffer);

			if (volume_display_count > 0 || battery_display_count > 0) {
				display_volume_battery(framebuffer);
			}

			display_render_screen(framebuffer);

		}

		// audio
	
		for (size_t i = 0; i < GW_AUDIO_BUFFER_LENGTH; i++) 
		{
			audio_buffer[i] = (gw_audio_buffer[i] > 0) * volume;
		}

		size_t bytes_written;

		i2s_channel_write(audio_handle, audio_buffer, audio_buffer_size, &bytes_written, portMAX_DELAY);

		gw_audio_buffer_copied = true;
		

	}
	
}

void app_main(void)
{

	// memory for sound and screen
	uint16_t *framebuffer = (uint16_t *)heap_caps_malloc(GW_SCREEN_WIDTH * GW_SCREEN_HEIGHT * sizeof(uint16_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
	uint16_t audio_buffer[GW_AUDIO_BUFFER_LENGTH];

	audio_init();
	display_init();
	button_init();
	nvs_init();
	
	display_blank_screen(framebuffer);

	volume_read();

	battery_get_level();
	
	state_menu(framebuffer);
	
	state_game(framebuffer, audio_buffer);
	
	reset_system(framebuffer);
	
}


