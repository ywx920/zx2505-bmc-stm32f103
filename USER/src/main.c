/********************        (C) COPYRIGHT 2017        **************************
 * 文件名  ：main.c
 * 描述    ：基于FreeRTOS的LED的测试程序  
 * 实验平台：STM32F103RBT6
 * 库版本  ：ST3.5.0
 * 编写日期：2015-03-30
 * 修改日期：2017-03-24
 * 作者    ：
 * 2017-03-24
 * (1)运行LED测试正常
 * 2017-03-24
 *（1）加入FreeRTOS的支持
**********************************************************************************/

#include "FreeRTOS.h"
#include "task.h"

#include "led.h"
#include "shell_port.h"
#include "usart.h"
#include "shell.h"



/* FreeRTOS task1 */
void LED1_Task(void *task)
{
	while(1)
	{
		LED1(ON);
		vTaskDelay(200);
		LED1(OFF);
		vTaskDelay(200);
	}
}

/* FreeRTOS task2 */
void LED2_Task(void *task)
{
	while(1)
	{
		// LED2(ON);
		// vTaskDelay(1000);
		// LED2(OFF);
		vTaskDelay(1000);
		// LED1(OFF);
		// printf("LED2 Task\r\n");
		// vTaskDelay(200);
	}
}


/* 
 * 函数名：main
 * 描述  : "主机"的主函数
 * 输入  ：无
 * 输出  : 无
 */
 
int main(void)
{	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
 	LED_GPIO_Config();	  						/* 运行LED初始化 */
	USART_Config();						/* 运行USART初始化 */
	// Usart_SendString(DEBUG_USARTx, "Hello World!\r\n");	/* 运行USART发送字符串 */	
	userShellInit();
	xTaskCreate(LED1_Task,		//任务函数指针
							"LED1_Task",		//任务名称
							40,						//堆栈深度（字）
							NULL,					//任务参数为空
							1,						//任务优先级
							NULL);				//任务句柄为空
	
	xTaskCreate(LED2_Task,		//任务函数指针
							"LED2_Task",		//任务名称
							40,						//堆栈深度（字）
							NULL,					//任务参数为空
							1,						//任务优先级
							NULL);				//任务句柄为空
							
	vTaskStartScheduler();
	while(1)
	{ 
		
	}
}
/******************* (C) COPYRIGHT 2017 *****END OF FILE************/


