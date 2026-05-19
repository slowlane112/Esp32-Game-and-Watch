#include "i2c.h"
#include "esp_check.h"
#include "driver/gpio.h"

#define I2C_MASTER_SDA_IO      GPIO_NUM_8
#define I2C_MASTER_SCL_IO      GPIO_NUM_9
#define I2C_MASTER_FREQ_HZ     400000
#define TCA8418_ADDR           0x34

i2c_master_bus_handle_t g_i2c_bus = NULL;
i2c_master_dev_handle_t g_tca8418_dev = NULL;
bool is_cardputer_adv = false;

void i2c_deinit(void)
{
    if (g_tca8418_dev != NULL) {
        ESP_ERROR_CHECK(i2c_master_bus_rm_device(g_tca8418_dev));
        g_tca8418_dev = NULL;
    }

    if (g_i2c_bus != NULL) {
        ESP_ERROR_CHECK(i2c_del_master_bus(g_i2c_bus));
        g_i2c_bus = NULL;
    }

    gpio_reset_pin(I2C_MASTER_SDA_IO);
    gpio_reset_pin(I2C_MASTER_SCL_IO);
}


void i2c_init(void)
{
    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_cfg, &g_i2c_bus));

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = TCA8418_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };

	ESP_ERROR_CHECK(i2c_master_bus_add_device(g_i2c_bus, &dev_cfg, &g_tca8418_dev));

    uint8_t reg = 0x01;
    uint8_t dummy_data = 0;

	esp_log_level_set("i2c.master", ESP_LOG_NONE);
    esp_err_t err = i2c_master_transmit_receive(g_tca8418_dev, &reg, 1, &dummy_data, 1, 20);
	esp_log_level_set("i2c.master", (esp_log_level_t)CONFIG_LOG_DEFAULT_LEVEL);

    if (err == ESP_OK) {
        is_cardputer_adv = true;
    } else {
		i2c_deinit();
    }
}
