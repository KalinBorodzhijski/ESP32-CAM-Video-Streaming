#ifndef LOGOUT_H
#define LOGOUT_H

#include "esp_http_server.h"
#include "server.h"

esp_err_t logout_post_handler(httpd_req_t *req);

#endif // LOGOUT_H
