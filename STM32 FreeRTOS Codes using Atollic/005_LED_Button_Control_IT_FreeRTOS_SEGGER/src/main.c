#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"



void LedFlashTask(void *params);

void GPIO_Config(void);

TaskHandle_t LedFlashHandle = NULL ;
uint8_t count = 0;
int main(void)
{

  DWT->CTRL |= (1 <<0); //



  RCC_DeInit();   // HSE OFF | HSI ON |	PLL OFF |
  SystemCoreClockUpdate(); // SYSTEM CLOCK 16 MHz

  SEGGER_SYSVIEW_Conf();
  SEGGER_SYSVIEW_Start();

  GPIO_Config();

  xTaskCreate(LedFlashTask,"Led Control",configMINIMAL_STACK_SIZE,NULL,3,&LedFlashHandle);

  vTaskStartScheduler(); // START SCHEDULAR
  while (1)
  {


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
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE); //ENABLE SYSCFG FOR EXTI LINE

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource13);//Selecting port and pin for EXTI

  EXTI_InitTypeDef EXTI_InitStruct = {0};
  GPIO_InitTypeDef GPIO_InitStruct ={0};

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1 |GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOC,&GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOC,&GPIO_InitStruct);

  EXTI_InitStruct.EXTI_Line = EXTI_Line13;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;

  EXTI_Init(&EXTI_InitStruct);

  NVIC_SetPriority(EXTI15_10_IRQn,2);
  NVIC_EnableIRQ(EXTI15_10_IRQn);

}

void EXTI15_10_IRQHandler()
{

  EXTI_ClearITPendingBit(EXTI_Line13);
  if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13) == Bit_SET)
  {
	  count++;
	  if(count == 5)
	  {
		  count = 0;
	  }
  }


}
