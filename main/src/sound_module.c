#include "sound_module.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


esp_err_t sound_module_init()
{

    esp_err_t ret = ESP_OK;

    ledc_timer_config_t ledc_timer =
    {
        .speed_mode = BUZZER_MODE,
        .timer_num = BUZZER_TIMER,
        .duty_resolution = BUZZER_DUTY,
        .freq_hz = 1500,
        .clk_cfg = BUZZER_CLK_CFG,
    };

    ret = ledc_timer_config(&ledc_timer);
    if (ret != ESP_OK) return ret;

    ledc_channel_config_t ledc_channel =
    {
        .speed_mode = BUZZER_MODE,
        .channel = BUZZER_CHANNEL,
        .timer_sel = BUZZER_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = BUZZER_PIN,
        .duty = 0,
        .hpoint = 0,
    };

    ret = ledc_channel_config(&ledc_channel);
    if (ret != ESP_OK) return ret;

    return ESP_OK;

}

void sound_module_beep(uint16_t frequency, uint16_t duration_ms)
{

    if (frequency <= 0) return;

    // Play Sound
    ledc_set_freq(BUZZER_MODE, BUZZER_TIMER, frequency);
    ledc_set_duty(BUZZER_MODE, BUZZER_CHANNEL, BUZZER_DUTY_CYCLE_50);
    ledc_update_duty(BUZZER_MODE, BUZZER_CHANNEL);

    // Wait
    vTaskDelay(pdMS_TO_TICKS(duration_ms));

    // Stop sound
    ledc_set_duty(BUZZER_MODE, BUZZER_CHANNEL, 0);
    ledc_update_duty(BUZZER_MODE, BUZZER_CHANNEL);

}

void sound_module_play_win_melody()
{

    sound_module_beep(2093, 117);
    vTaskDelay(pdMS_TO_TICKS(50));
    sound_module_beep(2093, 117);
    vTaskDelay(pdMS_TO_TICKS(50));
    sound_module_beep(2093, 117);
    vTaskDelay(pdMS_TO_TICKS(50));
    sound_module_beep(2093, 450);
    vTaskDelay(pdMS_TO_TICKS(50));
    sound_module_beep(1661, 450);
    vTaskDelay(pdMS_TO_TICKS(50));
    sound_module_beep(1864, 450);
    vTaskDelay(pdMS_TO_TICKS(50));
    sound_module_beep(2093, 250);
    vTaskDelay(pdMS_TO_TICKS(50));
    sound_module_beep(1864, 117);
    vTaskDelay(pdMS_TO_TICKS(50));
    sound_module_beep(2093, 750);

}

void sound_module_play_lose_melody()
{

    sound_module_beep(1975, 150);
    vTaskDelay(pdMS_TO_TICKS(50));
    sound_module_beep(2794, 150);
    vTaskDelay(pdMS_TO_TICKS(150));
    sound_module_beep(2794, 117);
    vTaskDelay(pdMS_TO_TICKS(50));
    sound_module_beep(2794, 200);
    vTaskDelay(pdMS_TO_TICKS(50));
    sound_module_beep(2637, 150);
    vTaskDelay(pdMS_TO_TICKS(50));
    sound_module_beep(2349, 150);
    vTaskDelay(pdMS_TO_TICKS(50));
    sound_module_beep(2093, 150);
    vTaskDelay(pdMS_TO_TICKS(50));
    sound_module_beep(1568, 100);
    vTaskDelay(pdMS_TO_TICKS(150));
    sound_module_beep(1568, 100);
    vTaskDelay(pdMS_TO_TICKS(50));
    sound_module_beep(1047, 450);

}