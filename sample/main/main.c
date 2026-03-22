#include "driver/gpio.h"
#include "event_bus.h"
#include "clickey.h"
#include "blinkey.h"

void app_main(void) {
    
    
   // Initialize the event bus
    event_bus_init();
   // Initialize the clickey 
    clickey_init(GPIO_NUM_0);

   // Initialize the blinkey
    blinkey_init(GPIO_NUM_2, 500); // Blink every 500 ms

}