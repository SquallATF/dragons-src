// -------------------------------
// ReWrite by chan78 at 2001/01/11
// -------------------------------
#include "stdafx.h"
#include "AgentServer.h"
#include "monitor.h"
#include "mylog.h"
#include "servertable.h"

//#include "ServerMonitor.cpp"
//#include "windows.h"

#include "CUseMoney.h"	// 030205 kyo
#include <process.h>

void Paythread( void *pVoid );	// 030205 kyo
extern void SendConnectServerCount( DWORD dwConnectionIndex );
extern void SendCurrServerPort( DWORD dwConnectionIndex );
extern void SendPbs( char *temp, int len );						//引入发送公告函数

//-----------------------------自动公告定义
int MsgCount;
bool isMsg;//发送判断标识
char MsgBufs[255][255];//欢迎公告
char MsgBuf[255][255];//事务自动公告
//-------------------------------



extern BOOL DumpException(LPEXCEPTION_POINTERS lpExcep,char* szOutMsg, void *pData = NULL, int nSize = 0); // 030514 kyo
void ProcessEnd(const int iDestroyType) // 030514 kyo
{
	g_pServerTable->DestroyServer( iDestroyType );
	MyLog( LOG_NORMAL, "-- Agent Demon ShutDown :: Now Release INetwork Module, ServerTable" );
	// acer7
	//exit( 0 );
}

HANDLE hIn;

HWND m_hProcWnd; // 030422 kyo
DWORD WINAPI PubMsg(LPVOID lpParameter);//事务自动发送公告线程
DWORD WINAPI PubMsgs(LPVOID lpParameter);//欢迎自动发送公告线程

void MainLoop()
{
	INPUT_RECORD				irBuffer;
	DWORD						dwResult;

	memset(&irBuffer, 0, sizeof(INPUT_RECORD));

	if (!StartAgentServer())
	{
		goto FinishAgentServer;
	}


	// Make connection with PROXY
	g_pINet->ResumeTimer(0);
	g_pINet->ResumeTimer(2);

	__try// 030514 kyo
	{
		if (LocalMgr.IsAbleNation(THAI) && !LocalMgr.IsFreeBeta())
		{	//030129 lsw 咯扁啊 Gump 辑滚啊 嘿阑 何盒.
			/////////////////////////////////////////
			if (0 < g_cAsyncPay.InitConnectPayServer())
			{
				_beginthread(Paythread, 0, NULL);

				MSG msg;
				//while( PeekMessage( &msg, NULL, 0,0, PM_REMOVE) )//)GetMessage(&msg,NULL,0,0) )
				//while( PeekMessage( &msg, NULL, 0,0, PM_NOREMOVE) )//)GetMessage(&msg,NULL,0,0) )
				while (GetMessage(&msg, NULL, 0, 0))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				MyLog(0, "GUMP Connection Fail");
				goto FinishAgentServer;
			}

		}

		else
		{
			while (g_pServerTable->IsServerRunning())
			{
				ReadConsoleInput(hIn, &irBuffer, 1, &dwResult);

				if (irBuffer.EventType == KEY_EVENT)
				{

					if (irBuffer.Event.KeyEvent.bKeyDown)
					{
						switch (irBuffer.Event.KeyEvent.wVirtualKeyCode)
						{
						case VK_ESCAPE:
						{
							if (MessageBox(NULL, "Are you sure to DESTROY this agent server?", "IMPORTANT", MB_YESNO) == IDYES)
							{
								g_pServerTable->DestroyServer(FINISH_TYPE_NORMAL);
							}
						}
						break;
						case VK_F1:
						{
							SetEvent(hKeyEvent[0]);
						}
						break;


						//---------------------------------------//事务自动公告
						case VK_F11:
						{

							// ---------------------------------------------//创建记录文件夹

							DWORD   dwAttr = GetFileAttributes(".\\事务公告");
							if (dwAttr == 0xFFFFFFFF)     //文件夹不存在   
							{
								CreateDirectory(".\\事务公告", NULL);
							}
							/*	else   if(dwAttr   &   FILE_ATTRIBUTE_DIRECTORY)     //是文件夹
							{
							//do   something
							}*/
							DWORD   dwAttrtxt = GetFileAttributes(".\\事务公告\\事务公告.txt");
							if (dwAttrtxt == 0xFFFFFFFF)     //文件夹不存在   
							{
								HANDLE   hFile = CreateFile(".\\事务公告\\事务公告.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW,
									FILE_ATTRIBUTE_NORMAL, NULL);
								//	DWORD   ByteWritten;   
								//	WriteFile(hFile,"测试,输入公告内容!",38,&ByteWritten,NULL);
								//WriteText(".\\事务公告\\事务公告.txt");
							}

							//----------------------------------------------

							if (!isMsg)//如果当前状态没有在发送公告
							{
								//我们打开文件并读出内容,很简单,不懂看下C语言的书；
								FILE *fp;
								fp = fopen(".\\事务公告\\事务公告.txt", "r");	//以只读方式打开,因为我们不要对文件进行修改；
								if (!fp)
								{
									//如果没打开MSG.TXT我们输出日志；
									MyLog(0, "打开公告文件 事务公告.txt失败!请检查文件是否存在或是否写入了公告内容!");
								}
								else
								{
									//如果正确打开,我们打文本文件内容读出并存储进刚才定义的MSGBUF数组中；
									MsgCount = 0;		//我们先把公告记录设为0；
									while (fgets(MsgBuf[MsgCount], 255, fp))//一行一行读,直到文件结束；
									{
										MsgCount++;	//公告条数加1；
									}
									fclose(fp);	//关闭打开的文件；
												//把ISMSG设置成TRUE,并启动线程；
									isMsg = true;
									CreateThread(NULL, 0, PubMsg, NULL, 0, 0);
									MyLog(0, "事务自动发送公告启动!");//输出日志；
								}
							}
							else
							{
								//退出时把ISMSG设为FALSE,线程自动结束 ；
								isMsg = false;
								MyLog(0, "事务自动发送公告结束!");
							}
						}
						break;


						//--------------------------------------------------------------------
						case VK_F12://欢迎自动公告
						{
							if (!isMsg)//如果当前状态没有在发送公告
							{
								MsgCount = 0;		//我们先把公告记录设为0；
								MsgCount++;	//公告条数加1；
								isMsg = true;
								CreateThread(NULL, 0, PubMsgs, NULL, 0, 0);
								MyLog(0, "欢迎自动发送公告启动!");//输出日志；
							}
							//	}
							else
							{
								//退出时把ISMSG设为FALSE,线程自动结束 ；
								isMsg = false;
								MyLog(0, "欢迎自动发送公告结束!");
							}
						}

						//--------------------------------------------------------------------------
						default:
						{
							// Nothing Now.
						}
						break;
						}
					}
				}
			}
			goto FinishAgentServer;
		}
	}
	//__except(pException = GetExceptionInformation())
	__except (DumpException(GetExceptionInformation(), "Exception Raised on CheckkProtect()"))
	{
		//DumpException( pException, "Exception Raised on main()" );
		ProcessEnd(FINISH_TYPE_UNKNOWN_ERROR);
	}
	// 鉴辑 瘤懦 巴.
FinishAgentServer:
	MyLog(LOG_NORMAL, "-- AgentServer ShutDown :: Now Release INetwork Module, ServerTable");
	EndAgentServer();

}

int main()
{
	//INPUT_RECORD				irBuffer;
	//DWORD						dwResult;
	COORD						dwSize;
	SMALL_RECT					srect;
	CONSOLE_SCREEN_BUFFER_INFO	info;
	BOOL						result;
	
	EXCEPTION_POINTERS* pException = NULL; // 030514 kyo

	// Console Initialize
	// ------------------
	AllocConsole();

	//memset(&irBuffer,0,sizeof(INPUT_RECORD));

	dwSize.X = 120;
	dwSize.Y = 500;

	hIn = GetStdHandle(STD_INPUT_HANDLE);
	g_hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleScreenBufferSize(g_hOut,dwSize);
	GetConsoleScreenBufferInfo(g_hOut,&info);
	dwSize = GetLargestConsoleWindowSize(g_hOut);

	srect.Top = 0;
	srect.Left = 0;
	srect.Right = info.dwMaximumWindowSize.X-1;
	srect.Bottom = info.dwMaximumWindowSize.Y-1;

	result = SetConsoleWindowInfo(g_hOut,TRUE, &srect);
	// ------------------

	// Init Log
	InitMyLog();

	char szNationName[100] = {0,} ;//021007 lsw
	if( GetPrivateProfileString( "NATION_SET", "NATION_NAME", "" , szNationName, 50,AGENT_SERVER_INI_ ) )
	{
		bool bIsFree = (bool)GetPrivateProfileInt( "NATION_SET", "BETA_SERVER", 0, AGENT_SERVER_INI_ );
		if(!LocalMgr.InitVersion(szNationName,bIsFree)){return false;}
	}
	else
	{
		return false;
	}

	MainLoop();

	//if( !StartAgentServer() )
	//{
	//	goto FinishAgentServer;
	//}


	//// Make connection with PROXY
	//g_pINet->ResumeTimer( 0 );
	//g_pINet->ResumeTimer( 2 );

//	__try// 030514 kyo
//	{
//		if(LocalMgr.IsAbleNation(THAI) && !LocalMgr.IsFreeBeta() )
//		{	//030129 lsw 咯扁啊 Gump 辑滚啊 嘿阑 何盒.
//			/////////////////////////////////////////
//			if( 0 < g_cAsyncPay.InitConnectPayServer() )
//			{
//				_beginthread( Paythread, 0, NULL );
//					
//				MSG msg;
//				//while( PeekMessage( &msg, NULL, 0,0, PM_REMOVE) )//)GetMessage(&msg,NULL,0,0) )
//				//while( PeekMessage( &msg, NULL, 0,0, PM_NOREMOVE) )//)GetMessage(&msg,NULL,0,0) )
//				while( GetMessage(&msg,NULL,0,0) )
//				{
//					TranslateMessage(&msg);
//					DispatchMessage(&msg);
//				}
//			}
//			else
//			{
//				MyLog( 0, "GUMP Connection Fail" );
//				goto FinishAgentServer;
//			}
//			
//		}
//		
//		else
//		{
//			while ( g_pServerTable->IsServerRunning() )
//			{
//				ReadConsoleInput(hIn,&irBuffer,1,&dwResult);
//			
//				if (irBuffer.EventType == KEY_EVENT)
//				{
//					
//					if (irBuffer.Event.KeyEvent.bKeyDown)
//					{
//						switch ( irBuffer.Event.KeyEvent.wVirtualKeyCode )
//						{
//						case VK_ESCAPE:
//							{
//								if( MessageBox( NULL, "Are you sure to DESTROY this agent server?", "IMPORTANT", MB_YESNO ) == IDYES )
//								{
//									g_pServerTable->DestroyServer( FINISH_TYPE_NORMAL );
//								}
//							}
//							break;
//						case VK_F1:
//							{
//								SetEvent(hKeyEvent[0]);
//							}
//							break;
//
//
////---------------------------------------//事务自动公告
//						case VK_F11:
//							{
//							
//							// ---------------------------------------------//创建记录文件夹
//		
//							DWORD   dwAttr   =   GetFileAttributes(".\\事务公告");   
//							if(dwAttr   ==   0xFFFFFFFF)     //文件夹不存在   
//							{ CreateDirectory(".\\事务公告",NULL);  } 
//							/*	else   if(dwAttr   &   FILE_ATTRIBUTE_DIRECTORY)     //是文件夹   
//							{   
//							//do   something   
//							}*/
//							DWORD   dwAttrtxt   =   GetFileAttributes(".\\事务公告\\事务公告.txt");   
//							if(dwAttrtxt   ==   0xFFFFFFFF)     //文件夹不存在   
//							{ 
//							HANDLE   hFile =	CreateFile(".\\事务公告\\事务公告.txt",GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_NEW,   
//							FILE_ATTRIBUTE_NORMAL,NULL);
//							//	DWORD   ByteWritten;   
//						//	WriteFile(hFile,"测试,输入公告内容!",38,&ByteWritten,NULL);
//					//WriteText(".\\事务公告\\事务公告.txt");
//							}
//
//							//----------------------------------------------
//
//								if(!isMsg)//如果当前状态没有在发送公告
//								{
//									//我们打开文件并读出内容,很简单,不懂看下C语言的书；
//									FILE *fp;
//									fp = fopen(".\\事务公告\\事务公告.txt","r");	//以只读方式打开,因为我们不要对文件进行修改；
//								if(!fp)
//								{
//									//如果没打开MSG.TXT我们输出日志；
//									MyLog(0,"打开公告文件 事务公告.txt失败!请检查文件是否存在或是否写入了公告内容!");
//								}
//								else
//								{
//									//如果正确打开,我们打文本文件内容读出并存储进刚才定义的MSGBUF数组中；
//									MsgCount=0;		//我们先把公告记录设为0；
//									while(fgets(MsgBuf[MsgCount],255,fp))//一行一行读,直到文件结束；
//									{
//									MsgCount ++;	//公告条数加1；
//									}
//									fclose(fp);	//关闭打开的文件；
//									//把ISMSG设置成TRUE,并启动线程；
//									isMsg=true;
//									CreateThread(NULL,0,PubMsg,NULL,0,0);
//									MyLog(0,"事务自动发送公告启动!");//输出日志；
//								}
//								}
//								else
//								{
//									//退出时把ISMSG设为FALSE,线程自动结束 ；
//									isMsg=false;
//									MyLog(0,"事务自动发送公告结束!");
//								}
//							}							
//							break;
//
//
//				//--------------------------------------------------------------------
//						case VK_F12://欢迎自动公告
//							{
//								if(!isMsg)//如果当前状态没有在发送公告
//								{
//									MsgCount=0;		//我们先把公告记录设为0；
//									MsgCount ++;	//公告条数加1；
//									isMsg=true;
//									CreateThread(NULL,0,PubMsgs,NULL,0,0);
//									MyLog(0,"欢迎自动发送公告启动!");//输出日志；
//								}
//							//	}
//								else
//								{
//									//退出时把ISMSG设为FALSE,线程自动结束 ；
//									isMsg=false;
//									MyLog(0,"欢迎自动发送公告结束!");
//								}
//							}
//
//				//--------------------------------------------------------------------------
//						default:
//							{
//								// Nothing Now.
//							}
//							break;
//						}
//					}
//				}
//			}
//			goto FinishAgentServer;
//		}
//	}
//	//__except(pException = GetExceptionInformation())
//	__except (DumpException(GetExceptionInformation(), "Exception Raised on CheckkProtect()"))
//	{
//		//DumpException( pException, "Exception Raised on main()" );
//		ProcessEnd(FINISH_TYPE_UNKNOWN_ERROR);
//	}
//	// 鉴辑 瘤懦 巴.
//FinishAgentServer:
//	MyLog( LOG_NORMAL, "-- AgentServer ShutDown :: Now Release INetwork Module, ServerTable" );
//	EndAgentServer();

	MyLog( LOG_NORMAL, "-- AgentServer Shutdown :: Now Release Console and Log Resources" );
	FreeMyLog();
	FreeConsole();

	return 0;
}

void Paythread( void *pVoid )
{
	INPUT_RECORD	irBuffer;
	DWORD			dwResult;

	while ( g_pServerTable->IsServerRunning() )
	{
		ReadConsoleInput(hIn,&irBuffer,1,&dwResult);
		
		//GetMessage(&msg,NULL,0,0);
		//TranslateMessage(&msg);
		//DispatchMessage(&msg);
		
		if (irBuffer.EventType == KEY_EVENT)
		{
			
			if (irBuffer.Event.KeyEvent.bKeyDown)
			{
				switch ( irBuffer.Event.KeyEvent.wVirtualKeyCode )
				{
				case VK_ESCAPE:
					{	
						if( MessageBox( NULL, "Are you sure to DESTROY this agent server?", "IMPORTANT", MB_YESNO ) == IDYES )
						{
							g_pServerTable->DestroyServer( FINISH_TYPE_NORMAL );
							MyLog(LOG_NORMAL, "-- AgentServer Shutdown " );
							g_cAsyncPay.LogoutAllUser(); // 030626 kyo
							PostMessage( m_hProcWnd, WM_QUIT, 0, 0 );		// 030422 kyo // 胶饭靛 辆丰
						}
					}
					break;
				case VK_F1:
					{
						SendMessage( m_hProcWnd, WM_USER_ALIVE_PAY, 0,0); // 030626 kyo 
						SetEvent(hKeyEvent[0]);
					}
					break;
				default:
					{
						// Nothing Now.
					}
					break;
				}
			}
		}
	}

	return;
}

DWORD WINAPI PubMsgs(LPVOID lpParameter)//这个是发送 欢迎公告 的线程
{
	int i = 0;				//定义一个变量来存储当前发送的信息是第几条；
	while(isMsg)			//用isMsg来控制发送,如果为TRUE就一直发送；
	{
	sprintf(MsgBufs[i],"[小亮龙族]提示：主动杜绝非法外挂,体验龙族之旅! 当前在线人数: %d 人!",g_pServerTable->GetNumOfUsers());//这个函数是取得当前在线用户数的
	SendPbs(MsgBufs[i],255);	//调用AGENT里的发送公告的函数来发送公告；
	i++;					//当前发送的信息+1,用于发送下一条；
	if(i>100) i=0;		//如果发送的信息大于总信息数,就是发完了,从头开始；
	Sleep(300*1000);			//等待30秒再执行,就是发了一条公告要等5秒再发下一条；
	}
	ExitThread(0);			//退出循环时退出线程；
	return 0;
}
DWORD WINAPI PubMsg(LPVOID lpParameter)//这个是发送事务公告的,就是维护什么的
{
	int i = 0;				//定义一个变量来存储当前发送的信息是第几条；
	while(isMsg)			//用isMsg来控制发送,如果为TRUE就一直发送；
	{
	SendPbs(MsgBuf[i],255);	//调用AGENT里的发送公告的函数来发送公告；
	i++;					//当前发送的信息+1,用于发送下一条；
	if(i>MsgCount) i=0;		//如果发送的信息大于总信息数,就是发完了,从头开始；
	Sleep(5000);			//等待5秒再执行,就是发了一条公告要等5秒再发下一条；
	}
	ExitThread(0);			//退出循环时退出线程；
	return 0;
}
