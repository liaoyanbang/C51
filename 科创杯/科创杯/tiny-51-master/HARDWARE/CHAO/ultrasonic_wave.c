#include "ultrasonic_wave.h"//������ͷ����


//��������ʼ��
void Init_ultrasonic_wave()
{
	 TX1 = 0;			   //�رշ���
	 TX2 = 0;					
	 TX3 = 0;
	 TX4 = 0;
	 TX5 = 0;
	 TX6 = 0;
	 TX7 = 0;
	//�رշ���
}

//����������
void StartModule1() 	//����ģ��
{
	TX1=1;			      //����һ��ģ��
	os_wait2(K_TMO,1);
	TX1=0;
}
//����������
void StartModule2() 	//����ģ��
{
	TX2=1;			      //����һ��ģ��
	os_wait2(K_TMO,1);
	TX2=0;
}
//����������
void StartModule3() 	//����ģ��
{
	TX3=1;			      //����һ��ģ��
	os_wait2(K_TMO,1);
	TX3=0;
}
//����������
void StartModule4() 	//����ģ��
{
	TX4=1;			      //����һ��ģ��
	os_wait2(K_TMO,1);
	TX4=0;
}


//����������
void StartModule5() 	//����ģ��
{
	TX5=1;			      //����һ��ģ��
	os_wait2(K_TMO,1);
	TX5=0;
}
//����������
void StartModule6() 	//����ģ��
{
	TX6=1;			      //����һ��ģ��
	os_wait2(K_TMO,1);
	TX6=0;
}


//����������
void StartModule7() 	//����ģ��
{
	TX7=1;			      //����һ��ģ��
	os_wait2(K_TMO,1);
	TX7=0;
}


