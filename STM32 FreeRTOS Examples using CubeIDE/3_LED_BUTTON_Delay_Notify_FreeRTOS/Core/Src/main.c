/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;
TaskHandle_t Task3_Handle;
TaskHandle_t Button_Control_Handle;
TaskHandle_t volatile next_task_handle = NULL;
#define DWT_CTRL (*(volatile uint32_t*)0xE0001000)
BaseType_t status;
char tx_buffer[50];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
static void Task1_Handler(void *params);
static void Task2_Handler(void *params);
static void Task3_Handler(void *params);
static void Button_Handler(void *params);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  DWT_CTRL |= (1<<0);


  SEGGER_SYSVIEW_Conf();
  vSetVarulMaxPRIGROUPValue();
  SEGGER_SYSVIEW_Start();


/**********************************PREEMPTIVE IS DISABLED SO CO-OPERATIVE SCHEDULING IS WORKING********************/
status = xTaskCreate(Task1_Handler,"Red Led Task",configMINIMAL_STACK_SIZE,NULL,3,&Task1_Handle);

configASSERT(status == pdPASS); // Check if it is succesfully created
next_task_handle = Task1_Handle;

status = xTaskCreate(Task2_Handler,"Green Led Task",configMINIMAL_STACK_SIZE,NULL,2,&Task2_Handle);

configASSERT(status == pdPASS);  // Check if it is succesfully created

status = xTaskCreate(Task3_Handler,"Blue Leds Task",configMINIMAL_STACK_SIZE,NULL,1,&Task3_Handle);

configASSERT(status == pdPASS); // Check if it is succesfully created

status = xTaskCreate(Button_Handler,"Button Control Task",configMINIMAL_STACK_SIZE,NULL,4,&Button_Control_Handle);

configASSERT(status == pdPASS); // Check if it is succesfully created

vTaskStartScheduler();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC0 PC1 PC2 PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
static void Task1_Handler(void *params)
{
    BaseType_t status;

	while(1)
	{
        HAL_UART_Transmit(&huart2,(uint8_t*)tx_buffer,sprintf(tx_buffer,"Toggling Red Button\r\n"),HAL_MAX_DELAY);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
        status = xTaskNotifyWait(0,0,NULL,pdMS_TO_TICKS(1000));
        if(status == pdTRUE)
        {
           vTaskSuspendAll();
           next_task_handle = Task2_Handle;
           xTaskResumeAll();
           HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);
           vTaskDelete(NULL); // Delete itself

        }

	}

}
static void Task2_Handler(void *params)
{
    BaseType_t status;

	while(1)
	{
        HAL_UART_Transmit(&huart2,(uint8_t*)tx_buffer,sprintf(tx_buffer,"Toggling Green Button\r\n"),HAL_MAX_DELAY);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
        status = xTaskNotifyWait(0,0,NULL,pdMS_TO_TICKS(800));
        if(status == pdTRUE)
        {
           vTaskSuspendAll();
           next_task_handle = Task3_Handle;
           xTaskResumeAll();
           HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
           vTaskDelete(NULL); // Delete itself

        }

	}
}
static void Task3_Handler(void *params)
{
    BaseType_t status;

	while(1)
	{
        HAL_UART_Transmit(&huart2,(uint8_t*)tx_buffer,sprintf(tx_buffer,"Toggling Blue Buttons\r\n"),HAL_MAX_DELAY);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2|GPIO_PIN_3);
        status = xTaskNotifyWait(0,0,NULL,pdMS_TO_TICKS(400));
        if(status == pdTRUE)
        {
           vTaskSuspendAll();
           next_task_handle = NULL;
           xTaskResumeAll();
           HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2|GPIO_PIN_3,GPIO_PIN_SET);
           vTaskDelete(Button_Control_Handle); // Delete Button Task
           vTaskDelete(NULL); // Delete itself

        }

	}
}
static void Button_Handler(void *params)
{
	bool btn_read = false;
	bool prvs_read = false;

	 while(1)
	 {
         btn_read = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10);
         if(btn_read)
         {
        	  if(!prvs_read)
        	  {
        		xTaskNotify(next_task_handle,0,eNoAction);
        	  }
         }
         prvs_read = btn_read;
         vTaskDelay(pdMS_TO_TICKS(10));


	 }

}
void vApplicationIdleHook(void)
{
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON,PWR_SLEEPENTRY_WFI);
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
