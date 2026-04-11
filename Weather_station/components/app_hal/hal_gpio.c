#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "hal_gpio.h"
#include "esp_err.h"
#include "esp_log.h"

static const char *TAG = "HAL_GPIO";

// Converting HAL to ESP32 modes
static gpio_mode_t hal_to_esp_mode_converter(hal_gpio_mode_t mode) {
    if(mode == HAL_GPIO_MODE_INPUT)
        return GPIO_MODE_INPUT;
    
    if(mode == HAL_GPIO_MODE_OUTPUT)
        return GPIO_MODE_OUTPUT;

    if(mode == HAL_GPIO_MODE_OUTPUT_OD)
        return GPIO_MODE_OUTPUT_OD;
    
    if(mode == HAL_GPIO_MODE_INPUT_OUTPUT_OD)
        return GPIO_MODE_INPUT_OUTPUT_OD;

    return GPIO_MODE_DISABLE;
}

static gpio_pullup_t hal_to_esp_pullup_converter(hal_gpio_pullup_t pullup) {
    if(pullup == HAL_GPIO_PULLUP_DISABLE)
        return GPIO_PULLUP_DISABLE;

    if(pullup == HAL_GPIO_PULLUP_ENABLE)
        return GPIO_PULLUP_ENABLE;

    return GPIO_PULLUP_DISABLE;
}

static gpio_pulldown_t hal_to_esp_pulldown_converter(hal_gpio_pulldown_t pulldown) {
    if(pulldown == HAL_GPIO_PULLDOWN_DISABLE)
        return GPIO_PULLDOWN_DISABLE;

    if(pulldown == HAL_GPIO_PULLDOWN_ENABLE)
        return GPIO_PULLDOWN_ENABLE;

    return GPIO_PULLDOWN_DISABLE;
}

static gpio_int_type_t hal_to_esp_intr_converter(hal_interupt_t intr_type) {
    if(intr_type == HAL_GPIO_INTR_ANYEDGE)
        return GPIO_INTR_ANYEDGE;

    if(intr_type == HAL_GPIO_INTR_POSEDGE)
        return GPIO_INTR_POSEDGE;

    if(intr_type == HAL_GPIO_INTR_NEGEDGE)
        return GPIO_INTR_NEGEDGE;

    if(intr_type == HAL_GPIO_INTR_DISABLE)
        return GPIO_INTR_DISABLE;

    return GPIO_INTR_DISABLE;    
}


void hal_gpio_set_direction(uint8_t pin, hal_gpio_mode_t mode) {
    gpio_set_direction(pin, hal_to_esp_mode_converter(mode));
}

// set interrupt type
void hal_gpio_set_intr_type(uint8_t pin, hal_interupt_t intr_type) {
    gpio_set_intr_type(pin, hal_to_esp_intr_converter(intr_type));
}

// Disabling pullup and pulldown
void hal_gpio_pulldown_dis(uint8_t gpio_pin) {
    gpio_pulldown_dis(gpio_pin);
}

void hal_gpio_pullup_dis(uint8_t gpio_pin) {
    gpio_pullup_dis(gpio_pin);
}


// GPIO Initialization Method.
void hal_gpio_init(uint8_t pin, hal_gpio_mode_t mode, hal_gpio_pullup_t pullup, hal_gpio_pulldown_t pulldown) {
    // Initializing the GPIO pin
    gpio_config_t pin_config = {
        .pin_bit_mask = (1ULL << pin),
        .mode = hal_to_esp_mode_converter(mode),
        .pull_up_en = hal_to_esp_pullup_converter(pullup),
        .pull_down_en = hal_to_esp_pulldown_converter(pulldown),
        .intr_type = GPIO_INTR_DISABLE
    };

    esp_err_t check = gpio_config(&pin_config);
        if(check == ESP_OK)
            ESP_LOGI(TAG, "gpio pin %d configured succcessful", pin);
        else
            ESP_LOGE(TAG, "Failed to configure pin %d, %s", pin, esp_err_to_name(check));

    vTaskDelay(pdMS_TO_TICKS(100));  // Cool down.
}

// GPIO write Method
void hal_gpio_write(uint8_t pin, bool level) {
    gpio_set_level(pin, level);
}

// GPIO Read Method
bool hal_gpio_read(uint8_t pin) {
    return gpio_get_level(pin);
}