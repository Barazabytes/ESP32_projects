#include "freertos/FreeRTOS.h"
#include "weather_module.h"
#include "freertos/task.h"
#include "dht22.h"

#define DATA_DELAY_MS 1000

// static const char *TAG = "WEATHER_MODULE";
weather_module_t weather;


extern void weather_task();

// Weather Module Initializer
void weather_module_init(uint8_t gpio_pin) {
    weather.gpio_num    = gpio_pin;
    weather.task_delay  = DATA_DELAY_MS;

    DHT22_driver_init(weather.gpio_num);

    xTaskCreatePinnedToCore(weather_task, "weather Task", 2048, NULL, 5, NULL, 1);
}


// Sending delay to the weather Task
uint32_t set_delay() {
    return weather.task_delay;
}