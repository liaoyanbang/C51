#include "delay.h"

void delay(unsigned int ms) //����
{	
    unsigned char i,j;
		for(j=ms; j>0; j--)	
			for(i=100;i>0;i--);	
}
