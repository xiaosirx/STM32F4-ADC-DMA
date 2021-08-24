
/********极其精确的延时*********/

#include "mydelay.h"

RCC_ClocksTypeDef RCC_Clocks;

void delay_us(u16 nus)
{   
	 u32 tickFreq,tick;   
	 RCC_GetClocksFreq(&RCC_Clocks);
     tickFreq = RCC_Clocks.SYSCLK_Frequency / 8; 	 		//得到当前systick频率（主频的八分之一）
	 tick = tickFreq / 1000000;
     SysTick -> LOAD = tick * (nus - 1);  			//得到nus需要的tick数量，并将其作为重装载值
	 SysTick -> VAL = 0; 									//将计数器数值清零
	 SysTick -> CTRL |= 0x0001;							    //打开systick定时器
	 while(!(SysTick -> CTRL>>15));  						//等待延时完成（计数器值为0时此位自动置1）
	 SysTick -> CTRL &= 0x7fff;  							//关闭定时器
}

void delay_ms(u16 nms)
{
     u32 tickFreq,tick;   
	 RCC_GetClocksFreq(&RCC_Clocks);
     tickFreq = RCC_Clocks.SYSCLK_Frequency / 8; 
	 tick = tickFreq / 1000;
     SysTick -> LOAD = tick * nms;
	 SysTick -> VAL = 0;
	 SysTick -> CTRL |= 0x0001;
	 while(!(SysTick -> CTRL>>15));  
	 SysTick -> CTRL &= 0x7fff;
}
