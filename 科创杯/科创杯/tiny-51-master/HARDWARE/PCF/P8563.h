/********************************
P8563.H

PCF8563������ͷ�ļ�
********************************/

#ifndef __P8563_H__
#define __P8563_H__

//PCF8563���ԼĴ�����ַ����
#define CTRL_STA1_REG		0
#define CTRL_STA2_REG		1
#define SECOND_REG			2
#define MINUTE_REG			3
#define HOUR_REG			4
#define DAY_REG				5
#define WEEK_REG			6
#define MONTH_REG			7
#define YEAR_REG			8
#define MINUTE_ALARM_REG	9
#define HOUR_ALARM_REG		10
#define DAY_ALARM_REG		11
#define WEEK_ALARM_REG		12
#define CLKOUT_CTRL_REG		13
#define TIMER_CTRL_REG		14
#define TIMER_VALUE_REG		15


/*************************�����б�*************************/
//��P8563��ָ���Ĵ���д��ֵ
bit P8563_SendStr(unsigned char sub_addr, unsigned char *send_buf, unsigned char data_no);
//��P8563��ָ���Ĵ�������ֵ
bit P8563_RcvStr(unsigned char sub_addr, unsigned char *rec_buf, unsigned char data_no);

#endif