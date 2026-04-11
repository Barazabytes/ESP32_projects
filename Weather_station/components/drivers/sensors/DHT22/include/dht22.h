#pragma once

#include <stdint.h>


typedef struct {
    double humidity;
    double temperature;
}dht_reading_t;

typedef enum {
  DHT_OK = 0,
  DHT_ERROR_CHECKSUM = -1,
  DHT_ERROR_TIMEOUT = -2,
  DHT_INVALID_VALUE = -999
} dht_result_t;

typedef struct {
    uint8_t gpio_pin;
} dht22_t;

// Method to initialize DHT22 Driver
void DHT22_driver_init(uint8_t gpio_pin);

dht_result_t read_dht_sensor_data();


// Returning temperature x Humidity
double getTemperature();
double getHumidity();
