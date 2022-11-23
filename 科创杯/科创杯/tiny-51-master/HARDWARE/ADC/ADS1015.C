//******************************************
/*------------------------------------------
               加载头文件
------------------------------------------*/
//******************************************
#include "i2c.h"
#include "main.h"


//ADS1015初始化
bit Start_conversion(unsigned char sla_addr, unsigned char sub_addr, unsigned char ch);
//从ADS1015读取转换结果
bit Read_conversion(unsigned char sla_addr, unsigned char sub_addr, unsigned char *rec_buf);


/**********************************
 
函数原形：bit Start_conversion(unsigned char sla_addr, unsigned char sub_addr, unsigned char ch)
函数功能：ADS1015初始化	   
入口参数：unsigned char sla_addr, unsigned char sub_addr, unsigned char ch 
出口参数：返回是否成功标志位
注意：    使用前必须已结束总线。
**********************************/
bit Start_conversion(unsigned char sla_addr, unsigned char sub_addr, unsigned char ch)
{
		bit	rec_result;
		unsigned char uca_wr_dat[2];
		
		if (ch == 0)
		{
				uca_wr_dat[0] = 0xC1; 
		}
		else	if (ch == 1)
		{
				uca_wr_dat[0] = 0xD1; 
		}
		else	if (ch == 2)
		{
				uca_wr_dat[0] = 0xE1; 
		}
		else	if (ch == 3)
		{
				uca_wr_dat[0] = 0xF1; 
		}
		uca_wr_dat[1] = 0x00;

		rec_result = ISendStr(sla_addr, sub_addr, uca_wr_dat, 2);
		
		return(rec_result);
}


/**********************************
函数原形：bit Read_conversion(unsigned char sla_addr, unsigned char sub_addr, unsigned char *rec_buf)
函数功能：从ADS1015读取转换结果 	  	   
入口参数：unsigned char sla_addr, unsigned char sub_addr, unsigned char *rec_buf	    	   
出口参数：返回是否成功标志位
注意：    使用前必须已结束总线。
**********************************/
bit Read_conversion(unsigned char sla_addr, unsigned char sub_addr, unsigned char *rec_buf)
{
		bit	rec_result;

		rec_result = IRcvStr(sla_addr, sub_addr, rec_buf, 2);
		
		return(rec_result);
}
 
 
 
 	