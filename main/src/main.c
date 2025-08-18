#include "gamelogic.h"
#include "gamerender.h"
#include "lcd_st7789_driver.h"
#include "sound_module.h"

#include <esp_err.h>
#include <stdio.h>

void app_main(void)
{

    // Framebuffer for LCD Screen
    static uint16_t framebuffer[LCD_WIDTH * LCD_HEIGHT] = {0};

    esp_err_t init_ret = ESP_OK;

    init_ret = sound_module_init();
    if (init_ret != ESP_OK) return;

    init_ret = lcd_st7789_init();
    if (init_ret != ESP_OK) return;

    game_logic_init();
    gamerender_draw_field(framebuffer);

    
    lcd_st7789_draw_framebuffer(framebuffer);

}