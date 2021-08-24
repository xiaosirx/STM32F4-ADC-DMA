#ifndef _IIC_H
#define _IIC_H

#include "stm32f4xx.h"
#include "mydelay.h"

/***************************************************/
/*         IO�������ο�������ԭ�ӵ�sys.h���룩	   */
/***************************************************/

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOC->MODER|=0<<9*2;}	//PB11����ģʽ
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOC->MODER|=1<<9*2;} //PB11���ģʽ
//IO��������
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

/***************************************************/

#define IIC_SCL    PAout(8) //SCL
#define IIC_SDA    PCout(9) //SDA	 
#define READ_SDA   PCin(9)  //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
//u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

#endif 
