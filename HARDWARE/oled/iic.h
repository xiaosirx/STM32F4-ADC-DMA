#ifndef _IIC_H
#define _IIC_H

#include "stm32f4xx.h"
#include "mydelay.h"

/***************************************************/
/*         IO操作（参考了正点原子的sys.h代码）	   */
/***************************************************/

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOC->MODER|=0<<9*2;}	//PB11输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOC->MODER|=1<<9*2;} //PB11输出模式
//IO操作函数
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

/***************************************************/

#define IIC_SCL    PAout(8) //SCL
#define IIC_SDA    PCout(9) //SDA	 
#define READ_SDA   PCin(9)  //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
//u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

#endif 
