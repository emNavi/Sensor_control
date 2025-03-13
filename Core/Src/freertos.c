/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for LED_FLASH */
osThreadId_t LED_FLASHHandle;
const osThreadAttr_t LED_FLASH_attributes = {
  .name = "LED_FLASH",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for UART_SEND */
osThreadId_t UART_SENDHandle;
const osThreadAttr_t UART_SEND_attributes = {
  .name = "UART_SEND",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for IMU_DATA */
osThreadId_t IMU_DATAHandle;
const osThreadAttr_t IMU_DATA_attributes = {
  .name = "IMU_DATA",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for BARO_DATA */
osThreadId_t BARO_DATAHandle;
const osThreadAttr_t BARO_DATA_attributes = {
  .name = "BARO_DATA",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for MAG_DATA */
osThreadId_t MAG_DATAHandle;
const osThreadAttr_t MAG_DATA_attributes = {
  .name = "MAG_DATA",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void led_task(void *argument);
void uart_task(void *argument);
void imu_task(void *argument);
void baro_task(void *argument);
void mag_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of LED_FLASH */
  LED_FLASHHandle = osThreadNew(led_task, NULL, &LED_FLASH_attributes);

  /* creation of UART_SEND */
  UART_SENDHandle = osThreadNew(uart_task, NULL, &UART_SEND_attributes);

  /* creation of IMU_DATA */
  IMU_DATAHandle = osThreadNew(imu_task, NULL, &IMU_DATA_attributes);

  /* creation of BARO_DATA */
  BARO_DATAHandle = osThreadNew(baro_task, NULL, &BARO_DATA_attributes);

  /* creation of MAG_DATA */
  MAG_DATAHandle = osThreadNew(mag_task, NULL, &MAG_DATA_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_led_task */
/**
  * @brief  Function implementing the LED_FLASH thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_led_task */
void led_task(void *argument)
{
  /* USER CODE BEGIN led_task */
  /* Infinite loop */
  for(;;)
  {
//	  HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_2);
    osDelay(500);
  }
  /* USER CODE END led_task */
}

/* USER CODE BEGIN Header_uart_task */
/**
* @brief Function implementing the UART_SEND thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_uart_task */
__weak void uart_task(void *argument)
{
  /* USER CODE BEGIN uart_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END uart_task */
}

/* USER CODE BEGIN Header_imu_task */
/**
* @brief Function implementing the IMU_DATA thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_imu_task */
__weak void imu_task(void *argument)
{
  /* USER CODE BEGIN imu_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END imu_task */
}

/* USER CODE BEGIN Header_baro_task */
/**
* @brief Function implementing the BARO_DATA thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_baro_task */
__weak void baro_task(void *argument)
{
  /* USER CODE BEGIN baro_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END baro_task */
}

/* USER CODE BEGIN Header_mag_task */
/**
* @brief Function implementing the MAG_DATA thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_mag_task */
__weak void mag_task(void *argument)
{
  /* USER CODE BEGIN mag_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END mag_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

