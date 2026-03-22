#include "hal_gpio.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"

const char *TAG = "HAL_GPIO";



// Convert HAL mode to ESPIDF mode
static gpio_mode_t mode_converter(hal_gpio_mode_t mode) {
    if(mode == HAL_GPIO_MODE_INPUT)
        return GPIO_MODE_INPUT;

    if(mode == HAL_GPIO_MODE_OUTPUT)
        return GPIO_MODE_OUTPUT;

    return GPIO_MODE_DISABLE; // Default case, should not happen
}


// GPIO pin configuration
void hal_gpio_init(uint8_t pin, hal_gpio_mode_t mode) {
       gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pin),
        .mode = mode_converter(mode),
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    
    esp_err_t check = gpio_config(&io_conf);
        if (check != ESP_OK)
            ESP_LOGE(TAG, "Failed to configure GPIO pin %d: %s", pin, esp_err_to_name(check));
        else 
            ESP_LOGI(TAG, "GPIO pin %d configured successfully", pin);

        vTaskDelay(pdMS_TO_TICKS(100)); // Short delay to ensure configuration is applied

}



// Write to a GPIO
void hal_gpio_write(uint8_t pin, bool level) {
    if(level)
        ESP_LOGI(TAG, "Setting GPIO pin %d HIGH", pin);
    else
        ESP_LOGI(TAG, "Setting GPIO pin %d LOW", pin);

    esp_err_t check = gpio_set_level(pin, level);
    if (check != ESP_OK)
        ESP_LOGE(TAG, "Failed to set GPIO pin %d level: %s", pin, esp_err_to_name(check));
    else
        ESP_LOGI(TAG, "GPIO pin %d level set successfully", pin);
}


// Read from a GPIO
bool hal_gpio_read(uint8_t pin) {

    esp_err_t check = gpio_get_level(pin);
        if (check == ESP_OK)
            ESP_LOGI(TAG, "GPIO pin %d level %d", pin, check);
        else
            ESP_LOGI(TAG, "GPIO pin %d level %d", pin, check);

    return check;
}


