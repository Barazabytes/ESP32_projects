idf_component_register(
    SRCS "WIFI/wifi_service_access_point.c" "WIFI/wifi_service_station.c" "HTTP/http_server.c"
    INCLUDE_DIRS "WIFI/include" "HTTP/include"
    REQUIRES esp_wifi esp_event nvs_flash spiffs esp_http_server
)