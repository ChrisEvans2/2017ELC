#include "uarttoesp32.h"

uint8_t TransmitCpl = 0;
uint8_t frame_send_buff[SEND_LEN] = {0};
uint8_t current_index = 1;


HAL_StatusTypeDef status;
UART_HandleTypeDef huart1;        // USART���

// ���ڳ�ʼ��
void MX_USART1_UART_Init(void)                       
{
    huart1.Instance = USART1;                       // ʹ���ĸ�����
    huart1.Init.BaudRate = 115200;                  // ������
    huart1.Init.WordLength = UART_WORDLENGTH_8B;    // ����
    huart1.Init.StopBits = UART_STOPBITS_1;         //ֹͣλ
    huart1.Init.Parity = UART_PARITY_NONE;          //��żУ��λ
    huart1.Init.Mode = UART_MODE_TX;             // �շ�
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;    // û��Ӳ��������
    
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
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;         // �������
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

/**************************************֡�����뷢��********************************************/
uint8_t SumCheck(uint8_t index)                           // ���ݺ�
{
    uint8_t sum_check = 0;
    for(uint8_t i=0; i<4;i++)
    {
        sum_check += frame_send_buff[index + i];
    }
    return sum_check;
}

void float_to_singleframe(float* data,uint8_t* buffer)                          // ���������ɵ�֡
{   
    uint8_t *byte_arr;                                                          // ȡ���ݵ��׵�ַ���������ֽڴ���������һ���ֽ�����
    byte_arr = (uint8_t*)data;
    for(uint8_t i=0; i<PRE_DATA_LEN; i++)
    {
        if (i<PRE_DATA_LEN - 1)     buffer[current_index+i] = byte_arr[i];                   // ǰ4λ������,���ֶ��ֽڴ洢��С�˴洢
        else                        buffer[current_index+i] = SumCheck(current_index);       // ���һλ��У���
    }
    current_index += PRE_DATA_LEN;
}

void FloatFrameSend(float* data)                                      // float���ͷ��ͣ��жϷ���
{
    frame_send_buff[0] = FRAME_HEAD;                                    // �ȼӸ�֡ͷ
    for(uint8_t i=0;i<DATA_NUM;i++)                                     // ѭ���������ݣ���֡����������
    {
        float_to_singleframe(&data[i],frame_send_buff);       
    }
    frame_send_buff[END_POS] = FRAME_END;                               // �Ӹ�֡β
    current_index = 1;
    HAL_UART_Transmit_IT(&huart1,frame_send_buff,SEND_LEN);             // �첽����
}


/*************************************�жϴ���******************8******************************/
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

