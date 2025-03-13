/*
 * sensor_task.c
 *
 *  Created on: Mar 10, 2025
 *      Author: xm
 */
#include "sensor_task.h"
#include "cmsis_os.h" // FreeRTOS 相关头文件

extern I2C_HandleTypeDef hi2c1;
extern BMP280_CalibData bmp280_calib;

IMUData_t imuData;
float bmp280_t, bmp280_p, altitude;
ist8310_raw_data_t ist8310_raw_data;
extern uint8_t gps_rx_data;

void imu_task(void *argument) {
    for (;;) {
        // 读取 IMU 数据
    	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_1);
        IMU_ReadAccel(imuData.accel);
        IMU_ReadGyro(imuData.gyro);
				osDelay(100);
    }
}

void baro_task(void *argument)
{
  for(;;)
  {
	  bmp280_t = BMP280_ReadTemperature(&hi2c1, &bmp280_calib);
	  bmp280_p = BMP280_ReadPressure(&hi2c1, &bmp280_calib);
//    altitude = BMP280_CalculateAltitude(bmp280_p);
    osDelay(100);
  }

}

void mag_task(void *argument)
{
	for(;;)
  {
		ReadIST8310Data(&hi2c1, &ist8310_raw_data);
		osDelay(200);
  }
}
