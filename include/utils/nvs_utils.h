#ifndef NVS_UTILS_H
#define NVS_UTILS_H

#include "esp_err.h"

void init_nvs(void);
esp_err_t nvs_erase_all_data();
esp_err_t nvs_get_wifi_credentials(char *ssid, size_t ssid_len, char *pass, size_t pass_len);
esp_err_t nvs_set_wifi_credentials(const char *ssid, const char *pass);
esp_err_t nvs_save_user(const char* username, const char* hashed_password, bool is_pending);
esp_err_t nvs_get_user(const char* username, char* hashed_password, size_t max_len, bool* is_pending);
bool nvs_user_exists(const char* username, bool* is_pending);
esp_err_t nvs_approve_user(const char* username);

#endif // NVS_UTILS_H
