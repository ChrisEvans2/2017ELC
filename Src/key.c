#include "key.h"
#include "delay.h"

//������ʼ������
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOAʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();           //����GPIOEʱ��

    
    GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����    ����ԭ��
//    GPIO_Initure.Pull=GPIO_NOPULL;           // Ұ����Ҫ�κβ���
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
	GPIO_Initure.Pin=GPIO_PIN_3|GPIO_PIN_4; //PE3,4
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����      ����ԭ��
//    GPIO_Initure.Pull=GPIO_NOPULL;             // Ұ����Ҫ�κβ���
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
    
}

// ����ԭ��
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��WKUP���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>WK_UP!!
uint8_t KEY_Scan(uint8_t mode)
{
    static uint8_t key_up=1;     //�����ɿ���־
    if(mode==1)key_up=1;    //֧������
    if(key_up&&(Key0==0||Key1==0||WK_UP==1))
    {
        Delay_ms(10);
        key_up=0;
        if(Key0==0)       return KEY0_PRES;
        else if(Key1==0)  return KEY1_PRES;
        else if(WK_UP==1) return WKUP_PRES;          
    }else if(Key0==1&&Key1==1&&WK_UP==0)key_up=1;
    return 0;   //�ް�������
}

//// Ұ��
//uint8_t KEY_Scan(uint8_t mode)
//{
//    static uint8_t key_up=1;     //�����ɿ���־
//    if(mode==1)key_up=1;    //֧������
//    if(key_up&&(Key0==1||Key1==1))
//    {
//        Delay_ms(10);
//        key_up=0;
//        if(Key0==1)       return KEY0_PRES;
//        else if(Key1==1)  return KEY1_PRES;          
//    }else if(Key0==0&&Key1==0)key_up=1;
//    return 0;   //�ް�������
//}
