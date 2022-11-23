#ifndef __WAVE_H
#define __WAVE_H	
#include <reg52.h>
#include <intrins.h>
#include "RTX51TNY.h"
//宏定义
#define uchar unsigned char 
#define uint unsigned int
#define ULint unsigned long int 
//超声波管脚定义
sbit RX1=P2^1;
sbit TX1=P2^0;
sbit RX2=P2^3;
sbit TX2=P2^2;
sbit RX3=P2^5;
sbit TX3=P2^4;
sbit RX4=P2^7;
sbit TX4=P2^6;
sbit RX5=P3^3;
sbit TX5=P3^2;
sbit RX6=P3^5;
sbit TX6=P3^4;
sbit RX7=P1^5;
sbit TX7=P1^4;


void Init_ultrasonic_wave();
void StartModule1();
void StartModule2();
void StartModule3();
void StartModule4();
void StartModule5();
void StartModule6();
void StartModule7();


#endif