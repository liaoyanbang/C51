#ifndef __USART_H
#define __USART_H

#define USART_REC_LEN  			30  													//�����������ֽ��� 5
	  	
extern xdata unsigned char  USART_RX_BUF[USART_REC_LEN]; 			//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern unsigned char  USART_RX_STA;         						//����״̬���	

void usart_init(void);
void send_byte(unsigned char dat);
void send_string(unsigned char *p);

#endif
