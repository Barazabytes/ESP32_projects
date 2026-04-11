#pragma once

#include <stdbool.h>
#include <stdint.h>


typedef enum {
    HAL_GPIO_MODE_INPUT,
    HAL_GPIO_MODE_OUTPUT,
    HAL_GPIO_MODE_OUTPUT_OD,
    HAL_GPIO_MODE_INPUT_OD,
    HAL_GPIO_MODE_INPUT_OUTPUT_OD
} hal_gpio_mode_t;

typedef enum {
    HAL_GPIO_PULLUP_ENABLE,
    HAL_GPIO_PULLUP_DISABLE
} hal_gpio_pullup_t;

typedef enum {
    HAL_GPIO_PULLDOWN_ENABLE,
    HAL_GPIO_PULLDOWN_DISABLE
} hal_gpio_pulldown_t;

typedef enum {
    HAL_GPIO_INTR_POSEDGE,
    HAL_GPIO_INTR_NEGEDGE,
    HAL_GPIO_INTR_ANYEDGE,
    HAL_GPIO_INTR_DISABLE
}hal_interupt_t;


// GPIO Initialization Method.
void hal_gpio_init(uint8_t pin, hal_gpio_mode_t mode, hal_gpio_pullup_t pullup, hal_gpio_pulldown_t pulldown);

// GPIO write Method
void hal_gpio_write(uint8_t pin, bool level);

// GPIO Read Method
bool hal_gpio_read(uint8_t pin);

// set direction
void hal_gpio_set_direction(uint8_t pin, hal_gpio_mode_t mode);

// set interrupt type
void hal_gpio_set_intr_type(uint8_t, hal_interupt_t intr_type);

// Disabling pullup and pulldown
void hal_gpio_pulldown_dis(uint8_t gpio_pin);
void hal_gpio_pullup_dis(uint8_t gpio_pin);