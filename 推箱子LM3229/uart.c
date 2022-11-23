#include "uart.h"
#include "reg52.h"
#include "led.h"
#include "delay.h"

void UART_init()
{
	//���ò�����9600
	TMOD|=0x20;  //��ʱ��1�������Զ���װ��ģʽ
	TH1=0xfd;
	TH0=0xfd;
	TR1=1; //������ʱ��1
	ET1=0; //��ֹ��ʱ��1�ж�
	PCON&=0x7f; //����SMOD=0�������ʲ��ӱ�
	SCON=0x50; //�����ڷ�ʽ1  8λ��У���첽ͨ�ŵ��շ�ģʽ��������շ��жϱ�־λ
	EA=1;  //���ж�����
	ES=1;//�����շ��жϣ��������в�ʹ�ô����жϣ�
}

void UART_Send_Data(unsigned char byte)
{
	SBUF=byte;
	while(!TI);//�ȴ��������ݷ������
	TI=0;//�������жϱ�־λ��0��Ϊ�´η���������׼��
}

unsigned char UART_Receive_Data()
{
	unsigned char temp;
	while(!RI);//�ȴ����ڽ��յ����ݣ�RI����1��
	temp=SBUF;
	RI=0;//��ս����жϱ�־λ��Ϊ�´ν�����׼��
	return temp;
}
