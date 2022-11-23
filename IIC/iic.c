#include "iic.h"
 
sbit SDA = P3^5;
sbit SCL = P3^6;
unsigned char ACK_flag = 0;



void delay(unsigned int z)
{
	unsigned int i;
	unsigned char j;
	for(i = z;i>0;i--)
		for(j =  114;j>0;j--);
}

/*	延时5微秒	*/
void delay_5us()
{
	_nop_();
}
 
/*	I2C初始化	*/
void I2C_init()		
{
	SCL = 1;  			//拉高SDA和SCL
	delay_5us();	
	SDA = 1;		
	delay_5us();		
}
 
/*	I2C开始信号	*/
void I2C_Start()
{
	SDA = 1;
	SCL = 1;			 //打开时钟
	delay_5us();
	SDA = 0;			 //产生SDA下降沿，触发开始信号
	delay_5us();
}
 
/*	I2C结束信号	*/
void I2C_Stop()
{
	SCL = 0;
	SDA = 0;
	_nop_();
	SCL = 1;		   	//打开时钟
	delay_5us();
	SDA = 1;	   		//产生SDA上升沿，触发结束信号
	delay_5us();
}
 
/*	I2C数据发送	*/
void I2C_Send(unsigned char byte)
{
	unsigned char i,temp;
	temp = byte;
	for(i = 0;i<8;i++)
	{
		SCL = 0;		//关闭时钟准备数据变化
		if(temp & 0x80)	//从最高位发送 1000 0000
		{
			SDA = 1;
		}
		else
		{
			SDA = 0;
		}
		delay_5us();
		SCL = 1;	  	//打开时钟发送数据
		delay_5us();
		temp <<= 1;
	}
}
 
/*	I2C数据接收	*/
unsigned char I2C_Get()
{
	unsigned char i,byte;
	for(i = 0;i<8;i++)
	{
		SCL = 0;  		//关闭时钟准备数据变化
		_nop_();
		SCL = 1;	   	//打开时钟接收数据
		delay_5us();
		if(SDA) byte++;	//从最高位接收
		SCL = 0;		//接收完毕关闭时钟
		if(i == 7) return byte; 
		byte <<= 1;
	}
	return 0x50;		
}
 
/*	I2C主机应答 */
void I2C_ACK_Send(bit A)
{
	SCL = 0;
	_nop_();
	if(A)				//如果i = 1那么拉低数据总线，表示主机应答。
	{
		SDA = 0;
	}				//如果i = 0发送非应答
	else
	{
		SDA = 1;
	}
	delay_5us();
	SCL = 1;
	_nop_();
	SCL = 0;
	_nop_();
	SDA = 1;
	_nop_();
}
/*	I2C从机应答*/
bit I2C_ACK_Get()
{
	bit flag;
	SCL = 0;
	SDA = 1;
	_nop_();
	SCL = 1;
	_nop_();
 	flag = SDA;
	_nop_();
	SCL = 0;
	return flag; 
}
 
/*	写入段	*/
void I2C_Write(unsigned char add_7,unsigned char add,unsigned char byte)
{
	I2C_Start();					//开始
	I2C_Send(add_7+0);				//写eeprom
	if(I2C_ACK_Get()) ACK_flag = 1;	 		//接收从机ACK
	I2C_Send(add);					//选择内存地址
	if(I2C_ACK_Get()) ACK_flag = 1;			//接收从机ACK
	I2C_Send(byte);					//写数据
	if(I2C_ACK_Get()) ACK_flag = 1;			//接收从机ACK
	I2C_Stop();					//主机停止
}
/*	读取段	*/
unsigned char I2C_Read(unsigned char add_7,unsigned char add)
{
	unsigned char message;
	I2C_Start();					//开始
	I2C_Send(add_7+0); 				//写eeprom
	if(I2C_ACK_Get()) ACK_flag = 1;	 		//接收从机ACK
	I2C_Send(add);					//选择内存地址
	if(I2C_ACK_Get()) ACK_flag = 1;			//接收从机ACK
 
	I2C_Start();					//重开始
	I2C_Send(add_7+1);				//读eeprom
	if(I2C_ACK_Get()) ACK_flag = 1;			//接收从机ACK
	message = I2C_Get();			        //接收从机数据
	I2C_ACK_Send(0);				//主机发送ACK
	I2C_Stop();					//主机停止
	return message;	
}
 