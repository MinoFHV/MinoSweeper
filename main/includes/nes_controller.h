#pragma once

#include <esp_err.h>
#include <stdint.h>


// Define GPIO Pins
#define NC_DATA_PIN                 1
#define NC_LATCH_PIN                2
#define NC_CLOCK_PIN                3

// Constants
#define LATCH_TIME_US               12
#define POST_LATCH_WAIT_TIME_US     6
#define CLOCK_DUTY_TIME_US          6

// Button Positions in Bits
#define BUTTON_A_BITPOS             0
#define BUTTON_B_BITPOS             1
#define BUTTON_SELECT_BITPOS        2
#define BUTTON_START_BITPOS         3

#define BUTTON_UP_BITPOS            4
#define BUTTON_DOWN_BITPOS          5
#define BUTTON_LEFT_BITPOS          6
#define BUTTON_RIGHT_BITPOS         7


esp_err_t nes_controller_init();
uint8_t nes_controller_read();