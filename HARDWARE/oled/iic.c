#include "iic.h"

void IIC_Init()
{
	GPIO_InitTypeDef GPIO_Initstructure;	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_Initstructure.GPIO_OType=GPIO_OType_PP;
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_Initstructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);	
	GPIO_Init(GPIOC,&GPIO_Initstructure);
	
	IIC_SCL = 1;
	IIC_SDA = 1;
	
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;  
	delay_us(5);
 	IIC_SDA=0;     //START:when CLK is high,DATA change form high to low 
	delay_us(5);
	IIC_SCL=0;     //ǯסI2C���ߣ�׼�����ͻ�������� 
}	  


void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(5);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(5);							   					   	
}

u8 IIC_Wait_Ack(void)   //�ȴ�������Ӧ��
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(2);   
	IIC_SCL=1;delay_us(2);	
  
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
}

void IIC_Ack(void)  //������Ӧ��
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(5);
	IIC_SCL=1;
	delay_us(5);
	IIC_SCL=0;
}

void IIC_NAck(void)  //��������Ӧ��
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(5);
	IIC_SCL=1;
	delay_us(5);
	IIC_SCL=0;
}


void IIC_Send_Byte(u8 txd)  
{                        
    u8 t;   
	  SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
	 
    for(t=0;t<8;t++)
    {             
			if((txd&0x80)>>7)
        IIC_SDA=1;
			else 
				IIC_SDA=0;
			
        txd<<=1; 	 
			  delay_us(5);
				IIC_SCL=1;
				delay_us(5); 
				IIC_SCL=0;	
				delay_us(5);
    }	 
} 
