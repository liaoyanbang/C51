#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "RTX51TNY.h"

void dsreset();
bit tmpreadbit();
unsigned char ReadOneChar(void);
void Init_DS18B20(void);
void Delay_DS18B20(int num);
void WriteOneChar(unsigned char dat);
void tmpchange();
unsigned int ReadTemperature(void);
void delay(unsigned int count) ;
#endif 