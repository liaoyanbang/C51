#include<reg52.h>
#define uint unsigned int 
#define uchar unsigned char

sbit rs=P2^6;
sbit rw=P2^5;
sbit en=P2^7;

uchar code t1[]={"Thermometer:     "};
uchar code t2[]={"Temp Now:   .  C "};
void delay(uint ms)
{
 uint i,j;
 for(i=ms;i>0;i--)
 for(j=110;j>0;j--);
}

void writelcd_cmd(uchar cmd)//��1602Һ����ʾ����д������ĺ���
{
en=0;
rs=0;
rw=0;
delay(1);
P0=cmd;
en=1;
delay(1);
en=0;
}

void writelcd_dat(uchar dat)//��1602Һ����ʾ����д�����ݵĺ���
{
en=0;
rs=1;
rw=0;
delay(1);
P0=dat;
en=1;
delay(1);
en=0;
}

void lcd_init()	 //��ʼ��1602Һ����ʾ���ĺ���
{
uchar i,j;
writelcd_cmd(0x38);
delay(5);
writelcd_cmd(0x38);
delay(5);
writelcd_cmd(0x38);
writelcd_cmd(0x08);
writelcd_cmd(0x01);
writelcd_cmd(0x06);
writelcd_cmd(0x0c);
writelcd_cmd(0x80);
for(i=0;i<16;i++)
{
writelcd_dat(t1[i]);
}
writelcd_cmd(0xc0);
for(j=0;j<16;j++)
{
writelcd_dat(t2[j]);
}
}