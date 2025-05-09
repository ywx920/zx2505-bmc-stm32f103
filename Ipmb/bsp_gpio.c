#include "bsp_gpio.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


void GPIO_Key_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // GPIO_InitTypeDef gpio;
    // gpio.GPIO_Mode = GPIO_Mode_IPU;
    // gpio.GPIO_Speed = GPIO_Speed_50MHz;
    // for (int i = 0; i <= 6; ++i) {
    //     gpio.GPIO_Pin = (1 << i);
    //     GPIO_Init(GPIOA, &gpio);
    // }

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);


}

uint8_t GPIO_Read_Keys(void) {
    // uint8_t keys = 0;
    // for (int i = 0; i <= 6; ++i) {
    //     if (!GPIO_ReadInputDataBit(GPIOA, 1 << i)) {
    //         keys |= (1 << i);
    //     }
    // }
    // printf("11keys: %02X\r\n", keys); // 调试输出
    // // 这里可以添加其他处理逻辑，比如发送到上位机等
    // return keys;
    // GPIOA1~A5: GA0~GA4
    // GPIOA6: GAP
    uint8_t ga_value = 0;
    uint8_t gap = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);  // GAP

    if (gap == 0) {
        // printf("GAP is GND, IPMB addressing disabled.\r\n");
        return 0;
    }

    // 读取 GA[4:0]
    ga_value |= (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) << 4);  // GA4
    ga_value |= (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) << 3);  // GA3
    ga_value |= (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) << 2);  // GA2
    ga_value |= (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) << 1);  // GA1
    ga_value |= (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) << 0);  // GA0

    uint8_t hardware_address = 0x40 | ga_value;
    uint8_t ipmb_address = hardware_address << 1;

    // printf("GA value: 0x%02X\r\n", ga_value);
    // printf("Hardware Address: 0x%02X\r\n", hardware_address);
    // printf("IPMB Address: 0x%02X\r\n", ipmb_address);

    return ipmb_address;

}
