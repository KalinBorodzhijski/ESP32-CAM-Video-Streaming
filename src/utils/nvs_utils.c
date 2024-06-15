#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include "utils/nvs_utils.h"

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

esp_err_t nvs_erase_all_data()
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error opening NVS handle!");
        return err;
    }

    err = nvs_erase_all(handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error erasing NVS!");
    }
    
    nvs_close(handle);
    return err;
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

esp_err_t nvs_save_user(const char* username, const char* hashed_password, bool is_pending)
{
    //TODO: Remove the below code when email approval is implemented
    is_pending = false;

    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error opening NVS handle!");
        return err;
    }

    char key[64];
    snprintf(key, sizeof(key), "%s_status", username);

    err = nvs_set_str(handle, username, hashed_password);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error saving user!");
        nvs_close(handle);
        return err;
    }

    err = nvs_set_u8(handle, key, is_pending ? 1 : 0);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error saving user status!");
        nvs_close(handle);
        return err;
    }

    err = nvs_commit(handle);
    nvs_close(handle);
    return err;
}

esp_err_t nvs_get_user(const char* username, char* hashed_password, size_t max_len, bool* is_pending)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error opening NVS handle!");
        return err;
    }

    size_t required_len = 0;
    err = nvs_get_str(handle, username, NULL, &required_len);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        nvs_close(handle);
        return err;
    }

    if (required_len > max_len)
    {
        nvs_close(handle);
        return ESP_ERR_NVS_INVALID_LENGTH;
    }

    err = nvs_get_str(handle, username, hashed_password, &required_len);

    char key[64];
    snprintf(key, sizeof(key), "%s_status", username);
    uint8_t status;
    err = nvs_get_u8(handle, key, &status);
    if (err == ESP_OK)
    {
        *is_pending = (status == 1);
    }

    nvs_close(handle);
    return err;
}

bool nvs_user_exists(const char* username, bool* is_pending)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error opening NVS handle!");
        return false;
    }

    size_t required_len = 0;
    err = nvs_get_str(handle, username, NULL, &required_len);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        nvs_close(handle);
        return false;
    }

    char key[64];
    snprintf(key, sizeof(key), "%s_status", username);
    uint8_t status;
    err = nvs_get_u8(handle, key, &status);
    if (err == ESP_OK)
    {
        *is_pending = (status == 1);
    }

    nvs_close(handle);
    return true;
}

esp_err_t nvs_approve_user(const char* username)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error opening NVS handle!");
        return err;
    }

    char key[64];
    snprintf(key, sizeof(key), "%s_status", username);
    err = nvs_set_u8(handle, key, 0);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error updating user status!");
        nvs_close(handle);
        return err;
    }

    err = nvs_commit(handle);
    nvs_close(handle);
    return err;
}