#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "clickey.h"
#include "BUTTON.h"


clickey_config_t clickey_config;
    
extern void clickey_task(void *ard);

// Clickey module initialization
void clickey_init(uint8_t button_pin) {
    clickey_config.pin = button_pin;

    // Initialize the button with the specified pin
    button_init(clickey_config.pin);

    // Create a FreeRTOS task to handle click events
    xTaskCreatePinnedToCore(clickey_task, "clickey_task", 2048, NULL, 1, NULL, 1);
}

