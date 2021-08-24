#ifndef _DMA_H
#define _DMA_H

#include "stm32f4xx.h"
#include "oled.h"

void Dma_Init(DMA_Stream_TypeDef* DMAy_Streamx, u32 DMA_Channel_x, u32 phaddr, u32 meaddr, u32 BufferSize);
void Dma_Switch_Stream(DMA_Stream_TypeDef* DMAy_Streamx, u32 BufferSize);

#endif
