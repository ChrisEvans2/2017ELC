#include "AD9954.h"

void AD9954_Init(void)
{
    GPIO_AD9954_Init();            //GPIO初始化
    AD9954_RESET();                    //DDS复位
    delay_ms(300);                    //延迟300ms
    AD9954_CS = 0;
    //single tone
    AD9954_SENDBYTE(CFR1);//地址0写操作
    AD9954_SENDBYTE(0x02);//
    AD9954_SENDBYTE(0x10);
    AD9954_SENDBYTE(0x00);
    AD9954_SENDBYTE(0x40);//比较器power down      
//    AD9954_SENDBYTE(0x00);//比较器使能        
 
    AD9954_SENDBYTE(CFR2);//地址1写操作
    AD9954_SENDBYTE(0x00);//
    AD9954_SENDBYTE(0x08);
    #if fs>400
        #error "系统频率超过芯片最大值"
    #endif
    #if fs>=250
        AD9954_SENDBYTE(PLL_MULTIPLIER<<3|0x04|0X03);
    #else
        AD9954_SENDBYTE(1<<3);
    #endif
    AD9954_CS=1;
}
 
 
// AD9954接口IO初始化
void GPIO_AD9954_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
    
	// 使能IO口时钟
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		// 推挽输出
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
 
 
// 复位AD9954
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
 
 
// 往AD9954发送一个字节的内容
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
 
 
// 往AD9954接收一个字节的内容
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
 
 
 
//产生一个更新信号，更新AD9954内部寄存器
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
** 函数名称 ：void AD9954_SETFRE(float f)
** 函数功能 ：设置AD9954当前的频率输出，采用的是单一频率输出
** 函数说明 ：因为采用的浮点数进行计算，转换过程中会出现误差，通过调整可以精确到0.1Hz以内
** 入口参数 ：欲设置的频率值
** 出口参数 ：无
*********************************************************************************************************/
void AD9954_SETFRE(double f)//single tone
{
    u32 date;
    AD9954_CS = 0;
    
    date = Get_FTW(f);//det=(f/fclk)x2^32=10.7374xf
    AD9954_SENDBYTE(FTW0);//FTW0地址
    AD9954_SENDBYTE((u8)(date >> 24));//频率控制字
    AD9954_SENDBYTE((u8)(date >> 16));
    AD9954_SENDBYTE((u8)(date >> 8));
    AD9954_SENDBYTE((u8)date);
    AD9954_CS=1;
    UPDATE();
    
    #ifdef EIT_USART_DEBUG
                printf("\n\r DDS输出频率为：%.2f Hz \n\r" ,f);            
    #endif
}
 
 
 
/*********************************************************************************************************
** 函数名称 ：void Write_ASF(u16 factor)
** 函数功能 ：改变scale factor数值，改变DAC输出幅度
** 函数说明 ：写入最大为0X3FFF,最小为0，最大峰峰值约500mv
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************/
void Write_ASF(u16 factor)  //
{
    AD9954_CS = 0;
 
    AD9954_SENDBYTE(0x02);//幅度    
    AD9954_SENDBYTE(factor >> 8);
    AD9954_SENDBYTE(factor);
    AD9954_CS = 1;
    UPDATE();
 
}