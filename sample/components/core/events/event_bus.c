#include "event_bus.h"
#include "clickey.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Event bus queue handles
QueueHandle_t event_to_bus_queue = NULL;
QueueHandle_t event_from_bus_queue = NULL;

// Method to initialize the event bus
void event_bus_init(void) {
    // Create queues for event communication
    event_to_bus_queue = xQueueCreate(10, sizeof(click_event_t)); 
    event_from_bus_queue = xQueueCreate(10, sizeof(click_event_t));
}

// Bus relay method to publish an event
void event_bus_relay() {
    click_event_t event_buffer;
    // Check for events from the clickey module
    if (xQueueReceive(event_to_bus_queue, &event_buffer, pdMS_TO_TICKS(10)) == pdPASS) {
        // Relay the event to the from_bus_queue for other modules to consume
        xQueueSend(event_from_bus_queue, &event_buffer, pdMS_TO_TICKS(10));
    }
}
