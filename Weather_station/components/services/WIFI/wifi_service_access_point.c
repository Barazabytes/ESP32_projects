#include "wifi_service.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_mac.h"

#include "lwip/err.h"
#include "lwip/sys.h"

static const char *TAG = "WIFI_SERVICE_AP";
wifi_service_t wifi_service;
const uint8_t WIFI_SERVICE_MAX_STA = 5;


#define WIFI_SERVICE_CHANNEL 6


static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data) {
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {

        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d", MAC2STR(event->mac), event->aid);

    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d, reason=%d", MAC2STR(event->mac), event->aid, event->reason);
    }
}

void wifi_service_init_ap(char *ssid, char *password) {

// Storing SSID and password in wifi_service struct
    wifi_service.wifi_ssid = ssid;
    wifi_service.wifi_password = password;

// Initializing nvs flash, network interface, event loop, and Wi-Fi driver

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = "",
            .ssid_len = strlen(wifi_service.wifi_ssid),
            .channel = WIFI_SERVICE_CHANNEL,
            .password = "",
            .max_connection = WIFI_SERVICE_MAX_STA,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                    .required = true,
            },
        },
    };

    if (strlen(wifi_service.wifi_password) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

// Copying SSID and password to wifi_config
    strncpy((char *)wifi_config.ap.ssid, wifi_service.wifi_ssid, sizeof(wifi_config.ap.ssid) - 1);
    strncpy((char *)wifi_config.ap.password, wifi_service.wifi_password, sizeof(wifi_config.ap.password) - 1);

// Setting Wi-Fi mode to access pointand starting the Wi-Fi service
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             wifi_service.wifi_ssid, wifi_service.wifi_password, WIFI_SERVICE_CHANNEL);    
}