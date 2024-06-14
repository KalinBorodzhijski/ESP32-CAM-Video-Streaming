#include "esp_log.h"
#include "esp_http_server.h"
#include "endpoints/home.h"
extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_index_html_end");
extern const uint8_t style_css_start[] asm("_binary_styles_css_start");
extern const uint8_t style_css_end[] asm("_binary_styles_css_end");

esp_err_t root_get_handler(httpd_req_t *req)
{
    const char* response = (const char*)index_html_start;
    size_t response_len = index_html_end - index_html_start;
    httpd_resp_send(req, response, response_len);
    return ESP_OK;
}

esp_err_t css_get_handler(httpd_req_t *req)
{
    const char* response = (const char*)style_css_start;
    size_t response_len = style_css_end - style_css_start;
    httpd_resp_set_type(req, "text/css");
    httpd_resp_send(req, response, response_len);
    return ESP_OK;
}