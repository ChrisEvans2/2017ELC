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
#include "tim.h"
#include "usart.h"
//#include "uarttoesp32.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "key.h"
#include "ad9854.h"
#include "dma.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LENGTH   3                              // 数据长度
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t F=1000000;  		// 设置正弦波的频率（1~100MHz）
uint16_t A=816;			// 设置正弦波的幅度（0~4095）        816为100mV输出


void key_change_fre(uint8_t s,uint8_t Key_Count)              // 按键改变频率
{
    if(s==2&&Key_Count==1)
    {
        HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_RESET);
        if (F < 135000000)
        {
            if (F < 1000)									// 输出频率小于1kHz，按100Hz步进
            {
                F += 100;
            }
            else if (F< 10000)						// 输出频率小于10kHz，按1kHz步进
            {
                F += 1000;
            }
            else if (F < 100000)					// 输出频率小于100kHz，按10kHz步进
            {
                F += 10000;
            }
            else if (F < 1000000)					// 输出频率小于1MHz，按100kHz步进
            {
                F += 100000;
            }
            else if (F < 10000000)				// 输出频率小于10MHz，按1MHz步进
            {
                F += 1000000;
            }
            else 
            {
                F += 5000000;								// 输出频率小于140MHz，按5MHz步进
            }
        }
            AD9854_SetSine (F, A);
        HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_SET);
        
    }
    if(s==3&&Key_Count==1)
    {
        HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_RESET);
        if (F > 100)
        {
            if (F > 10000000)
            {
                F -= 5000000;
            }
            else if (F > 1000000)
            {
                F -= 1000000;
            }
            else if (F > 100000)
            {
                F -= 100000;
            }
            else if (F > 10000)
            {
                F -= 10000;
            }
            else if (F > 1000)
            {
                F -= 1000;
            }
            else if (F > 100)
            {
                F -= 100;
            }
        }
        AD9854_SetSine (F, A);	
        HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_SET);
    }
}


void step_1MHz_fre(uint8_t s, uint8_t Key_Count)
{
    if (s==2 && Key_Count ==1)
    {
        if (F < 40000000)
            F += 1000000;
        AD9854_SetSine(F,A);
    }
    if(s==3 && Key_Count ==1)
    {
        if (F > 1000000)
            F -= 1000000;
        AD9854_SetSine(F,A);
    }
}

void key_change_vpp(uint8_t s,uint8_t Key_Count)                 // (2^12) / (2^4)，划分128个量级
{
    if(s==2 && Key_Count == 1)
    {
        HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_RESET);
        if (A > 1)
        {  A-=16; }
        AD9854_SetSine (F, A);
        
        HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_SET);
    }
    if(s==3 && Key_Count==1)
    {
        HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_RESET);
        if (A < 4095)
        {  A+=16;  }
        AD9854_SetSine(F,A); 
        HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_SET);
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
    uint8_t Key_Count=0;
    uint8_t s= 0;
    uint32_t lastFre=0;
    float floatarr[10] = {2.34, 45.6, 44.1, 6.7, 23.3, 3.3, 9.8, 74.2, 11.2, 34.2};
    float test = 44.1; 
    uint8_t arr[1] = {0x41};
    uint8_t current_index = 0;
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
    MX_ADC1_Init();

    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */
//    Delay_ms(100);
//    KEY_Init();                  // 独立键盘初始化
//    GPIO_AD9854_Configuration(); // AD9854IO口初始化
//    Delay_ms(5);  
//    AD9854_Init ();
//    Delay_ms(20);
//    HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
//        s = KEY_Scan(0);
//        if (s == 1)
//        {
//            HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
//            FloatFrameSend(floatarr);
//            
//////            sendDone = 1;
////            Send_float_Frame(2.34);
////            HAL_UART_Transmit_IT(&huart1,arr,1);
//        }
//        lastFre = F;
//        s = KEY_Scan(0);
//        if(s==1)
//        {
//            HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
//             Key_Count ++;
//            switch (Key_Count)
//            {
//                case  1 :	{
//                            AD9854_Init ();
//                            Delay_ms(20);
//                            AD9854_SetSine (F, A);
//                            }break;
//            
//                case  2 :	{  		
////												AD9854_Init ();				 
//                            Delay_ms(20);
//                            Sweep_out();
//                            Key_Count=0;
//                            F=1000000;  		
//                            A=4095;			
//                             }break;
//                default :	break;
//                 }	
//         }
//        step_1MHz_fre(s,Key_Count);
//         key_change_vpp(s,Key_Count);
//         
//        if (lastFre != F)
//        {
//            Send_u32(lastFre);
//        }
        
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
