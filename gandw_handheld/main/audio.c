#include "driver/i2s_std.h"
#include <gw_system.h>

#define AUD_I2S_BCK			10
#define AUD_I2S_WS			4
#define AUD_I2S_DATA		6

i2s_chan_handle_t audio_handle;

i2s_chan_handle_t audio_setup(void) {

	i2s_chan_handle_t new_i2s_audio_handle;
	
	i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
	ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &new_i2s_audio_handle, NULL));

	i2s_std_config_t i2s_config = {
		.clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(GW_SYS_FREQ),
		.slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
		.gpio_cfg = {
			.mclk = GPIO_NUM_NC,
			.bclk = AUD_I2S_BCK,	// Max98357 CLK
			.ws = AUD_I2S_WS,		// Max98357 LRC
			.dout = AUD_I2S_DATA,	// Max98357 DIN
			.din = GPIO_NUM_NC,  
		}
	};

	ESP_ERROR_CHECK(i2s_channel_init_std_mode(new_i2s_audio_handle, &i2s_config));
	ESP_ERROR_CHECK(i2s_channel_enable(new_i2s_audio_handle));
	
	return new_i2s_audio_handle;
}

void audio_init(void) 
{
	audio_handle = audio_setup();
}
