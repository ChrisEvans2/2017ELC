#ifndef _KEY_H
#define _KEY_H
#include "sys.h"

typedef enum
{
    KEY_CHECK = 0,  //按键检测状态
    KEY_COMFIRM,    //按键确认状态
    KEY_RELEASE     //按键释放状态
}keyState_e;        //状态枚举变量

typedef struct
{
  keyState_e keyState; //按键状态
  uint8_t keyFlag;     //按键按下标志
}key_t;                //按键状态结构体

//下面的方式是通过位带操作方式读取IO
//#define KEY0        PEin(4) 	//KEY0按键PE4
//#define KEY1        PEin(3) 	//KEY1按键PE3
//#define WK_UP       PAin(0) 	//WKUP按键PA0

//下面的方式是通过直接操作HAL库函数方式读取IO
#define KEY0        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)  //KEY0按键PE4
#define KEY1        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)  //KEY1按键PE3
#define WK_UP       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //WKUP按键PA0

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define WKUP_PRES   3

void KEY_Init(void);
void Key_Scan(key_t *key, GPIO_TypeDef *key_port, uint16_t key_pin);
void Key_Scan2(key_t* key, GPIO_TypeDef *key_port, uint16_t key_pin);
#endif
