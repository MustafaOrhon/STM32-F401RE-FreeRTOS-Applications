/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stdio.h"
#include "string.h"
#include "timers.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern TaskHandle_t Menu_Task_Handle;
extern TaskHandle_t CMD_Handle;
extern TaskHandle_t Print_Handle;
extern TaskHandle_t LED_Handle;
extern TaskHandle_t RTC_Handle;
/*********Private Queues*******/
extern QueueHandle_t Input_Data_Queue;
extern QueueHandle_t Print_Queue;

extern TimerHandle_t  handle_led_timer[4];

extern TimerHandle_t rtc_timer;


extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;


/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
typedef struct
{
	uint8_t payload[10];  /*Stores the Data of the command*/
	uint16_t len;        /*command length */
}command_t;
typedef enum
{
	sMainMenu = 0,
	sLedEffect,
	sRTCMenu,
	sRTCTimeConfig,
	sRTCDateConfig,
	sRTCReport,

}state_t;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
extern state_t current_state;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void Menu_Handler(void *params);

void CMD_Handler(void *params);

void Print_Handler(void *params);

void LED_Handler(void *params);

void RTC_Handler(void *params);

void led_effect_stop(void);

void led_effect(int n);
void LED_effect1(void);
void LED_effect2(void);
void LED_effect3(void);
void LED_effect4(void);
void show_time_date_itm(void);

void show_time_date(void);

void rtc_configure_time(RTC_TimeTypeDef *time);

void rtc_configure_date(RTC_DateTypeDef *date);

int validate_rtc_information(RTC_TimeTypeDef *time , RTC_DateTypeDef *date);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
