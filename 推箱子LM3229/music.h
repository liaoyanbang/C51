#include "reg52.h"
#include "intrins.h"

void delay_ms( unsigned int t)	;	 //������ʱ12MHz��
void delay_us( unsigned int t)	;	 //΢����ʱ12MHz��
void play_tone(unsigned int tone);	 //��������������������������
void time_init( void );				 //��ʼ����ʱ��
void delay_ms(unsigned int t);		 //������ʱ12MHz��
void delay_us(unsigned int t);		 //us��ʱ 12MHz��