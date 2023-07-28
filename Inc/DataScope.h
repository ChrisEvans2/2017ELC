#ifndef __DATA_SCOPE_H
#define __DATA_SCOPE_H
 
#include "main.h"
 
#define FRAME_HEAD			0xAB
#define FRAME_SADDR			0x01
#define FRAME_DADDR			0xFE

#define FRAME_ID1				0xF1
#define FRAME_ID2				0xF2
#define FRAME_ID3				0xF3
#define FRAME_ID4				0xF4
#define FRAME_ID5				0xF5
#define FRAME_ID6				0xF6
#define FRAME_ID7				0xF7
#define FRAME_ID8				0xF8
#define FRAME_ID9				0xF9
#define FRAME_ID10			0xFA

#define DATA_LEN				4
#define CHECK_LEN				(DATA_LEN  + 6)
#define SEND_LEN				(CHECK_LEN + 2)


void DataScope_Float_Generate( float f );
void DataScope_Uint32_Generate( uint32_t u32 );
void Send_float( float f );
void Send_u32( uint32_t u32 );

void Send_frame();  // ·¢ËÍÊý¾ÝÖ¡

 
#endif 



