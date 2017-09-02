#include "stdafx.h"
#include <mmsystem.h>
#include <direct.h>
#include "LowerLayers\monitor.h"
#include "LowerLayers\GameTimer.h"
#include "mapserver.h"
#include "HigherLayers\DefaultHeader.h"
#include "HigherLayers\Rajasystem.h"
#include "LowerLayers\mylog.h"
#include "HigherLayers\Npclist.h"
#include "HigherLayers\MenuServer.h"
#include "HigherLayers\Winzs.h"
#include "HigherLayers\Scrp_int.h"
#include "HigherLayers\WeatherControl.h"
#include "HigherLayers\Op_Magic.h"
#include "HigherLayers\SealStone.h" // 001209 KHS 
#include "HigherLayers\AREA.h"		// 010205 KHS
#include "LowerLayers\servertable.h"	// 001215 KHS
#include "HigherLayers\TeamBattle.h"		// 010205 KHS
#include "HigherLayers\ChrLog.h"
#include "HigherLayers\UserManager.h"
#include "HigherLayers\SymbolItemMgr.h"//soto-030512
#include "HigherLayers\EventTreasureBoxMgr.h"//soto-030711

//< 1.04럽棍밈
#include "HigherLayers\Prison.h"

//< TJJ-050819-CN
#include "CoreException.h"

CPrison* g_cpPrison = NULL;
//> LTH-040722-KO

//<050225_KCH 
#include "HigherLayers\ItemMallManager.h"
//>

extern CSymbolItemMgr g_CSymbolMgr;//soto-030512
extern void InitLimitedTimeAndAge(); // 030929 kyo
// 011130 YGI
int DRAGON_MAX_CONNECTIONS;

//< LTH-040311-KO Frog Function을 위한 함수들
inline int ran(int r)
{
	if (r == 0) return 0;
	return rand() % r;
}

struct t_ItemNumber
{
	int	ItemNumber;
	int	nCount;
	t_ItemNumber()
	{
		nCount = 0;
		ItemNumber = 0;
	}
};

bool CheckSameItem(vector<t_ItemNumber>* pvtItemList, int nItemNumber)
{
	int	 count = pvtItemList->size();
	for (int i = 0; i < count; ++i)
	{
		if (nItemNumber == (*pvtItemList)[i].ItemNumber)
		{
			(*pvtItemList)[i].nCount++;
			return true;
		}
	}
	return false;
}

extern int		SerchItemNPCINV(CHARLIST* ch, int nItemNumber);
//> LTH-040311-KO
static unsigned int random_next;
inline int ran()
{
	random_next = random_next * 1103515245 + 12345;
	return (random_next / 65536);
}

void sran(DWORD cnt)
{
	random_next = cnt;
}

// 020707 YGI
int srand_()
{
	sran((unsigned)time(NULL));
	return 1;
}

int rand_()
{
	static int a = srand_();
	return ran();
}

// ----------------------------------------------------------------------------------------------
// Global Variables Begin
// ----------------------------------------------------------------------------------------------
int g_year = 0;
int g_mon = 0;	// 0 - 11
int g_day = 0;
int g_yday = 0;
int g_wday = 0;
int g_hour = 0;
int g_min = 0;
int g_sec = 0;

int g_count_ok;

DWORD global_time = 0;	// 1ms 단위..
DWORD g_alive_time = 0;
DWORD g_curr_time = 0;	// 1초단위..
DWORD g_curr_time_with_out_year = 0;		// 월, 날, 시간을 초단위로 변환, 년 은 포함되지 않는다.	// 031009 CI YGI
int   g_weatherflag = 0;	// 1 이면 날씨에 관해서 BroadCast하지 않느다. 


char bbsString[MAX_PATH];
int  BBSBroadcast;

int	 DRAGON_PORT = 0;

HANDLE hIn;
// ----------------------------------------------------------------------------------------------
// Global Variables End
// ----------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------
// Externs Begin
// ----------------------------------------------------------------------------------------------
// for Item Init
extern struct CItem_List Item_Ref;
extern short int nNPC_Type;


int LoadSkillLevelTable(void); // skill.cpp
int initItem(void);
int initNPCTable(void);
int InitEffectTbl();
int InitMagicTbl();
void CleanUpConnectionList(void);

// 010904 LTS
extern bool InitNationSystem();
extern bool InitLocalWarSystem();		// LTS LOCALWAR
extern bool InitEventLocalWarSystem();		// 020115 LTS
extern int InitMapServerConfigINI();		// LTS NEW LOCALWAR
extern int  LoadGeneration();			// LTS AI2
extern int	InitAIData();				// LTS AI2

// 020430 YGI acer 2
extern int InitChrLogDB(int port);

//021011 KYO 
extern int InitLoadQuestTable();		//퀘스트관련 테이블 로드


// ----------------------------------------------------------------------------------------------
// Externs End
// ----------------------------------------------------------------------------------------------

//< LTH-040311-KO 아이템 확률 테스트를 위한 함수들
void SerchItemNPCINV(vector<t_ItemNumber>* pvtItemList, CHARLIST* ch)
{
	int count = 0;
	for (int a = 0; a < 3; a++)
		for (int b = 0; b < 3; b++)
			for (int c = 0; c < 8; c++)
				if (ch->inv[a][b][c].item_no)
				{
					if (!CheckSameItem(pvtItemList, ch->inv[a][b][c].item_no))
					{
						t_ItemNumber stItem;
						stItem.ItemNumber = ch->inv[a][b][c].item_no;
						stItem.nCount = 1;
						pvtItemList->push_back(stItem);
					}
				}
}

int FrogFunc(char* strName, int nMonsterNumber, int nItemNumber, int nKillCount)
{
	int nItemCount = 0;
	NPC_Generation* pNpcTable = &NPC_Gen_Ref[nMonsterNumber];
	CHARLIST* n = &NPCList[0];

	for (int i = 0; nKillCount > i; ++i)
	{
		memset(n, 0, sizeof(CHARLIST));
		n->npc_index = nMonsterNumber;
		n->GenerateNpcName(); // 이름 설정
		n->SprNo = pNpcTable->SprNO;
		n->mutant = pNpcTable->mutant;
		n->homemap = MapNumber;
		n->Class = CREATURE; // 클래스 설정
		n->Race = pNpcTable->nNpcRace; // 종족 설정
		n->SetLevel(pNpcTable->nLvMin + ran(pNpcTable->nLvMax - pNpcTable->nLvMin)); // CSD-030806 : 레벨 설정
		n->Exp = pNpcTable->nEpMax; // 경험치 설정
		n->SetClassStep(pNpcTable->nStep);
		n->SetAttribute(pNpcTable->nNpcRace); // 속성 설정
		n->name_status.nation = (pNpcTable->nNpcNK == 0) ? 2 : MapInfo[MapNumber].nation;
		n->Money = pNpcTable->Money_min + ran(pNpcTable->Money_Max - pNpcTable->Money_min);
		n->SetState(CON_NORMAL);

		SkillMgr.GenerateNPCItem2(n, pNpcTable);

		nItemCount += ::SerchItemNPCINV(n, nItemNumber);
	}

	strcpy(strName, n->Name);

	return nItemCount;

}

void FrogFunc(vector<t_ItemNumber>* pvtItemList, char* strName, int nMonsterNumber, int nKillCount)
{
	pvtItemList->clear();

	NPC_Generation* pNpcTable = &NPC_Gen_Ref[nMonsterNumber];
	CHARLIST* n = &NPCList[0];

	for (int i = 0; nKillCount > i; ++i)
	{
		memset(n, 0, sizeof(CHARLIST));
		n->npc_index = nMonsterNumber;
		n->GenerateNpcName(); // 이름 설정
		n->SprNo = pNpcTable->SprNO;
		n->mutant = pNpcTable->mutant;
		n->homemap = MapNumber;
		n->Class = CREATURE; // 클래스 설정
		n->Race = pNpcTable->nNpcRace; // 종족 설정
		n->SetLevel(pNpcTable->nLvMin + ran(pNpcTable->nLvMax - pNpcTable->nLvMin)); // CSD-030806 : 레벨 설정
		n->Exp = pNpcTable->nEpMax; // 경험치 설정
		n->SetClassStep(pNpcTable->nStep);
		n->SetAttribute(pNpcTable->nNpcRace); // 속성 설정
		n->name_status.nation = (pNpcTable->nNpcNK == 0) ? 2 : MapInfo[MapNumber].nation;
		n->Money = pNpcTable->Money_min + ran(pNpcTable->Money_Max - pNpcTable->Money_min);
		n->SetState(CON_NORMAL);

		SkillMgr.GenerateNPCItem2(n, pNpcTable);
		SerchItemNPCINV(pvtItemList, n);
	}

	strcpy(strName, n->Name);
}

void	CalculateNPCItemInfo()
{
reSelectProc:
	printf("Select Drop ItemProcdure1(Select Item) or ItemProcdure2(All Item) INPUT '1' or '2'(if Input '0' then Exit): ");
	char Select = getch();

	if ('1' != Select && '2' != Select) goto reSelectProc;

	if (Select == '1')
	{
		if (LocalMgr.IsAbleNation(KOREA))
		{
			printf("------------------------몬스터 아이템 드랍 확률계산 프로시져를 시작 합니다(선택된 아이템만)--------------------------아자!\n");
		}
		else
		{
			printf("------------------------Start Moster Item Drop Procedure(Only Selected ItemNumber Count)............................Fighting!!!\n");
		}

	reFrog:
		int nMonsterNumber = 0;
		int nItemNumber = 0;
		int nKillCount = 0;


		if (LocalMgr.IsAbleNation(KOREA))
		{
			printf("정목씨 어떤 몬스터를 확인 할까요?....................\n(몬스터 번호를 입력 하세요) 번호 : "); scanf("%d", &nMonsterNumber);
			printf("\n");
			printf("정목씨 몬스터의 어떤 아이템을 확인할까요?............\n(아이템 번호를 입력 하세요) 번호 : "); scanf("%d", &nItemNumber);
			printf("\n");
			printf("정목씨 몬스터를 몇번이나 죽일까요? ..................\n(죽일 횟수를 입력하세요) 횟수 : "); scanf("%d", &nKillCount);
			printf("\n");
		}
		else
		{
			printf("(Input Monster Number) INPUT : "); scanf("%d", &nMonsterNumber);
			printf("\n");
			printf("(Input Item Number) INPUT : "); scanf("%d", &nItemNumber);
			printf("\n");
			printf("(Input Kill Count) INPUT : "); scanf("%d", &nKillCount);
			printf("\n");
		}

		char strName[20] = { NULL, };

		int ret = FrogFunc(strName, nMonsterNumber, nItemNumber, nKillCount);

		if (ret < 0)//  에러.
		{
			//printf("그런 몬스터 없어요...ㅡㅡ;\n");
			//printf("그런 아이템 들고 있지도 않고만...ㅡㅡ^\n");
		}
		else
		{
			if (LocalMgr.IsAbleNation(KOREA))
			{

				printf("******************************************결과 입니다***************************************\n");
				printf("몬스터 : '%s'\n죽인 횟수 : %d 번\n나온 <%d>아이템의 갯수 : %d 개\n", strName, nKillCount, nItemNumber, ret);
				printf("정목씨 아무키나 누르시면 종료 됩니다...............(0 을누르면 재시작.)\n");
			}
			else
			{
				printf("****************************************** RESULT ***************************************\n");
				printf("Monster : '%s'\nKill Count : %d \n Dropped <%d>Item Count : %d \n", strName, nKillCount, nItemNumber, ret);
				printf("Press Any Key to End...............(If You Input '0' then ReStart Procdure)\n");
			}

			char Input = getch();
			if (Input == '0') goto reFrog;
		}

		//	cout << "
	}
	else// (All item)
	{
	reFrog2:

		int nMonsterNumber = 0;
		int nItemNumber = 0;
		int nKillCount = 0;

		if (LocalMgr.IsAbleNation(KOREA))
		{
			printf("------------------------몬스터 아이템 드랍 확률계산 프로시져를 시작 합니다(모든 아이템)--------------------------아자!\n");
		}
		else
		{
			printf("------------------------Start Moster Item Drop Procedure(All Item)............................Fighting!!!\n");
		}

		if (LocalMgr.IsAbleNation(KOREA))
		{
			printf("정목씨 어떤 몬스터를 확인 할까요?....................\n(몬스터 번호를 입력 하세요) 번호 : "); scanf("%d", &nMonsterNumber);
			printf("\n");
			printf("정목씨 몬스터를 몇번이나 죽일까요? ..................\n(죽일 횟수를 입력하세요) 횟수 : "); scanf("%d", &nKillCount);
			printf("\n");
		}
		else
		{
			printf("(Input Monster Number) INPUT : "); scanf("%d", &nMonsterNumber);
			printf("\n");
			printf("(Input Kill Count) INPUT : "); scanf("%d", &nKillCount);
			printf("\n");
		}

		char strName[20] = { NULL, };
		vector<t_ItemNumber> vtItemList;
		FrogFunc(&vtItemList, strName, nMonsterNumber, nKillCount);

		int nItemKindCount = vtItemList.size();
		if (LocalMgr.IsAbleNation(KOREA))
		{

			printf("******************************************결과 입니다***************************************\n");
			printf("몬스터 : '%s'\n죽인 횟수 : %d 번\n", strName, nKillCount);
			if (nItemKindCount)
			{
				for (int i = 0; i < nItemKindCount; ++i)
				{
					printf("Item 번호 : %5d, 나온갯수 : %3d\n", vtItemList[i].ItemNumber, vtItemList[i].nCount);
				}
				printf("끝...\n");
			}
			else
			{
				printf("어떤 아이템도 나오질 않았어요. 몬스터나...아이템관련 된 테이블을 살펴보세요. ㅡ.ㅡ;\n");
			}
			printf("정목씨 아무키나 누르시면 종료 됩니다...............(0 을누르면 재시작.)\n");
		}
		else
		{
			printf("****************************************** RESULT ***************************************\n");
			printf("Monster : '%s'\nKill Count : %d \n", strName, nKillCount);

			if (nItemKindCount)
			{
				for (int i = 0; i < nItemKindCount; ++i)
				{
					printf("Item Number : %5d, Count : %3d\n", vtItemList[i].ItemNumber, vtItemList[i].nCount);
				}
				printf("End...\n");
			}
			else
			{
				printf("Nothing!!!!  See.. Moster Table or Item Tables ㅡ.ㅡ;\n");
			}

		}
		printf("Press Any Key to End...............(If You Input '0' then ReStart Procdure)\n");

		char Input = getch();
		if (Input == '0') goto reFrog2;

	}
}
//> LTH-040311-KO

// ----------------------------------------------------------------------------------------------
// Functions Begin
// ----------------------------------------------------------------------------------------------
int InitialScript(void)
{
	int ret;

	InitTextScript();
	ret = LoadScript(MapName);

	if (!ret)
	{
		MyLog(LOG_NORMAL, " *** Error : Script Syntax ");
	}

	Script(0);
	MyLog(LOG_NORMAL, "   Script(0) Executed !");

	return ret;
};
// ----------------------------------------------------------------------------------------------
void RecvBBSLogin(char *msg)
{
	strcpy(bbsString, msg);

	BBSBroadcast = 30;
}
// ----------------------------------------------------------------------------------------------
void StartingBBS(void)
{
	static DWORD time;
	char temp[MAX_PATH];
	char *s;

	FILE *fp;

	wsprintf(temp, "%s/data/드래곤라자공지사항.txt", GameServerDataPath);

	fp = fopen(temp, "rt");
	if (fp == NULL) return;

	fgets(temp, MAX_PATH - 1, fp);
	s = EatRearWhiteChar(temp);
	strcpy(bbsString, s);
	fclose(fp);

	BBSBroadcast = 30;
}

void BroadCastBBS_Sub(char *msg, int len)
{	//< CSD-031213
	if (msg == NULL)
	{
		return;
	}

	if (len == 0)
	{
		return;
	}

	t_packet p;
	p.h.header.type = CMD_BBS;
	p.h.header.size = sizeof(t_server_bbs) - MAX_PATH + len;
	strcpy(p.u.server_bbs.bbs, msg);
	g_pUserManager->SendPacket(&p);
}	//> CSD-031213

void RecvTotalMapConnections(int cn)
{
	t_packet p;
	int no;
	no = PC_COUNT;
	if (no < 0) no = 0;
	p.h.header.type = CMD_TOTAL_MAP_CONNECTIONS;
	p.u.total_map_connections.no = no;
	p.h.header.size = sizeof(t_total_map_connections);
	QueuePacket(connections, cn, &p, 1);
}

void RecvTotalConnections(int cn)
{
	int no = TotalConnections();

	if (no < 0)
	{
		no = 0;
	}

	t_packet p;
	p.h.header.type = CMD_TOTAL_CONNECTIONS;
	p.h.header.size = sizeof(t_total_connections);
	p.u.total_connections.no = no;
	QueuePacket(connections, cn, &p, 1);
}

void RecvAbsLogOut2GameServer(char* id)
{	//< CSD-HK-030829
	for (int i = DRAGON_CONNECTIONS_START; i < DRAGON_MAX_CONNECTIONS; ++i)
	{
		if (connections[i].dwAgentConnectionIndex)
		{
			if (strcmp(connections[i].id, id) == 0)
			{
				closeconnection(connections, i, -203);
				break;
			}
		}
	}
}	//> CSD-HK-030829

void prepare(t_connection c[])
{
	global_time = ::timeGetTime();

	time_t lTime = { 0, };
	time(&lTime);

	struct tm *today = localtime(&lTime);

	g_year = today->tm_year + 1900;
	g_mon = today->tm_mon;
	g_yday = today->tm_yday;
	g_wday = today->tm_wday;
	g_day = today->tm_mday;
	g_hour = today->tm_hour;
	g_min = today->tm_min;
	g_sec = today->tm_sec;

	// 031009 CI YGI
	g_curr_time_with_out_year =
		(g_yday * 86400)
		+ (g_hour * 3600)
		+ (g_min * 60)
		+ g_sec;

	g_curr_time = ((g_year - 1999) * 31536000) + g_curr_time_with_out_year;
}

// ----------------------------------------------------------------------------------------------
// 020808 YGI 
struct ID_PASS
{
	char m_szID1[30];
	char m_szPASS1[30];
	char m_szID2[30];
	char m_szPASS2[30];
	char m_szID3[30];
	char m_szPASS3[30];

	ID_PASS()
	{
		m_szID1[0] = 0;
		m_szPASS1[0] = 0;
		m_szID2[0] = 0;
		m_szPASS2[0] = 0;
		m_szID3[0] = 0;
		m_szPASS3[0] = 0;
	}
};

////////////////////////////////////////////////////////////////////////////
// YGI-040510
bool DecodeFile(char *filename, void *pData, int size) // YGI 040503
{
	CHSEL_STREAM m_hsel;
	FILE *fp = fopen(filename, "rb");
	if (!fp) return false;

	int nVersion = 0;
	fread((void*)(&nVersion), sizeof(int), 1, fp);
	if (m_hsel.GetVersion() != nVersion)  return false;
	HselInit deinit;
	fread((void*)(&deinit), sizeof(HselInit), 1, fp);
	if (!m_hsel.Initial(deinit))  return false;
	fread(pData, 1, size, fp);
	m_hsel.Decrypt((char *)pData, size);
	fclose(fp);
	return true;
}
bool EncodeFile(char *filename, void *pData, int size)
{
	CHSEL_STREAM m_hsel;
	FILE *fp = fopen(filename, "wb");
	if (!fp) return false;

	HselInit eninit;
	eninit.iEncryptType = HSEL_ENCRYPTTYPE_RAND;
	eninit.iDesCount = HSEL_DES_TRIPLE;
	eninit.iCustomize = HSEL_KEY_TYPE_DEFAULT;
	eninit.iSwapFlag = HSEL_SWAP_FLAG_ON;

	if (!m_hsel.Initial(eninit))
	{
		return false;
	}

	HselInit deinit;
	deinit = m_hsel.GetHSELCustomizeOption();
	const int nVersion = m_hsel.GetVersion();
	fwrite((void *)(&nVersion), sizeof(nVersion), 1, fp);
	fwrite((void *)(&deinit), sizeof(HselInit), 1, fp);
	m_hsel.Encrypt((char*)pData, size);
	fwrite(pData, 1, size, fp);
	fclose(fp);
	return true;
}

// YGI-040510
// key값으로 XOR 한 후에 함호화 한다.
bool EncodeByKey(WORD key, char *filename, void *pData, int size)
{
	// 실제 데이타는 변환 시키지 않기 위해
	void *data;
	data = new char[size];
	memcpy(data, pData, size);
	int max_count = size / sizeof(WORD);
	for (int i = 0; i < max_count; i++)
	{
		*((WORD *)data + i) ^= key;
	}
	bool ret = EncodeFile(filename, data, size);
	delete[] data;
	return ret;
}

// 함호를 푼 후에 key로 XOR 한다.
bool DecodeByKey(WORD key, char *filename, void *pData, int size)
{
	if (DecodeFile(filename, pData, size))
	{
		int max_count = size / sizeof(WORD);
		for (int i = 0; i < max_count; i++)
		{
			*((WORD *)pData + i) ^= key;
		}
		return true;
	}
	else
		return false;
}


//////////////////////////////////////////////////////////////////

void MakeServerNeedFolder()
{	//< CSD-TW-030622
	::mkdir("./Output");
}	//> CSD-TW-030622

extern int InitMapInfo(t_MapInfo T[]); // CSD-030516
// 020808 YGI 주의사항 참조
int InitDRMapServerDatas(void)
{
	srand((unsigned)time(NULL));

	MakeServerNeedFolder();

	prepare(connections);		//021030 YGI

	if (InitGameMakeModeSetting(MAP_SERVER_INI_) < 0)
	{
		MyLog(LOG_FATAL, "File '%s' not FOUND", MAP_SERVER_INI_);
		return (0);
	}


	char path[MAX_PATH];
	ID_PASS id_password;
	int bIdPassword = 0;		// 정확히 암호를 읽어 왔는가?
	{
		sprintf(path, "%s/data/IdPassword.bin", GameServerDataPath);
		bIdPassword = DecodeFile(path, (char *)&id_password, sizeof(ID_PASS));	// YGI 040510
	}

	ID_PASS IdPassWord;
	::strcpy(IdPassWord.m_szID1, LocalMgr.GetDBAccount(TOTAL_DB, ID));//030102 lsw
	::strcpy(IdPassWord.m_szPASS1, LocalMgr.GetDBAccount(TOTAL_DB, PASS));//030102 lsw
	::strcpy(IdPassWord.m_szID2, LocalMgr.GetDBAccount(DRAGON_DB, ID));
	::strcpy(IdPassWord.m_szPASS2, LocalMgr.GetDBAccount(DRAGON_DB, PASS));

	if (bIdPassword)
		IdPassWord = id_password;

	//050224_KCH 마일리지몰 작업
	if ((Init_TotalDB_SQL("totaldb", IdPassWord.m_szID1, IdPassWord.m_szPASS1)) == 0)
	{
		MyLog(LOG_FATAL, "totaldb SQL server connect fail !");
		return(0);
	}
	MyLog(LOG_NORMAL, "totaldb SQL server connect OK!");

	if ((Init_DragonRajaDB_SQL("DragonRajaDB", IdPassWord.m_szID2, IdPassWord.m_szPASS2)) == 0)
	{
		MyLog(LOG_FATAL, "DragonRajaDB SQL server connect fail !");
		return(0);
	}
	MyLog(LOG_NORMAL, "DragonRajaDB SQL server connect OK!");

	// 020430 YGI acer 2
	if (!InitChrLogDB(GetManagementMapPort(MM_SAVE_LOG_FILE_MAP))) return 0;
	//< CSD-031229
	/*
	// 031009 CI YGI
	if( !InitItemLimitCountFromDB() )		// 리미트 번호 초기화가 실패 했다.
	{
		JustMsg( "Error!!! Check please ItemLimitMgrTable table" );
		return 0;
	}
	*/
	//> CSD-031229
	InitNPCList();
	if (initNPCTable() < 0) { Error("  NPC Table Loading Failed.\n"); }
	else
	{
		MyLog(LOG_NORMAL, "'NPC_NamebyGender' Table initializing  ..........  OK !");
		MyLog(LOG_NORMAL, "'NPC_Lv2Exp'       Table initializing  ..........  OK !");
		MyLog(LOG_NORMAL, "'NPC_Generation_SP'Table initializing  ..........  OK !\n");
	}

	g_MapPort = GetOwnPort(); // CSD-030506	
	InitMoveDelayTable();

	// 0527 YGI 욕설필터
	MyLog(LOG_NORMAL, "  Reading './Data/BadWord.TXT'      count = %d ", LoadFilterWord());

	InitItemList();

	if (initItem() < 0) { Error("  Item loading Fail !\n");	return(0); }
	else MyLog(LOG_NORMAL, "Item Table initializing    ...................... OK !\n");

	MakeStoreList();
	MyLog(LOG_NORMAL, "Store Item List  initializing    ................ OK !\n");

	if (InitSkillTable() < 0) { Error("  'skillmain' loading Fail !\n");	return(0); }
	MyLog(LOG_NORMAL, "'SkillMain' Table initializing   ................ OK !");

	if (InitGmQuest() < 0) { Error("  'GMquest' loading Fail !\n");	return(0); }
	else MyLog(LOG_NORMAL, "'GMquest' Table initializing    ................. OK !");

	if (InitMapInfo(MapInfo) < 0) { Error(" MapInfo Loading Fail !! \n"); return(0); }
	else	MyLog(LOG_NORMAL, "'Map_Info' Table loading        ................. OK !");
	if (LoadArenaInfo())
	{	//< CSD-030517
		if (LoadArenaGameInfo())
		{
			MyLog(LOG_NORMAL, "'arean_game_info' Table loading        ........... OK !");
		}

		if (LoadArenaTeamInfo())
		{
			MyLog(LOG_NORMAL, "'arean_Team_info' Table loading        ........... OK !");
		}

		if (LoadArenaBonusInfo())
		{
			MyLog(LOG_NORMAL, "'arean_bonus_info' Table loading       ........... OK !");
		}
	}	//> CSD-030517

	if (g_AliveMap.LoadAliveMapZone() < 0)	Error("  AliveMapZone Table loading Fail !\n");	// 010502 YGI
	else	MyLog(LOG_NORMAL, "'alive_zone' Table loading        ..............  OK !");

	// 010522 YGI
	if (LoadNationInfo(NationInfo) != 1)	Error("  Nation_Info Table loading fail!!!!! \n");
	else	MyLog(LOG_NORMAL, "'Nation_Info' Table loading         ............  OK !");

	if (LoadGuildListForMapServer() != 1) { Error("  Guild_List Table loading fail!!!!! ... new! "); return(0); }
	else	MyLog(LOG_NORMAL, "'Guild_List' Table loading         ............  OK");


	// 011130 YGI
	extern void LoadDRAGON_MAX_CONNECTIONS();
	LoadDRAGON_MAX_CONNECTIONS();
	if (!DRAGON_MAX_CONNECTIONS) DRAGON_MAX_CONNECTIONS = DRAGON_MAX_CONNECTIONS_;



	//< CSD-011126
	if (InitEffectTbl() < 0)
	{
		Error("  Effect loading Fail ! ... new! \n");
		return(0);
	}
	else
	{
		MyLog(LOG_NORMAL, "'Effect' Table initializing     ...............   OK !");
	}

	if (InitMagicTbl() < 0)
	{
		Error("  Magic loading Fail ! ... new! \n");
		return(0);
	}
	else
	{
		MyLog(LOG_NORMAL, "'Magic' Table initializing	     ...............   OK !");
	}
	//> CSD-011126
	if (InitSkillMapTable() < 0) { Error("  SkillMapTable loading Fail !");	return(0); }
	else MyLog(LOG_NORMAL, "'SkillMapTable' initializing   ................   OK !");

	if (InitialScript() <= 0) { Error(" Script Loading Error ! ");	return(0); }

	// 부수면 나오는 Item에 관한 Table..
	if (LoadItemsInBoxTableSQL() < 0) { Error("  'ItemsBox_new' Table loading Fail !");	return(0); }
	else MyLog(LOG_NORMAL, " 'ItemsBox_new' Loading  ......................   OK !");



	MapBuild(&g_Map, MapName);
	MyLog(LOG_NORMAL, "<%11s.map>  loading                        OK !", MapName);

	LoadTOI(MapName);
	MyLog(LOG_NORMAL, "<%11s.toi2> loading                        OK !", MapName);

	LoadSkillMapTable();
	MyLog(LOG_NORMAL, "<%11s.skb>  loading                        OK !", MapName);

	if (!LoadGeneration()) MyLog(LOG_NORMAL, "****** FAILED :: Event_Monster Table loading, Not Terminate......");		// LTS AI2
	else MyLog(LOG_NORMAL, "Event_Monster Table load Complete.........    OK !");


	if (LoadItemJoinTable()) MyLog(LOG_NORMAL, "'ItemJoin' Table initializing  ................   OK !");
	else { Error("******* FAILED !! :: Item Join Table initializing"); return 0; }

	if (!LoadWeatherTable()) MyLog(LOG_NORMAL, "WeatherTable(data/rain.tbl) initializing   ....   OK !");
	else { Error("***** FAILED :: Weather Table  initializing"); return 0; }

	//this2 lsw soksung 
	//011025 lsw >
	if (LoadItemTable())
	{
		MyLog(LOG_NORMAL, "Item Attr. initializing  ");
		MyLog(LOG_NORMAL,
			"     (Item_mutant/ Item_grade/ Item_mutant_kind ) OK !");
	}
	else { Error("******* FAILED !! :: Item mutant, rare Table  initializing"); return 0; }
	//011025 lsw <

	int t = LoadSkillLevelTable();
	if (t > 0) MyLog(LOG_NORMAL, " skill_lvexp/ skill_exp/ mon_bag Table Loading    OK !");
	else { Error("******* FAILED !! :: skill_lvexp/ skill_exp / mon_bag ( %d )", t); return 0; }

	if (LoadGenerateSpecialItem() < 0) { Error("  Special_Item initializing Fail !\n");	return(0); }
	else MyLog(LOG_NORMAL, "'Special_Item' Table initializing .............   OK !");

	if (LoadAbilityLimit())
	{
		MyLog(LOG_NORMAL, "'ability_limit' Table initializing .............   OK !");
	}
	else
	{
		Error("  ability_max initializing Fail!\n");
		return 0;
	}

	if (LoadAbilityDivide())
	{
		MyLog(LOG_NORMAL, "'ability_divide' Table initializing ............   OK !");
	}
	else
	{
		Error("  ability_divide initializing Fail!\n");
		return 0;
	}

	if (LoadDualInfo())
	{
		MyLog(LOG_NORMAL, "'dual_info' Table initializing .................   OK !");
	}
	else
	{
		Error("  dual_info initializing Fail!\n");
		return 0;
	}

	if (InitGameItem() == 1)
		MyLog(LOG_NORMAL, "'gamble_item' Table initializing ...............   OK !");
	else
	{
		Error("  gamble_item initializing Fail!\n");
		return 0;
	}

	LoadNationItem(g_wday);

	SaveMoveDelayTable();// 1타일 이동하는데 걸리는시간을 Text화일로 출력한다. ( 기획요구 )
	LoadConditionTable();

	if (!InitMapServerConfigINI())	// LTS NEW LOCALWAR
	{
		Error("******* FAILED !! :: Load MapServerConfig.ini\n");
		return 0;
	}	// LTS LOCALWAR

	if (!InitNationSystem()) // 010904 LTS
	{
		Error("******* FAILED !! :: Nation System Initializing Fault!\n");
		return 0;
	}

	if (!InitLocalWarSystem())		// LTS LOCALWAR
	{
		Error("******* FAILED !! :: LocalWar System Initializing Fault!\n");
		return 0;
	}

	if (!InitEventLocalWarSystem())		// 020115 LTS
	{
		Error("******* FAILED !! :: Event LocalWar System Initializing Fault!\n");
		return 0;
	}

	if (!InitAIData())		// LTS AI2
	{
		Error("******* FAILED !! :: AI Data Initializing Fault!\n");
		return 0;
	}

	if (LoadWarStartInfo())
	{	//< CSD-030723
		MyLog(LOG_NORMAL, "'WarStartTBL' Table initializing ...............   OK !");
	}	//> CSD-030723

	InitRefreshMenu();		// 020620 YGI
	InitNationRelation();	// 001209 KHS 국가관계에 대한 초기화.
	LoadGetGodTable();		// 0605 YGI	신전 테이블 가져오기
	LoadGameServerStatus();
	CheckingChrLogAvailable();	// 010301 KHS

	InitOnlyStart_kein();		// 020818 YGI


	int tflag = InitLoadQuestTable(); // 021106 kyo
	if (tflag == 1)
	{
		MyLog(LOG_NORMAL, "'quest_in_map'	Table initializing ..............   OK ! ");
		MyLog(LOG_NORMAL, "'quest_info_map' Table initializing ..............   OK ! ");
		MyLog(LOG_NORMAL, "'requital_list'  Table initializing ..............   OK ! ");
		MyLog(LOG_NORMAL, "'requital_item'  Table initializing ..............   OK ! ");	// BBD 040329
	}
	else
	{
		if (tflag == -1)
			Error("******* FAILED !! 'quest_in_map' QuestTable!\n");
		else if (tflag == -2)
			Error("******* FAILED !! 'quest_info_map' QuestTable!\n");
		else if (tflag == -3)
			Error("******* FAILED !! 'requital_list' QuestTable!\n");
		//<! BBD 040329		
		else if (tflag == -4)
			Error("******* FAILED !! 'requital_item' QuestTable!\n");
		//> BBD 040329		

		return 0;
	}

	//< soto-030331 지킴이 테이블 로드.
	int nRet = 0;
	if (nRet = LoadGuardianGuildList())
	{
		MyLog(LOG_NORMAL, "GuardianGuildList Table initializing ..............   OK ! ");
	}
	else
	{
		Error("******* FAILED !!  GuardianGuildList Table initializing!");
	}
	//> soto-030331
	if (LoadHuntInfo())
	{	//< CSD-030509
		if (LoadHuntGroupInfo())
		{
			LoadHuntMonsterInfo();
			LoadHuntPlaceInfo();
		}
	}	//> CSD-030509

	if (LocalMgr.IsAbleNation(THAI))// 030929 kyo
	{
		::InitLimitedTimeAndAge();
	}

	tflag = GetPrivateProfileInt("Option", "DisconnectAccelatorUser", 0, MAP_SERVER_INI_);
	if (tflag)
	{
		g_accelator_user_closeconnection = true;
		MyLog(LOG_NORMAL, "  ** Accelator Users will be disconnected and logged in!");
	}
	else
	{
		MyLog(LOG_NORMAL, "  ** Accelator Users will be ** NOT ** disconnected and logged in!");
	}

	g_alive_time = timeGetTime();
	MyLog(LOG_NORMAL, "All of 'GameServerData' Loaded, successfuly ............^ ^\n\n");

	MyLog(LOG_NORMAL, " F1 : Server Status");
	MyLog(LOG_NORMAL, " F5 : Re-Load a part of 'GameServerData'");
	MyLog(LOG_NORMAL, "ESC : Exit");

	return(1);
}



// 020430 acer 2
/////////////////////////////////////////////////////////////////////////////
extern void InitItemLog(int port);
int InitChrLogDB(int port)
{
	if (GetOwnPort() == port)		// 담당 서버 인가?
	{
		if (!Init_ChrLogDB_SQL("ChrLogDB",
			LocalMgr.GetDBAccount(CHRLOG_DB, ID),
			LocalMgr.GetDBAccount(CHRLOG_DB, PASS)))
		{
			MyLog(LOG_FATAL, "'ChrLogDB' Table Initializing Fail !");
			JustMsg("'ChrLogDB' Table Initializing Fail !");
			return(-1);

		}
		MyLog(LOG_NORMAL, "SQL server( ChrLogDB ) connect OK!");
	}
	// item_log 를 위한 chrlogdb odbc 설정
	InitItemLog(port);
	//MyLog( LOG_FATAL, "Error! init ChrLogDB : This Surver port = [%d], ManagementServer port = [%d]", GetOwnPort(), port );
	return(1);
}
/////////////////////////////////////////////////////////////////////////////



// Rewrote by chan78 at 2000/11/27
// 020620 YGI 
void SaveAllUserDatas(void)
{
	static int ct = 0;
	DWORD counter = 0;

	MyLog(LOG_IMPORTANT, "Saving Player Datas on DATABASE");
	int i = DRAGON_CONNECTIONS_START;
	for (; i < DRAGON_MAX_CONNECTIONS; i++)
	{
		if (i && !(i % ((DRAGON_MAX_CONNECTIONS / 5) ? (DRAGON_MAX_CONNECTIONS / 5) : 1)))
		{
			MyLog(0, "-- Now %d connections are saved (%d/%d)", counter, i, DRAGON_MAX_CONNECTIONS);
		}
		if (connections[i].dwAgentConnectionIndex && connections[i].state >= CONNECT_JOIN)
		{
			int ret1 = updateCharacterStatus(connections, i);
			int ret2 = UpdateCharStatusByKein(connections, i);		// 0410 YGI	// 추가 캐릭터 정보 저장
			if (ret1 != 1 || ret2 != 1)
			{
				ct++;
				CHARLIST *ch = &connections[i].chrlst;
				MyLog(0, "%03d - Name(%-20s): ERROR_CODE[%d/%d], ID(%d), ServerId(%d), EXP(%u)\n", ct, ch->Name, ret1, ret2, i, ch->GetServerID(), ch->Exp);
			}
			else counter++;
		}
	}
	MyLog(0, "-- Total %d connections are saved (%d:%d)", counter, i, DRAGON_MAX_CONNECTIONS);
	//SaveGameServerStatus();
};


void OnDestroy()
{
	EndMapServer();
}
// ----------------------------------------------------------------------------------------------
// Functions End
// ----------------------------------------------------------------------------------------------
#include "GameFactory.h"
// ----------------------------------------------------------------------------------------------
// Main()
// ----------------------------------------------------------------------------------------------
int orgmain();

int main() {
	int retvalue = 0;
	EXCEPTION_POINTERS* pException = NULL;//020508 lsw
	__try {
		retvalue = orgmain();
	}
	//__except(pException = GetExceptionInformation())//020508 lsw
	__except (DumpException(GetExceptionInformation(), "Exception Raised on main()"))//020508 lsw
	{
		//DumpException( pException, "Exception Raised on main()" );
	}
	return retvalue;
}

int orgmain()
//int main()
{	//< CSD-HK-030829

	CGameFactory gameFactory;
	CGameSystem gameSystem(&gameFactory);	//050224_KCH 마일리지몰 작업
//	gameSystem.SetFactory(&gameFactory);
	gameSystem.CreateManager(LogManager);
	gameSystem.CreateManager(UserManager);
	gameSystem.CreateManager(DualManager);
	gameSystem.CreateManager(GuildManager);
	gameSystem.CreateManager(AIManager);
	gameSystem.CreateManager(AdventManager);
	gameSystem.CreateManager(ArenaManager);
	gameSystem.CreateManager(RegenManager);
	gameSystem.CreateManager(BattleManager);
	gameSystem.CreateManager(RaidManager);
	//> CSD-HK-030829

	DWORD dwResult;
	bool bServerRunning = false;

	INPUT_RECORD	irBuffer;
	memset(&irBuffer, 0, sizeof(INPUT_RECORD));

	AllocConsole();
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	g_hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	//SetMonitorSize( SCREEN_TEXT_SIZE_X, SCREEN_TEXT_SIZE_Y );

	// Initialize LogManager
	InitMyLog();

	// 020620 YGI 
	extern void SetMenuFunctionsAll();
	SetMenuFunctionsAll();

	// Init Game Timers
	SettingGameTimers();

	// Init Lower Layer.
	if (!InitMapServer())
		goto FinishMapServer;

	// MAP Server Datas loading
	if (!InitDRMapServerDatas())
		goto FinishMapServer;

	// Clear Connection List
	CleanUpConnectionList();

	//<050224_KCH 마일리지몰 작업 특정매니저맵에서만 생성해서 관리하고자 한다.
	if (nITEM_MALL_MANAGER_PORT == g_pServerTable->GetOwnServerData()->wPort)
	{
		//맵서버의 포트정보 확인때문에, 여기서 처리
		gameSystem.CreateManager(ItemMallManager);
		MyLog(LOG_IMPORTANT, "*************************************************");
		MyLog(LOG_IMPORTANT, "********** ItemMallManager_050407_KCH  **********");
		MyLog(LOG_IMPORTANT, "*************************************************");
	}

	if (!LoadItemMallItemKind())
	{
		MessageBox(NULL, "ItemMall: 'LoadItemMallItemKind()' Fail !", _T("LoadItemMallItemKind"), MB_OK); return(-2);
	}
	//>050224_KCH

	CLottoSystem::Create();//soto-030505

	g_CSymbolMgr.Init(g_hDBC_DragonDB, g_hDBC_ChrLogDB);//soto-030512
	g_CSymbolMgr.LoadTable();//soto-030512

	CLottoSystem::Create();//soto-030505
	if (LottoSystem())
		LottoSystem()->LoadTable(g_hDBC_DragonDB);
	//<soto-030711
	CEventTreasureBoxMgr::Create();
	if (TreasureBoxMgr())
	{
		::TreasureBoxMgr()->LoadTable(g_hDBC_DragonDB);
	}
	//>soto-030711

	g_cpPrison = new CPrison();			// LTH-040723-KO 1.04p 감옥 시스템

	// Try to connect with PROXY
	g_pINet->ResumeTimer(0);
	// Now Start GameTimers
	g_pINet->ResumeTimer(1);
	// Now Start GhostChecker
	g_pINet->ResumeTimer(2);


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
					SERVER_DATA *pData;
					int			port = 0;
					if (g_pServerTable)
					{
						pData = g_pServerTable->GetOwnServerData();
						port = pData->wPort;
					}

					char temp[FILENAME_MAX];
					sprintf(temp, "Are you sure to DESTROY this map server( '%s': %d )?", MapName, port);
					if (MessageBox(NULL, temp, "IMPORTANT", MB_YESNO) == IDYES)
					{
						MyLog(LOG_IMPORTANT, "************ MapServer is closing (%d) **********", port);
						g_pServerTable->DestroyServer(FINISH_TYPE_NORMAL);
						bServerRunning = false;
					}
				}break;
				case VK_F1:
				{
					SetEvent(hKeyEvent[0]);
				}break;
				case VK_F5:
				{

					if (::MessageBox(NULL, "Re-Load 'GameServerData' (It will take several seconds) \n "
						"This is Call 'ReLoadGameServerDataByKeyInput()' Fuction "
						, "Re-Load GameServerData", MB_YESNO) == IDYES)
					{
						SetEvent(hKeyEvent[1]);
					}
				}break;
				case VK_F6:
				{
					SetEvent(hKeyEvent[2]);
				}break;
				case VK_F7:
				{
					SetEvent(hKeyEvent[3]);
				}break;
				case VK_F12:
				{
					CalculateNPCItemInfo();
					printf("프로시져가 종료 되었습니다.\n");
				}break;
				default:
					break;
				}
			}
		}
	}

FinishMapServer:
	MyLog(LOG_NORMAL, "-- MapServer ShutDown :: EndTextScript");	//050113_KCH_불안정한MapDown에대한 로그및 추적
	EndTextScript();//020314 lsw

	MyLog(LOG_NORMAL, "-- MapServer ShutDown :: Now Release SQL");
	Release_SQL();

	MyLog(LOG_NORMAL, "-- MapServer ShutDown :: Now Release INetwork Module, ServerTable");
	OnDestroy();

	MyLog(LOG_NORMAL, "-- MapServer Shutdown :: Now Release Console and Log Resources");
	FreeConsole();
	FreeMyLog();

	CLottoSystem::Destroy();//soto-030505
	CEventTreasureBoxMgr::Destory();//soto-030711
	SAFE_DELETE(g_cpPrison);		// LTH-040804-KO
	return 0;
}
