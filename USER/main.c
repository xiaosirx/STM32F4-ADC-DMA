#include "stm32f4xx.h"
#include "oled.h"
#include "adc.h"

u16 data[2];

int main()
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	ADC1_CH1_Init();
	Dma_Init(DMA2_Stream0, DMA_Channel_0, 0x40012000 + 0x4C, (u32)data, 2);
	OLED_Init();
	OLED_Clear();
	ADC_SoftwareStartConv(ADC1);
	while(1);
}
