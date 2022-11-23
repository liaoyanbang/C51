#include <reg51.h>
sbit p1_0 = P1^0;
unsigned char count = 1;//�����ı�LED״̬

//��ʼ������ �����ô��ڣ����������жϣ�
void init_uart(void)
{
	SCON = 0x50;		        // SCON: ��ʽ 1, 8-bit, ����������� 
	TMOD |= 0x20;               // TMOD: ���ö�ʱ��1�����ڷ�ʽ2, 8-bit �Զ���װ
	TH1 = 0xFD;               // TH1:  ��ʼֵΪ0xFD  �����ʣ�9600 ����Ƶ�ʣ�11.0592MHz  
	TL1 = 0x0;
	TR1 = 1;                  // TR1:  ������ʱ��1                         
	EA  = 1;                  //�����ж�
	ES  = 1;                  //�򿪴����ж�
}       

           
// ����һ���ֽ�����
void uart_send_byte(unsigned char dat)
{
	if(TI == 0)//��һ�η������
	{
		SBUF = dat; // �������͵����ͻ���Ĵ���SBUF��һλһλ�ķ���
		while(!TI); // �ȴ�������� (�������TIӲ����1)
	}
}

// �����ַ���
void uart_send_str(unsigned char *s)
{
	while(*s != '\0')// '\0':�ַ���������־
	{
		uart_send_byte(*s);// ����1���ֽ����ݣ�1���ַ�ռ8λ��1�ֽ�
		s++;// ָ����һ���ַ�
	}
}


// �����жϴ����� �����ڽ��յ����ݣ�����������϶��������𴮿��жϣ�
void uart_interrupt(void) interrupt 4 		//Ҳ�д����жϷ������
{
	unsigned char recv_data;// ������Ž��յ�������
	//unsigned char send_data[] = "ok";// Ҫ���͵���Ϣ
	
	if(RI) //��������(1�ֽ�)��ϣ�RI�ᱻӲ����1
	{
		RI = 0;           		// �� �����жϱ�־λ ����(�ô��ڿ��Լ�����������)
		recv_data = SBUF;           	//��ȡ���յ������ݣ�����ŵ�data
		
		count++;	 
		uart_send_byte(recv_data);//�յ�ʲô�ͻظ�ʲô
		p1_0 = count%2;//ÿ�յ�1���ַ�����LED�ı�״̬

	}
	if(TI)// ��������(1�ֽ�)���
	{
		TI = 0;// �� �����жϱ�־λ ����(�ô��ڿ��Լ�����������)
	}
} 

void main(void)
{
	init_uart();// ��ʼ������
	
	while(1)// ��ѭ�������κζ���������ѭ����
	{}
}

