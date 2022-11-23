#include <reg51.h>
sbit p1_0 = P1^0;
unsigned char count = 1;//用来改变LED状态

//初始化串口 （设置串口，开启串口中断）
void init_uart(void)
{
	SCON = 0x50;		        // SCON: 方式 1, 8-bit, 允许接收数据 
	TMOD |= 0x20;               // TMOD: 设置定时器1工作在方式2, 8-bit 自动重装
	TH1 = 0xFD;               // TH1:  初始值为0xFD  波特率：9600 晶振频率：11.0592MHz  
	TL1 = 0x0;
	TR1 = 1;                  // TR1:  开启定时器1                         
	EA  = 1;                  //打开总中断
	ES  = 1;                  //打开串口中断
}       

           
// 发送一个字节数据
void uart_send_byte(unsigned char dat)
{
	if(TI == 0)//上一次发送完毕
	{
		SBUF = dat; // 将数据送到发送缓冲寄存器SBUF，一位一位的发送
		while(!TI); // 等待发送完毕 (发送完毕TI硬件置1)
	}
}

// 发送字符串
void uart_send_str(unsigned char *s)
{
	while(*s != '\0')// '\0':字符串结束标志
	{
		uart_send_byte(*s);// 发送1个字节数据，1个字符占8位，1字节
		s++;// 指向下一个字符
	}
}


// 串口中断处理函数 （串口接收到数据，发送数据完毕都可以引起串口中断）
void uart_interrupt(void) interrupt 4 		//也叫串行中断服务程序
{
	unsigned char recv_data;// 用来存放接收到的数据
	//unsigned char send_data[] = "ok";// 要发送的信息
	
	if(RI) //接收数据(1字节)完毕，RI会被硬件置1
	{
		RI = 0;           		// 将 接收中断标志位 清零(让串口可以继续接收数据)
		recv_data = SBUF;           	//读取接收到的数据，并存放到data
		
		count++;	 
		uart_send_byte(recv_data);//收到什么就回复什么
		p1_0 = count%2;//每收到1个字符就让LED改变状态

	}
	if(TI)// 发送数据(1字节)完毕
	{
		TI = 0;// 将 发送中断标志位 清零(让串口可以继续发送数据)
	}
} 

void main(void)
{
	init_uart();// 初始化串口
	
	while(1)// 主循环不做任何动作。（死循环）
	{}
}

