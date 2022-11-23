#include <reg52.h>
#include <Text1.h> //�ؿ��Ľ���
#include <Text2.h> //�ڴ��ļ��ж����˺��ֱ��룬�õ���Һ����ģ��ȡ���
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
#define AT24C02_ADDR 0xa0  //AT24C02��ַ

#define d P0	  //���ݿ�
sbit cd=P3^0;	  //=0������ͨ����=1������ͨ��
sbit rd=P3^1;	  //=0����ѡͨ��Ч
sbit wr=P3^2;	  //=0��дѡͨ��Ч
char hp=100;//����Ѫ��
uc renh=0,renl=1,startf=0,guan=1,jiemian=0,money=50,count=0,Count1=10,TAcount=0;
//  �����  ��   ��Ϸ��־   ��     ����     Ǯ     �ѳ����� �ܹ���     ����   
/*
startf  0    1     2    3   4 
       ��ͣ ���� ���� ���� ѡ��*/
int fenshu=0;//����
extern unsigned int tone;			     //tone��������
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
	uc pinshu;//����Ƶ���Ź���һ��
}TA;

uc idata rammap[8][10];
TA idata ta[5];

void disguangbiao();//��ʾ���
void disjiemian0();//��Ϸ��ʼ����
void disjiemian1();//�ؿ�ѡ�����
void disjiemian2();
void disfenshu();//ʵʱ��ʾ����
void HP1();//��ʾѪ��
void disxg();
void Money1();
void keychuli(uc  k);
void xuanguan(uc k);		 //ѡ�ش���
void huifumubiao();		//�ָ�Ŀ����ʾ���ȵ�Ŀ�꣩
void game(uc k);			//��Ϸ����
void clear();	//����
void huata(); //����
void huaguai();//����
void shengli();//�ж��Ƿ�ʤ������ѭ���������
uc key();
void huamap();
void guaidong(GUAI guai[]);


void lcddatw(uc dat)  //д����
{
cd=0;
wr=0;
d=dat;
wr=1;
}

void lcdcmd0(uc cmd)	//д����
{
cd=1;
wr=0;
d=cmd;
wr=1;
}

void lcdcmd1(uc dat1,uc cmd)  //д����һ������������
{
lcddatw(dat1);
lcdcmd0(cmd);
}

void lcdcmd2(uc dat1,uc dat2,uc cmd)  //д������������������
{
lcddatw(dat1);
lcddatw(dat2);
lcdcmd0(cmd);
}

void lcdinit()	   //LCD��ʼ��
{
lcdcmd2(0x00,0x10,0x40); //�����ı���ʾ���׵�ַ
lcdcmd2(30,0x00,0x41); //�����ı���ʾ�����
lcdcmd2(0x00,0x00,0x42); //����ͼ����ʾ���׵�ַ
lcdcmd2(30,0x00,0x43); //����ͼ����ʾ�����
lcdcmd0(0xa2); //���ù����״
lcdcmd0(0x80); //������ʾ��ʽ���ı�ͼ�Ρ���
lcdcmd0(0x9e); //���ù����˸������ʾ���ı���ͼ����ʾ
}

void dischar(uc row,uc col,uc cha) //дASCII���ַ���row�ַ��У�=8ͼ���У���col�ַ��У�=8ͼ���У���chaASCII��
{  //��ʾASCII��ʱ��16��x30��
ui address;
uc dat1,dat2;
address=row*30+col+0x1000; //�����ı���ʾ����ַ
dat1=address;
dat2=address>>8;
lcdcmd2(dat1,dat2,0x24);//��ַָ�붨λ
lcdcmd1(cha,0xc4); //д�ַ�����ַָ�벻��
}

//���ֺͺ��ִ���д���Զ����ַ��洢��
void setcgram(uc count)  //count��Ҫд�ĺ��ָ���
{//�����Զ����ַ���GCRAM�д洢����ַ���Զ����ַ��׵�ַ0x80���̶�ֵ����һ���ַ�8x8=8�ֽڣ���Ӧ�ֽڵ�ַ0x0400
ui i;
lcdcmd2(0x03,0x00,0x22); //�����Զ����ַ�����GCRAM�У��洢���ֽڵ�ַ��5λΪ0x03��
lcdcmd2(0x00,0x1c,0x24); //0x03<<11+0x0400=0x1c00���ֽڵ�ַ��
for(i=0;i<16*8*12/8;i++)
lcdcmd1(shucode[i],0xc0);	//д���֣�ÿ��16*8��*12��0-9,:����GCRAM
for(i=0;i<16*16*count/8;i++)
lcdcmd1(hancode[i],0xc0);	 //д���֣�ÿ��16*16��*count��GCRAM
}
	 //д���ִ��룬row�У�col�У�sit�������Զ����ַ��е�λ��,ÿ�������൱��4���ַ�(2��2��)
void dishan(uc row,uc col,uc sit)//		sitλ����han[]����
{
dischar(row,col,sit);//ÿ�����ְ����ϣ����£����ϣ����´��
dischar(row+1,col,sit+1);
dischar(row,col+1,sit+2);
dischar(row+1,col+1,sit+3);
}
 //д���ִ��룬row�У�col�У�sit�������Զ����ַ��е�λ��,ÿ�������൱��2���ַ�(2��1��)
void disshu(uc row,uc col,uc sit)//	sitλ����shu[]����
{
dischar(row,col,sit);//ÿ�����ְ����ϣ����£����ϣ����´��
dischar(row+1,col,sit+1);
}

void updata()//ˢ�»�ԭ��������
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
void disjiemian0()//��ʼ����
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

void HP1()		//��ʾѪ��
{
	uc b1=0,b2=0,b3=0;
	if(hp>999)  {b1=9;b2=9;b3=9;}
	else {b1=hp/100%10;b2=hp/10%10;b3=hp%10%10;}
	disshu(12,23,shu[b1]);disshu(12,24,shu[b2]);disshu(12,25,shu[b3]);
	dishan(12,21,han[17]);
}
void Money1()//��ʾ��Ǯ
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
void huaguai(GUAI guai[])//��ʾ����
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
		if(rammap[x][y]==0x08||guai[i].guaihp<=0) continue;//�ѵ����˻�������
		else 
		{
			if(l%2==0)//����Ƶ��Ϊ2 �ɸ�
			{
				for(k=0;k>TAcount;k++)
				{
					if(ta[k].fanwei<=(x-ta[k].x)^2+(y-ta[k].y)^2)//�ֽ������Ĺ�����Χ
					{
						guai[i].guaihp-=ta[k].shanghai;//����˺�
						if(guai[i].guaihp<=0)
						{
							money+=8;//��Ǯ
							fenshu+=50;//�ӷ�
							deleteguai(guai,i);//ɾ������
							Count1--;
						}
					}
				}
			}
			if(rammap[x+1][y]==0x00||rammap[x+1][y]==0x08)//�ֵ������ǿհף�������
				{
					//dishan(x*2,y*2,han[0]);
					x++;
					//dishan(x*2,y*2,han[type]);
				}
				else if(rammap[x][y+1]==0x00||rammap[x][y+1]==0x08)//��
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
			if(rammap[guai[i].x][guai[i].y]==0x08) //�ƶ�����ʱ ����˺�  
			{
				hp=hp-10;
				deleteguai(guai,i);//ɾ���� ����i++
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

void huamap()	  //���ؿ���ͼ
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
	dismess();//��ʾ��Ϸ��Ϣ
	dishan(renh*2,renl*2,han[11]);
}


void disxg()		   //��ʾѡ����ʾ
{
dishan(0,22,han[19]);
dishan(0,24,han[20]);
dishan(0,26,han[10]);
disshu(0,28,shu[10]);
}

void shengli()			 //�����ж�
{
	if(Count1==0&&hp>0)//û���� ���ػ���Ѫ
	{
		startf=2;//����
		fenshu=hp+money+1000+fenshu;
	}
	else if(hp<=0)
	{
		startf=2;//����
		fenshu=hp+money+500+fenshu;
	}
}
void disfenshu()//������ ʵʱ��ʾ����
{
	DigDisplay(fenshu);
}

uc key()		   //��ⰴ��
{
	uc x,k=0;
	P1=0Xff;
	x=P1;
	if(x==0xff)	   //û��������
	return k;
	else
	{
		delay(0);	   //�а�������ʱ
		P1=0xff;
		x=P1;
		if(x==0xff)		//�ٲ�
		k=0;
		else
		{
			switch(x)	   //��ȡ��ֵ
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
			wait:	   //�ȴ������ͷ�
			P1=0xff;
			x=P1;
			if(x==0xff) return k;
			else goto wait;
		}
	}
	return k;
}


void xuanguan(uc k)		 //ѡ�ش���
{
   if((k==1||k==3)&&guan<5) {guan++;renh=0;renl=1;/*disjiemian0();*/}//���
   if((k==2||k==4)&&guan>1) {guan--;renh=0;renl=1;/*disjiemian0();*/}
}

void huifumubiao()		//�ָ�Ŀ����ʾ���ȵ�Ŀ�꣩
{
uc i,j;
for(i=0;i<8;i++)
for(j=0;j<10;j++)
{
	if(map[guan-1][i][j]!=0x00&&rammap[i][j]==0x00) 	//��հ��Ҳ��ǿհ�
	{
		//if(renh!=i||renl!=j) 
		
		
		dishan(2*i,2*j,han[map[guan-1][i][j]]);
	}
}	
}

void game(uc k)			//��Ϸ����
{
	
 if(k==1)			//����
	{
		if(renh-1>=0)
		{
			if(rammap[renh-1][renl]!=0x00) 		   //���治�ǿհ�
			{
				dishan(renh*2,renl*2,han[0]);				//��ԭ����λ����0
				rammap[renh][renl]=0x00;
				renh--;							  //����һ��
				dishan(renh*2,renl*2,han[11]);	  //����λ����ʾ
			}
			else //�����ǿհ� ��������治�ǿհ�
			{
				if(rammap[renh-2][renl]!=0x00)
				{
					dishan(renh*2,renl*2,han[0]);				//��ԭ����λ����0
					rammap[renh][renl]=0x00;
					renh--;//����һ��
					renh--;//����һ��
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
		
			else //�����ǿհ� ��������治�ǿհ�
			{
				if(rammap[renh+2][renl]!=0x00)
				{
					dishan(renh*2,renl*2,han[0]);				//���ԭ����λ����0
					rammap[renh][renl]=0x00;
					renh++;//����һ��
					renh++;//����һ��
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
		else //�����ǿհ� z���z�治�ǿհ�
			{
				if(rammap[renh][renl-2]!=0x00)
				{
					dishan(renh*2,renl*2,han[0]);				//��ԭ����λ����0
					rammap[renh][renl]=0x00;
					renl--;//����һ��
					renl--;//����һ��
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
		else //y���ǿհ� y���y�治�ǿհ�
			{
				if(rammap[renh][renl+2]!=0x00)
				{
					dishan(renh*2,renl*2,han[0]);				//��ԭ����λ����0
					rammap[renh][renl]=0x00;
					renl++;//����һ��
					renl++;//����һ��
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
void clear()	//����
{
	uc i,j;
for(i=0;i<8;i++)
for(j=0;j<15;j++)
{
	dishan(2*i,2*j,han[0]);	 //д0
}
}

void keychuli(uc  k)	  //��������
{
	if(k==6)	
	{
		startf=4;
		disxg();
	}	  //��ֵΪ6����ѡ�ء�
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
	 }	//��ֵΪ7������ʼ��
	else if(k==8)//��ͣ
	{
		if(startf==1) startf=0;
		else startf=1;
		
	}
	else if(k==5)//����/ȷ��
	{
		if(startf==1)//��Ϸ������
		{
			if(rammap[renh][renl]!=0x00)//ȷ�Ϲ��
			{
				if(money>=50)
				ta[TAcount].x=renh;
				ta[TAcount].y=renl;
				ta[TAcount].type=21;
				ta[TAcount].fanwei=sqrt(2);
				ta[TAcount].shanghai=5;
				ta[TAcount].pinshu=2;
				TAcount++;//������һ
				money-=50;;//��Ǯ
			}
		}
		else//ȷ��ѡ��
		{
			
			
		}
	}
	else if(k<5)
	{
		if(startf==4||startf==3)	xuanguan(k);  //ѡ�ش���
		else	if(startf==1)		
		{
			//LED=~LED;
			game(k);	  //��Ϸ����
			//huifumubiao();//�ָ��ȹ���ͼ��
			updata();//ˢ��Ϊԭ����
		}
	}
	else
	{
		
		return;
	}
}
// �����жϴ����� �����ڽ��յ����ݣ�����������϶��������𴮿��жϣ�
/*void uart_interrupt(void) interrupt 4 		//Ҳ�д����жϷ������
{
	 /*��������(1�ֽ�)��ϣ�RI�ᱻӲ����1
		RI = 0;           		// �������жϱ�־λ ����(�ô��ڿ��Լ�����������)
		receiveData = SBUF;    		//��ȡ���յ������ݣ�����ŵ�data
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
	setcgram(24);	 //д�뺺��
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
		guaidong(guai);//�ù�������
		dismess();
		//IIC_start();
		//ch=IIC_read_byte();
		//IIC_stop();
		keychuli(key());
		delay(1500);
		
	}
}
*/


void task_1(void) _task_ 1    // ������仯
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
		huaguai(guai);//�Ȼ� �����ȶ���
		guaidong(guai);//�ù�������
		huata();
		os_wait2(K_IVL,1000);
		shengli();
	}	
	else if(startf==2)//��Ϸ����
	{
		disxg();
	}
	else if(startf==3)//��������
	{
		disjiemian0();
	}
	else if(startf==0)//��Ϸ��ͣ
	{
		return;
	}
	else if(startf==4)
	{
		disxg();
	}
}
}
void task_2(void) _task_ 2  //��������  ���任
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

void task_3(void) _task_ 3   //ʵʱ��ʾ����
{
	while(1)
	{
		
    
	}
	
}
void task_4(void) _task_ 4//��������
{
	while(1)
	{
		//if(startf==1)
		if(tone!=0)play_tone(tone);	 //һֱ�ڲ������������ŵ������ɶ�ʱ���л���
									 //����ʱ���ɶ�ʱ������
	}									 //���tone==0 �����벥��
}
/*******************************************************************************
* �� �� ��       : task_create
* ��������		 	 : ����0
* ��    ��       : ��
* ��    ��    	 : ��
*******************************************************************************/
void task_create(void) _task_ 0
{
	
	lcdinit();
	time_init();	
	//IIC_Init();
	//UART_init();
	setcgram(22);	 //д�뺺��
	huamap();
	//os_create_task(2);
	//os_create_task(1);
	os_create_task(4);
	os_delete_task(0);//��������0
}
