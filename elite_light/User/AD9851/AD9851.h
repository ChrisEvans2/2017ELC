#ifndef __AD9851_H
#define __AD9851_H	 
#include "main.h"

#define ad9851_ad9850 0///0-ad9851  1-ad9850

#define ad9851_w_clk    PBout(1)  //P2.2�ڽ�AD9851��w_clk��/PIN7
#define ad9851_fq_up    PBout(10)  //P2.1�ڽ�AD9851��fq_up��/PIN8
#define ad9851_rest     PBout(0)  //P2.0�ڽ�AD9851��rest��/PIN12
#define ad9851_bit_data PAout(7)  //P1.7�ڽ�AD9851��D7��/PIN25
#define AD9851_DataBus	GPIOA->ODR//GPIOA->BSRR

#define ad9851_parallel    	0 
#define ad9851_serial    		1 
#define ad9851_FD_0    			0 
#define ad9851_FD_6   			1 

void AD9851_Init(u8 mode,u8 FD);
void AD9851_Setfq(double fq);
void ad9851_wr_parrel(u8 w0,double frequence);
void ad9851_wr_serial(u8 w0,double frequence);
void ad9851_reset_serial(void);


#endif

