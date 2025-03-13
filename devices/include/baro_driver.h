/*
 * baro_driver.h
 *
 *  Created on: Mar 10, 2025
 *      Author: xm
 */

#ifndef DEVICES_INCLUDE_BARO_DRIVER_H_
#define DEVICES_INCLUDE_BARO_DRIVER_H_

#include "main.h"
#include <math.h>
#define SEA_LEVEL_PRESSURE 102090.0  // 海平面标准气压（单位：Pa）

// BMP280 控制命令
#define BMP280_RESET_VALUE  0xB6

// BMP280 工作模式配置
#define BMP280_OSRS_T      (5 << 5)  // 温度超采样 x1
#define BMP280_OSRS_P      (5 << 2)  // 气压超采样 x1
#define BMP280_MODE        (3)       // 正常模式

// 传感器校准数据结构
typedef struct {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
    int32_t  t_fine;
} BMP280_CalibData;

// BMP280 相关函数声明
void BMP280_Init(I2C_HandleTypeDef *hi2c);
uint8_t BMP280_ReadID(I2C_HandleTypeDef *hi2c);
void BMP280_ReadCalibrationData(I2C_HandleTypeDef *hi2c, BMP280_CalibData *calibData);
float BMP280_ReadTemperature(I2C_HandleTypeDef *hi2c, BMP280_CalibData *calibData);
float BMP280_ReadPressure(I2C_HandleTypeDef *hi2c, BMP280_CalibData *calibData);
float BMP280_CalculateAltitude(float pressure);



#endif /* DEVICES_INCLUDE_BARO_DRIVER_H_ */
