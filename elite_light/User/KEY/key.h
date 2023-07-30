#ifndef _KEY_H
#define _KEY_H
#include "sys.h"

typedef enum
{
    KEY_CHECK = 0,  //�������״̬
    KEY_COMFIRM,    //����ȷ��״̬
    KEY_RELEASE     //�����ͷ�״̬
}keyState_e;        //״̬ö�ٱ���

typedef struct
{
  keyState_e keyState; //����״̬
  uint8_t keyFlag;     //�������±�־
}key_t;                //����״̬�ṹ��

//����ķ�ʽ��ͨ��λ��������ʽ��ȡIO
//#define KEY0        PEin(4) 	//KEY0����PE4
//#define KEY1        PEin(3) 	//KEY1����PE3
//#define WK_UP       PAin(0) 	//WKUP����PA0

//����ķ�ʽ��ͨ��ֱ�Ӳ���HAL�⺯����ʽ��ȡIO
#define KEY0        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)  //KEY0����PE4
#define KEY1        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)  //KEY1����PE3
#define WK_UP       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //WKUP����PA0

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define WKUP_PRES   3

void KEY_Init(void);
void Key_Scan(key_t *key, GPIO_TypeDef *key_port, uint16_t key_pin);
void Key_Scan2(key_t* key, GPIO_TypeDef *key_port, uint16_t key_pin);
#endif
