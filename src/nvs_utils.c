#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include "nvs_utils.h"

static const char *TAG = "nvs_utils";

void init_nvs(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_LOGI(TAG, "Erasing NVS flash due to error: %s", esp_err_to_name(ret));
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

esp_err_t nvs_get_wifi_credentials(char *ssid, size_t ssid_len, char *pass, size_t pass_len)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return err;
    }
    err = nvs_get_str(nvs_handle, "wifi_ssid", ssid, &ssid_len);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) reading SSID!", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return err;
    }
    err = nvs_get_str(nvs_handle, "wifi_pass", pass, &pass_len);
    nvs_close(nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) reading password!", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "WiFi credentials retrieved from NVS");
    return ESP_OK;
}

esp_err_t nvs_set_wifi_credentials(const char *ssid, const char *pass)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        return err;
    }
    err = nvs_set_str(nvs_handle, "wifi_ssid", ssid);
    if (err != ESP_OK)
    {
        nvs_close(nvs_handle);
        return err;
    }
    err = nvs_set_str(nvs_handle, "wifi_pass", pass);
    if (err != ESP_OK)
    {
        nvs_close(nvs_handle);
        return err;
    }
    err = nvs_commit(nvs_handle);
    nvs_close(nvs_handle);
    return err;
}