/********************************
ADS1015.H

ADS1015操作的头文件
********************************/

#ifndef __ADS1015_H__
#define __ADS1015_H__

//ADS1015所有寄存器地址定义
#define CONFIG_REG				1
#define CONVERSION_REG		0

#define ADS1015_SLA_ADDR  0x90

/*************************函数列表*************************/

//ADS1015初始化
bit Start_conversion(unsigned char sla_addr, unsigned char sub_addr, unsigned char ch);
//从ADS1015读取转换结果
bit Read_conversion(unsigned char sla_addr, unsigned char sub_addr, unsigned char *rec_buf);

#endif