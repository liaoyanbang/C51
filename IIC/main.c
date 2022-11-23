#include "reg52.h"
#include "iic.h"
#define u8 unsigned char
#define AT24C02_ADDR 0xa0

extern unsigned char ACK_flag;

sbit LED=P3^7;
u8 Keyscan(void);
void DelayMs(unsigned int ms); //毫秒
void UART_init()
{
	//设置波特率9600
	TMOD|=0x20;  //定时器1工作于自动重装载模式
	TH1=0xfd;
	TH0=0xfd;
	TR1=1; //启动定时器1
	SM0=0;
	SM1=1;
	EA=1;  //总中断允许
	ES=1;//串口收发中断
}
void main()
{
	I2C_init();			//初始化
	/*	写入段	*/
	I2C_Write(0xa0,0x20,55);
						
	delay(100);
 
	/*	读取段	*/
	P1 = I2C_Read(0xa0,0x20);
	if(ACK_flag) P1 = 0x00;		//校验是否出现无应答
	while(1);
}
u8 Keyscan(void)
{//定义局部变量
	u8 x,k=0;
	x=P2;
	if(x==0xff)	   //没按键跳出
		return 0;
	else
	{
		DelayMs(10);	   //有按键，延时
		P2=0xff;
		x=P2;
		if(x==0xff)		//再测
			return 0;
		else
		{
			switch(x)	   //获取键值
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
			wait:	   //等待按键释放
			P2=0xff;
			x=P2;
			if(x==0xff)  return k;
			else goto wait;
		}
	}
}
void DelayMs(unsigned int ms) //毫秒
{	
    unsigned int i,j;
		for(j=ms; j>0; j--)	
			for(i=100;i>0;i--);	
}
