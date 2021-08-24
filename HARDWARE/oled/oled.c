#include "oled.h"
#include "oledfont.h"

void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   IIC_Send_Byte(0x78);            //Slave address,SA0=0
	if(IIC_Wait_Ack())
	{
		while(1);
	}
   IIC_Send_Byte(0x00);			//write command
	IIC_Wait_Ack();	
   IIC_Send_Byte(IIC_Command); 
	IIC_Wait_Ack();	
   IIC_Stop();
}

void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   IIC_Send_Byte(0x78);			//D/C#=0; R/W#=0
   IIC_Wait_Ack();	
   IIC_Send_Byte(0x40);			//write data
   IIC_Wait_Ack();	
   IIC_Send_Byte(IIC_Data);
   IIC_Wait_Ack();	
   IIC_Stop();
}

void OLED_WR_Byte(uint8_t dat,uint8_t cmd)  
{	
	if(cmd)
	  Write_IIC_Data(dat);
	else
	  Write_IIC_Command(dat);
}


//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14, OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD);  //DISPLAY ON
}

//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}	

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}

void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //������ʾ
}

/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m = 0; m < 8; m ++)
	{
		OLED_WR_Byte(0xb0 + m, 0);		//page0-page7
		OLED_WR_Byte(0x00, 0);		//low column start address
		OLED_WR_Byte(0x10, 0);		//high column start address
		for(n = 0; n < 128; n ++)
			OLED_WR_Byte(fill_Data, 1);
	}
}

//�������� 
//x 0~127
//y 0~7
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xb0 + y, OLED_CMD);
	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte((x & 0x0f), OLED_CMD); 
} 

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~7
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16(16*8)/8(8*6)
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c = 0, i = 0;	
	c = chr - ' ';//�õ�ƫ�ƺ��ֵ			
	if(x > Max_Column - 1){x = 0; y = y + 2;}
	if(Char_Size == 16){
		OLED_Set_Pos(x, y);	
		for(i = 0; i < 8; i ++)
			OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
		OLED_Set_Pos(x, y + 1);
		for(i = 0; i < 8; i ++)
			OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
	}
	else {	
		OLED_Set_Pos(x, y);
		for(i = 0; i < 6; i ++)
			OLED_WR_Byte(F6x8[c][i], OLED_DATA);	
	}
}

//����m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}	

//��ʾ����
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С		8 / 16
//mode:ģʽ	0,���ģʽ:1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size)
{         	
	u8 t, temp;
	u8 enshow = 0;						   
	for(t = 0; t < len; t ++)
	{
		temp = (num / oled_pow (10, len - t - 1)) % 10;
		if(enshow == 0 && t < (len - 1))
		{
			if(temp == 0)
			{
				OLED_ShowChar(x + (size / 2) * t, y , ' ', size);
				continue;
			}else enshow = 1; 		 	 
		}
	 	OLED_ShowChar(x + (size / 2) * t, y, temp + '0', size); 
	}
} 

//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 Char_Size)
{
	unsigned char j = 0;
	while (chr[j] != '\0')
	{		
		OLED_ShowChar(x, y, chr[j], Char_Size);
		x += 8;
		if(x > 120){x = 0;y += 2;}
		j ++;
	}
}

//��ʾ����(16*16)
//noΪ��Ҫ��ʾ�ĺ������ֿ��е�λ��
void OLED_ShowCHinese(u8 x, u8 y, u8 no)
{      			    
	u8 t, adder = 0;
	OLED_Set_Pos(x, y);	
    for(t = 0;t < 16;t ++)
	{
		OLED_WR_Byte(Hzk[2 * no][t], OLED_DATA);
		adder += 1;
     }	
	OLED_Set_Pos(x, y + 1);	
    for(t = 0;t < 16;t ++)
	{	
		OLED_WR_Byte(Hzk[2 * no + 1][t], OLED_DATA);
		adder += 1;
    }					
}

/***********��ʾBMPͼƬ128��64*****************/
//x0,x1   0~127
//y0,y1   0~63
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{ 	
	unsigned int j = 0;
	unsigned char x, y; 
	if(y1 % 8 == 0) y = y1 / 8;      
	else y = y1 / 8 + 1;
	for(y = y0; y < y1; y ++)
	{
		OLED_Set_Pos(x0, y);
		for(x = x0; x < x1; x ++)    
	    	OLED_WR_Byte(BMP[j ++], OLED_DATA);	    	
	}
} 

//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
	IIC_Init();
	delay_ms(500);
			  
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  
