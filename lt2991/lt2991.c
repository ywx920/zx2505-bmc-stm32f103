#include "lt2991.h"

#include "FreeRTOS.h"
#include "task.h"



#define LT2991_ADDR    0x98

float lt2991_voltage_cache[LT2991_CHANNEL_COUNT] = {0};


uint8_t I2C_ReadReg(uint8_t reg)
{
    uint8_t val = 0;

    I2C_Start();
    I2C_SendByte(LT2991_ADDR);
    I2C_WaitACK();

    I2C_SendByte(reg);
    I2C_WaitACK();

    I2C_Start();
    I2C_SendByte(LT2991_ADDR | 0x01);
    I2C_WaitACK();

    val = I2C_ReadByte(0);
    I2C_Stop();

    return val;
}


void LT2991_WriteReg(uint8_t reg, uint8_t val)
{
    I2C_Start();
    I2C_SendByte(LT2991_ADDR);
    I2C_WaitACK();

    I2C_SendByte(reg);
    I2C_WaitACK();

    I2C_SendByte(val);
    // I2C_WaitACK();
    if (I2C_WaitACK() == 0)
        printf("LT2991 ACK success\r\n");
    else
        printf("LT2991 ACK failed\r\n");
    I2C_Stop();
}

uint16_t LT2991_ReadRaw(uint8_t reg)
{
    uint8_t msb, lsb;

    I2C_Start();
    I2C_SendByte(LT2991_ADDR);
    I2C_WaitACK();

    I2C_SendByte(reg);
    I2C_WaitACK();

    I2C_Start();
    I2C_SendByte((LT2991_ADDR) | 0x01);
    I2C_WaitACK();

    msb = I2C_ReadByte(1);
    lsb = I2C_ReadByte(0);
    // printf("msb: %02X lsb: %02X\r\n", msb, lsb);
    I2C_Stop();

    return ((uint16_t)msb << 8) | lsb;
}

// float LT2991_ReadVoltage(uint8_t msb_reg, uint8_t lsb_reg)
// {
//     uint16_t raw = (LT2991_ReadReg(msb_reg) << 8) | LT2991_ReadReg(lsb_reg);
//     raw >>= 4;  // 取高12位
//     float voltage = raw * 0.00030518f; // 单位为 V
//     return voltage;
// }
float ZGW2991_ReadVoltage(uint8_t reg_msb, uint8_t reg_lsb)
{
    uint8_t msb = I2C_ReadReg(reg_msb);
    uint8_t lsb = I2C_ReadReg(reg_lsb);

    uint16_t raw = ((msb & 0x3F) << 8) | lsb; // 14位有效值
    return raw * 0.00030518f;
}

float ZGW2991_ReadCurrent(uint8_t reg_msb, uint8_t reg_lsb, float r_sense)
{
    uint8_t msb = I2C_ReadReg(reg_msb);
    uint8_t lsb = I2C_ReadReg(reg_lsb);

    int16_t raw = ((msb & 0x3F) << 8) | lsb;
    if (msb & 0x40) raw = -(~raw + 1);  // 补码符号扩展

    return (raw * 19.075f / 1e6f) / r_sense;
}


// 读取指定通道电压，单位V
float LT2991_ReadVoltage(uint8_t reg)
{
    uint16_t val = LT2991_ReadRaw(reg);
    return (val * 2.5f) / 4096.0f;
}


// 批量读取你关心的电压通道
void LT2991_ReadAllVoltages(float *v_arr)
{
    v_arr[0] = LT2991_ReadVoltage(0x0A); // V1
    v_arr[1] = LT2991_ReadVoltage(0x0C); // V2
    v_arr[2] = LT2991_ReadVoltage(0x0E); // V3
    v_arr[3] = LT2991_ReadVoltage(0x10); // V4
    v_arr[4] = LT2991_ReadVoltage(0x12); // V5
    v_arr[5] = LT2991_ReadVoltage(0x14); // V7
    v_arr[6] = LT2991_ReadVoltage(0x16); // V8
}

// void LT2991_ReadAllVoltages(float *v_arr) 
// {
//     v_arr[0] = LT2991_ReadVoltage(0x0A, 0x0B);
//     v_arr[1] = LT2991_ReadVoltage(0x0C, 0x0D);
//     v_arr[2] = LT2991_ReadVoltage(0x0E, 0x0F);
//     v_arr[3] = LT2991_ReadVoltage(0x10, 0x11);
//     v_arr[4] = LT2991_ReadVoltage(0x12, 0x13);
//     v_arr[5] = LT2991_ReadVoltage(0x16, 0x17);
//     v_arr[6] = LT2991_ReadVoltage(0x18, 0x19);

//     float I = (V1 - V2) / 0.01f;  // 假设电流采样电阻为 0.01Ω

//     printf("V1: %.3f V\tV2: %.3f V\tI: %.3f A\r\n", V1, V2, I);
//     printf("V3: %.3f V\tV4: %.3f V\tV5: %.3f V\r\n", V3, V4, V5);
//     printf("V7: %.3f V\tV8: %.3f V\r\n", V7, V8);
// }


// 计算电流（基于 V1-V2 差分，0.01Ω 电阻）
float LT2991_CalcCurrent_V1V2(void)
{
    float v1 = LT2991_ReadVoltage(0x0A);
    float v2 = LT2991_ReadVoltage(0x0C);
    return (v1 - v2) / 0.01f; // 电流单位 A
}


void LT2991_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // 开漏输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    Soft_I2C_Init();
    printf("LT2991_Init\r\n");
    LT2991_WriteReg(0x01, 0xFF); // 启用所有通道
    LT2991_WriteReg(0x06, 0x00); // V1~V4：单端电压模式
    LT2991_WriteReg(0x07, 0x00); // V5~V8：单端电压模式
    LT2991_WriteReg(0x08, 0x10); // 启动重复采集（bit4=1）

}



