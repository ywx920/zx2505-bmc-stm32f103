#ifndef BSP_I2C_SLAVE_H
#define BSP_I2C_SLAVE_H

#include "stm32f10x.h"

void I2C_Slave_Init(void);
int ipmb_task(void);

#endif