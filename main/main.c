#include "lm75db.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void) {
   
    if(!lm75db_sensor.init()) {
        ESP_LOGE("MAIN", "Failed to initiallize LM75DB.");
        return;
    } 

    while(1) {
        int16_t raw;
        if(lm75db_sensor.read_raw(&raw) == 0) {
            float temp_c = lm75db_sensor.raw_to_celsius(raw);
            ESP_LOGI("MAIN", "Temperature: %.2f C", temp_c);
        } else {
            ESP_LOGE("MAIN", "Failed to read temperature.");
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }

}