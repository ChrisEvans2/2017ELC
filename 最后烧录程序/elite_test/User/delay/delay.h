#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f1xx_hal.h"

void SysTick_Init(uint8_t SYSCLK); //SysTick��ʱ����ʼ������
void delay_us(uint32_t nus);       //΢�뼶��ʱ����
void delay_ms(uint16_t nms);       //���뼶��ʱ����


#endif
