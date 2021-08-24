#include "adc.h"

void ADC1_CH1_Init()
{

	GPIO_InitTypeDef GpioIniture;
	ADC_CommonInitTypeDef AdcIniture;
	ADC_InitTypeDef Adc1Initure;
//	NVIC_InitTypeDef AdcItIniture;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);	//ADC配置复位
	
	GpioIniture.GPIO_Mode = GPIO_Mode_AN;
	GpioIniture.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_5;
	GpioIniture.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GpioIniture);
	
	AdcIniture.ADC_DMAAccessMode    = ADC_DMAAccessMode_1;
	AdcIniture.ADC_Mode 			= ADC_Mode_Independent;				//独立模式（只用一个adc）
	AdcIniture.ADC_Prescaler 		= ADC_Prescaler_Div4;				//adc的时钟频率不得大于36M，此处设为84M / 4
	AdcIniture.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;	//延时周期数 = 采样周期数 + 12（越大越准确）
	ADC_CommonInit(&AdcIniture);
	
	Adc1Initure.ADC_ContinuousConvMode   = ENABLE;							//使能连续转换
	Adc1Initure.ADC_DataAlign			 = ADC_DataAlign_Right;				//右对齐
	Adc1Initure.ADC_ExternalTrigConv	 = ADC_ExternalTrigConv_T1_CC1;		//随便给一个
	Adc1Initure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//外部触发方式（无）
	Adc1Initure.ADC_NbrOfConversion		 = 2;								//规则序列长度
	Adc1Initure.ADC_Resolution 			 = ADC_Resolution_12b;				//分辨率
	Adc1Initure.ADC_ScanConvMode		 = ENABLE;							//使能扫描模式
	ADC_Init(ADC1, &Adc1Initure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_56Cycles);	//转换周期数 = 采样周期数 + 12（越大越准确）
	
	ADC_Cmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);//使能后，只要发生数据转换，就可以发出DMA请求（对于独立模式）

//	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);//规则组转换完成中断
//	
//	AdcItIniture.NVIC_IRQChannel = ADC_IRQn;
//	AdcItIniture.NVIC_IRQChannelCmd = ENABLE;
//	AdcItIniture.NVIC_IRQChannelPreemptionPriority = 1;
//	AdcItIniture.NVIC_IRQChannelSubPriority = 1; 
//	NVIC_Init(&AdcItIniture);
	
}


//void ADC_IRQHandler()
//{
//	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET)
//		val1 = ADC_GetConversionValue(ADC1);
//	ADC_ClearITPendingBit(ADC1, ADC_FLAG_EOC);
//}                                            

/****************DMA模式下不需要ADC中断****************/
