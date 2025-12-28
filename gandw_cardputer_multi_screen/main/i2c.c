#include "driver/i2c.h"
#include "driver/i2s_std.h"

#define I2C_MASTER_SDA_IO  GPIO_NUM_8
#define I2C_MASTER_SCL_IO  GPIO_NUM_9
#define I2C_MASTER_NUM     I2C_NUM_0

bool is_cardputer_adv = false;

void i2c_deinit(void)
{
    // Uninstall I2C driver
    ESP_ERROR_CHECK(i2c_driver_delete(I2C_MASTER_NUM));

    // Reset pins so they can be reused
    gpio_reset_pin(I2C_MASTER_SDA_IO);
    gpio_reset_pin(I2C_MASTER_SCL_IO);
}

void i2c_init()
{
    i2c_config_t cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &cfg));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, cfg.mode, 0, 0, 0));
    
    
    uint8_t dummy_data;
    // try to read 1 byte from register 0x01
    esp_err_t err = i2c_master_write_read_device(I2C_MASTER_NUM, 0x34, 
                                                (uint8_t[]){0x01}, 1, 
                                                &dummy_data, 1, 
                                                20 / portTICK_PERIOD_MS);

    if (err == ESP_OK) {
		is_cardputer_adv = true;
    } else {
		i2c_deinit();
    }
    
}


