#include "uart_task.h"
#include "sensor_task.h"
#include "imu_driver.h"
#include "cmsis_os.h"

#define __TEST_FUNCTION_ 1
extern UART_HandleTypeDef huart1;

extern IMUData_t imuData;
extern float bmp280_t, bmp280_p, altitude;
extern ist8310_raw_data_t ist8310_raw_data;

extern osThreadId_t gpsReceiveTaskHandle;
extern osThreadId_t gpsTransmitTaskHandle;
extern osMessageQueueId_t gpsQueueHandle;

//mavlink_message_t msg;
//uint8_t buffer[50];
//uint16_t len;
char data[128];
#if __TEST_FUNCTION_
void uart_task(void *argument)
{
	for (;;) {
			uint8_t gps_message[128];
			//char data[128]; // 输出缓冲区
			//int len = snprintf(data, sizeof(data), "temperature =%.3f , altitude =%.3f\r\n", bmp280_t, altitude);
			//printf(data, sizeof(data), "ist8310: x=%.3f, y=%.3f, z=%.3f\r\n", ist8310_raw_data.x, ist8310_raw_data.y, ist8310_raw_data.z);
			//int len = snprintf(data, sizeof(data), "ist8310: x=%.3f, y=%.3f, z=%.3f\r\n", ist8310_raw_data.x, ist8310_raw_data.y, ist8310_raw_data.z);
			//int len = snprintf(data, sizeof(data), "ist8310: x=%.3f, y=%.3f, z=%.3f\ntemperature =%.3f , altitude =%.3f\n", ist8310_raw_data.x, ist8310_raw_data.y, ist8310_raw_data.z, bmp280_t, altitude);
		int len = snprintf(data, sizeof(data), "gyro: x=%.3f, y=%.3f, z=%.3f\r\n", imuData.gyro[0], imuData.gyro[1], imuData.gyro[2]);
		if (len > 0) {
			HAL_UART_Transmit(&huart1, (uint8_t *)data, len, HAL_MAX_DELAY);

			}
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
					// 延时
			osDelay(1000);
    }
}
#else
void uart_task(void *argument)
{
	for (;;) {

        // 打包 MAVLink 消息
        mavlink_msg_imu_data_pack(1, 1000, &msg, imuData.accel[0], imuData.accel[1], imuData.accel[2], imuData.gyro[0], imuData.gyro[1], imuData.gyro[2]);

        // 转换为字节流
        len = mavlink_msg_to_send_buffer(buffer, &msg);

        // 通过串口发送
        HAL_UART_Transmit(&huart1, buffer, len, HAL_MAX_DELAY);

        // 延时
				osDelay(100);
    }
}
#endif

// 串口输出函数
void uart_print(const char *msg) {
    HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
}
