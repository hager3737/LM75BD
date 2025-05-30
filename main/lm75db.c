#include "temp_driver.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "lm75db.h"

bool lm75db_init(void) {
    i2c_config_t config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ
    };

    i2c_param_config(I2C_NUM_0, &config);
    esp_err_t res = i2c_driver_install(I2C_NUM_0, config.mode, 1024, 1024, 0);

    if (res != ESP_OK) {
        ESP_LOGE("I2C", "Driver install failed: %s", esp_err_to_name(res));
    } else if(res == ESP_OK) {
        ESP_LOGI("I2C", "Successfully iinitiallized LM75DB.");
    }
    
    return res == ESP_OK;

}

int lm75db_read_raw(int16_t *raw) { //, int16_t address
    esp_err_t ret;
    uint8_t data[2];

    // Writing the temperature address
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, LM75DB_ADDR | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, TEMP_REG, true);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        ESP_LOGE("LM75DB", "Failed to write temperature register address: %s", esp_err_to_name(ret));
        return ret == ESP_FAIL;
    }

    // Reading 2 bytes from the temperature register.
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, LM75DB_ADDR | I2C_MASTER_READ, true);
    i2c_master_read(cmd, &data[0], 1, I2C_MASTER_ACK);
    i2c_master_read(cmd, &data[1], 1, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        ESP_LOGE("LM75DB", "Failed to read temperature data: %s", esp_err_to_name(ret));
        return ret == ESP_FAIL;
    }

    // Converting raw bytes to 11-bit temperature value with MSB first.
    *raw = (int16_t)((data[0] << 8) | data[1]) >> 5; //Combining MSB and LSB and shift right to match the format.

    return 0;
}

float lm75db_raw_to_celsius(int16_t raw) {
    return raw * 0.125f;
}

temp_sensor_t lm75db_sensor = {
    .init = lm75db_init,
    .read_raw = lm75db_read_raw,
    .raw_to_celsius = lm75db_raw_to_celsius
};