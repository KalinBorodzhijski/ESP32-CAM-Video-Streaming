#ifndef HOME_H
#define HOME_H

#include "esp_http_server.h"
#include "server.h"

esp_err_t root_get_handler(httpd_req_t *req);
esp_err_t css_get_handler(httpd_req_t *req);

#endif // HOME_H
