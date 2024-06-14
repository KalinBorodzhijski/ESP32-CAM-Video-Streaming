#include "esp_log.h"
#include "esp_http_server.h"
#include "endpoints/logout.h"


esp_err_t logout_post_handler(httpd_req_t *req)
{
    server_context_t *ctx = (server_context_t *)req->user_ctx;
    // Handle logout logic
    ctx->user_logged_in = false;
    const char* resp_str = "Logout successful!";
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}