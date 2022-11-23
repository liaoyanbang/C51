#include "main.h"

/*******************************************************************************
* 端口定义区域     : 
*******************************************************************************/
sbit LED = P1^0;
sbit LED1 = P0^1;
sbit LED2 = P0^2;
sbit LED3 = P0^3;
sbit LED4 = P0^4;
sbit LED5 = P0^5;
sbit LED6 = P0^6;
sbit LED7 = P0^7;

/*******************************************************************************
* 外部变量调用区域 : 
*******************************************************************************/
//定时器溢出标志
uchar flags1=0;
uchar flags2=0;
uchar flags3=0;
uchar flags4=0;
uchar flags5=0;
uchar flags6=0;
uchar flags7=0;
//计算定时间
xdata ULint time1=0;
//计算定时间
xdata ULint time2=0;
//计算定时间
xdata ULint time3=0;
//计算定时间
xdata ULint time4=0;
//计算定时间
xdata ULint time5=0;
//计算定时间
xdata ULint time6=0;
//计算定时间
xdata ULint time7=0;
//计算距离1
xdata float L_1=0;
//计算距离2
xdata float L_2=0;
//计算距离3
xdata float L_3=0;
//计算距离4
xdata float L_4=0;
//计算距离5
xdata float L_5=0;
//计算距离6
xdata float L_6=0;
//计算距离7
xdata float L_7=0;



//温度
float t_=0;

#define TH_ (65536-30000)/256
#define TL_ (65536-30000)%256
/*******************************************************************************
* 全局变量调用区域 : 
*******************************************************************************/
static unsigned char key;

/*******************************************************************************
* 自定义函数区域 : 
*******************************************************************************/
void delay_ms(unsigned char ms)
{     
	os_wait2(K_TMO,ms);
}

/*******************************************************************************
* 函 数 名         : MainTask
* 函数功能		  	   : 启动任务
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void MainTask(void) _task_ 0  													//启动任务
{
	Init_ultrasonic_wave();
	Init_DS18B20();
	OLED_Init();				  																//初始化OLED  
	OLED_Clear();        																	//清除屏幕
	LED = 0;
	P0 = 0X00;
	TMOD=0x10;		   // TMOD=0x10;// 使用定时器 T1 的模式 2 自动重装载初值  不需要再赋值       
	TL1=TL_;
	TH1=TH_;
	ET1=1;           //允许T1中断
	EA=1;			
	//开启总中断
	OLED_Clear(); 
	OLED_ShowString(0,0,"Dis:",12);
	OLED_ShowString(75,0,"Temp:",12);
  os_create_task (1);   																//创建任务
  os_create_task (2);  

	
  os_delete_task (0);   																//删除任务0
	while(1);
}
 
/*******************************************************************************
* 函 数 名         : TimeTask
* 函数功能		     : 检测时间任务
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TimeTask(void)  _task_ 1  //任务1
{
  while(1) 
  { 
		StartModule1();//启动超声波

		while(!RX1);	  //当RX为零时等待
		TR1=1;		  //开启计数
		while(RX1);	  //当RX为1计数并等待
		TR1=0;		  //关闭计数
		time1 = TH1*256+TL1-TH_*256-TL_;
		TL1=TL_;
		TH1=TH_;

		
		StartModule2();//启动超声波
		while(!RX2);	  //当RX为零时等待
		TR1=1;		  //开启计数
		while(RX2);	  //当RX为1计数并等待
		TR1=0;		  //关闭计数
		time2 = TH1*256+TL1-TH_*256-TL_;
		TL1=TL_;
		TH1=TH_;


		StartModule3();//启动超声波
		while(!RX3);	  //当RX为零时等待
		TR1=1;		  //开启计数
		while(RX3);	  //当RX为1计数并等待
		TR1=0;		  //关闭计数
		//delay_ms(10);
		time3 = TH1*256+TL1-TH_*256-TL_;
		TL1=TL_;
		TH1=TH_;


		StartModule4();//启动超声波
		while(!RX4);	  //当RX为零时等待
		TR1=1;		  //开启计数
		while(RX4);	  //当RX为1计数并等待
		TR1=0;		  //关闭计数
		//delay_ms(10);
		time4 = TH1*256+TL1-TH_*256-TL_;
		TL1=TL_;
		TH1=TH_;
		
		StartModule5();//启动超声波
		while(!RX5);	  //当RX为零时等待
		TR1=1;		  //开启计数
		while(RX5);	  //当RX为1计数并等待
		TR1=0;		  //关闭计数
		//delay_ms(10);
		time5 = TH1*256+TL1-TH_*256-TL_;
		TL1=TL_;
		TH1=TH_;
		
		StartModule6();//启动超声波
		while(!RX6);	  //当RX为零时等待
		TR1=1;		  //开启计数
		while(RX6);	  //当RX为1计数并等待
		TR1=0;		  //关闭计数
		//delay_ms(10);
		time6 = TH1*256+TL1-TH_*256-TL_;
		TL1=TL_;
		TH1=TH_;
		
		StartModule7();//启动超声波
		while(!RX7);	  //当RX为零时等待
		TR1=1;		  //开启计数
		while(RX7);	  //当RX为1计数并等待
		TR1=0;		  //关闭计数
		//delay_ms(10);
		time7 = TH1*256+TL1-TH_*256-TL_;
		TL1=TL_;
		TH1=TH_;
  }
}
/*******************************************************************************
* 函 数 名         : OLEDTask
* 函数功能		     : 检测时间任务
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void OLEDTask(void)  _task_ 2  //任务1
{
	static xdata uint temp = 0;
	char str[8];
	char temp_[6];
  while(1) 
  { 
		//Init_DS18B20();
		if((temp=ReadTemperature())<=350)
		{
			t_ = temp /10.0;
			sprintf(temp_,"%-4.1fC",t_);
			OLED_ShowString(77,1,temp_,12);
		}			
		if(flags1==1)
		{
			flags1 = 0;
		}
		else 
		{
			L_1=time1 *(331.5+t_*0.045)*50/1000000;
			sprintf(str,"1:%-5.1fcm",L_1);
			OLED_ShowString(1,1,str,12);
			if(L_1<3.0) LED1 = 1;
			else LED1 = 0;
		}
		if(flags2==1)
		{
			flags2 = 0;
		}
		else 
		{
			L_2=time2 *(331.5+t_*0.045)*50/1000000;
			sprintf(str,"2:%-5.1fcm",L_2);
			OLED_ShowString(1,2,str,12);
			if(L_2<3.0) LED2 = 1;
			else LED2 = 0;
		}
		if(flags3==1)
		{
			flags3 = 0;
		}
		else 
		{
			L_3=time3 *(331.5+t_*0.045)*50/1000000;
			sprintf(str,"3:%-5.1fcm",L_3);
			OLED_ShowString(1,3,str,12);
			if(L_3<3.0) LED3 = 1;
			else LED3 = 0;
		}
		if(flags4==1)
		{
			flags4 = 0;
		}
		else 
		{
			L_4=time4 *(331.5+t_*0.045)*50/1000000;
			sprintf(str,"4:%-5.1fcm",L_4);
			OLED_ShowString(1,4,str,12);
			if(L_4<3.0) LED4 = 1;
			else LED4 = 0;
		}
		if(flags5==1)
		{
			flags5 = 0;
		}
		else 
		{
			L_5=time5 *(331.5+t_*0.045)*50/1000000;
			sprintf(str,"5:%-5.1fcm",L_5);
			OLED_ShowString(1,5,str,12);
			if(L_5<3.0) LED5 = 1;
			else LED5 = 0;
		}
		if(flags6==1)
		{
			flags6 = 0;
		}
		else 
		{
			L_6=time6 *(331.5+t_*0.045)*50/1000000;
			sprintf(str,"6:%-5.1fcm",L_6);
			OLED_ShowString(1,6,str,12);
			if(L_6<3.0) LED6 = 1;
			else LED6 = 0;
		}
		if(flags7==1)
		{
			flags7 = 0;
		}
		else 
		{
			L_7=time7 *(331.5+t_*0.045)*50/1000000;
			sprintf(str,"7:%-5.1fcm",L_7);
			OLED_ShowString(1,7,str,12);
			if(L_7<3.0) LED7 = 1;
			else LED7 = 0;
		}
  }
}

 //T0中断用来计数器溢出,超过测距范围
void CJ_T1() interrupt 3
{
    if(RX1==1)
		{
			flags1=1;
			RX1=0;
		}
		else  if(RX2==1)
		{
			flags2=1;
			RX2=0;
		}
		else  if(RX3==1)
		{
			flags3=1;
			RX3=0;
		}
		else  if(RX4==1)
		{
			flags4=1;
			RX4=0;
		}
		else if(RX5==1)
		{
			flags5 = 1;
			RX5 = 0;
		}
		else if(RX6==1)
		{
			flags6 = 1;
			RX6 = 0;
		}
		else if(RX7==1)
		{
			flags7 = 1;
			RX7 = 0;
		}
		TR1 = 0;
		TL1=(65536-30000)%256;
		TH1=(65536-30000)/256;
}


          