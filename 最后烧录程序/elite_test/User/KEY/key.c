#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//KEY��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/9/17
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//������ʼ������
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOAʱ��
    __HAL_RCC_GPIOE_CLK_ENABLE();           //����GPIOEʱ��

    
    GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
	GPIO_Initure.Pin=GPIO_PIN_3|GPIO_PIN_4; //PE3,4
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
    HAL_GPIO_Init(GPIOE,&GPIO_Initure); 
}

// ����ɨ��
void Key_Scan(key_t* key, GPIO_TypeDef *key_port, uint16_t key_pin)
{
	switch(key->keyState)
	{
		case KEY_CHECK:
			// �����͵�ƽ�����밴��ȷ��״̬
			if(HAL_GPIO_ReadPin(key_port, key_pin) ==  GPIO_PIN_RESET)
			{
				key->keyState = KEY_COMFIRM;
			}
			break;
		case KEY_COMFIRM:
			if(HAL_GPIO_ReadPin(key_port, key_pin) ==  GPIO_PIN_RESET)
			{
				//�����͵�ƽ������ȷʵ���£�������־λ��1�������밴���ͷ�״̬
				key->keyState = KEY_RELEASE;
			}
				//�����ߵ�ƽ�������Ǹ����źţ����س�ʼ״̬
			else
			{
				key->keyState = KEY_CHECK;
			}
			break;
		case KEY_RELEASE:
			if(HAL_GPIO_ReadPin(key_port, key_pin) ==  GPIO_PIN_SET)
			{
				key->keyFlag = 1;
			 // �����ߵ�ƽ��˵�������ͷţ����س�ʼ״̬
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
			// �����͵�ƽ�����밴��ȷ��״̬
			if(HAL_GPIO_ReadPin(key_port, key_pin) ==  GPIO_PIN_SET)
			{
				key->keyState = KEY_COMFIRM;
			}
			break;
		case KEY_COMFIRM:
			if(HAL_GPIO_ReadPin(key_port, key_pin) ==  GPIO_PIN_SET)
			{
				//�����͵�ƽ������ȷʵ���£�������־λ��1�������밴���ͷ�״̬
				key->keyState = KEY_RELEASE;
			}
				//�����ߵ�ƽ�������Ǹ����źţ����س�ʼ״̬
			else
			{
				key->keyState = KEY_CHECK;
			}
			break;
		case KEY_RELEASE:
			if(HAL_GPIO_ReadPin(key_port, key_pin) ==  GPIO_PIN_RESET)
			{
				key->keyFlag = 1;
			 // �����ߵ�ƽ��˵�������ͷţ����س�ʼ״̬
			 key->keyState = KEY_CHECK;       
			}
			break;
		default: break;
	}
}

