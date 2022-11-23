#ifndef _iic_H_
#define _iic_H_

#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int 

void delay(unsigned int z);
void delay_5us();
void I2C_init();
void I2C_Start();
void I2C_Stop();
void I2C_Send(unsigned char byte);
unsigned char I2C_Get();
void I2C_ACK_Send(bit A);
bit I2C_ACK_Get();
 
void I2C_Write(unsigned char add_7,unsigned char add,unsigned char byte);
unsigned char I2C_Read(unsigned char add_7,unsigned char add);

#endif 



