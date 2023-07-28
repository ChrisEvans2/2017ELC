#ifndef __KEY_H
#define __KEY_H

#include "gpio.h"
#include "stdint.h"

// 正点原子
#define Key0    HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)
#define WK_UP   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)
#define Key1    HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define WKUP_PRES   3

//// 野火
//#define Key0    HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)
//#define Key1    HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)

//#define KEY0_PRES 	1
//#define KEY1_PRES	2



void KEY_Init();
uint8_t KEY_Scan(uint8_t mode);


#endif 