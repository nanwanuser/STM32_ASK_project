#include "lfsr.h"

// LFSR 寄存器
static uint16_t LFSR;

// 初始化 LFSR 状态
void LFSR_Init(uint16_t initial_state)
{
    LFSR = initial_state; // 设置初始状态
}

// 计算 LFSR 的下一个比特
uint8_t LFSR_NextBit(void)
{
    // 取第14位和第15位
    uint8_t bit14 = (LFSR >> 13) & 0x01;
    uint8_t bit15 = (LFSR >> 14) & 0x01;

    // XOR 操作计算反馈位
    uint8_t new_bit = bit14 ^ bit15;

    // 移位，将新生成的 bit 放入最低位
    LFSR = (LFSR << 1) | new_bit;

    // 返回 LFSR 的最高位（当前输出）
    return new_bit;
}

// 获取当前 LFSR 的状态
uint16_t LFSR_GetState(void)
{
    return LFSR;
}
