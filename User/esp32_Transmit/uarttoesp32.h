#ifndef __UARTTOESP32_H
#define __UARTTOESP32_H

#include "main.h"

#define FRAME_HEAD      0xFF        // 帧头
#define FRAME_END       0xDD        // 帧尾
#define PRE_DATA_LEN    5           // 每个数据的长度（加上校验和）
#define DATA_NUM        10          // 数据个数
#define DATA_AREA_LEN   (DATA_NUM*PRE_DATA_LEN)      // 数据域长度
#define END_POS         (DATA_AREA_LEN + 1)          // 帧尾位置
#define SEND_LEN        (DATA_AREA_LEN + 2)          // 发送帧的帧长度

void MX_USART1_UART_Init(void);

extern uint8_t TransmitCpl;
void USART1_IRQHandler(void);
void FloatFrameSend(float* data);

#endif
