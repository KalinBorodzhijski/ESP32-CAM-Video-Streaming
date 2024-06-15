#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_http_server.h"
#include "server.h"

#include "endpoints/hello.h"
#include "endpoints/home.h"
#include "endpoints/login.h"
#include "endpoints/logout.h"
#include "endpoints/stream.h"
#include "endpoints/register.h"
#include "endpoints/approve.h"
#include "session_manager.h"
#include <string.h>
#include <stdlib.h>


static const char *TAG = "server";

void start_server()
{
    static server_context_t server_ctx = {0};
    init_session_manager(&server_ctx);
    
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_open_sockets = 4;
    config.stack_size = 8192;

    httpd_handle_t server = NULL;

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

        httpd_uri_t register_uri = {
            .uri       = "/register",
            .method    = HTTP_POST,
            .handler   = register_post_handler,
            .user_ctx  = &server_ctx
        };
        httpd_register_uri_handler(server, &register_uri);

        httpd_uri_t approve_uri = {
            .uri       = "/approve",
            .method    = HTTP_POST,
            .handler   = approve_get_handler,
            .user_ctx  = &server_ctx
        };
        httpd_register_uri_handler(server, &approve_uri);

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