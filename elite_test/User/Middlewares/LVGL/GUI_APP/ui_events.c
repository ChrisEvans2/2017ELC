// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.1
// LVGL version: 8.3.6
// Project name: my_test

#include "ui.h"
#include "stm32f1xx_it.h"
#include "adc.h"
#include "dac.h"
#include "ad9854.h"
static uint32_t DDS_Fre = 1000;
static double DDS_Fre_1M = 1000026.313;
double DDS_Fre_test;
static uint16_t DDS_Vol = 810;
static uint32_t DDS_FreRange = 100000;
extern uint8_t DDS_Sweep_state;

void DDS_FreValueSet(lv_event_t * e)
{
	// Your code here
	DDS_Fre = lv_spinbox_get_value(ui_Fre_Value)*DDS_FreRange;
	DDS_Fre_test = DDS_Fre*DDS_Fre_1M/1000000;
	printf("DDS_Fre:%f\r\n", DDS_Fre_test);
	if(!DDS_Sweep_state)
	{
//		AD9854_SetSine(DDS_Fre, DDS_Vol);
		AD9854_SetSine_double(DDS_Fre_test, DDS_Vol);
	}
}

void DDS_FreRangeSet(lv_event_t * e)
{
	// Your code here
	if(lv_dropdown_get_selected(ui_DDS_Fre_Range) == 0)
	{
		DDS_FreRange = 100000;
		DDS_Fre = lv_spinbox_get_value(ui_Fre_Value)*DDS_FreRange;

		if(!DDS_Sweep_state)
		{
			AD9854_SetSine(DDS_Fre, DDS_Vol);
			printf("DDS_Fre:%d", DDS_Fre);
		}
	}else if(lv_dropdown_get_selected(ui_DDS_Fre_Range) == 1){
		DDS_FreRange = 100;
		DDS_Fre = lv_spinbox_get_value(ui_Fre_Value)*DDS_FreRange;
		if(!DDS_Sweep_state)
		{
			AD9854_SetSine(DDS_Fre, DDS_Vol);
		}
	}else if(lv_dropdown_get_selected(ui_DDS_Fre_Range) == 2){
		DDS_FreRange = 10;
		DDS_Fre = lv_spinbox_get_value(ui_Fre_Value)/10;
		if(!DDS_Sweep_state)
		{
			AD9854_SetSine(DDS_Fre, DDS_Vol);
		}
	}
}

void DDS_OutputSet(lv_event_t * e)
{
	// Your code here
	DDS_Vol = lv_slider_get_value(ui_DDS_Vol_Slider);
	if(!DDS_Sweep_state)
	{
		AD9854_SetSine(DDS_Fre, DDS_Vol);
	}
}

void DDS_Output(lv_event_t * e)
{
	// Your code here
}

void measure_FFT(lv_event_t * e)
{
	// Your code here
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_Array, ADC_NUM);
}

void DAC_OutputSet(lv_event_t * e)
{
	// Your code here
	uint16_t dacval;
	lv_obj_t * slider = lv_event_get_target(e);
	dacval = (uint16_t)lv_slider_get_value(slider);
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,dacval);//����DACֵ
	ADC_DAC_show();
}

void DDS_Sweep(lv_event_t * e)
{
	// Your code here
	if(lv_obj_has_state(ui_Sweep_Switch, LV_STATE_CHECKED))
	{
		DDS_Sweep_state = 1;
		delay_ms(5);
	}else{
		DDS_Sweep_state = 0;
		delay_ms(5);
		AD9854_SetSine(DDS_Fre, DDS_Vol);
	}
}

void DDS_calibrate(lv_event_t * e)
{
	// Your code here
	DDS_Fre_1M = 1000000.0 + (double)(lv_spinbox_get_value(ui_Spinbox1)/10000.0);
	DDS_Fre_test = DDS_Fre*DDS_Fre_1M/1000000;
	if(!DDS_Sweep_state)
	{
		AD9854_SetSine_double(DDS_Fre_test, DDS_Vol);
	}

	printf("DDS_Fre_1M:%f\r\n", DDS_Fre_1M);
	printf("DDS_Fre:%f\r\n", DDS_Fre_test);
}
