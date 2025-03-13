/*
 * imu_driver.h
 *
 *  Created on: Mar 10, 2025
 *      Author: xm
 */

#ifndef DEVICES_INCLUDE_IMU_DRIVER_H_
#define DEVICES_INCLUDE_IMU_DRIVER_H_


#include "main.h"

// 单位：g/LSB
#define ICM42688_ACCEL_2G_SEN 0.000061f
#define ICM42688_ACCEL_4G_SEN 0.000122f
#define ICM42688_ACCEL_8G_SEN 0.000244f
#define ICM42688_ACCEL_16G_SEN 0.000488f

// 单位：°/s/LSB
#define ICM42688_GYRO_2000_SEN 0.06103f
#define ICM42688_GYRO_1000_SEN 0.03052f
#define ICM42688_GYRO_500_SEN 0.01526f
#define ICM42688_GYRO_250_SEN 0.00763f

void ICM42688_Init(void);
uint8_t SPI_ReadRegister(uint8_t reg);
void SPI_WriteRegister(uint8_t reg, uint8_t data);
void IMU_ReadAccel(float *imuAccel);
void IMU_ReadGyro(float *imuGyro);
void SPI1_Select(void);
void SPI1_Deselect(void);
void UART_SendFormatted(const char *label, float x, float y, float z);
void OutputSensorData(float *accelData, float *gyroData);



#endif /* DEVICES_INCLUDE_IMU_DRIVER_H_ */
