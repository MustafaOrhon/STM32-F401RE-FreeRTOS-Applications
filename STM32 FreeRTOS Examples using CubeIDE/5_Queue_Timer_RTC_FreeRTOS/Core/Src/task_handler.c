/*
 * task_handler.c
 *
 *  Created on: May 23, 2023
 *      Author: Mustafa
 */
#include "main.h"
#include "string.h"
const char *msg_inv = "Not Correct Input\r\n";
extern UART_HandleTypeDef huart2;

int extract_command(command_t *cmd);
void process_command(command_t *cmd);
void Menu_Handler(void *params)
{
  const char* msg_menu = 	  "==================\n"
		  	  	  	  	  	  "|     MENU        \n"
		  	  	  	  	  	  "==================\n"
		  	  	  	  	  	  "Led EFFECT ----->0\n"
		  	  	  	  	  	  "Date and Time ----->1\n"
		  	  	  	  	  	  "Exit ----->2\n"
		  	  	  	  	  	  "Enter your choice here:";

  uint32_t cmd_addr;
  command_t *cmd;
  int option;

  while(1)
  {
    xQueueSend(Print_Queue,(void*)&msg_menu,portMAX_DELAY);
    xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);
    cmd =(command_t*)cmd_addr;

    if(cmd->len == 1)
    {
       option = cmd->payload[0] - 48;
       switch(option)
    	   {
       case 0:
    	   current_state = sLedEffect;
    	   xTaskNotify(LED_Handle,0,eNoAction);
    	   break;
       case 1:
    	   current_state = sRTCMenu;
    	   xTaskNotify(RTC_Handle,0,eNoAction);
    	   break;
       case 2:
              // Exit with amp
    	   break;
       default:
    	   xQueueSend(Print_Queue,(void*)&msg_inv,portMAX_DELAY);
    	   continue;
    	   }

    }
    else
    {
    	// Invalid Entry
    	xQueueSend(Print_Queue,(void*)&msg_inv,portMAX_DELAY);
    	continue;

    }
    xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);

  }
}
void CMD_Handler(void *params)
{
	BaseType_t status;
	command_t cmd;
	  while(1)
	  {
           status=xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
           if(status == pdTRUE)
           {
        	   process_command(&cmd);
           }

	  }
}
void Print_Handler(void *params)
{
	uint32_t *msg;
	  while(1)
	  {
		  xQueueReceive(Print_Queue,&msg,portMAX_DELAY);
		  HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen((char*)msg),HAL_MAX_DELAY);

	  }
}
void LED_Handler(void *params)
{

	uint32_t cmd_addr;
	command_t *cmd;
	const char* msg_led = "========================\n"
						  "|      LED Effect     |\n"
						  "========================\n"
						  "(none,e1,e2,e3,e4)\n"
						  "Enter your choice here : ";

	while(1){
		/*Wait for notification (Notify wait) */
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);

		/*Print LED menu */
         xQueueSend(Print_Queue,&msg_led,portMAX_DELAY);
		/*wait for LED command (Notify wait) */
         xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);
         cmd =(command_t*)cmd_addr;
		if(cmd->len <= 4)
		{
			if(! strcmp((char*)cmd->payload,"none"))
				led_effect_stop();
			else if (! strcmp((char*)cmd->payload,"e1"))
			{

				led_effect(1);

			}

			else if (! strcmp((char*)cmd->payload,"e2"))
				led_effect(2);
			else if (! strcmp((char*)cmd->payload,"e3"))
				led_effect(3);
			else if (! strcmp((char*)cmd->payload,"e4"))
				led_effect(4);
			else
				/*print invalid message */
				xQueueSend(Print_Queue,(void*)&msg_inv,portMAX_DELAY);
		}
		else
		{
			xQueueSend(Print_Queue,(void*)&msg_inv,portMAX_DELAY);
		}

		/*update state variable */
		current_state = sMainMenu;

		/*Notify menu task */
		xTaskNotify(Menu_Task_Handle,0,eNoAction);

	}
}
uint8_t getnumber(uint8_t *p , int len)
{

	int value ;

	if(len > 1)
	   value =  ( ((p[0]-48) * 10) + (p[1] - 48) );
	else
		value = p[0] - 48;

	return value;

}
void RTC_Handler(void *params)
{
	const char* msg_rtc1 = "========================\n"
								"|         RTC          |\n"
								"========================\n";

		const char* msg_rtc2 = "Configure Time            ----> 0\n"
								"Configure Date            ----> 1\n"
								"Enable reporting          ----> 2\n"
								"Exit                      ----> 3\n"
								"Enter your choice here : ";


		const char *msg_rtc_hh = "Enter hour(1-12):";
		const char *msg_rtc_mm = "Enter minutes(0-59):";
		const char *msg_rtc_ss = "Enter seconds(0-59):";

		const char *msg_rtc_dd  = "Enter date(1-31):";
		const char *msg_rtc_mo  ="Enter month(1-12):";
		const char *msg_rtc_dow  = "Enter day(1-7 sun:1):";
		const char *msg_rtc_yr  = "Enter year(0-99):";

		const char *msg_conf = "Configuration successful\n";
		const char *msg_rtc_report = "Enable time&date reporting(y/n)?: ";


		uint32_t cmd_addr;
		command_t *cmd;
		static int rtc_state = 0;
		int menu_code;

		RTC_TimeTypeDef time;
		RTC_DateTypeDef date;

#define HH_CONFIG 		0
#define MM_CONFIG 		1
#define SS_CONFIG 		2

#define DATE_CONFIG 	0
#define MONTH_CONFIG 	1
#define YEAR_CONFIG 	2
#define DAY_CONFIG 		3
		while(1){
			/*
			 Notify wait (wait till someone notifies)*/
			xTaskNotifyWait(0,0,NULL,portMAX_DELAY);

			/*Print the menu and show current date and time information */
	         xQueueSend(Print_Queue,&msg_rtc1,portMAX_DELAY);
	         show_time_date();
	         xQueueSend(Print_Queue,&msg_rtc2,portMAX_DELAY);


			while(current_state!= sMainMenu){

				/*Wait for command notification (Notify wait) */
		         xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);
		         cmd =(command_t*)cmd_addr;
				switch(current_state)
				{
					case sRTCMenu:{
						if(cmd->len ==1)
						{		/* process RTC menu commands */
							menu_code = cmd->payload[0] - 48;
							switch(menu_code)
							{
							case 0:
								current_state = sRTCTimeConfig;
								xQueueSend(Print_Queue,&msg_rtc_hh,portMAX_DELAY);
								break;
							case 1:
								current_state = sRTCDateConfig;
								xQueueSend(Print_Queue,&msg_rtc_dd,portMAX_DELAY);
								break;
							case 2:
								current_state = sRTCReport;
								xQueueSend(Print_Queue,&msg_rtc_report,portMAX_DELAY);
								break;
							case 3:
								current_state = sMainMenu;
								break;
							default:
								current_state = sMainMenu;
								xQueueSend(Print_Queue,&msg_inv,portMAX_DELAY);
							}
						}else
						{
							current_state = sMainMenu;
							xQueueSend(Print_Queue,&msg_inv,portMAX_DELAY);
						}

						break;}

					case sRTCTimeConfig:{
						/* get hh, mm, ss infor and configure RTC */
                         switch(rtc_state)
                         {
                         case HH_CONFIG :
                         {
                        	 uint8_t hour = getnumber(cmd->payload,cmd->len);
                        	 time.Hours = hour;
                        	 rtc_state = MM_CONFIG;
                        	 xQueueSend(Print_Queue,&msg_rtc_mm,portMAX_DELAY);
                        	 break;}
                         case MM_CONFIG :
                                           {
                         	 uint8_t min = getnumber(cmd->payload,cmd->len);
                        	 time.Minutes = min;
                        	 rtc_state = SS_CONFIG;
                        	 xQueueSend(Print_Queue,&msg_rtc_ss,portMAX_DELAY);
                        	 break;}
                         case SS_CONFIG :{
                         	 uint8_t sec = getnumber(cmd->payload,cmd->len);
                        	 time.Seconds = sec;
                        	 if(!validate_rtc_information(&time,NULL))
                        	 {
                        		 rtc_configure_time(&time);
                        		 xQueueSend(Print_Queue,&msg_conf,portMAX_DELAY);
                        		 show_time_date();
                        	 }
                        	 else
                        		 xQueueSend(Print_Queue,&msg_inv,portMAX_DELAY);

								current_state = sMainMenu;
								rtc_state = 0;
                        	 break;}


                         }
						break;}

					case sRTCDateConfig:{

						/*get date, month, day , year info and configure RTC */
						switch(rtc_state)
							{
								case DATE_CONFIG:{
									uint8_t d = getnumber(cmd->payload , cmd->len);
									date.Date = d;
									rtc_state = MONTH_CONFIG;
									xQueueSend(Print_Queue,&msg_rtc_mo,portMAX_DELAY);
									break;}
								case MONTH_CONFIG:{
									uint8_t month = getnumber(cmd->payload , cmd->len);
									date.Month = month;
									rtc_state = DAY_CONFIG;
									xQueueSend(Print_Queue,&msg_rtc_dow,portMAX_DELAY);
									break;}
								case DAY_CONFIG:{
									uint8_t day = getnumber(cmd->payload , cmd->len);
									date.WeekDay = day;
									rtc_state = YEAR_CONFIG;
									xQueueSend(Print_Queue,&msg_rtc_yr,portMAX_DELAY);
									break;}
								case YEAR_CONFIG:{
									uint8_t year = getnumber(cmd->payload , cmd->len);
									date.Year = year;

									if(!validate_rtc_information(NULL,&date))
									{
										rtc_configure_date(&date);
										xQueueSend(Print_Queue,&msg_conf,portMAX_DELAY);
										show_time_date();
									}else
										xQueueSend(Print_Queue,&msg_inv,portMAX_DELAY);

									current_state = sMainMenu;
									rtc_state = 0;
									break;}
							}

						break;}

					case sRTCReport:{
						/*enable or disable RTC current time reporting over ITM printf */
						if(cmd->len == 1)
						{
							if(cmd->payload[0] == 'y'){
								if(xTimerIsTimerActive(rtc_timer) == pdFALSE)
									xTimerStart(rtc_timer,portMAX_DELAY);
							}else if (cmd->payload[0] == 'n'){
								xTimerStop(rtc_timer,portMAX_DELAY);
							}else{
								xQueueSend(Print_Queue,&msg_inv,portMAX_DELAY);
							}

						}else
						    xQueueSend(Print_Queue,&msg_inv,portMAX_DELAY);

						current_state = sMainMenu;
						break;}

				}// switch end

			} //while end

			   /*Notify menu task */
              xTaskNotify(Menu_Task_Handle,0,eNoAction);

			}//while super loop end
}
void process_command(command_t *cmd)
{
    extract_command(cmd);

    switch(current_state)
    {
      case sMainMenu:
    	  xTaskNotify(Menu_Task_Handle,(uint32_t)cmd,eSetValueWithOverwrite);
    	  break;

      case sLedEffect:
    	  xTaskNotify(LED_Handle,(uint32_t)cmd,eSetValueWithOverwrite);
    	  break;

      case sRTCMenu:
      case sRTCTimeConfig:
      case sRTCDateConfig:
      case sRTCReport:
     	      xTaskNotify(RTC_Handle,(uint32_t)cmd,eSetValueWithOverwrite);
        	  break;

    }

}

int extract_command(command_t *cmd)
{
	uint8_t item;
	BaseType_t status;

	status = uxQueueMessagesWaiting(Input_Data_Queue);
	if(!status) return -1;
	uint8_t i = 0;

	do
	{
       status = xQueueReceive(Input_Data_Queue,&item,0);
       if(status == pdTRUE) cmd->payload[i++] = item;

	}while(item != '\n');
	cmd->payload[i-1] = '\0';
	cmd->len = i-1; // Save length without counting null char
	return 0;

}






