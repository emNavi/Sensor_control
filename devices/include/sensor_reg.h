#pragma once


/*-----ICM42688寄存器地址-----*/
#define ICM42688_WHO_AM_I           0x75
#define ICM42688_WHO_AM_I_VALUE     0x47  // ICM-42688 的 WHO_AM_I 返回值
#define ICM42688_PWR_MGMT0          0x4E  // 电源管理寄存器
#define ICM42688_GYRO_CONFIG0       0x4F  // 陀螺仪配置
#define ICM42688_ACCEL_CONFIG0      0x50  // 加速度计配置
#define ICM42688_INT_CONFIG         0x14  // 中断配置
#define ICM42688_ACCEL_DATA_X1      0x1F  // 加速度计 X 高字节
#define ICM42688_GYRO_DATA_X1       0x25  // 陀螺仪 X 高字节
/*-----------------------------*/


/*-----BMP280寄存器地址-----*/
#define BMP280_I2C_ADDRESS  0x76 << 1    // BMP280 I2C 地址 (SDO接GND为0x76, 接VCC为0x77)

// BMP280 主要寄存器地址
#define BMP280_REG_ID       0xD0  // 设备ID寄存器
#define BMP280_REG_RESET    0xE0  // 复位寄存器
#define BMP280_REG_CTRL     0xF4  // 控制测量寄存器
#define BMP280_REG_CONFIG   0xF5  // 配置寄存器
#define BMP280_REG_PRESS_MSB 0xF7 // 气压MSB寄存器
#define BMP280_REG_TEMP_MSB  0xFA // 温度MSB寄存器
/*--------------------------*/


/*-----IST8310寄存器地址-----*/
#define IST8310_CHIP_ID_ADDR 0x00
#define IST8310_CHIP_ID_VAL 0x10

#define IST8310_STAT1_ADDR 0x02

#define IST8310_DATA_XL_ADDR 0x03
#define IST8310_DATA_XH_ADDR 0x04
#define IST8310_DATA_YL_ADDR 0x05
#define IST8310_DATA_YH_ADDR 0x06
#define IST8310_DATA_ZL_ADDR 0x07
#define IST8310_DATA_ZH_ADDR 0x08

#define IST8310_STAT2_ADDR 0x09

#define IST8310_CNTL1_ADDR 0x0A
#define IST8310_CNTL1_SLEEP 0x00
#define IST8310_CNTL1_SINGLE 0x01
#define IST8310_CNTL1_CONTINUE 0x0B

#define IST8310_CNTL2_ADDR 0x0B
#define IST8310_STAT2_NONE_ALL 0x00

#define IST8310_SELF_CHECK_ADDR 0x0C

#define IST8310_TEMPL_ADDR 0x1C
#define IST8310_TEMPH_ADDR 0x1D

#define IST8310_AVGCNTL_ADDR 0x41
#define IST8310_AVGCNTL_TWICE 0x09
#define IST8310_AVGCNTL_FOURTH 0x12
/*-----------------------------*/
