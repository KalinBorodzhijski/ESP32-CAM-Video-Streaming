#ifndef SERVER_H
#define SERVER_H

#include <stdbool.h>

#define MAX_SESSIONS 4

typedef struct {
    char session_id[33];
    bool logged_in;
} session_t;

typedef struct {
    session_t sessions[MAX_SESSIONS];
} server_context_t;

void start_server();

#endif // SERVER_H
