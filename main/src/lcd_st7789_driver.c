#include "lcd_st7789_driver.h"

#include <driver/spi_master.h>
#include <driver/gpio.h>

#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_vendor.h>

#include <esp_check.h>
#include <esp_log.h>


static const char *TAG = "LCD_ST7789_DRIVER";
static esp_lcd_panel_handle_t esp_lcd_panel_handle = NULL;


esp_err_t lcd_st7789_init()
{

    esp_err_t ret = ESP_OK;

    ESP_LOGI(TAG, "Initialitizing ST7789 LCD (SPI)...");

    ESP_LOGI(TAG, "Configuring Backlight...");
    gpio_config_t bl_gpio_config =
    {
        .pin_bit_mask = (1 << LCD_BL_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    ret = gpio_config(&bl_gpio_config);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to configure backlight GPIO: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "Configuring SPI Bus...");
    spi_bus_config_t spi_bus_config =
    {
        .sclk_io_num = LCD_SCLK_PIN,
        .mosi_io_num = LCD_MOSI_PIN,
        .miso_io_num = -1,
        .quadhd_io_num = -1,
        .quadwp_io_num = -1,
        .max_transfer_sz = LCD_WIDTH * 80 * sizeof(uint16_t),
    };

    ret = spi_bus_initialize(SPI2_HOST, &spi_bus_config, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize SPI bus: %s", esp_err_to_name(ret));
        return ret; 
    }

    ESP_LOGI(TAG, "Configuring LCD Panel IO...");
    esp_lcd_panel_io_handle_t esp_lcd_panel_io = NULL;
    esp_lcd_panel_io_spi_config_t esp_lcd_panel_spi_config = {
        .cs_gpio_num = LCD_CS_PIN,
        .dc_gpio_num = LCD_DC_PIN,
        .pclk_hz = 40 * 1000 * 1000,
        .trans_queue_depth = 10,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 0,
        .user_ctx = NULL,
    };

    ret = esp_lcd_new_panel_io_spi(SPI2_HOST, &esp_lcd_panel_spi_config, &esp_lcd_panel_io);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to create LCD panel IO: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "Configuring LCD Panel Device...");
    esp_lcd_panel_dev_config_t esp_lcd_panel_dev_config =
    {
        .reset_gpio_num = LCD_RST_PIN,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,
    };

    ret = esp_lcd_new_panel_st7789(esp_lcd_panel_io, &esp_lcd_panel_dev_config, &esp_lcd_panel_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to create LCD panel device: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "Reset, Init, Invert Color, and Turning Display On...");
    ret = esp_lcd_panel_reset(esp_lcd_panel_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to reset LCD panel: %s", esp_err_to_name(ret));
        return ret;
    }
    ret = esp_lcd_panel_init(esp_lcd_panel_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize LCD panel: %s", esp_err_to_name(ret));
        return ret;
    }
    ret = esp_lcd_panel_invert_color(esp_lcd_panel_handle, true);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to invert color on LCD panel: %s", esp_err_to_name(ret));
        return ret;
    }
    ret = esp_lcd_panel_disp_on_off(esp_lcd_panel_handle, true);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to turn on display: %s", esp_err_to_name(ret));
        return ret;
    }

    gpio_set_level(LCD_BL_PIN, 1);

    ESP_LOGI(TAG, "LCD panel initialized successfully.");
    return ESP_OK;

}

void lcd_st7789_draw_framebuffer(const uint16_t *framebuffer)
{

    esp_err_t ret = esp_lcd_panel_draw_bitmap(esp_lcd_panel_handle, 0, 0, LCD_WIDTH, LCD_HEIGHT, framebuffer);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to draw framebuffer: %s", esp_err_to_name(ret));
    }

}

void lcd_st7789_draw_box(uint16_t *framebuffer, int x, int y, int width, int height, uint16_t color_border, uint16_t color_fill)
{

    for (int pixel_y = y; pixel_y < y + height; pixel_y++)
    {
        for (int pixel_x = x; pixel_x < x + width; pixel_x++)
        {
            if ((pixel_x == x) || (pixel_x == x + width - 1) || (pixel_y == y) || (pixel_y == y + height - 1))
            {
                framebuffer[(pixel_y * LCD_WIDTH) + pixel_x] = color_border;
            }
            else
            {
                framebuffer[(pixel_y * LCD_WIDTH) + pixel_x] = color_fill;
            }
        }
    }

}