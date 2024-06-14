#include "esp_log.h"
#include "esp_http_server.h"
#include "endpoints/stream.h"

esp_err_t stream_get_handler(httpd_req_t *req)
{
    server_context_t *ctx = (server_context_t *)req->user_ctx;
    if (!ctx->user_logged_in)
    {
        const char* resp_str = "Unauthorized";
        httpd_resp_send(req, resp_str, strlen(resp_str));
        return ESP_ERR_HTTPD_RESP_SEND;
    }

    // Handle video stream logic
    const char* resp_str = "Streaming video...";
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}