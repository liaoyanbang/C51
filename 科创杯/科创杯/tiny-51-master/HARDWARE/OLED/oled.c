#include "oled.h"
#include "oledfont.h"  
 /**************************************************************************************
    �������ƣ�void IIC_Start()
    �������ܣ�IIC_Start
    ��ڲ�������
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/
void IIC_Start()
{
	OLED_SCLK_Set();
//	_nop_();
	OLED_SDIN_Set();
//	_nop_();
//	_nop_();
//	_nop_();
//	_nop_();
//	_nop_();
	OLED_SDIN_Clr();
//	_nop_();
//	_nop_();
//	_nop_();
//	_nop_();
	OLED_SCLK_Clr();
//	_nop_();
//	_nop_();
}

 /**************************************************************************************
    �������ƣ�void IIC_Stop()
    �������ܣ�IIC_Stop
    ��ڲ�������
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/
void IIC_Stop()
{
	OLED_SCLK_Set();
//	_nop_();
	OLED_SDIN_Clr();
//	_nop_();
//	_nop_();
//	_nop_();
//	_nop_();
//	_nop_();
	OLED_SDIN_Set();
//	_nop_();
//	_nop_();
//	_nop_();
//	_nop_();
}

void IIC_Wait_Ack()
{
	OLED_SCLK_Set();
//	_nop_();
//	_nop_();
	OLED_SCLK_Clr();
//	_nop_();
//	_nop_();
}

 /**************************************************************************************
    �������ƣ�void Write_IIC_Byte(unsigned char IIC_Byte)
    �������ܣ�IICд���ֽ�
    ��ڲ�����unsigned char IIC_Byte
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
    unsigned char i;
		unsigned char m,da;
		da=IIC_Byte;
		OLED_SCLK_Clr();
		for(i=0;i<8;i++)		
		{
				m=da;
			  m=m&0x80;
			 if(m==0x80)
			 {
			     OLED_SDIN_Set();
			 }
			 else OLED_SDIN_Clr();
			da=da<<1;
//			 _nop_();
//			 _nop_();
			OLED_SCLK_Set();
//			 _nop_();
//			 _nop_();
			OLED_SCLK_Clr();
//			 _nop_();
//			 _nop_();
	  }
}

 /**************************************************************************************
    �������ƣ�void Write_IIC_Command(unsigned char IIC_Command)
    �������ܣ�IICд������
    ��ڲ�����unsigned char IIC_Command
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
    IIC_Start();
    Write_IIC_Byte(0x78);                 //SA0=0
	  IIC_Wait_Ack();	
    Write_IIC_Byte(0x00);			            //д����
	  IIC_Wait_Ack();	
    Write_IIC_Byte(IIC_Command); 
	  IIC_Wait_Ack();	
    IIC_Stop();
}
 /**************************************************************************************
    �������ƣ�void Write_IIC_Data(unsigned char IIC_Data)
    �������ܣ�IICд������
    ��ڲ�����unsigned char IIC_Data
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
    IIC_Start();
		Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
		IIC_Wait_Ack();	
		Write_IIC_Byte(0x40);			//write data
		IIC_Wait_Ack();	
		Write_IIC_Byte(IIC_Data);
		IIC_Wait_Ack();	
		IIC_Stop();
}
 /**************************************************************************************
    �������ƣ�void OLED_WR_Byte(unsigned dat,unsigned cmd)
    �������ܣ�ѡ��IICд�����ݻ�����
    ��ڲ�����unsigned dat,unsigned cmd
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
    if(cmd)
		{
				 Write_IIC_Data(dat);
		}
		else 
		{
				Write_IIC_Command(dat);
		}
}

 
/**************************************************************************************
    �������ƣ�void OLED_Set_Pos(unsigned char x, unsigned char y) 
    �������ܣ�����λ��
    ��ڲ�����unsigned char x, unsigned char y
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/		
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
		OLED_WR_Byte(0xb0+y,OLED_CMD);
		OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
		OLED_WR_Byte((x&0x0f),OLED_CMD); 
} 

/**************************************************************************************
    �������ƣ�void OLED_Set_Pos(unsigned char x, unsigned char y) 
    �������ܣ�OLED��ʾ
    ��ڲ�������
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/		
void OLED_Display_On(void)
{
		OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
		OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
		OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/**************************************************************************************
    �������ƣ�void OLED_Display_Off(void)
    �������ܣ��ر�OLED��ʾ
    ��ڲ�������
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/		
void OLED_Display_Off(void)
{
		OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
		OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
		OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}	

/**************************************************************************************
    �������ƣ�void OLED_Clear(void) 
    �������ܣ���������,������,������Ļ�Ǻ�ɫ
    ��ڲ�������
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/		
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

/**************************************************************************************
    �������ƣ�void OLED_On(void) 
    �������ܣ�����OLED
    ��ڲ�������
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/		
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

/**************************************************************************************
    �������ƣ�void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
    �������ܣ���ָ��λ����ʾһ���ַ�,���������ַ�
    ��ڲ�����x:0~127
              y:0~63
              mode:0,������ʾ;1,������ʾ				 
              size:ѡ������ 16/12  
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/		
void OLED_ShowChar (u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
		unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
		{
//				OLED_Set_Pos(x,y);	
//				for(i=0;i<8;i++)
//				OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
//				OLED_Set_Pos(x,y+1);
//				for(i=0;i<8;i++)
//				OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		}
		else
		{	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
		}
}

/**************************************************************************************
    �������ƣ�u32 oled_pow(u8 m,u8 n)
    �������ܣ�m^n����
    ��ڲ�����u8 m,u8 n  
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/		
u32 oled_pow(u8 m,u8 n)
{
		u32 result=1;	 
		while(n--)result*=m;    
		return result;
	
}	

/**************************************************************************************
    �������ƣ�void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
    �������ܣ���ʾ2������
    ��ڲ�����x,y :�������	 
              len :���ֵ�λ��
              size:�����С
              mode:ģʽ	0,���ģʽ;1,����ģʽ
              num:��ֵ(0~4294967295);	
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/		
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 Num_size)
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
							OLED_ShowChar(x+(Num_size/2)*t,y,' ',Num_size);
							continue;
						}
						else enshow=1; 				 
			  }
				OLED_ShowChar(x+(Num_size/2)*t,y,temp+'0',Num_size); 
		}
} 

/**************************************************************************************
    �������ƣ�void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
    �������ܣ���ʾһ���ַ��Ŵ�
    ��ڲ�����u8 x,u8 y,u8 *chr,u8 Char_Size
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/		
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
		unsigned char j=0;
		while (chr[j]!='\0')
		{		
		    OLED_ShowChar(x,y,chr[j],Char_Size);
				x+=8;
				if(x>120)
        {
				    x=0;
				    y+=2;
				}
				j++;
		}
}

/**************************************************************************************
    �������ƣ�void OLED_ShowCHinese(u8 x,u8 y,u8 no)
    �������ܣ���ʾ����
    ��ڲ�����u8 x,u8 y,u8 no
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/		
//void OLED_ShowCHinese(u8 x,u8 y,u8 no)
//{      			    
//		u8 t,adder=0;
//		OLED_Set_Pos(x,y);	
//		for(t=0;t<16;t++)
//		{
//				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
//				adder+=1;
//		 }	
//		OLED_Set_Pos(x,y+1);	
//		for(t=0;t<16;t++)
//		{	
//				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
//				adder+=1;
//		}					
//}


/**************************************************************************************
    �������ƣ�void OLED_Init(void)
    �������ܣ���ʼ��SSD1306
    ��ڲ�������
    ���ڲ�������
    �� �� ֵ����    
    ��    ����  
**************************************************************************************/	  
void OLED_Init(void)
{ 	
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
