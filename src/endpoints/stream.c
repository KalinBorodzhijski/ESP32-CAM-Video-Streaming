#include "esp_log.h"
#include "esp_http_server.h"
#include "endpoints/stream.h"
#include "session_manager.h"

static const char *TAG = "stream";

esp_err_t stream_get_handler(httpd_req_t *req)
{
    session_t *session = NULL;
    ESP_LOGI(TAG, "Handling stream GET request");

    if (check_user_logged_in(req, &session) != ESP_OK)
    {
        const char* resp_str = "Unauthorized";
        httpd_resp_send_err(req, HTTPD_401_UNAUTHORIZED, resp_str);
        ESP_LOGW(TAG, "Unauthorized access attempt");
        return ESP_ERR_HTTPD_RESP_SEND;
    }

    // Handle video stream logic
    const char* resp_str = "Streaming video...";
    httpd_resp_send(req, resp_str, strlen(resp_str));
    ESP_LOGI(TAG, "Streaming video to user with session ID: %s", session->session_id);
    return ESP_OK;
}