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

	// while(1)
	// {
	// // 	// LED2(ON);
	// // 	// vTaskDelay(1000);
	// // 	// LED2(OFF);
	// // 	vTaskDelay(1000);
	// // 	// LED1(OFF);
	// 	// printf("LED2 Task\r\n");
	// 	vTaskDelay(200);
	// }


    while (1)
    {
        // for (uint8_t i = 0; i < LT2991_CHANNEL_COUNT; i++)
        // {
        //     lt2991_voltage_cache[i] = LT2991_ReadVoltage(i);
			
        // }
		// printf("[LT2991] 电压：\r\n");
		// for (int i = 0; i < LT2991_CHANNEL_COUNT; i++)
		// {
			// 	printf("v%d : %.3f \r\n", i, lt2991_voltage_cache[i]);
			// }
		// printf("ID：%02x\r\n", LT2991_ReadChipID());
        LT2991_ReadAllVoltages(voltages);
        // float current = LT2991_CalcCurrent_V1V2();

        printf("V1: %.3f V\tV2: %.3f V\t \r\n", voltages[0], voltages[1]);
        printf("V3: %.3f V\tV4: %.3f V\tV5: %.3f V\r\n", voltages[2], voltages[3], voltages[4]);
        printf("V7: %.3f V\tV8: %.3f V\r\n", voltages[5], voltages[6]);
        printf("---------------\r\n");

		// ZGW2991_PrintAll();
        vTaskDelay(1000);  // 每秒更新一次
    }

}


// void LT2991_Task(void *task)
// {
//     LT2991_Init();

//     while (1)
//     {
//         for (uint8_t i = 0; i < LT2991_CHANNEL_COUNT; i++)
//         {
//             lt2991_voltage_cache[i] = LT2991_ReadVoltage(i);
//         }

//         vTaskDelay(pdMS_TO_TICKS(1000));  // 每秒更新一次
//     }
// }


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
/******************* (C) COPYRIGHT 2017 *****END OF FILE************/


// void vApplicationIdleHook(void)
// {
//     static TickType_t lastPrintTick = 0;

//     if ((xTaskGetTickCount() - lastPrintTick) > pdMS_TO_TICKS(1000))
//     {
//         lastPrintTick = xTaskGetTickCount();
//         printf("[LT2991] 电压：");
//         for (int i = 0; i < LT2991_CHANNEL_COUNT; i++)
//         {
//             printf("%.3f ", lt2991_voltage_cache[i]);
//         }
//         printf("V\r\n");
//     }
// }