#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t pin;
    bool state; // true for on, false for off
} button_config_t;

// Button initialization
void button_init(uint8_t pin);

// Reads the button state.
// Returns true if pressed, false otherwise.
bool button_read();

// Toggles the button state (for testing purposes).
void button_toggle();