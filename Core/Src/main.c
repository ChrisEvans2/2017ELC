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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32_dsp.h"
#include "table_fft.h"
#include "math.h"
#include "arm_math.h"
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

float ADC_Vol; 
uint32_t ADC_Data; 

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
  /*在STM32F1系列中用的是M3核心，并不包括FPU（浮点运算单元）。所以应该尽量减少浮点计算的使用来增加系统速度。
  常见的做法是将浮点数乘以一个倍数（倍数取决于想要的精度）进行整数的运算。软件模拟浮点运算（特别是除法）会非常慢。
  具体计算如下：
  软件模拟加减和乘法为几十到上百个指令周期，触发则需要几百到上千个指令周期。我们分别取100和500作为参考。
  本身的运算中N=256也就是要进行128次运算，本身是三个除法再乘法，也就是600*3 = 1800个指令周期/每一次运算
  1.8K * 128 = 230.4K个指令周期
  优化后外面把最占用时间的除法优化掉了只剩下三个乘法，也就是一个周期里面是100*3 = 300个指令周期
  300*128 +1.8K = 38.4K个指令周期
  总计节省了 192K个指令周期 
  也就是 192K * 14ns = 2.688ms （使用典型值14ns）
  不过考虑在项目里面其实不是一个反复调用的函数知识开始用了一下 应该没什么大的区别。。。 如果反复调用比如循环中使用是一个非常大的优化！
  */
  float normal_factor = (float)N / 32768.0f;  // 归一化系数，不需要每次循环都进行计算 同时 .0f 保证是浮点数不会丢失精度
  float normal_freq_factor = (float)Fs / N;  // 归一化频率系数, 同样的不需要每次都进行计算
	for(i=0; i<N/2; i++)
	{
		lX  = (signed short) FFT_OUT[i] >> 16;  // 提取高16位 使用signed short确保它是有符号的16位整数
		lY  = (signed short)(FFT_OUT[i] & 0xFFFF); // 用位运算比原本的连续两次移位更快
    // 直接使用归一化后的系数进行计算
		X = (float)lX * normal_factor;  
		Y = (float)lY * normal_factor;
		Mag = sqrt(X * X + Y * Y) / N;
		if(i == 0)
			lBufMagArray[i] = (unsigned long)(Mag * 32768);
     else
			lBufMagArray[i] = (unsigned long)(Mag * 65536);
		 
		printf("%d      ",i);
		printf("%f      ",normal_freq_factor*i);  // 使用归一化系数而不是每次都计算
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
  MX_DAC_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
	
	HAL_TIM_Base_Start(&htim6);
  HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_2,(uint32_t *) Sine12bit,32,DAC_ALIGN_12B_R);
	
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_ConvertedValue, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	printf("这是一个FFT实验\r\n");
	printf("初始化完成\r\n");
	
	HAL_Delay(10);
	
	// 生成FFT数据
	for(int i = 0;i<N;i++)
	{
		x[i] = 2048*sin((float)W*F*i)+2048;
		FFT_IN[i] = x[i] << 16;
	}
	printf("输入信号：\r\n");
	for(int i = 0;i<N;i++)
	{
		printf("%d (input :%d)\r\n", x[i], i+1);
	}
	
	// 计算FFT
	cr4_fft_256_stm32(FFT_OUT, FFT_IN, N);
	
	GetPowerMag();
	
  float normal_adc_factor = (float)3.3 / 4096.0f;  // 同样的归一化系数
  while (1)
  {
		ADC_Data = ADC_ConvertedValue;
    // 省去几百个指令周期 也就是us级别的优化 挺多的了一直读取，不过本身都等1s delay应该没什么区别
		ADC_Vol =(float) ADC_Data * normal_adc_factor; // 读取转换的AD值
//		printf("\r\n The current AD value = 0x%04X \r\n", ADC_Data); 
//		printf("\r\n The current AD value = %f V \r\n",ADC_Vol);     
		HAL_Delay(1000);  
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
