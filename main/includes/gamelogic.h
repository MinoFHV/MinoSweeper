#pragma once

#include <inttypes.h>


#define BOARD_DIMENSION_X_Y        13

#define BOMB_PERCENTAGE     0.15
#define BOMB_AMOUNT         ((uint8_t)(BOARD_DIMENSION_X_Y * BOARD_DIMENSION_X_Y) * BOMB_PERCENTAGE)


typedef enum
{
    GAME_STATE_PLAYING = 0,
    GAME_STATE_WON = 1,
    GAME_STATE_LOST = -1,
} game_logic_state_t;

void game_logic_init();