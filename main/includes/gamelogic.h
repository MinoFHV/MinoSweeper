#pragma once

#include <inttypes.h>


#define BOARD_WIDTH         13
#define BOARD_HEIGHT        13

#define BOMB_PERCENTAGE     0.15
#define BOMB_AMOUNT         ((uint8_t)(BOARD_WIDTH * BOARD_HEIGHT) * BOMB_PERCENTAGE)


typedef enum
{
    GAME_STATE_PLAYING = 0,
    GAME_STATE_WON = 1,
    GAME_STATE_LOST = -1,
} game_logic_state_t;

void game_logic_init();