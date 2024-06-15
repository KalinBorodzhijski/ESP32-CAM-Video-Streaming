#include "esp_log.h"
#include "esp_http_server.h"
#include "endpoints/login.h"
#include "utils/nvs_utils.h"
#include "utils/hash_utils.h"
#include "session_manager.h"
#include "utils/utils.h"

static const char *TAG = "login";

static void parse_query(char *query, char *username, char *password)
{
    char *token = strtok(query, "&");
    while (token != NULL)
    {
        if (strncmp(token, "username=", 9) == 0)
        {
            strcpy(username, token + 9);
            ESP_LOGI(TAG, "Parsed username: %s", username);
        }
        else if (strncmp(token, "password=", 9) == 0)
        {
            strcpy(password, token + 9);
            ESP_LOGI(TAG, "Parsed password: %s", password);
        }
        token = strtok(NULL, "&");
    }
}

esp_err_t login_post_handler(httpd_req_t *req)
{
    char buf[100];
    int ret = 0, remaining = req->content_len;
    char username[32] = {0}, password[32] = {0}, hashed_password[65];

    ESP_LOGI(TAG, "Starting login request handling");

    while (remaining > 0)
    {
        if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0)
        {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT)
            {
                ESP_LOGW(TAG, "Timeout receiving request");
                continue;
            }
            ESP_LOGE(TAG, "Error receiving request");
            return ESP_FAIL;
        }
        remaining -= ret;
    }
    buf[ret] = '\0';

    ESP_LOGI(TAG, "Received request data: %s", buf);

    parse_query(buf, username, password);

    bool is_pending;
    if (nvs_user_exists(username, &is_pending) && !is_pending)
    {
        ESP_LOGI(TAG, "User exists and is approved: %s", username);

        char stored_hashed_password[65];
        if (nvs_get_user(username, stored_hashed_password, sizeof(stored_hashed_password), &is_pending) == ESP_OK && !is_pending)
        {
            hash_password(password, hashed_password);
            ESP_LOGI(TAG, "Hashed password: %s", hashed_password);

            if (strcmp(stored_hashed_password, hashed_password) == 0)
            {
                session_t *session = create_session();
                if (session)
                {
                    snprintf(buf, sizeof(buf), "Set-Cookie: session_id=%s; Path=/; HttpOnly", session->session_id);
                    httpd_resp_set_hdr(req, "Set-Cookie", buf);
                    httpd_resp_sendstr(req, "<script>alert('Login successful!'); window.location.href = '/';</script>");
                    ESP_LOGI(TAG, "Login successful for user: %s", username);
                    ESP_LOGI(TAG, "Session ID set in cookie: %s", session->session_id);
                }
                else
                {
                    httpd_resp_sendstr(req, "<script>alert('Error creating session.'); window.location.href = '/';</script>");
                    ESP_LOGE(TAG, "Error creating session for user: %s", username);
                }
            }
            else
            {
                httpd_resp_sendstr(req, "<script>alert('Invalid username or password'); window.location.href = '/';</script>");
                ESP_LOGE(TAG, "Invalid password for user: %s", username);
            }
        }
        else
        {
            httpd_resp_sendstr(req, "<script>alert('Invalid username or password'); window.location.href = '/';</script>");
            ESP_LOGE(TAG, "Error retrieving user or user not approved: %s", username);
        }
    }
    else
    {
        httpd_resp_sendstr(req, "<script>alert('Invalid username or password'); window.location.href = '/';</script>");
        ESP_LOGE(TAG, "User does not exist or is pending approval: %s", username);
    }

    return ESP_OK;
}