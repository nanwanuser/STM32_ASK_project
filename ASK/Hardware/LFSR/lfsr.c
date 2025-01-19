#include "lfsr.h"

// LFSR �Ĵ���
static uint16_t LFSR;

// ��ʼ�� LFSR ״̬
void LFSR_Init(uint16_t initial_state)
{
    LFSR = initial_state; // ���ó�ʼ״̬
}

// ���� LFSR ����һ������
uint8_t LFSR_NextBit(void)
{
    // ȡ��14λ�͵�15λ
    uint8_t bit14 = (LFSR >> 13) & 0x01;
    uint8_t bit15 = (LFSR >> 14) & 0x01;

    // XOR �������㷴��λ
    uint8_t new_bit = bit14 ^ bit15;

    // ��λ���������ɵ� bit �������λ
    LFSR = (LFSR << 1) | new_bit;

    // ���� LFSR �����λ����ǰ�����
    return new_bit;
}

// ��ȡ��ǰ LFSR ��״̬
uint16_t LFSR_GetState(void)
{
    return LFSR;
}
