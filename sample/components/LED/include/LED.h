#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "hal_gpio.h"

typedef struct {
    uint8_t pin;
    bool active_state; // true for active high, false for active low
    hal_gpio_mode_t mode; // GPIO mode
} led_config_t;


// Initializes the LED with the given configuration.
void led_init(uint8_t pin);


// Turns the LED on.
void led_on();

// Turns the LED off.
void led_off();

// Toggles the LED state.
void led_toggle();
