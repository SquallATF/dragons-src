
#ifndef GAME_PROTECT_H
#define GAME_PROTECT_H
#define GAME_API __declspec(dllexport)

extern "C"
{

/*
函数说明:
	保护函数,程序启动后输入进程窗口句柄和进程pid.
参数说明:
	hWnd,要保护的进程窗口句柄.
	dwPid,要保护的进程的pid值,保护当前进程可用GetCurrentProcessId()得到.
返回值说明:
	0:加载保护成功.
	1:查找资源失败.
	2:加载资源失败.
	3:锁定资源失败.
	4:创建资源文件失败.
	5:输出资源文件失败.
	6:保护已经加载.
	7:打开设备管理器失败.
	8:创建或打开设备失败.
	9:启动服务失败.
	10:服务通信设备创建失败.
	11:服务通信失败.
	12:服务保护加载失败.
	13:错误服务请求.
	14:错误服务请求返回.
	15:保护线程创建失败.
*/
int GAME_API Protect(HWND hWnd, DWORD dwPid);

/*
函数说明:
	卸载保护,在程序结束时调用该函数即可.无参数,无返回值.
*/
void GAME_API UnProtect(void);

}

#endif