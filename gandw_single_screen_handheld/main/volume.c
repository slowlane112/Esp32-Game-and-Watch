#include <stdint.h>
#include <stdbool.h>
#include "volume.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "esp_timer.h"
#include <gw_system.h>
#include <math.h>
#include "driver/i2s_std.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

uint16_t volume = 1600;
uint8_t volume_level = 4;
uint32_t volume_time = 0;
uint16_t volume_display_count = 0;
uint16_t volume_low_color = BYTE_SWAP ? 0xe007 : 0x07e0;
uint16_t volume_medium_color = BYTE_SWAP ? 0xe0ff : 0xffe0;
uint16_t volume_high_color = BYTE_SWAP ? 0x00f8 : 0xf800;

uint16_t volume_get(uint8_t volume_level)
{
	
	/*	
		0 -> 0
		1 -> 100
		2 -> 400
		3 -> 900
		4 -> 1600
		5 -> 2500
		6 -> 3600
		7 -> 4900
		8 -> 6400
		9 -> 8100
		10 -> 10000
	*/

	double norm = volume_level / 10.0;
	double gain = pow(norm, 2.0);
	return 10000 * gain;
}	

void volume_read()
{
    nvs_handle_t my_handle;
    ESP_ERROR_CHECK(nvs_open("gandw", NVS_READWRITE, &my_handle));
    nvs_get_u8(my_handle, "volume_level", &volume_level);
    nvs_close(my_handle);

    if (volume_level > 10) {
        volume_level = 4;
    }

    volume = volume_get(volume_level);
}

void volume_save()
{
    nvs_handle_t my_handle;
    ESP_ERROR_CHECK(nvs_open("gandw", NVS_READWRITE, &my_handle));
    ESP_ERROR_CHECK(nvs_set_u8(my_handle, "volume_level", volume_level));
    ESP_ERROR_CHECK(nvs_commit(my_handle));
    nvs_close(my_handle);
}

void volume_set(bool is_up)
{

    uint32_t current_time = esp_timer_get_time() / 1000;

    if (current_time - volume_time > 200) {
        if (is_up) {
            if (volume_level < 10) {
                volume_level++;
            }
        }
        else {
            if (volume_level > 0) {
                volume_level--;
            }
        }

		volume = volume_get(volume_level);
        
        volume_time = current_time;
        volume_display_count = 255;
        volume_save();

    }
}

uint16_t volume_get_segment_colour(uint8_t seg_number)
{

    if (seg_number > 3 && seg_number < 8) { //4, 5, 6, 7
        return volume_medium_color;
    }
    else if (seg_number > 7) { // 8, 9, 10
        return volume_high_color;
    }
    else {
        return volume_low_color;
    }

}

void volume_display(unsigned short *framebuffer)
{
    uint8_t seg_count = 0;
    bool show_segment = false;
    uint8_t seg_number = 0;
    uint16_t pixel_colour = 0;
    uint16_t index = 0;

    #define S(x) ((BYTE_SWAP) ? __builtin_bswap16(x) : (x))
    
    uint16_t speaker_icon_13x20[] = {
		S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff),
		S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff),
		S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0x9472), S(0x0000), S(0xffff),
		S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0x9472), S(0x0000), S(0x0000), S(0xffff),
		S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0x9472), S(0x0000), S(0x0000), S(0x0000), S(0xffff),
		S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0x9472), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0xffff),
		S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0x9472), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0xffff),
		S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0x9472), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0xffff),
		S(0xffff), S(0x9472), S(0x0000), S(0x0000), S(0xffff), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0xffff),
		S(0xffff), S(0x0000), S(0x0000), S(0x0000), S(0xffff), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0xffff),
		S(0xffff), S(0x0000), S(0x0000), S(0x0000), S(0xffff), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0xffff),
		S(0xffff), S(0x9472), S(0x0000), S(0x0000), S(0xffff), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0xffff),
		S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0x9472), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0xffff),
		S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0x9472), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0xffff),
		S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0x9472), S(0x0000), S(0x0000), S(0x0000), S(0x0000), S(0xffff),
		S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0x9472), S(0x0000), S(0x0000), S(0x0000), S(0xffff),
		S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0x9472), S(0x0000), S(0x0000), S(0xffff),
		S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0x9472), S(0x0000), S(0xffff),
		S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff),
		S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff), S(0xffff)
	};

    uint16_t pixel_pos = 0;
    for (uint8_t y = 3; y < 23; y++) {
        for (uint8_t x = 2; x < 15; x++) {
            index = y * GW_SCREEN_WIDTH + x;
            framebuffer[index] = speaker_icon_13x20[pixel_pos];

            pixel_pos++;
        }
    }

    for (uint8_t y = 3; y < 23; y++) {
        seg_count = 0;
        seg_number = 0;
        for (uint8_t x = 15; x < 85; x++) {
            index = y * GW_SCREEN_WIDTH + x;
            seg_count++;
            if ((seg_count == 5 && show_segment) || (seg_count == 2 && !show_segment)) {
                seg_count = 0;
                show_segment = !show_segment;

                if (show_segment) {
                    seg_number++;
                }
            }

            pixel_colour = BYTE_SWAP ? 0x9ad6 : 0xd69a;

            if ((y > 3 && y < 22) && show_segment) {

                if (seg_count == 0 || seg_count == 4 || y == 4 || y == 21) {
                    pixel_colour = BYTE_SWAP ? 0x2c63 : 0x632c;
                }
                else {
                
                    if (volume_level >= seg_number) {
                        pixel_colour = volume_get_segment_colour(seg_number);
                    }
                    else {
                        pixel_colour = BYTE_SWAP ? 0x518c : 0x8c51;
                    }

                }
            }

            framebuffer[index] = pixel_colour;

        }
    }
    
}

void volume_menu_tone(i2s_chan_handle_t i2s_handle, int volume_level)
{
    const int sample_rate = 16000;
    const int freq = 100;
    const int period_samples = (sample_rate / freq) * 2;
    const int repeat = 5;
    int16_t buffer[period_samples];
    size_t written;
    
    uint16_t volume = volume_get(volume_level);

    for (int i = 0; i < repeat; i++) {
        for (int j = 0; j < period_samples; j++) {
            double t = (double)(i*period_samples + j) / sample_rate;
            double envelope = exp(-25.0 * t);  
            int half = period_samples / 2;
            int sq = (j < half) ? 1 : -1;
            buffer[j] = (int16_t)(volume * envelope * sq);
        }
        ESP_ERROR_CHECK(i2s_channel_write(i2s_handle, buffer, sizeof(buffer), &written, portMAX_DELAY));
    }


    memset(buffer, 0, sizeof(buffer));
    for (int j = 0; j < repeat; j++) {
        ESP_ERROR_CHECK(i2s_channel_write(i2s_handle, buffer, sizeof(buffer), &written, portMAX_DELAY));
    }
}



