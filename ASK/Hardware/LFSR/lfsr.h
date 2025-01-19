#ifndef __LFSR_H
#define __LFSR_H

#include "main.h"


// 初始化 LFSR 状态
void LFSR_Init(uint16_t initial_state);

// 计算 LFSR 的下一个比特
uint8_t LFSR_NextBit(void);

// 获取当前 LFSR 的状态
uint16_t LFSR_GetState(void);

#endif
