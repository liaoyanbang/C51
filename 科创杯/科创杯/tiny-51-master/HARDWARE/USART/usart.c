#include "usart.h"
#include "main.h"  



void usart_init(void)
{
	TMOD |= 0x20;
	SCON = 0x50;
	TH1 = 0xFA;
	TL1 = TH1;
	PCON = 0x00;
	TR1 = 1;
	ES=1;						//打开接收中断
	EA=1;						//打开总中断
	
}


void send_byte(unsigned char dat)
{
	ES = 0;
	SBUF = dat;
	while(!TI);
	TI = 0;   
	ES = 1;
}

/*发送一个字符串*/
void send_string(unsigned char *p)
{
	while(*p!= '\0')
	{
		send_byte(*p);
		p++;
  }
}

void Usart_Rx() interrupt 4
{
	unsigned char Res;
	if(RI==1)
	{
		RI=0;
		Res =SBUF;	
		if((USART_RX_STA&0x80)==0)
			{
			if(USART_RX_STA&0x40)
				{
				if(Res!=0x0a)USART_RX_STA=0;
				else USART_RX_STA|=0x80;	
				}
			else 
				{	
				if(Res==0x0d)USART_RX_STA|=0x40;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3F]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;
					}		 
				}
			} 
		}
}

