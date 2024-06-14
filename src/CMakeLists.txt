idf_component_register(
    SRCS "main.c"
         "server/server.c"
         "wifi_config.c"
         "nvs_utils.c"
         "endpoints/hello.c"
         "endpoints/home.c"
         "endpoints/login.c"
         "endpoints/stream.c"
         "endpoints/logout.c"
    INCLUDE_DIRS "../include"
    EMBED_FILES "web/home_page/index.html" "web/home_page/styles.css"
)