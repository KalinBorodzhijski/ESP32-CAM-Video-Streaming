#ifndef LOGIN_H
#define LOGIN_H

#include "esp_http_server.h"
#include "server.h"

esp_err_t login_post_handler(httpd_req_t *req);

#endif // LOGIN_H