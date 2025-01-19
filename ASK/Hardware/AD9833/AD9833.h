#ifndef _AD9833_H_
#define _AD9833_H_

#include "main.h"

// 波形类型定义
#define TRI_WAVE  0  // 三角波
#define SIN_WAVE  1  // 正弦波
#define SQU_WAVE  2  // 方波

// MCP41010 引脚定义
#define MCP41010_CS_PORT GPIOB
#define MCP41010_CS_PIN  GPIO_PIN_12

#define MCP41010_SCK_PORT GPIOB
#define MCP41010_SCK_PIN  GPIO_PIN_14

#define MCP41010_SI_PORT GPIOB
#define MCP41010_SI_PIN  GPIO_PIN_13

// 函数声明
void AD9833_Init_GPIO(void);
void AD9833_Write(uint16_t TxData);
void AD9833_AmpSet(uint8_t amp);
void AD9833_WaveSeting(double frequence, uint16_t frequence_SFR, uint16_t WaveMode, uint16_t Phase);
void MCP41010_SetResistance(uint8_t value);

#endif
