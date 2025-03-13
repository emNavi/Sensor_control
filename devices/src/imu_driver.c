/*
 * imu_driver.c
 *
 *  Created on: Mar 10, 2025
 *      Author: xm
 */
#include "imu_driver.h"

extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart1;

float ICM42688_ACCEL_SEN = ICM42688_ACCEL_16G_SEN;
float ICM42688_GYRO_SEN = ICM42688_GYRO_2000_SEN;

void ICM42688_Init(void) {
    // 检查 WHO_AM_I
    uint8_t whoAmI = SPI_ReadRegister(ICM42688_WHO_AM_I);
    if (whoAmI != ICM42688_WHO_AM_I_VALUE) {
        Error_Handler(); // 如果无法检测到 IMU，进入错误处理
    }

    // 开启加速度计和陀螺仪
    SPI_WriteRegister(ICM42688_PWR_MGMT0, 0x0F); // 打开陀螺仪和加速度计

    // 配置陀螺仪范围和采样率
    SPI_WriteRegister(ICM42688_GYRO_CONFIG0, 0x24); // 2000 dps, 1 kHz

    // 配置加速度计范围和采样率
    SPI_WriteRegister(ICM42688_ACCEL_CONFIG0, 0x06); // 16g, 1 kHz
}

uint8_t SPI_ReadRegister(uint8_t reg) {
    uint8_t txData[2] = {reg | 0x80, 0x00}; // MSB=1 表示读操作
    uint8_t rxData[2] = {0};

    SPI1_Select(); // 拉低 CS
    HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 2, HAL_MAX_DELAY);
    SPI1_Deselect(); // 拉高 CS

    return rxData[1]; // 返回数据
}

void SPI_WriteRegister(uint8_t reg, uint8_t data) {
    uint8_t txData[2] = {reg & 0x7F, data}; // MSB=0 表示写操作

    SPI1_Select(); // 拉低 CS
    HAL_SPI_Transmit(&hspi1, txData, 2, HAL_MAX_DELAY);
    SPI1_Deselect(); // 拉高 CS
}

void IMU_ReadAccel(float *imuAccel){

		uint8_t accelData[6];

    // 连续读取加速度计数据寄存器
    for (int i = 0; i < 6; i++) {
        accelData[i] = SPI_ReadRegister(ICM42688_ACCEL_DATA_X1 + i);
    }

    // 合并高低字节
    imuAccel[0] = (int16_t)((accelData[0] << 8) | accelData[1]) * ICM42688_ACCEL_SEN;
    imuAccel[1] = (int16_t)((accelData[2] << 8) | accelData[3]) * ICM42688_ACCEL_SEN;
    imuAccel[2] = (int16_t)((accelData[4] << 8) | accelData[5]) * ICM42688_ACCEL_SEN;
}

void IMU_ReadGyro(float *imuGyro) {

		uint8_t gyroData[6];

    // 连续读取陀螺仪数据寄存器
    for (int i = 0; i < 6; i++) {
        gyroData[i] = SPI_ReadRegister(ICM42688_GYRO_DATA_X1 + i);
    }

    // 合并高低字节
    imuGyro[0] = (int16_t)((gyroData[0] << 8) | gyroData[1]) * ICM42688_GYRO_SEN;
    imuGyro[1] = (int16_t)((gyroData[2] << 8) | gyroData[3]) * ICM42688_GYRO_SEN;
    imuGyro[2] = (int16_t)((gyroData[4] << 8) | gyroData[5]) * ICM42688_GYRO_SEN;
}

void SPI1_Select(void) {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET); // CS low
}

void SPI1_Deselect(void) {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET); // CS high
}

// 正常串口发送函数，无协议
void UART_SendFormatted(const char *label, float x, float y, float z) {
    char buffer[64]; // 输出缓冲区
    //int len = snprintf(buffer, sizeof(buffer), "%s: X=%.3f, Y=%.3f, Z=%.3f\r\n", label, x, y, z);
//    if (len > 0) {
//        HAL_UART_Transmit(&huart1, (uint8_t *)buffer, len, HAL_MAX_DELAY);
//    }
}

void OutputSensorData(float *accelData, float *gyroData) {
    UART_SendFormatted("Accel", accelData[0], accelData[1], accelData[2]);
		HAL_Delay(200);
    UART_SendFormatted("Gyro", gyroData[0], gyroData[1], gyroData[2]);
}
