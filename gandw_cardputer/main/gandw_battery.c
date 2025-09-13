#include <stdint.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include <gw_system.h>
#include "gandw_battery.h"

#define BATT_ADC_UNIT      ADC_UNIT_1
#define BATT_ADC_CHANNEL   ADC_CHANNEL_9
#define BATT_ADC_ATTEN     ADC_ATTEN_DB_12
#define BATT_ADC_WIDTH     ADC_BITWIDTH_DEFAULT


uint8_t battery_level = 0;
uint16_t battery_display_count = 0;

void get_battery_level() {

    esp_err_t ret;
    
    // --- ADC ONESHOT CONFIGURATION ---
    adc_oneshot_unit_handle_t adc_handle;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = BATT_ADC_UNIT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ret = adc_oneshot_new_unit(&init_config, &adc_handle);
    if (ret != ESP_OK) {
        return;
    }

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = BATT_ADC_WIDTH,
        .atten = BATT_ADC_ATTEN,
    };
    ret = adc_oneshot_config_channel(adc_handle, BATT_ADC_CHANNEL, &config);
    if (ret != ESP_OK) {
        adc_oneshot_del_unit(adc_handle);
        return;
    }

    // --- ADC CALIBRATION ---
    adc_cali_handle_t adc_cali_handle = NULL;
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = BATT_ADC_UNIT,
        .atten = BATT_ADC_ATTEN,
        .bitwidth = BATT_ADC_WIDTH,
    };
    ret = adc_cali_create_scheme_curve_fitting(&cali_config, &adc_cali_handle);
    if (ret != ESP_OK) {
        adc_oneshot_del_unit(adc_handle);
        return;
    }

    int total_adc_reading = 0;
    for (int i = 0; i < 10; i++) {
        int raw_adc_reading = 0;
        ret = adc_oneshot_read(adc_handle, BATT_ADC_CHANNEL, &raw_adc_reading);
        if (ret != ESP_OK) {
            adc_cali_delete_scheme_curve_fitting(adc_cali_handle);
            adc_oneshot_del_unit(adc_handle);
            return;
        }
        total_adc_reading += raw_adc_reading;
    }

    int averaged_adc_reading = total_adc_reading / 10.0f;

    // Convert averaged ADC reading to voltage (mV)
    int voltage_at_adc_pin_mv = 0;
    ret = adc_cali_raw_to_voltage(adc_cali_handle, averaged_adc_reading, &voltage_at_adc_pin_mv);
    if (ret != ESP_OK) {
        adc_cali_delete_scheme_curve_fitting(adc_cali_handle);
        adc_oneshot_del_unit(adc_handle);
        return;
    }

    // 1/2VBAT G10
    // max: 4.2V, min: 3.0V = 1.2V total

    //testing
    // Empty battery is 3V which should be 1500mV at G10
    // 1/2 battery is 3.6V which should be 1800mV at G10
    // Full battery is 4.2V which should be 2100mV at G10
    //voltage_at_adc_pin_mv = 1800;
    
    float battery_voltage_level = ((float)voltage_at_adc_pin_mv * 0.00200) - 3.0f;
    
    //printf("battery_voltage_level: %f\n", battery_voltage_level);
    
    float battery_percentage = 0.0f;

    if (battery_voltage_level >= 1.2f) {
        battery_percentage = 100.0f;
    }
    else if (battery_voltage_level <= 0.0f) {
        battery_percentage = 0.0f;
    }
    else {
        battery_percentage = (battery_voltage_level / 1.2f) * 100.0f;
    }

    if (battery_percentage > 100.0f) battery_percentage = 100.0f;
    if (battery_percentage < 0.0f) battery_percentage = 0.0f;

    if (battery_percentage >= 80.0f) {
        battery_level = 5;
    }
    else if (battery_percentage >= 60.0f) {
        battery_level = 4;
    }
    else if (battery_percentage >= 40.0f) {
        battery_level = 3;
    }
    else if (battery_percentage >= 20.0f) {
        battery_level = 2;
    }
    else if (battery_percentage >= 5.0f) {
        battery_level = 1;
    }
    else {
        battery_level = 0;
    }

    battery_display_count = 255;

    // --- Cleanup ---
    adc_cali_delete_scheme_curve_fitting(adc_cali_handle);
    adc_oneshot_del_unit(adc_handle);

}


void display_battery(unsigned short *framebuffer)
{
    uint8_t seg_count = 0;
    bool show_segment = false;
    uint8_t seg_number = 0;
    uint16_t pixel_colour = 0;
    uint16_t index = 0;
    uint16_t level_colour = 0xe007; // green: 0x07e0

    if (battery_level == 2 || battery_level == 3) {
        level_colour = 0xe0ff; // yellow: 0xffe0
    }
    else if (battery_level == 1) {
        level_colour = 0x00f8; // yellow: 0xf800
    }

    uint16_t batteryIcon_8x20[] = {
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
        0xffff,0xffff,0xffff,0x0000,0x0000,0xffff,0xffff,0xffff,
        0xffff,0xffff,0xffff,0x0000,0x0000,0xffff,0xffff,0xffff,
        0xffff,0x7294,0x0000,0x0000,0x0000,0x0000,0x7294,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xffff,
        0xffff,0x7294,0x0000,0x0000,0x0000,0x0000,0x7294,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
        0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff
    };


    uint16_t x_end = GW_SCREEN_WIDTH - 2 - 35;
    uint16_t x_start = x_end - 8;

    uint16_t pixel_pos = 0;
    for (uint8_t y = 3; y < 23; y++) {
        for (uint16_t x = x_start; x < x_end; x++) {
            index = y * GW_SCREEN_WIDTH + x;
            framebuffer[index] = batteryIcon_8x20[pixel_pos];

            pixel_pos++;
        }
    }

    x_end = GW_SCREEN_WIDTH - 2;
    x_start = x_end - 35;

    for (uint8_t y = 3; y < 23; y++) {
        seg_count = 0;
        seg_number = 0;
        for (uint16_t x = x_start; x < x_end; x++) {
            index = y * GW_SCREEN_WIDTH + x;
            seg_count++;
            if ((seg_count == 5 && show_segment) || (seg_count == 2 && !show_segment)) {
                seg_count = 0;
                show_segment = !show_segment;

                if (show_segment) {
                    seg_number++;
                }
            }

            pixel_colour = 0x9ad6;

            if ((y > 3 && y < 22) && show_segment) {
            
                if (seg_count == 0 || seg_count == 4 || y == 4 || y == 21) {
                    pixel_colour = 0x2c63;
                }
                else {
                
                    if (battery_level >= seg_number) {
                        pixel_colour = level_colour;
                    }
                    else {
                        pixel_colour = 0x518c;
                    }

                }
            }
           
            framebuffer[index] = pixel_colour;

        }
    }
}


