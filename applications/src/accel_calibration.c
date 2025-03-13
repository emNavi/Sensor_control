/*
 * accel_calibration.c
 *
 *  Created on: Mar 10, 2025
 *      Author: xm
 */


#include "accel_calibration.h"

#include <stdio.h>
#include <string.h>
#include "arm_math.h"
#include <math.h>
#include <uart_task.h>

static float accel_data[6][3];  // 存储六个方向的加速度数据
static accel_calibration_t accel_calib = {0};
char accel_msg[100];

static const char *direction_names[6] = {"POS_X", "NEG_X", "POS_Y", "NEG_Y", "POS_Z", "NEG_Z"};
static bool collected[6] = {false};  // 标记方向是否采集

// 计算加速度的方向
static int detect_orientation(float accel[3]) {
    float max_value = fabs(accel[0]);
    int axis = 0; // 0: X, 1: Y, 2: Z

    for (int i = 1; i < 3; i++) {
        if (fabs(accel[i]) > max_value) {
            max_value = fabs(accel[i]);
            axis = i;
        }
    }

    if (accel[axis] > ACCEL_THRESHOLD) return axis * 2;     // 正方向
    if (accel[axis] < -ACCEL_THRESHOLD) return axis * 2 + 1; // 负方向
    return -1; // 未识别方向
}

// 获取静止状态下的加速度均值
static bool get_accel_average(float avg[3]) {
    float sum[3] = {0};
    for (int i = 0; i < ACCEL_SAMPLES_NUM; i++) {
        float accel[3];
        IMU_ReadAccel(accel);
        for (int j = 0; j < 3; j++) {
            sum[j] += accel[j];
        }
    }
    for (int j = 0; j < 3; j++) {
        avg[j] = sum[j] / ACCEL_SAMPLES_NUM;
    }
    return true;
}

// 采集六个方向的数据
static bool collect_accel_data() {
    uart_print("Rotate sensor to different orientations...\n");
    int collected_count = 0;

    while (collected_count < 6) {
        float accel[3];
        IMU_ReadAccel(accel);  //读取IMU_accel数据

//				snprintf(accel_msg, sizeof(accel_msg), "X=%.3f, Y=%.3f, Z=%.3f\r\n", accel[0],accel[1],accel[2]);
//				uart_print(accel_msg);

        int detected_index = detect_orientation(accel);

        if (detected_index != -1 && !collected[detected_index]) {
			snprintf(accel_msg, sizeof(accel_msg), "Detected: %s, hold still...\n", direction_names[detected_index]);
			uart_print(accel_msg);
            HAL_Delay(3000); // 稳定等待
            if (get_accel_average(accel_data[detected_index])) {
                collected[detected_index] = true;
                collected_count++;
                snprintf(accel_msg, sizeof(accel_msg), "%s collected: [%.3f, %.3f, %.3f]\nPlease rotate sensor to next orientations\n",
                       direction_names[detected_index],
                       accel_data[detected_index][0],
                       accel_data[detected_index][1],
                       accel_data[detected_index][2]);
				uart_print(accel_msg);
				HAL_Delay(3000); // 稳定等待
            }
        }
				else {
//					snprintf(accel_msg, sizeof(accel_msg), "Unrecognized direction, detected_index = %d\n", detected_index);
//					uart_print(accel_msg);
				}
        HAL_Delay(500);
    }
    return true;
}

// 计算校准参数
static bool compute_calibration() {
    arm_matrix_instance_f32 A, A_inv, B, X;
    // float accel_row_ref[6][3];
	float mat_A[3][3];
    float mat_B[3][3] = {
						{GRAVITY, 0, 0},
						{0, GRAVITY, 0},
						{0, 0, GRAVITY}
		};
    float mat_A_inv[3][3], mat_X[3][3];

//    // 填充 A 矩阵
//    for (int i = 0; i < 6; i++) {
//        for (int j = 0; j < 3; j++) {
//            accel_row_ref[i][j] = accel_data[i][j];
//        }
//    }

		// 计算偏移量, 填充 A 矩阵
    for (int i = 0; i < 3; i++) {
        accel_calib.offset[i] = (accel_data[i*2][i] + accel_data[i*2+1][i]) / 2;
		for (int j = 0; j < 3; j++) {
			mat_A[i][j] = accel_data[i*2][j] - accel_calib.offset[i];
		}
    }

//    // 初始化矩阵
    arm_mat_init_f32(&A, 3, 3, (float32_t *)mat_A);
    arm_mat_init_f32(&A_inv, 3, 3, (float32_t *)mat_A_inv);
    arm_mat_init_f32(&B, 3, 3, (float32_t *)mat_B);
    arm_mat_init_f32(&X, 3, 3, (float32_t *)mat_X);


    // 计算 A 的逆
    if (arm_mat_inverse_f32(&A, &A_inv) != ARM_MATH_SUCCESS) {
        uart_print("Matrix inversion failed!\n");
        return false;
    }

    // 计算 X = A^(-1) B
    arm_mat_mult_f32(&A_inv, &B, &X);

    // 存储计算结果
    memcpy(accel_calib.scale, mat_X, sizeof(mat_X));


    snprintf(accel_msg, sizeof(accel_msg), "Calibration Complete!\nOffsets: [%.3f, %.3f, %.3f]\nScales: [%.3f, %.3f, %.3f]\n",
           accel_calib.offset[0], accel_calib.offset[1], accel_calib.offset[2],
           accel_calib.scale[0][0], accel_calib.scale[1][1], accel_calib.scale[2][2]);
		uart_print(accel_msg);

    return true;
}

// 运行校准流程
bool accel_calibrate() {
    uart_print("Starting accelerometer calibration...\n");
		memset(collected, false, sizeof(collected));

    if (!collect_accel_data()) {
        uart_print("Data collection failed!\n");
        return false;
    }
    return compute_calibration();
}
