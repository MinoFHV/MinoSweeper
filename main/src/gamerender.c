#include "gamerender.h"
#include "gamelogic.h"
#include "lcd_st7789_driver.h"

void gamerender_draw_field(uint16_t *framebuffer)
{

    uint16_t border_color = RGB565(255, 128, 0);
    uint16_t hidden_fill_color = RGB565(169, 169, 169);
    uint16_t revealed_fill_color = RGB565(200, 200, 200);
    uint8_t offset_y = 43;

    game_logic_state_t game_state = gamelogic_get_state();

    // Draw the orange background which will become the grid lines
    uint16_t grid_width_px = BOARD_DIMENSION_X_Y * CELL_SIZE;
    uint16_t grid_height_px = BOARD_DIMENSION_X_Y * CELL_SIZE;
    lcd_st7789_draw_box(framebuffer, 0, offset_y, grid_width_px + 1, grid_height_px + 1, border_color, border_color);

    uint8_t box_x = 0;
    uint16_t box_y = 0;
    uint8_t box_size = 0;

    cell_state_t state = 0;
    char content = 0;

    // Iterate through the board and draw each cell based on its state
    for (uint8_t y = 0; y < BOARD_DIMENSION_X_Y; y++)
    {
        for (uint8_t x = 0; x < BOARD_DIMENSION_X_Y; x++)
        {

            box_x = (x * CELL_SIZE) + 1;
            box_y = (y * CELL_SIZE) + offset_y + 1;
            box_size = CELL_SIZE - 1;

            state = gamelogic_get_player_cell_state(x, y);
            content = gamelogic_get_solution_cell(x, y);

            // If the game is lost, we want to show where the bombs were
            if (game_state == GAME_STATE_LOST && content == 'x') state = CELL_STATE_REVEALED;

            // Draw based on the cell's state
            if (state == CELL_STATE_HIDDEN) lcd_st7789_draw_box(framebuffer, box_x, box_y, box_size, box_size, hidden_fill_color, hidden_fill_color);
            else if (state == CELL_STATE_FLAGGED)
            {
                // Draw the hidden cell background first, then the flag on top
                lcd_st7789_draw_box(framebuffer, box_x, box_y, box_size, box_size, hidden_fill_color, hidden_fill_color);
                lcd_st7789_draw_flag(framebuffer, box_x, box_y, box_size, box_size, RGB565(255, 0, 0));
            }
            else if (state == CELL_STATE_REVEALED)
            {

                // Draw the lighter revealed background
                lcd_st7789_draw_box(framebuffer, box_x, box_y, box_size, box_size, revealed_fill_color, revealed_fill_color);

                // Draw the content (bomb or number) on top of the revealed background
                if (content == 'x') lcd_st7789_draw_bomb(framebuffer, box_x, box_y, box_size, box_size, RGB565(0, 0, 0));
                else if (content > '0' && content <= '8') lcd_st7789_draw_digit(framebuffer, box_x, box_y, box_size, box_size, content, RGB565(0, 0, 255));
                
            }

        }
    }

}

void gamerender_draw_cursor(uint16_t *framebuffer, uint8_t cursor_x, uint8_t cursor_y)
{

    uint16_t cursor_color = RGB565(0, 255, 0);
    uint8_t offset_y = 43;

    uint8_t box_x = (cursor_x * CELL_SIZE);
    uint16_t box_y = (cursor_y * CELL_SIZE) + offset_y;

    lcd_st7789_draw_cursor_box(framebuffer, box_x, box_y, CELL_SIZE + 1, CELL_SIZE + 1, cursor_color);

}