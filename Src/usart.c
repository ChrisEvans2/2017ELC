///* USER CODE BEGIN Header */
///**
//  ******************************************************************************
//  * @file    usart.c
//  * @brief   This file provides code for the configuration
//  *          of the USART instances.
//  ******************************************************************************
//  * @attention
//  *
//  * Copyright (c) 2023 STMicroelectronics.
//  * All rights reserved.
//  *
//  * This software is licensed under terms that can be found in the LICENSE file
//  * in the root directory of this software component.
//  * If no LICENSE file comes with this software, it is provided AS-IS.
//  *
//  ******************************************************************************
//  */
///* USER CODE END Header */
///* Includes ------------------------------------------------------------------*/
//#include "usart.h"

///* USER CODE BEGIN 0 */

//uint8_t sendDone = 1;
//uint8_t frame_send_buff[SEND_LEN] = {0};
//uint8_t sum_check = 0;
//HAL_StatusTypeDef status;
///* USER CODE END 0 */

//UART_HandleTypeDef huart1;        // USART句柄

///* USART1 init function */

//void MX_USART1_UART_Init(void)
//{

//    /* USER CODE BEGIN USART1_Init 0 */

//    /* USER CODE END USART1_Init 0 */

//    /* USER CODE BEGIN USART1_Init 1 */

//    /* USER CODE END USART1_Init 1 */   
//    huart1.Instance = USART1;                       // 使用哪个串口
//    huart1.Init.BaudRate = 115200;                  // 波特率
//    huart1.Init.WordLength = UART_WORDLENGTH_8B;    // 长度
//    huart1.Init.StopBits = UART_STOPBITS_1;         //停止位
//    huart1.Init.Parity = UART_PARITY_NONE;          //奇偶校验位
//    huart1.Init.Mode = UART_MODE_TX;             // 收发
//    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;    // 没有硬件流控制
////    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
//    if (HAL_UART_Init(&huart1) != HAL_OK)
//    {
//    Error_Handler();
//    }
//    /* USER CODE BEGIN USART1_Init 2 */
//    /* USER CODE END USART1_Init 2 */

//}

//void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
//{

//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(uartHandle->Instance==USART1)
//  {
//  /* USER CODE BEGIN USART1_MspInit 0 */

//  /* USER CODE END USART1_MspInit 0 */
//    /* USART1 clock enable */
//    __HAL_RCC_USART1_CLK_ENABLE();

////    __HAL_RCC_GPIOA_CLK_ENABLE();
//    /**USART1 GPIO Configuration
//    PA9     ------> USART1_TX
//    PA10     ------> USART1_RX
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_9;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;         // 复用输出
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//    /* USART1 interrupt Init */
//    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(USART1_IRQn);
//  /* USER CODE BEGIN USART1_MspInit 1 */

//  /* USER CODE END USART1_MspInit 1 */
//  }
//}

//void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
//{

//  if(uartHandle->Instance==USART1)
//  {
//  /* USER CODE BEGIN USART1_MspDeInit 0 */

//  /* USER CODE END USART1_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_USART1_CLK_DISABLE();

//    /**USART1 GPIO Configuration
//    PA9     ------> USART1_TX
//    PA10     ------> USART1_RX
//    */
//    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);

//    /* USART1 interrupt Deinit */
//    HAL_NVIC_DisableIRQ(USART1_IRQn);
//  /* USER CODE BEGIN USART1_MspDeInit 1 */

//  /* USER CODE END USART1_MspDeInit 1 */
//  }
//}

///* USER CODE BEGIN 1 */

//void SumCheck(uint8_t *frame_send_buff)                 // 数据和
//{
//    sum_check = 0;
//    for(uint8_t i=0; i<CHECK_BEGIN;i++)
//    {
//        sum_check += frame_send_buff[i];
//    }
//}

//void float_toByte(float* data,uint8_t* buffer,uint8_t lenth)      // 32位uint转字节
//{
//    uint8_t *byte_arr;                                               // 取数据的首地址，数据以字节存贮，看成一个字节数组
//    byte_arr = (uint8_t*)data;
//    for(uint8_t i=0; i<4; i++)
//    {
//        buffer[lenth+i] = byte_arr[3-i];
//    }
//}

//void Uint32toByte(uint32_t* data,uint8_t* buffer,uint8_t lenth)      // 32位uint转字节
//{
//    uint8_t *byte_arr;                                               // 取数据的首地址，数据以字节存贮，看成一个字节数组
//    byte_arr = (uint8_t*)data;
//    for(uint8_t i=0; i<4; i++)
//    {
//        buffer[lenth+i] = byte_arr[3-i];
//    }
//}

//void Generate_Frame(uint32_t data,uint8_t* buffer)                  // 生成帧
//{
//    buffer[0] = FRAME_HEAD;

//    
//    Uint32toByte(&data,frame_send_buff,DATA_BEGIN);
//    SumCheck(frame_send_buff);
//    buffer[CHECK_BEGIN] = sum_check;
//    buffer[CHECK_BEGIN+1] = 0xDD;
//}

//void Generate_float_Frame(float data,uint8_t* buffer)                  // 生成帧
//{
//    buffer[0] = FRAME_HEAD;
//    
//    float_toByte(&data,frame_send_buff,DATA_BEGIN);
////    SumCheck(frame_send_buff);
////    buffer[CHECK_BEGIN] = sum_check;
//    buffer[CHECK_BEGIN] = 0xDD;
//}
//  
//void SendFrame(uint32_t data)                                      // uint32类型发送，中断发送
//{
//    Generate_Frame(data,frame_send_buff);
//    HAL_UART_Transmit_IT(&huart1,frame_send_buff,SEND_LEN);
//}

//void Send_float_Frame(float data)                                      // float类型发送，中断发送
//{
//    Generate_float_Frame(data,frame_send_buff);
//    HAL_UART_Transmit_IT(&huart1,frame_send_buff,SEND_LEN);
//}

//// 中断服务函数
//void USART1_IRQHandler()
//{
//    HAL_UART_IRQHandler(&huart1);
//}

//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//    if (huart->Instance == USART1)
//    {
////        HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
//        sendDone = 1;
//    }
//}

///* USER CODE END 1 */
