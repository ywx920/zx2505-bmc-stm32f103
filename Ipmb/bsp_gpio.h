#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include <stdint.h>

void GPIO_Key_Init(void);
uint8_t GPIO_Read_Keys(void);

#endif
