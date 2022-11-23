#ifndef __DS18B20_H__
#define __DS18B20_H__
void dsreset();
bit tmpreadbit();
unsigned char tmpread();
void tmpwritebyte(unsigned char dat);
void tmpchange();
unsigned int tmp();
#endif 