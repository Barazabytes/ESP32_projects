#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal_gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "buzzer.h"

static const char *TAG = "BUZZZER";
static buzzer_t buzzer;

// Buzzer Initializing Method
void buzzer_init(uint8_t pin, uint32_t beep_delay) {
    
   // Initialization of struct creddentials
    buzzer.buzzer_pin   = pin;
    buzzer.beep_delay   = beep_delay;
    buzzer.buzzer_level = 0;

   // sending Initialization data to HAL
    hal_gpio_init(buzzer.buzzer_pin, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_PULLUP_DISABLE, HAL_GPIO_PULLDOWN_DISABLE);
}

// Buzzer set level
void buzzer_toggle() {
    buzzer.buzzer_level = !buzzer.buzzer_level;
    ESP_LOGI(TAG, "Buzzer Level %d", buzzer.buzzer_level);
    hal_gpio_write(buzzer.buzzer_pin, buzzer.buzzer_level);
}

// Buzzer beep sound
void buzzer_beep() {
    for(int i = 0; i < 5; i++) {
        buzzer_toggle();
        vTaskDelay(pdMS_TO_TICKS(buzzer.beep_delay));
    }
    hal_gpio_write(buzzer.buzzer_pin, 0);
}
