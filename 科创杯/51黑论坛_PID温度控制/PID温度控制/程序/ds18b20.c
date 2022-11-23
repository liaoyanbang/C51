#include <reg52.h>
#include"1602.h"
#define uchar unsigned char
#define uint unsigned int
sbit DS=P3^7;                                             

void dsreset()     
{
  uint i;
  DS=0;
  i=103;
  while(i>0)i--; //��ʱ��Լ480us~960us
  DS=1;
  i=4;
  while(i>0)i--; //��ʱ��Լ60us~240us
}

bit tmpreadbit()  //18b20��һλ����     
{
   uint i;
   bit dat;
   DS=0;i++;    //���ʹ�Լ1us����      
   DS=1;i++;i++;
   dat=DS;		  //��������
   i=8;while(i>0)i--; //��ʱ45ms����
   return (dat);
}

uchar tmpread()   //18b20��һ���ֽں���
{
  uchar i,j,dat;
  dat=0;
  for(i=1;i<=8;i++)
  {
    j=tmpreadbit();
    dat=(j<<7)|(dat>>1);   
  }
  return(dat);
}

void tmpwrite1()  //��18b20д��1
{	  uint i;
      DS=0;
      i++;i++;	//��ʱ15us����
      DS=1;
      i=8;while(i>0)i--;
}

void tmpwrite0()  //��18b20д��0
{	 uint i;
      DS=0;       
      i=8;while(i>0)i--; //��ʱ60us����
      DS=1;
      i++;i++;
}

void tmpwritebyte(uchar dat)//18b20дһ���ֽں���   
{
  uchar j;
  bit testb;
  for(j=1;j<=8;j++)
  {
    testb=dat&0x01;
    dat=dat>>1;
    if(testb)     
    {
     tmpwrite1();
    }
    else
    {
     tmpwrite0(); 
    }

  }
}

void tmpchange()  	//18b20�¶�ת������
{						//18b20ִ�����У�
  dsreset();			//1����ʼ��
  delay(1);				//
  tmpwritebyte(0xcc);  	//2��rom����ָ��
  tmpwritebyte(0x44);  	//3��18b20���ܲ���ָ��
}

uint tmp()               
{
  uint temp;
  float tt;
  uchar a,b;
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);
  tmpwritebyte(0xbe);
  a=tmpread();
  b=tmpread();
  temp=b;
  temp<<=8;             
  temp=temp|a;
  tt=temp*0.0625;
  temp=tt*10+0.5;  //��0.5��������
  return temp;
}



			
