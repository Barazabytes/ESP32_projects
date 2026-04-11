/**
 * @author Said Baraza
 * @file   Main file, where system starts Running
 * 
 */

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "weather_module.h"
#include "freertos/task.h"
#include "sound_module.h"
#include "wifi_service.h"
#include "http_server.h"
#include "event_bus.h"
#include "esp_log.h"

char *ssid = "ByteLink_6G";
char *password = "miminawewe";

void app_main(void) {

    wifi_service_init_ap(ssid, password);
    start_web_server();

    weather_module_init(4);
    event_bus_init();
    sound_module_init(5);

}
