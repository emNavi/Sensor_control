/*
 * gyro_calibration.c
 *
 *  Created on: Mar 10, 2025
 *      Author: xm
 */


#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include "main.h"
#include "gyro_calibration.h"
#include "uart_task.h"
#define SAMPLE_COUNT 1000        // 采样次数

extern UART_HandleTypeDef huart1;  // 串口4句柄

float gyro_data[SAMPLE_COUNT][3];  // 存储所有采样数据
float gyro_offset[3] = {0.0f, 0.0f, 0.0f};  // X, Y, Z 轴的零偏

int cmp(const void *a, const void *b) {
        return (*(float *)a > *(float *)b) - (*(float *)a < *(float *)b);
}

float median(float data[][3], int axis) {
    float sorted[SAMPLE_COUNT];

    for (int i = 0; i < SAMPLE_COUNT; i++) {
        sorted[i] = data[i][axis];
    }

    // 快速排序
    qsort(sorted, SAMPLE_COUNT, sizeof(float), cmp);

    return sorted[SAMPLE_COUNT / 2];  // 取中位数
}

// 插入新数据
void filter_insert(Filter *f, float value) {
    f->buffer[f->index % WINDOW_SIZE] = value;
    f->index++;
}


// 校准陀螺仪
int calibrate_gyro() {
    char gyro_msg[100];

    // 清空滤波器
    memset(&gyro_data, 0, sizeof(gyro_data));

    uart_print("Starting Gyro Calibration...\r\n");

		int collected_samples = 0;

    // 采集 1000 组数据
    while (collected_samples < SAMPLE_COUNT) {
        IMU_ReadGyro(gyro_data[collected_samples]);  // 读取陀螺仪数据
        collected_samples++;

				if ((collected_samples + 1) % (SAMPLE_COUNT / 10) == 0) {
            snprintf(gyro_msg, sizeof(gyro_msg), "Calibration Progress: %d%%\r\n", ((collected_samples + 1) * 100) / SAMPLE_COUNT);
            uart_print(gyro_msg);
        }

        HAL_Delay(2);  // 适当延时，保证采样时间间隔
    }

    // 计算零偏（均值）
    for (int axis = 0; axis < 3; axis++) {
        float sum = 0.0f;
        for (int i = 0; i < SAMPLE_COUNT; i++) {
            sum += gyro_data[i][axis];
        }
        gyro_offset[axis] = sum / SAMPLE_COUNT;  // 计算均值
    }

		snprintf(gyro_msg, sizeof(gyro_msg), "Gyro calibration done: offset X=%.6f, Y=%.6f, Z=%.6f\n", gyro_offset[0], gyro_offset[1], gyro_offset[2]);
    uart_print(gyro_msg);

    // **检测是否发生移动**
    float xdiff = median(gyro_data, 0) - gyro_offset[0];
    float ydiff = median(gyro_data, 1) - gyro_offset[1];
    float zdiff = median(gyro_data, 2) - gyro_offset[2];
		snprintf(gyro_msg, sizeof(gyro_msg), "Gyro: xm=%.6f, ym=%.6f, zm=%.6f\n", median(gyro_data, 0), median(gyro_data, 1), median(gyro_data, 2));
    uart_print(gyro_msg);
		snprintf(gyro_msg, sizeof(gyro_msg), "Gyro: xdiff=%.6f, ydiff=%.6f, zdiff=%.6f\n", xdiff, ydiff, zdiff);
    uart_print(gyro_msg);
    if (fabsf(xdiff) > MAX_OFFSET_RAD || fabsf(ydiff) > MAX_OFFSET_RAD || fabsf(zdiff) > MAX_OFFSET_RAD) {
        uart_print("Motion detected! Retrying calibration...\r\n");
        return calibrate_gyro();  // 重新校准
    }

    uart_print("Gyro Calibration Success!\r\n");
    return 0;
}
