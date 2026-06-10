#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_ili9341.h"
#include "freertos/FreeRTOS.h"
#include <gw_system.h>
#include "display.h"

#if defined(USE_ILI9341)
	#define LCD_PIXEL_CLOCK_HZ    (60 * 1000 * 1000)
#elif defined(USE_ST7789)
	#define LCD_PIXEL_CLOCK_HZ    (80 * 1000 * 1000)
#endif

#define LCD_CMD_BITS		8
#define LCD_PARAM_BITS 		8

#define LCD_1_HOST			SPI2_HOST
#define LCD_1_SCLK			12
#define LCD_1_MOSI			11
#define LCD_1_MISO			-1
#define LCD_1_DC			9
#define LCD_1_RST			13

#if defined(MODEL_MULTI_SCREEN_2_BUTTON) || defined(MODEL_MULTI_SCREEN_DPAD)
	#define LCD_2_HOST		SPI3_HOST
	#define LCD_2_SCLK		18
	#define LCD_2_MOSI		17
	#define LCD_2_MISO		-1
	#define LCD_2_DC		38
	#define LCD_2_RST		39
#endif	

esp_lcd_panel_handle_t display_1_handle;
esp_lcd_panel_handle_t display_2_handle;
bool display_transfer_in_progress = false;

static bool display_color_trans_done(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *event_data, void *user_ctx) {
	display_transfer_in_progress = false;
	return false;
}

static esp_lcd_panel_handle_t display_setup_lcd_1(void) {

	esp_lcd_panel_handle_t spi_lcd_handle = NULL;
	esp_lcd_panel_io_handle_t io_handle = NULL;
	
#if defined(USE_ILI9341)

	spi_bus_config_t buscfg = {
		.sclk_io_num = LCD_1_SCLK,
		.mosi_io_num = LCD_1_MOSI,
		.miso_io_num = LCD_1_MISO,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = GW_SCREEN_WIDTH * GW_SCREEN_HEIGHT * sizeof(uint16_t)
	};
	ESP_ERROR_CHECK(spi_bus_initialize(LCD_1_HOST, &buscfg, SPI_DMA_CH_AUTO));

	esp_lcd_panel_io_spi_config_t io_config = {
		.dc_gpio_num = LCD_1_DC,
		.cs_gpio_num = -1,
		.pclk_hz = LCD_PIXEL_CLOCK_HZ,
		.lcd_cmd_bits = LCD_CMD_BITS,
		.lcd_param_bits = LCD_PARAM_BITS,
		.spi_mode = 0,
		.trans_queue_depth = 10,
		.on_color_trans_done = display_color_trans_done
	};
	ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_1_HOST, &io_config, &io_handle));

	esp_lcd_panel_dev_config_t panel_config = {
		.reset_gpio_num = LCD_1_RST,
		.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
		.bits_per_pixel = 16
	};
	ESP_ERROR_CHECK(esp_lcd_new_panel_ili9341(io_handle, &panel_config, &spi_lcd_handle));

	ESP_ERROR_CHECK(esp_lcd_panel_reset(spi_lcd_handle));
	ESP_ERROR_CHECK(esp_lcd_panel_init(spi_lcd_handle));
	esp_lcd_panel_swap_xy(spi_lcd_handle, true);
	
#if defined(MODEL_SINGLE_SCREEN_2_BUTTON) || defined(MODEL_MULTI_SCREEN_2_BUTTON)	
	esp_lcd_panel_mirror(spi_lcd_handle, true, true);
#else
	esp_lcd_panel_mirror(spi_lcd_handle, false, false);
#endif
	
	ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(spi_lcd_handle, true));
	
#elif defined(USE_ST7789)

	spi_bus_config_t buscfg = {
		.sclk_io_num = LCD_1_SCLK,
		.mosi_io_num = LCD_1_MOSI,
		.miso_io_num = LCD_1_MISO,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = GW_SCREEN_WIDTH * GW_SCREEN_HEIGHT * sizeof(uint16_t)
	};
	ESP_ERROR_CHECK(spi_bus_initialize(LCD_1_HOST, &buscfg, SPI_DMA_CH_AUTO));

	esp_lcd_panel_io_spi_config_t io_config = {
		.dc_gpio_num = LCD_1_DC,
		.cs_gpio_num = -1,
		.pclk_hz = LCD_PIXEL_CLOCK_HZ,
		.lcd_cmd_bits = LCD_CMD_BITS,
		.lcd_param_bits = LCD_PARAM_BITS,
		.spi_mode = 3,
		.trans_queue_depth = 10,
		.on_color_trans_done = display_color_trans_done
	};
	ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_1_HOST, &io_config, &io_handle));
	
	esp_lcd_panel_dev_config_t panel_config = {
		.reset_gpio_num = LCD_1_RST,
		.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
		.bits_per_pixel = 16
	};

	ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &spi_lcd_handle));
	
	ESP_ERROR_CHECK(esp_lcd_panel_reset(spi_lcd_handle));
	ESP_ERROR_CHECK(esp_lcd_panel_init(spi_lcd_handle));
	esp_lcd_panel_swap_xy(spi_lcd_handle, true);
	
	#if defined(MODEL_SINGLE_SCREEN_2_BUTTON) || defined(MODEL_MULTI_SCREEN_2_BUTTON)	
		esp_lcd_panel_mirror(spi_lcd_handle, true, false);
	#else
		esp_lcd_panel_mirror(spi_lcd_handle, false, true);
	#endif

	ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(spi_lcd_handle, true));
	esp_lcd_panel_invert_color(spi_lcd_handle, true);

#endif	

	return spi_lcd_handle;
	
}

#if defined(MODEL_MULTI_SCREEN_2_BUTTON) || defined(MODEL_MULTI_SCREEN_DPAD)

	static esp_lcd_panel_handle_t display_setup_lcd_2(void) {

		esp_lcd_panel_handle_t spi_lcd_handle = NULL;
		esp_lcd_panel_io_handle_t io_handle = NULL;
		
	#if defined(USE_ILI9341)

		spi_bus_config_t buscfg = {
			.sclk_io_num = LCD_2_SCLK,
			.mosi_io_num = LCD_2_MOSI,
			.miso_io_num = LCD_2_MISO,
			.quadwp_io_num = -1,
			.quadhd_io_num = -1,
			.max_transfer_sz = GW_SCREEN_WIDTH * GW_SCREEN_HEIGHT * sizeof(uint16_t)
		};
		ESP_ERROR_CHECK(spi_bus_initialize(LCD_2_HOST, &buscfg, SPI_DMA_CH_AUTO));

		esp_lcd_panel_io_spi_config_t io_config = {
			.dc_gpio_num = LCD_2_DC,
			.cs_gpio_num = -1,
			.pclk_hz = LCD_PIXEL_CLOCK_HZ,
			.lcd_cmd_bits = LCD_CMD_BITS,
			.lcd_param_bits = LCD_PARAM_BITS,
			.spi_mode = 0,
			.trans_queue_depth = 10
		};
		ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_2_HOST, &io_config, &io_handle));

		esp_lcd_panel_dev_config_t panel_config = {
			.reset_gpio_num = LCD_2_RST,
			.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
			.bits_per_pixel = 16
		};
		ESP_ERROR_CHECK(esp_lcd_new_panel_ili9341(io_handle, &panel_config, &spi_lcd_handle));

		ESP_ERROR_CHECK(esp_lcd_panel_reset(spi_lcd_handle));
		ESP_ERROR_CHECK(esp_lcd_panel_init(spi_lcd_handle));
		esp_lcd_panel_swap_xy(spi_lcd_handle, true);
		
#if defined(MODEL_MULTI_SCREEN_2_BUTTON)	
		esp_lcd_panel_mirror(spi_lcd_handle, true, true);
#else
	esp_lcd_panel_mirror(spi_lcd_handle, false, false);
#endif		
		
		ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(spi_lcd_handle, true));
		
	#elif defined(USE_ST7789)

		spi_bus_config_t buscfg = {
			.sclk_io_num = LCD_2_SCLK,
			.mosi_io_num = LCD_2_MOSI,
			.miso_io_num = LCD_2_MISO,
			.quadwp_io_num = -1,
			.quadhd_io_num = -1,
			.max_transfer_sz = GW_SCREEN_WIDTH * GW_SCREEN_HEIGHT * sizeof(uint16_t)
		};
		ESP_ERROR_CHECK(spi_bus_initialize(LCD_2_HOST, &buscfg, SPI_DMA_CH_AUTO));

		esp_lcd_panel_io_spi_config_t io_config = {
			.dc_gpio_num = LCD_2_DC,
			.cs_gpio_num = -1,
			.pclk_hz = LCD_PIXEL_CLOCK_HZ,
			.lcd_cmd_bits = LCD_CMD_BITS,
			.lcd_param_bits = LCD_PARAM_BITS,
			.spi_mode = 3,
			.trans_queue_depth = 10
		};
		ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_2_HOST, &io_config, &io_handle));
		
		esp_lcd_panel_dev_config_t panel_config = {
			.reset_gpio_num = LCD_2_RST,
			.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
			.bits_per_pixel = 16
		};

		ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &spi_lcd_handle));
		
		ESP_ERROR_CHECK(esp_lcd_panel_reset(spi_lcd_handle));
		ESP_ERROR_CHECK(esp_lcd_panel_init(spi_lcd_handle));
		esp_lcd_panel_swap_xy(spi_lcd_handle, true);

#if defined(MODEL_SINGLE_SCREEN_2_BUTTON) || defined(MODEL_MULTI_SCREEN_2_BUTTON)	
		esp_lcd_panel_mirror(spi_lcd_handle, true, false);
#else
		esp_lcd_panel_mirror(spi_lcd_handle, false, true);
#endif

		ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(spi_lcd_handle, true));
		esp_lcd_panel_invert_color(spi_lcd_handle, true);

	#endif	

		return spi_lcd_handle;
	}

#endif

void display_render_screen(unsigned short *framebuffer) {
	#if defined(MODEL_MULTI_SCREEN_2_BUTTON) || defined(MODEL_MULTI_SCREEN_DPAD)
		esp_lcd_panel_draw_bitmap(display_2_handle, 0, RENDER_PADDING, GW_SCREEN_WIDTH, RENDER_HEIGHT + RENDER_PADDING, framebuffer);
		esp_lcd_panel_draw_bitmap(display_1_handle, 0, RENDER_PADDING, GW_SCREEN_WIDTH, RENDER_HEIGHT + RENDER_PADDING, framebuffer + (GW_SCREEN_WIDTH * RENDER_HEIGHT));
	#else
		esp_lcd_panel_draw_bitmap(display_1_handle, 0, 0, GW_SCREEN_WIDTH, DISPLAY_HEIGHT, framebuffer);
	#endif
}

void display_blank_screen(unsigned short *framebuffer) {
    memset(framebuffer, 0, DISPLAY_HEIGHT * GW_SCREEN_WIDTH * sizeof(uint16_t));
    esp_lcd_panel_draw_bitmap(display_1_handle, 0, 0, GW_SCREEN_WIDTH, DISPLAY_HEIGHT, framebuffer);
    vTaskDelay(pdMS_TO_TICKS(50));
#if defined(MODEL_MULTI_SCREEN_2_BUTTON) || defined(MODEL_MULTI_SCREEN_DPAD)    
    esp_lcd_panel_draw_bitmap(display_2_handle, 0, 0, GW_SCREEN_WIDTH, DISPLAY_HEIGHT, framebuffer);
	vTaskDelay(pdMS_TO_TICKS(50));
#endif		
}

void display_init(void) 
{
	display_1_handle = display_setup_lcd_1();
	
#if defined(MODEL_MULTI_SCREEN_2_BUTTON) || defined(MODEL_MULTI_SCREEN_DPAD)
	display_2_handle = display_setup_lcd_2();
#endif	
}
