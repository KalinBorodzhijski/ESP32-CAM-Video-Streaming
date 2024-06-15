#include "session_manager.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static const char *TAG = "session_manager";

static server_context_t *ctx;


void init_session_manager(server_context_t *context)
{
    ESP_LOGI(TAG, "Initializing session manager");
    ctx = context;
}

void generate_session_id(char *session_id, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    if (size)
    {
        --size;
        for (size_t n = 0; n < size; n++)
        {
            int key = rand() % (int)(sizeof(charset) - 1);
            session_id[n] = charset[key];
        }
        session_id[size] = '\0';
        ESP_LOGI(TAG, "Generated session ID: %s", session_id);
    }
}

session_t *find_session(const char *session_id)
{
    ESP_LOGI(TAG, "Finding session for ID: %s", session_id);
    for (int i = 0; i < MAX_SESSIONS; i++)
    {
        if (strcmp(ctx->sessions[i].session_id, session_id) == 0)
        {
            ESP_LOGI(TAG, "Session found for ID: %s", session_id);
            return &ctx->sessions[i];
        }
    }
    ESP_LOGW(TAG, "Session not found for ID: %s", session_id);
    return NULL;
}

session_t *create_session()
{
    ESP_LOGI(TAG, "Creating new session");
    for (int i = 0; i < MAX_SESSIONS; i++)
    {
        if (ctx->sessions[i].session_id[0] == '\0')
        {
            generate_session_id(ctx->sessions[i].session_id, sizeof(ctx->sessions[i].session_id));
            ctx->sessions[i].logged_in = true;
            ESP_LOGI(TAG, "New session created with ID: %s", ctx->sessions[i].session_id);
            return &ctx->sessions[i];
        }
    }
    ESP_LOGE(TAG, "No available slots for new session");
    return NULL;
}

void delete_session(const char *session_id)
{
    ESP_LOGI(TAG, "Deleting session for ID: %s", session_id);
    session_t *session = find_session(session_id);
    if (session)
    {
        session->session_id[0] = '\0';
        session->logged_in = false;
        ESP_LOGI(TAG, "Session deleted for ID: %s", session_id);
    }
    else
    {
        ESP_LOGW(TAG, "Session not found for deletion with ID: %s", session_id);
    }
}


esp_err_t check_user_logged_in(httpd_req_t *req, session_t **session)
{
    char session_id[33];
    size_t len = httpd_req_get_hdr_value_len(req, "Cookie");
    if (len > 0)
    {
        char *cookie = malloc(len + 1);
        if (cookie)
        {
            httpd_req_get_hdr_value_str(req, "Cookie", cookie, len + 1);
            ESP_LOGI(TAG, "Cookies received: %s", cookie);
            char *session_id_start = strstr(cookie, "session_id=");
            if (session_id_start)
            {
                session_id_start += strlen("session_id=");
                char *session_id_end = strchr(session_id_start, ';');
                if (session_id_end)
                {
                    size_t session_id_length = session_id_end - session_id_start;
                    strncpy(session_id, session_id_start, session_id_length);
                    session_id[session_id_length] = '\0';
                }
                else
                {
                    strncpy(session_id, session_id_start, sizeof(session_id) - 1);
                    session_id[sizeof(session_id) - 1] = '\0';
                }
                ESP_LOGI(TAG, "Session ID from cookie: %s", session_id);
                *session = find_session(session_id);
                free(cookie);
                if (*session && (*session)->logged_in)
                {
                    ESP_LOGI(TAG, "User is logged in with session ID: %s", session_id);
                    return ESP_OK;
                }
                else
                {
                    ESP_LOGW(TAG, "User is not logged in with session ID: %s", session_id);
                    return ESP_FAIL;
                }
            }
            free(cookie);
        }
    }
    ESP_LOGW(TAG, "No session ID found in cookie");
    return ESP_FAIL;
}