#include "gamerender.h"
#include "gamelogic.h"
#include "lcd_st7789_driver.h"

void gamerender_draw_field(uint16_t *framebuffer)
{

    uint16_t border_color = RGB565(255, 128, 0);
    uint16_t fill_color = RGB565(169, 169, 169);
    uint8_t grid_size = BOARD_DIMENSION_X_Y;
    uint8_t cell_size = CELL_SIZE;
    uint8_t offset_y = 43;

    uint8_t grid_width_px = grid_size * cell_size;
    uint16_t grid_height_px = grid_size * cell_size;

    // Draw the orange background, making it 1 pixel larger for the final border.
    lcd_st7789_draw_box(framebuffer, 0, offset_y, grid_width_px + 1, grid_height_px + 1, border_color, border_color);

    // Draw the gray cells on top, leaving a 1-pixel border.
    for (uint16_t y = 0; y < grid_size; y++)
    {
        for (uint8_t x = 0; x < grid_size; x++)
        {

            // Calculate position for the inner gray box, offset by 1 pixel for the border
            uint8_t box_x = (x * cell_size) + 1;
            uint16_t box_y = (y * cell_size) + offset_y + 1;
            
            // The inner box is smaller to leave the border visible
            uint8_t box_size = cell_size - 1;

            // Draw the gray box. We use the same color for its border and fill
            lcd_st7789_draw_box(framebuffer, box_x, box_y, box_size, box_size, fill_color, fill_color);
            
        }
    }

}