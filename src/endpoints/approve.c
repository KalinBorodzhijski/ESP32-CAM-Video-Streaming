#include "esp_log.h"
#include "esp_http_server.h"
#include "endpoints/approve.h"
#include "utils/nvs_utils.h"
#include "utils/utils.h"

esp_err_t approve_get_handler(httpd_req_t *req)
{
    char username[32];
    char buf[100];
    int ret, remaining = req->content_len;

    while (remaining > 0)
    {
        if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0)
        {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT)
            {
                continue;
            }
            return ESP_FAIL;
        }
        remaining -= ret;
    }

    sscanf(buf, "username=%31s", username);

    esp_err_t err = nvs_approve_user(username);
    if (err != ESP_OK)
    {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    httpd_resp_send(req, "User approved successfully", HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}