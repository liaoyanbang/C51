/********************************
ADS1015.H

ADS1015������ͷ�ļ�
********************************/

#ifndef __ADS1015_H__
#define __ADS1015_H__

//ADS1015���мĴ�����ַ����
#define CONFIG_REG				1
#define CONVERSION_REG		0

#define ADS1015_SLA_ADDR  0x90

/*************************�����б�*************************/

//ADS1015��ʼ��
bit Start_conversion(unsigned char sla_addr, unsigned char sub_addr, unsigned char ch);
//��ADS1015��ȡת�����
bit Read_conversion(unsigned char sla_addr, unsigned char sub_addr, unsigned char *rec_buf);

#endif