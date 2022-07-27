//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : Evk123
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : 0.69��OLED �ӿ���ʾ����(STM32F103ZEϵ��IIC)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PD9��SCL��
//              SDA   ��PE15��SDA��            
//              ----------------------------------------------------------------
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////�

#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"

//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
/**********************************************
//IIC Start
**********************************************/
/**********************************************
//IIC Start
**********************************************/


//IIC���ߵĳ�ʼ��  SCL--PB8  SDA--PB9
void IIC_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;			//���ģʽ
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;			//�������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;		//�������
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;			//��������
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;				//���ű��
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;				//���ű��
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	

	//3.����״̬��SDA��SCL��Ϊ�ߵ�ƽ
	IIC_SCL_WRITE(1);
	IIC_SDA_WRITE(1);
	
}	


void IIC_SDAOutputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//1.��GPIO����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	
	//2.����GPIO����
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;									//���ű��
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;								//���ģʽ
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;								//�������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;							//�������
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;									//��������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	

}

void IIC_SDAInputMode(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//1.��GPIO����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	
	//2.����GPIO����
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;									//���ű��
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;									//����ģʽ
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;									//��������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	

}

//IIC���߿�ʼ�ź�
void IIC_StartSignal(void)
{
	//1.����SDA����Ϊ���ģʽ
	IIC_SDAOutputMode();
	
	//2.ȷ��SDA��SCLΪ�ߵ�ƽ
	IIC_SCL_WRITE(1);
	IIC_SDA_WRITE(1);

    delay_us(5);            //��߳���ɿ���
    
	//3.��SDA��������
	IIC_SDA_WRITE(0);

	delay_us(5);   // IIC���ߵ�ͨ������Ϊ100KHZ  1000000us = 100000HZ  10us = 1HZ 
	
	//4.��SCL�������ͣ���ʾ׼��ͨ��
	IIC_SCL_WRITE(0);
}

//IIC����ֹͣ�ź�
void IIC_StopSignal(void)
{
	//1.����SDA����Ϊ���ģʽ
	IIC_SDAOutputMode();

	//2.ȷ��SDA��SCLΪ�͵�ƽ
	IIC_SCL_WRITE(0);
	IIC_SDA_WRITE(0);
	
	//4.��SCL��������
	IIC_SCL_WRITE(1);
	delay_us(5);
	
	//5.��SDA��������
	IIC_SDA_WRITE(1);
	delay_us(5);  //ȷ��SDA�ĵ�ƽ״̬���Ա�������������⵽
}

//�����ȴ��ӻ�Ӧ��  ����0 ˵���ӻ�Ӧ��   ����1 ˵���ӻ�ûӦ��
uint8_t IIC_WaitAck(void)
{
	uint8_t ack;
	
	//1.����SDA����Ϊ����ģʽ
	IIC_SDAInputMode();

	//2.SCLΪ�͵�ƽ      �ӻ�׼������
	IIC_SCL_WRITE(0);
	delay_us(5);
	
	//3.SCLΪ�ߵ�ƽ      �ӻ���������
	IIC_SCL_WRITE(1);
	delay_us(5);
	
	//�����жϴӻ�������
	if( IIC_SDA_READ == 1)
	{
		ack=1; //˵����Ӧ��
	}
	else
		ack=0; //˵����Ӧ��
	
	//4.SCLΪ�͵�ƽ       ������������
	IIC_SCL_WRITE(0);
	delay_us(5);
	
	return ack;
}

//�����������ݣ��ӻ���ȡ���ݣ�
void IIC_SendBytes(uint8_t Data)  
{
	uint8_t i= 0;
	
	//1.����SDA����Ϊ���ģʽ
	IIC_SDAOutputMode();
	
	//2.ȷ��SDA��SCLΪ�͵�ƽ
	IIC_SCL_WRITE(0);
	IIC_SDA_WRITE(0);
	
	//3.ѭ������bit
	for(i=0;i<8;i++)
	{
		//MSB ��λ�ȳ�   ����׼������
		if ( Data & 1<<(7-i) ) 
		{
			IIC_SDA_WRITE(1);
		}
		else
			IIC_SDA_WRITE(0);
		
		delay_us(5);
		
		//SCLΪ�ߵ�ƽ    ������������
		IIC_SCL_WRITE(1);
		delay_us(5);
		
		//SCLΪ�͵�ƽ    ����׼������
		IIC_SCL_WRITE(0);
		delay_us(5);
	}
}


/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_StartSignal();
	IIC_SendBytes(0x78);            //Slave address,SA0=0
	IIC_WaitAck();	
	IIC_SendBytes(0x00);			//write command
	IIC_WaitAck();	
	IIC_SendBytes(IIC_Command); 
	IIC_WaitAck();	
	IIC_StopSignal();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
	IIC_StartSignal();
	IIC_SendBytes(0x78);			//D/C#=0; R/W#=0
	IIC_WaitAck();	
	IIC_SendBytes(0x40);			//write data
	IIC_WaitAck();	
	IIC_SendBytes(IIC_Data);
	IIC_WaitAck();	
	IIC_StopSignal();
}




void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
			{

   Write_IIC_Data(dat);
   
		}
	else {
   Write_IIC_Command(dat);
		
	}


}


/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Byte(fill_Data,1);
			}
	}
}


/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}

//��������

	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
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
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//��ʾ����
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

//��ʼ��SSD1306	  SCL  PD9   SDA PE15				    
void OLED_Init(void)
{ 	
 

	GPIO_InitTypeDef   GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;			//���ģʽ
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;			//�������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;		//�������
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;			//��������
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;				//���ű��
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;				//���ű��
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOD,GPIO_Pin_9);
	GPIO_SetBits(GPIOE,GPIO_Pin_15);

	delay_ms(200);

	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  




























