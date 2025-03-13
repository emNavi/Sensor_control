/*
 * baro_driver.c
 *
 *  Created on: Mar 10, 2025
 *      Author: xm
 */
#include "baro_driver.h"

// I2C 写寄存器
static void BMP280_WriteRegister(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t data) {
    uint8_t buf[2] = { reg, data };
    HAL_I2C_Master_Transmit(hi2c, BMP280_I2C_ADDRESS, buf, 2, HAL_MAX_DELAY);
}

// I2C 读寄存器
static void BMP280_ReadRegister(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data, uint8_t length) {
    HAL_I2C_Master_Transmit(hi2c, BMP280_I2C_ADDRESS, &reg, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(hi2c, BMP280_I2C_ADDRESS, data, length, HAL_MAX_DELAY);
}

// 初始化 BMP280
void BMP280_Init(I2C_HandleTypeDef *hi2c) {
    // 复位 BMP280
    BMP280_WriteRegister(hi2c, BMP280_REG_RESET, BMP280_RESET_VALUE);
    HAL_Delay(100);  // 等待复位完成

    // 配置控制测量寄存器 (温度 x1, 气压 x1, 正常模式)
    BMP280_WriteRegister(hi2c, BMP280_REG_CTRL, BMP280_OSRS_T | BMP280_OSRS_P | BMP280_MODE);
}

// 读取设备 ID
uint8_t BMP280_ReadID(I2C_HandleTypeDef *hi2c) {
    uint8_t id;
    BMP280_ReadRegister(hi2c, BMP280_REG_ID, &id, 1);
    return id;
}

// 读取校准数据
void BMP280_ReadCalibrationData(I2C_HandleTypeDef *hi2c, BMP280_CalibData *calibData) {
    uint8_t buf[24];
    BMP280_ReadRegister(hi2c, 0x88, buf, 24);

    calibData->dig_T1 = (buf[1] << 8) | buf[0];
    calibData->dig_T2 = (buf[3] << 8) | buf[2];
    calibData->dig_T3 = (buf[5] << 8) | buf[4];
    calibData->dig_P1 = (buf[7] << 8) | buf[6];
    calibData->dig_P2 = (buf[9] << 8) | buf[8];
    calibData->dig_P3 = (buf[11] << 8) | buf[10];
    calibData->dig_P4 = (buf[13] << 8) | buf[12];
    calibData->dig_P5 = (buf[15] << 8) | buf[14];
    calibData->dig_P6 = (buf[17] << 8) | buf[16];
    calibData->dig_P7 = (buf[19] << 8) | buf[18];
    calibData->dig_P8 = (buf[21] << 8) | buf[20];
    calibData->dig_P9 = (buf[23] << 8) | buf[22];
}

// 读取温度
float BMP280_ReadTemperature(I2C_HandleTypeDef *hi2c, BMP280_CalibData *calibData) {
    uint8_t buf[3];
    BMP280_ReadRegister(hi2c, BMP280_REG_TEMP_MSB, buf, 3);
    int32_t adc_T = (buf[0] << 16 | buf[1] << 8 | buf[2]) >> 4;

    int32_t var1 = ((((adc_T >> 3) - ((int32_t)calibData->dig_T1 << 1))) * ((int32_t)calibData->dig_T2)) >> 11;
    int32_t var2 = (((((adc_T >> 4) - ((int32_t)calibData->dig_T1)) * ((adc_T >> 4) - ((int32_t)calibData->dig_T1))) >> 12) * ((int32_t)calibData->dig_T3)) >> 14;
    calibData->t_fine = var1 + var2;

    return ((calibData->t_fine * 5 + 128) >> 8) / 100.0;
}

// 读取气压
float BMP280_ReadPressure(I2C_HandleTypeDef *hi2c, BMP280_CalibData *calibData) {
    uint8_t buf[3];
    BMP280_ReadRegister(hi2c, BMP280_REG_PRESS_MSB, buf, 3);
    int32_t adc_P = (buf[0] << 16 | buf[1] << 8 | buf[2]) >> 4;

    int64_t var1 = ((int64_t)calibData->t_fine) - 128000;
    int64_t var2 = var1 * var1 * (int64_t)calibData->dig_P6;
    var2 += ((var1 * (int64_t)calibData->dig_P5) << 17);
    var2 += ((int64_t)calibData->dig_P4 << 35);
    var1 = ((var1 * var1 * (int64_t)calibData->dig_P3) >> 8) + ((var1 * (int64_t)calibData->dig_P2) << 12);
    var1 = (((((int64_t)1 << 47) + var1)) * (int64_t)calibData->dig_P1) >> 33;

    if (var1 == 0) return 0;
    int64_t p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;

    var1 = (((int64_t)calibData->dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)calibData->dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)calibData->dig_P7) << 4);

    return (float)p / 256.0f;  // **单位：Pa**
}

// 气压（Pa） -----> 海拔（m）
float BMP280_CalculateAltitude(float pressure) {
    return 44330.0 * (1.0 - pow(pressure / SEA_LEVEL_PRESSURE, 1.0 / 5.255));
}
