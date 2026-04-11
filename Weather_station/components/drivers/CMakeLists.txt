idf_component_register(
    SRCS "actuators/BUZZER/buzzer.c" "sensors/DHT22/dht22.c"
    INCLUDE_DIRS "actuators/BUZZER/include" "sensors/DHT22/include"
    REQUIRES app_hal
)