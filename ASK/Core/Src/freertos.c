/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
float VPP=500;
float Freq=2;
uint8_t Mode=0;
uint8_t pointer_pos=16;
uint16_t value=0;
float tempVPP;  // ��ʱ����� VPP ֵ
double tempFreq; // ��ʱ����� Freq ֵ
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void DWT_Delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT; // ��ȡ��ǰ����ֵ
    uint32_t ticks = us * (HAL_RCC_GetHCLKFreq() / 1000000);

    while ((DWT->CYCCNT - start) < ticks); // �ȴ�ָ��ʱ��
}

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for usartTask */
osThreadId_t usartTaskHandle;
const osThreadAttr_t usartTask_attributes = {
  .name = "usartTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for keyTask */
osThreadId_t keyTaskHandle;
const osThreadAttr_t keyTask_attributes = {
  .name = "keyTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for oledTask */
osThreadId_t oledTaskHandle;
const osThreadAttr_t oledTask_attributes = {
  .name = "oledTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void appusartTask(void *argument);
void appkeyTask(void *argument);
void appoledTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of usartTask */
  usartTaskHandle = osThreadNew(appusartTask, NULL, &usartTask_attributes);

  /* creation of keyTask */
  keyTaskHandle = osThreadNew(appkeyTask, NULL, &keyTask_attributes);

  /* creation of oledTask */
  oledTaskHandle = osThreadNew(appoledTask, NULL, &oledTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_appusartTask */
/**
  * @brief  Function implementing the usartTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_appusartTask */
void appusartTask(void *argument)
{
  /* USER CODE BEGIN appusartTask */
  uint8_t rx_buffer[1]; // �洢��������
  uint32_t bit_time_us = 104; // ���貨����Ϊ 9600��ÿλ����ʱ��ԼΪ 104 ��s

  /* Infinite loop */
  for (;;)
  {
    // ֱ�Ӷ�ȡ��������
    if (HAL_UART_Receive(&huart1, rx_buffer, 1, 100) == HAL_OK)
    {
      uint8_t data = rx_buffer[0]; // ��ȡ���յ����ֽ�

      // ��ʼλ������ 0��ռ�ձ�Ϊ 0%
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
			DWT_Delay_us(bit_time_us); // ��ʱ 1 λʱ��

      // ����λ����λ����
      for (int i = 0; i < 8; i++)
      {
        uint8_t bit = (data >> i) & 0x01; // ��ȡ�� i λ
        if (bit)
        {
          // �����߼� 1������ռ�ձ�Ϊ 100%
          __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (htim1.Init.Period + 1));
        }
        else
        {
          // �����߼� 0������ռ�ձ�Ϊ 0%
          __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        }
        DWT_Delay_us(bit_time_us);  // ��ʱ 1 λʱ��
      }

      // ֹͣλ������ 1��ռ�ձ�Ϊ 100%
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (htim1.Init.Period + 1));
      DWT_Delay_us(bit_time_us);  // ��ʱ 1 λʱ��
    }

    osDelay(10); 
  }
  /* USER CODE END appusartTask */
}

/* USER CODE BEGIN Header_appkeyTask */
/**
* @brief Function implementing the keyTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_appkeyTask */
void appkeyTask(void *argument)
{
  /* USER CODE BEGIN appkeyTask */
	
		tempVPP = VPP;   // ��ʼ����ʱֵ
		tempFreq = Freq; // ��ʼ����ʱֵ
  /* Infinite loop */
  for (;;)
  {
    // ������ʱֵ
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0)
    {
      // ����ֵ���޸���ʱֵ��
      if (pointer_pos / 16 == 1) // ���� VPP
      {
        tempVPP += 100;
        if (tempVPP > 1200) tempVPP = 1200;
      }
      if (pointer_pos / 16 == 2) // ���� Freq
      {
        tempFreq += 0.2;
        if (tempFreq > 4) tempFreq = 4;
      }
      vTaskDelay(pdMS_TO_TICKS(200)); // ��ֹ��������
    }

    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0)
    {
      // ����ֵ���޸���ʱֵ��
      if (pointer_pos / 16 == 1) // ���� VPP
      {
        tempVPP -= 100;
        if (tempVPP < 100) tempVPP = 100;
      }
      if (pointer_pos / 16 == 2) // ���� Freq
      {
        tempFreq -= 0.2;
        if (tempFreq <= 1.0) tempFreq = 1.0;
      }
      vTaskDelay(pdMS_TO_TICKS(200)); // ��ֹ��������
    }

    // ȷ���޸�
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == 0)
    {
      // ����ȷ�ϼ�������ʱֵ����Ϊʵ��ֵ
      if (pointer_pos / 16 == 1) // ȷ�� VPP
      {
        VPP = tempVPP;
      }
      if (pointer_pos / 16 == 2) // ȷ�� Freq
      {
        Freq = tempFreq;
				
			}
      if (pointer_pos / 16 == 3) // �л� Mode
			{
					Mode = (Mode == 0) ? 1 : 0; 
					if (Mode == 1)
					{
							__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_UPDATE);
							__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
							vTaskResume(usartTaskHandle); // �ָ� appusartTask ����
					}
					else
					{
							__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);
							vTaskSuspend(usartTaskHandle); // ���� appusartTask ����
					}
			}
			AD9833_WaveSeting(Freq*1000000,0,SIN_WAVE,0);
			MCP41010_SetResistance((uint8_t)(VPP/5000*255)); //���÷�ֵ����ֵ��� 255
      vTaskDelay(pdMS_TO_TICKS(200)); // ��ֹ��������
    }

    // �л����λ��
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == 0)
    {
			tempVPP = VPP;   
			tempFreq = Freq; 
      pointer_pos += 16;
      if (pointer_pos == 64) pointer_pos = 16; // ѭ���л�
      vTaskDelay(pdMS_TO_TICKS(200)); // ��ֹ��������
    }

    vTaskDelay(pdMS_TO_TICKS(10)); // �ӳ� 10 ����
  }
  /* USER CODE END appkeyTask */
}

/* USER CODE BEGIN Header_appoledTask */
/**
* @brief Function implementing the oledTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_appoledTask */
void appoledTask(void *argument)
{
  /* USER CODE BEGIN appoledTask */
  taskENTER_CRITICAL();
  char string[30];
  sprintf(string, "param");
  OLED_ShowString(0, 0, (u8 *)string, 16, 1);
	sprintf(string, "value");
  OLED_ShowString(48, 0, (u8 *)string, 16, 1);
  sprintf(string, "VPP");
  OLED_ShowString(0, 16, (u8 *)string, 16, 1);
  sprintf(string, "Freq");
  OLED_ShowString(0, 32, (u8 *)string, 16, 1);
  sprintf(string, "Mode");
  OLED_ShowString(0, 48, (u8 *)string, 16, 1);
	OLED_ShowString(100, 16, (u8 *)"<-", 16, 1);
  OLED_Refresh();
  taskEXIT_CRITICAL();

  /* Infinite loop */
  uint8_t prev_pointer_pos = 16; // ��¼��һ�εĹ��λ��
  int16_t displayedVPP = -1;    // ��ǰ��ʾ�� VPP ֵ
  int32_t displayedFreq = -1;   // ��ǰ��ʾ�� Freq ֵ
  uint8_t displayedMode = 255; // ��ǰ��ʾ�� Mode

  for (;;)
  {
    taskENTER_CRITICAL();

    // ʵʱ��ʾ VPP ֵ
    if (tempVPP != displayedVPP)
    {
      sprintf(string, "%.0fmV ", tempVPP); // ��ӿո������Ӱ
      OLED_ShowString(48, 16, (u8 *)string, 16, 1);
      displayedVPP = tempVPP; // ������ʾ��ֵ
    }

    // ʵʱ��ʾ Freq ֵ
    if (tempFreq != displayedFreq)
    {
			
			if(tempFreq < 1.2)
			{
				sprintf(string, "1.0MHz"); // ��ӿո������Ӱ
			}
			else
			{
				sprintf(string, "%.1fMHz", tempFreq); // ��ӿո������Ӱ
			}
      OLED_ShowString(48, 32, (u8 *)string, 16, 1);
      displayedFreq = tempFreq; // ������ʾ��ֵ
    }

    // ʵʱ��ʾ Mode ֵ
    if (Mode != displayedMode)
    {
      if (Mode == 0)
      {
        sprintf(string, "Random");
      }
      else
      {
        sprintf(string, "Usart ");
      }
      OLED_ShowString(48, 48, (u8 *)string, 16, 1);
      displayedMode = Mode; // ������ʾ�� Mode
    }

    // ��ʾ��ǰ���
    if (prev_pointer_pos != pointer_pos)
    {
      // ����ɵĹ��λ��
      OLED_ShowString(100, prev_pointer_pos, (u8 *)"   ", 16, 1);
      // ��ʾ�¹��λ��
      OLED_ShowString(100, pointer_pos, (u8 *)"<-", 16, 1);
      prev_pointer_pos = pointer_pos;
    }

    // ˢ�� OLED ��ʾ
    OLED_Refresh();

    taskEXIT_CRITICAL();

    vTaskDelay(pdMS_TO_TICKS(50)); // �ӳ� 50 ����
  }
  /* USER CODE END appoledTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

