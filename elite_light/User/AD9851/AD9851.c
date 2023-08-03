#include "AD9851.h"
#include <stdio.h>

u8 AD9851_FD=0x01; //��Ƶ��
void (*_AD9851_Setfq)(u8 w0,double frequence);

// 9851��λ
void AD9851_reset_serial()
{
	AD9851_CLK_Clr;
	AD9851_FQ_Clr;
	//rest�ź�
	AD9851_RST_Clr;
	AD9851_RST_Set;
	AD9851_RST_Clr;
	//w_clk�ź�
	AD9851_CLK_Clr;
	AD9851_CLK_Set;
	AD9851_CLK_Clr;
	//fq_up�ź�
	AD9851_FQ_Clr;
	AD9851_FQ_Set;
	AD9851_FQ_Clr;
}

//          ��ad9851��д����������(����)             //
void AD9851_wr_serial(u8 w0,double frequence)
{
	unsigned char i,w;
	long int y;
	double x;
	//����Ƶ�ʵ�HEXֵ
	x=4294967295/180;//�ʺ�180M����/180Ϊ����ʱ��Ƶ�ʣ���30M����Ƶ��
	//���ʱ��Ƶ�ʲ�Ϊ180MHZ���޸ĸô���Ƶ��ֵ����λMHz  ������
	frequence=frequence/1000000;
	frequence=frequence*x;
	y=(long int)frequence;
	//дw4����
	w=(y>>=0);
	for(i=0;i<8;i++)
	{
		ad9851_bit_data=(w>>i)&0x01;
		AD9851_CLK_Set;
		AD9851_CLK_Clr;
	}
	//дw3����
	w=(y>>8);
	for(i=0;i<8;i++)
	{
		ad9851_bit_data=(w>>i)&0x01;
		AD9851_CLK_Set;
		AD9851_CLK_Clr;
	}
	//дw2����
	w=(y>>16);
	for(i=0;i<8;i++)
	{
		ad9851_bit_data=(w>>i)&0x01;
		AD9851_CLK_Set;
		AD9851_CLK_Clr;
	}
	//дw1����
	w=(y>>24);
	for(i=0;i<8;i++)
	{
		ad9851_bit_data=(w>>i)&0x01;
		AD9851_CLK_Set;
		AD9851_CLK_Clr;
	}
	//дw0����
	w=w0;   
	for(i=0;i<8;i++)
	{
		ad9851_bit_data=(w>>i)&0x01;
		AD9851_CLK_Set;
		AD9851_CLK_Clr;
	}
	//����ʼ��
	AD9851_FQ_Set;
	AD9851_FQ_Clr;
}


void GPIO_AD9851_Configuration(void)
{ 

	GPIO_InitTypeDef GPIO_InitStructure = {0};
    
	// ʹ��IO��ʱ��
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		// �������
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
