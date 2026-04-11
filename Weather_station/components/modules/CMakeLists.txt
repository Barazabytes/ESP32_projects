idf_component_register(
    SRCS "weather/weather_module.c" "weather/weather_task.c" "sound/sound_module.c" "sound/sound_task.c"
    INCLUDE_DIRS "weather/include" "sound/include"
    REQUIRES drivers core
)