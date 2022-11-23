//******************************************
/*------------------------------------------
               加载头文件
------------------------------------------*/
//******************************************
#include "main.h"   
#include "I2C.h"


/*
#define	YEAR_REG	0x08
#define MONTH_REG	0x07
#define WEEK_REG	0x06
#define DATA_REG	0x05
#define HOUR_REG	0x04
#define MIN_REG		0x03
#define SEC_REG		0x02*/

#define P8563_SLA	0xA2


/*********************************全局函数定义******************************/
//向P8563内指定寄存器写入值
bit P8563_SendStr(unsigned char sub_addr, unsigned char *send_buf, unsigned char data_no);
//从P8563内指定寄存器读出值
bit P8563_RcvStr(unsigned char sub_addr, unsigned char *rec_buf, unsigned char data_no);




/**********************************
向P8563内指定寄存器写入值
函数原形：P8563_SendStr(unsigned char sub_addr, unsigned char *send_buf, unsigned char data_no)
函数功能：从启动总线到发送地址，子地址,数据，结束总线的全过程,
		  子地址suba，发送内容是s指向的内容，
		  发送no个字节。如果返回1表示操作成功，否则操作有误。
入口参数：unsigned char sub_addr, 
		  unsigned char *send_buf, 
		  unsigned char data_no
出口参数：返回是否成功标志位
注意：    使用前必须已结束总线。
**********************************/
bit P8563_SendStr(unsigned char sub_addr, unsigned char *send_buf, unsigned char data_no)
{
		bit	rec_result;

		rec_result = ISendStr(P8563_SLA, sub_addr, send_buf, data_no);
		
		return(rec_result);
}





/**********************************
//从P8563内指定寄存器读出值
函数原形：P8563_RcvStr(unsigned char sub_addr, unsigned char *rec_buf, unsigned char data_no)
函数功能：从启动总线到发送地址，子地址,数据，结束总线的全过程,
		  子地址suba，发送内容是s指向的内容，
		  发送no个字节。如果返回1表示操作成功，否则操作有误。
入口参数：unsigned char sub_addr, 
		  unsigned char *rec_buf, 
		  unsigned char data_no
出口参数：返回是否成功标志位
注意：    使用前必须已结束总线。
**********************************/
bit P8563_RcvStr(unsigned char sub_addr, unsigned char *rec_buf, unsigned char data_no)
{
		bit	rec_result;

		rec_result = IRcvStr(P8563_SLA, sub_addr, rec_buf, data_no);

		rec_buf[0] &= 0x7F;
		rec_buf[1] &= 0x7F;
		rec_buf[2] &= 0x3F;
		rec_buf[3] &= 0x3F;
		rec_buf[4] &= 0x07;
		rec_buf[5] &= 0x1F;
		
		return(rec_result);
}
 
 
 
 	