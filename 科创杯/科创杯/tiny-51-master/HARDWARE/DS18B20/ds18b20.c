#include "ds18b20.h"
#include <reg52.h>
#define uchar unsigned char 
#define uint unsigned int 
//define interface  ���� DS18B20 �ӿ�
sbit DQ=P3^6; 
//variable of temperature                      
uint temp=0;
/*****��ʱ�ӳ���*****/
void Delay_DS18B20(int num)
{
  while(num--) ;
}
/*****��ʼ��DS18B20*****/
void Init_DS18B20(void)
{
  unsigned char x=0;
  DQ = 1;         //DQ��λ
  Delay_DS18B20(8);    //������ʱ
  DQ = 0;         //��Ƭ����DQ����
  Delay_DS18B20(80);   //��ȷ��ʱ������480us
  DQ = 1;         //��������
  Delay_DS18B20(14);
  x = DQ;           //������ʱ�����x=0���ʼ���ɹ���x=1���ʼ��ʧ��
  Delay_DS18B20(20);
}
/*****��һ���ֽ�*****/
unsigned char ReadOneChar(void)
{
  unsigned char i=0;
  unsigned char dat = 0;
  for (i=8;i>0;i--)
  {
    DQ = 0;     // �������ź�
    dat>>=1;
    DQ = 1;     // �������ź�
    if(DQ)
    dat|=0x80;
    Delay_DS18B20(4);
  }
  return(dat);
}
/*****дһ���ֽ�*****/
void WriteOneChar(unsigned char dat)
{
  unsigned char i=0;
  for (i=8; i>0; i--)
  {
    DQ = 0;
    DQ = dat&0x01;
    Delay_DS18B20(5);
    DQ = 1;
    dat>>=1;
  }
}
/*****��ȡ�¶�*****/
unsigned int ReadTemperature(void)
{
  unsigned char a=0;
  unsigned char b=0;
  unsigned int t=0;
  float tt=0;
  Init_DS18B20();
  WriteOneChar(0xCC);  //����������кŵĲ���
  WriteOneChar(0x44);  //�����¶�ת��
  Init_DS18B20();
  WriteOneChar(0xCC);  //����������кŵĲ���
  WriteOneChar(0xBE);  //��ȡ�¶ȼĴ���
  a=ReadOneChar();     //����8λ
  b=ReadOneChar();    //����8λ
  t=b;                                   //��8λת�Ƶ�t
  t<<=8;                           //t��������8λ
  t=t|a;                           //��t��a��λ�򣬵õ�һ��16λ����
  tt=t*0.0625;                   //��t����0.0625�õ�ʵ���¶�ֵ���¶ȴ���������12λ���ȣ���С�ֱ�����0.0625��
  t= tt*10+0.5;     //�Ŵ�10������С�����һλ��ʾ�������������������
  return(t);                   //�����¶�ֵ
}


