idf_component_register(
    SRCS "weather_module.c" "weather_task.c"
    INCLUDE_DIRS "include"
    REQUIRES drivers core
)