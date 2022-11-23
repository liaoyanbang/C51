#ifndef __1602_H__
#define __1602_H__
void delay(unsigned int ms);
void writelcd_cmd(unsigned char cmd);
void writelcd_dat(unsigned char dat);
void lcd_init();
#endif