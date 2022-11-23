/*
********************************
*						I2C.H
*			ģ��I2C������ͷ�ļ�
********************************
*/

#ifndef __I2C_H__
#define __I2C_H__
#include "main.h"

#define  _Nop()  _nop_()        /*�����ָ��*/

//���Ŷ���
sbit SDA = P0^5;
sbit SCL = P0^4;


/*************************�����б�*************************/

bit ISendByte(unsigned char sla, unsigned char c);																			//�����ӵ�ַ���������ֽ����ݺ���

	
bit ISendStr(unsigned char sla, unsigned char suba, unsigned char *s, unsigned char no);//�����ӵ�ַ�������Ͷ��ֽ����ݺ���	


//extern bit ISendStr_Int(unsigned char sla, unsigned char suba, unsigned char *s, unsigned char no);//����������⣬������ȫͬISendStr()


//extern bit IRcvByte(unsigned char sla, unsigned char *c);															//�����ӵ�ַ�������ֽ����ݺ���


bit IRcvStr(unsigned char sla, unsigned char suba, unsigned char *s, unsigned char no);	//�����ӵ�ַ������ȡ���ֽ����ݺ���


bit mRcvByte(unsigned char sla, unsigned char *s,unsigned char no);											//�����ӵ�ַ���������ֽ����ݺ���


bit mSendStr(unsigned char sla, unsigned char *s, unsigned char no);										//�����ӵ�ַ�������Ͷ��ֽ����ݺ���


/*************************��Ҫ��ͷ�ļ��������ĺ���*************************/
static void Start_I2c(void);						 //�������ߺ���
static void Stop_I2c(void);							 //�������ߺ���
static void SendByte(unsigned char c);	 //�ֽ����ݷ��ͺ���
static unsigned char RcvByte(void);			 //�ֽ����ݽ��պ���
static void Ack_I2c(bit a);							 //Ӧ���Ӻ���


#endif

