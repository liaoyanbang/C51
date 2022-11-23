#include<reg52.h>
#include"1602.h"
#include"ds18b20.h"
#define uint unsigned int
#define uchar unsigned char
sbit speaker=P1^5;		  //定义蜂鸣器
sbit red=P2^0;			  //定义报警灯
//sbit green=P2^1;
sbit key1=P3^0;			  //定义温度按键
sbit key2=P3^1;			  //定义时间按键

uint tem;				  //定义采集温度
int htem,time;			  //定义温度、时间
int cnt1=1,cnt2=1;		  //定义计数变量
int min,i;
uchar mode=0;			  //定义模式变量
uchar code t3[]={"high temp:     C"};
uchar code t4[]={"time:        MIN"};


//延时函数
void delay1(uint i)
{
	while(i--);	
}


//显示函数1
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


//显示函数2
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


//显示函数3
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


//按键扫描函数
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
		   	case 1:time=1;min=1000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //1分钟
	        case 2:time=2;min=2000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //2分钟
	        case 3:time=3;min=3000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //3分钟
			case 4:time=4;min=4000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //4分钟
			case 5:time=5;min=5000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //5分钟
			case 6:time=6;min=6000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //6分钟
			case 7:time=7;min=7000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //7分钟
			case 8:time=8;min=8000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //8分钟
			case 9:time=9;min=9000;display3(time,0xca);writelcd_cmd(0xca+11);break;			 //9分钟
			
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
		    case 1:htem=20;display2(htem,0x8b);writelcd_cmd(0x80+11);break;		  //20摄氏度
		    case 2:htem=50;display2(htem,0x8b);writelcd_cmd(0x80+11);break;		  //50摄氏度
		    case 3:htem=80;display2(htem,0x8b);writelcd_cmd(0x80+11);break;		  //80摄氏度
		    default:break;
		   }
		  }
	  }
   }
  }
  lcd_init();
}


//中断初始化函数
void init()
{
  lcd_init();		   //lcd初始化
   speaker=1;
   red=0;
   mode=0;
   htem=20;
   time=1;
   EA=1;				//总中断开启
   EX0=1;				//外部中断0允许
   ET0=1;				//定时器0允许
   TMOD=0X01;			//定时中断0，模式1
   IT0=1;				//下降沿触发
   TH0=0Xee;			//0.5us
   TL0=0X00;
   //TR0=0;
}



//主函数
void main()
{  
    red=0;
 	init();		  //中断初始化
	while(1)
	{  
		if(mode!=0)
		{keyscan();}

     	else if(mode==0)
		{	
		    delay(100);
		    tmpchange();
			tem=tmp();			 //温度值获取

			if(tem>=(htem*10))		//高于设定温度
			{ 
			  TR0=1;				//中断开启
			  red=1;			  	//报警灯亮		  			  
			}
			else 				    //低于设定温度
			{
			TR0=0;					//中断关闭
            red=0;        			//报警灯灭
			}
			
			delay(10);
		    display(tem,0xca);
		    delay(100);
			TR0=0;
			  
		}
	}
}



//外部中断服务函数
void exter()interrupt 0
{
delay(50);		 //按键消抖
if(P3^2==0)
mode++;			 
mode=mode%4 ;	 //每按4次置0
}


//定时中断服务函数
void timer0()interrupt 1
{
 TH0=0Xee;		 //重装载
 TL0=0X00;
 min--;
 if(min<=0)		 //计时时间到
 {
   red=1;	 //报警灯闪烁
   delay(200);
   speaker=~speaker;delay1(1); 	 //蜂鸣器报警
   TR0=0;		 //定时中断关闭
 }
 
}