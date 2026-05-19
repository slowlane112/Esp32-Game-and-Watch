#include <gw_system.h>
#include "freertos/FreeRTOS.h"
#include "driver/i2c_master.h"
#include "driver/i2s_std.h"
#include "audio.h"
#include "i2c.h"

#define AUD_I2S_BCK         41
#define AUD_I2S_WS          43
#define AUD_I2S_DATA        42

i2s_chan_handle_t i2s_audio_handle;

// ###### Cardputer Adv ###### 

#define ADV_ES8311_ADDR        0x18

i2c_master_dev_handle_t g_es8311_dev = NULL;

void adv_es8311_bulk_write(const uint8_t *data)
{
    if (g_es8311_dev == NULL) {
        return;
    }

    while (*data) {
        data++; // Skip length byte
        uint8_t reg = *data++;
        uint8_t val = *data++;

        uint8_t write_buf[2] = {reg, val};

        i2c_master_transmit(g_es8311_dev, write_buf, sizeof(write_buf), pdMS_TO_TICKS(100));
        
    }
}

void adv_es8311_init(void)
{

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = ADV_ES8311_ADDR,
        .scl_speed_hz = 400000,
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(g_i2c_bus, &dev_cfg, &g_es8311_dev));

    const uint8_t enabled_bulk_data[] = {
        2, 0x00, 0x80,
        2, 0x01, 0xB5,
        2, 0x02, 0x18,
        2, 0x0D, 0x01,
        2, 0x12, 0x00,
        2, 0x13, 0x10,
        2, 0x32, 0xBF,
        2, 0x37, 0x08,
        0
    };
    
    vTaskDelay(pdMS_TO_TICKS(20));
    adv_es8311_bulk_write(enabled_bulk_data);
    
}

// ###### End Cardputer Adv ###### 

void setup_audio_i2s() 
{
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &i2s_audio_handle, NULL));

    i2s_std_config_t i2s_config = {
        .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(GW_SYS_FREQ),
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .mclk = GPIO_NUM_NC,
            .bclk = AUD_I2S_BCK,
            .ws = AUD_I2S_WS,
            .dout = AUD_I2S_DATA,
            .din = GPIO_NUM_NC,  
        }
    };

    ESP_ERROR_CHECK(i2s_channel_init_std_mode(i2s_audio_handle, &i2s_config));
    ESP_ERROR_CHECK(i2s_channel_enable(i2s_audio_handle));
}

void audio_setup() 
{

    
    if (is_cardputer_adv) 
    {
		adv_es8311_init();
    }

    setup_audio_i2s();    
}
