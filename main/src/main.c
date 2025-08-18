#include "sound_module.h"

#include <esp_err.h>
#include <stdio.h>

void app_main(void)
{

    esp_err_t init_ret = ESP_OK;

    init_ret = sound_module_init();
    if (init_ret != ESP_OK)
    {
        printf("Failed to initialize sound module: %s\n", esp_err_to_name(init_ret));
        return;
    }
    

}