#ifndef __SOFT_I2C_H
#define __SOFT_I2C_H

#include "stm32f10x.h"

#define I2C_SCL_GPIO    GPIOC
#define I2C_SCL_PIN     GPIO_Pin_7
#define I2C_SDA_GPIO    GPIOC
#define I2C_SDA_PIN     GPIO_Pin_6

void delay_us(uint32_t us);
void Soft_I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendACK(uint8_t ack);
uint8_t I2C_WaitACK(void);
void I2C_SendByte(uint8_t byte);
uint8_t I2C_ReadByte(uint8_t ack);


#endif
