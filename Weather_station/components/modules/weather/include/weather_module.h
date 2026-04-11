#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t  gpio_num;
    uint32_t task_delay;
} weather_module_t;



// Weather Module Initializer
void weather_module_init(uint8_t gpio_pin);

uint32_t set_delay();