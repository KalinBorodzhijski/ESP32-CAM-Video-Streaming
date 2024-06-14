#include "esp_log.h"
#include "esp_http_server.h"
#include "endpoints/login.h"

esp_err_t login_post_handler(httpd_req_t *req)
{
    server_context_t *ctx = (server_context_t *)req->user_ctx;
    // Handle login logic (authentication)
    ctx->user_logged_in = true;
    const char* resp_str = "Login successful!";
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}