/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "dac.h"
#include "stm32_dsp.h"
#include "table_fft.h"
#include "math.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "tim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint16_t temp = 0;

u16 dacval=620;
u16 adcx;
float ADC_Vol; 
uint32_t ADC_Data; 

int32_t FFT_IN[ADC_NUM],FFT_OUT[ADC_NUM];
int32_t lBufMagArray[ADC_NUM] = {0};

extern key_t Key0, Key1, Key2;
extern uint16_t A;
extern uint32_t F;

static uint32_t DDS_Fre_set = 10000000;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void ADC_DAC_show()
{
	float temp_adc;
	
	LCD_ShowxNum(10,160,temp_adc,3,16,0X80); 	    //显示电压值的小数部分
	LCD_ShowxNum(110,180,A,3,16,0X80); 	    //显示电压值的小数部分
}
void LCD_DDS_Show()
{
	LCD_ShowString(30,150,200,16,16,(unsigned char*)"DDS Fre:");	      
	LCD_ShowxNum(94,150,DDS_Fre_set,8,16,0);     	    //显示DAC寄存器值
}

//void find_max(uint32_t *array, int array_size, uint16_t *max, uint16_t *second_max, uint16_t *max_index, uint16_t *second_max_index) {
//	*max = array[1];
//	*second_max = array[2];
//	*max_index = 1;
//	*second_max_index = 2;
//	for (int i = 2; i < array_size; i++) {
//		if (array[i] > *max) {
//			*second_max = *max;
//			*second_max_index = *max_index;
//			*max = array[i];
//			*max_index = i;
//		} else if (array[i] > *second_max) {
//					*second_max = array[i];
//					*second_max_index = i;
//		}
//	}
//}
void find_max(uint32_t *array, int array_size, uint16_t *max, uint16_t *second_max, uint16_t *max_index, uint16_t *second_max_index) {
	*max = 0;
	*max_index = 0;
	*second_max = 0;
	*second_max_index = 0;
	for (int i = 1; i < array_size; i++) {
//		printf("array[%d]: %d\r\n", i, array[i]);
		if (array[i] > *max) {
			*max = array[i];
			*max_index = i;
		}
	}
	for(int i = *max_index-10; i < *max_index+10; i++)
	{
		if(i>=0&&i<=array_size-1) {array[i] = 0;}
	}
	for(int i = 1; i<array_size; i++)
	{
		if (array[i] > *second_max) {
			*second_max = array[i];
			*second_max_index = i;
		}
	}
}
void GetPowerMag()
{
	signed short lX,lY;
	float X,Y,Mag;
	unsigned short i;

	for(i=0; i<ADC_NUM/2; i++)
	{
		lX  =  FFT_OUT[i] >> 16;
		lY  = (FFT_OUT[i] << 16 ) >> 16;
		X = ADC_NUM * ((float)lX) / 32768;
		Y = ADC_NUM * ((float)lY) / 32768;
		Mag = sqrt(X * X + Y * Y) / ADC_NUM;
		if(i == 0)
			lBufMagArray[i] = (unsigned long)(Mag * 32768);
     else
			lBufMagArray[i] = (unsigned long)(Mag * 65536);
		 
		printf("%d      ",i+1);
		printf("%f      ",(float)Fs/ADC_NUM*i);
		printf("%d      \r\n",lBufMagArray[i]);
//		printf("%f      ",X);
//		printf("%f      \r\n",Y);
	}
	for(i=0; i<ADC_NUM/2; i++)
	{
		Send_u32(lBufMagArray[i]);
	}
	uint16_t max, max_index;
	uint16_t second_max, second_max_index;
	float Div_Fre1 = 0, Div_Fre2 = 0, Div_Fre_temp;
	find_max((uint32_t*)lBufMagArray, ADC_NUM/2, &max, &second_max, &max_index, &second_max_index);
	Div_Fre1 = (float)Fs/ADC_NUM*max_index;
	Div_Fre2 = (float)Fs/ADC_NUM*second_max_index;
	if(Div_Fre1 < Div_Fre2)
	{
		Div_Fre_temp = Div_Fre1;
		Div_Fre1 = Div_Fre2;
		Div_Fre2 = Div_Fre_temp;
	}
	printf("\r\n                       \r\n");
	printf("max1:%d  max_index:%d  Fre:%f\r\nmax2:%d  max2_index:%d  Fre:%f\r\n", max, max_index, Div_Fre1, second_max, second_max_index, Div_Fre2);
	
	AD9854_SetSine((uint32_t)Div_Fre1, 4000);
	AD9851_Setfq(Div_Fre2);
	
	HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
}

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_dac_ch1;
extern TIM_HandleTypeDef htim6;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel5 global interrupt.
  */
void DMA1_Channel5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */

  /* USER CODE END DMA1_Channel5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA1_Channel5_IRQn 1 */

  /* USER CODE END DMA1_Channel5_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
	uint8_t tmp_flag = 0;
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
	tmp_flag =__HAL_UART_GET_FLAG(&UART_TYPE,UART_FLAG_IDLE); //获取IDLE标志位
	if((tmp_flag != RESET)){//通过标志位判断接收是否结束
		
		rx_flag = 1; //置1表明接收结束
		__HAL_UART_CLEAR_IDLEFLAG(&UART_TYPE);//清除标志位
		HAL_UART_DMAStop(&UART_TYPE); 
		
		temp = __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
		temp = huart1.hdmarx->Instance->CNDTR;

		rx_len =BUFFSIZE-temp; //计算出数据长度
		
		printf("传输长度:%d\r\r\n", rx_len);
		HAL_UART_Transmit(&UART_TYPE, rx_buffer,rx_len, 10); //将收到的数据发送出去

		HAL_UART_Receive_DMA(&UART_TYPE,rx_buffer,BUFFSIZE); //开启DMA接收，方便下一次接收数据
  }
  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt.
  */
void TIM6_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_IRQn 0 */

  /* USER CODE END TIM6_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_IRQn 1 */

  /* USER CODE END TIM6_IRQn 1 */
}

/**
  * @brief This function handles DMA2 channel3 global interrupt.
  */
void DMA2_Channel3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Channel3_IRQn 0 */

  /* USER CODE END DMA2_Channel3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_dac_ch1);
  /* USER CODE BEGIN DMA2_Channel3_IRQn 1 */

  /* USER CODE END DMA2_Channel3_IRQn 1 */
}

/* USER CODE BEGIN 1 */
uint16_t time6 = 0, time6_s = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim6){	
		time6++;
		if(time6%10 == 0) // 按键状态更新
		{
			Key_Scan(&Key0, KEY0_GPIO_Port, KEY0_Pin);
			Key_Scan(&Key1, KEY1_GPIO_Port, KEY1_Pin);
			Key_Scan2(&Key2, KEY2_GPIO_Port, KEY2_Pin);
		}
		if(Key0.keyFlag == 1)
		{
			Key0.keyFlag = 0;
			DDS_Fre_set += 10000;
			AD9854_SetSine(DDS_Fre_set, A);
			LCD_DDS_Show();
			
//			if(dacval<1240)
//			{
//				dacval+=16;
//				HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,dacval);//设置DAC值
//			}
//			ADC_DAC_show();	
		}
		if(Key1.keyFlag == 1)
		{
			Key1.keyFlag = 0;
			DDS_Fre_set -= 10000;
			AD9854_SetSine(DDS_Fre_set, A);
			LCD_DDS_Show();
			
//			if(dacval>16)
//			{
//				dacval-=16;
//				HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,dacval);//设置DAC值				
//			}
//			else 
//			{
//				dacval=0;
//				HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,dacval);//设置DAC值
//			}
//			ADC_DAC_show();	
		}
		if(Key2.keyFlag == 1)
		{
			Key2.keyFlag = 0;
			HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_Array, ADC_NUM);
			
//			A += 10;
//			printf("A:%d", A);
//			AD9854_SetSine (F, A);
//			ADC_DAC_show();	
		}


		if(time6 == 1000)  // 1s中断
		{
			time6 = 0;
			time6_s++;
//			printf("This is 1s (%d)", time6_s);
			
			LCD_DDS_Show();	
		}
	}
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
//	printf("ADC DMA complete!\r\n");
	// 生成FFT数据
	for(int i = 0;i<ADC_NUM;i++)
	{
		FFT_IN[i] = ADC_Array[i] << 16;
//		Send_u32(ADC_Array[i]);  // 查看波形
	}
	// 计算FFT
	cr4_fft_1024_stm32(FFT_OUT, FFT_IN, ADC_NUM);
	GetPowerMag();
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

	
}
/* USER CODE END 1 */
