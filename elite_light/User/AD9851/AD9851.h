#ifndef __AD9851_H
#define __AD9851_H

#include "main.h"

#define	AD9851_CLK_PORT			GPIOB
#define AD9851_CLK_PIN			GPIO_PIN_6

#define	AD9851_FQ_PORT			GPIOB
#define AD9851_FQ_PIN				GPIO_PIN_7

#define	AD9851_RST_PORT			GPIOB
#define AD9851_RST_PIN			GPIO_PIN_8

#define	AD9851_DATA_PORT		GPIOB
#define AD9851_DATA_PIN			GPIO_PIN_9


#define AD9851_CLK_Set    HAL_GPIO_WritePin(AD9851_CLK_PORT,AD9851_CLK_PIN,GPIO_PIN_SET)           // 时钟信号
#define AD9851_CLK_Clr    HAL_GPIO_WritePin(AD9851_CLK_PORT,AD9851_CLK_PIN,GPIO_PIN_RESET)

#define AD9851_RST_Set    HAL_GPIO_WritePin(AD9851_RST_PORT,AD9851_RST_PIN,GPIO_PIN_SET)			   //开关电源板
#define AD9851_RST_Clr    HAL_GPIO_WritePin(AD9851_RST_PORT,AD9851_RST_PIN,GPIO_PIN_RESET)

#define AD9851_FQ_Set   HAL_GPIO_WritePin(AD9851_FQ_PORT,AD9851_FQ_PIN,GPIO_PIN_SET)         
#define AD9851_FQ_Clr    HAL_GPIO_WritePin(AD9851_FQ_PORT,AD9851_FQ_PIN,GPIO_PIN_RESET)

#define AD9851_DATA_Set    HAL_GPIO_WritePin(AD9851_DATA_PORT,AD9851_DATA_PIN,GPIO_PIN_SET)         // 串行低电平，并行高电平
#define AD9851_DATA_Clr    HAL_GPIO_WritePin(AD9851_DATA_PORT,AD9851_DATA_PIN,GPIO_PIN_RESET)

#define ad9851_bit_data PBout(9)  //接AD9851的D7脚


void AD9851_Init(void);
void AD9851_Setfq(double fq);
void AD9851_reset_serial(void);
void AD9851_wr_serial(u8 w0,double frequence);
void ad9851_reset_serial(void);

#endif 
