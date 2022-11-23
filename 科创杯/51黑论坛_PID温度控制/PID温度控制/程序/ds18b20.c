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
  while(i>0)i--; //延时大约480us~960us
  DS=1;
  i=4;
  while(i>0)i--; //延时大约60us~240us
}

bit tmpreadbit()  //18b20读一位数据     
{
   uint i;
   bit dat;
   DS=0;i++;    //拉低大约1us左右      
   DS=1;i++;i++;
   dat=DS;		  //读回数据
   i=8;while(i>0)i--; //延时45ms左右
   return (dat);
}

uchar tmpread()   //18b20读一个字节函数
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

void tmpwrite1()  //向18b20写入1
{	  uint i;
      DS=0;
      i++;i++;	//延时15us左右
      DS=1;
      i=8;while(i>0)i--;
}

void tmpwrite0()  //向18b20写入0
{	 uint i;
      DS=0;       
      i=8;while(i>0)i--; //延时60us左右
      DS=1;
      i++;i++;
}

void tmpwritebyte(uchar dat)//18b20写一个字节函数   
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

void tmpchange()  	//18b20温度转换函数
{						//18b20执行序列：
  dsreset();			//1、初始化
  delay(1);				//
  tmpwritebyte(0xcc);  	//2、rom操作指令
  tmpwritebyte(0x44);  	//3、18b20功能操作指令
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
  temp=tt*10+0.5;  //加0.5四舍五入
  return temp;
}



			
