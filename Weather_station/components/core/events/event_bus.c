/**
 * @brief This is a relay to all Modules, coordinating them using Queues
 * @file  This file is the implementation of "event_bus.h"
 */


#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "event_bus.h"
#include "esp_log.h"

#define MAX_TEMP 30.0

static const char *TAG = "EVENT_BUS";

QueueHandle_t temperature_from_weather = NULL;
QueueHandle_t humidity_from_weather    = NULL;

QueueHandle_t temperature_to_buzzer    = NULL;
QueueHandle_t temperature_to_http      = NULL;
QueueHandle_t humidity_to_http         = NULL;

extern void event_bus_task();

// Method to initialize the event bus
void event_bus_init(void) {
    temperature_from_weather = xQueueCreate(10, sizeof(double));
    humidity_from_weather    = xQueueCreate(10, sizeof(double));

    temperature_to_http      = xQueueCreate(10, sizeof(double));
    humidity_to_http         = xQueueCreate(10, sizeof(double));
    temperature_to_buzzer    = xQueueCreate(5,  sizeof(double));


    xTaskCreatePinnedToCore(event_bus_task, "Event Bus Task", 2048, NULL, 5, NULL, 1);
}


// Bus relay method to publish an event
void event_bus_relay() {
    double temperature = 0;
    double humidity    = 0;

    if(xQueueReceive(temperature_from_weather, &temperature, pdMS_TO_TICKS(10))) {
        ESP_LOGI(TAG, "Received Temp: %.2f", temperature);
            if(temperature > MAX_TEMP)
                xQueueSend(temperature_to_buzzer, &temperature, pdMS_TO_TICKS(10));

            xQueueSend(temperature_to_http, &temperature, pdMS_TO_TICKS(10));

        if(xQueueReceive(humidity_from_weather, &humidity, pdMS_TO_TICKS(10))) {
            xQueueSend(humidity_to_http, &humidity, pdMS_TO_TICKS(10));
            ESP_LOGI(TAG, "Received Hum: %.2f", humidity);
        }

    }
}