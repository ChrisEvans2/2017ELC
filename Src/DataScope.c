#include "DataScope.h"
#include "usart.h"
#include "uarttoesp32.h"
unsigned char DataScope_OutPut_Buffer[SEND_LEN] = {0};	   //串口发送缓冲区
uint8_t sumcheck = 0, addcheck = 0;

extern float ADC_Vol;

// 获取校验和
void Get_check(uint8_t *tx_buffer)
{
	sumcheck = 0;
	addcheck = 0;
	for(uint8_t i = 0; i<CHECK_LEN; i++	)
	{
		sumcheck += tx_buffer[i];
		addcheck += sumcheck;
	}
}

void Float2Byte(float *target,unsigned char *buf,unsigned char beg)
{
    unsigned char *point;
    point = (unsigned char*)target;	  //得到float的地址
    buf[beg]   = point[0];
    buf[beg+1] = point[1];
    buf[beg+2] = point[2];
    buf[beg+3] = point[3];
}
void Uint32_t2Byte(uint32_t *target,unsigned char *buf,unsigned char beg)
{
    unsigned char *point;
    point = (unsigned char*)target;	  //得到uint32的地址
    buf[beg]   = point[0];
    buf[beg+1] = point[1];
    buf[beg+2] = point[2];
    buf[beg+3] = point[3];
}
uint32_t my_data = 10;
//函数说明：生成帧格式
//返回0表示帧格式生成失败 
void DataScope_Float_Generate( float f )
{
	DataScope_OutPut_Buffer[0] = FRAME_HEAD;  	// 帧头
	DataScope_OutPut_Buffer[1] = FRAME_SADDR;  	// 源地址
	DataScope_OutPut_Buffer[2] = FRAME_DADDR;  	// 目标地址
	DataScope_OutPut_Buffer[3] = FRAME_ID1;  				// 帧ID
	
	DataScope_OutPut_Buffer[4] = 0x04;  				// 数据长度低字节
	DataScope_OutPut_Buffer[5] = 0x00;  				// 数据长度高字节
	
	Float2Byte(&f, DataScope_OutPut_Buffer, 6);

	Get_check(DataScope_OutPut_Buffer);
	DataScope_OutPut_Buffer[CHECK_LEN] 		= sumcheck;  		// 校验和
	DataScope_OutPut_Buffer[CHECK_LEN+1] 	= addcheck;  		// 附加校验
}

void DataScope_Uint32_Generate( uint32_t u32 )
{
	DataScope_OutPut_Buffer[0] = FRAME_HEAD;  	// 帧头
	DataScope_OutPut_Buffer[1] = FRAME_SADDR;  	// 源地址
	DataScope_OutPut_Buffer[2] = FRAME_DADDR;  	// 目标地址
	DataScope_OutPut_Buffer[3] = FRAME_ID2;  				// 帧ID
	
	DataScope_OutPut_Buffer[4] = 0x04;  				// 数据长度低字节
	DataScope_OutPut_Buffer[5] = 0x00;  				// 数据长度高字节
	
	Uint32_t2Byte(&u32, DataScope_OutPut_Buffer, 6);

	Get_check(DataScope_OutPut_Buffer);
	DataScope_OutPut_Buffer[CHECK_LEN] 		= sumcheck;  		// 校验和
	DataScope_OutPut_Buffer[CHECK_LEN+1] 	= addcheck;  		// 附加校验
}

void Send_float( float f )
{
	DataScope_Float_Generate(f);
	HAL_UART_Transmit_IT(&huart1, DataScope_OutPut_Buffer, SEND_LEN);
}

void Send_u32( uint32_t u32 )
{
	DataScope_Uint32_Generate(u32);
	HAL_UART_Transmit_IT(&huart1, DataScope_OutPut_Buffer, SEND_LEN);
}