#include "reg52.h"
#include "intrins.h"

void delay_ms( unsigned int t)	;	 //毫秒延时12MHz下
void delay_us( unsigned int t)	;	 //微妙延时12MHz下
void play_tone(unsigned int tone);	 //播放音调函数，即方波发生器
void time_init( void );				 //初始化定时器
void delay_ms(unsigned int t);		 //毫秒延时12MHz下
void delay_us(unsigned int t);		 //us延时 12MHz下