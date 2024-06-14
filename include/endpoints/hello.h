#ifndef HELLO_H
#define HELLO_H

#include "esp_http_server.h"

esp_err_t hello_get_handler(httpd_req_t *req);

#endif // HELLO_H
