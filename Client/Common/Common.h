//	Common.h
//	本文件内容改动后,必须新编译所有文件(4个DLL,SafeMem,Demo_DX)
//	修改时间: 2004.3.12
/////////////////////////////////////////////////////////////

//AppDll,MainDll释放到系统DLL目录下的名称定义
#define HOOKAPI_DLL_NAME	"mcirlecd.dll" //MainDll
#define MY_DLL_NAME			"mcirleda.dll" //AppDll

//共享内存的标志,一般不需改动
#define SM_SAFEHWND "EB3CD6A8C" //传递SafeMem窗口句柄的共享内存标志
#define SM_GAMEHWND "ADD4A86EB" //传递Game窗口句柄的共享内存标志
#define SM_SAFEMEMPATH "F53D4CDEA" ////传递SafeMem路径的共享内存标志

//
#define DELAYTIME 25

