#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "nvs_utils.h"
#include "wifi_config.h"
#include "server.h"

#define BLINK_GPIO GPIO_NUM_33
#define BLINK_TASK_STACK_SIZE 2048

static const char *TAG = "main";

void blink_task(void *pvParameter)
{
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    while (1)
    {
        ESP_LOGI(TAG, "LED ON");
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        ESP_LOGI(TAG, "LED OFF");
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void start_blink_task(void)
{
    xTaskCreate(&blink_task, "blink_task", BLINK_TASK_STACK_SIZE, NULL, 5, NULL);
}

void app_main(void)
{
    // Initialize NVS
    init_nvs();
    // Initialize Wi-Fi
    wifi_init_sta();

    // Start the server
    ESP_LOGI("main", "ESP32 Server starting...");
    start_server();


    // Create a task to blink the LED
    start_blink_task();

    // Print messages to the console
    ESP_LOGI("main", "ESP32 Server is running...");
    ESP_LOGI("main", "Blink task started, check the LED on GPIO %d", BLINK_GPIO);

}