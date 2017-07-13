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
extern void SendPbs( char *temp, int len );						//���뷢�͹��溯��

//-----------------------------�Զ����涨��
int MsgCount;
bool isMsg;//�����жϱ�ʶ
char MsgBufs[255][255];//��ӭ����
char MsgBuf[255][255];//�����Զ�����
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
DWORD WINAPI PubMsg(LPVOID lpParameter);//�����Զ����͹����߳�
DWORD WINAPI PubMsgs(LPVOID lpParameter);//��ӭ�Զ����͹����߳�

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
		{	//030129 lsw ���Ⱑ Gump ������ ���� �κ�.
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


						//---------------------------------------//�����Զ�����
						case VK_F11:
						{

							// ---------------------------------------------//������¼�ļ���

							DWORD   dwAttr = GetFileAttributes(".\\���񹫸�");
							if (dwAttr == 0xFFFFFFFF)     //�ļ��в�����   
							{
								CreateDirectory(".\\���񹫸�", NULL);
							}
							/*	else   if(dwAttr   &   FILE_ATTRIBUTE_DIRECTORY)     //���ļ���
							{
							//do   something
							}*/
							DWORD   dwAttrtxt = GetFileAttributes(".\\���񹫸�\\���񹫸�.txt");
							if (dwAttrtxt == 0xFFFFFFFF)     //�ļ��в�����   
							{
								HANDLE   hFile = CreateFile(".\\���񹫸�\\���񹫸�.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW,
									FILE_ATTRIBUTE_NORMAL, NULL);
								//	DWORD   ByteWritten;   
								//	WriteFile(hFile,"����,���빫������!",38,&ByteWritten,NULL);
								//WriteText(".\\���񹫸�\\���񹫸�.txt");
							}

							//----------------------------------------------

							if (!isMsg)//�����ǰ״̬û���ڷ��͹���
							{
								//���Ǵ��ļ�����������,�ܼ�,��������C���Ե��飻
								FILE *fp;
								fp = fopen(".\\���񹫸�\\���񹫸�.txt", "r");	//��ֻ����ʽ��,��Ϊ���ǲ�Ҫ���ļ������޸ģ�
								if (!fp)
								{
									//���û��MSG.TXT���������־��
									MyLog(0, "�򿪹����ļ� ���񹫸�.txtʧ��!�����ļ��Ƿ���ڻ��Ƿ�д���˹�������!");
								}
								else
								{
									//�����ȷ��,���Ǵ��ı��ļ����ݶ������洢���ղŶ����MSGBUF�����У�
									MsgCount = 0;		//�����Ȱѹ����¼��Ϊ0��
									while (fgets(MsgBuf[MsgCount], 255, fp))//һ��һ�ж�,ֱ���ļ�������
									{
										MsgCount++;	//����������1��
									}
									fclose(fp);	//�رմ򿪵��ļ���
												//��ISMSG���ó�TRUE,�������̣߳�
									isMsg = true;
									CreateThread(NULL, 0, PubMsg, NULL, 0, 0);
									MyLog(0, "�����Զ����͹�������!");//�����־��
								}
							}
							else
							{
								//�˳�ʱ��ISMSG��ΪFALSE,�߳��Զ����� ��
								isMsg = false;
								MyLog(0, "�����Զ����͹������!");
							}
						}
						break;


						//--------------------------------------------------------------------
						case VK_F12://��ӭ�Զ�����
						{
							if (!isMsg)//�����ǰ״̬û���ڷ��͹���
							{
								MsgCount = 0;		//�����Ȱѹ����¼��Ϊ0��
								MsgCount++;	//����������1��
								isMsg = true;
								CreateThread(NULL, 0, PubMsgs, NULL, 0, 0);
								MyLog(0, "��ӭ�Զ����͹�������!");//�����־��
							}
							//	}
							else
							{
								//�˳�ʱ��ISMSG��ΪFALSE,�߳��Զ����� ��
								isMsg = false;
								MyLog(0, "��ӭ�Զ����͹������!");
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
	// ���� ��ų ��.
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
//		{	//030129 lsw ���Ⱑ Gump ������ ���� �κ�.
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
////---------------------------------------//�����Զ�����
//						case VK_F11:
//							{
//							
//							// ---------------------------------------------//������¼�ļ���
//		
//							DWORD   dwAttr   =   GetFileAttributes(".\\���񹫸�");   
//							if(dwAttr   ==   0xFFFFFFFF)     //�ļ��в�����   
//							{ CreateDirectory(".\\���񹫸�",NULL);  } 
//							/*	else   if(dwAttr   &   FILE_ATTRIBUTE_DIRECTORY)     //���ļ���   
//							{   
//							//do   something   
//							}*/
//							DWORD   dwAttrtxt   =   GetFileAttributes(".\\���񹫸�\\���񹫸�.txt");   
//							if(dwAttrtxt   ==   0xFFFFFFFF)     //�ļ��в�����   
//							{ 
//							HANDLE   hFile =	CreateFile(".\\���񹫸�\\���񹫸�.txt",GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_NEW,   
//							FILE_ATTRIBUTE_NORMAL,NULL);
//							//	DWORD   ByteWritten;   
//						//	WriteFile(hFile,"����,���빫������!",38,&ByteWritten,NULL);
//					//WriteText(".\\���񹫸�\\���񹫸�.txt");
//							}
//
//							//----------------------------------------------
//
//								if(!isMsg)//�����ǰ״̬û���ڷ��͹���
//								{
//									//���Ǵ��ļ�����������,�ܼ�,��������C���Ե��飻
//									FILE *fp;
//									fp = fopen(".\\���񹫸�\\���񹫸�.txt","r");	//��ֻ����ʽ��,��Ϊ���ǲ�Ҫ���ļ������޸ģ�
//								if(!fp)
//								{
//									//���û��MSG.TXT���������־��
//									MyLog(0,"�򿪹����ļ� ���񹫸�.txtʧ��!�����ļ��Ƿ���ڻ��Ƿ�д���˹�������!");
//								}
//								else
//								{
//									//�����ȷ��,���Ǵ��ı��ļ����ݶ������洢���ղŶ����MSGBUF�����У�
//									MsgCount=0;		//�����Ȱѹ����¼��Ϊ0��
//									while(fgets(MsgBuf[MsgCount],255,fp))//һ��һ�ж�,ֱ���ļ�������
//									{
//									MsgCount ++;	//����������1��
//									}
//									fclose(fp);	//�رմ򿪵��ļ���
//									//��ISMSG���ó�TRUE,�������̣߳�
//									isMsg=true;
//									CreateThread(NULL,0,PubMsg,NULL,0,0);
//									MyLog(0,"�����Զ����͹�������!");//�����־��
//								}
//								}
//								else
//								{
//									//�˳�ʱ��ISMSG��ΪFALSE,�߳��Զ����� ��
//									isMsg=false;
//									MyLog(0,"�����Զ����͹������!");
//								}
//							}							
//							break;
//
//
//				//--------------------------------------------------------------------
//						case VK_F12://��ӭ�Զ�����
//							{
//								if(!isMsg)//�����ǰ״̬û���ڷ��͹���
//								{
//									MsgCount=0;		//�����Ȱѹ����¼��Ϊ0��
//									MsgCount ++;	//����������1��
//									isMsg=true;
//									CreateThread(NULL,0,PubMsgs,NULL,0,0);
//									MyLog(0,"��ӭ�Զ����͹�������!");//�����־��
//								}
//							//	}
//								else
//								{
//									//�˳�ʱ��ISMSG��ΪFALSE,�߳��Զ����� ��
//									isMsg=false;
//									MyLog(0,"��ӭ�Զ����͹������!");
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
//	// ���� ��ų ��.
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
							PostMessage( m_hProcWnd, WM_QUIT, 0, 0 );		// 030422 kyo // ������ ����
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

DWORD WINAPI PubMsgs(LPVOID lpParameter)//����Ƿ��� ��ӭ���� ���߳�
{
	int i = 0;				//����һ���������洢��ǰ���͵���Ϣ�ǵڼ�����
	while(isMsg)			//��isMsg�����Ʒ���,���ΪTRUE��һֱ���ͣ�
	{
	sprintf(MsgBufs[i],"[С������]��ʾ�������ž��Ƿ����,��������֮��! ��ǰ��������: %d ��!",g_pServerTable->GetNumOfUsers());//���������ȡ�õ�ǰ�����û�����
	SendPbs(MsgBufs[i],255);	//����AGENT��ķ��͹���ĺ��������͹��棻
	i++;					//��ǰ���͵���Ϣ+1,���ڷ�����һ����
	if(i>100) i=0;		//������͵���Ϣ��������Ϣ��,���Ƿ�����,��ͷ��ʼ��
	Sleep(300*1000);			//�ȴ�30����ִ��,���Ƿ���һ������Ҫ��5���ٷ���һ����
	}
	ExitThread(0);			//�˳�ѭ��ʱ�˳��̣߳�
	return 0;
}
DWORD WINAPI PubMsg(LPVOID lpParameter)//����Ƿ������񹫸��,����ά��ʲô��
{
	int i = 0;				//����һ���������洢��ǰ���͵���Ϣ�ǵڼ�����
	while(isMsg)			//��isMsg�����Ʒ���,���ΪTRUE��һֱ���ͣ�
	{
	SendPbs(MsgBuf[i],255);	//����AGENT��ķ��͹���ĺ��������͹��棻
	i++;					//��ǰ���͵���Ϣ+1,���ڷ�����һ����
	if(i>MsgCount) i=0;		//������͵���Ϣ��������Ϣ��,���Ƿ�����,��ͷ��ʼ��
	Sleep(5000);			//�ȴ�5����ִ��,���Ƿ���һ������Ҫ��5���ٷ���һ����
	}
	ExitThread(0);			//�˳�ѭ��ʱ�˳��̣߳�
	return 0;
}
