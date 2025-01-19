#include "AD9833.h"

// 定义 GPIO 操作宏
#define FSYNC_0() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET)
#define FSYNC_1() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET)

#define SCK_0() HAL_GPIO_WritePin(MCP41010_SCK_PORT, MCP41010_SCK_PIN, GPIO_PIN_RESET)
#define SCK_1() HAL_GPIO_WritePin(MCP41010_SCK_PORT, MCP41010_SCK_PIN, GPIO_PIN_SET)

#define SI_0() HAL_GPIO_WritePin(MCP41010_SI_PORT, MCP41010_SI_PIN, GPIO_PIN_RESET)
#define SI_1() HAL_GPIO_WritePin(MCP41010_SI_PORT, MCP41010_SI_PIN, GPIO_PIN_SET)

#define MCP41010_CS_LOW() HAL_GPIO_WritePin(MCP41010_CS_PORT, MCP41010_CS_PIN, GPIO_PIN_RESET)
#define MCP41010_CS_HIGH() HAL_GPIO_WritePin(MCP41010_CS_PORT, MCP41010_CS_PIN, GPIO_PIN_SET)

// 初始化 GPIO
void AD9833_Init_GPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 启用 GPIO 时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // 配置 AD9833 和 MCP41010 的引脚为推挽输出
    GPIO_InitStruct.Pin = GPIO_PIN_15 | MCP41010_SCK_PIN | MCP41010_SI_PIN | MCP41010_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 默认拉高 CS 和 FSYNC 引脚
    FSYNC_1();
    MCP41010_CS_HIGH();
}

// 延时函数
static void AD9833_Delay(void)
{
    for (volatile uint16_t i = 0; i < 10; i++); // 简单延时
}

// 向 AD9833 写数据
void AD9833_Write(uint16_t TxData)
{
    uint8_t i;

    SCK_1();
    FSYNC_1();
    FSYNC_0();
    for (i = 0; i < 16; i++)
    {
        if (TxData & 0x8000)
            SI_1();
        else
            SI_0();

        AD9833_Delay();
        SCK_0();
        AD9833_Delay();
        SCK_1();

        TxData <<= 1;
    }
    FSYNC_1();
}

// 设置幅值 (通过 MCP41010)
void AD9833_AmpSet(uint8_t amp)
{
    MCP41010_SetResistance(amp);
}

// 配置波形输出
void AD9833_WaveSeting(double Freq, uint16_t Freq_SFR, uint16_t WaveMode, uint16_t Phase)
{
    uint32_t frequence_mid, frequence_hex;
    uint16_t frequence_LSB, frequence_MSB, Phs_data;
    double frequence_DATA;

    // 计算频率值
    frequence_mid = 268435456 / 25; // 25MHz时钟
    frequence_DATA = Freq / 1000000.0;
    frequence_DATA *= frequence_mid;
    frequence_hex = (uint32_t)frequence_DATA;

    frequence_LSB = frequence_hex & 0x3FFF;
    frequence_MSB = (frequence_hex >> 14) & 0x3FFF;
    Phs_data = Phase | 0xC000;

    AD9833_Write(0x0100); // 复位AD9833
    AD9833_Write(0x2100); // B28位和RESET位为1

    if (Freq_SFR == 0)
    {
        frequence_LSB |= 0x4000;
        frequence_MSB |= 0x4000;
        AD9833_Write(frequence_LSB);
        AD9833_Write(frequence_MSB);
        AD9833_Write(Phs_data);
    }
    else if (Freq_SFR == 1)
    {
        frequence_LSB |= 0x8000;
        frequence_MSB |= 0x8000;
        AD9833_Write(frequence_LSB);
        AD9833_Write(frequence_MSB);
        AD9833_Write(Phs_data);
    }

    if (WaveMode == TRI_WAVE)
        AD9833_Write(0x2002);
    else if (WaveMode == SQU_WAVE)
        AD9833_Write(0x2028);
    else if (WaveMode == SIN_WAVE)
        AD9833_Write(0x2000);
}

// 设置 MCP41010 电阻值 (调整幅值)
void MCP41010_SetResistance(uint8_t value)
{
    uint8_t i;

    // MCP41010 使用16位协议：命令字节+数据字节
    uint8_t command = 0x11; // 写入命令

    MCP41010_CS_LOW(); // 拉低CS引脚，开始通信

    // 发送命令字节
    for (i = 0; i < 8; i++)
    {
        if (command & 0x80)
            SI_1();
        else
            SI_0();

        AD9833_Delay();
        SCK_0();
        AD9833_Delay();
        SCK_1();

        command <<= 1;
    }

    // 发送数据字节
    for (i = 0; i < 8; i++)
    {
        if (value & 0x80)
            SI_1();
        else
            SI_0();

        AD9833_Delay();
        SCK_0();
        AD9833_Delay();
        SCK_1();

        value <<= 1;
    }

    MCP41010_CS_HIGH(); // 拉高CS引脚，结束通信
}
