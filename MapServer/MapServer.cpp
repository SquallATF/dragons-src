#include "LowerLayers\protocol.h"
#include "LowerLayers\servertable.h"
#include "LowerLayers\monitor.h"
#include "LowerLayers\define.h"
#include "LowerLayers\recvmsg.h"
#include "LowerLayers\GameTimer.h"
#include "LowerLayers\GameTimer.h"
#include "mapserver.h"
#include "LowerLayers\network_guid.h"
#include "LowerLayers\mylog.h"

#include "HigherLayers\DefaultHeader.h"
#include "HigherLayers\Scrp_int.h"
#include "HigherLayers\NPC_Pattern.h"
#include "HigherLayers\MenuServer.h"
//#include "HigherLayers\citem.h"	//050224_KCH 마일리지몰 작업
#include "HigherLayers\ItemList.h"
#include "HigherLayers\LogManager.h"
#include "HigherLayers\UserManager.h"

I4DyuchiNET* g_pINet = NULL;
HANDLE hKeyEvent[5] = { NULL, };	// 010205 KHS

// 001211 KHS
extern char MapName[MAX_PATH];
extern int MAX_EXP_LEVEL;  // CSD-040204
extern int MAX_TAC_LEVEL;  // CSD-040204
extern int MAX_JOB_LEVEL;  // CSD-040223

extern int Load_NPC_ItemGenerateTable();
extern int initItem(void);
extern void CloseNationSystem();  // 010904 LTS
extern void CloseLocalWarSystem();	// LTS LOCALWAR
extern void CloseEventLocalWarSystem();	// 020115 LTS

extern void InitLimitedTimeAndAge();	// 030929 kyo

bool InitMapServer()
{	//< CSD-TW-030622
	return Init();
}	//> CSD-TW-030622

void __stdcall ShowMapServerStatus(DWORD dwValue)//020511 lsw		// VK_F1
{
	if (!g_pServerTable)
	{
		MyLog(LOG_NORMAL, "CServerTable Is Not Available.");
	}
	else
	{
		LocalMgr.DisplayLocalizingSet();//030102 lsw
		g_pServerTable->ShowServerStatus();
	}

	return;
}

extern int InitEffectTbl();
extern int InitMagicTbl();
extern void ReLoadLocalWarfieldData();	// 011215 LTS
extern void ReLoadNationData();			// 011218 LTS
extern void ReLoadEventLocalWarData();			// LTS BUG
extern void LoadAIConfig();				// LTS AI
extern int InitMapServerConfigINI();	// 020115 LTS		// LTS NEW LOCALWAR
//acer7
extern int g_NoPkMode;

extern int InitLoadQuestTable();

void __stdcall ReLoadGameServerDataByKeyInput(DWORD dwValue)//020511 lsw		// vk_f5
{
	if (!g_pServerTable)
	{
		MyLog(LOG_NORMAL, "ReLoadGameServerData() :: Server Is not Ready, Retry Later");
		return;
	}

	//020121 KBS
	g_pServerTable->SetServerStatus(STATUS_RELOAD_GAMESERVER_DATA);

	//010305 YGI
	LoadAIConfig();				// LTS AI		// 030919 HK YGI
	Load_NPC_ItemGenerateTable();
	initItem();
	LoadScript(MapName);		// 0201
	LoadFilterWord();			// 0527 YGI
	LoadGetGodTable();			// 0605 YGI
	InitSkillTable();
	//LoadAIConfig();				// LTS AI 		// 030919 HK YGI
	InitSkillMapTable();
	initNPCGenerationTable();
	LoadItemJoinTable();
	LoadGenerateSpecialItem();	//1027 zhh
	MakeStoreList();

	LoadItemTable();//020505 lsw

	// 011020 YGI
	InitGameItem();
	InitRefreshMenu();
	InitEffectTbl();
	InitMagicTbl();
	ReLoadLocalWarfieldData();	// 011215 LTS
	ReLoadNationData();			// 011218 LTS
	ReLoadEventLocalWarData();	// LTS BUG
	InitMapServerConfigINI();	// 020115 LTS		// LTS NEW LOCALWAR
	MyLog(LOG_NORMAL, "Re-Loaded a part of 'GameServerData'");

	//020121 KBS
	g_pServerTable->SetServerStatus(STATUS_FINISH_RELOAD_GAMESERVER_DATA);
	SetGlobalBlockValues();

	InitLoadQuestTable();	// 021015 kyo
	SetGlobalBlockValues();
	if (LottoSystem())
	{
		LottoSystem()->LoadTable(g_hDBC_DragonDB);
	}

	if (LocalMgr.IsAbleNation(THAI)) // 030929 kyo
	{
		::InitLimitedTimeAndAge();
	}
}


// 021128 YGI
// 030211 YGI
void __stdcall SaveNPCStatusByKeyInput(DWORD dwValue)//020511 lsw
{
	if (!g_pServerTable)
	{
		MyLog(LOG_NORMAL, "SaveNPCStatus() :: Server Is not Ready, Retry Later");
		return;
	}

	FILE *fp;
	char tmp[FILENAME_MAX];
	int alive = 0;
	int eventnpc = 0;
	int dummy = 0;
	int save = 0;		// 기록할지 말지를 결정

	static char explain_str[3][50] =		// 
	{
		"GT_EVENT_MONSTER",
		"GT_SCENARIO_MONSTER",
		"GT_EVENT_NPC"
	};
	char *temp_str = 0;		// 설명글 받아 올 변수

	sprintf(tmp, "%02d.%02d %02d.%02d NPC Status.txt", g_mon + 1, g_day, g_hour, g_min);
	fp = fopen(tmp, "at+");
	if (fp)
	{
		fprintf(fp, "alive SprNo ScriptNo Name        X  Y \n");

		for (int i = NPC_LIST_START; i < MAX_NPC_LIST; i++)
		{
			if (NPCList[i].bAlive == REMOVE_)
				continue;

			alive++;
			save = 0;
			temp_str = 0;
			if (NPCList[i].eventno > 0)
			{
				if (NPCList[i].bAlive != REMOVE_) eventnpc++;
				save = 1;
			}

			if (NPCList[i].Race == DUMMY)
			{
				dummy++;
				save = 1;
			}

			if (save)
			{
				fprintf(fp, "%3d     %3d     %3d  %20s   %4d %4d\n", NPCList[i].bAlive,
					NPCList[i].SprNo, NPCList[i].eventno, NPCList[i].Name, NPCList[i].MoveSx, NPCList[i].MoveSy);
			}
			else
			{
				if (NPCList[i].generationtype == GT_EVENT_MONSTER)
				{
					temp_str = explain_str[0];
				}
				else if (NPCList[i].generationtype == GT_SCENARIO_MONSTER)
				{
					temp_str = explain_str[1];
				}
				else if (NPCList[i].generationtype == GT_EVENT_NPC)
				{
					temp_str = explain_str[2];
				}
				if (temp_str)
				{
					fprintf(fp, "%3d     %3d     %3d  %20s   %4d %4d	%s\n",
						NPCList[i].bAlive, NPCList[i].SprNo, NPCList[i].eventno, NPCList[i].Name,
						NPCList[i].MoveSx, NPCList[i].MoveSy, temp_str);
				}
			}
		}

		fprintf(fp, "Alive NPC No.       :  %d \n", alive);
		fprintf(fp, "Alive Event NPC No. :  %d \n\n", eventnpc);


		fclose(fp);
	}
}


void __stdcall AwaitingProxyServerConnect(DWORD dwValue)//020511 lsw
{
	// Proxy 에 연결되지 않은 경우.
	if (g_pServerTable && g_pServerTable->GetOwnProxyServerData() && !g_pServerTable->GetOwnProxyServerData()->dwConnectionIndex)
	{
		MyLog(LOG_NORMAL, "@@ Trying to connect PROXY Server ...");

		// Added by chan78 at 2001/04/11 :: 이미 접속 시도중인 경우 시도하지 않는다.
		if (g_pServerTable->GetConnectionResultData()->dwConnectionType != CONNECT_TYPE_NONE)
		{
			return;
		}

		// 접속 시도.
		if (g_pServerTable->ConnectToProxyServer())
		{
			// Modified by chan78 at 2001/04/11 :: 타이머 포즈 처리는 결과처리 콜백함수에서.
			return;
		}
	}
	return;
}

// Added by chan78 at 2001/02/08
extern t_connection	connections[DRAGON_MAX_CONNECTIONS_ + 1];
void __stdcall CleanUpDeadConnections(DWORD dwValue)//020511 lsw
{	//< CSD-CN-031213
	// 주기적으로 t_connection 배열을 뒤져서 해당 Agent로 사용자가 GHOST가 아닌지 체크하는 패킷을 날린다.
	// AGENT는 이 패킷을 받았을때 그 사용자가 존재하지 않으면 그 사용자가 GHOST 이므로 제거를 요청한다.
	// 제거할때는 일반적인 종료 PTCL 이 아니라 다른것을 사용하므로 따로 로그를 남긴다.
	// 한번 진입할때마다 반(홀/짝) 으로 나누어 처리한다.
	char pMsg[256 + 1] = { 0, };
	pMsg[0] = PTCL_MAP_TO_AGENT_CHECK_IS_USER_GHOST;

	LP_CHECK_IS_USER_GHOST_PACKET pPacket = (LP_CHECK_IS_USER_GHOST_PACKET)(pMsg + 1);

	CUserManager::HASH_USER mpUser = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = mpUser.begin(); i != mpUser.end(); ++i)
	{
		const int idGhost = i->second;

		if (connections[idGhost].dwAgentConnectionIndex && connections[idGhost].dwIDForAgent)
		{
			pPacket->dwID = connections[idGhost].dwIDForAgent;
			strcpy(pPacket->szID, connections[idGhost].id);
			// Send
			if (!g_pServerTable->Send(connections[idGhost].dwAgentConnectionIndex, pMsg, sizeof(BYTE) + sizeof(CHECK_IS_USER_GHOST_PACKET)))
			{
				closeconnection(connections, idGhost, -220);
			}
		}
	}
}	//> CSD-CN-031213

// 011020 YGI
#define CUSTOM_EVENT_NUM_ 7	//050224_KCH 마일리지몰 작업 MapServer를 ReleaseMode로 세팅할때, 배열값을 줄여서 세팅(10->7)

extern void InitNationValues();//030102 lsw
bool Init()
{
	CUSTOM_EVENT ev[CUSTOM_EVENT_NUM_];	// 010205 KHS
	// Added by chan78 at 2000/11/27
	ev[0].dwPeriodicTime = 1000;
	ev[0].pEventFunc = AwaitingProxyServerConnect;
	ev[1].dwPeriodicTime = GTE_ROUNDING_TIME;
	ev[1].pEventFunc = GameTimerProcess;
	ev[2].dwPeriodicTime = 75000;
	ev[2].pEventFunc = CleanUpDeadConnections;
	ev[3].dwPeriodicTime = 0;
	ev[3].pEventFunc = ShowMapServerStatus;
	ev[4].dwPeriodicTime = 0;
	ev[4].pEventFunc = ReLoadGameServerDataByKeyInput;
	ev[5].dwPeriodicTime = 0;
	ev[5].pEventFunc = SaveNPCStatusByKeyInput;
	ev[6].dwPeriodicTime = 0;
	ev[6].pEventFunc = MakeMapDataFile;

	DESC_NETWORK desc;
	desc.OnAcceptServer = OnAcceptServer;
	desc.OnAcceptUser = OnAcceptUser;
	desc.OnDisconnectServer = OnDisconnectServer;
	desc.OnDisconnectUser = NULL;
	desc.dwMainMsgQueMaxBufferSize = 2560000;			// 001214 khs  5,120,000 
	desc.dwMaxServerNum = 120;//020110 LSW 
	desc.dwMaxUserNum = 0;
	desc.dwServerBufferSizePerConnection = 128000;		// 001214 khs  512,000
	desc.dwServerMaxTransferSize = 65000;
	desc.dwUserBufferSizePerConnection = 0;
	desc.dwUserMaxTransferSize = 0;
	desc.OnRecvFromServerTCP = ReceivedMsgFromServer;
	desc.OnRecvFromUserTCP = ReceivedMsgFromUser;
	desc.dwCustomDefineEventNum = CUSTOM_EVENT_NUM_;
	desc.dwConnectNumAtSameTime = 200;//020110 LSW
	desc.pEvent = ev;
	desc.dwFlag = 0; // CSD-CN-031219 : NETDDSC_DEBUG_LOG : 로그기록

	HRESULT hr;
	CoInitialize(NULL);

	//CoCreateInstance(,);
	hr = CoCreateInstance(
		CLSID_4DyuchiNET,
		NULL,
		CLSCTX_SERVER,
		// CLSCTX_INPROC_SERVER,
		IID_4DyuchiNET,
		(void**)&g_pINet);

	if (FAILED(hr)) {
		//REGDB_E_CLASSNOTREG;
		void *pMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&pMsgBuf,
			0,
			NULL);

		MyLog(LOG_FATAL, "ERROR: Com Initialize Failed. Msg:%s", pMsgBuf);
		LocalFree(pMsgBuf);
		return false;
	}

	if (!g_pINet || !g_pINet->CreateNetwork(&desc, 10, 10))
	{
		return false;
	}

	// 일단 정지...	
	g_pINet->PauseTimer(0);	// 040406 kyo
	g_pINet->PauseTimer(1); // 040406 kyo
	g_pINet->PauseTimer(2); // 040406 kyo
	g_pINet->PauseTimer(3);	// 040406 kyo

	hKeyEvent[0] = g_pINet->GetCustomEventHandle(3);
	hKeyEvent[1] = g_pINet->GetCustomEventHandle(4);
	hKeyEvent[2] = g_pINet->GetCustomEventHandle(5);

	// 011020 YGI
	hKeyEvent[3] = g_pINet->GetCustomEventHandle(6);

	// 서버 테이블 생성.
	g_pServerTable = new CServerTable(MAP_SERVER_INI_, MAX_SERVER_NUM, g_pINet);
	SERVER_DATA * pServer = g_pServerTable->GetOwnServerData();

	if (!g_pServerTable)
	{
		MyLog(LOG_FATAL, "ERROR: g_pServerTable is NULL");
		return false;
	}

	char szNationName[100] = { 0, };
	if (GetPrivateProfileString("nation_set", "nation_name", "", szNationName, 50, MAP_SERVER_INI_))
	{
		const bool bIsFree = (GetPrivateProfileInt("NATION_SET", "BETA_SERVER", 0, MAP_SERVER_INI_)) ? true : false;
		if (!LocalMgr.InitVersion(szNationName, bIsFree)) { return false; }
		::InitNationValues();
	}
	else
	{
		MyLog(0, "Nation_Set Data Fault!!");
		return false;
	}

	char szLogPath[MAX_PATH];
	memset(szLogPath, 0, MAX_PATH);
	// 로그 디렉토리 생성
	if (GetPrivateProfileString("option", "LogPath", "", szLogPath, MAX_PATH, MAP_SERVER_INI_) > 0)
	{	//< CSD-040407
		g_pLogManager->CreateMainPath(szLogPath);
		g_pLogManager->CreateSubPath(LT_ACCELATER, "Accelater");
		g_pLogManager->CreateSubPath(LT_BATTLE, "Battle");
		g_pLogManager->CreateSubPath(LT_CHANGE, "Change");
		g_pLogManager->CreateSubPath(LT_CHECK, "Check");
		g_pLogManager->CreateSubPath(LT_LEVEL_UP, "LevelUp");
		g_pLogManager->CreateSubPath(LT_CHAT, "Chat");
		g_pLogManager->CreateSubPath(LT_SCRIPT, "Script");	// 030827 kyo
		g_pLogManager->CreateSubPath(LT_AUCTION, "Auction");	// BBD 040226	옥션로그 폴더
		g_pLogManager->CreateSubPath(LT_EVENTITEM, "EventItem");	// BBD 040308	EventItem Log 폴더
		g_pLogManager->CreateSubPath(LT_LOCAL_WAR, "LocalWar");	// CSD-040407
		g_pLogManager->CreateSubPath(LT_NEONATIONWAR, "NeoNationWar");		// LTH-040322-KO 1.04 신규 국가전 로그
		g_pLogManager->CreateSubPath(LT_MOVETOWARFIELD, "MoveToWarfield");	// LTH-040515-KO 1.04+ 평화기간 전쟁터 이동 관련 로그 폴더.
		g_pLogManager->CreateSubPath(KCH_ITEM_MALL_ITEM, "ItemMall_Item");	// 아이템몰 로그
		g_pLogManager->CreateSubPath(KCH_SKILL_RARE_UPGRADE_FAIL, "SKILL_RARE_UPGRADE_FAIL");	// 아이템몰 로그
		g_pLogManager->CreateSubPath(KCH_ITEM_MALL_ITEM_USE, "ItemMall_Item_Use");	// 아이템몰 아이템 사용시 변화되는 내용을 로그로 남김
	}	//> CSD-040407
	//< CSD-040223
	char szLevel[MAX_PATH];
	memset(szLevel, 0, MAX_PATH);

	if (GetPrivateProfileString("option", "max_exp_level", "", szLevel, MAX_PATH, MAP_SERVER_INI_) > 0)
	{
		MAX_EXP_LEVEL = atoi(szLevel);
		Dbg_Assert(MAX_EXP_LEVEL > 0 && MAX_EXP_LEVEL <= 200);
	}
	else
	{
		MAX_EXP_LEVEL = 200;//된섬 龜?
	}

	if (GetPrivateProfileString("option", "max_tac_level", "", szLevel, MAX_PATH, MAP_SERVER_INI_) > 0)
	{
		MAX_TAC_LEVEL = atoi(szLevel) - 1;
		Dbg_Assert(MAX_TAC_LEVEL > 0 && MAX_TAC_LEVEL <= 499);
	}
	else
	{
		MAX_TAC_LEVEL = 499;//濫섀 龜?
	}

	if (GetPrivateProfileString("option", "max_job_level", "", szLevel, MAX_PATH, MAP_SERVER_INI_) > 0)
	{
		MAX_JOB_LEVEL = atoi(szLevel);
		Dbg_Assert(MAX_JOB_LEVEL > 0 && MAX_JOB_LEVEL <= 100);
	}
	else
	{
		MAX_JOB_LEVEL = 100;//묏鱗세콘 龜?
	}

	MyLog(LOG_NORMAL, "max_exp_level(%d), max_tac_level(%d), max_job_level(%d)\n", MAX_EXP_LEVEL, MAX_TAC_LEVEL, MAX_JOB_LEVEL);
	//> CSD-040223
	return true;
}

//#include "HigherLayers/skill.h"
extern void DeleteSkillAll();
extern void DeleteMenuData();
extern void MapServerFree_kein();		// 020701 YGI

//021030 YGI
void EndMapServer()
{
	// 일단 정지...
	//맵서버 닫을때는 게임 타이머를 모두 닫아야 한다
	StopGameTimers();//020902 lsw

	SAFE_RELEASE(g_pINet);
	SAFE_DELETE(g_pServerTable);

	CloseNationSystem();  // 010904 LTS
	CloseLocalWarSystem();		// LTS LOCALWAR
	CloseEventLocalWarSystem();		// 020115 LTS

	DeleteSkillAll();
	DeleteMenuData();

	CoFreeUnusedLibraries();

	CoUninitialize();
	MapServerFree_kein();		// 020701 YGI
}
