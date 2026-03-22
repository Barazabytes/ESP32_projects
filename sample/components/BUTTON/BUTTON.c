#include "hal_gpio.h"
#include "BUTTON.h"

static button_config_t button;

// Button initialization
void button_init(uint8_t pin) {
    button.pin = pin;
    button.state = 0; // Default to not pressed

    hal_gpio_init(pin, HAL_GPIO_MODE_INPUT); // Initialize the GPIO pin as input
}

// Reads the button state.
// Returns true if pressed, false otherwise.
bool button_read() {
    button.state = hal_gpio_read(button.pin);
    return button.state;
}

// Toggles the button state (for testing purposes).
void button_toggle() {
    button.state = !button.state;
    hal_gpio_write(button.pin, button.state);
}