#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "server.h"
#include <stdio.h>
#include "esp_err.h"
#include "esp_http_server.h"


void init_session_manager(server_context_t *context);
void generate_session_id(char *session_id, size_t size);
session_t *find_session(const char *session_id);
session_t *create_session();
void delete_session(const char *session_id);
esp_err_t check_user_logged_in(httpd_req_t *req, session_t **session);

#endif // SESSION_MANAGER_H