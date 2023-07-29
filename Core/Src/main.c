/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32_dsp.h"
#include "table_fft.h"
#include "math.h"
#include "arm_math.h"
#include "arm_const_structs.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define F 	50
#define Fs 	1000
#define MY_PI 	3.1416
#define W		2*MY_PI/Fs
#define N		256

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int32_t x[N];
int32_t FFT_IN[N],FFT_OUT[N];
int32_t lBufMagArray[N] = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void GetPowerMag()
{
	signed short lX,lY;
	float X,Y,Mag;
	unsigned short i;
	int a;
	for(i=0; i<N/2; i++)
	{
		lX  =  FFT_OUT[i] >> 16;
		lY  = (FFT_OUT[i] << 16 ) >> 16;
		X = N * ((float)lX) / 32768;
		Y = N * ((float)lY) / 32768;
		Mag = sqrt(X * X + Y * Y) / N;
		if(i == 0)
			lBufMagArray[i] = (unsigned long)(Mag * 32768);
     else
			lBufMagArray[i] = (unsigned long)(Mag * 65536);
		 
		printf("%d      ",i);
		printf("%f      ",(float)Fs/N*i);
		printf("%d      ",lBufMagArray[i]);
		printf("%f      ",X);
		printf("%f      \r\n",Y);                       
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_USART1_UART_Init();
  MX_FSMC_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  /* USER CODE BEGIN 2 */
//  HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_1,(uint32_t *) Sine12bit,32,DAC_ALIGN_12B_R);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_ConvertedValue, 1);
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOB, LCD_BL_Pin|LED0_Pin, GPIO_PIN_SET);
//	LCD_Init();
//	tp_dev.init();

//	
//	POINT_COLOR=RED; 
//	LCD_ShowString(30,50,200,16,16,(unsigned char*)"ELITE STM32");	
//	LCD_ShowString(30,70,200,16,16,(unsigned char*)"DAC TEST");	
//	LCD_ShowString(30,90,200,16,16,(unsigned char*)"ATOM@ALIENTEK");
//	LCD_ShowString(30,110,200,16,16,(unsigned char*)"2019/9/18");	 
//	LCD_ShowString(30,130,200,16,16,(unsigned char*)"KEY1:-  KEY1:+");	  
//	POINT_COLOR=BLUE;//设置字体为蓝色      	 
//	LCD_ShowString(30,150,200,16,16,(unsigned char*)"DAC VAL:");	      
//	LCD_ShowString(30,170,200,16,16,(unsigned char*)"DAC VOL:0.000V");	      
//	LCD_ShowString(30,190,200,16,16,(unsigned char*)"ADC VOL:0.000V"); 	

//	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	HAL_TIM_Base_Start_IT(&htim6);
//	
//  /* USER CODE END 2 */

//  /* Infinite loop */
//  /* USER CODE BEGIN WHILE */
//	printf("初始化完成");
//	// 生成FFT数据
//	for(int i = 0;i<N;i++)
//	{
//		x[i] = 2048*sin((float)W*F*i)+2048;
//		FFT_IN[i] = x[i] << 16;
//	}
//	// 计算FFT
//	cr4_fft_256_stm32(FFT_OUT, FFT_IN, N);
//	
//	GetPowerMag();

	while (1)
  {
//		tp_dev.scan(0);
//		HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
      
		Delay_ms(5);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
