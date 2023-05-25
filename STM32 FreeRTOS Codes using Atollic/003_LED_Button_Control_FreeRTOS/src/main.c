#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
void ReadButtonStateTask(void *Params);
void LedFlashTask(void *params);

void GPIO_Config(void);
TaskHandle_t ReadButtonHandle = NULL;
TaskHandle_t LedFlashHandle = NULL ;
uint8_t count = 0;
int main(void)
{
  RCC_DeInit();   // HSE OFF | HSI ON |	PLL OFF |
  SystemCoreClockUpdate(); // SYSTEM CLOCK 16 MHz

  GPIO_Config();

  xTaskCreate(ReadButtonStateTask,"Button Control",configMINIMAL_STACK_SIZE,NULL,3,&ReadButtonHandle);
  xTaskCreate(LedFlashTask,"Led Control",configMINIMAL_STACK_SIZE,NULL,3,&LedFlashHandle);

  vTaskStartScheduler(); // START SCHEDULAR
  while (1)
  {


  }
}

void ReadButtonStateTask(void *params)
{
   while(1)
   {
      if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10) == Bit_SET)
      {

    	  while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)); // This is not good way for professional app.
    	  count++;
    	  if( count < 0 || count > 4)
    	  {
    		  count=0;
    	  }
          taskYIELD();
      }
   }
}
void LedFlashTask(void *params)
{
	   while(1)
	 {
	   if(count == 0)
	      {
	        GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1 |GPIO_Pin_2|GPIO_Pin_3);
	      }
	   else if(count == 1)
        {
        	GPIO_ToggleBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1 |GPIO_Pin_2|GPIO_Pin_3);
        	vTaskDelay(1000);

        }
	   else if(count == 2)
        {
        	GPIO_ToggleBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1 |GPIO_Pin_2|GPIO_Pin_3);
        	vTaskDelay(750);

        }
	   else if(count == 3)
        {
        	GPIO_ToggleBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1 |GPIO_Pin_2|GPIO_Pin_3);
        	vTaskDelay(500);

        }
	   else if(count == 4)
        {
        	GPIO_ToggleBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1 |GPIO_Pin_2|GPIO_Pin_3);
        	vTaskDelay(250);
        }
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
