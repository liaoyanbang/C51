/*****************************************************
�ļ�����I2C.c
�ļ����ܣ�ģ��I2C�ĵײ��C�ӳ���
��д�ߣ�����
��д���ڣ�2020-08-10

*****************************************************/




/*ͷ�ļ��İ���*/
#include <absacc.h> 
#include <intrins.h>
#include "I2C.h"



/*************************ȫ�ֱ���*************************/
bit ack;	         /*Ӧ���־λ*/



/*************************����ʵ��*************************/
/**********************************
�������ߺ���
����ԭ�Σ�void Start_I2c(void)
�������ܣ�����I2C����,������I2C��ʼ����
��ڲ�������
���ڲ�������
**********************************/
void Start_I2c(void)
{
		SDA=1;   		  //������ʼ�����������ź�
		_Nop();
		SCL=1;
		_Nop();    		//��ʼ��������ʱ�����4.7us,��ʱ
		_Nop();
		_Nop();
		_Nop();
		_Nop();        
		SDA=0;   		  //������ʼ�ź�
		_Nop();    		//��ʼ��������ʱ�����4us
		_Nop();
		_Nop();
		_Nop();
		_Nop();       
		SCL=0;   		  //ǯסI2C���ߣ�׼�����ͻ��������
		_Nop();
		_Nop();
}




/**********************************
�������ߺ���
����ԭ�Σ�void Stop_I2c(void)
�������ܣ�����I2C����,������I2C��������
��ڲ�������
���ڲ�������
**********************************/
void Stop_I2c(void)
{
		SDA=0;  		  //���ͽ��������������ź�
		_Nop();   		//���ͽ���������ʱ���ź�
		SCL=1;  		  //������������ʱ�����4us
		_Nop();
		_Nop();
		_Nop();
		_Nop();
		_Nop();
		SDA=1;  		  //����I2C���߽����ź�
		_Nop();
		_Nop();
		_Nop();
		_Nop();
}




/**********************************
�ֽ����ݷ��ͺ���
����ԭ�Σ�void SendByte(unsigned char c)
�������ܣ�������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������,
		      �����ȴ�Ӧ��,���Դ�״̬λ���в���.
		      (��Ӧ����Ӧ��ʹack=0 ��)     
     	    ��������������ack=1;ack=0��ʾ��������Ӧ����𻵡�
��ڲ�����unsigned char c
���ڲ�������
**********************************/
void SendByte(unsigned char c)
{
		unsigned char BitCnt;
		for(BitCnt=0;BitCnt<8;BitCnt++)  	//Ҫ���͵����ݳ���Ϊ8λ
		{
				if((c<<BitCnt)&0x80)
				{
						SDA=1;   					            //�жϷ���λ
				}
				else
				{
						SDA=0;
				}  
												
				_Nop();
				SCL=1;               			     //��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ
				_Nop(); 
				_Nop();               			   //��֤ʱ�Ӹߵ�ƽ���ڴ���4us
				_Nop();
				_Nop();
				_Nop();         
				SCL=0; 
		}
			
		_Nop();
		_Nop();
		SDA=1;               				     //8λ��������ͷ������ߣ�׼������Ӧ��λ
		_Nop();
		_Nop();   
		SCL=1;
		_Nop();
		_Nop();
		_Nop();
		if(SDA==1)
		{
				ack=0;
		}
		else
		{
				ack=1;        					       //�ж��Ƿ���յ�Ӧ���ź�
		}     
				 
		SCL=0;
		_Nop();
		_Nop();
}




/**********************************
�ֽ����ݽ��պ���
����ԭ�Σ�unsigned char RcvByte(void)
�������ܣ��������մ���������������,���ж����ߴ���(����Ӧ���ź�)��
     	    ���������Ӧ������
��ڲ�������
���ڲ�����unsigned char�ͣ����ؽ��յ�������
**********************************/
unsigned char RcvByte(void)
{
		unsigned char retc;
		unsigned char BitCnt;
		
		retc=0; 
		SDA=1;             					//��������Ϊ���뷽ʽ
		for(BitCnt=0;BitCnt<8;BitCnt++)
		{
				_Nop();           
				SCL=0;       					    //��ʱ����Ϊ�ͣ�׼����������λ
				_Nop();
				_Nop();         				  //ʱ�ӵ͵�ƽ���ڴ���4.7us
				_Nop();
				_Nop();
				_Nop();
				SCL=1;       					    //��ʱ����Ϊ��ʹ��������������Ч
				_Nop();
				_Nop();
				retc=retc<<1;
				if(SDA==1)retc=retc+1; 		//������λ,���յ�����λ����retc��
				_Nop();
				_Nop(); 
		}
		SCL=0;    
		_Nop();
		_Nop();

		return(retc);
}




/**********************************
Ӧ���Ӻ���
����ԭ�Σ�void Ack_I2c(bit a)
�������ܣ�����������Ӧ���ź�,(������Ӧ����Ӧ���ź�)
��ڲ�����bit a
���ڲ�������
**********************************/
void Ack_I2c(bit a)
{
  
		if(a==0)
		{
				SDA=0;
		}     								//�ڴ˷���Ӧ����Ӧ���ź�
		else 
		{
				SDA=1;
		}
		_Nop();
		_Nop();
		_Nop();      
		SCL=1;
		_Nop();
		_Nop();              	//ʱ�ӵ͵�ƽ���ڴ���4us
		_Nop();
		_Nop();
		_Nop();  
		SCL=0;                //��ʱ���ߣ�ǯסI2C�����Ա��������
		_Nop();
		_Nop();    
}




/**********************************
�����ӵ�ַ���������ֽ����ݺ���
����ԭ�Σ�bit ISendByte(unsigned char sla, unsigned char c)
�������ܣ����������ߵ����͵�ַ�����ݣ��������ߵ�ȫ����,��������ַsla.
          �������1��ʾ�����ɹ��������������
��ڲ�����unsigned char sla, 
		      unsigned char c
���ڲ����������Ƿ�ɹ���־λ
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
**********************************/
bit ISendByte(unsigned char sla, unsigned char c)
{
		Start_I2c();               			//��������
		SendByte(sla);            			//����������ַ
		if(ack==0)
		{
				return(0);
		}
		SendByte(c);               			//��������
		if(ack==0)
		{	
				return(0);
		}
		Stop_I2c();                 	//�������� 

		return(1);
}




/**********************************
�����ӵ�ַ�������Ͷ��ֽ����ݺ���
����ԭ�Σ�ISendStr(uchar sla,uchar suba,uchar *s,uchar no)
�������ܣ����������ߵ����͵�ַ���ӵ�ַ,���ݣ��������ߵ�ȫ����,
		      ��������ַsla���ӵ�ַsuba������������sָ������ݣ�
		      ����no���ֽڡ��������1��ʾ�����ɹ��������������
��ڲ�����unsigned char sla, 
		      unsigned char suba, 
		      unsigned char *s, 
		      unsigned char no
���ڲ����������Ƿ�ɹ���־λ
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
**********************************/
bit ISendStr(unsigned char sla, unsigned char suba, unsigned char *s, unsigned char no)
{
		unsigned char i;

		Start_I2c();               			//��������
		SendByte(sla);            			//����������ַ
		if(ack==0)return(0);
		SendByte(suba);            			//���������ӵ�ַ
		if(ack==0)return(0);

		for(i=0;i<no;i++)
		{   
				SendByte(*s);               	//��������
				if(ack==0)return(0);
				s++;
		} 
		Stop_I2c();                 		//�������� 

		return(1);
}







/**********************************
�����ӵ�ַ������ȡ���ֽ����ݺ���
����ԭ�Σ�bit IRcvStr(unsigned char sla, unsigned char suba, unsigned char *s, unsigned char no)
�������ܣ����������ߵ����͵�ַ���ӵ�ַ�������ݣ��������ߵ�ȫ����,
		      ��������ַsla���ӵ�ַsuba�����������ݷ���sָ��Ĵ洢������no���ֽڡ�
          �������1��ʾ�����ɹ��������������
��ڲ�����unsigned char sla, 
		      unsigned char suba, 
		      unsigned char *s, 
		      unsigned char no
���ڲ����������Ƿ�ɹ���־λ
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
**********************************/
bit IRcvStr(unsigned char sla, unsigned char suba, unsigned char *s, unsigned char no)
{
		unsigned char i;

		Start_I2c();               //��������
		SendByte(sla);             //����������ַ
		if(ack==0)return(0);
		SendByte(suba);            //���������ӵ�ַ
		if(ack==0)return(0);

		Start_I2c();
		SendByte(sla+1);
		if(ack==0)return(0);

		for(i=0;i<no-1;i++)
		{   
				*s=RcvByte();             //��������
				Ack_I2c(0);               //����Ӧ��λ  
				s++;
		} 
		*s=RcvByte();
		Ack_I2c(1);                 //���ͷ�Ӧλ
		Stop_I2c();                 //�������� 
		return(1);
} 




/**********************************
�����ӵ�ַ���������ֽ����ݺ���
����ԭ�Σ�bit mRcvByte(unsigned char sla, unsigned char *s,unsigned char no)
�������ܣ����������ߵ����͵�ַ���ӵ�ַ�������ݣ��������ߵ�ȫ����,
		      ��������ַsla,���������ݷ���sָ��Ĵ洢������no���ֽڡ�
          �������1��ʾ�����ɹ��������������
��ڲ�����unsigned char sla, 
		      unsigned char *c
          
���ڲ����������Ƿ�ɹ���־λ
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
**********************************/
bit mRcvByte(unsigned char sla, unsigned char *s,unsigned char no)
{
		unsigned char i;

		Start_I2c();               //��������
		SendByte(sla+1);           //����������ַ
		if(ack==0)return(0);
		for(i=0;i<no-1;i++)
		{   
				*s=RcvByte();            //��������
				Ack_I2c(0);              //����Ӧ��λ  
				s++;
		} 
		*s=RcvByte();
		Ack_I2c(1);                //���ͷ�Ӧλ
		Stop_I2c();                //�������� 
		return(1);
} 




/**********************************
�����ӵ�ַ�������Ͷ��ֽ����ݺ���
����ԭ�Σ�bit SendStr(unsigned char sla, unsigned char *s, unsigned char no)
�������ܣ����������ߵ����͵�ַ���ӵ�ַ,���ݣ��������ߵ�ȫ����,
		      ��������ַsla���ӵ�ַsuba������������sָ������ݣ�
		      ����no���ֽڡ��������1��ʾ�����ɹ��������������
��ڲ�����unsigned char sla, 
		      unsigned char suba, 
		      unsigned char *s, 
		      unsigned char no
���ڲ����������Ƿ�ɹ���־λ
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
**********************************/
bit mSendStr(unsigned char sla, unsigned char *s, unsigned char no)
{
		unsigned char i;

		Start_I2c();               			//��������
		SendByte(sla);            			//����������ַ
		if(ack==0)return(0);

		for(i=0;i<no;i++)
		{   
				SendByte(*s);               	//��������
				if(ack==0)return(0);
				s++;
		} 
		Stop_I2c();                 		//�������� 
		return(1);
}

