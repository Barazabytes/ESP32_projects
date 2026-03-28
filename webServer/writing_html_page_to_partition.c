#include "esp_spiffs.h"
#include "esp_log.h"
#include "esp_err.h"
#include <stdio.h>


/**
 * @author
 *      Said Baraza (Barazabytes)
 *      Youtube: https://www.youtube.com/@Barazabytes
 *      GitHub: https://github.com/Barazabytes
 *      LinkedIn: https://www.linkedin.com/in/said-baraza
 *      instagram: https://www.instagram.com/barazabytes/
 * 
 * 
 * @brief
 *      This file is responsible for writing the HTML page to the SPIFFS partition, and then serve it to the client when requested.
 *      The HTML page is stored in the "storage" partition, which is mounted to the "/storage" path. The HTML page is read from the file system and then sent to the client
 *          - This code was ran earlier before the server is started, but it can be moved to the root handler if needed. However, it is better to run it earlier to avoid the overhead of mounting the file system and reading the file every time the client requests the root URI.
 */


// The function is responsible for mounting the SPIFFS partition and writing the HTML page to it. It is called before starting the server to ensure that the HTML page is available when the client requests it.
void write_html_page_to_partition() {

    // Mounting the file system first.
    esp_vfs_spiffs_conf_t spiffs_config = {
        .base_path = "/storage",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };

    esp_err_t check = esp_vfs_spiffs_register(&spiffs_config);
        if(check != ESP_OK) {
            if(check == ESP_FAIL)
                ESP_LOGE("SPIFFS", "Failed to mount partition");
            
            if(check == ESP_ERR_NOT_FOUND)
                ESP_LOGE("SPIFFS", "Partition not found");

            return;
        } else 
            ESP_LOGI("SPIFFS", "Mounting was successful");

    // Writing the HTML page to the file system.
    FILE *fptr;
    fptr = fopen("/storage/index.html", "w");
        if(fptr == NULL) {
            ESP_LOGE("SPIFFS", "Failed to open file for writing");
            return;
        }

    const char *html_content = 
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "    <title>ESP32 Web Server</title>"
        "</head>"
        "<body>"
        "    <h1>Hello From SPIFFS!</h1>"
        "    <p>This page was created during runtime.</p>"
        "</body>"
        "</html>";

    fprintf(fptr, "%s", html_content);
    fclose(fptr);

    ESP_LOGI("SPIFFS", "HTML page written to partition successfully");
}