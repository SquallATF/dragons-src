﻿#include "struct.h"
#include "servertable.h"
#include "usertable.h"
#include "recvmsg.h"
#include "packed.h"
#include "server.h"
#include "monitor.h"
#include "proxy.h"
#include "mylog.h"
#include "dr_agent_structures.h"
#include "DrServerManager.h"
// 011106 KBS
#include "RMTable.h"
//#include "RMDefine.h"
//< CSD-030322
#include "UserManager.h"
extern CUserManager g_mgrUser;
//> CSD-030322

void OnConnectUser(DWORD dwConnectionIndex);
void OnConnectServer(DWORD dwConnectionIndex);

char txt[512];
char szMsg[MM_MAX_PACKET_SIZE];

// 011106 KBS
extern void RMProc(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

// Added by chan78 at 2000/12/17
bool RequestClearPayTable( DWORD dwConnectionIndex )
{
	// 먼저 해당 AGENT의 정보를 구함.
	LP_SERVER_DATA pServerData = g_pServerTable->GetServerData( dwConnectionIndex );

	if( !pServerData )
	{
		return false;
	}
	if( pServerData->dwServerType != SERVER_TYPE_AGENT )
	{
		return false;
	}

	// 처리할 DB Demon을 선정
	LP_SERVER_DATA pDBDemon = g_pServerTable->GetServerListHead();
	DWORD dwTargetDBDemonCI = 0;
	for(; pDBDemon; pDBDemon = pDBDemon->pNextServerData )
	{
		if( pDBDemon->dwConnectionIndex && (pDBDemon->dwServerType == SERVER_TYPE_DB) )
		{
			dwTargetDBDemonCI = pDBDemon->dwConnectionIndex;
			break;
		}
	}

	if( dwTargetDBDemonCI == 0 )
	{
		return false;
	}

	// 패킷만듬
	char szMsg[64+1];

	szMsg[0] = (BYTE)PTCL_ORDER_TO_CLEAR_PAY_TABLE;
	memcpy( szMsg+1, &pServerData->wPort, 2 );

	if( !g_pServerTable->Send( dwTargetDBDemonCI, szMsg, 1+2 ) )
	{
		MyLog( LOG_FATAL, "RequestClearPayTable() :: Failed to Send Packet(TargetCI:%d, AgentPort:%d)", dwTargetDBDemonCI, pServerData->wPort );
		return false;
	}

	// 성공적으로 종료.
	return true;
}

void __stdcall OnAcceptUser(DWORD dwConnectionIndex)
{
	char msg[26];

	g_pProxy->dwTotalLogUser++;
	
	LP_SERVER_DATA pToAssign = g_pServerTable->GetAssignableAgentServer();
	// Max user check per Set
	if (g_pServerTable->GetNumOfUsersInServerSet() > g_pProxy->dwMaxUser)
	{
		g_pProxy->bLimitMaxUser = 0;
	}
	else g_pProxy->bLimitMaxUser = 1;
	// if proxy limit login try
	if (g_pProxy->bLimitLoginTryPerSec)
	{
		g_pProxy->bTryLoginThisSec++;
		if (g_pProxy->bLimitLoginTryPerSec < g_pProxy->bTryLoginThisSec)
			g_pProxy->bLimit = 0;
		else g_pProxy->bLimit = 1;
	} 
	else g_pProxy->bLimit = 1;
	
	if (!pToAssign || !g_pServerTable->IsUserAcceptAllowed() || g_pProxy->bLimit == 0 || g_pProxy->bLimitMaxUser == 0)
	{
		t_header buffer;
		if (g_pProxy->bLimit == 0 && g_pServerTable->IsUserAcceptAllowed() )
			buffer.type = 13005;
		else if (g_pProxy->bLimitMaxUser == 0 && g_pServerTable->IsUserAcceptAllowed() )
			buffer.type = 13005;
		else
			buffer.type = 10174;
		buffer.size = 0;

		// 현재 접속할 서버가 없음을 보낸다.
		g_pINet->SendToUser(dwConnectionIndex,(char*)&buffer,5,FLAG_SEND_NOT_ENCRYPTION);

		if ( g_pServerTable->IsUserAcceptAllowed() )
		{
			//MyLog( LOG_IMPORTANT, "WARNING : No Agent server are ready to service" );
		}
		else if (g_pProxy->bLimit == 0)
		{
			MyLog( LOG_JUST_DISPLAY, "INFO : Proxy LIMIT User login try per Sec");
		}
		else if (g_pProxy->bLimitMaxUser == 0)
		{
			MyLog( LOG_JUST_DISPLAY, "INFO : MAX USER LIMIT at this server set");
		}
		g_pProxy->dwFailtoAllocUserNum++;
	}
	else
	{
       // 접속한 유저에게 Agent의 IP를 보내준다. 
		t_header buffer;
		buffer.type = 8930;
		buffer.size = 21;
		memcpy(msg,&buffer,5);
		memcpy(msg+5,pToAssign->szIPForUser,18);
		memcpy(msg+5+19,&pToAssign->wPortForUser,2);
		

		if( !g_pINet->SendToUser(dwConnectionIndex,msg,26,FLAG_SEND_NOT_ENCRYPTION) )
		{
			MyLog( LOG_NORMAL, "Failed To Send To User (%d)", dwConnectionIndex );
		}
		pToAssign->dwNumOfUsers++;
		 
		//MyLog( LOG_NORMAL, "OK : User(%d) push to %s", dwConnectionIndex, pToAssign->szIPForUser);

		// add by slowboat
		g_pServerTable->AddNumOfUsersInServerSet();
	}

	// List에 추가.
	DWORD dwNewUserID = g_pUserTable->AddUser( dwConnectionIndex );
	if( dwNewUserID )
	{
		USERINFO *pNewUserInfo;
		pNewUserInfo = g_pUserTable->GetUserInfo( dwNewUserID );
		if( !pNewUserInfo )
		{
			MyLog( LOG_FATAL, "OnConnectUser() :: pNewUserInfo is NULL!!!(CI:%d/dwID:%d)", dwConnectionIndex, dwNewUserID );
			g_pINet->CompulsiveDisconnectUser( dwConnectionIndex );
		}
		else
		{
			if(LocalMgr.IsAbleNation(CHINA))//021007 lsw
			{
				MyLog( 0, "CLIENT at (CI:%d, IP:%s, PORT:%d)", dwConnectionIndex, pNewUserInfo->szIP, pNewUserInfo->wPort );
			}
			g_pUserTable->DisconnectUserBySuggest( pNewUserInfo );// 5초후에 끊기도록 한다.
		}
	}
	else
	{
		// CUserTable에 추가하지 못한 경우.
		g_pINet->CompulsiveDisconnectUser( dwConnectionIndex );
	}
	return;
}

void __stdcall OnAcceptServer(DWORD dwConnectionIndex)
{
	int k=0;
}

void __stdcall ReceivedMsgServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	BYTE bID;
	bID = (BYTE)pMsg[0];

	if (dwConnectionIndex == 0)
	{
#ifdef __ON_DEBUG
		//_asm int 3;
#endif
		return;
	}

	if( bID == (BYTE)PTCL_NOTIFY_SERVER_UP )
	{
		if( g_pServerTable->OnRecvServerUpMsg(dwConnectionIndex, *(WORD*)(pMsg+1)) )
		{
#ifdef __ON_DEBUG
			//_asm int 3;
#endif
		}
		return;
	}

	// 011106 KBS
	if( bID == PTCL_RM || bID == PTCL_RM_FROM_PROXY )
	{
		BYTE header;
		memcpy(&header,pMsg+1, 1);
		if( header == MSG_RM_LOGIN ) return;
		RMProc(dwConnectionIndex, pMsg, dwLength);
		return;
	}
	//



	LP_SERVER_DATA pSender = g_pServerTable->GetConnectedServerData( dwConnectionIndex );

	if( !pSender ) return;

	switch (bID)
	{
	// -------------
	// 기본패킷 
	// -------------
		// PROXY 전용
	case PTCL_REQUEST_SET_SERVER_LIST:
	case PTCL_REQUEST_TO_CONNECT_SERVER_LIST:
	case PTCL_REQUEST_SET_DB_DEMON:
	case PTCL_SERVER_LIST_SETTING_RESULT:
	case PTCL_SERVER_CONNECTING_RESULT:
	case PTCL_DB_DEMON_SETTING_RESULT:
	case PTCL_REPORT_SERVER_DATAS:
	case PTCL_REPORT_SERVER_DESTROY:
	case PTCL_REPORT_SERVER_STATUS:
	case PTCL_REPORT_SERVER_CONNECTION_STATUS_CHANGE:

		// 공통
	case PTCL_ORDER_DESTROY_SERVER:
	case PTCL_NOTIFY_SERVER_STATUS:
	case PTCL_SERVER_TRY_TO_CHECK_CONNECTION:
	case PTCL_SERVER_CONNECTION_OK:
	case PTCL_ORDER_TO_REPORT_SERVER_STATUS:
		{
			if( !g_pServerTable->OnRecvNegotiationMsgs( pSender, bID, pMsg+1, dwLength-1 ) )
			{
				MyLog( LOG_FATAL, "OnRecvNegotiationMsg() Failed :: (pSender(%d), bId(%d), MsgLength(%d))", pSender->wPort, bID, dwLength );
#ifdef __ON_DEBUG
				//_asm int 3;
#endif
			}
		}
		break;
	// Added by chan78 at 2001/03/16
	// -----------------------------
	case PTCL_MANAGER_QUERY:
		{
			// 있을 수 없다.
			MyLog( LOG_FATAL, "PTCL_MANAGER_QUERY :: has received!!!(%d)", pSender->wPort );
			g_pServerTable->DestroyServer( FINISH_TYPE_UNKNOWN_ERROR);
		}
		break;
	// Added by chan78 at 2001/03/16
	case PTCL_MANAGER_ANSWER:
		{
			if( !AnswerToManager( (LP_MANAGER_PACKET)pMsg, dwLength ) )
			{
				MyLog( LOG_FATAL, "PTCL_MANAGER_ANSWER :: AnswerToManager() has return false(%d)", pSender->wPort );
				break;
			}
		}
		break;
	case PTCL_PROXY_TO_ACCESS:
		{	//< CSD-030509
			t_packet* pPacket = (t_packet*)(pMsg + 5);

			const char* pID = pPacket->u.server_accept_login.id;

			if (g_mgrUser.IsExistLogin(pID))
			{
				MyLog(LOG_NORMAL, "Exist Login : %s", pID);
				break;
			}

			if (g_mgrUser.IsExistLogout(pID))
			{
				MyLog(LOG_NORMAL, "Exist Logout : %s", pID);
				break;
			}
			
			g_mgrUser.AddLogin(pID);

			pMsg[0] = BYTE(PTCL_AGENT_TO_COMMIT);

			if (!g_pServerTable->Send(dwConnectionIndex, pMsg, dwLength))
			{
				MyLog(LOG_IMPORTANT, "Failed To send 'PTCL_AGENT_TO_COMMIT' to Agent");
			}

			break;
		}	//> CSD-030509
	case PTCL_PROXY_TO_LOGOUT:
		{	//< CSD-030509
			const char* pName = pMsg + 1;

			if (g_mgrUser.IsExistLogin(pName))
			{
				if (!g_mgrUser.IsExistLogout(pName))
				{
					g_mgrUser.AddLogout(pName);
				}
			}
			
			g_mgrUser.DelLogin(pName);

			break;
		}	//> CSD-030509
//<! BBD 040311	맵으로부터의 동접수 요청
	case PTCL_SERVERSET_USERNUM_REQUEST:
		{
			pMsg[0] = BYTE(PTCL_SERVERSET_USERNUM_REQUEST);

			unsigned short count = g_pServerTable->m_dwNumOfUsersInServerSet;
			memcpy(&(pMsg[1]), &count, sizeof(count));

			g_pServerTable->Send(dwConnectionIndex, pMsg, 1 + sizeof(count));
			break;
		}
//> BBD 040311	맵으로부터의 동접수 요청
	default:
		{
			MyLog( LOG_FATAL, "Unknown bID(%d) Received. From %s(wPort:%d) Size(%d) ConnectionID(%d)", bID, GetTypedServerText(pSender->dwServerType), pSender->wPort, dwLength, dwConnectionIndex);
#ifdef __ON_DEBUG
//			_asm int 3;
#endif
		}
		break;
	}
}

//Added by KBS 020330
extern void MgrSend(DWORD dwConnectionIndex, void* pMsg, DWORD dwLength);
//
void __stdcall ReceivedMsgUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	// Added by chan78 at 2001/03/16
	// 접속된 클라이언트가 Packet을 보내는 경우, CONTROL_CLIENT의 인증 시도 패킷이 아닌경우
	// 서버에 위해를 끼치려는 시도로 가정, 필요한 기록을 남긴 후 접속을 끊는다.

	USERINFO *pUserInfo = NULL;

	if( !(pUserInfo = (USERINFO *)g_pINet->GetUserInfo( dwConnectionIndex )) )
	{
		MyLog( LOG_FATAL, "RecvMsgUser() :: pUserInfo is NULL!!!" );
		// 있어서는 안되는 상황.
		return;
	}

	// 인증 상태에 따른 처리.
	switch( pUserInfo->dwType )
	{
	case CLIENT_TYPE_ILLEGAL:
		{
			// 불량 클라이언트로부터 패킷이 날아오면 무시한다.
			// 이 경우 불량 클라이언트로 정의할때 CompulsiveDisconnect() 해 둔 상태이고.
			// 이 분류는 접속종료 처리가 되기 전까지 날아올 수 있는 패킷들을 처리한다.
			// 로그의 양을 줄이기 위해 한번 불량 클라이언트로 처리된것 들은 무시한다.
			return;
		}
		break;
	case CLIENT_TYPE_MANAGER:			// 인증된 MANAGER
		{
			//Modified 020330 KBS
			BYTE bID;
			bID = (BYTE)pMsg[0];
				
			if( bID == PTCL_RM || bID == PTCL_RM_FROM_PROXY )
			{
				RMProc(dwConnectionIndex, pMsg, dwLength);
				return;
			}
			//	
			
			
			//
			//// 여기서 디코딩한다.
			//// Not Yet.

			//// 디코딩해서 얻은 값은...
			//LP_MANAGER_PACKET pPacket = (LP_MANAGER_PACKET)pMsg;

			//if( dwLength < sizeof(MANAGER_PACKET_HEADER) )
			//{
			//	MyLog( LOG_IMPORTANT, "CLIENT_TYPE_MANAGER :: Illegal Packet(%d)", dwLength );
			//	MyLog( LOG_IMPORTANT, "_____ at (CI:%d, IP:%d, PORT:%d) (PacketSize:%d)", dwConnectionIndex, pUserInfo->szIP, pUserInfo->wPort );

			//	pUserInfo->dwType = CLIENT_TYPE_ILLEGAL;
			//	g_pINet->CompulsiveDisconnectUser( dwConnectionIndex );
			//	return;
			//}

			//if( !OnRecvMsgFromManager( pUserInfo, pPacket, dwLength ) )
			//{
			//	// 불량 CLIENT로 분류. 이 경우는 크래킹이라기보다는 버그일 확률이 높고, 해킹이라면 엄청 심각하다.
			//	MyLog( LOG_IMPORTANT, "CLIENT_TYPE_MANAGER :: Illegal Packet(wCMD:%d, CRC:%d, dwLength:%d)", pPacket->h.wCMD, pPacket->h.dwCRC, dwLength );
			//	MyLog( LOG_IMPORTANT, "_____ at (CI:%d, IP:%d, PORT:%d) (PacketSize:%d)", dwConnectionIndex, pUserInfo->szIP, pUserInfo->wPort );

			//	pUserInfo->dwType = CLIENT_TYPE_ILLEGAL;
			//	g_pINet->CompulsiveDisconnectUser( dwConnectionIndex );
			//	return;
			//}

			// 정상처리됨.
			return;
		}
		break;


	case CLIENT_TYPE_MANAGER_UNDER_AUTHENTICATION:
		{
			//Modified 020330 KBS
			BYTE header;
			memcpy(&header,pMsg+1, 1);

			switch(header)
			{
			case MSG_RM_LOGIN:
				{
					
					PACKET_RM_LOGIN *packet = (PACKET_RM_LOGIN*)pMsg;

					if(g_pRMTable->CheckCertainIP(dwConnectionIndex, packet->IP))
					{
						pUserInfo->dwType = CLIENT_TYPE_MANAGER;
						pUserInfo->dwStatus = STATUS_USER_ACTIVATED;

						// 접속종료 대기 리스트에서 빼준다.
						g_pUserTable->RemoveUserFromAwaitingDisconnectUserList( pUserInfo );

						//로그인 승인 메세지..   자신이 서버셋 넘버와 함께..
						PACKET_RM_LOGIN_OK pck(g_pServerTable->m_dwServerSetNumber);
						MgrSend(dwConnectionIndex, &pck, pck.GetPacketSize());

					}
					else
					{
						//등록된 IP가 아닌자리에서 로그인 한 경우 
						PACKET_RM_LOGIN_FAIL pck;
						MgrSend(dwConnectionIndex, &pck, pck.GetPacketSize());

						MyLog( LOG_IMPORTANT, "CLIENT_TYPE_MANAGER_UNDER_AUTHENTICATION :: Illegal Packet(%d)", dwLength );
						MyLog( LOG_IMPORTANT, "_____ at (CI:%d, IP:%s, PORT:%d) (PacketSize:%d)", dwConnectionIndex, pUserInfo->szIP, pUserInfo->wPort );
						pUserInfo->dwType = CLIENT_TYPE_ILLEGAL;
						g_pINet->CompulsiveDisconnectUser( dwConnectionIndex );
						return;
					}

					g_pRMTable->AddClient(dwConnectionIndex, packet);
					
					if(g_pRMTable->GetClientNum() == 1)	//RMClient가 하나라도 접속된 상태면 체크 시작!
					{
						StopWaitTimer();
						StartEchoTimer();			//서버 다운됐나 안됐나 체크 작업 시작
					}
				}
				break;
			}

			//


			//// 이곳에서 SSL(Secure Socket Layer)를 구성하고,

			//// 일단 MANAGER CLIENT일 가능성이 있다고 인정된 CLIENT 로서.
			//// 일련의 인증 과정을 거쳐 CLIENT_TYPE_MANAGER 로 인증되면 MANAGER 로서 역할을 수행할 수 있게된다.

			//// 인증용 패킷을 받는다.
			//if( !OnRecvAuthMsgFromManager( pUserInfo, (LP_MANAGER_PACKET)pMsg, dwLength ) )
			//{
			//	MyLog( LOG_IMPORTANT, "CLIENT_TYPE_MANAGER_UNDER_AUTHENTICATION :: Illegal Packet(%d)", dwLength );
			//	MyLog( LOG_IMPORTANT, "_____ at (CI:%d, IP:%d, PORT:%d) (PacketSize:%d)", dwConnectionIndex, pUserInfo->szIP, pUserInfo->wPort );
			//	pUserInfo->dwType = CLIENT_TYPE_ILLEGAL;
			//	g_pINet->CompulsiveDisconnectUser( dwConnectionIndex );
			//	return;
			//}
			//else
			//{
			//	// 인증 되었다.
			//	pUserInfo->dwType = CLIENT_TYPE_MANAGER;
			//	pUserInfo->dwStatus = STATUS_USER_ACTIVATED;

			//	// 접속종료 대기 리스트에서 빼준다.
			//	g_pUserTable->RemoveUserFromAwaitingDisconnectUserList( pUserInfo );

			//	// 20010508 Add Proxy-OwnPort 필요
			//	AnswerAuthPacket( pUserInfo );
			//}


		}
		break;
	case CLIENT_TYPE_NORMAL: // 일반 유저.
		{
			// 정상적인 클라이언트는 PROXY에 메시지를 보내지 않는다.
			// 하지만 MANAGER CLIENT 일 가능성이 있으므로,
			// 가장 처음 날아온 PACKET 으로 불량 CLIENT 와 MANAGER CLIENT를 1차적으로 분류한다.
			// --------------------------------

			//MyLog( LOG_IMPORTANT, "RecvPacket Size : %d, %d", dwLength, sizeof(MANAGER_PACKET_HEADER));

			if( dwLength <= sizeof(MANAGER_PACKET_HEADER) )
			{
				LP_MANAGER_PACKET pPacket = (LP_MANAGER_PACKET)pMsg;

				if( pPacket->h.wCMD == MANAGER_CLIENT_FIRST_PACKET_TYPE )
				{
					if( pPacket->h.dwCRC == MANAGER_CLIENT_FIRST_PACKET_CRC )
					{
						// 일단 MANAGER CLIENT 일 가능성을 인정한다.
						// pUserInfo->dwType = CLIENT_TYPE_MANAGER_UNDER_AUTHENTICATION;
						pUserInfo->dwType = CLIENT_TYPE_MANAGER_UNDER_AUTHENTICATION;	//  일단 무조건 인증.
						return;
					}
				}
			}
			// --------------------------------

			// 위 조건에서 걸러지지 않은 CLIENT 들은 불량.
			// 불량 접속시도로 기록하고 접속을 끊는다.
			MyLog( LOG_IMPORTANT, "--- ILLEGAL CLIENT at (CI:%d, IP:%s, PORT:%d)", dwConnectionIndex, pUserInfo->szIP, pUserInfo->wPort );
			
			pUserInfo->dwType = CLIENT_TYPE_ILLEGAL;
			g_pINet->CompulsiveDisconnectUser( dwConnectionIndex );
			return;
		}
	default:
		{
			// 범위를 벗어나는 타입을 가져서는 안된다.
			// 허용할 수 있는 결함으로 분류하면 더 큰 버그를 만들수도 있으므로
			// 서버를 죽인다.
			MyLog( LOG_FATAL, "RecvMsgUser() :: UNKNOWN 'CLIENT_TYPE'(%d)", pUserInfo->dwType );
#ifdef __ON_DEBUG
			_asm int 3;
#else
			g_pServerTable->DestroyServer( FINISH_TYPE_UNKNOWN_ERROR );
#endif
		}
	}

	return;
}

void __stdcall OnDisconnectUser(DWORD dwConnectionIndex)
{
	
	//Modified by KBS 020330
	USERINFO *pUserInfo = (USERINFO*)g_pINet->GetUserInfo( dwConnectionIndex );

	if(pUserInfo->dwType == CLIENT_TYPE_MANAGER)
	{
		g_pRMTable->RemoveClient( dwConnectionIndex );

		if(g_pRMTable->GetClientNum() == 0)	//RMClient가 모두 접속 해제 되었으면 체크 그만 
		{
			StopWaitTimer();
			StopEchoTimer();	
		}
	}
	//
	
	// Modified by chan78 at 2001/02/27
	g_pUserTable->RemoveUser( dwConnectionIndex );

	return;
}

//Added by KBS 011220
BOOL CheckValidConnection(DWORD dwConnectionIndex, int type)
{
	RM_LISTENER_INFO* cur = NULL;
	RM_LISTENER_INFO* next = NULL;

	cur = g_pRMTable->m_ListenerTable.m_ppInfoTable[ type ];

	while (cur)
	{
		next = cur->pNextInfo;
	
		if(cur->dwConnectionIndex == dwConnectionIndex)
		{
			return TRUE;
		}
			
		cur = next;
	}
	
	return FALSE;
}
//

void __stdcall OnDisconnectServer(DWORD dwConnectionIndex)
{
	//Added by KBS 011119
	void *pVoid = g_pINet->GetServerInfo(dwConnectionIndex);

	if(pVoid)	//Listener Disconnect
	{	
		BYTE bConnectType;
		memcpy(&bConnectType,pVoid,1);

		switch((int)bConnectType)
		{

		case RM_TYPE_TOOL:
			{
				// 021008 YGI
				//
				////Modified by KBS 011213
				//g_pRMTable->RemoveClient( dwConnectionIndex );

				//if(g_pRMTable->GetClientNum() == 0)	//RMClient가 모두 접속 해제 되었으면 체크 그만 
				//{
				//	StopWaitTimer();
				//	StopEchoTimer();	
				//}
			}
			break;

		case RM_TYPE_LISTENER:
			{
				//Modified by KBS 0112
				if(CheckValidConnection( dwConnectionIndex, 1 ))
				{
					//Listener가 접속 끊어짐..
					in_addr addr;
					addr.S_un = g_pINet->GetServerAddress( dwConnectionIndex )->sin_addr.S_un;
		
					g_pRMTable->m_ListenerTable.MoveToDisconnectStatus(inet_ntoa(addr));

					MyLog( LOG_NORMAL, "Valid Listener Disconnected :: dwConnectionIndex = %d", dwConnectionIndex );
				}
				else
				{
					char ip[20];	memset(ip,0,20);
					WORD port;
					g_pINet->GetServerAddress( dwConnectionIndex, ip, &port);
					
					MyLog( LOG_NORMAL, "Invalid Listener Disconnected :: dwConnectionIndex = %d, IP = %s, Port = %d",dwConnectionIndex, ip, port );
				}
			}
			break;
		}
	}
	else	//일반 서버 
	{	
		//Added by KBS 011205
		LP_SERVER_DATA pServerData;
		pServerData = g_pServerTable->GetConnectedServerData( dwConnectionIndex );

		if(pServerData)
		{
			PACKET_RM_SERVER_DISCONNECT packet((BYTE)g_pServerTable->m_dwServerSetNumber, 
												pServerData->wPort);
		
			g_pRMTable->BroadcastAllRMClient((char*)&packet,packet.GetPacketSize());
		}
		//
		
		g_pServerTable->RemoveConnectedServerDataFromHashTable(dwConnectionIndex);
		
	}
	
	return;
}