#include "bsp_i2c_slave.h"
#include "ipmb_protocol.h"
#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define RX_BUF_LEN 32
#define TX_BUF_LEN 32

static uint8_t i2c1_rx_buf[RX_BUF_LEN];
static uint8_t i2c1_tx_buf[TX_BUF_LEN];
static uint8_t i2c2_rx_buf[RX_BUF_LEN];
static uint8_t i2c2_tx_buf[TX_BUF_LEN];


void I2C_Slave_Init(I2C_TypeDef* I2Cx, uint8_t address) {
    printf("I2C_Slave_Init\r\n");
    I2C_InitTypeDef i2c;
    GPIO_InitTypeDef gpio;
    NVIC_InitTypeDef nvic;

    if (I2Cx == I2C1) RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    if (I2Cx == I2C2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    gpio.GPIO_Mode = GPIO_Mode_AF_OD;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Pin = (I2Cx == I2C1) ? (GPIO_Pin_6 | GPIO_Pin_7) : (GPIO_Pin_10 | GPIO_Pin_11);
    GPIO_Init(GPIOB, &gpio);

    I2C_Cmd(I2Cx, DISABLE);
    I2C_DeInit(I2Cx);
    i2c.I2C_Mode = I2C_Mode_I2C;
    i2c.I2C_DutyCycle = I2C_DutyCycle_2;
    i2c.I2C_OwnAddress1 = address;
    i2c.I2C_Ack = I2C_Ack_Enable;
    i2c.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    i2c.I2C_ClockSpeed = 100000;
    I2C_Init(I2Cx, &i2c);

    I2C_ITConfig(I2Cx, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
    I2C_ITConfig(I2Cx, I2C_IT_ERR, ENABLE); 
    I2C_Cmd(I2Cx, ENABLE);

    if (I2Cx == I2C1) {
        nvic.NVIC_IRQChannel = I2C1_EV_IRQn;
    } else {
        nvic.NVIC_IRQChannel = I2C2_EV_IRQn;
    }
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}


void I2C1_EV_IRQHandler(void) {
    static uint8_t idx = 0;
    if (I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED)) {
        idx = 0;
    } else if (I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_BYTE_RECEIVED)) {
        i2c1_rx_buf[idx++] = I2C_ReceiveData(I2C1);
    } else if (I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_STOP_DETECTED)) {
        uint8_t len = 0;
        IPMB_Process_Command(i2c1_rx_buf, i2c1_tx_buf, &len);
        idx = 0;
        I2C_ClearFlag(I2C1, I2C_FLAG_STOPF);
    } else if (I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED) ||
               I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_BYTE_TRANSMITTING)) {
        I2C_SendData(I2C1, i2c1_tx_buf[idx++]);
    }
}

void I2C2_EV_IRQHandler(void) {
    static uint8_t idx = 0;
    if (I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED)) {
        idx = 0;
    } else if (I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_BYTE_RECEIVED)) {
        i2c2_rx_buf[idx++] = I2C_ReceiveData(I2C2);
    } else if (I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_STOP_DETECTED)) {
        uint8_t len = 0;
        IPMB_Process_Command(i2c2_rx_buf, i2c2_tx_buf, &len);
        idx = 0;
        I2C_ClearFlag(I2C2, I2C_FLAG_STOPF);
    } else if (I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED) ||
               I2C_CheckEvent(I2C2, I2C_EVENT_SLAVE_BYTE_TRANSMITTING)) {
        I2C_SendData(I2C2, i2c2_tx_buf[idx++]);
    }
}

int ipmb_task(void) {
    GPIO_Key_Init();
    // uint8_t i2c_address = GPIO_Read_Keys(); // 读取按键状态
    uint8_t i2c_address = 0xa0; // 读取按键状态
    // I2C_Slave_Init(I2C1, i2c_address);
    // I2C_Slave_Init(I2C2, i2c_address | 0x02); // I2C2 地址加1

    while (1) {
        // 所有处理在中断里
        printf("I2C1 RX: ");
        for (int i = 0; i < RX_BUF_LEN; i++) {
            printf("%02X ", i2c1_rx_buf[i]);
        }
		vTaskDelay(2000);
    }
}


