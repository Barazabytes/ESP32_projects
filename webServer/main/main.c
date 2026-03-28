#include <stdio.h>
#include "wifi_service.h"
#include "esp_spiffs.h"
#include "esp_err.h"
#include "esp_log.h"
#include "http_server.h"

const char *TAG = "SPIFFS";

char *ssid = "ByteLink_6G";
char *pass = "miminawewe";

void app_main(void) {
    // Initializing Wifi AP mode
        wifi_service_init_ap(ssid, pass);
        start_web_server();
    }
