#include "nvs_flash.h"
#include "nvs.h"

void nvs_init(void)
{
	esp_err_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		printf("NVS Error\n");
		ESP_ERROR_CHECK(nvs_flash_erase());
		ESP_ERROR_CHECK(nvs_flash_init());
	}
}

uint8_t nvs_get_value(const char *name, uint8_t default_value) {
	uint8_t value = default_value;
	nvs_handle_t nvs_handle;
	ESP_ERROR_CHECK(nvs_open("gandw", NVS_READWRITE, &nvs_handle));
	nvs_get_u8(nvs_handle, name, &value);
	nvs_close(nvs_handle);
	return value;
}

void nvs_set_value(const char *name, uint8_t value) {
	nvs_handle_t nvs_handle;
	ESP_ERROR_CHECK(nvs_open("gandw", NVS_READWRITE, &nvs_handle));
	ESP_ERROR_CHECK(nvs_set_u8(nvs_handle, name, value));
	ESP_ERROR_CHECK(nvs_commit(nvs_handle));
	nvs_close(nvs_handle);
}
