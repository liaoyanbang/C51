#include "uart.h"
#include "reg52.h"
#include "led.h"
#include "delay.h"

void UART_init()
{
	//设置波特率9600
	TMOD|=0x20;  //定时器1工作于自动重装载模式
	TH1=0xfd;
	TH0=0xfd;
	TR1=1; //启动定时器1
	ET1=0; //禁止定时器1中断
	PCON&=0x7f; //设置SMOD=0，波特率不加倍
	SCON=0x50; //工作于方式1  8位无校验异步通信的收发模式，并清空收发中断标志位
	EA=1;  //总中断允许
	ES=1;//串口收发中断（本程序中不使用串口中断）
}

void UART_Send_Data(unsigned char byte)
{
	SBUF=byte;
	while(!TI);//等待串口数据发送完毕
	TI=0;//将发送中断标志位清0，为下次发送数据做准备
}

unsigned char UART_Receive_Data()
{
	unsigned char temp;
	while(!RI);//等待串口接收到数据（RI被置1）
	temp=SBUF;
	RI=0;//清空接收中断标志位，为下次接收做准备
	return temp;
}
