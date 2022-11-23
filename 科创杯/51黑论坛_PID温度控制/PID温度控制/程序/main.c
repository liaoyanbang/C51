#include<reg52.h>
#include"1602.h"
#include"ds18b20.h"
#define uint unsigned int
#define uchar unsigned char
sbit speaker=P1^5;		  //���������
sbit red=P2^0;			  //���屨����
//sbit green=P2^1;
sbit key1=P3^0;			  //�����¶Ȱ���
sbit key2=P3^1;			  //����ʱ�䰴��

uint tem;				  //����ɼ��¶�
int htem,time;			  //�����¶ȡ�ʱ��
int cnt1=1,cnt2=1;		  //�����������
int min,i;
uchar mode=0;			  //����ģʽ����
uchar code t3[]={"high temp:     C"};
uchar code t4[]={"time:        MIN"};


//��ʱ����
void delay1(uint i)
{
	while(i--);	
}


//��ʾ����1
void display(uint dat,uchar add)
{
uchar bai,shi ,ge;
bai=dat/100;
shi=dat%100/10;
ge=dat%10;										   
writelcd_cmd(add);
writelcd_dat(bai+0x30);
writelcd_dat(shi+0x30);
writelcd_cmd(add+3);
writelcd_dat(ge+0x30);
}


//��ʾ����2
void display2(uint dat,uchar add)
{
uchar bai,shi ;
bai=dat/100;
shi=dat%100/10;
//ge=dat%10;										   
writelcd_cmd(add);
writelcd_dat(shi+0x30);
writelcd_dat(bai+0x30);
writelcd_cmd(add+3);
//writelcd_dat(ge+0x30);
}


//��ʾ����3
void display3(uint dat,uchar add)
{
uchar ge;
//bai=dat/100;
//shi=dat%100/10;
ge=dat%10;										   
writelcd_cmd(add);
//writelcd_dat(shi+0x30);
//writelcd_dat(bai+0x30);
//writelcd_cmd(add+3);
writelcd_dat(ge+0x30);
}


//����ɨ�躯��
void keyscan()
{  uchar i,j;
   writelcd_cmd(0x80);
   for(i=0;i<16;i++)
   {writelcd_dat(t3[i]);}
   writelcd_cmd(0xc0);
   for(j=0;j<16;j++)
   {writelcd_dat(t4[j]);}
   
   
   while(mode!=0)
   {
display2(htem,0x8b);
display3(time,0xca);
   while(mode!=0)
   {
	   switch(mode)
	   {
	   case 1:writelcd_cmd(0xc0+10);writelcd_cmd(0x0f);break;
	   case 2:writelcd_cmd(0x80+11);writelcd_cmd(0x0f);break;
	   default:mode=0;break;
	   }
	  if(key1==0&&mode==1)
	  {
	    
	  	delay(50);
	  	if(key1==0)
		{  
		   cnt1++;
		   while(key1==0);
		   switch(cnt1%10)
		   {
		   	case 1:time=1;min=1000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //1����
	        case 2:time=2;min=2000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //2����
	        case 3:time=3;min=3000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //3����
			case 4:time=4;min=4000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //4����
			case 5:time=5;min=5000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //5����
			case 6:time=6;min=6000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //6����
			case 7:time=7;min=7000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //7����
			case 8:time=8;min=8000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //8����
			case 9:time=9;min=9000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //9����
			
			default:break;	
		   }	
		}
	  }

	  
	  if(key2==0&&mode==2)
	  {
	  delay(10);
	  if(key2==0)
		  {
		   cnt2++;
		   while(key2==0);
		   switch(cnt2%4)
		   {
		    case 1:htem=20;display2(htem,0x8b);writelcd_cmd(0x80+11);break;		  //20���϶�
		    case 2:htem=50;display2(htem,0x8b);writelcd_cmd(0x80+11);break;		  //50���϶�
		    case 3:htem=80;display2(htem,0x8b);writelcd_cmd(0x80+11);break;		  //80���϶�
		    default:break;
		   }
		  }
	  }
   }
  }
  lcd_init();
}


//�жϳ�ʼ������
void init()
{
  lcd_init();		   //lcd��ʼ��
   speaker=1;
   red=0;
   mode=0;
   htem=20;
   time=1;
   EA=1;				//���жϿ���
   EX0=1;				//�ⲿ�ж�0����
   ET0=1;				//��ʱ��0����
   TMOD=0X01;			//��ʱ�ж�0��ģʽ1
   IT0=1;				//�½��ش���
   TH0=0Xee;			//0.5us
   TL0=0X00;
   //TR0=0;
}



//������
void main()
{  
    red=0;
 	init();		  //�жϳ�ʼ��
	while(1)
	{  
		if(mode!=0)
		{keyscan();}

     	else if(mode==0)
		{	
		    delay(100);
		    tmpchange();
			tem=tmp();			 //�¶�ֵ��ȡ

			if(tem>=(htem*10))		//�����趨�¶�
			{ 
			  TR0=1;				//�жϿ���
			  red=1;			  	//��������		  			  
			}
			else 				    //�����趨�¶�
			{
			TR0=0;					//�жϹر�
            red=0;        			//��������
			}
			
			delay(10);
		    display(tem,0xca);
		    delay(100);
			TR0=0;
			  
		}
	}
}



//�ⲿ�жϷ�����
void exter()interrupt 0
{
delay(50);		 //��������
if(P3^2==0)
mode++;			 
mode=mode%4 ;	 //ÿ��4����0
}


//��ʱ�жϷ�����
void timer0()interrupt 1
{
 TH0=0Xee;		 //��װ��
 TL0=0X00;
 min--;
 if(min<=0)		 //��ʱʱ�䵽
 {
   red=1;	 //��������˸
   delay(200);
   speaker=~speaker;delay1(1); 	 //����������
   TR0=0;		 //��ʱ�жϹر�
 }
 
}