/**
 * @file http_server.c
 * @author Ahmed Elghandour (barazars45@gmail.com)
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



// creating the uri handler functions
static esp_err_t root_uri_function(httpd_req_t *req) {
    // mount the storage first
        mount_storage_to_flash();

    // Reading the web page
        FILE *file;
        file = fopen("/storage/index.html", "r");
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

static esp_err_t signin_uri_function(httpd_req_t *req) {
    // Mount the storage to flash
        mount_storage_to_flash();

    // Read the signin page
        FILE *file;
        file = fopen("/storage/signin.html", "r");
            if(file == NULL) {
                ESP_LOGE(TAG, "Failed to read sign in page from storage/pages");
                httpd_resp_send_500(req);

                return ESP_FAIL;
            }

        httpd_resp_set_type(req, "text/html");

        char buffer[1024];
        size_t byte_read;
            while((byte_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
                httpd_resp_send_chunk(req, buffer, byte_read);

        fclose(file);
            httpd_resp_send_chunk(req, NULL, 0); // Telling reading is over/done


    return ESP_OK;

}

static esp_err_t styles_handler(httpd_req_t *req) {
    // Mounting storage to flash memory first
    mount_storage_to_flash();

    // Reading the styles file from storage.
    FILE *file = fopen("/storage/style.css", "r");
        if (file == NULL) {
            ESP_LOGE(TAG, "Failed to read styles file from Storage/pages");
            httpd_resp_send_500(req);
            
            return ESP_FAIL;
        }

    httpd_resp_set_type(req, "text/css");
    char buffer[1024];
    size_t read_bytes;
    
    // Reading chunks of the pages and print on Browser.
    while ((read_bytes = fread(buffer, 1, sizeof(buffer), file)) > 0)
        httpd_resp_send_chunk(req, buffer, read_bytes);
            
    fclose(file);

    httpd_resp_send_chunk(req, NULL, 0);
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
 httpd_uri_t index_uri = {
    .uri =      "/index.html",
    .method =   HTTP_GET,
    .handler =  root_uri_function,
    .user_ctx = NULL
 };  

 // signin handler
 httpd_uri_t signin_uri = {
    .uri =      "/signin.html",
    .method =   HTTP_GET,
    .handler =  signin_uri_function,
    .user_ctx = NULL
 };

 // styles handler
 httpd_uri_t style_uri = {
    .uri =      "/style.css",
    .method =   HTTP_GET,
    .handler =  styles_handler,
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
                    httpd_register_uri_handler(server_instance, &index_uri);
                    httpd_register_uri_handler(server_instance, &signin_uri);
                    httpd_register_uri_handler(server_instance, &style_uri);
            } else 
                ESP_LOGE(TAG, "Failed to start Server");

    return server_instance;
 }

