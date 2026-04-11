#include "event_bus.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


// Event bus task function
void event_bus_task(void *arg) {
    while (1){
        event_bus_relay();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
}