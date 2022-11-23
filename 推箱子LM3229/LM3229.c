#include <reg52.h>
#include <Text1.h> //关卡的界面
#include <Text2.h> //在此文件中定义了汉字编码，用点阵液晶字模提取软件
#include <math.h>
//#include "iic.h"
#include "shuma.h"
//#include "uart.h"
//#include "led.h"
#include "delay.h"
#include "RTX51TNY.h"
#include "music.h"
#define uc unsigned char
#define ui unsigned int
#define AT24C02_ADDR 0xa0  //AT24C02地址

#define d P0	  //数据口
sbit cd=P3^0;	  //=0，数据通道；=1，命令通道
sbit rd=P3^1;	  //=0，读选通有效
sbit wr=P3^2;	  //=0，写选通有效
char hp=100;//基地血量
uc renh=0,renl=1,startf=0,guan=1,jiemian=0,money=50,count=0,Count1=10,TAcount=0;
//  光标行  列   游戏标志   关     界面     钱     已出怪数 总怪数     塔数   
/*
startf  0    1     2    3   4 
       暂停 进行 结束 启动 选关*/
int fenshu=0;//分数
extern unsigned int tone;			     //tone保存音调
 typedef xdata struct   GUAI
{
	uc x;
	uc y;
	uc type;
	char guaihp;
} GUAI;

typedef xdata struct  TA
{
	uc x;
	uc y;
	uc type;
	uc fanwei;
	uc shanghai;
	uc pinshu;//多少频数才攻击一次
}TA;

uc idata rammap[8][10];
TA idata ta[5];

void disguangbiao();//显示光标
void disjiemian0();//游戏开始界面
void disjiemian1();//关卡选择界面
void disjiemian2();
void disfenshu();//实时显示分数
void HP1();//显示血量
void disxg();
void Money1();
void keychuli(uc  k);
void xuanguan(uc k);		 //选关处理
void huifumubiao();		//恢复目标显示（踩的目标）
void game(uc k);			//游戏控制
void clear();	//清屏
void huata(); //画塔
void huaguai();//画怪
void shengli();//判断是否胜利，在循环的最后面
uc key();
void huamap();
void guaidong(GUAI guai[]);


void lcddatw(uc dat)  //写数据
{
cd=0;
wr=0;
d=dat;
wr=1;
}

void lcdcmd0(uc cmd)	//写命令
{
cd=1;
wr=0;
d=cmd;
wr=1;
}

void lcdcmd1(uc dat1,uc cmd)  //写带有一个参数的命令
{
lcddatw(dat1);
lcdcmd0(cmd);
}

void lcdcmd2(uc dat1,uc dat2,uc cmd)  //写带有两个参数的命令
{
lcddatw(dat1);
lcddatw(dat2);
lcdcmd0(cmd);
}

void lcdinit()	   //LCD初始化
{
lcdcmd2(0x00,0x10,0x40); //设置文本显示区首地址
lcdcmd2(30,0x00,0x41); //设置文本显示区宽度
lcdcmd2(0x00,0x00,0x42); //设置图形显示区首地址
lcdcmd2(30,0x00,0x43); //设置图形显示区宽度
lcdcmd0(0xa2); //设置光标形状
lcdcmd0(0x80); //设置显示方式，文本图形“或”
lcdcmd0(0x9e); //设置光标闪烁，不显示，文本与图形显示
}

void dischar(uc row,uc col,uc cha) //写ASCII码字符，row字符行（=8图形行），col字符列（=8图形列），chaASCII码
{  //显示ASCII码时，16行x30列
ui address;
uc dat1,dat2;
address=row*30+col+0x1000; //计算文本显示区地址
dat1=address;
dat2=address>>8;
lcdcmd2(dat1,dat2,0x24);//地址指针定位
lcdcmd1(cha,0xc4); //写字符，地址指针不变
}

//数字和汉字代码写入自定义字符存储区
void setcgram(uc count)  //count是要写的汉字个数
{//定义自定义字符在GCRAM中存储区地址，自定义字符首地址0x80（固定值），一个字符8x8=8字节，对应字节地址0x0400
ui i;
lcdcmd2(0x03,0x00,0x22); //定义自定义字符（在GCRAM中）存储区字节地址高5位为0x03，
lcdcmd2(0x00,0x1c,0x24); //0x03<<11+0x0400=0x1c00（字节地址）
for(i=0;i<16*8*12/8;i++)
lcdcmd1(shucode[i],0xc0);	//写数字（每个16*8）*12（0-9,:）到GCRAM
for(i=0;i<16*16*count/8;i++)
lcdcmd1(hancode[i],0xc0);	 //写汉字（每个16*16）*count到GCRAM
}
	 //写汉字代码，row行，col列，sit汉字在自定义字符中的位置,每个汉字相当于4个字符(2行2列)
void dishan(uc row,uc col,uc sit)//		sit位置用han[]描述
{
dischar(row,col,sit);//每个汉字按左上，左下，右上，右下存放
dischar(row+1,col,sit+1);
dischar(row,col+1,sit+2);
dischar(row+1,col+1,sit+3);
}
 //写数字代码，row行，col列，sit数字在自定义字符中的位置,每个数字相当于2个字符(2行1列)
void disshu(uc row,uc col,uc sit)//	sit位置用shu[]描述
{
dischar(row,col,sit);//每个汉字按左上，左下，右上，右下存放
dischar(row+1,col,sit+1);
}

void updata()//刷新回原来的坐标
{
		uc i,j;
	for(i=0;i<8;i++)
	for(j=0;j<10;j++)
	{
		//dishan(2*i,2*j,han[map[guan-1][i][j]]);
		rammap[i][j]=map[guan-1][i][j];//
		//if(rammap[i][j]==0x07) rammap[i][j]=0x00;
	}
}
void disjiemian0()//开始界面
{
	dishan(0,6,han[1]);
	dishan(0,8,han[12]);
	dishan(0,10,han[15]);
	dishan(0,12,han[16]);
	/*dischar(2,10,'u');
	dischar(2,11,'p');
	dischar(4,2,'l');
	dischar(4,3,'e');
	dischar(4,4,'f');
	dischar(4,5,'t');
	dischar(4,15,'r');
	dischar(4,16,'i');
	dischar(4,17,'g');
	dischar(4,18,'h');
	dischar(4,19,'t');
	dischar(6,9,'d');
	dischar(6,10,'o');
	dischar(6,11,'w');
	dischar(6,12,'n');*/
	dishan(4,22,han[9]);
	disshu(4,24,shu[0]);
	disshu(4,25,shu[guan]);
	dishan(4,26,han[10]);
	//disxg();
	
}

void HP1()		//显示血量
{
	uc b1=0,b2=0,b3=0;
	if(hp>999)  {b1=9;b2=9;b3=9;}
	else {b1=hp/100%10;b2=hp/10%10;b3=hp%10%10;}
	disshu(12,23,shu[b1]);disshu(12,24,shu[b2]);disshu(12,25,shu[b3]);
	dishan(12,21,han[17]);
}
void Money1()//显示金钱
{
	uc b1=0,b2=0,b3=0;
	if(money>999)  {b1=9;b2=9;b3=9;}
	else {b1=money/100%10;b2=money/10%10;b3=money%10%10;}
	disshu(10,23,shu[b1]);disshu(10,24,shu[b2]);disshu(10,25,shu[b3]);
	dishan(10,21,han[18]);
}
void dismess()
{
	HP1();
	Money1();
}

GUAI guai1(GUAI guai)
{
	guai.x=0;
	guai.y=2;
	guai.guaihp=10;
	guai.type=12;
	//dishan(guai.x*2,guai.y*2,han[guai.type]);
	return guai;
}
void deleteguai(GUAI guai[],uc i)
{
	GUAI temp;
	for(i=i;i<9;i++)
	{
		temp=guai[i];
		guai[i]=guai[i+1];
		guai[i+1]=temp;
	}

}
void huaguai(GUAI guai[])//显示出怪
{
	uc i=0;
	for(i=0;i<Count1;i++)
	{
		dishan(guai[i].x*2,guai[i].y*2,han[0]);
		dishan(guai[i].x*2,guai[i].y*2,han[guai[i].type]);
	}
}
void guaidong(GUAI guai[])
{
	uc x,y,type,i=0,k=0,l=0;
	while(i<Count1)
	{
		l++;
		x=guai[i].x;
		y=guai[i].y;
		type=guai[i].type;
		if(rammap[x][y]==0x08||guai[i].guaihp<=0) continue;//已到达人或者死了
		else 
		{
			if(l%2==0)//攻击频率为2 可改
			{
				for(k=0;k>TAcount;k++)
				{
					if(ta[k].fanwei<=(x-ta[k].x)^2+(y-ta[k].y)^2)//怪进入塔的攻击范围
					{
						guai[i].guaihp-=ta[k].shanghai;//造成伤害
						if(guai[i].guaihp<=0)
						{
							money+=8;//加钱
							fenshu+=50;//加分
							deleteguai(guai,i);//删除死怪
							Count1--;
						}
					}
				}
			}
			if(rammap[x+1][y]==0x00||rammap[x+1][y]==0x08)//怪的下面是空白，往下走
				{
					//dishan(x*2,y*2,han[0]);
					x++;
					//dishan(x*2,y*2,han[type]);
				}
				else if(rammap[x][y+1]==0x00||rammap[x][y+1]==0x08)//右
				{
					//dishan(x*2,y*2,han[0]);
					y++;
					//dishan(x*2,y*2,han[type]);
				}
				else if(rammap[x][y-1]==0x00||rammap[x][y-1]==0x08)
				{
					//dishan(x*2,y*2,han[0]);
					y--;
					//dishan(x*2,y*2,han[type]);
				}
			guai[i].x=x;
			guai[i].y=y;
				
			updata();//
			//dishan(12,18,han[8]);
			delay(10);
			if(rammap[guai[i].x][guai[i].y]==0x08) //移动到人时 造成伤害  
			{
				hp=hp-10;
				deleteguai(guai,i);//删除怪 不用i++
				Count1--;
			}
			else i++;
			if(Count1==0) break;
		}
		
	}
	
}

void huata()
{

	uc i=0;
	for(i=0;i<TAcount;i++)
	{
		dishan(ta[i].x*2,ta[i].y*2,han[ta[i].type]);
	}
	
}	

void huamap()	  //画关卡地图
{
	uc i,j;
	clear();
	for(i=0;i<8;i++)
		for(j=0;j<10;j++)
		{
			dishan(2*i,2*j,han[map[guan-1][i][j]]);
			rammap[i][j]=map[guan-1][i][j];
			//if(rammap[i][j]==0x07) rammap[i][j]=0x00;
		}
	dismess();//显示游戏信息
	dishan(renh*2,renl*2,han[11]);
}


void disxg()		   //显示选关提示
{
dishan(0,22,han[19]);
dishan(0,24,han[20]);
dishan(0,26,han[10]);
disshu(0,28,shu[10]);
}

void shengli()			 //过关判断
{
	if(Count1==0&&hp>0)//没怪了 基地还有血
	{
		startf=2;//结束
		fenshu=hp+money+1000+fenshu;
	}
	else if(hp<=0)
	{
		startf=2;//结束
		fenshu=hp+money+500+fenshu;
	}
}
void disfenshu()//任务三 实时显示分数
{
	DigDisplay(fenshu);
}

uc key()		   //检测按键
{
	uc x,k=0;
	P1=0Xff;
	x=P1;
	if(x==0xff)	   //没按键跳出
	return k;
	else
	{
		delay(0);	   //有按键，延时
		P1=0xff;
		x=P1;
		if(x==0xff)		//再测
		k=0;
		else
		{
			switch(x)	   //获取键值
			{
				case 0xfe:k=1;break;
				case 0xfd:k=2;break;
				case 0xfb:k=3;break;
				case 0xf7:k=4;break;
 				case 0xef:k=5;break;
				case 0xdf:k=6;break;
				case 0xbf:k=7;break;
				case 0x7f:k=8;break;
				default:break;
			}
			wait:	   //等待按键释放
			P1=0xff;
			x=P1;
			if(x==0xff) return k;
			else goto wait;
		}
	}
	return k;
}


void xuanguan(uc k)		 //选关处理
{
   if((k==1||k==3)&&guan<5) {guan++;renh=0;renl=1;/*disjiemian0();*/}//五关
   if((k==2||k==4)&&guan>1) {guan--;renh=0;renl=1;/*disjiemian0();*/}
}

void huifumubiao()		//恢复目标显示（踩的目标）
{
uc i,j;
for(i=0;i<8;i++)
for(j=0;j<10;j++)
{
	if(map[guan-1][i][j]!=0x00&&rammap[i][j]==0x00) 	//变空白且不是空白
	{
		//if(renh!=i||renl!=j) 
		
		
		dishan(2*i,2*j,han[map[guan-1][i][j]]);
	}
}	
}

void game(uc k)			//游戏控制
{
	
 if(k==1)			//向上
	{
		if(renh-1>=0)
		{
			if(rammap[renh-1][renl]!=0x00) 		   //上面不是空白
			{
				dishan(renh*2,renl*2,han[0]);				//人原来的位置清0
				rammap[renh][renl]=0x00;
				renh--;							  //上移一行
				dishan(renh*2,renl*2,han[11]);	  //人新位置显示
			}
			else //上面是空白 上面的上面不是空白
			{
				if(rammap[renh-2][renl]!=0x00)
				{
					dishan(renh*2,renl*2,han[0]);				//人原来的位置清0
					rammap[renh][renl]=0x00;
					renh--;//上移一行
					renh--;//上移一行
					dishan(renh*2,renl*2,han[11]);
				}
			}
		}
	}
	if(k==4)
	{
		if(renh+1<=8)
		{
			if(rammap[renh+1][renl]!=0x00) 
			{
				dishan(renh*2,renl*2,han[0]);
				rammap[renh][renl]=0x00;
				renh++;
				dishan(renh*2,renl*2,han[11]);
				
			}
		
			else //上面是空白 上面的上面不是空白
			{
				if(rammap[renh+2][renl]!=0x00)
				{
					dishan(renh*2,renl*2,han[0]);				//光标原来的位置清0
					rammap[renh][renl]=0x00;
					renh++;//下移一行
					renh++;//下移一行
					dishan(renh*2,renl*2,han[11]);
				}
			}
		}
	}
   	if(k==2)
	{
		if(renl-1>=0)
		{
			if(rammap[renh][renl-1]!=0x00) 
			{
				dishan(renh*2,renl*2,han[0]);
				rammap[renh][renl]=0x00;
				renl--;
				dishan(renh*2,renl*2,han[11]);
			}
		else //左面是空白 z面的z面不是空白
			{
				if(rammap[renh][renl-2]!=0x00)
				{
					dishan(renh*2,renl*2,han[0]);				//人原来的位置清0
					rammap[renh][renl]=0x00;
					renl--;//左移一列
					renl--;//左移一列
					dishan(renh*2,renl*2,han[11]);
				}
			}
		}
	}
   	if(k==3)
	{
		if(renl+1<=10)
		{
			if(rammap[renh][renl+1]!=0x00) 
			{
				dishan(renh*2,renl*2,han[0]);
				rammap[renh][renl]=0x00;
				renl++;
				dishan(renh*2,renl*2,han[11]);
			}
		else //y面是空白 y面的y面不是空白
			{
				if(rammap[renh][renl+2]!=0x00)
				{
					dishan(renh*2,renl*2,han[0]);				//人原来的位置清0
					rammap[renh][renl]=0x00;
					renl++;//右移一列
					renl++;//右移一列
					dishan(renh*2,renl*2,han[11]);
				}
			}
		}
	}
}
void disguangbiao()
{
	dishan(renh*2,renl*2,han[11]);
}
void clear()	//清屏
{
	uc i,j;
for(i=0;i<8;i++)
for(j=0;j<15;j++)
{
	dishan(2*i,2*j,han[0]);	 //写0
}
}

void keychuli(uc  k)	  //按键处理
{
	if(k==6)	
	{
		startf=4;
		disxg();
	}	  //键值为6，“选关”
	else if(k==7)	
	{
	  clear();
		startf=1;
		hp=100;
		money=50;
		renl=1;
		renh=0;
		Count1=10;
		count=0;
		huamap();
	 }	//键值为7，“开始”
	else if(k==8)//暂停
	{
		if(startf==1) startf=0;
		else startf=1;
		
	}
	else if(k==5)//建塔/确定
	{
		if(startf==1)//游戏进行中
		{
			if(rammap[renh][renl]!=0x00)//确认光标
			{
				if(money>=50)
				ta[TAcount].x=renh;
				ta[TAcount].y=renl;
				ta[TAcount].type=21;
				ta[TAcount].fanwei=sqrt(2);
				ta[TAcount].shanghai=5;
				ta[TAcount].pinshu=2;
				TAcount++;//塔数加一
				money-=50;;//扣钱
			}
		}
		else//确认选项
		{
			
			
		}
	}
	else if(k<5)
	{
		if(startf==4||startf==3)	xuanguan(k);  //选关处理
		else	if(startf==1)		
		{
			//LED=~LED;
			game(k);	  //游戏控制
			//huifumubiao();//恢复踩过的图案
			updata();//刷新为原坐标
		}
	}
	else
	{
		
		return;
	}
}
// 串口中断处理函数 （串口接收到数据，发送数据完毕都可以引起串口中断）
/*void uart_interrupt(void) interrupt 4 		//也叫串行中断服务程序
{
	 /*接收数据(1字节)完毕，RI会被硬件置1
		RI = 0;           		// 将接收中断标志位 清零(让串口可以继续接收数据)
		receiveData = SBUF;    		//读取接收到的数据，并存放到data
		LED=0;
		delay(1000);
		flag=1;*/
		/*RI=0;
		IIC_start();
IIC_read_byte();
		IIC_stop();
	
}*/
/*void main()
{
	uc i=0,ch=0;
	GUAI idata guai[10];
	lcdinit();
	//IIC_Init();
	//UART_init();
	setcgram(24);	 //写入汉字
	disjiemian0();
	delay(500);
	huamap();
	while(1)
	{	
		if(i<10)
		{
			guai[i]=guai1(guai[i]);
			i++;
		}
		guaidong(guai);//让怪走起来
		dismess();
		//IIC_start();
		//ch=IIC_read_byte();
		//IIC_stop();
		keychuli(key());
		delay(1500);
		
	}
}
*/


void task_1(void) _task_ 1    // 怪坐标变化
{
	GUAI  guai[10];
	while(1)
	{
	if(startf==1)
	{
		clear();
		huamap();
		dismess();
		disguangbiao();
		if(count<10)
		{
			guai[count]=guai1(guai[count]);
			count++;
		}
		huaguai(guai);//先画 还是先动？
		guaidong(guai);//让怪走起来
		huata();
		os_wait2(K_IVL,1000);
		shengli();
	}	
	else if(startf==2)//游戏结束
	{
		disxg();
	}
	else if(startf==3)//启动界面
	{
		disjiemian0();
	}
	else if(startf==0)//游戏暂停
	{
		return;
	}
	else if(startf==4)
	{
		disxg();
	}
}
}
void task_2(void) _task_ 2  //按键处理  光标变换
{
	uc ch;
	while(1)
	{
		
		/*ch=IIC_single_byte_read(AT24C02_ADDR,255);
		if(ch!=0&&ch<=6)	keychuli(ch);
		os_wait2(K_IVL,1000);*/
		
		
		ch=key();
		if(ch!=0)
		keychuli(ch);
		os_wait2(K_IVL,100);
	}		
}

void task_3(void) _task_ 3   //实时显示分数
{
	while(1)
	{
		
    
	}
	
}
void task_4(void) _task_ 4//播放音乐
{
	while(1)
	{
		//if(startf==1)
		if(tone!=0)play_tone(tone);	 //一直在播放音调，播放的音调由定时器切换，
									 //音调时常由定时器决定
	}									 //如果tone==0 不进入播放
}
/*******************************************************************************
* 函 数 名       : task_create
* 函数功能		 	 : 任务0
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void task_create(void) _task_ 0
{
	
	lcdinit();
	time_init();	
	//IIC_Init();
	//UART_init();
	setcgram(22);	 //写入汉字
	huamap();
	//os_create_task(2);
	//os_create_task(1);
	os_create_task(4);
	os_delete_task(0);//挂起任务0
}
