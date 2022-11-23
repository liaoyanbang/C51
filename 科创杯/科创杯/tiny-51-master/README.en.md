@[toc]
# 基于Tiny-51操作系统的51单片机温度测控系统设计

## 一、设计题目

​		温度测控仪（扩展板AD仿真）

## 二、设计要求

1. 使用电位器输出电压进行采样仿真温度测量，设定温度范围从-20-200摄氏度，采样分辨率为0.01摄氏度；
2. 采样进行定时采样，采样时间自定（推荐1秒）；
3. 采样得到的数字量通过曲线拟合出温度值，并将温度数值显示在OLED显示屏，同时OLED也要显示当前时间；
4. 分三个不同时间段进行报警，可以切换摄氏度与华氏度单位；
5. 通过矩阵键盘和OLED显示屏设计人机交互界面，设置温度的上下限值、设置时间段、设置当前时间；16个矩阵键盘的按键功能分配如下，0-9、设置、确认、上移、下移（兼具单位切换）、左移、右移；下载程序第一次上电后，设定值有默认值，后续如果修改设定值，设定值存入AT24C02，掉电不丢失；
6. 制定通信协议，可以在PC机通过串口助手获取当前的温度值、当前时间、当前运行状态（正常、超上限、超下限）；
7. 制定通讯协议，可以发送指令，通过上位机设定温度的上下限、时间段和当前时间

## 三、设计作用及目的

- 设计作用

  ​		本次设计的主要作用是提高同学们对于51单片机的掌握程度，掌握使用51单片机进行项目开发的流程，通过团队合作制作一个具有一定项目意义的项目，让每一位成员参与到项目的设计之中，了解设计的流程，将所学的知识应用到项目之中，实现所有功能。

- 设计目的

  ​		基于STC89C52RC单片机，编写程序，调度外围模块，达到可以实时ad采集电压转化为温度，同时显示时间，以及温度上下限，并且可以进入设置界面，设置温度单位，3个温度时间段，以及在其时间段范围内，温度上下限的设定。

## 四、硬件设计

- 功能模块选择：

1. 温度模拟采集：温度模拟使用A-D模块，将电位器的输入电压（模拟量Analog）转换为输出量（数字量Digital），在使用单片机读取数字量电压信号。此处我们使用的是ADS1015采集模块，该模块通过IIC配置模块内部寄存器，读写地址位分别为：0x91和0x90，通过对不同寄存器写入对应值，可以控制开始转换，读取采集数据的功能。IIC总线协议不清楚的可以看我的另外一篇文章：链接；
2. 时间模块：P8563时间模块该模块也是通过IIC总线进行读写操作，写入对应时间寄存器设定时间后，自动进行时间计数，搭配外部电源与晶振，可以在断电时继续运行，维持时间计数，可以通过IIC读取寄存器，读取当前时间。
3. 显示模块：采用OLED模块做显示，本模块通过IIC配置对应寄存器，IIC写入GRAM，OLED显示对应的设置，OLED显示清晰，成本也较为低廉，故选用此模块。
4. 存储模块：AT24C02，该模块通过IIC写入数据，将需要存储的数据写入24C02（EEPROM）内部对应的区域，每次写入一个字节，读取时也是通过IIC总线读取，每次读取一个字节。

- 硬件电路连接：

1. IO口分配：电路硬件连接的设计结合各个模块的IO口使用情况来设计，时间、存储、模拟采集、数据显示皆使用IIC，且地址不重复，故可以使用同一组IIC总线，根据各个模块的地址进行不同模块的读写操作。此处由于使用模块皆为5v或5v以下，使用电脑供电足够，电源可直接进行连接，无需适配其他的稳压方式。

2. 硬件连接图：

   2.1 整体硬件思路：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021010312590931.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1Mzk2Njcy,size_16,color_FFFFFF,t_70)
   2.2 实际连接方案：

   ADS1015电路设计图，图中SCL：P0.4；SDA：P0.5；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210103125918415.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1Mzk2Njcy,size_16,color_FFFFFF,t_70)

   PCF8563时钟模块以及AT24C02EEPROM模块，SCL：P0.4；SDA：P0.5；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210103125937900.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1Mzk2Njcy,size_16,color_FFFFFF,t_70)
​	OLED显示模块：SCL：P0.4；SDA：P0.5；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210103125943136.png)
​	蜂鸣器及LED连接电路：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021010312594912.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1Mzk2Njcy,size_16,color_FFFFFF,t_70)
​	51单片机最小系统电路：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210103125958986.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1Mzk2Njcy,size_16,color_FFFFFF,t_70)
​	矩阵键盘硬件原理图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210103130005530.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1Mzk2Njcy,size_16,color_FFFFFF,t_70)
## 五、软件流程
- 程序流程图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210103130025785.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1Mzk2Njcy,size_16,color_FFFFFF,t_70)
- 通信协议定义：
  接收帧定义
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210103130032321.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1Mzk2Njcy,size_16,color_FFFFFF,t_70)
  发送帧定义

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210103130045336.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1Mzk2Njcy,size_16,color_FFFFFF,t_70)
- 矩阵按键功能表：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210103130050222.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1Mzk2Njcy,size_16,color_FFFFFF,t_70)
- IIC总线协议：
  见文档

## 六、调试界面
数据界面：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210103235010884.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1Mzk2Njcy,size_16,color_FFFFFF,t_70)
设置界面：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210103235002473.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ1Mzk2Njcy,size_16,color_FFFFFF,t_70)
## 七、心得体会
过去的51单片机开发都是以裸机直接进行开发，程序编写复杂，逻辑难以布置，在程序任务量大了之后一旦出现bug，难以进行修改，因为使用过操作系统进行开发，我也比较倾向于使用操作系统进行开发，分析了本次题目之后，因为程序的功能较为复杂，所以使用操作系统有一定的必要性，使用的实时系统是keil公司推出的RTX51 tiny版本，采取分时调度，任务主程序占据RAM9字节，每创建一个任务占据3个字节，每个信号量占据3字节，ROM最大900个字节，这些使用资源对于拥有256RAM和8kROM的stc89c52rc来说开销也很小，在使用了RTX之后，任务的开发难度下降了很多，每种类型的任务单独封装成一个线程，线程间通过信号和全局量进行同步与通信，在出现BUG时，可以很快根据现象找到对应错误的线程位置，提高开发的速度，同时通过这次课设我也进一步加强了我对于51单片机内部结构的熟悉，加强了我对51外设的了解与使用程度，进一步加深了我对51配置外围模块的理解，以及串口通信，IIC总线的使用程度。
## 八、程序源码

Gitee：链接

Github：链接

