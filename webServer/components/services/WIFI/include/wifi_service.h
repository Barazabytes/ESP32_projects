#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    char    *wifi_ssid;
    char    *wifi_password;
} wifi_service_t;


// Initializing Wi-Fi station mode
void wifi_service_init_sta(char *ssid, char *password);

// Initializing Wi-Fi access point mode
void wifi_service_init_ap(char *ssid, char *password);
