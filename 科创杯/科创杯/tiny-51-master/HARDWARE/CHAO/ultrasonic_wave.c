#include "ultrasonic_wave.h"//超声波头函数


//超声波初始化
void Init_ultrasonic_wave()
{
	 TX1 = 0;			   //关闭发射
	 TX2 = 0;					
	 TX3 = 0;
	 TX4 = 0;
	 TX5 = 0;
	 TX6 = 0;
	 TX7 = 0;
	//关闭发射
}

//启动超声波
void StartModule1() 	//启动模块
{
	TX1=1;			      //启动一次模块
	os_wait2(K_TMO,1);
	TX1=0;
}
//启动超声波
void StartModule2() 	//启动模块
{
	TX2=1;			      //启动一次模块
	os_wait2(K_TMO,1);
	TX2=0;
}
//启动超声波
void StartModule3() 	//启动模块
{
	TX3=1;			      //启动一次模块
	os_wait2(K_TMO,1);
	TX3=0;
}
//启动超声波
void StartModule4() 	//启动模块
{
	TX4=1;			      //启动一次模块
	os_wait2(K_TMO,1);
	TX4=0;
}


//启动超声波
void StartModule5() 	//启动模块
{
	TX5=1;			      //启动一次模块
	os_wait2(K_TMO,1);
	TX5=0;
}
//启动超声波
void StartModule6() 	//启动模块
{
	TX6=1;			      //启动一次模块
	os_wait2(K_TMO,1);
	TX6=0;
}


//启动超声波
void StartModule7() 	//启动模块
{
	TX7=1;			      //启动一次模块
	os_wait2(K_TMO,1);
	TX7=0;
}


