/*
 * gyro_calibration.h
 *
 *  Created on: Mar 10, 2025
 *      Author: xm
 */

#ifndef INCLUDE_GYRO_CALIBRATION_H_
#define INCLUDE_GYRO_CALIBRATION_H_

#include "imu_driver.h"

#define WINDOW_SIZE 51         // Median 过滤窗口大小
#define CALIBRATION_COUNT 500  // 校准采样次数
#define MAX_OFFSET_DEG 0.6f    // 最大允许偏差 (°)
#define MAX_OFFSET_RAD (MAX_OFFSET_DEG * (M_PI / 180.0f))  // 角度转换为弧度
#define MAX_RETRY 5            // 允许最大重试次数
#define M_PI 3.1415926

// 结构体存储校准数据
typedef struct {
    float buffer[WINDOW_SIZE];  // 采样窗口
    uint16_t index;
} Filter;

float median(float data[][3], int axis);

int calibrate_gyro();

#endif /* INCLUDE_GYRO_CALIBRATION_H_ */
