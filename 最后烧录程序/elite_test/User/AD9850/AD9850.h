#ifndef __AD9850_H
#define __AD9850_H

#include "main.h"

#define	AD9850_CLK_PORT			GPIOD
#define AD9850_CLK_PIN			GPIO_PIN_6

#define	AD9850_FQ_PORT			GPIOD
#define AD9850_FQ_PIN				GPIO_PIN_7

#define	AD9850_RST_PORT			GPIOG
#define AD9850_RST_PIN			GPIO_PIN_9

#define	AD9850_DATA_PORT		GPIOG
#define AD9850_DATA_PIN			GPIO_PIN_10


#define AD9850_CLK_Set    HAL_GPIO_WritePin(AD9850_CLK_PORT,AD9850_CLK_PIN,GPIO_PIN_SET)           // 时钟信号
#define AD9850_CLK_Clr    HAL_GPIO_WritePin(AD9850_CLK_PORT,AD9850_CLK_PIN,GPIO_PIN_RESET)

#define AD9850_RST_Set    HAL_GPIO_WritePin(AD9850_RST_PORT,AD9850_RST_PIN,GPIO_PIN_SET)			   //开关电源板
#define AD9850_RST_Clr    HAL_GPIO_WritePin(AD9850_RST_PORT,AD9850_RST_PIN,GPIO_PIN_RESET)

#define AD9850_FQ_Set   	HAL_GPIO_WritePin(AD9850_FQ_PORT,AD9850_FQ_PIN,GPIO_PIN_SET)         
#define AD9850_FQ_Clr    	HAL_GPIO_WritePin(AD9850_FQ_PORT,AD9850_FQ_PIN,GPIO_PIN_RESET)

#define AD9850_DATA_Set   HAL_GPIO_WritePin(AD9850_DATA_PORT,AD9850_DATA_PIN,GPIO_PIN_SET)         // 串行低电平，并行高电平
#define AD9850_DATA_Clr   HAL_GPIO_WritePin(AD9850_DATA_PORT,AD9850_DATA_PIN,GPIO_PIN_RESET)

#define ad9850_bit_data PGout(10)  //接AD9851的D7脚


void AD9850_Init(void);
void AD9850_Setfq(double fq);
void AD9850_reset_serial(void);
void AD9850_wr_serial(u8 w0,double frequence);
void ad9850_reset_serial(void);

#endif 
