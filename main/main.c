#include "lm75db.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void) {
   
    if(!lm75db_sensor.init()) {
        ESP_LOGE("MAIN", "Failed to initiallize LM75DB.");
        return;
    } 

    //uint8_t toggle = 0;

    while (1) {
       // uint16_t address = toggle ? LM75DB_ADDR_1 : LM75DB_ADDR;

        int16_t raw;
        if (lm75db_sensor.read_raw(&raw) == 0) { //, address
            float temp_c = lm75db_sensor.raw_to_celsius(raw);
            ESP_LOGI("MAIN", "Temperature for address: %.2f ", temp_c); //0x%X is: %.2f C", address
        } 
        // else {
        //     ESP_LOGE("MAIN", "Failed to read temperature from address: 0x%X", address);
        // }

        //toggle = !toggle;  // Alternate for next loop
        vTaskDelay(pdMS_TO_TICKS(5000));
    }

}