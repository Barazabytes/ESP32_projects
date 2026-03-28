#include "esp_http_server.h"
#include "esp_spiffs.h"
#include "esp_log.h"
#include "esp_err.h"
#include "http_server.h"

const char *S_TAG = "HTTP_SERVER";



// HTTP fetch Reponse generator, for root uri

/**
 * Mounting the file system for file Read operation
 */

static void mounting_file_system() { 

    esp_vfs_spiffs_conf_t spiffs_config = {
        .base_path = "/storage",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };

    esp_err_t check = esp_vfs_spiffs_register(&spiffs_config);
        if(check != ESP_OK) {
            if(check == ESP_FAIL)
                ESP_LOGE(S_TAG, "Failed to mount partition");
            
            if(check == ESP_ERR_NOT_FOUND)
                ESP_LOGE(S_TAG, "Partition not found");

            return;
        } else 
            ESP_LOGI(S_TAG, "Mounting was successful");

}





static esp_err_t root_handler(httpd_req_t *req) {
    
    // Mount a storage first.
    mounting_file_system();


    // Reading and respond a file.
    FILE *fptr;
    fptr = fopen("/storage/index.html", "r");
        if(fptr == NULL) {
            httpd_resp_send_404(req);
            ESP_LOGE(S_TAG, "Failed to read HTML file");

            return ESP_FAIL;
        }

    httpd_resp_set_type(req, "text/html");
    char buffer[1024];
    size_t byte_read;
    while((byte_read = fread(buffer, 1, sizeof(buffer), fptr)))
        httpd_resp_send_chunk(req, buffer, byte_read);
    
    fclose(fptr);
        httpd_resp_send_chunk(req, NULL, 0);  // Telling that, it's over.
    
    return ESP_OK;

}


// Root handler
    static httpd_uri_t root_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = root_handler,
        .user_ctx = NULL
    };



// Starting the HTTP server
httpd_handle_t start_web_server() {
    httpd_handle_t server_instance = NULL;
    httpd_config_t server_config = HTTPD_DEFAULT_CONFIG();

    // Starting the actual server
        esp_err_t check = httpd_start(&server_instance, &server_config);
        if(check == ESP_OK) {
            httpd_register_uri_handler(server_instance, &root_uri);
        } else
            ESP_LOGE(S_TAG, "Failed to start Server (%s)", esp_err_to_name(check));
        
    return server_instance;        
}