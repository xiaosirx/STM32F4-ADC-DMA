#include "dma.h"

void Dma_Init(DMA_Stream_TypeDef* DMAy_Streamx, u32 DMA_Channel_x, u32 phaddr, u32 meaddr, u32 BufferSize)
{

	DMA_InitTypeDef DmaIniture;
	NVIC_InitTypeDef Dma_It_Initure;
	
	if((u32)DMAy_Streamx >= (u32)DMA2)
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	else
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	
	DMA_DeInit(DMAy_Streamx);
		
	DmaIniture.DMA_Channel 			  = DMA_Channel_x;						//指定通道
	DmaIniture.DMA_BufferSize		  = BufferSize;							//传输的数据数目
	DmaIniture.DMA_FIFOMode			  = DMA_FIFOMode_Disable;				//失能FIFO
	DmaIniture.DMA_FIFOThreshold	  = DMA_FIFOThreshold_HalfFull;			//FIFO阈值（随便给一个值）
	DmaIniture.DMA_DIR 				  = DMA_DIR_PeripheralToMemory;			//外设到内存
	DmaIniture.DMA_Mode				  = DMA_Mode_Circular;					//循环模式
	DmaIniture.DMA_Priority			  = DMA_Priority_VeryHigh;				//优先级为非常高
	DmaIniture.DMA_Memory0BaseAddr	  = meaddr;								//指定内存地址
	DmaIniture.DMA_PeripheralBaseAddr = phaddr;								//指定外设地址
	DmaIniture.DMA_MemoryDataSize 	  = DMA_MemoryDataSize_HalfWord;		//内存半字（16位）传输
	DmaIniture.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//外设半字传输
	DmaIniture.DMA_MemoryBurst		  = DMA_MemoryBurst_Single;				//内存突发单次传输
	DmaIniture.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;			//外设突发单次传输
	DmaIniture.DMA_MemoryInc		  = DMA_MemoryInc_Enable;				//使能内存地址递增
	DmaIniture.DMA_PeripheralInc	  = DMA_PeripheralInc_Disable;			//失能外设地址递增
	DMA_Init(DMAy_Streamx, &DmaIniture);
	while(DMA_GetCmdStatus(DMAy_Streamx) == ENABLE);//等待DMA配置成功（若参数配置错误，即使使能对应的数据流，仍然处于失能状态）
		
	DMA_ITConfig(DMAy_Streamx, DMA_IT_TC, ENABLE);
	
	Dma_It_Initure.NVIC_IRQChannel 					 = DMA2_Stream0_IRQn;
	Dma_It_Initure.NVIC_IRQChannelCmd 				 = ENABLE;
	Dma_It_Initure.NVIC_IRQChannelPreemptionPriority = 0;
	Dma_It_Initure.NVIC_IRQChannelSubPriority 		 = 0;
	NVIC_Init(&Dma_It_Initure);
		
	DMA_Cmd(DMAy_Streamx, ENABLE);
	
	
}

void Dma_Switch_Stream(DMA_Stream_TypeDef* DMAy_Streamx, u32 BufferSize)//改变数据流和传输数据个数
{
	DMA_Cmd(DMAy_Streamx, DISABLE);
	while(DMA_GetCmdStatus(DMAy_Streamx) == ENABLE);//等待DMA可配置
	DMA_SetCurrDataCounter(DMAy_Streamx, BufferSize);
	DMA_Cmd(DMAy_Streamx, ENABLE);
}

extern u16 data[2];
void DMA2_Stream0_IRQHandler()
{
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)//判断DMA中断是否发生
	{
		OLED_ShowNum(63, 3, data[0], 4, 16);
		OLED_ShowNum(63, 5, data[1], 4, 16);
	}
	DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TC);//清除中断标志位
}

