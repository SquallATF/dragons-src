#include "..\stdafx.h"
#include "..\LowerLayers\mylog.h"
#include "..\LowerLayers\servertable.h"

#include "Map.h"
#include "MenuDefine.h"
#include "Chatting.h"
#include "Scrp_int.h"
#include "quest_cpp.h"
#include "OP_magic.h" 
#include "citem.h"
#include "SealStoneGameServer.h"
#include "Scrp_exe.h"
#include "NationSys.h"	// 010904 LTS
#include "teambattle.h"
#include "LTSSupport.h"
#include "QuestFunction.h"	// 021015 kyo
#include "eventmgr.h"
#include "SymbolItemMgr.h"	// 030419 kyo
#include "ArenaManager.h"
#include "BattleManager.h"
#include "UserManager.h"
#include "KickOffAll.h"		// BBD 040329

#include "LogManager.h"			//040719_KJHuNs g_pLogManager를 사용하기 위함.

int var[DRAGON_MAX_CONNECTIONS_][_SCRIPT_VARABLE_POINT_] = { 0, };
char String[MAX_STRING_] = { 0, };

int EventPC, EventNPC[DRAGON_MAX_CONNECTIONS_] = { 0, };

extern void UpdateCharacterData(t_connection c[], int cn, bool bDirect = false);
extern void SendCMD_MAGIC_POSITION(t_server_magic_position *p);
extern void SendCharInfoSkill(t_connection c[], int cn);
extern void makemyjobexpsetting(int skillno, CHARLIST *ch);
extern void SendLearnSkillOk(int skill_no, t_connection c[], int cn);
extern void RecvCMD_CLIENT_CHARACTER_DEMAND(int cn, t_client_character_demand *p);

extern CKickOffAll g_KickMgr;	// BBD 040329

void LogInFile(string szFilePath, char *szFormat, ...) //kyo
{
	FILE *fp;
	if (!(fp = fopen(szFilePath.c_str(), "a+")))
	{
		return;
	}

	char szMessage[512];//
	va_list val;
	va_start(val, szFormat);
	vsprintf(szMessage, szFormat, val);
	va_end(val);

	time_t nowTime;
	struct tm *now;
	time(&nowTime);
	now = localtime(&nowTime);
	int year = 0, mon = 0, day = 0;
	int hour = 0, min = 0, sec = 0;
	// Make it usable.

	year = now->tm_year + 1900;
	mon = now->tm_mon + 1;
	day = now->tm_mday;
	hour = now->tm_hour;
	min = now->tm_min;
	sec = now->tm_sec;
	char szLog[512];
	sprintf(szLog, "%d %d/%d %d:%d:%d\t%s\n", year, mon, day, hour, min, sec, szMessage);

	if (!fputs(szLog, fp))
	{
		fclose(fp);
		return;
	}

	fclose(fp);
}

void ReCallScript(void)
{	//< CSD-CN-031213
	CUserManager::HASH_USER mpUser = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = mpUser.begin(); i != mpUser.end(); ++i)
	{
		const int idUser = i->second;

		if (!var[idUser][1])
		{
			continue;
		}

		CHARLIST* pUser = CheckServerId(idUser);

		if (pUser != NULL)
		{
			if (var[idUser][3] < g_curr_time)
			{
				EventPC = idUser;
				Script(var[idUser][1]);
			}
		}
	}
}	//> CSD-CN-031213

int SearchItemByInv(int item_no, CHARLIST *ch)
{

	for (int a = 0; a < 3; a++) {
		for (int b = 0; b < 3; b++) {
			for (int c = 0; c < 8; c++) {
				if (ch->inv[a][b][c].item_no == item_no)
				{
					return 1;
				}
			} // for
		} // for
	}  // for

	return 0;
}

int SearchItemByInv(int item_no, CHARLIST *ch, int &a, int &b, int &c)
{
	for (a = 0; a < 3; a++) {
		for (b = 0; b < 3; b++) {
			for (c = 0; c < 8; c++) {
				if (ch->inv[a][b][c].item_no == item_no)
				{
					return 1;
				}
			} // for
		} // for
	}  // for

	return 0;
}


int SearchItemByEquip(int item_no, CHARLIST *ch)
{
	for (int a = 0; a < 8; a++)
	{
		if (ch->equip[a].item_no == item_no) return 1;
	}

	return 0;
}

int SearchItemByQuick(int item_no, CHARLIST *ch)
{
	for (int a = 0; a < 6; a++)
	{
		if (ch->quick[a].item_no == item_no) return 1;
	}

	return 0;
}

// 021107 YGI
int SendDial(short int cn, int npc_index, char *msg, int length)
{
	if (!CheckServerIndex(cn)) return 0;

	t_packet packet;
	packet.h.header.type = CMD_CHAT_DATA;
	{
		packet.u.server_chat_data.server_id = npc_index + 10000;
		packet.u.server_chat_data.type = (unsigned char)CHATTYPE_NORMAL;
		packet.u.server_chat_data.length = (unsigned char)length;
		memset(packet.u.server_chat_data.text, 0, TEXT_LENGTH);
		memcpy(packet.u.server_chat_data.text, msg, length);
	}
	packet.h.header.size = sizeof(t_server_chat_data) - TEXT_LENGTH + length;
	QueuePacket(connections, cn, &packet, 1);
	return 1;
}


/******************************************************************************
* 함수명 : void sDialogD( void )
* 설  명 : 대화를 하는데 대화하는 자의 방향이 주인공을 향한다.
* 인  자 :
* 리  턴 :
******************************************************************************/
void sDialOnlyPC(void)// 0
{
	int length;

	// Client에 대화를 보내준다....
	GetString(String);
	length = strlen(String);
	SendDial(EventPC, EventNPC[EventPC], String, strlen(String));
}

// Added by chan78 at 2000/11/28
void sSendMessageBox()			// 001212 YGI
{
	int length;
	t_packet packet;

	// Client에 대화를 보내준다....
	GetString(String);

	length = strlen(String);
	packet.h.header.type = CMD_SCRIPT_MESSAGE_BOX;
	{
		memcpy(packet.u.kein.default_msg, String, length);
	}
	packet.h.header.size = length + 1;

	if (connections[EventPC].dwAgentConnectionIndex && connections[EventPC].state >= CONNECT_JOIN)
		QueuePacket(connections, EventPC, &packet, 1);
}

void sSendSound()			// 001222 YGI
{
	t_packet packet;
	// Client에 대화를 보내준다....
	int type = GetInt();
	int sound = GetInt();

	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	packet.h.header.type = CMD_SEND_SOUND;
	{
		packet.u.kein.default_short_int = sound;
	}
	packet.h.header.size = sizeof(short int);

	switch (type)
	{	//< CSD-CN-031213
	case 0:
	{	// 자기자신한테만
		QueuePacket(connections, EventPC, &packet, 1);
		break;
	}
	case 1:
	{	// 주변사람들까지
		CastMe2Other(EventPC, &packet);
		QueuePacket(connections, EventPC, &packet, 1);
		break;
	}
	case 2:
	{	// 현재 맵
		g_pUserManager->SendPacket(&packet);
		break;
	}
	case 3:
	{
		g_pUserManager->SendPacket(&packet);
		SendNationPacket(&packet, ch->name_status.nation);
		break;
	}
	case 4:
	{	// 모든맵
		g_pUserManager->SendPacket(&packet);
		g_pServerTable->BroadCastToEveryServer((char*)&packet, (sizeof(t_header) + packet.h.header.size), SERVER_TYPE_MAP);
		break;
	}
	}	//> CSD-CN-031213
}

void sSendEffect()			// 010522 YGI
{
	int type = GetInt();
	int effect = GetInt();
	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	t_packet p;
	p.h.header.type = CMD_SEND_EFFECT;
	p.u.kein.send_effect.effect_num = effect;
	p.u.kein.send_effect.target_id = EventPC;
	// 040105 YGI 보물
	p.u.kein.send_effect.x = 0;
	p.u.kein.send_effect.y = 0;
	p.h.header.size = sizeof(k_send_effect);

	QueuePacket(connections, EventPC, &p, 1);

	if (type == 1)	// 주변사람에게도
	{
		CastMe2Other(EventPC, &p);
	}
}


void sDialInAreaPC(void)
{
	GetString(String);
}

void sDialEveryone(void)
{
	GetString(String);
}


void sAddItem(void)// 1
{
	int x, y;
	ItemAttr i;

	i.item_no = GetInt();
	i.attr[0] = 1;
	i.attr[1] = 1;
	i.attr[2] = 0;
	i.attr[3] = 1;
	i.attr[4] = 1;
	i.attr[5] = 1;

	x = GetInt();
	y = GetInt();

	AddItemList(i.item_no, i.attr, 0, x, y, 0, 0);
}


void sAddNpc(void)// 2
{
	int npcindex, x, y;

	npcindex = GetInt();
	x = GetInt();
	y = GetInt();

	AddNPCList(npcindex, x, y);
}

void sSetQuestNumStep(void)
{
	int quest_no = GetInt();
	int quest_value = GetInt();
	//EventPC		// 스크립트를 진행하고 있는 캐릭터 아이디를 가지고 있다.
	//connections	// 현재 서버에 접속해있는 클라이언트 구조체 ( 캐릭터 정보도 가지고 있다. )

	//CHARLIST *ch = &connections[EventPC].chrlst;
	//if( !ch ) return;

	var[EventPC][quest_no] = quest_value;
	//return ch->var[quest_no] = quest_value;
}

void sSetQuestNumStep(int quest_no, int quest_value)
{
	// EventPC		// 스크립트를 진행하고 있는 캐릭터 아이디를 가지고 있다.
	// connections	// 현재 서버에 접속해있는 클라이언트 구조체 ( 캐릭터 정보도 가지고 있다. )
	// CHARLIST *ch = &connections[EventPC].chrlst;
	// if( !ch ) return;
	var[EventPC][quest_no] = quest_value;
}

void sCmpQuestNumStep(int quest_no, int quest_value)
{
	if (var[EventPC][quest_no] - quest_value > 0)
	{
		sYES = 2;
	}
	if (var[EventPC][quest_no] - quest_value == 0)
	{
		sYES = 1;
	}
	if (var[EventPC][quest_no] - quest_value < 0)
	{
		sYES = 0;
	}
}

void sCmpQuestNumStep(void)
{
	int quest_no = GetInt();
	int quest_value = GetInt();
	if (var[EventPC][quest_no] - quest_value > 0)
	{
		sYES = 2;
	}
	if (var[EventPC][quest_no] - quest_value == 0)
	{
		sYES = 1;
	}
	if (var[EventPC][quest_no] - quest_value < 0)
	{
		sYES = 0;
	}
}

void sMapName(void)// 3
{
	GetString(MapName);
	CharUpper(MapName);

	strcpy(MAPNAME, MapName);

	int i = 0;
	for (; i < MAX_MAP_; i++)
	{
		if (strcmp(MapInfo[i].mapfile, MAPNAME) == 0)
		{
			MapNumber = i;
			return;
		}
	}

	//050314_KCH 5999맵(아템관리로그)은 TotalDB의 MapInfo에 없기때문(기획쪽에서 넣어주지 않음.) 임시로 Skip하게 함.
	if (0 == (strcmp(_T("ITEMSEARCH"), MAPNAME))) {
	}
	else if (i == MAX_MAP_) {
		Error("*** Error : Can NOT find the MapNumber of '%s'", MapName);
	}
}

extern void DeleteSquadMember(const int cn); // 010904 LTS//020903 lsw
void DecLocalWarfieldMemberCount(const int cn);	// LTS LOCALWAR
extern void CheckMapMove(int ID);			// LTS DRAGONLORDWAR
// Modified by chan78 at 2000/11/27

int MapMove(int idUser, const char* pMapName, int nX, int nY)
{	//< CSD-HK-030829

	//<! BBD 040329		유저방출중 맵무브 금지한다
	if (g_KickMgr.IsKicking())
	{
		return 0;
	}
	//> BBD 040329		유저방출중 맵무브 금지한다

	CHARLIST* pUser = CheckServerId(idUser);

	if (pUser == NULL)
	{
		return 0;
	}
	// 맵이동시 캐릭터 데이타 저장
	UpdateCharacterData(connections, idUser);

	LPMAPINFO pMapInfo = GetMapInfoByMapName(pMapName);

	if (pMapInfo == NULL)
	{
		return 0;
	}

	MAP_MOVE_INFO infMapMove;
	infMapMove.idUser = idUser;
	infMapMove.nMapPort = pMapInfo->port;
	infMapMove.nX = nX;
	infMapMove.nY = nY;
	g_pUserManager->Push(infMapMove);
	return 1;
}	//> CSD-HK-030829

int GotoMap(int cn, int nMapPort, int nMoveX, int nMoveY)
{	//< CSD-HK-030829
	CHARLIST *ch = ::CheckServerId(cn);
	if (!ch) return 0;

	LPMAPINFO pMapInfo = GetMapInfoByMapPort(nMapPort);

	if (!pMapInfo)
	{
		return 0;
	}

	const int iLimitLvMin = pMapInfo->LvMin;//030116 lsw
	const int iLimitLvMax = pMapInfo->LvMax;
	if (!ch->IsCounselor() &&
		((iLimitLvMin && iLimitLvMin > ch->GetLevel()) || (iLimitLvMax && iLimitLvMax < ch->GetLevel())))
	{
		t_packet p;
		p.h.header.type = CMD_MAP_MOVE_LV_LIMIT;
		p.h.header.size = sizeof(MAPMOVELVLIMIT);

		p.u.Hwoa.rang.MapMoveLvLimit.iLvMin = iLimitLvMin;
		p.u.Hwoa.rang.MapMoveLvLimit.iLvMax = iLimitLvMax;

		::QueuePacket(connections, cn, &p, 1);
		return 0;
	}

	if (!g_LightVersion.IsAbleMapMove(ch, pMapInfo->mapfile))
	{	// 라이트 버전이라 실패
		ch->Message(MK_WARNING, 5, 28);
		ch->Message(MK_WARNING, 5, 29);
		return 0;		// 라이트 버전이라 실패..
	}

	if (!ch->IsCounselor() && !ch->IsReporter()) // 운영자나 도우미, 기자는 들어갈수 있다.
	{
		const int type = pMapInfo->m_MoveType; //020903 lsw

		switch (type)
		{
		case MMT_NORMAL:
		{
			break;		// 이동하는데 아무런 제약이 없다.
		}
		case MMT_FAIL: 		// 없는 맵이다.
		case MMT_NOT_MOVE:
		{
			CallClient(cn, CMD_CHANGE_MAP_FAIL);
			return 0;		// 이동할 수 없다.
		}
		}
	}

	if (ch->IsDead())
	{
		::SendViewTypeChange(ch->GetServerID(), VIEWTYPE_GHOST_);//030102 lsw
	}

	t_packet dummy_packet;
	strcpy(connections[cn].mapname, pMapInfo->mapfile);
	// 맵체인지 실패상황을 위한 백업.
	ch->MapChange_MoveGox = ch->MoveGox;
	ch->MapChange_MoveGoy = ch->MoveGoy;
	ch->MapChange_X = ch->X;
	ch->MapChange_Y = ch->Y;
	ch->MapChange_MoveSx = ch->MoveSx;
	ch->MapChange_MoveSy = ch->MoveSy;
	ch->MapChange_MoveLength = ch->MoveLength;
	ch->MapChange_MovePathCount = ch->MovePathCount;
	// 새 맵을 위한 세팅.
	ch->MovePathCount = 0;
	ch->MoveLength = 0;
	ch->MoveGox = ch->X = nMoveX * TILE_SIZE + 16;
	ch->MoveGoy = ch->Y = nMoveY * TILE_SIZE + 16;
	ch->MoveSx = nMoveX;
	ch->MoveSy = nMoveY;
	strcpy(ch->MapName, pMapInfo->mapfile);
	// Packet Setting
	memcpy(dummy_packet.u.server_change_map.mapname, pMapInfo->mapfile, 20);
	dummy_packet.u.server_change_map.x = nMoveX;
	dummy_packet.u.server_change_map.y = nMoveY;
	// Build Packet by Querying.
	if (GetChangeMapPort(connections, cn, &dummy_packet) != 1)
	{
		MyLog(LOG_FATAL, "sMapMove() :: Failed To Query");
		goto NO_PORT_;
	}
	// Copy it connections[]
	memcpy(&connections[cn].server_change_map, &dummy_packet.u.server_change_map, sizeof(t_server_change_map));
	// Get My Own Agent ServerID
	WORD wMyAgentServerID;
	if (g_pServerTable->GetServerData(connections[cn].dwAgentConnectionIndex))
	{
		wMyAgentServerID = g_pServerTable->GetServerData(connections[cn].dwAgentConnectionIndex)->wPort;
	}
	else
	{
		wMyAgentServerID = 0;
	}

	if (NewSendGameToLoginChangeMapData(cn, wMyAgentServerID, WORD(dummy_packet.u.server_change_map.port)))
	{
		DeleteSquadMember(cn);
		DecLocalWarfieldMemberCount(cn);
		connections[cn].state = CONNECT_CHANGING_MAP;
		CheckMapMove(cn);								// LTS DRAGONLORDWAR
		return 1;
	}

NO_PORT_:
	// 커넥션 값 복구
	connections[cn].state = CONNECT_JOIN;
	strcpy(connections[cn].mapname, MapName);
	// 백업해놓았던 값으로 되돌리기.
	ch->MoveGox = ch->MapChange_MoveGox;
	ch->MoveGoy = ch->MapChange_MoveGoy;
	ch->X = ch->MapChange_X;
	ch->Y = ch->MapChange_Y;
	ch->MoveSx = ch->MapChange_MoveSx;
	ch->MoveSy = ch->MapChange_MoveSy;
	ch->MoveGox = ch->X;
	ch->MoveGoy = ch->Y;
	ch->MoveLength = ch->MapChange_MoveLength;
	ch->MovePathCount = ch->MapChange_MovePathCount;

	CallClient(cn, CMD_CHANGE_MAP_FAIL);
	return 0;
}	//> CSD-HK-030829

// 021117 YGI
void sMapMove(void)// 4
{
	k_map_move data;
	//char mapname[ 20];
	//short int	 x, y;

	GetString(data.mapname);
	strupr(data.mapname);
	data.x = GetInt();
	data.y = GetInt();

	//SendPacketDefault( CMD_MOVEMAP_FOR_LIGHT_VERSION, &data, sizeof( k_map_move )-20+strlen( data.mapname ), EventPC );
	//sYES = 1;

	sYES = MapMove(EventPC, data.mapname, data.x, data.y);
}

void sIsLevel(void)
{	//< CSD-030806
	const int lev = GetInt();
	// EventPC의 레벨이 lev보다 높으면 1를 sYES에 넣고 그렇지 않으면 0을 넣는다. 
	if (connections[EventPC].chrlst.GetLevel() >= lev)
	{
		sYES = 1;
	}
	else
	{
		sYES = 0;
	}

}	//> CSD-030806

void sIsJobPC(void)
{
	int job = GetInt() - 1 + 20;//직업은 0번 부턴데... 1번부터로 잡았음... 
	// EventPC의 직업이 job이면  1을  sYES에 넣고 그렇지 않으면 0을 넣는다. 

	if (connections[EventPC].chrlst.Job == job)
	{
		sYES = 1;
	}
	else
	{
		sYES = 0;
	}

}

void sGetGender(void)			// 010210 YGI
{
	// 030930 kyo <<
	CHARLIST *ch = CheckServerId(EventPC);
	if (NULL == ch)
	{
		return;
	}
	// 030930 kyo >>

	sYES = connections[EventPC].chrlst.Gender;		// 0 : 여자, 1 : 남자
}

void sQuestComplete(int magic_no)
{	//< CSD-TW-030606
	CHARLIST* pCaster = CheckServerId(EventPC);

	if (pCaster == NULL)
	{
		return;
	}

	t_packet packet;
	packet.h.header.type = CMD_MAGIC_EXECUTE;
	packet.h.header.size = sizeof(t_server_magic_execute);
	packet.u.magic.server_magic_execute.nMagic = BYTE(magic_no);
	packet.u.magic.server_magic_execute.idCaster = pCaster->GetServerID();
	packet.u.magic.server_magic_execute.idTarget = pCaster->GetServerID();
	packet.u.magic.server_magic_execute.nX = pCaster->X;
	packet.u.magic.server_magic_execute.nY = pCaster->Y;
	QueuePacket(connections, EventPC, &packet, 1);
	CastMe2Other(EventPC, &packet);
}	//> CSD-TW-030606

void sQuestComplete(void)
{
	short int command_type = GetInt();
	t_client_character_demand p;
	p.target_id = connections[EventPC].chrlst.GetServerID();
	p.command_type = command_type;
	RecvCMD_CLIENT_CHARACTER_DEMAND(EventPC, &p);

}
void sQuestSound(void)
{
	//15001
	//
	short int command_type = GetInt();
	t_client_character_demand p;
	p.target_id = connections[EventPC].chrlst.GetServerID();
	p.command_type = command_type;
	RecvCMD_CLIENT_CHARACTER_DEMAND(EventPC, &p);

}
void sQuestTeleport(void)
{
	int commandNum = GetInt();
	int tile_X = GetInt();
	int tile_Y = GetInt();
	//	int count=0;
	//	t_packet packet;
	//	CHARLIST	*caster;
	t_server_magic_position	smp;

	//클라이언트로 보낼 패킷 셋팅
	smp.nType = PT_TELEPORT;
	smp.idTarget = EventPC;
	smp.nX = tile_X;
	smp.nY = tile_Y;
	SendCMD_MAGIC_POSITION(&smp);
}

void QuestTeleport(const int nCn, const int nX, const int nY)
{// 031030 kyo

	CHARLIST *ch = CheckServerId(nCn);
	if (!ch) return;

	t_server_magic_position	smp;

	//클라이언트로 보낼 패킷 셋팅
	smp.nType = PT_TELEPORT;
	smp.idTarget = nCn;
	smp.nX = nX;
	smp.nY = nY;
	SendCMD_MAGIC_POSITION(&smp);
}

void sIsInvHavePC(void)
{
	int ItemNum = GetInt();
	int ret;
	CHARLIST	*caster;
	caster = &connections[EventPC].chrlst;
	ret = SearchItemByInv(ItemNum, caster);
	if (ret == 1)
	{
		sYES = 1;
	}
	else
	{
		ret = SearchItemByEquip(ItemNum, caster);
		if (ret == 1)
		{
			sYES = 2;
		}
		else
		{
			ret = SearchItemByQuick(ItemNum, caster);
			if (ret == 1)
			{
				sYES = 3;
			}
			else
			{
				sYES = 0;
			}
		}
	}
}

//YGI acer
void sSetItemInvPC(void)
{
	int change_no = GetInt();
	int item_no = GetInt();
	int item_dur = GetInt();
	//	int item_durmax = GetInt();

	CHARLIST *ch = &connections[EventPC].chrlst;
	for (int a = 0; a < 3; a++) {
		for (int b = 0; b < 3; b++) {
			for (int c = 0; c < 8; c++) {
				if (ch->inv[a][b][c].item_no == change_no)
				{
					SendItemEventLog(&ch->inv[a][b][c], EventPC, SN_NOT_USER, SILT_DELETE_BY_SCRIPT, 2);//넣기전.

					ItemAttr target = GenerateItem(item_no, IATTR_MUCH, item_dur);//020509 lsw

					ch->inv[a][b][c] = target;
					if (target.item_no)
					{
						SendItemEventLog(&ch->inv[a][b][c], EventPC, SN_NOT_USER, SILT_MAKE_BY_SCRIPT, 2);//넣은후
					}

					POS pos;
					SetItemPos(INV, a, b, c, &pos);
					SendServerEachItem(&pos, &target, EventPC);		// 001028 YGI

					sYES = 1;
					return;
				} // if
			} // for
		} // for
	}  // for

	sYES = 0;
}

void sSetSkillPC(void)
{
	int SkillNum = GetInt();
	int SkillLearn = GetInt();
	CHARLIST *ch = &connections[EventPC].chrlst;
	if (!ch) return;

	if (SkillLearn)
	{
		ch->Skill[SkillNum] = true;
		makemyjobexpsetting(SkillNum, ch);
		SendLearnSkillOk(SkillNum, connections, EventPC);
	}
	return;
}

void Recv_CMD_QUEST_JOB_FINISH(int cn, int type)
{
	sCmpQuestNumStep(52, 1997);
	if (sYES == 1)
	{
		sSetQuestNumStep(52, 1998);
	}
}

void sIsItemHavePC(void)
{
	int ItemNum = GetInt();
	int ItemPos = GetInt();
	//해당하는 아이템이 해당 장비에  착용하고 있으면.. 에스아니면 노
	if (connections[EventPC].chrlst.equip[ItemPos].item_no == ItemNum)
	{
		sYES = 1;
	}
	else
	{
		sYES = 0;
	}
	return;


}
void sIsClassPC(void)
{
	int ClassNum = GetInt();

	//해당하는 클래스면.. 에스아니면 노
	if (connections[EventPC].chrlst.Class == ClassNum)
	{
		sYES = 1;
	}
	else
	{
		sYES = 0;
	}
	return;
}

void sIsSkillHavePC(void)
{
	int SkillNum = GetInt();
	//해당 기술이 있으면 에스 아니면 노 
	if (connections[EventPC].chrlst.Skill[SkillNum] == 1)
	{
		sYES = 1;
	}
	else
	{
		sYES = 0;
	}
	return;
}

void sCheckNation(void)
{
	int NationNum = GetInt();
	// 같은 나라라면 예스  

	if (connections[EventPC].chrlst.name_status.nation == (DWORD)NationNum)
	{
		sYES = 1;
	}
	else
	{
		sYES = 0;
	}
	return;
}

// Monster가 발생할 장소와 Sprite번호를 정의한다. 
void sInitMonsterGeneratedPosition(void)
{
	int x = GetInt();
	int y = GetInt();
	int npcindex = GetInt();

	return;//

	NPCGeneratePosition(npcindex, x, y, 0, 0);
}

void sIsPlayArenaGame()		// 전투맵 시작인지 아닌지
{	//< CSD-030517
	const int nGame = GetInt();
	CBaseArena* pGame = g_pArenaManager->GetGameInfo(nGame);

	if (pGame == NULL)
	{
		sYES = 0;
		return;
	}

	if (!pGame->IsPlayGame())
	{
		sYES = 0;
	}
	else
	{
		sYES = 1;
	}
}	//> CSD-030517

void sAddFightMap(void)
{	//< CSD-RESERVE

}	//> CSD-RESERVE

void sDeleteFightMap(void)				// 914 YGI
{	//< CSD-RESERVE

}	//> CSD-RESERVE

void sIsFullFightMapTeam(void)		// 0915 YGI
{	//< CSD-RESERVE

}	//> CSD-RESERVE

int callmenudef[] =
{
	MN_QUEST			,	// 임시 5개 YGI  0923
	MN_FAME				,
	MN_QUEST_INDEX		,
	MN_RELATION			,
	MN_MAN_TO_MAN		,

	MN_SKILLGUILD_BUY	,
	MN_SKILLGUILD_SELL	,
	MN_SKILLGUILD_SKILL	,
	MN_SKILLGUILD_ITEM	,
	MN_SKILLGUILD_REPAIR,		// 9
	MN_BANK_DEPOSIT,
	MN_BANK_DEFRAYAL,
	MN_BANK_CUSTODY,
	MN_BANK_MAIN,
	MN_BANK_LOAN,
	MN_BANK_REPAYMENT,
	MN_BANK_AUCTION,
	MN_BANK_AUCTION_SALE,
	MN_BLACKSMITH			,
	MN_FARM		,			// 19
	MN_RESTAURANT,
	MN_HERBSHOP,
	MN_SHAMBLES	,			// 22
	MN_CANDLE	,			// 23
	MN_SLOPSHOP	,
	MN_ALCHEMY	,
	MN_WOODWORKING,			// 26
	MN_BOWCRAFT	,
	MN_SHIPYARD	,
	MN_TOWNHALL ,		// 29
	MN_TOWNHALL_PUBLICNOTICE ,
	MN_TOWNHALL_BUILDING,
	MN_TOWNHALL_COMMERCE1,
	MN_TOWNHALL_COMMERCE2,
	MN_TOWNHALL_COMMERCE3,
	MN_TOWNHALL_HOUSEREPAIR,
	MN_TOWNHALL_GUILD,
	MN_TOWNHALL_FORUM,
	MN_TOWNHALL_EVENT,
	MN_TOWNHALL_RANK1,		// 39
	MN_TOWNHALL_RANK2,
	MN_TOWNHALL_NOTICE4,
	MN_TOWNHALL_NOTICE3,
	MN_TOWNHALL_NOTICE2,
	MN_TOWNHALL_NOTICE1,
	MN_TOWNHALL_COMMERCE4,
	MN_TOWNHALL_CONSTRUCTION,
	MN_MY_HOME,
	MN_MY_HOME_EXPLAN,
	MN_HOUSE,				// 49
	MN_MY_HOME_IN,
	MN_HOME_PERSON,
	MN_GOD1,
	MN_GOD2,
	MN_GOD3,
	MN_MAGICBOOK,
	MN_FUNCTION,
	MN_DRINK_MAIN,
	MN_DRINK_RUMOR	,
	MN_DRINK_BUY	,		// 59
	MN_SHOP_MAIN,			//60
	MN_SHOP_SELL,
	MN_SHOP_BUY	,
	MN_HEAL_MAIN,			//63
	MN_HEAL_HEALING,
	MN_HEAL_PREVENTING,
	MN_HEAL_LEARN,
	MN_INN_MAIN,			// 67
	MN_INN_REST,
	MN_INN_SLEEP,			// 69
	MN_SYSTEM_LOGOUT,		// 70
	MN_MAGIC_GUILD,			// 71 마법사 길드		// 0208_2 YGI
	MN_ACADEMY,				// 72 아카데미			// 0208_2 YGI

	MN_GOD_EDELBLHOY,		// 73	에델브로이 신전		// 0410 YGI
	MN_GOD_TEFFERY,			// 74	테페리 신전			// 0410 YGI
	MN_GOD_LETTY,			// 75	레티 신전			// 0410 YGI

	MN_GUILD_ADMIN,			// 76	GM 등록 메뉴 		// 1027 YGI
	MN_SALVATION2,			// 77	적십자 메뉴			//  YGI				// 010522 YGI
	MN_CHANGE_MONEY,		// 78						// 010204 YGI

	MN_MERCHANT_MAIN,		// 79	// 상인 메뉴 메인		// 010410 YGI
	MN_ARENA_LOBBY,			// 80	// 배틀메뉴 // CSD-030509

	MN_LADDER_RANK,			// 81	// 010511 YGI
	MN_GOD_OREM,			// 82		// 010525 YGI

	MN_NATION_MAIN_VA,		// 83
	MN_NATION_MAIN_ZY,		// 84
	MN_NATION_MAIN_ILL,		// 85

	MN_MERCHANT_OLDMAIN,	// 86	// 상인등록메뉴 되찾기 메뉴 // BBD 040303 	
};

// 010530 YGI
extern void SendCallSmallMenu(int cn, int storelistindex, int menu);
void sCallMenu(void)
{
	int menu = GetInt();
	SendCallSmallMenu(EventPC, 0, menu);
}

void sGetNationByMap(void)	// 특정 맵의 국적알아 오기
{
	char	mapname[30];
	GetString(mapname);

	s_Nation = GetNationByMapName(mapname);
}

void sCalcFriendShip(void)	// 특정 국가의 적대수치 수정하기	// 빼고 싶을때는 -값을 넣어 준다.
{
	int nation = GetInt();
	int friend_ship = GetInt();

	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	sYES = 1;

	short int *ch_friend_ship;
	switch (nation)
	{
	case N_VYSEUS:	ch_friend_ship = &ch->nk[N_VYSEUS]; break;
	case N_ZYPERN: ch_friend_ship = &ch->nk[N_ZYPERN]; break;
	case N_YILSE: ch_friend_ship = &ch->nk[N_YILSE]; break;

	default: sYES = 0; return;
	}

	*ch_friend_ship += friend_ship;
	return;
}

void sCompFriendShip(void)	// 적대수치 비교하기
{
	int nation = GetInt();
	int comp_data = GetInt();

	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	short int *ch_friend_ship;
	switch (nation)
	{
	case N_VYSEUS:	ch_friend_ship = &ch->nk[N_VYSEUS]; break;
	case N_ZYPERN: ch_friend_ship = &ch->nk[N_ZYPERN]; break;
	case N_YILSE: ch_friend_ship = &ch->nk[N_YILSE]; break;

	default: sYES = -2; return;
	}

	if (*ch_friend_ship > comp_data) sYES = 1;
	else if (*ch_friend_ship == comp_data) sYES = 0;
	else sYES = -1;
}

// 010531			YGI
void sCompLadderScore(void)
{
	int score = GetInt();

	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	sYES = ch->LadderScore - score;
}


///////////////////////////////////////////////////////////////////////////x

void sCallSmallMenu(void)
{
	int index = GetInt();	// 상점 List Index..
	int menu = GetInt();	// SmallMenu Index..


//<soto-030430
	if (256 == menu || 257 == menu || 258 == menu)
	{
		CallSmallMenu(EventPC, index, menu);
		return;
	}
	//>soto-030430


		//020707 YGI
	if (menu < 1000)
	{
		CallSmallMenu(EventPC, index, callmenudef[menu]);
	}
	else
	{
		SendCallSmallMenu(EventPC, index, menu - 1000);
	}
}

void sWeatherOn(void)
{
	g_weatherflag = 1;
}

void sWeatherOff(void)
{
	g_weatherflag = 0;
}

void sRecallScriptStart(void)
{
	int aftertime;

	sRECALL_SCRIPT_COUNT = GetInt();
	aftertime = GetInt();

	sRECALL_SCRP_NO = NScriptNo; //    (var[EventPC][1])
	sRECALL_CURTIME = g_curr_time;
	sRECALL_AFTTIME = g_curr_time + aftertime;
}
void sRecallScriptEnd(void)
{
	sRECALL_SCRIPT_COUNT = 0;
	sRECALL_SCRP_NO = 0;
}

void sMaxMonsterGeneration(void)
{
	Max_NPC_Generation = GetInt();

	if (Max_NPC_Generation > MAX_NPC_LIST) Max_NPC_Generation = MAX_NPC_LIST;
}

void sLoadItemValueOneMaul(void)
{
	int maulno = GetInt();
}

void srandom(void)		// 0131 YGI
{
	int start, end;
	start = GetInt();
	end = GetInt();
	var[EventPC][5] = (rand() % (end - start)) + start;
}

void sEventMsg(void)// 0
{
	// Client에 대화를 보내준다....	
	GetString(String);

	if (connections[EventPC].dwAgentConnectionIndex && connections[EventPC].state >= CONNECT_JOIN)
	{
		SendBBS2(String, (short)EventPC);
		//SendBBS2라는 이벤트 메세지 발생 함수를 실행한당.
		//QueuePacket(connections, EventPC, &packet, 1);
	}

	return;
}

void sReturnSealStoneBreakable(void)
{
	switch (ReturnSealStoneBreakable())
	{
	case CMD_STEALSTONE_BREAKABLE:			sYES = 1;	break;
	case CMD_STEALSTONE_NON_BREAKABLE_PEACE:	sYES = 2;	break;
	case CMD_STEALSTONE_NON_BREAKABLE:			sYES = 0;	break;
	default:									sYES = 0;	break;
	}
}

// 020428 YGI acer
void sSaveItemAtDb(void)
{
	int a, b, c;
	POS pos;
	ItemAttr *pItem;

	int item_no = GetInt();
	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) goto ERROR__;

	if (!SearchItemByInv(item_no, ch, a, b, c)) goto ERROR__;

	// 먼저 아이템을 지우고 디비로 보낸다.

	SetItemPos(INV, a, b, c, &pos);
	pItem = GetItemByPOS(EventPC, pos);
	if (!pItem) goto ERROR__;

	// 020428 YGI acer
	SendItemEventLog(pItem, EventPC, 0, SILT_DELETE_BY_SCRIPT, 7);	 //YGI acer
	SendDeleteItem(pItem, &pos, ch);
	SendSaveEventItem(ch, item_no, 1);		// 이벤트 아이템 디비에 저장

	sYES = 1;
	return;

ERROR__:
	sYES = 0;
	return;
}

void sCheckMoney(void)	// 피시의 돈을 알아오기
{
	int money = GetInt();
	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	DWORD pc_money;

	s_PcMoney = pc_money = GetMoneyByItem(ch);

	//sYes = pc_money - money;		// 더많이 가지고 있을경우엔 양의 정수
	if (pc_money > money) sYES = 1;		// 더많이 가지고 있을경우
	else if (pc_money == money) sYES = 0;
	else sYES = -1;
}

// 피시에게서 돈 깍기
void sSubtractMoney(void)
{
	int money = GetInt();
	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	DWORD pc_money;
	pc_money = GetMoneyByItem(ch);
	if (pc_money < money) money = pc_money;

	SubtractMoney(money, ch);
}

// 010129 KHS		
// 몬스터가 발생 조건을 읽어들인다. 
// 기존에 NPC의 발생은 .SKB화일에 들어 있는 Data를 이용하는것을 원칙으로 하지만
// Quest조건에 맞는 NPC를 발생시키기 위해 추가 하였다. 
// 이는 10분마다 조건을 검색하여 조건에 맞으면 발생한다. 
void sGenMonSpec(void)
{
	//int x, y;
	//int monsprno;
	//int lev;
	//int rate;
	//int time;
	//int iftime;
	//int turn;

	//x = GetInt();	// 맵의 좌표.(단위: Tile )
	//y = GetInt();
	//monsprno = GetInt();	// 몬스터의 번호.
	//lev = GetInt();	// 발생할 Monster의 Level
	//rate = GetInt();	// 발생활률 ( 10000 = 100% )

	//genyear = GetInt();	// 발생조건시각시간( 이 시간이후 부터 아래 시간에 따라 발생한다. )
	//genmon = GetInt();
	//genday = GetInt();
	//genhour = GetInt();
	//genmin = GetInt();

	//iftime = GetInt();	// 위의 gen*의 시간의 차이를 발생조건으로 한다.

	//turn = GetInt();	// 몇턴마다 발생하는가.( 1이면 10분마다 발생한다. )

}

void sGetGuild()		// 010511 YGI
{	//< CSD-030324
	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	sYES = ch->GetGuildCode();
	return;
}	//> CSD-030324

extern int g_pLoadingPoint[NW_NATION_COUNT][NW_SQUAD_MAX]; // 010904 LTS

void sCheckLoadingPoint() // 010904 LTS	
{
	CHARLIST* ch = CheckServerId(EventPC);   //공격자 방어자를 분리해야 한다... ㅜ.ㅜ 
	if (!ch) return;
	sYES = g_pLoadingPoint[ch->NWCharacter.WarfieldNo][ch->NWCharacter.SquadNo];	// 010915 LTS
}

extern bool CheckDefence(int cn); // 010904 LTS
extern bool CanGoBonus();			// 010915 LTS

void sCheckDefence()			// 방어자 인지 확인한다. 
{
	if (CheckDefence(EventPC)) sYES = 1;
	else sYES = 0;
}

void sCanGoBonus()
{
	if (CanGoBonus()) sYES = 1;
	else sYES = 0;
}

void sFameDown()
{
	int tempValue = GetInt();
	g_pLogManager->SaveLogChange_Fame(&connections[EventPC].chrlst, connections[EventPC].chrlst.fame, connections[EventPC].chrlst.fame - tempValue, LF_SCRIPT);		// 020909 LTS
	connections[EventPC].chrlst.fame -= tempValue;
}

void sFameUp()
{
	int tempValue = GetInt();
	g_pLogManager->SaveLogChange_Fame(&connections[EventPC].chrlst, connections[EventPC].chrlst.fame, connections[EventPC].chrlst.fame + tempValue, LF_SCRIPT);		// 020909 LTS
	connections[EventPC].chrlst.fame += tempValue;
}

void sFameCheck()
{
	int tempValue = GetInt();
	if (connections[EventPC].chrlst.fame >= tempValue) sYES = 1;
	else sYES = 0;
}
//_________________________________________________________________	// 010915 LTS

// CSB AddCode
// CSB AddCode
// CSB AddCode
extern void(*SFunction[_SCRIPT_FUNCTION_POINT_])(void);

QT QTickData[DRAGON_MAX_CONNECTIONS_];
DWORD QTemp, QTick = 1;

// 초당 한번씩 진입됨
void QuestSecond()	// 1000 ms
{
	QTick++;

	for (QTemp = 0; QTemp < DRAGON_MAX_CONNECTIONS; QTemp++)
	{
		if (QTick == QTickData[QTemp].Tick)
		{
			if (!QTickData[QTemp].Func)
			{
				continue;
			}

			QTickData[QTemp].Func();
		}
	}
}

void TimerFirstFunc(void)
{
	// CSB 코딩필요

	// 나중에 이곳에 해당 유저( QTemp )의 퀘스트 아이템(기밀문서?)
	// 가 있는지 체크하여 없애주고 유저에게 늦었다는 메시지를 보내 주어야한다.
	// 퀘스트 스텝(퀘스트 번호 미정)을 특정 숫자(미정--;)로 셋팅해주어야 한다.
	// 2~3줄 코딩 요망 ( 수정 될지도 모름 )

	// 예제(실제)
	// if( 기밀문서가 있는가? ) {
	//		퀘스트 스텝 설정
	//		2분 지남을 유저에게 알림
	//		기밀문서 삭제
	// } 

	int change_no = 0;
	if (SearchItemByInv(10176, &connections[QTemp].chrlst)) change_no = 10176;
	if (SearchItemByInv(10177, &connections[QTemp].chrlst)) change_no = 10177;
	if (SearchItemByInv(10178, &connections[QTemp].chrlst)) change_no = 10178;
	if (change_no) {

		var[QTemp][56] = 10;

		int item_no = 0;
		int item_dur = 0;

		CHARLIST *ch = &connections[QTemp].chrlst;
		for (int a = 0; a < 3; a++) {
			for (int b = 0; b < 3; b++) {
				for (int c = 0; c < 8; c++) {
					if (ch->inv[a][b][c].item_no == change_no)
					{
						ItemAttr target = GenerateItem(item_no, IATTR_MUCH, item_dur);//020509 lsw
						ch->inv[a][b][c] = target;
						POS pos;
						SetItemPos(INV, a, b, c, &pos);
						SendServerEachItem(&pos, &target, QTemp);
						sYES = 1;
						return;
					} // if
				} // for
			} // for
		}  // for
	} // if (change_no)

	int length;
	t_packet packet;

	// Client에 대화를 보내준다....
	strcpy(String, "2분이 경과");

	length = strlen(String);
	packet.h.header.type = CMD_SCRIPT_MESSAGE_BOX;
	{
		memcpy(packet.u.kein.default_msg, String, length);
	}
	packet.h.header.size = length + 1;

	if (connections[QTemp].dwAgentConnectionIndex && connections[QTemp].state >= CONNECT_JOIN)
		QueuePacket(connections, QTemp, &packet, 1);
}

void TimerSecondFunc(void)
{

	// 이곳에 코딩.

}

// 특정 시간후 실행될수 있는 함수 리스트
void(*TimerFunc[_SCRIPT_FUNCTION_POINT_])(void) =
{
	TimerFirstFunc,	 // 0 번 타이머 함수
	TimerSecondFunc, // 1 번 타이머 함수
};

// 유저별 타이머 설정
void sSetTimer()
{
	QTickData[EventPC].Tick = QTick + GetInt();	   // 몇 초후 실행 (최대 오차 범위 1초)
	GetInt();									   // 여분
	QTickData[EventPC].Func = TimerFunc[GetInt()]; // 초후 실행될 함수
	QTickData[EventPC].ExtInt = GetInt();		   // 실행후 사용될 확장값(Integer)
	GetString(QTickData[EventPC].ExtChar);		   // 실행후 사용될 확장값(String)

}
// 유저의 타이머 중지
void sStopTimer()
{
	QTickData[EventPC].Func = NULL;
}
// 레벨 가져오기 - yes 에 레벨 저장
void sGetLevel()
{	//< CSD-030806
	sYES = connections[EventPC].chrlst.GetLevel();
}	//> CSD-030806
// 클래스 가져오기 - yes 에 클래스값 저장
void sGetClass()
{
	sYES = connections[EventPC].chrlst.Class;
}
// 어빌리티 가져오기 - yes 에 어빌리티 번호 저장
void sGetAbil()
{
	int abil = GetInt();
	sYES = 0;

	switch (abil)
	{
	case 1:  sYES = connections[EventPC].chrlst.Str;   break;
	case 2:  sYES = connections[EventPC].chrlst.Con;   break;
	case 3:  sYES = connections[EventPC].chrlst.Dex;   break;
	case 4:  sYES = connections[EventPC].chrlst.MoveP; break;
	case 5:  sYES = connections[EventPC].chrlst.Wis;   break;
	case 6:  sYES = connections[EventPC].chrlst.Int;   break;
	case 7:  sYES = connections[EventPC].chrlst.wsps;  break;
	case 8:  sYES = connections[EventPC].chrlst.Char;  break;
	case 9:  sYES = connections[EventPC].chrlst.Endu;  break;
	case 10: sYES = connections[EventPC].chrlst.Moral; break;
	case 11: sYES = connections[EventPC].chrlst.Luck;  break;
	}
}
// 특정 유저와 파티를 맺었는지 확인 - yes에 있으면 1 없으면 0
void sCheckParty()
{
	char id[0xff];
	int ti;

	GetString(id);

	sYES = 0;
	for (ti = 0; ti < MAX_PARTY_MEMBER; ti++)
	{
		if (connections[EventPC].chrlst.party[ti].On
			&& !strcmp(connections[EventPC].chrlst.party[ti].Name, id)) sYES = 1;
	}


}
// 특정 유저의 국가를 가져옴 - yes 에 국가 번호
void sGetNat()
{
	sYES = connections[EventPC].chrlst.name_status.nation;
}

// 털뙤KING
void sGetKing()
{
	sYES = connections[EventPC].chrlst.name_status.king;
}

// 스펠 종류 얻어오기 - yes 에 스펠 종류 번호 반환
void sGetSpell()
{
	sYES = connections[EventPC].chrlst.Spell;
}
// 파티 수 반환 - yes 에 현재 맺고 있는 파티수 반환
void sCheckPartyCount()
{
	int ti;

	sYES = 0;
	for (ti = 0; ti < MAX_PARTY_MEMBER; ti++) {
		if (connections[EventPC].chrlst.party[ti].On) sYES++;
	}
}
// 최근 사용한 마법 - yes 에 최근에(마지막에) 사용한 마법번호 반환
void sCheckLastMagic()
{
	sYES = connections[EventPC].chrlst.GetMagic();
}
// 택틱 가져오기 - yes 에 인자값으로 받은 택틱값 반환
void sGetTactic()
{
	int tac = GetInt();
	sYES = connections[EventPC].chrlst.Skill[87 + tac];
}
// 모든 택틱중 특정 수치이상의 택틱이 있는지 체크 - yes 에 없으면 0 있으면 1 반환
void sCheckTacticPoint()
{
	int mode = GetInt();
	int point = GetInt();
	int ti;

	sYES = 0;
	for (ti = 0; ti < 13; ti++) {
		if ((mode == 0 && point > connections[EventPC].chrlst.Skill[88 + ti])
			|| (mode == 1 && point == connections[EventPC].chrlst.Skill[88 + ti])
			|| (mode == 2 && point < connections[EventPC].chrlst.Skill[88 + ti])) {

			sYES = 1;
			break;
		}  // if
	}  // for
}


// 듀얼 인지 아닌지 - yes 에 인자값으로 0은 아님 1은 듀얼 반환
void sIsDual()
{
	sYES = connections[EventPC].chrlst.IsDual();
}


// 상우 요청
void sCallCustomSmallMenu(void)
{
	int iMenuNo = GetInt();
	int iPicNo = GetInt();
	int iWidth = GetInt();
	int iHeight = GetInt();
	int iExt = GetInt();

	t_packet p;
	p.h.header.type = CMD_CALL_SMALLMENUCUSTOM;
	{
		p.u.server_call_smallmenucustom.iMenuNo = iMenuNo;
		p.u.server_call_smallmenucustom.iPicNo = iPicNo;
		p.u.server_call_smallmenucustom.iWidth = iWidth;
		p.u.server_call_smallmenucustom.iHeight = iHeight;
		p.u.server_call_smallmenucustom.iExt = iExt;
	}
	p.h.header.size = sizeof(t_server_call_smallmenucustom);

	QueuePacket(connections, EventPC, &p, 1);
}

// 021117 YGI
void sSetRareItemInvPc(void)
{
	const int iItemNo = GetInt();
	const int iTryMulti = GetInt();
	const int iGradeMin = GetInt();
	const int iGradeMax = GetInt();
	const int iMutant = GetInt();

	CHARLIST *ch = ::CheckServerId(EventPC);
	if (!ch) return;

	ItemAttr item;//021213 lsw 0으로 나눠지는 문제 수정
	const int iGradeGab = iGradeMax - iGradeMin;
	const int iResultGrade = iGradeMin + ((0 < iGradeGab) ? rand() % iGradeGab : 0);
	const int ret = ItemMgr.MakeDefaultRareItem(&item, iItemNo, iMutant, iResultGrade, iTryMulti);
	if (ret == 1)
	{
		::SendItemEventLog(&item, EventPC, SN_NOT_USER, SILT_MAKE_BY_SCRIPT, 2);
		::MoveEmptyInv(&item, ch);
		sYES = 1;
		return;
	}
	sYES = 0;
}

// 021106 kyo
int SetRareItemInvPc(const int cn, const int iItemNo, const int iGradeMin, const int iGradeMax, const int iTryMulti, const int iDur, const int iHigh)
{
	if (iHigh != 1 && iHigh != 0)
		return 0;

	int a, b, c;
	CHARLIST *ch = CheckServerId(cn);//&connections[cn].chrlst;
	if (!ch) return -1;

	RareMain RareAttr;
	ItemAttr  Item = ItemMgr.GiveRareItem(iItemNo, iDur, RARE_ABLE, iTryMulti, iGradeMin, iGradeMax, RareAttr, iHigh);//020725 lsw

	if (Item.item_no)
	{
		if (SearchInv(ch->inv, a, b, c))
		{
			ch->inv[a][b][c] = Item;
			POS pos;
			SetItemPos(INV, a, b, c, &pos);
			SendServerEachItem(&pos, &Item, cn);		// 001028 YGI
			return 1;
		}
	}
	return 0;
}

extern int g_LocalWarBegin;										// LTS LOCALWAR	
extern cNation* g_pNation;
extern cLocalWarfield* g_pLocalWarfield;						// LTS NEW LOCALWAR

extern bool isNationManageServer();
extern void SendPacket2NWManager(t_packet* p);

void sLocalWarMode()
{
	MyLog(0, "LocalWarMode Call LocalWar Status : %d, User Name : %s, JoinLocalWar : %d", g_LocalWarBegin, connections[EventPC].name, connections[EventPC].chrlst.JoinLocalWar);
	if (g_LocalWarBegin&&connections[EventPC].chrlst.JoinLocalWar) sYES = 1;
	else sYES = 0;
}

void sFameCheck2()
{
	int Type = GetInt();
	int Value = GetInt();

	switch (Type)
	{
	case 1:
		if (connections[EventPC].chrlst.fame >= Value) sYES = 1;
		else sYES = 0;
		break;
	case 2:
		if (connections[EventPC].chrlst.GainedFame >= Value) sYES = 1;
		else sYES = 0;
		break;
	}
}

void sFameUp2()
{
	int Type = GetInt();
	int Value = GetInt();

	switch (Type)
	{
	case 1:
		g_pLogManager->SaveLogChange_Fame(&connections[EventPC].chrlst, connections[EventPC].chrlst.fame, connections[EventPC].chrlst.fame + Value, LF_SCRIPT);		// 020909 LTS
		connections[EventPC].chrlst.fame += Value;		break;
	case 2:
		connections[EventPC].chrlst.GainedFame += Value;	break;
	}
}

void sNationBelong()
{
	int Nation = connections[EventPC].chrlst.name_status.nation;
	if (Nation == NW_YL)
		Nation = connections[EventPC].chrlst.NWCharacter.YL_JoinNation;

	switch (Nation)
	{
	case NW_BY: sYES = 1; break;
	case NW_ZY: sYES = 2; break;
	default: sYES = 3; break;
	}
}

void FameUp2(const int cn, const int Type, const int Value)	// 021107 kyo
{
	switch (Type)
	{
	case 1:
		g_pLogManager->SaveLogChange_Fame(&connections[cn].chrlst, connections[cn].chrlst.fame, connections[cn].chrlst.fame + Value, LF_SCRIPT);		// 020909 LTS
		connections[cn].chrlst.fame += Value;		break;
	case 2:
		connections[cn].chrlst.GainedFame += Value;	break;
	}
}

void sFameDown2()
{
	int Type = GetInt();
	int Value = GetInt();

	switch (Type)
	{
	case 1:
		g_pLogManager->SaveLogChange_Fame(&connections[EventPC].chrlst, connections[EventPC].chrlst.fame, connections[EventPC].chrlst.fame - Value, LF_SCRIPT);		// 020909 LTS
		connections[EventPC].chrlst.fame -= Value;		break;
	case 2: connections[EventPC].chrlst.GainedFame -= Value;	break;
	}
}


void sLocalWarPointUp()
{
	t_packet packet;

	int NationIndex = GetInt();
	int Value = GetInt();

	switch (NationIndex)
	{
	case 3: NationIndex = 0; break;
	case 4: NationIndex = 1; break;
	case 6: NationIndex = 2; break;
	}

	if (isNationManageServer())
	{
		g_pNation->IncLocalWarPoint(NationIndex, Value);
	}
	else
	{
		packet.h.header.type = CMD_LOCALWAR_NATION_POINT_CHANGED;
		switch (NationIndex)
		{
		case 0:
			packet.u.LocalWar.NationPointChanged.NationPoint[0] = Value;
			packet.u.LocalWar.NationPointChanged.NationPoint[1] = 0;
			packet.u.LocalWar.NationPointChanged.NationPoint[2] = 0;
			break;
		case 1:
			packet.u.LocalWar.NationPointChanged.NationPoint[0] = 0;
			packet.u.LocalWar.NationPointChanged.NationPoint[1] = Value;
			packet.u.LocalWar.NationPointChanged.NationPoint[2] = 0;
			break;
		case 2:
			packet.u.LocalWar.NationPointChanged.NationPoint[0] = 0;
			packet.u.LocalWar.NationPointChanged.NationPoint[1] = 0;
			packet.u.LocalWar.NationPointChanged.NationPoint[2] = Value;
			break;
		}
		packet.h.header.size = sizeof(t_NationPointChanged);
		SendPacket2NWManager(&packet);
	}
}

void sLocalWarPointDown()
{
	t_packet packet;

	int NationIndex = GetInt();
	int Value = GetInt();

	Value *= -1;	//음수로 만들어 준다.

	switch (NationIndex)
	{
	case 3: NationIndex = 0; break;
	case 4: NationIndex = 1; break;
	case 6: NationIndex = 2;	break;
	}

	if (isNationManageServer())
	{
		g_pNation->IncLocalWarPoint(NationIndex, Value);
	}
	else
	{
		packet.h.header.type = CMD_LOCALWAR_NATION_POINT_CHANGED;
		switch (NationIndex)
		{
		case 0:
			packet.u.LocalWar.NationPointChanged.NationPoint[0] = Value;
			packet.u.LocalWar.NationPointChanged.NationPoint[1] = 0;
			packet.u.LocalWar.NationPointChanged.NationPoint[2] = 0;
		case 1:
			packet.u.LocalWar.NationPointChanged.NationPoint[0] = 0;
			packet.u.LocalWar.NationPointChanged.NationPoint[1] = Value;
			packet.u.LocalWar.NationPointChanged.NationPoint[2] = 0;
		case 2:
			packet.u.LocalWar.NationPointChanged.NationPoint[0] = 0;
			packet.u.LocalWar.NationPointChanged.NationPoint[1] = 0;
			packet.u.LocalWar.NationPointChanged.NationPoint[2] = Value;
		}
		packet.h.header.size = sizeof(t_NationPointChanged);
		SendPacket2NWManager(&packet);
	}
}

//extern t_Warfield	g_pWarfieldStatus[NW_NATION_COUNT];
//extern cWarfield*	g_pWarfield;
//extern bool isNationWarfieldServer();
//
//
//void sWarfieldMany()				// 011213 LTS		// 허접
//{
//	int PoseNo = GetInt();
//	int BYPose = 0, ZYPose = 0;
//	int Nation;
//
//	if (!isNationWarfieldServer()) { sYES = 0; return; }
//
//	if (g_pWarfieldStatus[0].Possession == NW_BY) BYPose++;
//	else ZYPose++;
//	if (g_pWarfieldStatus[1].Possession == NW_BY) BYPose++;
//	else ZYPose++;
//	if (g_pWarfieldStatus[2].Possession == NW_BY) BYPose++;
//	else ZYPose++;
//
//	Nation = g_pWarfieldStatus[g_pWarfield->GetWarfieldCode()].Possession;
//	if (Nation == NW_BY)
//	{
//		if (BYPose >= PoseNo) sYES = 1;
//		else sYES = 0;
//	}
//	else
//	{
//		if (ZYPose >= PoseNo) sYES = 1;
//		else sYES = 0;
//	}
//}
//
//void sWarfieldOwner()
//{
//	int WarfieldNo = GetInt();
//	int Nation = GetInt();
//
//	if (Nation == g_pWarfieldStatus[WarfieldNo].Possession) sYES = 1;
//	else sYES = 0;
//}

void sSetOXSelect()
{
	int type = GetInt();

	CHARLIST *ch = CheckServerId(EventPC);

	if (!ch) return;
	SetOXSelect(ch, type);
}

// 020620 YGI-------------------------
extern void SendCheckGotoScenarioDungeon(short int cn, char *map_name, int x, int y);
void sMoveDungeon()
{
	char map_name[20];
	GetString(map_name);
	int x = GetInt();
	int y = GetInt();
	SendCheckGotoScenarioDungeon(EventPC, map_name, x, y);
}
//-------------------------------------

// 020707 YGI
void sDualCheck()
{
	int type = GetInt();

	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	sYES = (type == ch->GetClassStep()) ? 1 : 0;
}

// 엠포리아로 이동
void sGotoGuildHouse()					// 김영주
{	//< CSD-030324
	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	sYES = g_GuildHouse.GoToHouse(ch);

	if (sYES)
	{
		t_packet packet;
		packet.h.header.type = CMD_GOTO_GUILD_HOUSE_RESULT;
		packet.u.LocalWar.CommonDataDw.Data = ch->GetGuildCode();
		packet.h.header.size = sizeof(t_CommonDataDw);
		QueuePacket(connections, ch->GetServerID(), &packet, 1);
	}
}	//> CSD-030324

// 자기 길드 집인지 확인
void sIsMyGuildHouse()			//김영주
{
	int event_no = GetInt();
	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	sYES = g_GuildHouse.IsMyGuildHouse(ch, event_no);

	CGuildHouseUnit *house = g_GuildHouse.GetPointByEventNo(MapName, event_no);
	if (!sYES)
	{
		if (house) house->GoToHouse(ch->GetServerID());
	}

	t_packet packet;
	packet.h.header.type = CMD_CHECK_GUILD_HOUSE_RESULT;
	if (house) packet.u.LocalWar.CommonDataDw.Data = house->guild_code;
	else packet.u.LocalWar.CommonDataDw.Data = 0;
	packet.h.header.size = sizeof(t_CommonDataDw);
	QueuePacket(connections, ch->GetServerID(), &packet, 1);
}

extern int g_EventLocalWarStatus;
extern int g_EventLocalWarStep;

void sCheckEventLocalWarStatus()			// 020115 LTS
{
	sYES = g_EventLocalWarStatus;
}

void sCheckEventLocalWarStep()			// 020115 LTS
{
	sYES = g_EventLocalWarStep;
}

void sCheckEventJoin()					// 020115 LTS	//이벤트 번호를 넣으면 참인지 아닌지를 리턴
{
	int EventNo = GetInt();
	switch (EventNo)
	{
	case 1: sYES = connections[EventPC].chrlst.EventJoin.Event1; break;
	case 2: sYES = connections[EventPC].chrlst.EventJoin.Event2; break;
	case 3: sYES = connections[EventPC].chrlst.EventJoin.Event3; break;
	case 4: sYES = connections[EventPC].chrlst.EventJoin.Event4; break;
	case 5: sYES = connections[EventPC].chrlst.EventJoin.Event5; break;
	case 6: sYES = connections[EventPC].chrlst.EventJoin.Event6; break;
	case 7: sYES = connections[EventPC].chrlst.EventJoin.Event7; break;
	default: sYES = connections[EventPC].chrlst.EventJoin.Event8; break;
	}
}
void sClearEventJoin()
{
	int EventNo = GetInt();
	t_packet packet;
	packet.h.header.type = CMD_DELETE_EVENT_JOIN;
	strcpy(packet.u.LocalWar.DeleteEventJoin.UserName, connections[EventPC].name);
	packet.u.LocalWar.DeleteEventJoin.EventNo = EventNo;
	packet.h.header.size = sizeof(t_DeleteEventJoin);
	QueuePacket(connections, DB_DEMON, &packet, 1);

	switch (EventNo)
	{
	case 1:	connections[EventPC].chrlst.EventJoin.Event1 = 0;	break;
	case 2:	connections[EventPC].chrlst.EventJoin.Event2 = 0; break;
	case 3:	connections[EventPC].chrlst.EventJoin.Event3 = 0; break;
	case 4:	connections[EventPC].chrlst.EventJoin.Event4 = 0; break;
	case 5:	connections[EventPC].chrlst.EventJoin.Event5 = 0; break;
	case 6:	connections[EventPC].chrlst.EventJoin.Event6 = 0; break;
	case 7:	connections[EventPC].chrlst.EventJoin.Event7 = 0; break;
	case 8:	connections[EventPC].chrlst.EventJoin.Event8 = 0; break;
	}
}

extern int	 GetNationIndex(int Nation);			// LTS NEW LOCALWAR

void sCheckAllSealStoneBroked()						// LTS NEW LOCALWAR
{
	int Nation = GetInt();

	if (!g_pLocalWarfield)
	{
		MyLog(0, " Scrip Call Error!! CheckAllSealStoneBroked()");
		sYES = 0;
	}
	sYES = g_pLocalWarfield->CheckAllSealStonebroked(GetNationIndex(Nation));
}

void sSetFame()										// LTS NEW LOCALWAR
{
	CHARLIST *ch = CheckServerId(EventPC);
	int sFame = GetInt();
	if (!ch) return;
	MyLog(0, "User Fame Setting Changed By Script......");
	MyLog(0, "UserName : %s, OldFame : %d, NewFame : %d", ch->Name, ch->fame, sFame);
	ch->fame = sFame;
}

void sCheckQuestMode()								// LTS DRAGON MODIFY
{
	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch)
	{
		sYES = 0;
		return;
	}
	sYES = ch->QuestMode;
	MyLog(0, "%d User Set Quest Mode : %d", ch->GetServerID(), sYES);
}


void sResetCombatSkill()	//89 // 021010	kyo
{
	g_pBattleManager->RecvCombatReset(EventPC);
}

void sSpellMapMove()	//90 // 021014 kyo
{
	char szSpell[128];
	char szMapName[20];
	GetString(szSpell);
	GetString(szMapName);
	int iMap_x = GetInt();
	int iMap_y = GetInt();

	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	// 021128 kyo 맵서버에 부하를 준단다.
	//sYES =  g_QuestInMap.CheckSpell( EventPC, szSpell );
	////int port = GetMapPortByName( dungeon_name );
	//if( sYES == 1)
	//{//맵이동한다.
	//sYES = MapMove( EventPC, szMapName, iMap_x, iMap_y );
	//}

	g_QuestInMap.SendSpellMapMove(EventPC, szSpell, szMapName, iMap_x, iMap_y);  // 021128 kyo
}

void sShowStateMessage()		//91 //021015 kyo
{
	GetString(String);
	int iR = GetInt();
	int iG = GetInt();
	int iB = GetInt();

	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	g_QuestInMap.ShowStateMsg(EventPC, String, iR, iG, iB);
}


int SearchItemMuch(const int& iItemNum, const int& cn)
{
	CHARLIST *ch = CheckServerId(cn);
	if (!ch) return 0;

	int iRet = 0;
	//inv
	for (int a = 0; a < 3; a++) {
		for (int b = 0; b < 3; b++) {
			for (int c = 0; c < 8; c++) {
				if (ch->inv[a][b][c].item_no == iItemNum)
					iRet++;
			}  // for
		}  // for
	}  // for 

	////equip
	//for( a=0; a<8; a++ )
	//if( ch->equip[a].item_no == iItemNum ) 
	//iRet++;
	//
	////quick
	//for( a=0; a<6; a++ )
	//if( ch->quick[a].item_no == iItemNum ) 
	//iRet++;

	return iRet;
}

bool DeleteItemMuch(const int iItemNum, const int iNumMuch, const int cn)
{
	CHARLIST *ch = CheckServerId(cn);
	if (!ch) return false;

	int iCheckNum = 0;

	//지우려는 아이템의 갯수가 가진거보다 적어야지
	if (iNumMuch > SearchItemMuch(iItemNum, cn)) return false;

	//아이템 삭제는 기존의 setiteminvPc 그대로
	//inv
	for (int a = 0; a < 3; a++) {
		for (int b = 0; b < 3; b++) {
			for (int c = 0; c < 8; c++) {
				if (ch->inv[a][b][c].item_no == iItemNum) {
					ItemAttr target = GenerateItem(iItemNum, 0, 0);
					SendItemEventLog(&target, cn, SN_NOT_USER, SILT_MAKE_BY_SCRIPT, 2);
					ch->inv[a][b][c] = target;
					POS pos;
					SetItemPos(INV, a, b, c, &pos);
					SendServerEachItem(&pos, &target, cn);		// 001028 YGI
					iCheckNum++;
					if (iCheckNum >= iNumMuch)
						return true;
				}
			}
		}
	}

	////equip
	//for (a = 0; a<8; a++)
	//	if (ch->equip[a].item_no == iItemNum)
	//	{
	//		ItemAttr target = GenerateItem(iItemNum, 0, 0);
	//		SendItemEventLog(&target, cn, SN_NOT_USER, SILT_MAKE_BY_SCRIPT, 2);
	//		ch->equip[a] = target;
	//		POS pos;
	//		SetItemPos(EQUIP, a, &pos);
	//		SendServerEachItem(&pos, &target, cn);		// 001028 YGI
	//		iCheckNum++;
	//		if (iCheckNum >= iNumMuch)
	//			return true;
	//	}

	////quick
	//for (a = 0; a<6; a++)
	//	if (ch->quick[a].item_no == iItemNum)
	//	{
	//		ItemAttr target = GenerateItem(iItemNum, 0, 0);
	//		SendItemEventLog(&target, cn, SN_NOT_USER, SILT_MAKE_BY_SCRIPT, 2);
	//		ch->quick[a] = target;
	//		POS pos;
	//		SetItemPos(QUICK, a, &pos);
	//		SendServerEachItem(&pos, &target, cn);		// 001028 YGI
	//		iCheckNum++;
	//		if (iCheckNum >= iNumMuch)
	//			return true;
	//	}

	if (iCheckNum >= iNumMuch)
		return true;
	else
		return false;
}


// 021117 YGI
void sCheckItemMulti()							//92 여러개의 아이템이 있는가?
{
	int iItemNumber = GetInt();
	int iNumMuch = GetInt();

	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	const int iCurNum = SearchItemMuch(iItemNumber, EventPC);

	if (iNumMuch > iCurNum)		sYES = 0;
	else if (iNumMuch <= iCurNum)	sYES = 1; // 021121 kyo
	else if (iNumMuch < iCurNum)	sYES = 2;
}

void sDeleteItemMulti()							//93 여러개의 아이템을 지우기
{
	int iItemNumber = GetInt();
	int iDelMuch = GetInt();

	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	if (DeleteItemMuch(iItemNumber, iDelMuch, EventPC) == true)
		sYES = 1;
	else
		sYES = 0;
}

void sScriptTimer()								//94 // 021020 kyo //초로 시간을 계산한다. 
{//0이면 타이머를 멈춰라
	//한 클라이언트당 하나만의 타이머가 주어진다.
	DWORD dwTime;

	int iTime = GetInt();
	char mapfile[30];
	GetString(mapfile);
	int x = GetInt();
	int y = GetInt();

	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	if (iTime > 0)
		dwTime = iTime * 1000;	//1000이 1초
	else
		dwTime = 0;

	//타이머의 시작 타이머를 발동시킬때의 cn과 name를 기록시킨다. 
	if (g_QuestInMap.StartScriptTimer(EventPC, ch->Name, dwTime, MIN_SYNCH_TIME, mapfile, x, y))
		sYES = 1;
	else
		sYES = 0;

}

void sScriptCounter()				//95 // 021023 kyo 
{	//(숫자셀꺼 번호, 종류, 갯수, 플러그)
	//종류: 0=ncp,1=아이템,2=미정. 
	//플러그: 0=멈춤,1=시작,2=갯수랑 일치
	int iNumber = GetInt();
	int iType = GetInt();
	int iMuch = GetInt();
	int iFlag = GetInt();

	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	if (g_QuestInMap.StartScriptCounter(EventPC, ch->Name, iNumber, iType, iMuch, iFlag))
		sYES = 1;
	else
		sYES = 0;
}

// 021117 YGI
void sUpRareInEquip()			//96 // 021028 kyo
{	// iii
	const int iEquipPos = GetInt();
	const int iMulti = GetInt();
	const int iMin = GetInt();
	const int iMax = GetInt();
	const int iMutant = GetInt();

	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	//차례
	// 장착 되어 있는 아이템 번호를 얻어 온다.
	// 그 아이템 번호를 기초로 레어 아이템을 만든다.
	// 인벤토리에 넣어 준다.

	if (WT_WEAPON > iEquipPos || iEquipPos > WT_UNIQUE2)
	{//위치 에러
		sYES = 0;
		return;
	}
	int item_no = ch->equip[iEquipPos].item_no;
	if (!item_no)
	{// 아이템을 안 들고 있다.
		sYES = 0;
		return;
	}

	int grade_gab = iMax - iMin + 1;

	if (0 >= grade_gab)
	{
		sYES = 0;
		return;
	}

	ItemAttr item = { 0, };

	int ret = ItemMgr.MakeDefaultRareItem(&item, item_no, iMutant, rand() % grade_gab + iMin, iMulti);

	if (ret == 1)		// 성공
	{
		SendItemEventLog(&item, EventPC, SN_NOT_USER, SILT_MAKE_BY_SCRIPT, 2);
		MoveEmptyInv(&item, ch);
	}
	else
	{
		sYES = 2;
		return;
	}

	sYES = 1;
}

void sBossTimerCheck()		//97 // 021106 kyo
{//iiiiii (npc번호,x,y, 인원제한, 타이머시간, 제약시간)
	const int iNPC = GetInt();
	const int iNpc_x = GetInt();
	const int iNpc_y = GetInt();
	const int iMaxEntree = GetInt();//Entree limit 
	const int iTime = 1000 * GetInt();
	const int iLimiteTime = 1000 * GetInt();	// 030123 kyo

	const int nFlag = GetInt();	// 031028 kyo

	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	int nKey = iNpc_x * 1000 + iNpc_y;

	//flag 0 -> 보스 한명 등장
	//flag 1 -> 보스 여럿 등장
	//flag 2 -> 보스방에서 나오고 다 나오면 방 제거
	if (nFlag == CBossTimer::TYPE_BOSS_OUT)
	{
		g_QuestInMap.DecreaseBossTimerUser(nKey, EventPC);
		g_QuestInMap.CheckBossTimerBoss(nKey);
		sYES = -1;
		return;
	}

	if (g_QuestInMap.GetBossTimerEntree(nKey) > iMaxEntree)
	{
		sYES = 0;
		return;
	}

	if (g_QuestInMap.GetCurrentBossTimer(nKey) <= (iTime - iLimiteTime))	//타이머 없으면 0, 있으면 타이머시간(dwtime)
	{
		sYES = (g_QuestInMap.StartBossTimer(EventPC, iNPC, iNpc_x, iNpc_y, iTime, iMaxEntree, nFlag)) / 1000; //031028 kyo
	}
	else //제약시간안에 있다.
	{
		sYES = 1;
	}
}

void sIsGetMagic()
{// 030415 kyo 
	const int nMagic = GetInt();
	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;
	switch (ch->Spell)
	{
	case WIZARD_SPELL:
	{
		if (ch->Ws[nMagic])
		{
			sYES = 1;
		}
		break;
	}
	case PRIEST_SPELL:
	{
		if (ch->Ws[nMagic])
		{
			sYES = 1;
		}
		break;
	}
	default:
	{
		sYES = 0;
		break;
	}
	}
}

void sDualClassCheck()
{ //100 내가 해당 듀얼클래스인가 맞으면 1 틀리면 0
	const int n1Class = GetInt();
	const int n2Class = GetInt();
	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;
	sYES = 0; // 030513 kyo
	if (ch->Class == n1Class || ch->Class >= 5)
	{
		if (ch->GetDualClass() == n2Class)
		{
			sYES = 1;
		}
	}
	else {
		sYES = 0;
	}
}

void sUpDualClassItem()
{ //101 듀얼클래스아이템을 한단계올려준다.

	const int nFlag = GetInt();
	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	if (0 == nFlag)
	{
		sYES = g_CSymbolMgr.GetSymbolGrade(ch);
	}
	else if (1 == nFlag)
	{

		if (!g_CSymbolMgr.DoHaveSymbol(ch))
		{
			g_CSymbolMgr.MakeSymbolItem(ch);
			sYES = 0;
		}
		else
		{
			int nDualStep = g_CSymbolMgr.GetSymbolGrade(ch);
			sYES = nDualStep;
			g_CSymbolMgr.UpGradeSymbol(ch, ++nDualStep);
		}
	}

}

void sShowLearnDualMagic()
{// 102 듀얼마법을 배울수 있는 창을 띄운다.
	::SendCallDualMagicLearnMenu(EventPC);
}

void sLogDualQuest()
{// 103 iisi 듀얼퀘스트만 로그를 남긴다.
	const int nQnum = GetInt();	//퀘스트 번호
	const int nQCount = GetInt();	//수행횟수.
	char szMsg[128];
	GetString(szMsg);	//메시지 내용
	const int nStart = GetInt();	//시작인지 아닌지

	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;
	//< CSD-TW-030618
	//::LogInFile("./changelog/DualQuestLog.txt", "chName=%s, QuestNum=%d, Qcount=%d, Msg=%s, Start=%d", ch->Name, nQnum, nQCount, szMsg, nStart);
	g_pLogManager->SaveLogChange_ClassQuest(ch, szMsg, nQnum, nQCount, nStart);
}	//> CSD-TW-030618

void sLogForScript()
{// 104 is 스크립트를 수행한 로그를 남긴다.  // 030826 kyo
	const int nQnum = GetInt();
	char szMsg[128];
	GetString(szMsg);

	if (0 > nQnum || _SCRIPT_VARABLE_POINT_ <= nQnum)
	{
		return;
	}
	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	g_pLogManager->SaveLogScriptLog(ch, nQnum, var[EventPC][nQnum], szMsg);//kyo
}

//<! BBD 040308	이벤트 아이템 지급을 해주는 함수
void sEventItemInsert()
{
	GetString(String);

	CHARLIST *ch = CheckServerId(EventPC);

	if (connections[EventPC].dwAgentConnectionIndex && connections[EventPC].state >= CONNECT_JOIN)
	{
		//SendBBS2라는 이벤트 메세지 발생 함수를 실행한당.
		SendBBS2(String, (short)EventPC);

		if (!ch->IsEventRecv())
		{
			// 인벤의 빈칸 세기
			int blankcount = 0;
			for (int a = 0; a < 3; a++) {
				for (int b = 0; b < 3; b++) {
					for (int c = 0; c < 8; c++) {
						if (!ch->inv[a][b][c].item_no)
						{
							blankcount++;
						} // if
					} // for
				} // for
			} // for

			if (blankcount < 10)	// 10칸 이상인가?
			{
				::OutMessage(ch, 2, 13);//임시,,인벤토리 공간이 충분치 않습니다.
				return;
			}

			t_packet	p;
			p.h.header.type = CMD_EVENTRAREITEM_REQ;
			p.h.header.size = sizeof(t_EVENT_RARE_ITEM_LIST);

			p.u.Event_Item_List.nCn = EventPC;
			strcpy(p.u.Event_Item_List.szChrName, ch->Name);
			strcpy(p.u.Event_Item_List.szLoginID, connections[EventPC].id);

			QueuePacket(connections, DB_DEMON, &p, 1);

			ch->SetEventRecv(true);
		}
	}
}
//> BBD 040308	이벤트 아이템 지급을 해주는 함수

//<! kjy 040406  캐릭터 능력치 가져오는 함수
void sGetCharacterStatus()
{
	int type = GetInt();
	int type2 = GetInt();
	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	sYES = 0;
	switch (type)
	{
	case STR:	sYES = ch->Str; break;
	case CON:	sYES = ch->Con; break;
	case DEX:	sYES = ch->Dex; break;
	case WIS:	sYES = ch->Wis; break;
	case INT_:	sYES = ch->Int; break;
	case MOVP:	sYES = ch->MoveP; break;
	case CHA:	sYES = ch->Char; break;
	case ENDU:	sYES = ch->Endu; break;
	case MOR:	sYES = ch->Moral; break;
	case LUCK:	sYES = ch->Luck; break;
	case WSPS:	sYES = ch->wsps; break;
	}

	if (sYES < type2)
	{
		sYES = 0;
	}
	else if (sYES >= type2)
	{
		sYES = 1;
	}
}
//> kjy 040406  캐릭터 능력치 가져오는 함수


//<! kjy 040406  레어 아이템 개수 확인 함수
void sSearchItemMuchForRare()
{
	int item_number = GetInt();
	int rare_type = GetInt();
	int grade = GetInt();
	int tt = GetInt();

	CHARLIST *ch = CheckServerId(EventPC);
	if (!ch) return;

	int iRet = 0;

	// 베낭 속의 모든 아이템 검색..
	for (int a = 0; a < 3; a++) {
		for (int b = 0; b < 3; b++) {
			for (int c = 0; c < 8; c++) {
				ItemAttr *pItem = &ch->inv[a][b][c];
				if (pItem->item_no == item_number)
				{
					RareMain *pRare = (RareMain *)&pItem->attr[IATTR_RARE_MAIN];

					if (pRare->soksung1 == rare_type && pRare->grade == grade)
					{
						iRet++;
					}
				} // if
			} // for
		} // for
	} // for

	if (iRet < tt)		sYES = 0;
	else if (iRet >= tt)	sYES = 1;
}
//> kjy 040406  레어 아이템 개수 확인 함수

// 040601 YGI
void sGetItemMallItem()
{
	//	CHARLIST *ch = CheckServerId( EventPC );
	//	if( !ch ) return;

		//050224_KCH 마일리지몰 작업 이부분에서 홈페이에서 산 아이템중에 몇몇을 인터페이스창을 통해서 받게하자.
		//구입한 전체마일리지 목록(길드창고,은행창고식)에서 유저가 받을 아이템 목록만을 요청해서 받도록 하자.
		//(ItemMall IDX를 보내서 받아오면 되겠다.)

	//	int menu = GetInt(); 
	SendCallSmallMenu(EventPC, 0, MN_ITEM_MALL_ACCOUNT_BOX_GET);

	//	RecvGetItemMalltoDB( EventPC );
}

void(*SFunction[_SCRIPT_FUNCTION_POINT_])(void) =
{
	sDialOnlyPC,			//  0
	sDialInAreaPC,
	sDialEveryone,
	sAddItem,				//  4 //특정 위치에 아이템 갖다 놓기 (x좌표 * 32, y좌표 *32)
	sAddNpc,				//  5 //특정 위치에 NPC 갖다 놓기	(x좌표 , y좌표)
	sMapName,
	sMapMove,				//  7 
	sIsLevel,
	sInitMonsterGeneratedPosition,
	sCallSmallMenu,
	sWeatherOn,				// 10 // 날씨에 관해 BroadCast한다. 
	sWeatherOff,			// 11 // 날씨에 관해 BroadCast하지 않는다.  
	sRecallScriptStart,		// 12
	sRecallScriptEnd,		// 13
	sMaxMonsterGeneration,	// 14
	sLoadItemValueOneMaul,  // 15 //031116 lsw 제거된 함수.
	srandom,				// 16 // 0131 YGI
	sEventMsg,				// 17 //이벤트 메세지를 뿌려준다.
	sIsItemHavePC,			// 18 // 0720 LHS 특정 아이템을 가지고 있는지 확인한다. 장착에 관해서
	sIsClassPC,				// 19 // 0721 LHS 특정 클래서인가? i
	sSetQuestNumStep,		// 20 // 0721 LHS 퀘스트 번호를 설정한다. ii
	sCmpQuestNumStep,		// 21 // 0722 LHS 같은 퀘스트 번호에서 진행 단계를 확인한다. 0 낮은값 1 같은값 2 높은값 ii
	sQuestComplete,			// 22 // 0722 LHS 퀘스트 완료시 축포빠방~~ i
	sQuestTeleport,			// 23 // 0726 LHS 퀘스트텔레포트 iii
	sIsJobPC,				// 24 // 0728 LHS 해당 직업인가? i	
	sIsInvHavePC,			// 25 // 0728 LHS 퀵인벤이큅에 해당 아이템이 있는가? i
	sSetItemInvPC,			// 26 // 0729 LHS 인벤토리창에 해당 아이템을 다른 아이템으로 변경한다.(바뀔아템,바꿀아템,아템듀) iii
	sIsSkillHavePC,			// 27 // 0729 LHS 스킬을 배웠는가? i
	sSetSkillPC,			// 28 // 0729 LHS 스킬을 배우게 한다. 안배운거로 한다. ii(스킬,배움/안배움)	
	sQuestSound,			// 29 // 0809 LHS 퀘스트용 사운드 i
	sAddFightMap,			// 30 // 0905 YGI 전투맵에서 팀 등록 i
	sIsPlayArenaGame,		// 31 // CSD-030517 : 아레나 경기 중인지 여부 검사
	sIsFullFightMapTeam,	// 32 // 0907 YGI 전투맵에서 팀원 모두 등록한 상태인지 아닌지 i( 팀 번호 )
	sDeleteFightMap,		// 33 // 0915 YGI 전투맵에서 팀 해제 하기 
	sCheckNation,
	sReturnSealStoneBreakable,	// 35 // 001026 KHS sYES == 1 이동가능, sYES == 0 이동불가.
	// Added by chan78
	sSendMessageBox,			// 36 // 001124 YGI  // 말풍선이 뜨게 한다.		// 001212 YGI
	sSendSound,					// 37 // 001205 YGI		// 특정 사운드 번호의 사운드를 출력하게 한다. 001212 YGI
	sGenMonSpec,				// 38 // 010129 KHS
	sGetGender,					// 39 // 010205 YGI				// 010210 YGI

	// 010303 YGI
	sSaveItemAtDb,				// 40 // 특정 아이템을 디비에 저장한다.
	sCheckMoney,				// 41 // 돈이 얼마나 있는지 알아본다.
	sSubtractMoney,				// 42 // pc한테서 돈을 빼낸다.
	sGetGuild,					// 43 // 길드 코드를 가져온다. sYES 에 저장된다.		// 010511 YGI
	sSendEffect,				// 44	// effect 보여주기 010522 YGI

	sCallMenu,					// 45	직접 디파인된 번호로 메뉴를 호출한다.		// 010528 YGI
	sGetNationByMap,			// 46	특정 맵의 국적알아 오기
	sCalcFriendShip,			// 47	특정 국가의 적대수치 더해주기
	sCompFriendShip,			// 48	적대수치 비교하기
	sCompLadderScore,			// 49	래더 점수 비교					// 010531			YGI
	sCheckLoadingPoint,			// 50	부대의 공격거점을 확인			// 010904 LTS
	sCheckDefence,				// 51	방어자 공격자만 이동가능 한 포탈..		// 010904 LTS
	sFameDown,					// 52   캐릭터의 페임을 낮춘다			// 010915 LTS
	sFameUp,					// 53	캐릭터의 페임을 높인다
	sFameCheck,					// 54	캐릭터의 페임이 주어진 값이상인지 판별판다
	sCanGoBonus,				// 55	// 국가전 보너스 맵으로 갈수 있는 기간인지 확인한다.
	// CSB AddCode
	sGetLevel,					// 56
	sGetClass,					// 57
	sGetAbil,					// 58
	sCheckParty,				// 59	
	sGetNat,					// 60
	sGetSpell,					// 61
	sCheckPartyCount,			// 62
	sCheckLastMagic,			// 63
	sSetTimer,					// 64
	sGetTactic,					// 65
	sCheckTacticPoint,			// 66
	sIsDual,					// 67
	sStopTimer,					// 68
	sCallCustomSmallMenu,		// 69

	sSetRareItemInvPc,			// 70
	sLocalWarMode,				// 71		// LTS LOCALWAR
	sFameCheck2,				// 72
	sFameUp2,					// 73
	sFameDown2,					// 74
	sLocalWarPointUp,			// 75
	sLocalWarPointDown,			// 76
	// CSB AddCode End
	// 추가시 아래 맥스 값을 늘려준다.
	sCheckEventLocalWarStatus,	// 77		// 020115 LTS
	sCheckEventLocalWarStep,	// 78		// 020115 LTS
	sCheckEventJoin,			// 79		// 020115 LTS
	sClearEventJoin,			// 80		// 020115 LTS

	sSetOXSelect,				// 81		// YGI
	sGotoGuildHouse,			// 82	
	sIsMyGuildHouse,			// 83		// i ( 이벤트 번호 )
	sCheckAllSealStoneBroked,	// 84		// LTS NEW LOCALWAR
	sSetFame,					// 85		// LTS NEW LOCALWAR
	sCheckQuestMode,			// 86		// LTS DRAGON MODIFY
	// 020620 YGI
	sMoveDungeon,				// 87		// sii
	sDualCheck,					// 88		// i		// 020707 YGI
	sResetCombatSkill,    // 89    //   // 021008 KYO
	sSpellMapMove,				//90		// ssii (주문, 지도,지도x,지도y) // 021014 kyo 주문을 외면 이동한다. 
	sShowStateMessage,			//91		// siii	(메시지, R,G,B )	//색에 맞는 글자를 보여준다.
	sCheckItemMulti,			//92 		// ii   다수의 아이템 확인 (아이템번호, 갯수)  yes값 실패 =0, 성공 =1		
	sDeleteItemMulti,			//93		// ii	다수의 아이템 삭세 (아이템번호, 갯수)  yes값 실패 =0, 성공 =1
	sScriptTimer,				//94		// isii	(타이머 설정 시간, 맵이름,맵x,맵y) //설정 시간: 0=해제, 0!=시간
	sScriptCounter,				//95		// iiii (숫자셀꺼 번호, 종류, 갯수, 플러그) 종류: 0=ncp,1=아이템,2=안정해따. 플러그: 0=멈춤,1=시작,2=갯수랑 일치?
	sUpRareInEquip,				//96		// iii	장착노말아이템을 레어 아템으로 바꾼다.: 속성만 올림 ( 장착위치, 등급, 속성) yes값 0=아템없다, 1=성공, 2=노말아템아님
	sBossTimerCheck,			//97		// iiiiii (npc번호,x,y, 인원제한, 타이머시간, 제약시간)
	// yes 0=인원초과, 1=제약시간안에 있다. 그외= 현재 타이머의 시간
	sNationBelong,        // 98   yes 1 : 바이서스 2:자이펀 3 : error
	sIsGetMagic,				//99		// i (마법번호) 마법을  배웠으면 1, 아니면 0 // 030415 kyo 
	sDualClassCheck,			//100		//ii 선행클래스 후행 클래스 선행클래스가 5이상이면 선행클래스는 무시한다.
								//0.전사 1,도둑 2. 궁 수 3. 마법사 4.  성직자
	sUpDualClassItem,				//101		//i 두얼아이템이 없는 사림은 주고 있는 사람은 단계를 올린다. syes값은 현재 유저의 상태를 말함
	sShowLearnDualMagic,		//102		//null 캐릭터가 배울수 잇는 마법책이 보여진다.
	sLogDualQuest,				//103		//iisi	qcount, 내용, 시작(0:시작, 1:끝) 
	sLogForScript,				//104		//is	퀘스트번호	msg	 //현재의 상태를 로깅한다. 
	sEventItemInsert,			//105		// BBD 040226	이벤트아이템 지급을 하는 함수
	sGetCharacterStatus,		//106		// kjy 040406   캐릭터의 능력치를 가져와서 값을비교  i = 0 : str, 1 : con, 2 : dex..., 10 : wsps, tt = 비교할 값 (가져온 능력치가 tt보다 작으면 syes값이 0, 크면 1, 같으면 2)
	sSearchItemMuchForRare,		//107		// kjy 040406   특정 Rare 아이템의 개수를 얻어와서 값을 비교 

	// 040601 YGI
	sGetItemMallItem,			//108		// YGI 040531	아이템 몰에서 아이템을 가져온다.
	sGetKing,					// 109 털뙤KING

	//MAX_SCRIPT_FUNCTION 에서 현재 맥스값 확인할껏. 
}; // 사용자 정의 함수들의 포인터
