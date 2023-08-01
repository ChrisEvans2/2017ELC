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

//�ӱ�̼Ĵ����ж�ȡ�������ݡ���CS�����Ĺ��ܡ��봮�б�������������оƬѡ���źš��͵�ƽ��Ч����ѡ����ģʽʱ����������RD������
#define AD9854_RD_Set    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET)         
#define AD9854_RD_Clr    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET)

//����������д��I/O�˿ڻ���������SCLK�����ĺ���������ʱ���ź��봮�б�������������������������ע�ᡣ��ѡ����ģʽʱ����������WR����
//ģʽȡ����Pin 70��S/P SELECT����
#define AD9854_WR_Set    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET)         
#define AD9854_WR_Clr    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET)

#define AD9854_UDCLK_Set    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET)           // ʱ���ź�
#define AD9854_UDCLK_Clr    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET)

#define AD9854_RST_Set    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET)			   //���ص�Դ��
#define AD9854_RST_Clr    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET)

#define AD9854_SP_Set    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET)         // ���е͵�ƽ�����иߵ�ƽ
#define AD9854_SP_Clr    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET)

#define AD9854_OSK_Set   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET)         
#define AD9854_OSK_Clr    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET)

#define AD9854_FDATA_Set    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET)        //fsk/bpsk/hold  
#define AD9854_FDATA_Clr    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_RESET)

// I/O����  ʹ�ñ���
#define SPI_IO_RST_Set    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)            // DDS��A2����STM32��PA7����
#define SPI_IO_RST_Clr    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)          // ����IO������λ

#define SPI_SDO_Set    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)               // DDS��A1����STM32��PA6����
#define SPI_SDO_Clr    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)             // �����д���˿�,3�ߴ���ͨ��

#define SPI_SDI_Set 	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)             // DDS��A0����STM32��PA5����
#define SPI_SDI_Clr 	 HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)           // ˫�����������������2�ߴ���

 // AD9854�Ĵ�����ַ
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

//**************************���²���Ϊ��������********************************
extern void GPIO_AD9854_Configuration(void);						// AD9854_IO�ڳ�ʼ��
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
