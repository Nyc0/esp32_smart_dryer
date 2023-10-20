idf_component_register(SRCS  "http_server.c" "wifi_app.c" "app_nvs.c" "main.c" "rgb_led.c" "wifi_reset_button.c" "sntp_time_sync.c" "mqtt_app.c"
                    INCLUDE_DIRS "."
                    EMBED_FILES webpage/app.css webpage/app.js webpage/favicon.ico webpage/index.html webpage/jquery-3.3.1.min.js
                    )