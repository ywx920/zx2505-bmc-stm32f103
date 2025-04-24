#include "soft_i2c.h"
// #include "delay.h"  // 延时函数：delay_us()



void delay_us(uint32_t us)
{
    for (int i = 0; i < us*100; i++)
    {

    }
}


static void SDA_OUT(void)
{
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = I2C_SDA_PIN;
    gpio.GPIO_Mode = GPIO_Mode_Out_OD;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_SDA_GPIO, &gpio);
}

static void SDA_IN(void)
{
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = I2C_SDA_PIN;
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_SDA_GPIO, &gpio);
}

void Soft_I2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    SDA_OUT();

    GPIO_SetBits(I2C_SDA_GPIO, I2C_SDA_PIN);
    GPIO_SetBits(I2C_SCL_GPIO, I2C_SCL_PIN);
}

void I2C_Start(void)
{
    // printf("I2C_Start\r\n");
    SDA_OUT();
    GPIO_SetBits(I2C_SDA_GPIO, I2C_SDA_PIN);
    GPIO_SetBits(I2C_SCL_GPIO, I2C_SCL_PIN);
    delay_us(5);
    GPIO_ResetBits(I2C_SDA_GPIO, I2C_SDA_PIN);
    delay_us(5);
    GPIO_ResetBits(I2C_SCL_GPIO, I2C_SCL_PIN);
}

void I2C_Stop(void)
{
    // printf("I2C_Stop\r\n");
    SDA_OUT();
    GPIO_ResetBits(I2C_SDA_GPIO, I2C_SDA_PIN);
    GPIO_SetBits(I2C_SCL_GPIO, I2C_SCL_PIN);
    delay_us(5);
    GPIO_SetBits(I2C_SDA_GPIO, I2C_SDA_PIN);
    delay_us(5);
}

void I2C_SendByte(uint8_t byte)
{
    SDA_OUT();
    for (int i = 0; i < 8; i++)
    {
        if (byte & 0x80)
            GPIO_SetBits(I2C_SDA_GPIO, I2C_SDA_PIN);
        else
            GPIO_ResetBits(I2C_SDA_GPIO, I2C_SDA_PIN);

        GPIO_SetBits(I2C_SCL_GPIO, I2C_SCL_PIN);
        delay_us(5);
        GPIO_ResetBits(I2C_SCL_GPIO, I2C_SCL_PIN);
        delay_us(5);
        byte <<= 1;
    }
}

uint8_t I2C_ReadByte(uint8_t ack)
{
    uint8_t i, byte = 0;

    SDA_IN();
    for (i = 0; i < 8; i++)
    {
        GPIO_SetBits(I2C_SCL_GPIO, I2C_SCL_PIN);
        delay_us(5);
        byte <<= 1;
        if (GPIO_ReadInputDataBit(I2C_SDA_GPIO, I2C_SDA_PIN))
            byte++;
        GPIO_ResetBits(I2C_SCL_GPIO, I2C_SCL_PIN);
        delay_us(5);
    }
    I2C_SendACK(ack);
    return byte;
}

uint8_t I2C_WaitACK(void)
{
    uint8_t timeout = 100;

    SDA_IN();
    GPIO_SetBits(I2C_SDA_GPIO, I2C_SDA_PIN);
    delay_us(1);
    GPIO_SetBits(I2C_SCL_GPIO, I2C_SCL_PIN);
    delay_us(1);

    while (GPIO_ReadInputDataBit(I2C_SDA_GPIO, I2C_SDA_PIN))
    {
        timeout--;
        if (timeout == 0)
        {
            I2C_Stop();
            return 1;
        }
    }

    GPIO_ResetBits(I2C_SCL_GPIO, I2C_SCL_PIN);
    return 0;
}

void I2C_SendACK(uint8_t ack)
{
    SDA_OUT();
    if (ack)
        GPIO_SetBits(I2C_SDA_GPIO, I2C_SDA_PIN);
    else
        GPIO_ResetBits(I2C_SDA_GPIO, I2C_SDA_PIN);

    GPIO_SetBits(I2C_SCL_GPIO, I2C_SCL_PIN);
    delay_us(2);
    GPIO_ResetBits(I2C_SCL_GPIO, I2C_SCL_PIN);
    delay_us(2);
}
