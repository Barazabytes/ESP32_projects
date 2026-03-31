/**
 * @file wifi_service_station.c
 * @author Said Baraza (barazars45@gmail.com)
 * @brief This file contains the implementation of the Wi-Fi service for station mode. It initializes the Wi-Fi driver, connects to the specified access point, and handles Wi-Fi events such as connection and disconnection.
 * @version 1.0
 * @date 2026-03-31
 * 
 */



#include "wifi_service.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

uint8_t retry_count = 0;
wifi_service_t wifi_service;
static EventGroupHandle_t s_wifi_event_group;
static const char *TAG = "WIFI_SERVICE_STATION";

const uint8_t MAX_RETRYS = 5;

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        
        if (retry_count < MAX_RETRYS) {
            esp_wifi_connect();
            retry_count++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else 
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        
        ESP_LOGI(TAG,"connect to the AP fail");

    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        retry_count = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}


void wifi_service_init_sta(char *ssid, char *password) {

    // Initialize the wifi_service struct with the provided parameters
    wifi_service.wifi_ssid = ssid;
    wifi_service.wifi_password = password;

// Initialize NVS, Wi-Fi, and event loop
    ESP_ERROR_CHECK(nvs_flash_init());

    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

// Configure Wi-Fi driver with default settings
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

//  Configure Wi-Fi connection settings

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "",
            .password = ""
        }
    };

// Copy the SSID and password from the wifi_service struct to the wifi_config struct
    strncpy((char*)wifi_config.sta.ssid, wifi_service.wifi_ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char*)wifi_config.sta.password, wifi_service.wifi_password, sizeof(wifi_config.sta.password));

// Set the Wi-Fi mode to station and apply the configuration
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    
// Start the Wi-Fi driver 
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_sta finished. Waiting for connection...");

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 wifi_service.wifi_ssid, wifi_service.wifi_password);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 wifi_service.wifi_ssid, wifi_service.wifi_password);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

}