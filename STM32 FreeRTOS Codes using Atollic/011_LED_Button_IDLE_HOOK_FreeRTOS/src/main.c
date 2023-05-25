#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"


void Task1Handler(void *params);
void Task2Handler(void *params);
void GPIO_Config(void);

TaskHandle_t Task1Handle = NULL ;
TaskHandle_t Task2Handle = NULL ;

int main(void)
{
  RCC_DeInit();   // HSE OFF | HSI ON |	PLL OFF |
  SystemCoreClockUpdate(); // SYSTEM CLOCK 16 MHz

  GPIO_Config();
  xTaskCreate(Task1Handler,"Task 1",configMINIMAL_STACK_SIZE,NULL,2,&Task1Handle);
  xTaskCreate(Task2Handler,"Task 2",configMINIMAL_STACK_SIZE,NULL,2,&Task2Handle);

  vTaskStartScheduler(); // START SCHEDULAR
  while (1)
  {


  }
}

void Task1Handler(void *params)
{
	   while(1)
	 {
         GPIO_ToggleBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);
		 vTaskDelay(pdMS_TO_TICKS(1000));
	 }
}

void Task2Handler(void *params)
{

	   while(1)
	 {
	      GPIO_ToggleBits(GPIOC,GPIO_Pin_2|GPIO_Pin_3);
	      vTaskDelay(pdMS_TO_TICKS(1000));

	 }
}
void GPIO_Config(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);


  GPIO_InitTypeDef GPIO_InitStruct ={0};

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOC,&GPIO_InitStruct);


}


void vApplicationIdleHook(void)
{
   __WFI();

}
