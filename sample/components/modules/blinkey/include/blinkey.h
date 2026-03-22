#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t pin;
    uint32_t delay_ms;
} blinkey_config_t;

// Blinkey Initialization function
void blinkey_init(uint8_t led_pin, uint32_t delay_ms);

