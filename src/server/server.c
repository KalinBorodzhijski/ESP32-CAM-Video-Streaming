#include "esp_log.h"
#include "esp_http_server.h"
#include "server.h"

#include "endpoints/hello.h"
#include "endpoints/home.h"
#include "endpoints/login.h"
#include "endpoints/logout.h"
#include "endpoints/stream.h"


static const char *TAG = "server";

void start_server()
{
    static server_context_t server_ctx = {
        .user_logged_in = false,
    };

    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_uri_t root = {
            .uri       = "/",
            .method    = HTTP_GET,
            .handler   = root_get_handler,
            .user_ctx  = &server_ctx
        };
        httpd_register_uri_handler(server, &root);

        httpd_uri_t css = {
            .uri       = "/style.css",
            .method    = HTTP_GET,
            .handler   = css_get_handler,
            .user_ctx  = &server_ctx
        };
        httpd_register_uri_handler(server, &css);

        httpd_uri_t login = {
            .uri       = "/login",
            .method    = HTTP_POST,
            .handler   = login_post_handler,
            .user_ctx  = &server_ctx
        };
        httpd_register_uri_handler(server, &login);

        httpd_uri_t stream = {
            .uri       = "/stream",
            .method    = HTTP_GET,
            .handler   = stream_get_handler,
            .user_ctx  = &server_ctx
        };
        httpd_register_uri_handler(server, &stream);

        httpd_uri_t logout = {
            .uri       = "/logout",
            .method    = HTTP_POST,
            .handler   = logout_post_handler,
            .user_ctx  = &server_ctx
        };
        httpd_register_uri_handler(server, &logout);

        httpd_uri_t hello = {
            .uri       = "/hello",
            .method    = HTTP_GET,
            .handler   = hello_get_handler,
            .user_ctx  = &server_ctx
        };
        httpd_register_uri_handler(server, &hello);
    }
    else
    {
        ESP_LOGI(TAG, "Error starting server!");
    }
}