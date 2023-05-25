
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

void GPIO_Config(void);
void Task1Handler(void * params);
void Task2Handler(void * params);
TaskHandle_t Task1Handle = NULL;
TaskHandle_t Task2Handle = NULL ;

void RTOS_Delay(uint32_t delayms)
{
   uint32_t currentTick = xTaskGetTickCount();
   uint32_t delayTick = pdMS_TO_TICKS(delayms);

   while(xTaskGetTickCount() < (currentTick + delayTick));

}

int main(void)
{
  RCC_DeInit(); // System Clock SET TO HSI 16MHZ

  SystemCoreClockUpdate(); // SystemCoreClock Value Updated

  GPIO_Config();

  xTaskCreate(Task1Handler,"Task 1",configMINIMAL_STACK_SIZE,NULL,4,&Task1Handle);
  xTaskCreate(Task2Handler,"Task 2",configMINIMAL_STACK_SIZE,NULL,3,&Task2Handle);

  vTaskStartScheduler(); // START SCHEDULAR


  while (1)
  {



  }
}

void GPIO_Config(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
  GPIO_InitTypeDef GPIO_InitStruct ={0};

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOC,&GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1 |GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOC,&GPIO_InitStruct);

}
void Task1Handler(void * params)
{
    while(1)
    {
       if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10) == Bit_SET)
       {
    	   GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);
      	  vTaskDelete(Task1Handle);
       }
	  GPIO_ToggleBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);
      RTOS_Delay(1000);

    }

}
void Task2Handler(void * params)
{
	while(1)
	 {
       GPIO_ToggleBits(GPIOC,GPIO_Pin_2|GPIO_Pin_3);
       RTOS_Delay(500);
	 }
}
