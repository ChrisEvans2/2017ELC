#ifndef _AD9850_H
#define _AD9850_H

#include "main.h"  
typedef unsigned char u8;
//typedef unsigned int u16;
//typedef unsigned long int u32;

#define AD9850_CONTROL_PORT  GPIOC
#define AD9850_FQUD   GPIO_Pin_1  //改成自己连接的引脚
#define AD9850_WCLK   GPIO_Pin_2  //改成自己连接的引脚
#define AD9850_DATA   GPIO_Pin_11  //改成自己连接的引脚
#define AD9850_RST    GPIO_Pin_12  //改成自己连接的引脚

#define AD9850_WCLK_H	HAL_GPIO_(GPIOB, GPIO_Pin_2, Bit_SET)
#define AD9850_WCLK_L	GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET)
#define AD9850_FQUD_H	GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET)
#define AD9850_FQUD_L	GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET)
#define AD9850_DATA_H	GPIO_WriteBit(GPIOF, GPIO_Pin_11, Bit_SET)
#define AD9850_DATA_L	GPIO_WriteBit(GPIOF, GPIO_Pin_11, Bit_RESET)
#define AD9850_RESET_H	GPIO_WriteBit(GPIOF, GPIO_Pin_12, Bit_SET)
#define AD9850_RESET_L	GPIO_WriteBit(GPIOF, GPIO_Pin_12, Bit_RESET)


void AD9850_Delay(unsigned int z); 
void AD9850_Write(unsigned char w0,double frequence);
//void AD9850_Write(u8 w0,double frequence);
void AD9850_Init(void);

#endif

