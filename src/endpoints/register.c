#include "esp_log.h"
#include "esp_http_server.h"
#include "endpoints/register.h"
#include "utils/nvs_utils.h"
#include "utils/hash_utils.h"
#include "utils/email_utils.h"
#include "utils/utils.h"

#include <string.h>


static const char *TAG = "register";


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

esp_err_t register_post_handler(httpd_req_t *req)
{
    char buf[100];
    int ret = 0, remaining = req->content_len;
    char username[32] = {0}, password[32] = {0}, hashed_password[65];

    ESP_LOGI(TAG, "Starting registration request handling");

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

    // Basic verification
    if (strlen(username) < 4 || strlen(password) < 6)
    {
        httpd_resp_sendstr(req, "<script>alert('Username or password too short'); window.location.href = '/';</script>");
        ESP_LOGE(TAG, "Username or password too short: %s, %s", username, password);
        return ESP_OK;
    }

    bool is_pending;
    if (nvs_user_exists(username, &is_pending))
    {
        httpd_resp_sendstr(req, "<script>alert('User already exists'); window.location.href = '/';</script>");
        ESP_LOGE(TAG, "User already exists: %s", username);
        return ESP_OK;
    }

    // Hash the password
    hash_password(password, hashed_password);
    ESP_LOGI(TAG, "Hashed password: %s", hashed_password);

    // Save user to NVS with pending status
    esp_err_t err = nvs_save_user(username, hashed_password, true);
    if (err != ESP_OK)
    {
        httpd_resp_sendstr(req, "<script>alert('Error saving user to NVS'); window.location.href = '/';</script>");
        ESP_LOGE(TAG, "Error saving user to NVS: %s", username);
        return ESP_FAIL;
    }

    // Send approval email
    send_approval_email(username);
    ESP_LOGI(TAG, "Sent approval email for user: %s", username);

    httpd_resp_sendstr(req, "<script>alert('Registration request sent for approval'); window.location.href = '/';</script>");

    return ESP_OK;
}