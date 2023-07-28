#include "delay.h"
TIM_HandleTypeDef delay;


void delay_us(uint16_t time_us)
{
	delay.Instance = TIM7;
	__HAL_TIM_SET_COUNTER(&delay, 0);
	delay.State = HAL_TIM_STATE_READY;

	HAL_TIM_Base_Start(&delay);
	while(__HAL_TIM_GetCounter(&delay)<time_us);
	HAL_TIM_Base_Stop(&delay);
}

void delay_ms(uint16_t time_ms)
{
	for(uint16_t i=0; i<time_ms; i++)
	{
		delay_us(1000);
	}	
}
