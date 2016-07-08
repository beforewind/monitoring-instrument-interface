#ifndef _USB2089_DEVICE_
#define _USB2089_DEVICE_

#ifdef __cplusplus
extern "C" {
#endif

#include<windows.h>
//***********************************************************
// 用于AD采集的参数结构
typedef struct _USB2089_PARA_AD
{
	LONG ADMode;			// AD采集方式(连续和分组)
	LONG ReadSizeWords;     // 读取长度(点)
	LONG FirstChannel;      // 首通道
	LONG LastChannel;		// 末通道
	LONG Frequency;		    // AD采集频率(Hz)
	LONG GroupInterval;	    // 分组采样时，相邻组的时间间隔(uS) 
	LONG Gains;             // 采集增益
	LONG TriggerMode;	    // 内外触发方式选择
	LONG TriggerType;		// 电平触发/边沿触发
	LONG TriggerDir;	    // 正向/负向触发选择
	LONG ClockSource;       // 时钟源选择
	LONG bClockOutput;      // 允许时钟输出
} USB2089_PARA_AD, *PUSB2089_PARA_AD;

//***********************************************************
// AD参数(USB2089_PARA_AD)中的ADMode使用的采集方式选项
const long USB2089_ADMODE_SEQUENCE		= 0x00;	// 连续采集
const long USB2089_ADMODE_GROUP			= 0x01;	// 分组采集

//***********************************************************
// AD参数USB2089_PARA_AD中的Gains使用的硬件增益选项
const long USB2089_GAINS_1MULT			= 0x00; // 1倍增益(使用PGA202或PGA203放大器)
const long USB2089_GAINS_10MULT			= 0x01; // 10倍增益(使用PGA202放大器)
const long USB2089_GAINS_100MULT		= 0x02; // 100倍增益(使用PGA202放大器)
const long USB2089_GAINS_1000MULT		= 0x03; // 1000倍增益(使用PGA202放大器)
const long USB2089_GAINS_2MULT			= 0x01; // 2倍增益(使用PGA203放大器)
const long USB2089_GAINS_4MULT			= 0x02; // 4倍增益(使用PGA203放大器)
const long USB2089_GAINS_8MULT			= 0x03; // 8倍增益(使用PGA203放大器)

//***********************************************************
// AD硬件参数USB2089_PARA_AD中的TriggerMode成员变量所使用AD触发模式选项
const long USB2089_TRIGMODE_SOFT		= 0x00;  // 软件内触发方式
const long USB2089_TRIGMODE_POST		= 0x01;  // 硬件后触发方式

// AD硬件参数USB2089_PARA_AD中的TriggerType成员变量所使用的触发类型选项
const long USB2089_TRIGTYPE_EDGE		= 0x00; // 边沿触发
const long USB2089_TRIGTYPE_PULSE		= 0x01; // 脉冲触发(电平)

//***********************************************************
// AD硬件参数USB2089_PARA_AD中的TriggerDir成员变量所使用的触发方向选项
const long USB2089_TRIGDIR_NEGATIVE		= 0x00; // 负向触发(低电平/下降沿触发)
const long USB2089_TRIGDIR_POSITIVE		= 0x01; // 正向触发(高电平/上升沿触发)
const long USB2089_TRIGDIR_POSIT_NEGAT	= 0x02; // 正负向触发(高/低电平或上升/下降沿触发)

//***********************************************************
// 驱动函数接口
#ifndef _USB2089_DRIVER_
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	//######################## 常规通用函数 #################################
	HANDLE DEVAPI FAR PASCAL USB2089_CreateDevice(int DeviceLgcID = 0); // 创建设备对象(该函数使用系统内逻辑设备ID）
	int DEVAPI FAR PASCAL USB2089_GetDeviceCount(HANDLE hDevice);      // 取得USB2089在系统中的设备数量
	int DEVAPI FAR PASCAL USB2089_GetDeviceCurrentID(HANDLE hDevice);
	BOOL DEVAPI FAR PASCAL USB2089_ResetDevice(HANDLE hDevice);		 // 复位整个USB设备
	BOOL DEVAPI FAR PASCAL USB2089_ReleaseDevice(HANDLE hDevice);    // 释放设备

	//####################### AD数据读取函数 #################################
    BOOL DEVAPI FAR PASCAL USB2089_InitDeviceAD( // 初始化设备,当返回TRUE后,设备即刻开始传输.
											HANDLE hDevice,     // 设备句柄
											PUSB2089_PARA_AD pADPara);  // 硬件参数, 它仅在此函数中决定硬件状态
										
	BOOL DEVAPI FAR PASCAL USB2089_GetDeviceStatusAD( HANDLE hDevice,  // 设备句柄
											PBOOL bTrigFlag,    // 触发标志
											PBOOL bConverting,  // 转换标志
											PBOOL bOverflow);   // 溢出标志

    BOOL DEVAPI FAR PASCAL USB2089_ReadDeviceAD(  // 初始化设备后，即可用此函数读取设备上的AD数据
											HANDLE hDevice,     // 设备句柄
											SHORT ADBuffer[],   // 将用于接受数据的用户缓冲区
											LONG nReadSizeWords,	// 读取AD数据的长度
											PLONG nRetSizeWords);    // 返回实际读取的AD数据

	BOOL DEVAPI FAR PASCAL USB2089_ReleaseDeviceAD( HANDLE hDevice); // 停止AD采集，释放AD对象所占资源

	//################# AD的硬件参数操作函数 ########################	
	BOOL DEVAPI FAR PASCAL USB2089_SaveParaAD(HANDLE hDevice, PUSB2089_PARA_AD pADPara);  
	BOOL DEVAPI FAR PASCAL USB2089_LoadParaAD(HANDLE hDevice, PUSB2089_PARA_AD pADPara);
	BOOL DEVAPI FAR PASCAL USB2089_ResetParaAD(HANDLE hDevice, PUSB2089_PARA_AD pADPara); // 将AD采样参数恢复至出厂默认值

	//####################### DA数据输出函数 #################################
	// 适于大多数普通用户，这些接口最简单、最快捷、最可靠，让用户不必知道设备
	// 低层复杂的硬件控制协议和繁多的软件控制编程，仅用下面一个函数便能轻
	// 松实现高速、连续的DA数据输出
	BOOL DEVAPI FAR PASCAL USB2089_WriteDeviceDA(
											HANDLE hDevice,	// 设备对象句柄
											WORD nDALsb,		// 准备输出的DA数据LSB原码[0, 4095]
											int nDAChannel);	// DA通道(0-3)

	//####################### 数字I/O输入输出函数 #################################
	BOOL DEVAPI FAR PASCAL USB2089_GetDeviceDI(					// 取得开关量状态     
											HANDLE hDevice,				// 设备句柄,它应由CreateDevice函数创建								        
											BYTE bDISts[8]);			// 开关输入状态(注意: 必须定义为8个字节元素的数组)

    BOOL DEVAPI FAR PASCAL USB2089_SetDeviceDO(					// 输出开关量状态
											HANDLE hDevice,				// 设备句柄,它应由CreateDevice函数创建								        
											BYTE bDOSts[8]);			// 开关输出状态(注意: 必须定义为8个字节元素的数组)

	//############################ 线程操作函数 ################################
	HANDLE DEVAPI FAR PASCAL USB2089_CreateSystemEvent(void); 	// 创建内核系统事件对象
	BOOL DEVAPI FAR PASCAL USB2089_ReleaseSystemEvent(HANDLE hEvent); // 释放内核事件对象

#ifdef __cplusplus
}
#endif

// 自动包含驱动函数导入库
/*
#ifndef _USB2089_DRIVER_
#ifndef _WIN64
#pragma comment(lib, "USB2089_32.lib")
#pragma message("======== Welcome to use our art company's products!")
#pragma message("======== Automatically linking with USB2089_32.dll...")
#pragma message("======== Successfully linked with USB2089_32.dll")
#else
#pragma comment(lib, "USB2089_64.lib")
#pragma message("======== Welcome to use our art company's products!")
#pragma message("======== Automatically linking with USB2089_64.dll...")
#pragma message("======== Successfully linked with USB2089_64.dll")
#endif
#endif
*/

#ifdef __cplusplus
}
#endif

#endif // _USB2089_DEVICE_
