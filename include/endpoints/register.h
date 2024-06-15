#ifndef REGISTER_H
#define REGISTER_H

#include "esp_http_server.h"
#include "server.h"

esp_err_t register_post_handler(httpd_req_t *req);

#endif // REGISTER_H
