#include "gamelogic.h"

#include <stdio.h>
#include <esp_random.h>


static char solution_board[BOARD_DIMENSION_X_Y][BOARD_DIMENSION_X_Y] = {0};
static cell_state_t player_board[BOARD_DIMENSION_X_Y][BOARD_DIMENSION_X_Y] = {0};
static game_logic_state_t current_state = GAME_STATE_PLAYING;

void gamelogic_init()
{

    current_state = GAME_STATE_PLAYING;

    // Clear boards
    for (uint8_t y = 0; y < BOARD_DIMENSION_X_Y; ++y)
    {
        for (uint8_t x = 0; x < BOARD_DIMENSION_X_Y; ++x)
        {
            player_board[y][x] = CELL_STATE_HIDDEN;
            solution_board[y][x] = 0;
        }
    }

    // Generate Solution Board
    uint8_t bomb_count = 0;
    uint8_t bomb_x = 0;
    uint8_t bomb_y = 0;

    // Randomly place bombs
    while (bomb_count < BOMB_AMOUNT)
    {

        bomb_x = esp_random() % BOARD_DIMENSION_X_Y;
        bomb_y = esp_random() % BOARD_DIMENSION_X_Y;

        if (solution_board[bomb_y][bomb_x] != 'x')
        {
            solution_board[bomb_y][bomb_x] = 'x';
            bomb_count++;
        }

    }

    // Calculate adjacent bomb
    uint8_t adjacent_bombs = 0;
    int16_t check_x = 0;
    int16_t check_y = 0;

    for (uint8_t y = 0; y < BOARD_DIMENSION_X_Y; ++y)
    {
        for (uint8_t x = 0; x < BOARD_DIMENSION_X_Y; ++x)
        {

            if (solution_board[y][x] == 0)
            {

                adjacent_bombs = 0;
                for (int8_t nearby_y = -1; nearby_y <= 1; ++nearby_y)
                {
                    for (int8_t nearby_x = -1; nearby_x <= 1; ++nearby_x)
                    {

                        if ((nearby_y == 0) && (nearby_x == 0)) continue; 

                        check_x = x + nearby_x;
                        check_y = y + nearby_y;

                        if ((check_y >= 0) && (check_y < BOARD_DIMENSION_X_Y) && (check_x >= 0) && (check_x < BOARD_DIMENSION_X_Y))
                        {
                            if (solution_board[check_y][check_x] == 'x') adjacent_bombs++;
                        }

                    }
                }

                solution_board[y][x] = adjacent_bombs + '0';

            }
        }

    }

}

char** gamelogic_get_solution_board()
{
    return (char**)solution_board;
}

cell_state_t** gamelogic_get_player_board()
{
    return (cell_state_t**)player_board;
}

char gamelogic_get_solution_cell(uint8_t x, uint8_t y)
{
    if ((x >= BOARD_DIMENSION_X_Y) || (y >= BOARD_DIMENSION_X_Y)) return 0;
    return solution_board[y][x];
}

cell_state_t gamelogic_get_player_cell_state(uint8_t x, uint8_t y)
{
    if ((x >= BOARD_DIMENSION_X_Y) || (y >= BOARD_DIMENSION_X_Y)) return 0;
    return player_board[y][x];
}

static void gamelogic_check_for_win()
{

    for (uint8_t y = 0; y < BOARD_DIMENSION_X_Y; y++)
    {
        for (uint8_t x = 0; x < BOARD_DIMENSION_X_Y; x++)
        {
            if ((solution_board[y][x] != 'x') && (player_board[y][x] != CELL_STATE_REVEALED)) return;
        }
    }

    current_state = GAME_STATE_WON;

}

void gamelogic_toggle_flag(uint8_t x, uint8_t y)
{

    if (current_state != GAME_STATE_PLAYING) return;

    if (player_board[y][x] == CELL_STATE_HIDDEN) player_board[y][x] = CELL_STATE_FLAGGED;
    else if (player_board[y][x] == CELL_STATE_FLAGGED) player_board[y][x] = CELL_STATE_HIDDEN;

    gamelogic_check_for_win();

}

game_logic_state_t gamelogic_reveal_cell(uint8_t x, uint8_t y)
{

    // Can only reveal hidden cells while the game is active
    if ((player_board[y][x] != CELL_STATE_HIDDEN) || (current_state != GAME_STATE_PLAYING)) return current_state;

    player_board[y][x] = CELL_STATE_REVEALED;

    if (solution_board[y][x] == 'x')
    {
        current_state = GAME_STATE_LOST;
        return current_state;
    }

    if (solution_board[y][x] == '0')
    {

        int16_t check_x = 0;
        int16_t check_y = 0;

        // Recursively call reveal_cell for all 8 neighbors
        for (int8_t nearby_y = -1; nearby_y <= 1; nearby_y++)
        {
            for (int8_t nearby_x = -1; nearby_x <= 1; nearby_x++)
            {

                if ((nearby_x == 0) && (nearby_y == 0)) continue;
                check_x = x + nearby_x;
                check_y = y + nearby_y;

                // Ensure the neighbor is within board boundaries before revealing
                if ((check_x >= 0) && (check_x < BOARD_DIMENSION_X_Y) && (check_y >= 0) && (check_y < BOARD_DIMENSION_X_Y)) gamelogic_reveal_cell(check_x, check_y);

            }
        }
    }

    gamelogic_check_for_win();
    
    return current_state;

}

game_logic_state_t gamelogic_get_state()
{
    return current_state;
}