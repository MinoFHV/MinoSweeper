#pragma once

#include <esp_err.h>
#include <driver/ledc.h>

#define BUZZER_PIN      10

#define BUZZER_TIMER            LEDC_TIMER_0
#define BUZZER_MODE             LEDC_LOW_SPEED_MODE
#define BUZZER_CHANNEL          LEDC_CHANNEL_0
#define BUZZER_CLK_CFG          LEDC_AUTO_CLK
#define BUZZER_DUTY             LEDC_TIMER_10_BIT
#define BUZZER_DUTY_CYCLE_50    (BUZZER_DUTY >> 1)


esp_err_t sound_module_init();
void sound_module_beep(uint16_t frequency, uint16_t duration_ms);
void sound_module_play_win_melody();
void sound_module_play_lose_melody();