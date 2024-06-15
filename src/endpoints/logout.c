#include "esp_log.h"
#include "esp_http_server.h"
#include "endpoints/logout.h"
#include "session_manager.h"
#include "esp_log.h"

static const char *TAG = "logout";


esp_err_t logout_post_handler(httpd_req_t *req)
{
    char session_id[33];
    size_t len = httpd_req_get_hdr_value_len(req, "Cookie");

    ESP_LOGI(TAG, "Handling logout POST request");

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
                delete_session(session_id);
                ESP_LOGI(TAG, "User logged out with session ID: %s", session_id);
            }
            free(cookie);
        }
    }

    httpd_resp_sendstr(req, "<script>alert('Logout successful!'); window.location.href = '/';</script>");
    return ESP_OK;
}