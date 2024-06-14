#ifndef NVS_UTILS_H
#define NVS_UTILS_H

#include "esp_err.h"

void init_nvs(void);
esp_err_t nvs_get_wifi_credentials(char *ssid, size_t ssid_len, char *pass, size_t pass_len);
esp_err_t nvs_set_wifi_credentials(const char *ssid, const char *pass);

#endif // NVS_UTILS_H
