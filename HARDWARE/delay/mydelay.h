#ifndef _MYDELAY_H
#define _MYDELAY_H

#include "stm32f4xx.h"

void Delay_Init(void);
void delay_us(u16 nus);
void delay_ms(u16 nms);

#endif
