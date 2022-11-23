/*
********************************
*						I2C.H
*			模拟I2C操作的头文件
********************************
*/

#ifndef __I2C_H__
#define __I2C_H__
#include "main.h"

#define  _Nop()  _nop_()        /*定义空指令*/

//引脚定义
sbit SDA = P0^5;
sbit SCL = P0^4;


/*************************函数列表*************************/

bit ISendByte(unsigned char sla, unsigned char c);																			//向无子地址器件发送字节数据函数

	
bit ISendStr(unsigned char sla, unsigned char suba, unsigned char *s, unsigned char no);//向有子地址器件发送多字节数据函数	


//extern bit ISendStr_Int(unsigned char sla, unsigned char suba, unsigned char *s, unsigned char no);//解决重入问题，功能完全同ISendStr()


//extern bit IRcvByte(unsigned char sla, unsigned char *c);															//向无子地址器件读字节数据函数


bit IRcvStr(unsigned char sla, unsigned char suba, unsigned char *s, unsigned char no);	//向有子地址器件读取多字节数据函数


bit mRcvByte(unsigned char sla, unsigned char *s,unsigned char no);											//向无子地址器件读多字节数据函数


bit mSendStr(unsigned char sla, unsigned char *s, unsigned char no);										//向无子地址器件发送多字节数据函数


/*************************不要在头文件中声明的函数*************************/
static void Start_I2c(void);						 //启动总线函数
static void Stop_I2c(void);							 //结束总线函数
static void SendByte(unsigned char c);	 //字节数据发送函数
static unsigned char RcvByte(void);			 //字节数据接收函数
static void Ack_I2c(bit a);							 //应答子函数


#endif

