#include "reg52.h"
#include "iic.h"
#define u8 unsigned char
#define AT24C02_ADDR 0xa0

extern unsigned char ACK_flag;

sbit LED=P3^7;
u8 Keyscan(void);
void DelayMs(unsigned int ms); //����
void UART_init()
{
	//���ò�����9600
	TMOD|=0x20;  //��ʱ��1�������Զ���װ��ģʽ
	TH1=0xfd;
	TH0=0xfd;
	TR1=1; //������ʱ��1
	SM0=0;
	SM1=1;
	EA=1;  //���ж�����
	ES=1;//�����շ��ж�
}
void main()
{
	I2C_init();			//��ʼ��
	/*	д���	*/
	I2C_Write(0xa0,0x20,55);
						
	delay(100);
 
	/*	��ȡ��	*/
	P1 = I2C_Read(0xa0,0x20);
	if(ACK_flag) P1 = 0x00;		//У���Ƿ������Ӧ��
	while(1);
}
u8 Keyscan(void)
{//����ֲ�����
	u8 x,k=0;
	x=P2;
	if(x==0xff)	   //û��������
		return 0;
	else
	{
		DelayMs(10);	   //�а�������ʱ
		P2=0xff;
		x=P2;
		if(x==0xff)		//�ٲ�
			return 0;
		else
		{
			switch(x)	   //��ȡ��ֵ
			{
				case 0xfe:k=1;break;
				case 0xfd:k=2;break;
				case 0xfb:k=3;break;
				case 0xf7:k=4;break;
 				case 0xef:k=5;break;
				case 0xdf:k=6;break;
				case 0xbf:k=7;break;
				case 0x7f:k=8;break;
				default:break;
			}
			wait:	   //�ȴ������ͷ�
			P2=0xff;
			x=P2;
			if(x==0xff)  return k;
			else goto wait;
		}
	}
}
void DelayMs(unsigned int ms) //����
{	
    unsigned int i,j;
		for(j=ms; j>0; j--)	
			for(i=100;i>0;i--);	
}
