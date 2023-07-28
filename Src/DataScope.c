#include "DataScope.h"
#include "usart.h"
#include "uarttoesp32.h"
unsigned char DataScope_OutPut_Buffer[SEND_LEN] = {0};	   //���ڷ��ͻ�����
uint8_t sumcheck = 0, addcheck = 0;

extern float ADC_Vol;

// ��ȡУ���
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
    point = (unsigned char*)target;	  //�õ�float�ĵ�ַ
    buf[beg]   = point[0];
    buf[beg+1] = point[1];
    buf[beg+2] = point[2];
    buf[beg+3] = point[3];
}
void Uint32_t2Byte(uint32_t *target,unsigned char *buf,unsigned char beg)
{
    unsigned char *point;
    point = (unsigned char*)target;	  //�õ�uint32�ĵ�ַ
    buf[beg]   = point[0];
    buf[beg+1] = point[1];
    buf[beg+2] = point[2];
    buf[beg+3] = point[3];
}
uint32_t my_data = 10;
//����˵��������֡��ʽ
//����0��ʾ֡��ʽ����ʧ�� 
void DataScope_Float_Generate( float f )
{
	DataScope_OutPut_Buffer[0] = FRAME_HEAD;  	// ֡ͷ
	DataScope_OutPut_Buffer[1] = FRAME_SADDR;  	// Դ��ַ
	DataScope_OutPut_Buffer[2] = FRAME_DADDR;  	// Ŀ���ַ
	DataScope_OutPut_Buffer[3] = FRAME_ID1;  				// ֡ID
	
	DataScope_OutPut_Buffer[4] = 0x04;  				// ���ݳ��ȵ��ֽ�
	DataScope_OutPut_Buffer[5] = 0x00;  				// ���ݳ��ȸ��ֽ�
	
	Float2Byte(&f, DataScope_OutPut_Buffer, 6);

	Get_check(DataScope_OutPut_Buffer);
	DataScope_OutPut_Buffer[CHECK_LEN] 		= sumcheck;  		// У���
	DataScope_OutPut_Buffer[CHECK_LEN+1] 	= addcheck;  		// ����У��
}

void DataScope_Uint32_Generate( uint32_t u32 )
{
	DataScope_OutPut_Buffer[0] = FRAME_HEAD;  	// ֡ͷ
	DataScope_OutPut_Buffer[1] = FRAME_SADDR;  	// Դ��ַ
	DataScope_OutPut_Buffer[2] = FRAME_DADDR;  	// Ŀ���ַ
	DataScope_OutPut_Buffer[3] = FRAME_ID2;  				// ֡ID
	
	DataScope_OutPut_Buffer[4] = 0x04;  				// ���ݳ��ȵ��ֽ�
	DataScope_OutPut_Buffer[5] = 0x00;  				// ���ݳ��ȸ��ֽ�
	
	Uint32_t2Byte(&u32, DataScope_OutPut_Buffer, 6);

	Get_check(DataScope_OutPut_Buffer);
	DataScope_OutPut_Buffer[CHECK_LEN] 		= sumcheck;  		// У���
	DataScope_OutPut_Buffer[CHECK_LEN+1] 	= addcheck;  		// ����У��
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