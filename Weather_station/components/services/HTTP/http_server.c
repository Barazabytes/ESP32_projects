/**
 * @file http_server.c
 * @author Said Baraza (barazars45@gmail.com)
 * @brief This file contains the implementation of the Web Server, and the URI handlers.
 * @version 0.1
 * @date 2026-03-31
 * 
 */

#include "esp_http_server.h"
#include "http_server.h"
#include "esp_spiffs.h"
#include "esp_err.h"
#include "esp_log.h"
#include "cJSON.h"


extern QueueHandle_t temperature_to_http;
extern QueueHandle_t humidity_to_http;

static const char *TAG = "SERVER";

/**
 * @brief
 *      - Creating a Method to mount storage to Flash Memory
 ***********************************************************************************/

static void mount_storage_to_flash() {
    // The mounting
    esp_vfs_spiffs_conf_t spiffs_config = {
        .base_path = "/storage",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };

    esp_err_t check = esp_vfs_spiffs_register(&spiffs_config);
        if(check != ESP_OK) {
            if(check == ESP_FAIL)
                ESP_LOGE(TAG, "Failed to mount the storage");

            if(check == ESP_ERR_NOT_FOUND)
                ESP_LOGE(TAG, "Failed to file the storage partition");

            return;
        } else
            ESP_LOGI(TAG, "Mounting was successful");

}


/**
 * @brief
 *      - Implementing uri Functions
 ***********************************************************************************/

// extern float send_temp_to_browser();
// extern float send_hum_to_browser ();

// creating the uri handler functions
static esp_err_t root_uri_function(httpd_req_t *req) {
    // mount the storage first
        mount_storage_to_flash();

    // Reading the web page
        FILE *file;
        file = fopen("/storage/cJSON.html", "r");
            if(file == NULL) {
                ESP_LOGE(TAG, "Failed to read index page from storage/pages");
                httpd_resp_send_500(req);

                return ESP_FAIL;
            }

        httpd_resp_set_type(req, "text/html");

        char buffer[1024];
        size_t bytes_read;
        
        // reading chunks of WebPage
            while((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
                httpd_resp_send_chunk(req, buffer, bytes_read);

        fclose(file);
            httpd_resp_send_chunk(req, NULL, 0); // Telling the page is over
        
    return ESP_OK;

}

static esp_err_t data_uri_function(httpd_req_t *req) {

 // cJSON object container
    cJSON *root = cJSON_CreateObject();

    // Adding sensor Data
    double temperature = 0;
    double humidity    = 0;

// cJSON object container
    if(xQueueReceive(temperature_to_http, &temperature, pdMS_TO_TICKS(10))) {
        ESP_LOGI(TAG, "Received Temp: %.2f", temperature);
        cJSON_AddNumberToObject(root, "temperature", temperature);
        
        if(xQueueReceive(humidity_to_http, &humidity, pdMS_TO_TICKS(10))) {
            ESP_LOGI(TAG, "Received Hum: %.2f", humidity);
            cJSON_AddNumberToObject(root, "humidity", humidity);

        }
    }

    cJSON_AddStringToObject(root, "status", "Normal");

    // Serializing JSON
    const char *json_string = cJSON_PrintUnformatted(root);

    // set and send the response
    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, json_string);

    // Clearing memory
    free((void*)json_string);
    cJSON_Delete(root);

 return ESP_OK;

}




/**
 * @brief
 *      - Implementing the URI handlers
 ***********************************************************************************/

 // Root handler 
 httpd_uri_t root_uri = {
    .uri =      "/",
    .method =   HTTP_GET,
    .handler =  root_uri_function,
    .user_ctx = NULL
 };

 // index handler
 httpd_uri_t data_uri = {
    .uri =      "/json_data",
    .method =   HTTP_GET,
    .handler =  data_uri_function,
    .user_ctx = NULL
 };  


 /**
 * @brief
 *      - Starting the Web Server
 ***********************************************************************************/

 httpd_handle_t start_web_server() {
    // The handles and configs of a server
        httpd_handle_t server_instance = NULL;
        httpd_config_t server_config = HTTPD_DEFAULT_CONFIG();

        esp_err_t check = httpd_start(&server_instance, &server_config);
            if(check == ESP_OK) {
                ESP_LOGI(TAG, "Server Started successful");

                // registering the uri handlers
                    httpd_register_uri_handler(server_instance, &root_uri);
                    httpd_register_uri_handler(server_instance, &data_uri);
            } else 
                ESP_LOGE(TAG, "Failed to start Server");

    return server_instance;
 }

