#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "wifi_config.h"
#include "utils/nvs_utils.h"
#include <string.h>

static EventGroupHandle_t s_wifi_event_group;
static const char *TAG = "wifi_config";

static const char *DEFAULT_SSID = "";
static const char *DEFAULT_PASSWORD = "";

static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        esp_wifi_connect();
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        xEventGroupSetBits(s_wifi_event_group, BIT0);
    }
}

void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));

    char ssid[32] = {0};
    char pass[64] = {0};
    esp_err_t err = nvs_get_wifi_credentials(ssid, sizeof(ssid), pass, sizeof(pass));
    if (err != ESP_OK)
    {
        ESP_LOGW(TAG, "WiFi credentials not found in NVS. Setting default credentials.");
        err = nvs_set_wifi_credentials(DEFAULT_SSID, DEFAULT_PASSWORD);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to set WiFi credentials to NVS");
            return;
        }
        err = nvs_get_wifi_credentials(ssid, sizeof(ssid), pass, sizeof(pass));
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to get WiFi credentials from NVS");
            return;
        }
    }
    
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "",
            .password = "",
        },
    };

    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, pass, sizeof(wifi_config.sta.password));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    xEventGroupWaitBits(s_wifi_event_group, BIT0, false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "Connected to WiFi SSID: %s", ssid);
}