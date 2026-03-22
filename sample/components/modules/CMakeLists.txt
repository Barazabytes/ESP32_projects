idf_component_register(
    SRCS "blinkey/blinkey.c" "blinkey/blinkey_task.c" "clickey/clickey.c" "clickey/clickey_task.c"
    INCLUDE_DIRS "blinkey/include" "clickey/include"

    REQUIRES LED BUTTON core esp_timer
)