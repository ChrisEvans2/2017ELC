#include "AD9851.h"
#include <stdio.h>

u8 AD9851_FD=0x01; //倍频数
void (*_AD9851_Setfq)(u8 w0,double frequence);

// 9851复位
void AD9851_reset_serial()
{
	AD9851_CLK_Clr;
	AD9851_FQ_Clr;
	//rest信号
	AD9851_RST_Clr;
	AD9851_RST_Set;
	AD9851_RST_Clr;
	//w_clk信号
	AD9851_CLK_Clr;
	AD9851_CLK_Set;
	AD9851_CLK_Clr;
	//fq_up信号
	AD9851_FQ_Clr;
	AD9851_FQ_Set;
	AD9851_FQ_Clr;
}

//          向ad9851中写命令与数据(串口)             //
void AD9851_wr_serial(u8 w0,double frequence)
{
	unsigned char i,w;
	long int y;
	double x;
	//计算频率的HEX值
	x=4294967295/180;//适合180M晶振/180为最终时钟频率（或30M六倍频）
	//如果时钟频率不为180MHZ，修改该处的频率值，单位MHz  ！！！
	frequence=frequence/1000000;
	frequence=frequence*x;
	y=(long int)frequence;
	//写w4数据
	w=(y>>=0);
	for(i=0;i<8;i++)
	{
		ad9851_bit_data=(w>>i)&0x01;
		AD9851_CLK_Set;
		AD9851_CLK_Clr;
	}
	//写w3数据
	w=(y>>8);
	for(i=0;i<8;i++)
	{
		ad9851_bit_data=(w>>i)&0x01;
		AD9851_CLK_Set;
		AD9851_CLK_Clr;
	}
	//写w2数据
	w=(y>>16);
	for(i=0;i<8;i++)
	{
		ad9851_bit_data=(w>>i)&0x01;
		AD9851_CLK_Set;
		AD9851_CLK_Clr;
	}
	//写w1数据
	w=(y>>24);
	for(i=0;i<8;i++)
	{
		ad9851_bit_data=(w>>i)&0x01;
		AD9851_CLK_Set;
		AD9851_CLK_Clr;
	}
	//写w0数据
	w=w0;   
	for(i=0;i<8;i++)
	{
		ad9851_bit_data=(w>>i)&0x01;
		AD9851_CLK_Set;
		AD9851_CLK_Clr;
	}
	//移入始能
	AD9851_FQ_Set;
	AD9851_FQ_Clr;
}


void GPIO_AD9851_Configuration(void)
{ 

	GPIO_InitTypeDef GPIO_InitStructure = {0};
    
	// 使能IO口时钟
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		// 推挽输出
	GPIO_InitStructure.Pin = AD9851_CLK_PIN  | AD9851_RST_PIN  |	AD9851_FQ_PIN | AD9851_DATA_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void AD9851_Setfq(double fq)
{
	AD9851_wr_serial(AD9851_FD,fq);
}

void AD9851_Init()
{
	GPIO_AD9851_Configuration();
	AD9851_CLK_Clr;	
	AD9851_FQ_Clr;
	AD9851_RST_Clr;
	AD9851_DATA_Clr;	
	AD9851_reset_serial();
}
