#ifndef NVS_H
#define NVS_H

void nvs_init(void);
uint8_t nvs_get_value(const char *name, uint8_t default_value);
void nvs_set_value(const char *name, uint8_t value);

#endif




