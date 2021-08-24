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
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);	//ADC���ø�λ
	
	GpioIniture.GPIO_Mode = GPIO_Mode_AN;
	GpioIniture.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_5;
	GpioIniture.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GpioIniture);
	
	AdcIniture.ADC_DMAAccessMode    = ADC_DMAAccessMode_1;
	AdcIniture.ADC_Mode 			= ADC_Mode_Independent;				//����ģʽ��ֻ��һ��adc��
	AdcIniture.ADC_Prescaler 		= ADC_Prescaler_Div4;				//adc��ʱ��Ƶ�ʲ��ô���36M���˴���Ϊ84M / 4
	AdcIniture.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;	//��ʱ������ = ���������� + 12��Խ��Խ׼ȷ��
	ADC_CommonInit(&AdcIniture);
	
	Adc1Initure.ADC_ContinuousConvMode   = ENABLE;							//ʹ������ת��
	Adc1Initure.ADC_DataAlign			 = ADC_DataAlign_Right;				//�Ҷ���
	Adc1Initure.ADC_ExternalTrigConv	 = ADC_ExternalTrigConv_T1_CC1;		//����һ��
	Adc1Initure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//�ⲿ������ʽ���ޣ�
	Adc1Initure.ADC_NbrOfConversion		 = 2;								//�������г���
	Adc1Initure.ADC_Resolution 			 = ADC_Resolution_12b;				//�ֱ���
	Adc1Initure.ADC_ScanConvMode		 = ENABLE;							//ʹ��ɨ��ģʽ
	ADC_Init(ADC1, &Adc1Initure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_56Cycles);	//ת�������� = ���������� + 12��Խ��Խ׼ȷ��
	
	ADC_Cmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);//ʹ�ܺ�ֻҪ��������ת�����Ϳ��Է���DMA���󣨶��ڶ���ģʽ��

//	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);//������ת������ж�
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

/****************DMAģʽ�²���ҪADC�ж�****************/
