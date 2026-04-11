#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "sound_module.h"
#include "event_bus.h"
#include "esp_log.h"
#include "buzzer.h"

static const char *TAG = "SOUND_TASK";

extern QueueHandle_t temperature_to_buzzer;

// Implementing a sound beeping Task
void sound_task() {
    double temperature = 0;
    
    while(1) {
        if(xQueueReceive(temperature_to_buzzer, &temperature, pdMS_TO_TICKS(10))) {
            ESP_LOGI(TAG, "Received Temp: %.2f", temperature);
            buzzer_beep();

            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }

}