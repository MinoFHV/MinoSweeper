#pragma once

#include <stdint.h>
#include "lcd_st7789_driver.h"

#define CELL_SIZE   18

void gamerender_draw_field(uint16_t *framebuffer);