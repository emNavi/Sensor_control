/*
 * mag_driver.c
 *
 *  Created on: Mar 10, 2025
 *      Author: xm
 */
#include "mag_driver.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;

/*-----I2C接口定义-----*/
#define IST8310_I2C_ADDR 0x0E

void IST8310_INIT(I2C_HandleTypeDef *hi2c, ist8310_data_t* ist8310_data) {
    memset(ist8310_data, 0, sizeof(ist8310_data_t));

    ist8310_data->meg_error = IST8310_NO_ERROR;

    // 基础配置
    // 不使能中断，直接读取
    WriteSingleDataFromIST8310(hi2c, IST8310_CNTL2_ADDR, IST8310_STAT2_NONE_ALL);
    // 平均采样四次
    WriteSingleDataFromIST8310(hi2c, IST8310_AVGCNTL_ADDR, IST8310_AVGCNTL_FOURTH);
    // 200Hz的输出频率
    WriteSingleDataFromIST8310(hi2c, IST8310_CNTL1_ADDR, IST8310_CNTL1_CONTINUE);

    ist8310_data->meg_error |= VerifyMegId(hi2c, &ist8310_data->chip_id);
}

uint8_t ReadSingleDataFromIST8310(I2C_HandleTypeDef *hi2c, uint8_t addr) {
    uint8_t data;
    HAL_I2C_Mem_Read(hi2c, (IST8310_I2C_ADDR << 1), addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    return data;
}

void ReadMultiDataFromIST8310(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t* data, uint8_t len) {
    HAL_I2C_Mem_Read(hi2c, (IST8310_I2C_ADDR << 1), addr, I2C_MEMADD_SIZE_8BIT, data, len, 10);
}

void WriteSingleDataFromIST8310(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t data) {
    HAL_I2C_Mem_Write(hi2c, (IST8310_I2C_ADDR << 1), addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

void WriteMultiDataFromIST8310(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t* data, uint8_t len) {
    HAL_I2C_Mem_Write(hi2c, (IST8310_I2C_ADDR << 1), addr, I2C_MEMADD_SIZE_8BIT, data, len, 10);
}

void ReadIST8310Data(I2C_HandleTypeDef *hi2c, ist8310_raw_data_t* meg_data) {
    uint8_t buf[6];
    int16_t temp_ist8310_data = 0;
    ReadMultiDataFromIST8310(hi2c, IST8310_DATA_XL_ADDR, buf, 6);
    temp_ist8310_data = (int16_t)((buf[1] << 8) | buf[0]);
    meg_data->x = 0.3f * temp_ist8310_data;
    temp_ist8310_data = (int16_t)((buf[3] << 8) | buf[2]);
    meg_data->y = 0.3f * temp_ist8310_data;
    temp_ist8310_data = (int16_t)((buf[5] << 8) | buf[4]);
    meg_data->z = 0.3f * temp_ist8310_data;
}

int CheckIST8310()
{

	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);
	uint8_t who_am_i = 0x20;  // 用于存储读取的值

	HAL_I2C_Mem_Read(&hi2c1, (IST8310_I2C_ADDR << 1), 0x00, I2C_MEMADD_SIZE_8BIT, &who_am_i, 1, 10);
	HAL_UART_Transmit(&huart1, &who_am_i, 1, HAL_MAX_DELAY);
	if (who_am_i == 0x10) {

			return 1;

	} else {
		//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
			return 0;

	}
}

ist8310_error_e VerifyMegId(I2C_HandleTypeDef *hi2c, uint8_t* id) {
    *id = ReadSingleDataFromIST8310(hi2c, IST8310_CHIP_ID_ADDR);
    if (*id != IST8310_CHIP_ID_VAL) {
        return MEG_ID_ERROR;
    } else {
        return IST8310_NO_ERROR;
    }
}
