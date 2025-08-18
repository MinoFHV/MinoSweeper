#include "gamelogic.h"

#include <stdio.h>
#include <esp_random.h>


static char game_board[BOARD_HEIGHT][BOARD_WIDTH] = {0};

void game_logic_init()
{

    uint8_t bomb_count = 0;
    uint8_t bomb_x = 0;
    uint8_t bomb_y = 0;

    // Randomly place bombs
    while (bomb_count < BOMB_AMOUNT)
    {

        bomb_x = esp_random() % BOARD_WIDTH;
        bomb_y = esp_random() % BOARD_HEIGHT;

        if (game_board[bomb_y][bomb_x] != 'x')
        {

            game_board[bomb_y][bomb_x] = 'x';
            bomb_count++;

        }

    }

    uint8_t adjacent_bombs = 0;
    uint8_t check_x = 0;
    uint8_t check_y = 0;

    // Go through the entire board
    for (uint8_t y = 0; y < BOARD_HEIGHT; ++y)
    {
        for (uint8_t x = 0; x < BOARD_WIDTH; ++x)
        {

            // Only calculate if cell is 0
            if (game_board[y][x] == 0)
            {

                adjacent_bombs = 0;

                // Iterate through neighbouring cells
                for (int8_t nearby_y = -1; nearby_y <= 1; ++nearby_y)
                {
                    for (int8_t nearby_x = -1; nearby_x <= 1; ++nearby_x)
                    {
                        // Skip the current cell, otherwise check for bombs
                        if ((nearby_y == 0) && (nearby_x == 0)) continue; 

                        check_x = x + nearby_x;
                        check_y = y + nearby_y;

                        // Check if the neighbouring cell is within bounds
                        if ((check_y >= 0) && (check_y < BOARD_HEIGHT) && (check_x >= 0) && (check_x < BOARD_WIDTH))
                        {
                            // Check if the neighbouring cell is a bomb
                            if (game_board[check_y][check_x] == 'x')
                            {
                                adjacent_bombs++;
                            }
                        }
                        
                    }
                }

                game_board[y][x] = adjacent_bombs + '0';

            }
        }
    }

}