#ifndef LM75DB_H
#define LM75DB_H

#include "temp_driver.h"

#define LM75DB_ADDR (0x49 << 1)
//#define LM75DB_ADDR_1 (0x48 << 1)
#define SCL_IO 22
#define SDA_IO 21
#define I2C_FREQ 100000
#define TEMP_REG 0x00

extern temp_sensor_t lm75db_sensor;


#endif // LM75DB_H