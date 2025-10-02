#pragma once

#include <stdint.h>
#include "lcd_st7789_driver.h"

#define CELL_SIZE   18

void gamerender_draw_game_board(uint16_t *framebuffer);
void gamerender_draw_cursor(uint16_t *framebuffer, uint8_t cursor_x, uint8_t cursor_y);