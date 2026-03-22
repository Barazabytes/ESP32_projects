#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    CLICK_EVENT_NONE = 0,
    CLICK_EVENT_PRESSED,
    CLICK_EVENT_HELD
} click_event_t;

typedef struct {
    uint8_t pin;  
} clickey_config_t;


// Clickey module initialization
void clickey_init(uint8_t button_pin);
