#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "clickey.h"
#include "BUTTON.h"


uint64_t last, now, duration;
extern QueueHandle_t event_to_bus_queue;


static void send_click_event_to_bus(click_event_t event) {
    xQueueSend(event_to_bus_queue, &event, pdMS_TO_TICKS(10));
}

void clickey_task(void *arg) {
    last = esp_timer_get_time()/1000;
        while(1) {
            if(button_read()) {
                now = esp_timer_get_time()/1000;
                duration = now - last;

                    if(duration < 200)
                        send_click_event_to_bus(CLICK_EVENT_PRESSED);
                    
                    if(duration >= 1000)    
                        send_click_event_to_bus(CLICK_EVENT_HELD);
                    
                    send_click_event_to_bus(CLICK_EVENT_NONE);
                last = now;
            }
        }
}
