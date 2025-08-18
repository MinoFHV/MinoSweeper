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

typedef enum
{
    CELL_STATE_HIDDEN = 0,
    CELL_STATE_REVEALED = 1,
    CELL_STATE_FLAGGED = 2,
} cell_state_t;


void gamelogic_init();
char gamelogic_get_solution_cell(uint8_t x, uint8_t y);
cell_state_t gamelogic_get_player_cell_state(uint8_t x, uint8_t y);
void gamelogic_toggle_flag(uint8_t x, uint8_t y);
game_logic_state_t gamelogic_reveal_cell(uint8_t x, uint8_t y);
game_logic_state_t gamelogic_get_state();
