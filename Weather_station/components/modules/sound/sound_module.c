#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sound_module.h"
#include "buzzer.h"

#define BEEP_DELAY_MS 200

extern void sound_task();

sound_module_t sound;
// Weather Module Initializer
void sound_module_init(uint8_t gpio_pin) {
    sound.gpio_num   = gpio_pin;
    sound.beep_delay = BEEP_DELAY_MS;

    buzzer_init(sound.gpio_num, sound.beep_delay);

    xTaskCreatePinnedToCore(sound_task, "sound Task", 2048, NULL, 5, NULL, 1);
}