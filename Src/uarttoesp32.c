#include "uarttoesp32.h"

uint8_t TransmitCpl = 0;
uint8_t frame_send_buff[SEND_LEN] = {0};
uint8_t current_index = 1;


HAL_StatusTypeDef status;
UART_HandleTypeDef huart1;        // USART句柄

// 串口初始化
void MX_USART1_UART_Init(void)                       
{
    huart1.Instance = USART1;                       // 使用哪个串口
    huart1.Init.BaudRate = 115200;                  // 波特率
    huart1.Init.WordLength = UART_WORDLENGTH_8B;    // 长度
    huart1.Init.StopBits = UART_STOPBITS_1;         //停止位
    huart1.Init.Parity = UART_PARITY_NONE;          //奇偶校验位
    huart1.Init.Mode = UART_MODE_TX;             // 收发
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;    // 没有硬件流控制
    
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
    Error_Handler();
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;         // 复用输出
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)         
{

  if(uartHandle->Instance==USART1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  }
}

/**************************************帧生成与发送********************************************/
uint8_t SumCheck(uint8_t index)                           // 数据和
{
    uint8_t sum_check = 0;
    for(uint8_t i=0; i<4;i++)
    {
        sum_check += frame_send_buff[index + i];
    }
    return sum_check;
}

void float_to_singleframe(float* data,uint8_t* buffer)                          // 把数据做成单帧
{   
    uint8_t *byte_arr;                                                          // 取数据的首地址，数据以字节存贮，看成一个字节数组
    byte_arr = (uint8_t*)data;
    for(uint8_t i=0; i<PRE_DATA_LEN; i++)
    {
        if (i<PRE_DATA_LEN - 1)     buffer[current_index+i] = byte_arr[i];                   // 前4位是数据,这种多字节存储是小端存储
        else                        buffer[current_index+i] = SumCheck(current_index);       // 最后一位是校验和
    }
    current_index += PRE_DATA_LEN;
}

void FloatFrameSend(float* data)                                      // float类型发送，中断发送
{
    frame_send_buff[0] = FRAME_HEAD;                                    // 先加个帧头
    for(uint8_t i=0;i<DATA_NUM;i++)                                     // 循环遍历数据，将帧加入数据域
    {
        float_to_singleframe(&data[i],frame_send_buff);       
    }
    frame_send_buff[END_POS] = FRAME_END;                               // 加个帧尾
    current_index = 1;
    HAL_UART_Transmit_IT(&huart1,frame_send_buff,SEND_LEN);             // 异步发送
}


/*************************************中断处理******************8******************************/
void USART1_IRQHandler()
{
    HAL_UART_IRQHandler(&huart1);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
        TransmitCpl = 1;
    }
}

