#include <reg52.h>
#include <iic.h>

sbit IIC_SDA=P3^5;        //����IIC���ߵ������߽��ڵ�Ƭ����P2.6�˿ڡ�
sbit IIC_SCL=P3^6;        //����IIC���ߵ�ʱ���߽��ڵ�Ƭ����P2.5�˿ڡ�


//void delay(unsigned int t)
//{//��ʱ����
//    while(t){t--;};                    //��ʱѭ������
//}
//-------------------------------------------------------------------
void delay_IIC(void)    
{//IIC����������ʱ������
//�ú����ǿպ�������ʱ4���������ڡ�
	;
	;
}
//-------------------------------------------------------------------
void IIC_Init(void)
{//IIC���߳�ʼ������
    IIC_SDA=1;//�ͷ�IIC���ߵ������ߡ�
    IIC_SCL=1;//�ͷ�IIC���ߵ�ʱ���ߡ�
}
//-------------------------------------------------------------------
void IIC_start(void)
{//IIC���߲�����ʼ�źź���  
   IIC_SDA=1;//����������
	 IIC_SCL=1;//����ʱ����
	 delay_IIC();
	 IIC_SDA=0;//��ʱ����Ϊ�ߵ�ƽʱ�����������ߣ�������ʼ�źš�
	 delay_IIC();
   IIC_SCL=0;//����ʱ����
}
//-------------------------------------------------------------------
void IIC_stop(void) 
{//IIC���߲���ֹͣ�źź���
    IIC_SDA=0;//����������
    delay_IIC();
    IIC_SCL=1;//����ʱ���ߡ�
    delay_IIC();
    IIC_SDA=1;//ʱ��ʱ��Ϊ�ߵ�ƽʱ�����������ߣ�����ֹͣ�źš�
    delay_IIC();
}
//-------------------------------------------------------------------
bit IIC_Tack(void)
{//����Ӧ���źź���
    bit ack;//����һ��λ���������ݴ�Ӧ��״̬��
    IIC_SDA=1;//�ͷ��������ߣ�׼������Ӧ���źš�
    delay_IIC();
    IIC_SCL=1;//����ʱ���ߡ�
    delay_IIC();
    ack=IIC_SDA;//��ȡӦ���źŵ�״̬��
    delay_IIC();
    IIC_SCL=0;//����ʱ���ߡ�
    delay_IIC();
    return ack;//����Ӧ���źŵ�״̬��0��ʾӦ��1��ʾ��Ӧ��
}
//-------------------------------------------------------------------
void IIC_write_byte(unsigned char Data)
{//��IIC����д��һ���ֽڵ����ݺ��� 
	unsigned char i;
	 for(i=0;i<8;i++)//��8λ����
	{
			IIC_SDA=Data&0x80;//д���λ������
			delay_IIC();
			IIC_SCL=1; //����ʱ���ߣ�����д�뵽�豸�С�
			delay_IIC();
			IIC_SCL=0;//����ʱ���ߣ�����ı������ߵ�״̬
			delay_IIC();
			Data=Data<<1;//��������һλ���Ѵθ�λ�������λ,Ϊд��θ�λ��׼��
	}
}
//-------------------------------------------------------------------
unsigned char IIC_read_byte()
{//��IIC���߶�ȡһ���ֽڵ����ݺ���
    unsigned char i;
    unsigned char Data;       //����һ������Ĵ�����
    for(i=0;i<8;i++)//��8λ����
    {
        IIC_SCL=1;//����ʱ���ߣ�Ϊ��ȡ��һλ������׼����
        delay_IIC();
        Data=Data<<1;//�������ֽڵ���������һλ��׼����ȡ���ݡ�
        delay_IIC();
        
        if(IIC_SDA)//���������Ϊ��ƽ��ƽ��
            Data=Data|0x1;//��������ֽڵ����λд1��
        IIC_SCL=0;//����ʱ���ߣ�Ϊ��ȡ��һλ������׼����
        delay_IIC();
    }
    return Data;//���ض�ȡ��һ���ֽ����ݡ�
}
void IIC_single_byte_write(unsigned char Daddr,unsigned char Waddr,unsigned char Data)
{//�������ַд��һ���ֽ����ݺ���
    IIC_start();//������ʼ�ź�
    IIC_write_byte(Daddr);//д���豸��ַ��д��
    IIC_Tack();//�ȴ��豸��Ӧ��
    IIC_write_byte(Waddr);//д��Ҫ�����ĵ�Ԫ��ַ��
    IIC_Tack();//�ȴ��豸��Ӧ��
    IIC_write_byte(Data);//д�����ݡ�
    IIC_Tack();//�ȴ��豸��Ӧ��
    IIC_stop();//����ֹͣ���š�
}
//-------------------------------------------------------------------
unsigned char IIC_single_byte_read(unsigned char Daddr,unsigned char Waddr)
{//�������ַ��ȡһ���ֽ����ݺ���
    unsigned char Data;//����һ������Ĵ�����

    IIC_start();//������ʼ�ź�
    IIC_write_byte(Daddr);//д���豸��ַ��д��
    IIC_Tack();//�ȴ��豸��Ӧ��
    IIC_write_byte(Waddr);//д��Ҫ�����ĵ�Ԫ��ַ��
    IIC_Tack();//�ȴ��豸��Ӧ��
    
    IIC_start();//������ʼ�ź�
    IIC_write_byte(Daddr+1);//д���豸��ַ��������
    IIC_Tack();//�ȴ��豸��Ӧ�� 
    Data=IIC_read_byte();//д�����ݡ�
    IIC_stop();//����ֹͣ���š�
    //-------------------���ض�ȡ������--------------------
    return Data;//���ض�ȡ��һ���ֽ����ݡ�
}
