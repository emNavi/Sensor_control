/*
 * mag_driver.h
 *
 *  Created on: Mar 10, 2025
 *      Author: xm
 */

#ifndef DEVICES_INCLUDE_MAG_DRIVER_H_
#define DEVICES_INCLUDE_MAG_DRIVER_H_

#include "main.h"

typedef struct ist8310_raw_data_t {
    float x;
    float y;
    float z;
} ist8310_raw_data_t;

typedef enum ist8310_error_e {
    IST8310_NO_ERROR = 0x00,
    MEG_ID_ERROR = 0x01,
} ist8310_error_e;

typedef struct ist8310_data_t {
    uint8_t chip_id;
    ist8310_raw_data_t meg_data;
    ist8310_error_e meg_error;
} ist8310_data_t;

void IST8310_INIT(I2C_HandleTypeDef *hi2c, ist8310_data_t* ist8310_data);
uint8_t ReadSingleDataFromIST8310(I2C_HandleTypeDef *hi2c, uint8_t addr);
void ReadMultiDataFromIST8310(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t* data, uint8_t len);
void WriteSingleDataFromIST8310(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t data);
void WriteMultiDataFromIST8310(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t* data, uint8_t len);

ist8310_error_e VerifyMegId(I2C_HandleTypeDef *hi2c, uint8_t* id);

void ReadIST8310Data(I2C_HandleTypeDef *hi2c, ist8310_raw_data_t* meg_data);
int CheckIST8310();



#endif /* DEVICES_INCLUDE_MAG_DRIVER_H_ */
