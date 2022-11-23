#include <reg52.h>
#include <iic.h>

sbit IIC_SDA=P3^5;        //声明IIC总线的数据线接在单片机的P2.6端口。
sbit IIC_SCL=P3^6;        //声明IIC总线的时钟线接在单片机的P2.5端口。


//void delay(unsigned int t)
//{//延时函数
//    while(t){t--;};                    //延时循环计数
//}
//-------------------------------------------------------------------
void delay_IIC(void)    
{//IIC总线限速延时函数。
//该函数是空函数，延时4个机器周期。
	;
	;
}
//-------------------------------------------------------------------
void IIC_Init(void)
{//IIC总线初始化函数
    IIC_SDA=1;//释放IIC总线的数据线。
    IIC_SCL=1;//释放IIC总线的时钟线。
}
//-------------------------------------------------------------------
void IIC_start(void)
{//IIC总线产生起始信号函数  
   IIC_SDA=1;//拉高数据线
	 IIC_SCL=1;//拉高时钟线
	 delay_IIC();
	 IIC_SDA=0;//在时钟线为高电平时，拉低数据线，产生起始信号。
	 delay_IIC();
   IIC_SCL=0;//拉低时钟线
}
//-------------------------------------------------------------------
void IIC_stop(void) 
{//IIC总线产生停止信号函数
    IIC_SDA=0;//拉低数据线
    delay_IIC();
    IIC_SCL=1;//拉高时钟线。
    delay_IIC();
    IIC_SDA=1;//时钟时线为高电平时，拉高数据线，产生停止信号。
    delay_IIC();
}
//-------------------------------------------------------------------
bit IIC_Tack(void)
{//接收应答信号函数
    bit ack;//定义一个位变量，来暂存应答状态。
    IIC_SDA=1;//释放数据总线，准备接收应答信号。
    delay_IIC();
    IIC_SCL=1;//拉高时钟线。
    delay_IIC();
    ack=IIC_SDA;//读取应答信号的状态。
    delay_IIC();
    IIC_SCL=0;//拉低时钟线。
    delay_IIC();
    return ack;//返回应答信号的状态，0表示应答，1表示非应答。
}
//-------------------------------------------------------------------
void IIC_write_byte(unsigned char Data)
{//向IIC总线写入一个字节的数据函数 
	unsigned char i;
	 for(i=0;i<8;i++)//有8位数据
	{
			IIC_SDA=Data&0x80;//写最高位的数据
			delay_IIC();
			IIC_SCL=1; //拉高时钟线，将数写入到设备中。
			delay_IIC();
			IIC_SCL=0;//拉低时钟线，允许改变数据线的状态
			delay_IIC();
			Data=Data<<1;//数据左移一位，把次高位放在最高位,为写入次高位做准备
	}
}
//-------------------------------------------------------------------
unsigned char IIC_read_byte()
{//从IIC总线读取一个字节的数据函数
    unsigned char i;
    unsigned char Data;       //定义一个缓冲寄存器。
    for(i=0;i<8;i++)//有8位数据
    {
        IIC_SCL=1;//拉高时钟线，为读取下一位数据做准备。
        delay_IIC();
        Data=Data<<1;//将缓冲字节的数据左移一位，准备读取数据。
        delay_IIC();
        
        if(IIC_SDA)//如果数据线为高平电平。
            Data=Data|0x1;//则给缓冲字节的最低位写1。
        IIC_SCL=0;//拉低时钟线，为读取下一位数据做准备。
        delay_IIC();
    }
    return Data;//返回读取的一个字节数据。
}
void IIC_single_byte_write(unsigned char Daddr,unsigned char Waddr,unsigned char Data)
{//向任意地址写入一个字节数据函数
    IIC_start();//产生起始信号
    IIC_write_byte(Daddr);//写入设备地址（写）
    IIC_Tack();//等待设备的应答
    IIC_write_byte(Waddr);//写入要操作的单元地址。
    IIC_Tack();//等待设备的应答。
    IIC_write_byte(Data);//写入数据。
    IIC_Tack();//等待设备的应答。
    IIC_stop();//产生停止符号。
}
//-------------------------------------------------------------------
unsigned char IIC_single_byte_read(unsigned char Daddr,unsigned char Waddr)
{//从任意地址读取一个字节数据函数
    unsigned char Data;//定义一个缓冲寄存器。

    IIC_start();//产生起始信号
    IIC_write_byte(Daddr);//写入设备地址（写）
    IIC_Tack();//等待设备的应答
    IIC_write_byte(Waddr);//写入要操作的单元地址。
    IIC_Tack();//等待设备的应答。
    
    IIC_start();//产生起始信号
    IIC_write_byte(Daddr+1);//写入设备地址（读）。
    IIC_Tack();//等待设备的应答。 
    Data=IIC_read_byte();//写入数据。
    IIC_stop();//产生停止符号。
    //-------------------返回读取的数据--------------------
    return Data;//返回读取的一个字节数据。
}
