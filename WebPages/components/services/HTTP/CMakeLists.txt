idf_component_register(
    SRCS "http_server.c"
    INCLUDE_DIRS "include"
    REQUIRES spiffs esp_http_server)


spiffs_create_partition_image(storage ../pages FLASH_IN_PROJECT)