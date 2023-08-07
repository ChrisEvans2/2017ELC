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
#include "lvgl.h"
#include "ui.h"
#include "adc.h"
#include "dac.h"
#include "stm32_dsp.h"
#include "table_fft.h"
#include "math.h"
#include "tim.h"
#include "arm_math.h"
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
uint8_t DDS_Sweep_state = 0;

double DDS1_Fre_array[9] = {1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0};
double DDS2_Fre_array[9] = {1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0};

double DDS1_Fre_1M;
double DDS2_Fre_1M;

uint16_t max, max_index;
uint16_t second_max, second_max_index;
double Div_Fre1 = 0, Div_Fre2 = 0, Div_Fre_temp;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void My_Sweep_out()
{
	uint32_t start_Fre = 1000000, end_Fre = 100000000, step_Fre = 1000000;
	static uint32_t Fre = 1000000;
	if(DDS_Sweep_state)
	{
		AD9854_SetSine(Fre ,815);
		Fre += step_Fre;
		if(Fre >= end_Fre)
		{
			Fre = start_Fre;
		}
	}else if(Fre != start_Fre){
			Fre = start_Fre;
	}
}

void ADC_DAC_show()
{
	float temp_adc;
	
	adcx=HAL_DAC_GetValue(&hdac,DAC_CHANNEL_1);//读取前面设置DAC的值
	printf("DAC_reg: %d\r\n", adcx);
	temp_adc=(float)adcx*(3.3/4096);			    //得到DAC电压值
	printf("DAC_out: %f\r\n", temp_adc);
	adcx=HAL_ADC_GetValue(&hadc1);     //得到ADC转换值	  
	temp_adc=(float)adcx*(3.3/4096);			    //得到ADC电压值
	printf("ADC_out: %f\r\n", temp_adc);
}
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
	for(int i = *max_index-1; i < *max_index+1; i++)
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
	float X,Y,Mag,Phase[ADC_NUM/2];
	float pha = 0;
	unsigned short i;
	
	for(i=0; i<ADC_NUM/2; i++)
	{
		lX  =  FFT_OUT[i] >> 16;
		lY  = (FFT_OUT[i] << 16 ) >> 16;
		X = ADC_NUM * ((float)lX) / 32768;
		Y = ADC_NUM * ((float)lY) / 32768;
		Mag = sqrt(X * X + Y * Y) / ADC_NUM;
		Phase[i] = atan2(Y,X);
		if(pha > 180)
		{
			pha = 180;
		}else if(pha < -180)
		{
			pha = -180;
		}
		if(i == 0)
			lBufMagArray[i] = (unsigned long)(Mag * 32768);
     else
			lBufMagArray[i] = (unsigned long)(Mag * 65536);
		 
//		printf("%d      ",i+1);
//		printf("%f      ",(float)Fs/ADC_NUM*i);
//		printf("%d      ",lBufMagArray[i]);
//		printf("%f      ",X);
//		printf("%f      ",Y);
//		printf("%f			\r\n",Phase[i]*180/3.1415926535);
	}
	printf("\n");
//	for(i=0; i<ADC_NUM/2; i++)
//	{
//		Send_u32(lBufMagArray[i]);
//	}
//	for(i=0; i<ADC_NUM/2; i++)
//	{
//		Send_float(Phase[i]*180/3.1415926535);
//	}

}
float find_near_fre(float fre)
{
	int a = fre / 5000; 	// 整除5KHz
	float b = fre - a * 5000; 	// 取余
	if (b > 2500) {
		return (a + 1) * 5000;
	}
	return a * 5000;

}
// 设定20k-100k修正参数
double find_calibrate(double Fre, uint8_t chanel)
{
	int16_t temp;
	double calibrate;
	temp = Fre/5000 ;
	if(temp<=4) // 小于20K用20K
	{
		if(chanel == 1)
		{
			calibrate = DDS1_Fre_array[0];
		}
		if(chanel == 2)
		{
			calibrate = DDS2_Fre_array[0];
		}
	}else if(temp>=20) // 大于100K用100K
	{
		if(chanel == 1)
		{
			calibrate = DDS1_Fre_array[8];
		}
		if(chanel == 2)
		{
			calibrate = DDS2_Fre_array[8];
		}
	}else
	{
		if( (int)((temp-4)/2)>0 && (int)((temp-4)/2)<9 ) // 防止数组越界
		{ 
			if(chanel == 1)
			{
				calibrate = DDS1_Fre_array[(int)(temp-4)/2]; 
			}
			if(chanel == 2)
			{
				calibrate = DDS2_Fre_array[(int)(temp-4)/2]; 
			}
		}
	}
	return calibrate;
}


void DDS_Output_Set()
{
	find_max((uint32_t*)lBufMagArray, ADC_NUM/2, &max, &second_max, &max_index, &second_max_index);
	Div_Fre1 = (int)Fs/ADC_NUM*max_index;
	Div_Fre2 = (int)Fs/ADC_NUM*second_max_index;
	if(Div_Fre1 < Div_Fre2)
	{
		Div_Fre_temp = Div_Fre1;
		Div_Fre1 = Div_Fre2;
		Div_Fre2 = Div_Fre_temp;
	}
	if(Div_Fre2 < 10000)
	{
		Div_Fre2 = Div_Fre1;
	}
//	printf("\r\n                       \r\n");
	printf("max1:%d  max_index:%d  Fre:%f\r\nmax2:%d  max2_index:%d  Fre:%f\r\n", max, max_index, Div_Fre1, second_max, second_max_index, Div_Fre2);
	
	Div_Fre1 = find_near_fre(Div_Fre1);
	Div_Fre2 = find_near_fre(Div_Fre2);
	
	Div_Fre1 = Div_Fre1 * find_calibrate(Div_Fre1, 1) / 1000000.0;
	Div_Fre2 = Div_Fre2 * find_calibrate(Div_Fre2, 2) / 1000000.0;
	
	printf("Set_Fre1:%f\r\nSet_Fre2:%f\r\n", Div_Fre1, Div_Fre2);
	
	AD9854_SetSine((uint32_t)Div_Fre1, 4000);
	AD9851_Setfq(Div_Fre2);

}

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern DMA_HandleTypeDef hdma_dac_ch1;
extern TIM_HandleTypeDef htim6;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */
extern key_t Key0, Key1, Key2;
extern uint16_t A;
extern uint32_t F;

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
  * @brief This function handles ADC1 and ADC2 global interrupts.
  */
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */

  /* USER CODE END ADC1_2_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  HAL_ADC_IRQHandler(&hadc2);
  /* USER CODE BEGIN ADC1_2_IRQn 1 */

  /* USER CODE END ADC1_2_IRQn 1 */
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
		HAL_UART_Transmit(&UART_TYPE, rx_buffer,rx_len, 10);//将收到的数据发送出去

		HAL_UART_Receive_DMA(&UART_TYPE,rx_buffer,BUFFSIZE);//开启DMA接收，方便下一次接收数据
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
uint8_t keep_count_DDS1 = 0, keep_count_DDS2 = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim6){	
		time6++;
		lv_tick_inc(1);
		My_Sweep_out();
		if(time6%10 == 0) // 按键状态更新
		{
			Key_Scan(&Key0, KEY0_GPIO_Port, KEY0_Pin);
			Key_Scan(&Key1, KEY1_GPIO_Port, KEY1_Pin);
			Key_Scan2(&Key2, KEY2_GPIO_Port, KEY2_Pin);
		}
		if(Key0.keyFlag == 1)
		{
			Key0.keyFlag = 0;
			if(keep_count_DDS1 <9)
			{
				DDS1_Fre_array[keep_count_DDS1] = 1000000.0 + lv_spinbox_get_value(ui_DDS1_calibrate_Spinbox_1)/10000.0;
				printf("DDS1_Fre_array[%d] set : %f\r\n", keep_count_DDS1, DDS1_Fre_array[keep_count_DDS1]);
				keep_count_DDS1 += 1;
				HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
			}
		}
		if(Key1.keyFlag == 1)
		{
			Key1.keyFlag = 0;
			if(keep_count_DDS2 <9)
			{
				DDS2_Fre_array[keep_count_DDS2] = 1000000.0 + lv_spinbox_get_value(ui_DDS1_calibrate_Spinbox_2)/10000.0;
				printf("DDS2_Fre_array[%d] set : %f\r\n", keep_count_DDS2, DDS1_Fre_array[keep_count_DDS2]);
				keep_count_DDS2 += 1;
				HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
			}			
		}
		if(Key2.keyFlag == 1)
		{
			Key2.keyFlag = 0;
			HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_Array, ADC_NUM);
//			A += 10;
//			printf("A:%d", A);
//			AD9854_SetSine (F, A);
		}


		if(time6 == 1000)
		{
			time6 = 0;
			time6_s++;
//			printf("This is 1s (%d)\r\n", time6_s);
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
	cr4_fft_256_stm32(FFT_OUT, FFT_IN, ADC_NUM);
	
	GetPowerMag();
	DDS_Output_Set();
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
}

/* USER CODE END 1 */
