#include "AD9954.h"

void AD9954_Init(void)
{
    GPIO_AD9954_Init();            //GPIO��ʼ��
    AD9954_RESET();                    //DDS��λ
    delay_ms(300);                    //�ӳ�300ms
    AD9954_CS = 0;
    //single tone
    AD9954_SENDBYTE(CFR1);//��ַ0д����
    AD9954_SENDBYTE(0x02);//
    AD9954_SENDBYTE(0x10);
    AD9954_SENDBYTE(0x00);
    AD9954_SENDBYTE(0x40);//�Ƚ���power down      
//    AD9954_SENDBYTE(0x00);//�Ƚ���ʹ��        
 
    AD9954_SENDBYTE(CFR2);//��ַ1д����
    AD9954_SENDBYTE(0x00);//
    AD9954_SENDBYTE(0x08);
    #if fs>400
        #error "ϵͳƵ�ʳ���оƬ���ֵ"
    #endif
    #if fs>=250
        AD9954_SENDBYTE(PLL_MULTIPLIER<<3|0x04|0X03);
    #else
        AD9954_SENDBYTE(1<<3);
    #endif
    AD9954_CS=1;
}
 
 
// AD9954�ӿ�IO��ʼ��
void GPIO_AD9954_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
    
	// ʹ��IO��ʱ��
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		// �������
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;

	GPIO_InitStructure.Pin = GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_11 | GPIO_PIN_13;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
    
	GPIO_InitStructure.Pin = GPIO_PIN_7 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14| GPIO_PIN_15;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_11;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
	  
	AD9954_IOSY=0;
	AD9954_OSK=0;
	AD9954_PWR=0;
}
 
 
// ��λAD9954
void AD9954_RESET(void)
{
    AD9954_CS = 1;
    AD9954_RES = 0;
    delay_ms(10);
    AD9954_RES = 1;
    delay_ms(10);
    AD9954_RES = 0;
    delay_ms(10);
    AD9954_CS = 0;
    AD9954_SCLK = 0;
    PS0 = 0;
    PS1 = 0;
    IOUPDATE = 0;
    AD9954_CS = 1;
}
 
 
// ��AD9954����һ���ֽڵ�����
void AD9954_SENDBYTE(u8 dat)
{
    u8 i;
    for (i = 0;i<8;i++)
    {
        AD9954_SCLK = 0;
        if (dat & 0x80)
        {
            AD9954_SDIO = 1;
        }
        else
        {
            AD9954_SDIO = 0;
        }
        AD9954_SCLK = 1;
        dat <<= 1;
    }
}
 
 
// ��AD9954����һ���ֽڵ�����
u8 AD9954_ReadByte(void)
{
    u8 i,dat=0;
    for (i = 0;i<8;i++)
    {
        AD9954_SCLK = 0;
        dat|=AD9954_SDO;
        AD9954_SCLK = 1;
        dat <<= 1;
    }
    return dat;
}
 
 
 
//����һ�������źţ�����AD9954�ڲ��Ĵ���
void UPDATE(void)
{
    IOUPDATE=0;
//    delay_us(100); 
    IOUPDATE = 1;
    delay_us(2);
    IOUPDATE = 0;
}
 
u32 Get_FTW(double Real_fH)
{
	return (u32)(10.7374*Real_fH);
}
 
 
/*********************************************************************************************************
** �������� ��void AD9954_SETFRE(float f)
** �������� ������AD9954��ǰ��Ƶ����������õ��ǵ�һƵ�����
** ����˵�� ����Ϊ���õĸ��������м��㣬ת�������л������ͨ���������Ծ�ȷ��0.1Hz����
** ��ڲ��� �������õ�Ƶ��ֵ
** ���ڲ��� ����
*********************************************************************************************************/
void AD9954_SETFRE(double f)//single tone
{
    u32 date;
    AD9954_CS = 0;
    
    date = Get_FTW(f);//det=(f/fclk)x2^32=10.7374xf
    AD9954_SENDBYTE(FTW0);//FTW0��ַ
    AD9954_SENDBYTE((u8)(date >> 24));//Ƶ�ʿ�����
    AD9954_SENDBYTE((u8)(date >> 16));
    AD9954_SENDBYTE((u8)(date >> 8));
    AD9954_SENDBYTE((u8)date);
    AD9954_CS=1;
    UPDATE();
    
    #ifdef EIT_USART_DEBUG
                printf("\n\r DDS���Ƶ��Ϊ��%.2f Hz \n\r" ,f);            
    #endif
}
 
 
 
/*********************************************************************************************************
** �������� ��void Write_ASF(u16 factor)
** �������� ���ı�scale factor��ֵ���ı�DAC�������
** ����˵�� ��д�����Ϊ0X3FFF,��СΪ0�������ֵԼ500mv
** ��ڲ��� ����
** ���ڲ��� ����
*********************************************************************************************************/
void Write_ASF(u16 factor)  //
{
    AD9954_CS = 0;
 
    AD9954_SENDBYTE(0x02);//����    
    AD9954_SENDBYTE(factor >> 8);
    AD9954_SENDBYTE(factor);
    AD9954_CS = 1;
    UPDATE();
 
}