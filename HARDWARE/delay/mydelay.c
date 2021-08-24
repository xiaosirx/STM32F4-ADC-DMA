
/********���侫ȷ����ʱ*********/

#include "mydelay.h"

RCC_ClocksTypeDef RCC_Clocks;

void delay_us(u16 nus)
{   
	 u32 tickFreq,tick;   
	 RCC_GetClocksFreq(&RCC_Clocks);
     tickFreq = RCC_Clocks.SYSCLK_Frequency / 8; 	 		//�õ���ǰsystickƵ�ʣ���Ƶ�İ˷�֮һ��
	 tick = tickFreq / 1000000;
     SysTick -> LOAD = tick * (nus - 1);  			//�õ�nus��Ҫ��tick��������������Ϊ��װ��ֵ
	 SysTick -> VAL = 0; 									//����������ֵ����
	 SysTick -> CTRL |= 0x0001;							    //��systick��ʱ��
	 while(!(SysTick -> CTRL>>15));  						//�ȴ���ʱ��ɣ�������ֵΪ0ʱ��λ�Զ���1��
	 SysTick -> CTRL &= 0x7fff;  							//�رն�ʱ��
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
