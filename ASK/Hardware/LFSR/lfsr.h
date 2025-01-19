#ifndef __LFSR_H
#define __LFSR_H

#include "main.h"


// ��ʼ�� LFSR ״̬
void LFSR_Init(uint16_t initial_state);

// ���� LFSR ����һ������
uint8_t LFSR_NextBit(void);

// ��ȡ��ǰ LFSR ��״̬
uint16_t LFSR_GetState(void);

#endif
