/*
 * accel_calibration.h
 *
 *  Created on: Mar 10, 2025
 *      Author: xm
 */

#ifndef INCLUDE_ACCEL_CALIBRATION_H_
#define INCLUDE_ACCEL_CALIBRATION_H_

#include "imu_driver.h"
#include <stdint.h>
#include <stdbool.h>

#define ACCEL_SAMPLES_NUM 750  // 采样次数
#define GRAVITY 9.80665f       // 标准重力加速度
#define ACCEL_THRESHOLD 0.9f   // 用于判断方向的加速度阈值

typedef struct {
    float offset[3];   // XYZ 轴偏移
    float scale[3][3];    // XYZ 轴缩放因子
} accel_calibration_t;

bool accel_calibrate(void);
void apply_accel_calibration(float accel[3]);


#endif /* INCLUDE_ACCEL_CALIBRATION_H_ */
