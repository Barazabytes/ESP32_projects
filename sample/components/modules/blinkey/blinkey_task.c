#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "event_bus.h"
#include "clickey.h"
#include "blinkey.h"
#include "LED.h"


extern QueueHandle_t event_from_bus_queue;
extern blinkey_config_t blinkey_config;


static click_event_t read_event_from_bus() {
    click_event_t event_buffer;
    if (xQueueReceive(event_from_bus_queue, &event_buffer, pdMS_TO_TICKS(10)) == pdTRUE) {
        return event_buffer;
    }

    return CLICK_EVENT_NONE;

}


// Blinkey Task function
void blinkey_task(void *arg) {
    while (1) {
        if (read_event_from_bus() == CLICK_EVENT_PRESSED)
            led_toggle();

        if (read_event_from_bus() == CLICK_EVENT_HELD) {
            for(int i = 0; i < 5; i++) {
                led_on();
                vTaskDelay(pdMS_TO_TICKS(blinkey_config.delay_ms));
                led_off();
                vTaskDelay(pdMS_TO_TICKS(blinkey_config.delay_ms));
            }

        }
    }
}