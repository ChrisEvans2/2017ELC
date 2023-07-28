#ifndef __UARTTOESP32_H
#define __UARTTOESP32_H

#include "main.h"

#define FRAME_HEAD      0xFF        // ֡ͷ
#define FRAME_END       0xDD        // ֡β
#define PRE_DATA_LEN    5           // ÿ�����ݵĳ��ȣ�����У��ͣ�
#define DATA_NUM        10          // ���ݸ���
#define DATA_AREA_LEN   (DATA_NUM*PRE_DATA_LEN)      // �����򳤶�
#define END_POS         (DATA_AREA_LEN + 1)          // ֡βλ��
#define SEND_LEN        (DATA_AREA_LEN + 2)          // ����֡��֡����

void MX_USART1_UART_Init(void);

extern uint8_t TransmitCpl;
void USART1_IRQHandler(void);
void FloatFrameSend(float* data);

#endif
