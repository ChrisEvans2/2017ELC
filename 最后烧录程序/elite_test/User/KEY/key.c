#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//KEY驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/9/17
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//按键初始化函数
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();           //开启GPIOE时钟

    
    GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
	GPIO_Initure.Pin=GPIO_PIN_3|GPIO_PIN_4; //PE3,4
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure); 
}

// 按键扫描
void Key_Scan(key_t* key, GPIO_TypeDef *key_port, uint16_t key_pin)
{
	switch(key->keyState)
	{
		case KEY_CHECK:
			// 读到低电平，进入按键确认状态
			if(HAL_GPIO_ReadPin(key_port, key_pin) ==  GPIO_PIN_RESET)
			{
				key->keyState = KEY_COMFIRM;
			}
			break;
		case KEY_COMFIRM:
			if(HAL_GPIO_ReadPin(key_port, key_pin) ==  GPIO_PIN_RESET)
			{
				//读到低电平，按键确实按下，按键标志位置1，并进入按键释放状态
				key->keyState = KEY_RELEASE;
			}
				//读到高电平，可能是干扰信号，返回初始状态
			else
			{
				key->keyState = KEY_CHECK;
			}
			break;
		case KEY_RELEASE:
			if(HAL_GPIO_ReadPin(key_port, key_pin) ==  GPIO_PIN_SET)
			{
				key->keyFlag = 1;
			 // 读到高电平，说明按键释放，返回初始状态
			 key->keyState = KEY_CHECK;       
			}
			break;
		default: break;
	}
}

void Key_Scan2(key_t* key, GPIO_TypeDef *key_port, uint16_t key_pin)
{
	switch(key->keyState)
	{
		case KEY_CHECK:
			// 读到低电平，进入按键确认状态
			if(HAL_GPIO_ReadPin(key_port, key_pin) ==  GPIO_PIN_SET)
			{
				key->keyState = KEY_COMFIRM;
			}
			break;
		case KEY_COMFIRM:
			if(HAL_GPIO_ReadPin(key_port, key_pin) ==  GPIO_PIN_SET)
			{
				//读到低电平，按键确实按下，按键标志位置1，并进入按键释放状态
				key->keyState = KEY_RELEASE;
			}
				//读到高电平，可能是干扰信号，返回初始状态
			else
			{
				key->keyState = KEY_CHECK;
			}
			break;
		case KEY_RELEASE:
			if(HAL_GPIO_ReadPin(key_port, key_pin) ==  GPIO_PIN_RESET)
			{
				key->keyFlag = 1;
			 // 读到高电平，说明按键释放，返回初始状态
			 key->keyState = KEY_CHECK;       
			}
			break;
		default: break;
	}
}

