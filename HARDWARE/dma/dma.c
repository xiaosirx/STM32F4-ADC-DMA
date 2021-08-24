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
		
	DmaIniture.DMA_Channel 			  = DMA_Channel_x;						//ָ��ͨ��
	DmaIniture.DMA_BufferSize		  = BufferSize;							//�����������Ŀ
	DmaIniture.DMA_FIFOMode			  = DMA_FIFOMode_Disable;				//ʧ��FIFO
	DmaIniture.DMA_FIFOThreshold	  = DMA_FIFOThreshold_HalfFull;			//FIFO��ֵ������һ��ֵ��
	DmaIniture.DMA_DIR 				  = DMA_DIR_PeripheralToMemory;			//���赽�ڴ�
	DmaIniture.DMA_Mode				  = DMA_Mode_Circular;					//ѭ��ģʽ
	DmaIniture.DMA_Priority			  = DMA_Priority_VeryHigh;				//���ȼ�Ϊ�ǳ���
	DmaIniture.DMA_Memory0BaseAddr	  = meaddr;								//ָ���ڴ��ַ
	DmaIniture.DMA_PeripheralBaseAddr = phaddr;								//ָ�������ַ
	DmaIniture.DMA_MemoryDataSize 	  = DMA_MemoryDataSize_HalfWord;		//�ڴ���֣�16λ������
	DmaIniture.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//������ִ���
	DmaIniture.DMA_MemoryBurst		  = DMA_MemoryBurst_Single;				//�ڴ�ͻ�����δ���
	DmaIniture.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;			//����ͻ�����δ���
	DmaIniture.DMA_MemoryInc		  = DMA_MemoryInc_Enable;				//ʹ���ڴ��ַ����
	DmaIniture.DMA_PeripheralInc	  = DMA_PeripheralInc_Disable;			//ʧ�������ַ����
	DMA_Init(DMAy_Streamx, &DmaIniture);
	while(DMA_GetCmdStatus(DMAy_Streamx) == ENABLE);//�ȴ�DMA���óɹ������������ô��󣬼�ʹʹ�ܶ�Ӧ������������Ȼ����ʧ��״̬��
		
	DMA_ITConfig(DMAy_Streamx, DMA_IT_TC, ENABLE);
	
	Dma_It_Initure.NVIC_IRQChannel 					 = DMA2_Stream0_IRQn;
	Dma_It_Initure.NVIC_IRQChannelCmd 				 = ENABLE;
	Dma_It_Initure.NVIC_IRQChannelPreemptionPriority = 0;
	Dma_It_Initure.NVIC_IRQChannelSubPriority 		 = 0;
	NVIC_Init(&Dma_It_Initure);
		
	DMA_Cmd(DMAy_Streamx, ENABLE);
	
	
}

void Dma_Switch_Stream(DMA_Stream_TypeDef* DMAy_Streamx, u32 BufferSize)//�ı��������ʹ������ݸ���
{
	DMA_Cmd(DMAy_Streamx, DISABLE);
	while(DMA_GetCmdStatus(DMAy_Streamx) == ENABLE);//�ȴ�DMA������
	DMA_SetCurrDataCounter(DMAy_Streamx, BufferSize);
	DMA_Cmd(DMAy_Streamx, ENABLE);
}

extern u16 data[2];
void DMA2_Stream0_IRQHandler()
{
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)//�ж�DMA�ж��Ƿ���
	{
		OLED_ShowNum(63, 3, data[0], 4, 16);
		OLED_ShowNum(63, 5, data[1], 4, 16);
	}
	DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TC);//����жϱ�־λ
}

