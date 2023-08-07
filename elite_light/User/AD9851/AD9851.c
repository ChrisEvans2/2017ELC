//***************************************************//
//����1:           ad9851_reset()                    //
//����2:           ad9851_reset_serial()             //
//����3:           ad9851_wr_parrel(unsigned char w0,double frequence)//
//����4:           ad9851_wr_serial(unsigned char w0,double frequence)//
//�汾:              V1.1                            //
//���ڣ�             2008/12/19                      //
//�޸����ڣ�         2008/12/19                      //
//��д�ߣ�         xyc5208@163.com)            //
//***************************************************//
//                  �ӳ���˵��                       //
//***************************************************//
//����1:  ad9851_reset()
//        ��λad9851��֮��Ϊ����д��ģʽ        
//����2:  ad9851_reset_serial()
//        ��λad9851��֮��Ϊ����д��ģʽ
//����3:  ad9851_wr_parrel(unsigned char w0,double frequence)
//        ����дad9851���ݣ�w0Ϊad9851��w0�����ݣ�frequence
//        Ϊд���Ƶ��	
//����4:  ad9851_wr_serial(unsigned char w0,double frequence)
//        ����дad9851���ݣ�w0Ϊad9851��w0�����ݣ�frequence
//        Ϊд���Ƶ��	
//�趨���λ��
         //ad9851_w_clk    ;
         //ad9851_fq_up    ;
         //ad9851_rest     ;
         //ad9851_bit_data ;
//����
         //sbit ad9851_w_clk    =P2^2;
         //sbit ad9851_fq_up    =P2^1;
         //sbit ad9851_rest     =P2^0;
         //sbit ad9851_bit_data =P1^7;
//***************************************************//
//                 д����˵��                        //
//***************************************************//
//д��������
//       ad9851_reset()
//       wr_lcd02_data(unsigned char x)
//       ad9851_wr_parrel(0x01,1000)
//       ad9851_wr_serial(0x01,1000)
//***************************************************//
//---------------------------------------------------//
//                   ����                            //
//---------------------------------------------------//
# include <AD9851.h>
# include <stdio.h>

u8 AD9851_FD=0x01; //��Ƶ��
void (*_AD9851_Setfq)(u8 w0,double frequence);
//P1Ϊ8λ���ݿ�
//***************************************************//
//              ad9851��λ(����ģʽ)                 //
//---------------------------------------------------//
void ad9851_reset()
{
ad9851_w_clk=0;
ad9851_fq_up=0;
//rest�ź�
ad9851_rest=0;
ad9851_rest=1;
ad9851_rest=0;
}
//***************************************************//
//              ad9851��λ(��ģʽ)                 //
//---------------------------------------------------//
void ad9851_reset_serial()
{
ad9851_w_clk=0;
ad9851_fq_up=0;
//rest�ź�
ad9851_rest=0;
ad9851_rest=1;
ad9851_rest=0;
//w_clk�ź�
ad9851_w_clk=0;
ad9851_w_clk=1;
ad9851_w_clk=0;
//fq_up�ź�
ad9851_fq_up=0;
ad9851_fq_up=1;
ad9851_fq_up=0;
}
//***************************************************//
//          ��ad9851��д����������(����)             //
//---------------------------------------------------//
void ad9851_wr_parrel(u8 w0,double frequence)
{
volatile u32 w;
long int y;
double x;
volatile u32 w_test;
	
//����Ƶ�ʵ�HEXֵ
x=4294967295/125;//�ʺ�180M����/180Ϊ����ʱ��Ƶ�ʣ���30M����Ƶ��
//���ʱ��Ƶ�ʲ�Ϊ180MHZ���޸ĸô���Ƶ��ֵ����λMHz ������
frequence=frequence/1000000;
frequence=frequence*x;
y=(long int)frequence;
//дw0����
w=w0;   
//AD9851_DataBus=w|(w^0xff)<<16;     //w0
w_test = (w&0X000000FF);
AD9851_DataBus = (AD9851_DataBus & 0XFF00)|(w_test);
ad9851_w_clk=1;
ad9851_w_clk=0;
//дw1����
w=(y>>24);
//AD9851_DataBus=w|(w^0xff)<<16;     //w0
//w_test = w|(w^0xff)<<16;
	
w_test = (w&0X000000FF);
AD9851_DataBus = (AD9851_DataBus & 0XFF00)|(w_test);

ad9851_w_clk=1;
ad9851_w_clk=0;
//дw2����
w=(y>>16);
//AD9851_DataBus=w|(w^0xff)<<16;     //w0
//w_test = w|(w^0xff)<<16;

w_test = (w&0X000000FF);
AD9851_DataBus = (AD9851_DataBus & 0XFF00)|(w_test);
ad9851_w_clk=1;
ad9851_w_clk=0;
//дw3����
w=(y>>8);
//AD9851_DataBus=w|(w^0xff)<<16;     //w0
//w_test = w|(w^0xff)<<16;
w_test = (w&0X000000FF);
AD9851_DataBus = (AD9851_DataBus & 0XFF00)|(w_test);
ad9851_w_clk=1;
ad9851_w_clk=0;
//дw4����
w=(y>>=0);
//AD9851_DataBus=w|(w^0xff)<<16;     //w0
w_test = (w&0X000000FF);
AD9851_DataBus = (AD9851_DataBus & 0XFF00)|(w_test);

ad9851_w_clk=1;
ad9851_w_clk=0;
//����ʼ��
ad9851_fq_up=1;
ad9851_fq_up=0;
}
//***************************************************//
//          ��ad9851��д����������(����)             //
//---------------------------------------------------//
void ad9851_wr_serial(u8 w0,double frequence)
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
ad9851_bit_data=(w>>i)&0x01;
ad9851_w_clk=1;
ad9851_w_clk=0;
}
//дw3����
w=(y>>8);
for(i=0;i<8;i++)
{
ad9851_bit_data=(w>>i)&0x01;
ad9851_w_clk=1;
ad9851_w_clk=0;
}
//дw2����
w=(y>>16);
for(i=0;i<8;i++)
{
ad9851_bit_data=(w>>i)&0x01;
ad9851_w_clk=1;
ad9851_w_clk=0;
}
//дw1����
w=(y>>24);
for(i=0;i<8;i++)
{
ad9851_bit_data=(w>>i)&0x01;
ad9851_w_clk=1;
ad9851_w_clk=0;
}
//дw0����
w=w0;   
for(i=0;i<8;i++)
{
ad9851_bit_data=(w>>i)&0x01;
ad9851_w_clk=1;
ad9851_w_clk=0;
}
//����ʼ��
ad9851_fq_up=1;
ad9851_fq_up=0;
}
void AD9851_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
    
	// ʹ��IO��ʱ��
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();

	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		// �������
	GPIO_InitStructure.Pin = GPIO_PIN_0  |	GPIO_PIN_1 | GPIO_PIN_10;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_7;
}
void AD9851_Setfq(double fq)
{
	if(ad9851_ad9850) 
	{
		fq *= 1.44;
		AD9851_FD = 0;
	}
	_AD9851_Setfq(AD9851_FD,fq);
}
//***************************************************//
//                   ���Գ���1000Hz                  //
//---------------------------------------------------//
//���룺mode  ad9851_parallel ����    ad9851_serial ����
//FD��0������Ƶ 1��2��Ƶ
void AD9851_Init(u8 mode,u8 FD)
{
	AD9851_IO_Init();
//	AD9851_DataBus=0x00|(~0x00)<<16;
	
//	ad9851_reset_serial();
//	ad9851_wr_serial(0x00,1000000);
	if(mode==ad9851_parallel){_AD9851_Setfq=ad9851_wr_parrel;ad9851_reset();}
	else {_AD9851_Setfq=ad9851_wr_serial;ad9851_reset_serial();}
	if(FD==1)AD9851_FD=0x01;
	AD9851_Setfq(10000);
}





