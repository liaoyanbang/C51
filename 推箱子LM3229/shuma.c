#include "shuma.h"

unsigned char code wxcode[4]={0xfe,0xfd,0xfb,0xf7};
unsigned char code dxcode[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
#define duanxuan P1       
#define weixuan P2    

void DigDisplay(int num)
{
	unsigned char i;
	unsigned int j;
	int gewei,shiwei,baiwei,qianwei;
	
	gewei = num % 10;
	shiwei = num / 10 % 10;
	baiwei = num / 100 % 10;
	qianwei = num / 1000;
	
	for(i=0; i<4; i++)
	{
		 weixuan = wxcode[i];
		 duanxuan = 0x00;
		if(i == 3)
		{
			duanxuan = dxcode[gewei];
		}
		if(i == 2)
		{
			duanxuan = dxcode[shiwei];
		}
		if(i == 1)
		{
			duanxuan = dxcode[baiwei];
		}
		if(i == 0)
		{
			duanxuan = dxcode[qianwei];
		}
		j=10;
		while(j--);
		duanxuan=0x00;
	}
}