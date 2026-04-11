#pragma once

#include <stdbool.h>
#include <stdint.h>


typedef struct {
    uint8_t  buzzer_pin;
    uint32_t beep_delay;
    bool     buzzer_level;
} buzzer_t;


// Buzzer Initializing Method
void buzzer_init(uint8_t pin, uint32_t beep_delay);

// Buzzer set level
void buzzer_toggle();

// Buzzer beep sound
void buzzer_beep();