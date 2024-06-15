#include "esp_log.h"
#include "esp_http_server.h"
#include "endpoints/home.h"
#include "session_manager.h"


static const char *TAG = "home";

extern const uint8_t style_css_start[] asm("_binary_styles_css_start");
extern const uint8_t style_css_end[] asm("_binary_styles_css_end");

static const char *index_html_logged_out = 
"<!DOCTYPE html>"
"<html lang=\"en\">"
"<head>"
    "<meta charset=\"UTF-8\">"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
    "<title>Personal Web Server</title>"
    "<link rel=\"stylesheet\" href=\"style.css\">"
    "<script>"
        "function showMessage(message) {"
            "alert(message);"
        "}"
    "</script>"
"</head>"
"<body>"
    "<header>"
        "<h1>Welcome to My Personal Web Server</h1>"
        "<nav>"
            "<a href=\"/\">Home</a>"
        "</nav>"
    "</header>"
    "<main>"
        "<section class=\"hero\">"
            "<h2>Secure, Reliable, and Accessible Anywhere</h2>"
            "<p>This project hosts a site and allows connection to a connected camera with user authentication. Enjoy secure video streaming and more!</p>"
            "<div class=\"form-container\">"
                "<form method=\"POST\" action=\"/login\">"
                    "<input type=\"text\" name=\"username\" placeholder=\"Username\" required>"
                    "<input type=\"password\" name=\"password\" placeholder=\"Password\" required>"
                    "<button type=\"submit\">Login</button>"
                "</form>"
                "<form method=\"POST\" action=\"/register\">"
                    "<input type=\"text\" name=\"username\" placeholder=\"Username\" required>"
                    "<input type=\"password\" name=\"password\" placeholder=\"Password\" required>"
                    "<button type=\"submit\">Register</button>"
                "</form>"
            "</div>"
        "</section>"
    "</main>"
    "<footer>"
        "<p>© 2024 Personal Web Server Project</p>"
    "</footer>"
"</body>"
"</html>";

static const char *index_html_logged_in = 
"<!DOCTYPE html>"
"<html lang=\"en\">"
"<head>"
    "<meta charset=\"UTF-8\">"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
    "<title>Personal Web Server</title>"
    "<link rel=\"stylesheet\" href=\"style.css\">"
    "<script>"
        "function showMessage(message) {"
            "alert(message);"
        "}"
    "</script>"
"</head>"
"<body>"
    "<header>"
        "<h1>Welcome to My Personal Web Server</h1>"
        "<nav>"
            "<a href=\"/\">Home</a>"
            "<form method=\"POST\" action=\"/logout\" style=\"display: inline;\">"
                "<button type=\"submit\">Logout</button>"
            "</form>"
        "</nav>"
    "</header>"
    "<main>"
        "<section class=\"hero\">"
            "<h2>Welcome back!</h2>"
            "<p>You are logged in. Access your live stream below.</p>"
            "<a href=\"/stream\" class=\"button\">View Live Stream</a>"
        "</section>"
    "</main>"
    "<footer>"
        "<p>© 2024 Personal Web Server Project</p>"
    "</footer>"
"</body>"
"</html>";



esp_err_t root_get_handler(httpd_req_t *req)
{
    session_t *session = NULL;
    ESP_LOGI(TAG, "Handling root GET request");

    if (check_user_logged_in(req, &session) != ESP_OK)
    {
        ESP_LOGI(TAG, "User is not logged in");
        httpd_resp_send(req, index_html_logged_out, strlen(index_html_logged_out));
        return ESP_OK;
    }

    ESP_LOGI(TAG, "User is logged in with session ID: %s", session->session_id);
    httpd_resp_send(req, index_html_logged_in, strlen(index_html_logged_in));
    return ESP_OK;
}

esp_err_t css_get_handler(httpd_req_t *req)
{
    const char* response = (const char*)style_css_start;
    size_t response_len = style_css_end - style_css_start;

    ESP_LOGI(TAG, "Serving CSS");

    httpd_resp_set_type(req, "text/css");
    httpd_resp_send(req, response, response_len);
    return ESP_OK;
}