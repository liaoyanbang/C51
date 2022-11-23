#ifndef _iic_H_
#define _iic_H_

#define uchar unsigned char
#define uint unsigned int 


//void delay(unsigned int t);
void delay_IIC(void);
void IIC_Init(void);
void IIC_start(void);
void IIC_stop(void); 
bit IIC_Tack(void);
void IIC_write_byte(unsigned char Data);
unsigned char IIC_read_byte();
void IIC_single_byte_write(unsigned char Daddr,unsigned char Waddr,unsigned char Data);
unsigned char IIC_single_byte_read(unsigned char Daddr,unsigned char Waddr);

#endif 