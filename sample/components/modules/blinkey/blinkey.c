#include "blinkey.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "LED.h"

blinkey_config_t blinkey_config;
extern void blinkey_task(void *arg);

void blinkey_init(uint8_t led_pin, uint32_t delay_ms) {
    blinkey_config.pin = led_pin;
    blinkey_config.delay_ms = delay_ms;
 
    // Initialize the LED pin
    led_init(led_pin);

    // Create a task for blinking the LED
    xTaskCreatePinnedToCore(blinkey_task, "Blinkey Task", 2048, NULL, 5, NULL, 1);
}