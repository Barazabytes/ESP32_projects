/**
 * @file http_server.h
 * @author  Said Baraza (barazars45@gmail.com)
 * @brief   This file contains the declaration of the function to start the HTTP server.
 * @version 0.1
 * @date    2026-03-31
 * 
 */


#pragma once

#include <stdint.h>
#include "esp_http_server.h"


// Starting the Server
httpd_handle_t start_web_server();