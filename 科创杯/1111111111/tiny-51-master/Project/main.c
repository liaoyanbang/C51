#include "main.h"

/*******************************************************************************
* 端口定义区域     : 
*******************************************************************************/
sbit led0=P2^0;
sbit led1=P2^1;
sbit beep=P0^0;
/*******************************************************************************
* 外部变量调用区域 : 
*******************************************************************************/
xdata unsigned char  USART_RX_BUF[USART_REC_LEN];						//串口接收
unsigned char  USART_RX_STA;
xdata unsigned char run_state=0;
xdata unsigned char mark,ui_mark=0,set_mark,run_mark=0;														//标志位
unsigned char key_buf[3]={40,40,40};												
xdata int  ui_convert_dat;																	//当前温度+上下限
xdata short  temp_max=10000;
xdata short  temp_min=0;

xdata short  temp_max_buf[4]={2000,4000,6000,10000};					//温度缓存区
xdata short  temp_min_buf[4]={0,-500,-2000,0};
xdata char ride_flag=1;																			//默认乘以1

xdata unsigned char pos_x[9]={56,80,104,4,16+31,16+32+16+8+31,4,16+16,72+16};								//设置索引
xdata unsigned char pos_y[9]={2,2,2,4,4,4,6,6,6};
xdata char pos_i=0;

unsigned char cut_pos=0, time_cut[6]={8,12,12,14,14,18};					//时间段

xdata unsigned char uca_send_time_dat[9];										//时间设置
xdata unsigned char uca_wr_time_dat[7];
xdata unsigned char uca_rd_time_dat[7];
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
	unsigned char j;
	OLED_Init();				  																//初始化OLED  
	usart_init();																					//初始化串口
	
	for(j=0;j<6;j++)
	{
		time_cut[j]=At24c02Read(j);
	}
	for(j=0;j<5;j+=2)
	{
		temp_max_buf[j/2]=(At24c02Read(j+6)+((At24c02Read(j+7))<<8));
		delay_ms(100);
	}
	
	for(j=0;j<5;j+=2)
	{
		temp_min_buf[j/2]=(At24c02Read(j+12)+((At24c02Read(j+13))<<8));
		delay_ms(100);
	}
	
//	OLED_ShowCHinese(0,0,0);
  os_create_task (1);   																//创建任务
  os_create_task (2);  
	os_create_task (3); 
	os_create_task (4);  
	os_create_task (5);
	os_create_task (6);
	os_create_task (7);
	
  os_delete_task (0);   																//删除任务0
	while(1);
}
 
/*******************************************************************************
* 函 数 名         : ADCTask
* 函数功能		    	 : 检测任务
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void ADCTask(void)  _task_ 1  //任务1
{
	unsigned char uca_rd_dat[2];	
	
	float dat;
  while(1) 
  { 
		led0=!led0;
		
		//ADS1015初始化, 0通道采样
		Start_conversion(ADS1015_SLA_ADDR, CONFIG_REG, 0);		
		delay_ms(20);
		Read_conversion(ADS1015_SLA_ADDR, CONVERSION_REG, uca_rd_dat);
		
		ui_convert_dat = uca_rd_dat[0] * 256 + uca_rd_dat[1];
		ui_convert_dat /= 16;
		
		if(ui_convert_dat>1600)
			ui_convert_dat=1600;
		else if(ui_convert_dat<=1)
		{
			ui_convert_dat=1;
		}
		
		dat = (float)ui_convert_dat*13.75-2000.0;
		ui_convert_dat=(int)dat;
		os_send_signal(4);
		os_send_signal(6);
		delay_ms(200);
		delay_ms(200);
		delay_ms(200);
		delay_ms(200);
		delay_ms(200);
		
  }
}

/*******************************************************************************
* 函 数 名         : UARTTask
* 函数功能		  	   : 串口任务
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void UARTTask(void) _task_ 2
{
	xdata unsigned char temp_send_buf[9],j; 
  while(1)
  {
		if((USART_RX_STA&0x80)==0x80)
		{
			USART_RX_STA=0;
			if(USART_RX_BUF[0]=='G')
			{
				if(USART_RX_BUF[1]=='S')
				{
					send_string(uca_send_time_dat);
				}
				if(USART_RX_BUF[1]=='T')
				{		
					temp_send_buf[0] = ui_convert_dat/10000 + 0x30;
					temp_send_buf[1] = (ui_convert_dat%10000)/1000 + 0x30;
					temp_send_buf[2] = (ui_convert_dat%1000)/100 + 0x30;
					temp_send_buf[4] = (ui_convert_dat/100)%10 + 0x30;
					temp_send_buf[5] = ui_convert_dat%10 + 0x30;
					
					temp_send_buf[3] = '.';
					temp_send_buf[6] = 'C';
					temp_send_buf[7] = '\n';
					temp_send_buf[7] = '\0';
					send_string(temp_send_buf);
				}
				if(USART_RX_BUF[1]=='Z')
				{
					if(run_mark==0)
						send_string("运行稳定");
					else if(run_mark==1)
						send_string("低于下限");
					else if(run_mark==2)
						send_string("高于上限");
				}
			}
			if(USART_RX_BUF[0]=='S')
			{
				if(USART_RX_BUF[1]=='T')
				{
					USART_RX_BUF[2]=(USART_RX_BUF[3]-48)*16+(USART_RX_BUF[4]-48);
					P8563_SendStr(HOUR_REG,&USART_RX_BUF[2],6);
					USART_RX_BUF[2]=0;
					delay_ms(200);
					delay_ms(200);

					USART_RX_BUF[5]=(USART_RX_BUF[6]-48)*16+(USART_RX_BUF[7]-48);
					P8563_SendStr(MINUTE_REG,&USART_RX_BUF[5],1);
					USART_RX_BUF[5]=0;		
					delay_ms(200);
					delay_ms(200);
				}
				if(USART_RX_BUF[1]=='D')
				{
					time_cut[0]=(USART_RX_BUF[3]-48)*10+USART_RX_BUF[4]-48;
					time_cut[1]=(USART_RX_BUF[6]-48)*10+USART_RX_BUF[7]-48;
					
					time_cut[2]=(USART_RX_BUF[9]-48)*10+USART_RX_BUF[10]-48;
					time_cut[3]=(USART_RX_BUF[12]-48)*10+USART_RX_BUF[13]-48;
					
					time_cut[4]=(USART_RX_BUF[15]-48)*10+USART_RX_BUF[16]-48;
					time_cut[5]=(USART_RX_BUF[18]-48)*10+USART_RX_BUF[19]-48;
					
					for(j=0;j<6;j++)
					{
						At24c02Write(j,time_cut[j]);
						delay_ms(250);
					}
				}
				if(USART_RX_BUF[1]=='S')
				{
					if(USART_RX_BUF[3]=='+')
					{
						temp_max_buf[USART_RX_BUF[13]-48]=((USART_RX_BUF[4]-48)*100+(USART_RX_BUF[5]-48)*10+(USART_RX_BUF[6]-48))*100;
						
					}
					else if(USART_RX_BUF[3]=='-')
					{
						temp_max_buf[USART_RX_BUF[13]-48]=((USART_RX_BUF[4]-48)*100+(USART_RX_BUF[5]-48)*10+(USART_RX_BUF[6]-48))*-100;
					}
					
			
          if(USART_RX_BUF[8]=='+')
					{
						temp_min_buf[USART_RX_BUF[13]-48]=((USART_RX_BUF[9]-48)*100+(USART_RX_BUF[10]-48)*10+(USART_RX_BUF[11]-48))*100;
					}
					else if(USART_RX_BUF[8]=='-')
					{
						temp_min_buf[USART_RX_BUF[13]-48]=((USART_RX_BUF[9]-48)*100+(USART_RX_BUF[10]-48)*10+(USART_RX_BUF[11]-48))*-100;
					}
					
					
					At24c02Write((USART_RX_BUF[13]-48)*2+6,(unsigned char)(temp_max_buf[USART_RX_BUF[13]-48]));
					delay_ms(100);
					At24c02Write((USART_RX_BUF[13]-48)*2+7,(unsigned char)((temp_max_buf[USART_RX_BUF[13]-48])>>8));
					delay_ms(100);
					
					At24c02Write((USART_RX_BUF[13]-48)*2+12,(unsigned char)(temp_min_buf[USART_RX_BUF[13]-48]));
					delay_ms(100);
					At24c02Write((USART_RX_BUF[13]-48)*2+13,(unsigned char)((temp_min_buf[USART_RX_BUF[13]-48])>>8));
					delay_ms(100);
				}
			}
			
		}

		delay_ms(200);
  }
}

/*******************************************************************************
* 函 数 名         : KeyTask
* 函数功能		  		 : 按键任务
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void KeyTask(void) _task_ 3
{
	unsigned char num=0;
  while(1)
  {
		key=KeyDown();
		if(key==11)
		{
			ui_mark=!ui_mark;
		}else if(key==12)
		{
			set_mark++;
			set_mark %=2;
		}else if((key==13))//上
		{
			if(set_mark!=1)
			{
				if((pos_i>=3))
					pos_i-=3;
			}else if((pos_i==7) || (pos_i==8))
			{
				ride_flag=1;
			}
			
		}else if((key==14))//下
		{
			if(ui_mark==0)
				mark=!mark;
			if(set_mark!=1)
			{
				if((pos_i<=5))
					pos_i+=3;
			}else if((pos_i==7) || (pos_i==8))
			{
				ride_flag=-1;
			}
		}else if((key==15))//左
		{
			if(set_mark!=1)
			{
				pos_i--;
				if(pos_i<0)
					pos_i=8;
			}
		}else if((key==16))//右
		{
			if(set_mark!=1)
			{
				pos_i++;
				if(pos_i>8)
					pos_i=0;
			}
		}
		
		else if(key==1)
		{
			if(set_mark==1)
			{
				for(num=0;num<3;num++)
				{
					if(key_buf[num] == 40)
					{
						key_buf[num]=1;
						break;
					}
				}
			}
			os_send_signal(7);
		}else if(key==2)
		{
			if(set_mark==1)
			{
			for(num=0;num<3;num++)
			{
				if(key_buf[num] == 40)
				{
					key_buf[num]=2;
					break;
				}
			}
			}
			os_send_signal(7);
		}else if(key==3)
		{
			if(set_mark==1)
			{
				for(num=0;num<3;num++)
				{
					if(key_buf[num] == 40)
					{
						key_buf[num]=3;
						break;
					}
				}
			}
			os_send_signal(7);
		}else if(key==4)
		{
			if(set_mark==1)
			{
				for(num=0;num<3;num++)
				{
					if(key_buf[num] == 40)
					{
						key_buf[num]=4;
						break;
					}
				}
			}
			os_send_signal(7);
		}else if(key==5)
		{
			if(set_mark==1)
			{
				for(num=0;num<3;num++)
				{
					if(key_buf[num] == 40)
					{
						key_buf[num]=5;
						break;
					}
				}
			}
			os_send_signal(7);
		}else if(key==6)
		{
			if(set_mark==1)
			{
				for(num=0;num<3;num++)
				{
					if(key_buf[num] == 40)
					{
						key_buf[num]=6;
						break;
					}
				}
			}
			os_send_signal(7);
		}else if(key==7)
		{
			if(set_mark==1)
			{
				for(num=0;num<3;num++)
				{
					if(key_buf[num] == 40)
					{
						key_buf[num]=7;
						break;
					}
				}
			}
			os_send_signal(7);
		}else if(key==8)
		{
			if(set_mark==1)
			{
				for(num=0;num<3;num++)
				{
					if(key_buf[num] == 40)
					{
						key_buf[num]=8;
						break;
					}
				}
			}
			os_send_signal(7);
		}else if(key==9)
		{
			if(set_mark==1)
			{
				for(num=0;num<3;num++)
				{
					if(key_buf[num] == 40)
					{
						key_buf[num]=9;
						break;
					}
				}
			}
			os_send_signal(7);
		}
		else if(key==10)
		{
			if(set_mark==1)
			{
				for(num=0;num<3;num++)
				{
					if(key_buf[num] == 40)
					{
						key_buf[num]=0;
						break;
					}
				}
			}
			os_send_signal(7);
		}
		delay_ms(50);
  }
}

/*******************************************************************************
* 函 数 名         : OLEDTask
* 函数功能		  		 : 显示任务
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void OLEDTask(void) _task_ 4
{
	char event;
	float cov_f;
//	xdata unsigned char x;
  while(1)
  {	
		OLED_Clear();
		OLED_ShowString(0,0,"TEMP:     .",12);
		OLED_ShowString(0,2,"TIME:",12);
		
		OLED_ShowString(0 ,4,"MAX:",12);
		OLED_ShowString(65,4,"MIN:",12);
		while(ui_mark==0)      
		{
			if(run_state==1)
				OLED_ShowString(0,6,"TEMP OVER!",12);
			else if(run_state==2)
				OLED_ShowString(0,6,"TEMP  LOW!",12);
			else
				OLED_ShowString(0,6,"TEMP  NOR!",12);
			if(mark==0 )
			{
					event=os_wait2(K_SIG | K_TMO,10);
					if(event==SIG_EVENT)
					{
						if(ui_convert_dat>0)
						{
							OLED_ShowChar(52,0,'+',12);
							OLED_ShowNum(60,0,ui_convert_dat / 100,3,12);
							OLED_ShowNum(94,0,ui_convert_dat % 100,2,12);
							OLED_ShowString(118,0,"C",12);
						}else
						{
							OLED_ShowChar(52,0,'-',12);
							ui_convert_dat=-1*ui_convert_dat;
							OLED_ShowNum(60,0,ui_convert_dat / 100,3,12);
							OLED_ShowNum(94,0,ui_convert_dat % 100,2,12);
							OLED_ShowString(118,0,"C",12);
						}
						
						if(temp_max>0)
						{
							OLED_ShowChar(32,4,'+',12);
							OLED_ShowNum(40,4,temp_max/100,3,12);
						}
						else
						{
							OLED_ShowChar(32,4,'-',12);
							OLED_ShowNum(40,4,-temp_max/100,3,12);
						}
						
						if(temp_min>0)
						{
							OLED_ShowChar(97,4,'+',12);
							OLED_ShowNum(105,4,temp_min/100,3,12);
						}
						else
						{
							OLED_ShowChar(97,4,'-',12);
							OLED_ShowNum(105,4,-temp_min/100,3,12);
						}
					}
				}else
				{
					event=os_wait2(K_SIG | K_TMO,10);
					if(event==SIG_EVENT)
					{
						event=0;
						cov_f=3200.0 + ui_convert_dat*1.8;
						ui_convert_dat=(int)cov_f;
						if(ui_convert_dat>0)
						{
							OLED_ShowChar(52,0,'+',12);
							OLED_ShowNum(60,0,ui_convert_dat/100,3,12);
							OLED_ShowNum(94,0,ui_convert_dat%100,2,12);
							OLED_ShowString(118,0,"F",12);
						}
						else
						{
							OLED_ShowChar(52,0,'-',12);
							ui_convert_dat=-1*ui_convert_dat;
							OLED_ShowNum(60,0,ui_convert_dat/100,3,12);
							OLED_ShowNum(94,0,ui_convert_dat%100,2,12);
							OLED_ShowString(118,0,"F",12);
						}
						cov_f=3200.0 + temp_max*1.8;
						temp_max=(int)cov_f;
						cov_f=3200.0 + temp_min*1.8;
						temp_min=(int)cov_f;
						if(temp_max>0)
						{
							OLED_ShowChar(32,4,'+',12);
							OLED_ShowNum(40,4,temp_max/100,3,12);
						}
						else
						{
							OLED_ShowChar(32,4,'-',12);
							OLED_ShowNum(40,4,-temp_max/100,3,12);
						}
						
						if(temp_min>0)
						{
							OLED_ShowChar(97,4,'+',12);
							OLED_ShowNum(105,4,temp_min/100,3,12);
						}
						else
						{
							OLED_ShowChar(97,4,'-',12);
							OLED_ShowNum(105,4,-temp_min/100,3,12);
						}
					}
				}
				
				OLED_ShowString(50,2,uca_send_time_dat,12);
				
				delay_ms(200);
		}
		OLED_Clear();
		//设置界面
		OLED_ShowString(0,0,"SET:",12);
		OLED_ShowString(0,2,"TIME:",12);
		
		OLED_ShowString(16,4,"STA:",12);
		OLED_ShowString(16+32+16+8,4,"END:",12);
		
		OLED_ShowString(16,6,"S:",12);
		OLED_ShowString(16+16+32+8,6,"X:",12);
		while(ui_mark==1)
		{
//			if(set_mark==1)
//			{
//				if(ride_flag==1)
//					OLED_ShowChar(56,0,'+',12);
//				else
//					OLED_ShowChar(56,0,'-',12);
//				for(x=0;x<3;x++)
//				{
//					if(key_buf[x]!=40)
//					{
//						OLED_ShowChar(72+x*8,0,key_buf[x]+48,12);
//					}
//				}
//			}
			//符号显示
			OLED_ShowString(56,2,uca_send_time_dat,12);
			OLED_ShowString(4,4,"+",12);
			OLED_ShowString(4,6,"-",12);

			//上下限分頁显示
			if(temp_max_buf[cut_pos/2]>=0)
			{
				OLED_ShowChar(32,6,'+',12);
				OLED_ShowNum(40,6,temp_max_buf[cut_pos/2]/100,3,12);
			}
			else
			{
				OLED_ShowChar(32,6,'-',12);
				OLED_ShowNum(40,6,-1*temp_max_buf[cut_pos/2]/100,3,12);
			}

			if(temp_min_buf[cut_pos/2]>=0)
			{
				OLED_ShowChar(16+16+32+8+16,6,'+',12);
				OLED_ShowNum(16+16+32+8+16+8,6,temp_min_buf[cut_pos/2]/100,3,12);
			}
			else
			{
				OLED_ShowChar(16+16+32+8+16,6,'-',12);
				OLED_ShowNum(16+16+32+8+16+8,6,-1*temp_min_buf[cut_pos/2]/100,3,12);
			}
			
			//时间分段显示
			OLED_ShowNum(40,0,cut_pos/2,1,12);	
			OLED_ShowNum(16+32+1,4,time_cut[cut_pos],2,12);
			OLED_ShowNum(16+32+16+8+32+1,4,time_cut[cut_pos+1],2,12);		
	
			delay_ms(150);

			if(pos_i==3 || pos_i==6)
				OLED_ShowString(pos_x[pos_i],pos_y[pos_i]," ",12);
			else if(pos_i==7||pos_i==8)
				OLED_ShowString(pos_x[pos_i],pos_y[pos_i],"    ",12);
			else
				OLED_ShowString(pos_x[pos_i],pos_y[pos_i],"  ",12);	
			delay_ms(150);
	
			if(set_mark==0)
				OLED_ShowString(103,0,"OK!",12);
			else if(set_mark==1)
				OLED_ShowString(103,0,"ING",12);
		}
  }
}

/*******************************************************************************
* 函 数 名         : TimeTask
* 函数功能		 		 	: 时钟任务
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TimeTask(void) _task_ 5
{
  while(1)
  {
				    //读取时钟
		P8563_RcvStr(SECOND_REG, uca_rd_time_dat, 7);		//读取实时时钟数据
	
		uca_send_time_dat[0] = uca_rd_time_dat[2] / 16 + 0x30;
		uca_send_time_dat[1] = uca_rd_time_dat[2] % 16 + 0x30;
		uca_send_time_dat[3] = uca_rd_time_dat[1] / 16 + 0x30;
		uca_send_time_dat[4] = uca_rd_time_dat[1] % 16 + 0x30;
		uca_send_time_dat[6] = uca_rd_time_dat[0] / 16 + 0x30;
		uca_send_time_dat[7] = uca_rd_time_dat[0] % 16 + 0x30;
		uca_send_time_dat[2] = ':';
		uca_send_time_dat[5] = ':';
		uca_send_time_dat[8] = '\0';
		
		delay_ms(200);
		delay_ms(200);
		delay_ms(200);
		delay_ms(200);
		delay_ms(200);
  }
}

/*******************************************************************************
* 函 数 名         : BEEPTask
* 函数功能		  		 : 报警任务
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void BEEPTask(void) _task_ 6
{
	unsigned char time_dat,event1,beep_mark;
  while(1)
  {
		event1=os_wait2(K_SIG | K_TMO,150);
		if((ui_mark==0) && (event1==SIG_EVENT))
		{
				event1=0;
				if(ui_convert_dat>temp_max)
				{
					beep_mark=1;
					beep=!beep;
					run_mark=2;
					run_state=1;
				}
				else if(ui_convert_dat<temp_min)
				{
					beep_mark=2;
					beep=0;
					run_mark=1;
					run_state=2;
				}
				else
				{
					beep_mark=0;
					beep=1;
					run_mark=0;
					run_state=0;
				}
		}
		//蜂鳴器發聲
		if(beep_mark==1)
			beep=!beep;
		else if(beep_mark==2)
			beep=0;
		else
			beep=1;
		
		
		time_dat=uca_rd_time_dat[2]/16*10+uca_rd_time_dat[2]%16;
		if((time_cut[0]<=time_dat) && (time_cut[1]>=time_dat))
		{
			temp_max=temp_max_buf[0];
			temp_min=temp_min_buf[0];
		}
		else if((time_cut[2]<=time_dat) && (time_cut[3]>=time_dat))
		{
			temp_max=temp_max_buf[1];
			temp_min=temp_min_buf[1];
		}
		else if((time_cut[4]<time_dat) && (time_cut[5]>time_dat))
		{
			temp_max=temp_max_buf[2];
			temp_min=temp_min_buf[2];
		}
		else
		{
			temp_max=temp_max_buf[3];
			temp_min=temp_min_buf[3];
		}
  }
}

/*******************************************************************************
* 函 数 名         : NumSetTask
* 函数功能		  		 : 存储任务
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void NumSetTask(void) _task_ 7
{
	char event1,x;

	while(1)
	{
		if(set_mark==0)
		{
			for(x=0;x<3;x++)
			{
				key_buf[x]=40;
			}
		}

		
		event1=os_wait2(K_SIG | K_TMO,250);
		
			if(set_mark==1)
			{
				if(event1==SIG_EVENT)
				{
					//时间设置
					if(pos_i<=2)
					{
						if(key_buf[1]!=40)
						{
							uca_wr_time_dat[2-pos_i]=key_buf[0]*16+key_buf[1];
							P8563_SendStr(HOUR_REG-pos_i,&uca_wr_time_dat[2-pos_i],7);
							set_mark=0;
							delay_ms(1000);
						}
					}
					
					//时间段设置
					else if(pos_i==4)
					{
						if(key_buf[1]!=40)
						{
							time_cut[cut_pos]=key_buf[0]*10+key_buf[1];
							
							At24c02Write(cut_pos,time_cut[cut_pos]);
							delay_ms(200);
							delay_ms(200);
							set_mark=0;
						}
					}else if(pos_i==5)
					{
						if(key_buf[1]!=40)
						{
							time_cut[cut_pos+1]=key_buf[0]*10+key_buf[1];
							
							At24c02Write(cut_pos+1,time_cut[cut_pos+1]);
							delay_ms(200);
							set_mark=0;
						}
					}
					
					//上下限设置
					else if(pos_i==7)
					{
						if(key_buf[2]!=40)
						{
							
							temp_max_buf[cut_pos/2]=(int)(key_buf[0]*100+key_buf[1]*10+key_buf[2])*100*ride_flag;
							
							At24c02Write(cut_pos+6,(char)(temp_max_buf[cut_pos/2]));
							delay_ms(100);
							At24c02Write(cut_pos+7,(char)((temp_max_buf[cut_pos/2])>>8));
							delay_ms(100);
							set_mark=0;
						}
					}
					else if(pos_i==8)
					{
						if(key_buf[2]!=40)
						{
							temp_min_buf[cut_pos/2]=(int)(key_buf[0]*100+key_buf[1]*10+key_buf[2])*100*ride_flag;
							At24c02Write(cut_pos+12,(char)(temp_min_buf[cut_pos/2]));
							delay_ms(100);
							At24c02Write(cut_pos+13,(char)(temp_min_buf[cut_pos/2]>>8));
							delay_ms(100);
							set_mark=0;
						}
					}
				}
				
				if(pos_i==3)
				{
					if(cut_pos>=2)
						cut_pos-=2;
					set_mark=0;
				}
				else if(pos_i==6)
				{
					if(cut_pos<4)
						cut_pos+=2;
					set_mark=0;
				}
			}

	}
}





 