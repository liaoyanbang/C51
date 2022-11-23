#ifndef __AT24C02_H_
#define __AT24C02_H_

#include "main.h"

sbit SCL1=P0^4;
sbit SDA1=P0^5;

void I2cStart();
void I2cStop();
unsigned char I2cSendByte(unsigned char dat);
unsigned char I2cReadByte();
void At24c02Write(unsigned char addr,unsigned char dat);
unsigned char At24c02Read(unsigned char addr);

#endif
