#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    HAL_GPIO_MODE_INPUT,
    HAL_GPIO_MODE_OUTPUT
} hal_gpio_mode_t;

// GPIO pin Initialization
void hal_gpio_init(uint8_t pin, hal_gpio_mode_t mode);

// Write to a GPIO
void hal_gpio_write(uint8_t pin, bool level);

// Read from a GPIO
bool hal_gpio_read(uint8_t pin);
