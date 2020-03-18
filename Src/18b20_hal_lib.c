/**************************************************************************//**
 * @file     18b20_hal_lib.c
 * @brief    DS18B20 / DS1820 Library 
 * @version  V1.00
 * @date     22. July 2016
 ******************************************************************************/

/***************************************
     DS18B20 and DS1820 HAL Library
     for USART 1 Half-Duplex mode
***************************************/

#include "18b20_hal_lib.h"
#include "stm32l1xx_hal.h"

extern UART_HandleTypeDef huart3;

uint8_t receiveDataUART[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0,0x00,0x00};

void ResetChip18b20(void)
{
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;                  
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_HalfDuplex_Init(&huart3);

  char sendDataUART = 0xF0;
  HAL_UART_Transmit(&huart3,(uint8_t*)&sendDataUART, 1, 100);                   // Reset chip
  
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_HalfDuplex_Init(&huart3);
}
  

void WriteByte18b20(unsigned char sendDataUART)
{ 
  uint8_t i = 0;
  for (i = 0; i < 8; i++)                                                       // Sending 1 byte of data
  {
    if ((sendDataUART & (1 << i)) == 1 << i)
    {
      uint8_t transmitValue = 0xFF;
      HAL_UART_Transmit(&huart3,(uint8_t*)&transmitValue, 1, 100);
    }
    else 
    {
      uint8_t transmitValue = 0x00;
      HAL_UART_Transmit(&huart3,(uint8_t*)&transmitValue, 1, 100);
    }
  }
}


void Configuration18b20(void)
{
  ResetChip18b20();
  WriteByte18b20 (0xCC);
  HAL_Delay(1);
  WriteByte18b20 (0x4E);
  HAL_Delay(1);
  WriteByte18b20 (0x4B);
  HAL_Delay(1);
  WriteByte18b20 (0x46);
  HAL_Delay(1);
  WriteByte18b20 (0x5F);
  HAL_Delay(10);
}


void TemperatureSense18b20(void)
{
  ResetChip18b20();
  WriteByte18b20 (0xCC);
  HAL_Delay(1);
  WriteByte18b20 (0x44);                                                        // Convert T
  /*
  NOTE: It's necessary  to make a delay 750+ uS to wait until conversation done
  */
}


void ReadTemperature18b20(void)
{ 
  ResetChip18b20();
  WriteByte18b20 (0xCC);
  HAL_Delay(1);
  WriteByte18b20 (0xBE);
  HAL_Delay(2);
  
  uint8_t i = 0;
  char FF = 0xFF;
  for (i=0; i<16; i++)                                                          // Receiving 2 byte of data
  {    
    HAL_UART_Transmit(&huart3,(uint8_t*)&FF, 1, 100);        
    HAL_UART_Receive(&huart3,(uint8_t*)&receiveDataUART[i], 1, 100);  
  }  
}

uint16_t Data_to_2Bytes (uint8_t *ow_bits) 
{
  uint16_t ow_byte, i;
  ow_byte = 0;
  for (i = 0; i < 16; i++) 
  {
    ow_byte = ow_byte >> 1;
    if (*ow_bits == 0xFF) 
      ow_byte |= 0x8000;
    ow_bits++;
  }
  return ow_byte;
}