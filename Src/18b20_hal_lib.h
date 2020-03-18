/**************************************************************************//**
 * @file     18b20_hal_lib.h
 * @brief    DS18B20 / DS1820 Library header
 * @version  V1.00
 * @date     22. July 2016
 ******************************************************************************/

#ifndef 18B20_HAL_LIB_H
#define 18B20_HAL_LIB_H

#endif

#include "stm32l1xx_hal.h"


void ResetChip18b20(void);

void WriteByte18b20(unsigned char sendDataUART);

void Configuration18b20(void);

void TemperatureSense18b20(void);

void ReadTemperature18b20(void);

uint16_t Data_to_2Bytes(uint8_t *ow_bits);
