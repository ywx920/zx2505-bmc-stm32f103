#ifndef __LT2991_H
#define __LT2991_H

#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "soft_i2c.h"

void LT2991_WriteReg(uint8_t reg, uint8_t val);


void LT2991_Init(void);

uint16_t LT2991_ReadRaw(uint8_t reg);
void LT2991_ReadAllVoltages(float *v_arr);

// float LT2991_ReadVoltage(uint8_t msb_reg, uint8_t lsb_reg);
void LT2991_ReadAllVoltages(float *v_arr);

uint8_t LT2991_ReadChipID(void);

float LT2991_CalcCurrent_V1V2(void);  // 差分测量 V1~V2，结合0.01Ω采样电阻

#define LT2991_CHANNEL_COUNT 8

extern float lt2991_voltage_cache[LT2991_CHANNEL_COUNT];  // 缓存所有通道电压

void ZGW2991_PrintAll(void);

#endif
