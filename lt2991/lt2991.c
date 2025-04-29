#include "lt2991.h"

#include "FreeRTOS.h"
#include "task.h"

// 读取指定通道电压，单位V
float LT2991_ReadVoltage(uint8_t reg)
{
    uint8_t data = 0;
    uint8_t data1 = 0;
    uint16_t tem = 0;
    float voltage = 0.0;

    data = I2C_ReadByte(reg, XZC2991Adr, 1);
    data1 = I2C_ReadByte(reg + 1, XZC2991Adr, 1);

    tem = ((data & 0x00ff) << 8) | data1;
    tem = tem & 0x3fff;
    voltage = ((float)tem * Vol_SINGLE_LSB) / 1000;
    return voltage/1000; // 单位 V
}


void LT2991_Init(void)
{
    Soft_I2C_Init(); // 初始化 I2C
    printf("LT2991_Init\r\n");
    // I2C_WriteByte(0x01, 0xFB, LT2991_ADDR);
    // I2C_WriteByte(0x06, 0x89, LT2991_ADDR);
    // I2C_WriteByte(0x07, 0xA0, LT2991_ADDR);
    // I2C_WriteByte(0x08, 0x38, LT2991_ADDR); 
    // I2C_WriteByte(0x09, 0x10, LT2991_ADDR); 


    I2C_WriteByte(0x01, 0xFB, LT2991_ADDR);
    I2C_WriteByte(0x06, 0x80, LT2991_ADDR);
    I2C_WriteByte(0x07, 0xA0, LT2991_ADDR);
    I2C_WriteByte(0x08, 0x38, LT2991_ADDR); 
    I2C_WriteByte(0x09, 0x10, LT2991_ADDR); 

    delay_us(1000); // 等待稳定

    uint8_t reg  = I2C_ReadByte(0x01, LT2991_ADDR, 1); // 读取配置寄存器，确认配置成功
    printf("reg01: %02X\r\n", reg);
    reg = I2C_ReadByte(0x06, LT2991_ADDR, 1); // 读取配置寄存器，确认配置成功
    printf("reg06: %02X\r\n", reg);
    reg = I2C_ReadByte(0x07, LT2991_ADDR, 1);
    printf("reg07: %02X\r\n", reg);

}

float LT2991_ReadCurrent_V1V2(void)
{
    float v1, v2;

    v1 = LT2991_ReadVoltage(0x0A);
    v2 = LT2991_ReadVoltage(0x0C);

    printf("V1: %.3f V\tV2: %.3f V\r\n", v1, v2);



	// uint8_t sign = 0;
	// uint8_t data[1]={0};
	// uint8_t data1[1]={0};
	// uint16_t tem=0;
	// float vol_d=0.0;
	// uint8_t data2[1]={0};

    // data[0] = I2C_ReadByte(0x0C, LT2991_ADDR, 1); // 读取 V1 寄存器
    // data1[0] = I2C_ReadByte(0x0D, LT2991_ADDR, 1); // 读取 V2 寄存器

    // printf("0C: %02X\t 0D: %02X\r\n", data[0], data1[0]);
	// tem=((data[0]&0xffff)<<8)|data1[0];
	// if(tem & 0x4000)
	// {
	// 	tem=tem^0x3fff+1;
	// 	sign = 1;
	// }
	// else
	// {
	// }
	// tem=tem&0x3fff;
	// vol_d=((float)tem * 19.075)/1000;
	// // vol_d=((float)tem * 11.39)/1000;
	// if(sign==1)
	// {
	// 	vol_d=(float)(0.0)-vol_d;
	// }
	// printf("Current: %.3f mV\r\n",vol_d);


    
}

void LT2991_Read_V3V4V5(float *v3, float *v4, float *v5)
{
    *v3 = LT2991_ReadVoltage(0x0E) * 2;  // V3
    delay_us(1000); // 延时，避免连续读干扰
    *v4 = LT2991_ReadVoltage(0x10) * 4 / 3;  // V4
    delay_us(1000); // 延时，避免连续读干扰
    *v5 = LT2991_ReadVoltage(0x12);  // V5
}

float LT2991_ReadTemperature(uint8_t reg)
{
    float temperature = 0.0;
    uint16_t tem, tem1 = 0;
    uint8_t temdata = 0;
    uint8_t temdata1 = 0;

    temdata = I2C_ReadByte(reg, XZC2991Adr, 1);
    temdata1 = I2C_ReadByte(reg + 1, XZC2991Adr, 1);

    tem = ((temdata & 0x00ff) << 8) | temdata1;
    tem1 = tem & 0x1fff;

    temperature = (float)(tem1)*T_LSB;
    return temperature; // 单位 °C
}

void LT2991_Read_Temperatures(float *temp7, float *temp8)
{
    *temp7 = LT2991_ReadTemperature(0x16);  // 通道7
    *temp8 = LT2991_ReadTemperature(0x18);  // 通道8
}


void LT2991_ReadAll()
{
    float v3, v4, v5;
    float temp7, temp8;
    float current = LT2991_ReadCurrent_V1V2();
    vTaskDelay(100);
    LT2991_Read_V3V4V5(&v3, &v4, &v5);
    vTaskDelay(100);
    LT2991_Read_Temperatures(&temp7, &temp8);


    // printf("%x\r\n", LT2991_ReadVoltage(0x0C));
    // printf("V1-V2 Current: %.3f A\r\n", current);
    printf("V3: %.3f V\tV4: %.3f V\tV5: %.3f V\r\n", v3, v4, v5);
    printf("Temp7: %.2f ℃\tTemp8: %.2f \r\n", temp7, temp8);
}

