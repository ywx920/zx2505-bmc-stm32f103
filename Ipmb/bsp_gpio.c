#include "bsp_gpio.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void GPIO_Key_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Mode = GPIO_Mode_IPU;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    for (int i = 0; i <= 6; ++i) {
        gpio.GPIO_Pin = (1 << i);
        GPIO_Init(GPIOA, &gpio);
    }
}

uint8_t GPIO_Read_Keys(void) {
    uint8_t keys = 0;
    for (int i = 0; i <= 6; ++i) {
        if (!GPIO_ReadInputDataBit(GPIOA, 1 << i)) {
            keys |= (1 << i);
        }
    }
    return keys;
}
