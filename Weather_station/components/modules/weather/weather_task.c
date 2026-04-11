#include "freertos/FreeRTOS.h"
#include "weather_module.h"
#include "freertos/task.h"
#include "event_bus.h"
#include "esp_log.h"
#include "dht22.h"

static const char *TAG = "WEATHER_MODULE_TASK";

extern QueueHandle_t temperature_from_weather;
extern QueueHandle_t humidity_from_weather;


void weather_task(){
    double temp, hum;

    while(1) {
        temp = getTemperature();
        hum  = getHumidity();

        xQueueSend(temperature_from_weather, &temp, pdMS_TO_TICKS(10));
        ESP_LOGI(TAG, "Temperature sent: %.2f", temp);
        xQueueSend(humidity_from_weather, &hum, pdMS_TO_TICKS(10));
        ESP_LOGI(TAG, "Humidity sent   : %.2f", hum);

        vTaskDelay(pdMS_TO_TICKS(set_delay()));
    }
}