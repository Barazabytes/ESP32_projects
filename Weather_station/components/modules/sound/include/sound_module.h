#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t  gpio_num;
    uint32_t beep_delay;
} sound_module_t;



// Weather Module Initializer
void sound_module_init(uint8_t gpio_pin);

uint32_t set_delay();