#ifndef __AD9854_H
#define __AD9854_H
#include "gpio.h"

//    AD9854         |      STM32               
// PIV               -->   +5V
// GND               -->   GND
// A0/SDIO           -->   GPIOA_PIN5/PA5				
// A1/SDO            -->   GPIOA_PIN6/PA6			
// A2/IORST          -->   GPIOA_PIN7/PA7
// RD                -->   GPIOC_PIN8/PC8
// WR                -->   GPIOC_PIN9/PC9
// UDCLK             -->   GPIOC_PIN10/PC10
// RESET             -->   GPIOC_PIN11/PC11
// S/P               -->   GPIOC_PIN12/PC12

#define AD9854_RD_PORT 				GPIOF
#define AD9854_RD_PIN					GPIO_PIN_0

#define AD9854_WR_PORT 				GPIOF
#define AD9854_WR_PIN					GPIO_PIN_1

#define AD9854_UDCLK_PORT 		GPIOF 
#define AD9854_UDCLK_PIN			GPIO_PIN_2

#define AD9854_RST_PORT 			GPIOF
#define AD9854_RST_PIN				GPIO_PIN_6

#define AD9854_SP_PORT 				GPIOF	
#define AD9854_SP_PIN					GPIO_PIN_7

#define AD9854_A0_PORT 				GPIOF
#define AD9854_A0_PIN					GPIO_PIN_3

#define AD9854_A1_PORT 				GPIOF
#define AD9854_A1_PIN					GPIO_PIN_4

#define AD9854_A2_PORT 				GPIOF
#define AD9854_A2_PIN					GPIO_PIN_5


//从编程寄存器中读取并行数据。与CS共享的功能。与串行编程总线相关联的芯片选择信号。低电平有效。当选择并行模式时，此引脚与RD共享。
#define AD9854_RD_Set    HAL_GPIO_WritePin(AD9854_RD_PORT,AD9854_RD_PIN,GPIO_PIN_SET)         
#define AD9854_RD_Clr    HAL_GPIO_WritePin(AD9854_RD_PORT,AD9854_RD_PIN,GPIO_PIN_RESET)

//将并行数据写入I/O端口缓冲区。与SCLK共享的函数。串行时钟信号与串行编程总线相关联。数据在上升沿注册。当选择并行模式时，此引脚与WR共享
//模式取决于Pin 70（S/P SELECT）。
#define AD9854_WR_Set    HAL_GPIO_WritePin(AD9854_WR_PORT,AD9854_WR_PIN,GPIO_PIN_SET)         
#define AD9854_WR_Clr    HAL_GPIO_WritePin(AD9854_WR_PORT,AD9854_WR_PIN,GPIO_PIN_RESET)

#define AD9854_UDCLK_Set    HAL_GPIO_WritePin(AD9854_UDCLK_PORT,AD9854_UDCLK_PIN,GPIO_PIN_SET)           // 时钟信号
#define AD9854_UDCLK_Clr    HAL_GPIO_WritePin(AD9854_UDCLK_PORT,AD9854_UDCLK_PIN,GPIO_PIN_RESET)

#define AD9854_RST_Set    HAL_GPIO_WritePin(AD9854_RST_PORT,AD9854_RST_PIN,GPIO_PIN_SET)			   //开关电源板
#define AD9854_RST_Clr    HAL_GPIO_WritePin(AD9854_RST_PORT,AD9854_RST_PIN,GPIO_PIN_RESET)

#define AD9854_SP_Set    HAL_GPIO_WritePin(AD9854_SP_PORT,AD9854_SP_PIN,GPIO_PIN_SET)         // 串行低电平，并行高电平
#define AD9854_SP_Clr    HAL_GPIO_WritePin(AD9854_SP_PORT,AD9854_SP_PIN,GPIO_PIN_RESET)

#define AD9854_OSK_Set   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET)         
#define AD9854_OSK_Clr    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET)

#define AD9854_FDATA_Set    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET)        //fsk/bpsk/hold  
#define AD9854_FDATA_Clr    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_RESET)

// I/O驱动  使用别名
#define SPI_IO_RST_Set    HAL_GPIO_WritePin(AD9854_A2_PORT,AD9854_A2_PIN,GPIO_PIN_SET)            // DDS的A2口与STM32的PA7连接
#define SPI_IO_RST_Clr    HAL_GPIO_WritePin(AD9854_A2_PORT,AD9854_A2_PIN,GPIO_PIN_RESET)          // 串行IO允许复位

#define SPI_SDO_Set    HAL_GPIO_WritePin(AD9854_A1_PORT,AD9854_A1_PIN,GPIO_PIN_SET)               // DDS的A1口与STM32的PA6连接
#define SPI_SDO_Clr    HAL_GPIO_WritePin(AD9854_A1_PORT,AD9854_A1_PIN,GPIO_PIN_RESET)             // 单向串行传输端口,3线串行通信

#define SPI_SDI_Set 	 HAL_GPIO_WritePin(AD9854_A0_PORT,AD9854_A0_PIN,GPIO_PIN_SET)             // DDS的A0口与STM32的PA5连接
#define SPI_SDI_Clr 	 HAL_GPIO_WritePin(AD9854_A0_PORT,AD9854_A0_PIN,GPIO_PIN_RESET)           // 双向串行数据输入输出，2线串行

 // AD9854寄存器地址
#define PHASE1	0x00	  //phase adjust register #1
#define PHASE2  0x01		//phase adjust register #2
#define FREQ1   0x02		//frequency tuning word 1
#define FREQ2   0x03		//frequency tuning word 2
#define DELFQ   0x04		//delta frequency word
#define UPDCK   0x05		//update clock
#define RAMPF   0x06		//ramp rate clock
#define CONTR   0x07		//control register
#define SHAPEI  0x08		//output shape key I mult
#define SHAPEQ  0x09		//output shape key Q mult 
#define RAMPO   0x0A		//output shape key ramp rate
#define CDAC    0x0B		//QDAC

//**************************以下部分为函数定义********************************
extern void GPIO_AD9854_Configuration(void);						// AD9854_IO口初始化
static void AD9854_WR_Byte(uint8_t Adata);	  
extern void AD9854_Init(void);						  
static void Freq_convert(long Freq);	         	  
extern void AD9854_SetSine(uint32_t Freq,uint16_t Shape);	  
static void Freq_double_convert(double Freq);		  
extern void AD9854_SetSine_double(double Freq,uint16_t Shape);
extern void AD9854_InitFSK(void);				
extern void AD9854_SetFSK(uint32_t Freq1,uint32_t Freq2);					  
extern void AD9854_InitBPSK(void);					  
extern void AD9854_SetBPSK(uint16_t Phase1,uint16_t Phase2);					
extern void AD9854_InitOSK(void);					 
extern void AD9854_SetOSK(uint8_t RateShape);					  
extern void AD9854_InitAM(void);					 
extern void AD9854_SetAM(uint16_t Shape);					
extern void AD9854_InitRFSK(void);					 
extern void AD9854_SetRFSK(uint32_t Freq_Low,uint32_t Freq_High,uint32_t Freq_Up_Down,uint32_t FreRate);				
extern void Sweep_out (void);

#endif
