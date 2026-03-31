/**
 * @file main.c
 * @author  Said Baraza (barazars45@gmail.com)
 * @brief
 *     - This is the main file of the project, it initializes the wifi and starts the web server.
 * @version 0.1
 * @date 2026-03-31
 * 
 */


#include "wifi_service.h"
#include "http_server.h"


char *ssid = "ByteLink_6G";
char *password = "miminawewe";

void app_main(void) {
    wifi_service_init_ap(ssid, password);

    start_web_server();
}
