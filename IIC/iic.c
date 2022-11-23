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

/*	��ʱ5΢��	*/
void delay_5us()
{
	_nop_();
}
 
/*	I2C��ʼ��	*/
void I2C_init()		
{
	SCL = 1;  			//����SDA��SCL
	delay_5us();	
	SDA = 1;		
	delay_5us();		
}
 
/*	I2C��ʼ�ź�	*/
void I2C_Start()
{
	SDA = 1;
	SCL = 1;			 //��ʱ��
	delay_5us();
	SDA = 0;			 //����SDA�½��أ�������ʼ�ź�
	delay_5us();
}
 
/*	I2C�����ź�	*/
void I2C_Stop()
{
	SCL = 0;
	SDA = 0;
	_nop_();
	SCL = 1;		   	//��ʱ��
	delay_5us();
	SDA = 1;	   		//����SDA�����أ����������ź�
	delay_5us();
}
 
/*	I2C���ݷ���	*/
void I2C_Send(unsigned char byte)
{
	unsigned char i,temp;
	temp = byte;
	for(i = 0;i<8;i++)
	{
		SCL = 0;		//�ر�ʱ��׼�����ݱ仯
		if(temp & 0x80)	//�����λ���� 1000 0000
		{
			SDA = 1;
		}
		else
		{
			SDA = 0;
		}
		delay_5us();
		SCL = 1;	  	//��ʱ�ӷ�������
		delay_5us();
		temp <<= 1;
	}
}
 
/*	I2C���ݽ���	*/
unsigned char I2C_Get()
{
	unsigned char i,byte;
	for(i = 0;i<8;i++)
	{
		SCL = 0;  		//�ر�ʱ��׼�����ݱ仯
		_nop_();
		SCL = 1;	   	//��ʱ�ӽ�������
		delay_5us();
		if(SDA) byte++;	//�����λ����
		SCL = 0;		//������Ϲر�ʱ��
		if(i == 7) return byte; 
		byte <<= 1;
	}
	return 0x50;		
}
 
/*	I2C����Ӧ�� */
void I2C_ACK_Send(bit A)
{
	SCL = 0;
	_nop_();
	if(A)				//���i = 1��ô�����������ߣ���ʾ����Ӧ��
	{
		SDA = 0;
	}				//���i = 0���ͷ�Ӧ��
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
/*	I2C�ӻ�Ӧ��*/
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
 
/*	д���	*/
void I2C_Write(unsigned char add_7,unsigned char add,unsigned char byte)
{
	I2C_Start();					//��ʼ
	I2C_Send(add_7+0);				//дeeprom
	if(I2C_ACK_Get()) ACK_flag = 1;	 		//���մӻ�ACK
	I2C_Send(add);					//ѡ���ڴ��ַ
	if(I2C_ACK_Get()) ACK_flag = 1;			//���մӻ�ACK
	I2C_Send(byte);					//д����
	if(I2C_ACK_Get()) ACK_flag = 1;			//���մӻ�ACK
	I2C_Stop();					//����ֹͣ
}
/*	��ȡ��	*/
unsigned char I2C_Read(unsigned char add_7,unsigned char add)
{
	unsigned char message;
	I2C_Start();					//��ʼ
	I2C_Send(add_7+0); 				//дeeprom
	if(I2C_ACK_Get()) ACK_flag = 1;	 		//���մӻ�ACK
	I2C_Send(add);					//ѡ���ڴ��ַ
	if(I2C_ACK_Get()) ACK_flag = 1;			//���մӻ�ACK
 
	I2C_Start();					//�ؿ�ʼ
	I2C_Send(add_7+1);				//��eeprom
	if(I2C_ACK_Get()) ACK_flag = 1;			//���մӻ�ACK
	message = I2C_Get();			        //���մӻ�����
	I2C_ACK_Send(0);				//��������ACK
	I2C_Stop();					//����ֹͣ
	return message;	
}
 