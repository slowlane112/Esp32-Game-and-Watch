#include <stdio.h>
#include <gw_system.h>
#include <gw_romloader.h>
#include "driver/gpio.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_ili9341.h"
#include "driver/i2s_std.h"

#define DISPLAY_HEIGHT      240
#define RENDER_HEIGHT       240
#define RENDER_PADDING      0

// buttons
#define BUTTON_GAME_A       GPIO_NUM_14
#define BUTTON_GAME_B       GPIO_NUM_5
#define BUTTON_TIME         GPIO_NUM_3
#define BUTTON_LEFT         GPIO_NUM_2
#define BUTTON_RIGHT        GPIO_NUM_1
#define BUTTON_ALARM        GPIO_NUM_7
#define BUTTON_ACL          GPIO_NUM_8

// LCD
#define LCD_PIXEL_CLOCK_HZ  (60 * 1000 * 1000)
#define LCD_CMD_BITS        8
#define LCD_PARAM_BITS      8

#define LCD_HOST            SPI2_HOST
#define LCD_SCLK            12
#define LCD_MOSI            11
#define LCD_MISO            -1
#define LCD_DC              9
#define LCD_RST             13

// audio
#define AUD_I2S_BCK         10
#define AUD_I2S_WS          4
#define AUD_I2S_DATA        6

unsigned char *ROM_DATA;
unsigned int ROM_DATA_LENGTH;

unsigned int gw_get_buttons()
{
    uint32_t hw_buttons = 0;

    /*
        GAME A: GW_BUTTON_GAME
        GAME B: GW_BUTTON_TIME
        Time: GW_BUTTON_B + GW_BUTTON_TIME
        ALARM: GW_BUTTON_B + GW_BUTTON_GAME
        ACL: gw_system_reset()
        LEFT: GW_BUTTON_LEFT
        RIGHT: GW_BUTTON_RIGHT
    */


    if (gpio_get_level(BUTTON_TIME) == 0) {
        hw_buttons |= GW_BUTTON_B + GW_BUTTON_TIME; 
    }
    else if (gpio_get_level(BUTTON_RIGHT) == 0) { 
        hw_buttons |= GW_BUTTON_RIGHT;
    }
    else if (gpio_get_level(BUTTON_LEFT) == 0) {
        hw_buttons |= GW_BUTTON_LEFT;
    }
    else if (gpio_get_level(BUTTON_GAME_A) == 0) { 
        hw_buttons |= GW_BUTTON_GAME;
    }
    else if (gpio_get_level(BUTTON_GAME_B) == 0) {
        hw_buttons |= GW_BUTTON_TIME;
    }
    else if (gpio_get_level(BUTTON_ALARM) == 0) {
        hw_buttons |= GW_BUTTON_B + GW_BUTTON_GAME;
    }
    else if (gpio_get_level(BUTTON_ACL) == 0) {
        gw_system_reset();
    }

    return hw_buttons;
}

i2s_chan_handle_t setup_audio_i2s() {

    i2s_chan_handle_t i2s_audio_handle;
    
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);

    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &i2s_audio_handle, NULL));

    i2s_std_config_t i2s_config = {
        .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(GW_SYS_FREQ),
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .mclk = GPIO_NUM_NC,
            .bclk = AUD_I2S_BCK,    // Max98357 CLK
            .ws = AUD_I2S_WS,       // Max98357 LRC
            .dout = AUD_I2S_DATA,   // Max98357 DIN
            .din = GPIO_NUM_NC,  
        }
    };

    ESP_ERROR_CHECK(i2s_channel_init_std_mode(i2s_audio_handle, &i2s_config));
    ESP_ERROR_CHECK(i2s_channel_enable(i2s_audio_handle));

    return i2s_audio_handle;

}

esp_lcd_panel_handle_t setup_lcd_spi() {

    esp_lcd_panel_handle_t spi_lcd_handle = NULL;
    esp_lcd_panel_io_handle_t io_handle = NULL;

    spi_bus_config_t buscfg = {
        .sclk_io_num = LCD_SCLK,
        .mosi_io_num = LCD_MOSI,
        .miso_io_num = LCD_MISO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = GW_SCREEN_WIDTH * 80 * sizeof(uint16_t),
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = LCD_DC,
        .cs_gpio_num = -1,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
        .bits_per_pixel = 16,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_ili9341(io_handle, &panel_config, &spi_lcd_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(spi_lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(spi_lcd_handle));
    esp_lcd_panel_swap_xy(spi_lcd_handle, true);
    //esp_lcd_panel_mirror(spi_lcd_handle, true, true);
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(spi_lcd_handle, true));

    return spi_lcd_handle;
}

void setup_buttons() {

    esp_rom_gpio_pad_select_gpio(BUTTON_TIME);
    gpio_set_direction(BUTTON_TIME, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_TIME, GPIO_PULLUP_ONLY);

    esp_rom_gpio_pad_select_gpio(BUTTON_GAME_A);
    gpio_set_direction(BUTTON_GAME_A, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GAME_A, GPIO_PULLUP_ONLY);

    esp_rom_gpio_pad_select_gpio(BUTTON_GAME_B);
    gpio_set_direction(BUTTON_GAME_B, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GAME_B, GPIO_PULLUP_ONLY);

    esp_rom_gpio_pad_select_gpio(BUTTON_ALARM);
    gpio_set_direction(BUTTON_ALARM, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_ALARM, GPIO_PULLUP_ONLY);

    esp_rom_gpio_pad_select_gpio(BUTTON_ACL);
    gpio_set_direction(BUTTON_ACL, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_ACL, GPIO_PULLUP_ONLY);

    esp_rom_gpio_pad_select_gpio(BUTTON_LEFT);
    gpio_set_direction(BUTTON_LEFT, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_LEFT, GPIO_PULLUP_ONLY);

    esp_rom_gpio_pad_select_gpio(BUTTON_RIGHT);
    gpio_set_direction(BUTTON_RIGHT, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_RIGHT, GPIO_PULLUP_ONLY);

}

void app_main(void)
{


    // memory for sound and screen

    uint16_t *framebuffer = (uint16_t *)heap_caps_malloc(GW_SCREEN_WIDTH * GW_SCREEN_HEIGHT * sizeof(uint16_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);

    uint16_t audio_buffer[GW_AUDIO_BUFFER_LENGTH];

    // sound
    i2s_chan_handle_t i2s_audio_handle = setup_audio_i2s();

    // screen
    esp_lcd_panel_handle_t spi_lcd_handle = setup_lcd_spi();

    // buttons
    setup_buttons();

    // blank screen at start up
    for (int i = 0; i < DISPLAY_HEIGHT * GW_SCREEN_WIDTH; i++) {
        framebuffer[i] = 0x0000;
    }
    esp_lcd_panel_draw_bitmap(spi_lcd_handle, 0, 0, GW_SCREEN_WIDTH, DISPLAY_HEIGHT, framebuffer);

    // Load ROM
    extern const uint8_t rom_gw_start[] asm("_binary_gnwparachute_gw_start");
    extern const uint8_t rom_gw_end[] asm("_binary_gnwparachute_gw_end");

    ROM_DATA = (unsigned char *)rom_gw_start;
    ROM_DATA_LENGTH = rom_gw_end - rom_gw_start;

    gw_system_romload();

    gw_system_sound_init();
    gw_system_config();
    gw_system_start();
    gw_system_reset();

    uint16_t sample = 0;
    int display_update_count = 0;

    while (true) {

        display_update_count++;

        gw_system_run(GW_SYSTEM_CYCLES);
        

        // lcd

        if (display_update_count == 8) {

            gw_system_blit(framebuffer);

            esp_lcd_panel_draw_bitmap(spi_lcd_handle, 0, RENDER_PADDING, GW_SCREEN_WIDTH, RENDER_HEIGHT + RENDER_PADDING, framebuffer);

            display_update_count = 0;

        }


        // audio
 
        for (size_t i = 0; i < GW_AUDIO_BUFFER_LENGTH; i++)
        {

            sample = 0;
                       
            if (gw_audio_buffer[i] > 0) {
                sample = 2000;
            }

            audio_buffer[i] = sample;
                        
        }

        size_t bytes_written;

        i2s_channel_write(i2s_audio_handle, audio_buffer, sizeof(audio_buffer), &bytes_written, portMAX_DELAY);

        gw_audio_buffer_copied = true;

    }

}


