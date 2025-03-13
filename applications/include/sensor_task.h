/*
 * sensor_task.h
 *
 *  Created on: Mar 10, 2025
 *      Author: xm
 */

#ifndef APPLICATIONS_INCLUDE_SENSOR_TASK_H_
#define APPLICATIONS_INCLUDE_SENSOR_TASK_H_

//#include "imu_driver.h"
//#include "baro_driver.h"
//#include "mag_driver.h"
#include "main.h"
// 数据结构
typedef struct {
    float accel[3];
    float gyro[3];
} IMUData_t;



#endif /* APPLICATIONS_INCLUDE_SENSOR_TASK_H_ */
