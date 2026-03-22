#include <stdlib.h>
#include "hal_gpio.h"
#include "LED.h"

// static led_config_t *led;
static led_config_t led;

// Initializes the LED with the given configuration.
void led_init(uint8_t pin) {
    led.pin = pin;
    led.active_state = 0; // Default to off
    led.mode = HAL_GPIO_MODE_OUTPUT;

    // Passing the Initialization to HAL layer
    hal_gpio_init(led.pin, led.mode);

}


// Turns the LED on.
void led_on() {
    led.active_state = 1;

    hal_gpio_write(led.pin, led.active_state);
}
    
// Turn LED off
void led_off() {
    led.active_state = 0;

    hal_gpio_write(led.pin, led.active_state);
}

// Toggles the LED state.
void led_toggle() {
    led.active_state = !led.active_state;

    hal_gpio_write(led.pin, led.active_state);
}