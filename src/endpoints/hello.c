#include <esp_log.h>
#include <esp_http_server.h>
#include "endpoints/hello.h"

static const char *TAG = "hello";

esp_err_t hello_get_handler(httpd_req_t *req) 
{
    ESP_LOGI(TAG, "Hello endpoint was called");
    const char* resp_str = (const char*) "Hello World!";
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}
