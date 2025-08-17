#include <driver/gpio.h>
#include <esp_log.h>
#include <esp_rom_sys.h>

#include "nes_controller.h"


void nesControllerInit()
{

    // GPIO Config for Latch
    gpio_config_t gpioLatchConfig =
    {
        .pin_bit_mask = (1 << NC_LATCH_PIN),
        .mode = GPIO_MODE_OUTPUT, // from MCU to Controller
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    // GPIO Config for Clock
    gpio_config_t gpioClockConfig =
    {
        .pin_bit_mask = (1 << NC_CLOCK_PIN),
        .mode = GPIO_MODE_OUTPUT, // from MCU to Controller
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    // GPIO Config for Data
    gpio_config_t gpioDataConfig =
    {
        .pin_bit_mask = (1 << NC_DATA_PIN),
        .mode = GPIO_MODE_INPUT, // from Control (Shift Register) to MCU
        .pull_up_en = GPIO_PULLUP_ENABLE, // Negative True, thus Pullup
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    // Active the GPIO Configurations
    gpio_config(&gpioLatchConfig);
    gpio_config(&gpioClockConfig);
    gpio_config(&gpioDataConfig);

}

uint8_t nesControllerRead()
{

    // Set Latch to HIGH for 12µs, wait for 6µs
    // Save Button States in Internal Shift Register of NES Controller
    gpio_set_level(NC_LATCH_PIN, 1);
    esp_rom_delay_us(LATCH_TIME_US);
    gpio_set_level(NC_LATCH_PIN, 0);
    esp_rom_delay_us(POST_LATCH_WAIT_TIME_US);

    uint8_t allButtonStates = 0;

    // Read all 8 bits out for each clock cycle
    for (uint8_t cycle = 0; cycle < 8; ++cycle)
    {
        
        // Set Clock signal to HIGH
        gpio_set_level(NC_CLOCK_PIN, 1);

        // Read bit from serial data connection
        allButtonStates |= (gpio_get_level(NC_DATA_PIN) << cycle);

        // Wait for 6µs for 50% Duty Cycle, then set Clock to 0, then wait another 6µs for 50% Duty Cycle
        esp_rom_delay_us(CLOCK_DUTY_TIME_US);
        gpio_set_level(NC_CLOCK_PIN, 0);
        esp_rom_delay_us(CLOCK_DUTY_TIME_US);

    }

    // Negate button states to reverse inverse true (results in 0 = not pressed, 1 = pressed)
    return ~allButtonStates;

}