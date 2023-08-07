#include "AD9850.h"
#include <stdio.h>

u8 AD9850_FD=0x00; //��Ƶ��
void (*_AD9850_Setfq)(u8 w0,double frequence);

// 9851��λ
void AD9850_reset_serial()
{
	AD9850_CLK_Clr;
	AD9850_FQ_Clr;
	//rest�ź�
	AD9850_RST_Clr;
	AD9850_RST_Set;
	AD9850_RST_Clr;
	//w_clk�ź�
	AD9850_CLK_Clr;
	AD9850_CLK_Set;
	AD9850_CLK_Clr;
	//fq_up�ź�
	AD9850_FQ_Clr;
	AD9850_FQ_Set;
	AD9850_FQ_Clr;
}

//          ��ad9851��д����������(����)             //
void AD9850_wr_serial(u8 w0,double frequence)
{
	unsigned char i,w;
	long int y;
	double x;
	//����Ƶ�ʵ�HEXֵ
	x=4294967295/125;//�ʺ�180M����/180Ϊ����ʱ��Ƶ�ʣ���30M����Ƶ��
	//���ʱ��Ƶ�ʲ�Ϊ180MHZ���޸ĸô���Ƶ��ֵ����λMHz  ������
	frequence=frequence/1000000;
	frequence=frequence*x;
	y=(long int)frequence;
	//дw4����
	w=(y>>=0);
	for(i=0;i<8;i++)
	{
		ad9850_bit_data=(w>>i)&0x01;
		AD9850_CLK_Set;
		AD9850_CLK_Clr;
	}
	//дw3����
	w=(y>>8);
	for(i=0;i<8;i++)
	{
		ad9850_bit_data=(w>>i)&0x01;
		AD9850_CLK_Set;
		AD9850_CLK_Clr;
	}
	//дw2����
	w=(y>>16);
	for(i=0;i<8;i++)
	{
		ad9850_bit_data=(w>>i)&0x01;
		AD9850_CLK_Set;
		AD9850_CLK_Clr;
	}
	//дw1����
	w=(y>>24);
	for(i=0;i<8;i++)
	{
		ad9850_bit_data=(w>>i)&0x01;
		AD9850_CLK_Set;
		AD9850_CLK_Clr;
	}
	//дw0����
	w=w0;   
	for(i=0;i<8;i++)
	{
		ad9850_bit_data=(w>>i)&0x01;
		AD9850_CLK_Set;
		AD9850_CLK_Clr;
	}
	//����ʼ��
	AD9850_FQ_Set;
	AD9850_FQ_Clr;
}


void GPIO_AD9850_Configuration(void)
{ 

	GPIO_InitTypeDef GPIO_InitStructure = {0};
    
	// ʹ��IO��ʱ��
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();

	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		// �������
	GPIO_InitStructure.Pin = AD9850_CLK_PIN  |	AD9850_FQ_PIN;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.Pin =  AD9850_RST_PIN | AD9850_DATA_PIN;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void AD9850_Setfq(double fq)
{
	AD9850_wr_serial(AD9850_FD,fq);
}

void AD9850_Init()
{
	GPIO_AD9850_Configuration();
	delay_ms(5);
	AD9850_CLK_Clr;	
	AD9850_FQ_Clr;
	AD9850_RST_Clr;
	AD9850_DATA_Clr;	
	AD9850_reset_serial();
}
