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
#include "lt2991.h"
float voltages[7];  // V1~V5, V7, V8


// static uint8_t  fac_us = 0;

// void delay_init(void)
// {
//     SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); // SysTick使用HCLK/8
//     fac_us = SystemCoreClock / 8000000; // 72MHz / 8 = 9
// }

// void delay_us(uint32_t us)
// {
//     uint32_t temp;
//     SysTick->LOAD = us * fac_us;           // 设置重装载值
//     SysTick->VAL = 0x00;                   // 清空计数器
//     SysTick->CTRL = SysTick_CTRL_ENABLE_Msk; // 启动定时器

//     do {
//         temp = SysTick->CTRL;
//     } while ((temp & SysTick_CTRL_ENABLE_Msk) && !(temp & SysTick_CTRL_COUNTFLAG_Msk));

//     SysTick->CTRL = 0;  // 关闭计时器
//     SysTick->VAL = 0;   // 清空计数器
// }

// void delay_ms(uint32_t ms)
// {
//     while (ms--)
//         delay_us(1000);
// }


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
    while (1)
    {
		// LED2(ON);
		// delay_us(1);
		// LED2(OFF);
		// delay_us(1);
		LT2991_ReadAll();
        vTaskDelay(2000);  // 每秒更新一次
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
	LT2991_Init();

	xTaskCreate(LED1_Task,		//任务函数指针
							"LED1_Task",		//任务名称
							40,						//堆栈深度（字）
							NULL,					//任务参数为空
							1,						//任务优先级
							NULL);				//任务句柄为空
	
	xTaskCreate(LED2_Task,		//任务函数指针
							"LED2_Task",		//任务名称
							128,						//堆栈深度（字）
							NULL,					//任务参数为空
							2,						//任务优先级
							NULL);				//任务句柄为空
	// xTaskCreate(LT2991_Task,		//任务函数指针
	// 						"LT2991_Task",		//任务名称
	// 						128,						//堆栈深度（字）
	// 						NULL,					//任务参数为空
	// 						1,						//任务优先级
	// 						NULL);				//任务句柄为空
	vTaskStartScheduler();
	while(1)
	{ 

	}
}

