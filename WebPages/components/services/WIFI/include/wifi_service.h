/**
 * @file wifi_service.h
 * @brief Wi-Fi service header file for ESP32
 *      - This file defines the interface for initializing Wi-Fi in both station and access point modes.
 *      - It includes the necessary data structures and function prototypes for managing Wi-Fi connections.
 *      - The implementation of these functions will be in the corresponding .c file, where the actual logic for setting up Wi-Fi will be handled.
 * 
 * @author Said Baraza
 * @version 1.0
 * @date 2026-03-31
 * 
 */


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
