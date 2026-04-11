idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES drivers modules services)

spiffs_create_partition_image(storage ../components/services/HTTP/pages FLASH_IN_PROJECT)
