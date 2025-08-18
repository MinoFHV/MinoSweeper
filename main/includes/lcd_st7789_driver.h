#pragma once

#include <stdint.h>
#include <esp_err.h>

#define LCD_SCLK_PIN    4
#define LCD_DC_PIN      5
#define LCD_MOSI_PIN    6
#define LCD_CS_PIN      7
#define LCD_RST_PIN     8
#define LCD_BL_PIN      9

#define LCD_WIDTH       240
#define LCD_HEIGHT      320

#define RGB565(r,g,b)   __builtin_bswap16((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))


esp_err_t lcd_st7789_init();
void lcd_st7789_draw_framebuffer(const uint16_t *framebuffer);
void lcd_st7789_draw_box(uint16_t *framebuffer, int x, int y, int width, int height, uint16_t color_border, uint16_t color_fill);
void lcd_st7789_draw_digit(uint16_t *framebuffer, int x, int y, int width, int height, uint16_t color_cursor);
void lcd_st7789_draw_flag(uint16_t *framebuffer, int x, int y, int width, int height, uint16_t color);
void lcd_st7789_draw_bomb(uint16_t *framebuffer, int x, int y, int width, int height, uint16_t color);
