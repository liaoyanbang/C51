#ifndef __USART_H
#define __USART_H

#define USART_REC_LEN  			30  													//定义最大接收字节数 5
	  	
extern xdata unsigned char  USART_RX_BUF[USART_REC_LEN]; 			//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern unsigned char  USART_RX_STA;         						//接收状态标记	

void usart_init(void);
void send_byte(unsigned char dat);
void send_string(unsigned char *p);

#endif
