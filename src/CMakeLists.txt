idf_component_register(
    SRCS "main.c"
         "server/server.c"
         "wifi_config.c"
         "utils/nvs_utils.c"
         "utils/hash_utils.c"
         "utils/email_utils.c"
         "session_manager.c"
         "endpoints/hello.c"
         "endpoints/home.c"
         "endpoints/login.c"
         "endpoints/stream.c"
         "endpoints/logout.c"
         "endpoints/register.c"
         "endpoints/approve.c"
    INCLUDE_DIRS "../include"
    EMBED_FILES "web/home_page/styles.css"
)