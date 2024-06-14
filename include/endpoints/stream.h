#ifndef STREAM_H
#define STREAM_H

#include "esp_http_server.h"
#include "server.h"

esp_err_t stream_get_handler(httpd_req_t *req);

#endif // STREAM_H
