#include "gamelogic.h"
#include "gamerender.h"
#include "lcd_st7789_driver.h"
#include "nes_controller.h"
#include "sound_module.h"

#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>


#define REPEAT_DELAY_FRAMES     1


void app_main(void)
{

    esp_err_t init_ret = ESP_OK;

    // Component Initialization
    init_ret = sound_module_init();
    if (init_ret != ESP_OK) return;

    init_ret = lcd_st7789_init();
    if (init_ret != ESP_OK) return;

    init_ret = nes_controller_init();
    if (init_ret != ESP_OK) return;

    gamelogic_init();

    static uint16_t framebuffer[LCD_WIDTH * LCD_HEIGHT] = {0};

    uint8_t cursor_x = 0;
    uint8_t cursor_y = 0;
    uint8_t button_state = 0;
    uint8_t repeat_timer = 0;
    bool moved = false;

    game_logic_state_t previous_game_state = GAME_STATE_PLAYING;
    uint8_t last_button_state = 0;

    // Game Loop
    while (1)
    {

        button_state = nes_controller_read();
        uint8_t pressed_buttons = button_state & ~last_button_state;
        game_logic_state_t current_game_state = gamelogic_get_state();

        // Loop-Prevention: If game state has changed, play appropriate sound ONCE
        if (current_game_state != previous_game_state)
        {
            if (current_game_state == GAME_STATE_WON) { sound_module_play_win_melody(); }
            else if (current_game_state == GAME_STATE_LOST) { sound_module_play_lose_melody(); }
        }

        // Otherwise, if still playing, update previous state
        previous_game_state = current_game_state;

        if (current_game_state == GAME_STATE_PLAYING)
        {

            // Reset repeat timer if no button is pressed
            if (button_state == 0) repeat_timer = 0;

            if ((button_state != 0) && (repeat_timer == 0))
            {

                moved = false;

                // Update cursor position based on button presses
                if (button_state & (1 << BUTTON_UP_BITPOS)) { if (cursor_y > 0) { cursor_y--; moved = true; } }
                else if (button_state & (1 << BUTTON_DOWN_BITPOS)) { if (cursor_y < (BOARD_DIMENSION_X_Y - 1)) { cursor_y++; moved = true; } }
                else if (button_state & (1 << BUTTON_LEFT_BITPOS)) { if (cursor_x > 0) { cursor_x--; moved = true; } }
                else if (button_state & (1 << BUTTON_RIGHT_BITPOS)) { if (cursor_x < (BOARD_DIMENSION_X_Y - 1)) { cursor_x++; moved = true; } }
                
                if (moved)
                {
                    sound_module_beep(1760, 50);
                    repeat_timer = REPEAT_DELAY_FRAMES;
                }

            }
            else if (repeat_timer > 0) { repeat_timer--; }

            // Handle button presses for Reveal / Flag Toggle
            if (pressed_buttons & (1 << BUTTON_A_BITPOS)) gamelogic_reveal_cell(cursor_x, cursor_y);
            else if (pressed_buttons & (1 << BUTTON_B_BITPOS)) gamelogic_toggle_flag(cursor_x, cursor_y);

        }
        else
        {

            // Restart Game
            if (pressed_buttons & (1 << BUTTON_START_BITPOS))
            {

                gamelogic_init();
                cursor_x = 0;
                cursor_y = 0;

            }

        }

        // Update last button state
        last_button_state = button_state;

        // Render the game board and cursor
        gamerender_draw_game_board(framebuffer);
        if (gamelogic_get_state() == GAME_STATE_PLAYING) gamerender_draw_cursor(framebuffer, cursor_x, cursor_y);
        lcd_st7789_draw_framebuffer(framebuffer);

        vTaskDelay(pdMS_TO_TICKS(33));

    }

}
