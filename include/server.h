#ifndef SERVER_H
#define SERVER_H

typedef struct {
    bool user_logged_in;
} server_context_t;

void start_server();

#endif // SERVER_H
