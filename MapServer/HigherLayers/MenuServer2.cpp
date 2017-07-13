#include "..\stdafx.h"
#include "..\LowerLayers\servertable.h"
#include "..\LowerLayers\recvmsg.h"
#include "..\LowerLayers\mylog.h"
#include "DefaultHeader.h"
#include "MenuDefine.h"
#include "ItemList.h"
#include "CItem.h"
#include "Scrp_exe.h"
#include "SealStone.h"
#include "SealStonegameserver.h"
#include "op_magic.h"
#include "map.h"
#include "teambattle.h"
#include "LTSSupport.h"
#include "dungeon.h"
#include "eventmgr.h" 
#include "LogManager.h"
#include "ArenaManager.h"
#include "UserManager.h"
#include <direct.h>						//mkdir()
#include "ItemMallManager.h"			//050224_KCH 마일리지몰 작업
#include "..\HigherLayers/ItemMgr.h"	//ItemMgr.MakeRareAttr(...)

//莖샙
#define BEGIN_SELECT(e) switch (e) {
#define CASE_SELECT(e)  case(e): return CS(e);
#define END_SELECT(e)   } return CS(e);


void SendSaveEventItem( CHARLIST *ch, int item_no, int count )
{
	t_packet packet;
	packet.h.header.type = CMD_ADD_EVENT_ITEM;
	packet.u.kein.save_event_item.item_no = item_no;
	packet.u.kein.save_event_item.count = count;
	strcpy( packet.u.kein.save_event_item.name, ch->Name );
	packet.h.header.size = sizeof( k_save_event_item ) - 20 + strlen( ch->Name )+1 ;
	QueuePacket( connections, DB_DEMON, &packet, 1 );
}

void RecvOXSelect( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	SetOXSelect( ch,p->u.kein.default_char ); 
	return;
}

void SetOXSelect( CHARLIST *ch, int type )
{
	ch->ox_select = type;
}

void RecvOXAnser( t_packet *p, short int cn )
{	//< CSD-CN-031213
	const int type = p->u.kein.default_char;

	CUserManager::HASH_USER mpUser = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = mpUser.begin(); i != mpUser.end(); ++i)
	{
		const int idUser = i->second;

		if (idUser == cn)
		{
			continue;
		}

		CHARLIST* pUser = CheckServerId(idUser);
		
		if (pUser == NULL)
		{
			continue;
		}

		if (pUser->IsCounselor() || pUser->IsReporter())
		{
			continue;
		}

		if (pUser->ox_select == type) 
		{
			continue;
		}

		MapMove(idUser, "G_BATTLE", 140, 100);	// 030929 kyo
	}
}	//> CSD-CN-031213

// 이 맵에 있는 모든 유저를 지정한 곳으로 맵이동 시킨다.
// type == 1 이면 기자나, 운영자는 남긴다.
int UserAllMapMove( int type, char *map_name, int x, int y )
{	//< CSD-CN-031213
	if (strcmp(map_name, MapName) == 0)
	{
		return 0;
	}

	if (GetMapMoveType(map_name) == MMT_FAIL)
	{	// 없는 맵이다.
		return 0;
	}

	CUserManager::HASH_USER mpUser = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = mpUser.begin(); i != mpUser.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = CheckServerId(idUser);
		
		if (pUser != NULL)
		{
			if (type == 1)
			{
				if (pUser->IsCounselor() || pUser->IsReporter()) 
				{
					continue;
				}
			}

			MapMove(idUser, map_name, x, y);
		}
	}

	return 1;
}	//> CSD-CN-031213

void RecvCheckSkill( short int cn )
{
	CHARLIST *ch= CheckServerId( cn );
	if( !ch ) return;

	// 염색 기술을 배웠으면 돈을 준다.
	if( ch->Skill[46] )
	{
		ch->IncBankMoney(50000, BMCT_SKILL_RESET); // CSD-030723
		ch->Skill[46] = 0;
		SendPutMenuString( KM_BBS, 121, cn ); // 염색하기 스킬을 삭제했습니다.
	}	
}


void DeleteMenuData()
{

	// 종료시 프리시키는 곳..

}

void RecvPutGuildItem( t_packet *p, short int cn )
{	//< CSD-030324
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	POS pos = p->u.kein.default_pos;
	ItemAttr *pItem = GetItemByPOS( ch, pos );
	if( !pItem ) return;
	if( !pItem->item_no ) return;
	if( !ch->GetGuildCode() ) return;		// 길드가 없다.
	//if( ch->name_status.guild_master ) return;		// 길드내 등급 제약을 하려면 여기서

	// 040601 YGI
	if( GetAttr2( pItem->attr[IATTR_ATTR], IA2_ITEMMALL_ITEM ) )
	{
		// 아이템 몰 아이템은 길드 창고에 저장할 수 없다.
		return;
	}

	t_packet packet;
	packet.h.header.type = CMD_PUT_GUILD_ITEM;
		packet.u.kein.put_guild_item.item = *pItem;
		packet.u.kein.put_guild_item.guild_code = ch->GetGuildCode();
		strcpy( packet.u.kein.put_guild_item.name, ch->Name );
	packet.h.header.size = sizeof( k_put_guild_item );
	QueuePacket( connections, DB_DEMON, &packet, 1 );	// DB에 저장한다.

	//021030 YGI
	SendItemEventLog( pItem, cn, SN_NOT_USER, SILT_PUT_GUILD_BOX, 1 ); // 021018 YGI

	// 아이템을 지우고
	memset( pItem, 0, sizeof( ItemAttr ) );
	// 저장한 사실을 클라이언트에 보내준다.
	SendPutMenuString( KM_OK, 132, cn );		// 보관되었습니다.
}	//> CSD-030324

void RecvGetGuildItemClient( t_packet *p, short int cn )
{	//< CSD-030324
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

//	if( !ch->GetGuildCode() ) return;
//	int degree = ch->name_status.guild_master;
//	if( !degree || degree > 2 ) return;		// 1과 2만 사용 가능
	if( !CheckGuildPower( GDP_GUILD_BOX, ch ) ) return;

	DWORD index = p->u.kein.default_dword;
	t_packet packet;
	packet.h.header.type = CMD_GET_GUILD_ITEM_MAP;

		packet.u.kein.get_guild_item_map.server_id = cn;
		packet.u.kein.get_guild_item_map.guild_code = ch->GetGuildCode();
		packet.u.kein.get_guild_item_map.index = index;

	packet.h.header.size = sizeof( k_get_guild_item_map );
	QueuePacket( connections, DB_DEMON, &packet, 1 );	// DB에서 해결
}	//> CSD-030324

// 디비 데몬에서 아이템을 꺼내 왔다.
void RecvGetGuildItemMap( t_packet *p )
{
	int server_id = p->u.kein.get_guild_item_db.server_id;
	ItemAttr *pItem = &p->u.kein.get_guild_item_db.item;

	CHARLIST *ch = CheckServerId( server_id );
	if( !ch ) return;

	//021030 YGI
	SendItemEventLog( pItem, server_id, SN_NOT_USER, SILT_GET_GUILD_BOX, 3 ); // 021018 YGI
	MoveEmptyInv( pItem, ch );
}

void RecvChangeGuildMaster(short int cn)
{	//< CSD-030326
	CHARLIST* ch = CheckServerId(cn);
	if (!ch)  return;

	int guild_code = ch->GetGuildCode();
	const int guild_master = ch->name_status.guild_master;
	
	if (guild_code <= 0 || guild_master != 1) 
	{
		return;		// 길드 마스터가 아니다.
	}

	SendPacketDefault(CMD_CHANGE_GUILD_MASTER, &guild_code, sizeof(short int), DB_DEMON);
	ch->name_status.guild_master = 0;		// 길드 마스터
	SendChangeGuildDegree(cn, guild_code, 0, cn);
	SendPutMenuString(KM_OK, 178, cn);
}	//> CSD-030326

// 길드원 정보를 변경
void SendChangeGuildDegree( int server_id, int guild_code, int guild_degree, short int cn )
{
	t_packet packet;

	packet.h.header.type = CMD_CHANGE_GUILD_MASTER;
		packet.u.kein.change_guild_degree.server_id = server_id;
		packet.u.kein.change_guild_degree.guild_code = guild_code;
		packet.u.kein.change_guild_degree.degree = guild_degree;
	packet.h.header.size = sizeof( k_change_guild_degree );
	QueuePacket( connections, cn, &packet, 1 );
	CastMe2Other( cn, &packet );
}

// 길마가 바꼈다.
void RecvChangeGuildMasterAllMap( t_packet *p )
{	//< CSD-030324
	char *name = p->u.kein.default_name;
	int cn = ExistHe( name );
	if( cn <= 0 ) return;

	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	ch->name_status.guild_master = 1;
	SendChangeGuildDegree( cn, ch->GetGuildCode(), ch->name_status.guild_master, cn );
}	//> CSD-030324

int GetDegree2Param( int degree )		// degree 값을 파라메터로 변경한다.
{
	static int degree2param[] = { 4, 0, 1, 2, 3 };
	return degree2param[degree];
}

// 020620 YGI
bool CheckGuildPower( int type, CHARLIST *ch )
{	//< CSD-030324
	if( !ch ) return 0;
	if( !ch->GetGuildCode() ) return 0;
	if( type < 0 || type > GDP_MAX ) return 0;
	
	static int power_table[][5] =
	{
		{ 1, 1, 0, 0, 0 },		// 길드 가입				// GDP_JOINING
		{ 1, 1, 0, 0, 0 },		// 길드 전체 메시지			// GDP_BBS
		{ 1, 0, 0, 0, 0 },		// 길드원 직책변경			// GDP_CHANGE_DEGREE  // 020620 YGI
		{ 1, 1, 0, 0, 0 },		// 길드 창고 이용			// GDP_GUILD_BOX
		{ 1, 1, 0, 0, 0 },		// 강제 길드 탈퇴 시키기	// GDP_EXIT_GUILD_HIM
		{ 1, 0, 0, 0, 0 },		// 길드 마크 변경			// GDP_CHANGE_MARK
		{ 1, 0, 0, 0, 0 },		// 길드 직책 명 변경		// GDP_CHANGE_DEGREE_INFO
		{ 1, 0, 0, 0, 0 },		// 길드 마스터 변경			// GDP_GUILD_CHANGE_MASTER
		{ 1, 0, 0, 0, 0 },		// 길드 제거				// GDP_GUILD_BREAK
		{ 1, 0, 0, 0, 0 },		// 부길마 등록				// GDP_REGIST_SUB_MASTER
		{ 0, 1, 1, 1, 1 },		// 스스로 길드 탈퇴			// GDP_EXIT_GUILD_SELF
		{ 1, 0, 0, 0, 0 },		// 엠포리아 구입			// GDP_BUY_GUILD_HOUSE
	};

	int para = GetDegree2Param( ch->name_status.guild_master );
	return ((power_table[type][para])?true:false);
}	//> CSD-030324

void RecvGetNationItem( short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	t_packet packet;
	packet.h.header.type = CMD_GET_NATION_ITEM;
		packet.u.kein.nation_item.item_no = g_nation_item.item_no;
		if( g_nation_item.money > 0 )
		{
			packet.u.kein.nation_item.give_type = 0;		// money_type
			packet.u.kein.nation_item.give_much = g_nation_item.money;
		}
		else
		{
			packet.u.kein.nation_item.give_type = 1;		// money_type
			packet.u.kein.nation_item.give_much = g_nation_item.fame;
		}
	packet.h.header.size = sizeof( k_nation_item );
	QueuePacket( connections, cn, &packet, 1 );
}

/*040720_KJHuNs g_pLogManager로 이동.(정리목적)
extern void SaveLogChange_NationItem(CHARLIST* pTarget, int type, ItemAttr* item); // CSD-040224
*/

void RecvBuyNationItem( short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int ret = 0;
	if( g_nation_item.money > 0 )
	{
		if( ch->Money< g_nation_item.money )
		{
			// 돈이 부족하다. 
			ret = 2;
		}
		else
		{
			// 021209 YGI
			if( SubtractMoney( g_nation_item.money, ch ) == 1 )
			{
				ItemAttr item = GenerateItem( g_nation_item.item_no);
				SendItemEventLog( &item, cn, SN_NOT_USER, SILT_BUY_NATION_ITEM_MONEY, 100 );
				g_pLogManager->SaveLogChange_NationItem(ch, 0, &item);	// CSD-040224
				MoveEmptyInv( &item, ch );
				SendNationMoney( NM_ADD, ch->name_status.nation, g_nation_item.money); // 국고를 변화시킨다.
				ret = 1;
			}
			else
			{
				ret = 2;
			}
		}
	}
	else
	{
		if( ch->fame - g_nation_item.fame < 1000 )
		{
			// 명성치가 부족하다.
			ret = 3;
		}
		else
		{
			// 021209 YGI
			ret = 1;
			g_pLogManager->SaveLogChange_Fame(ch,ch->fame,ch->fame-g_nation_item.fame,LF_ITEMBUY);
			ch->fame -= g_nation_item.fame;
			ItemAttr item = GenerateItem( g_nation_item.item_no);
			SendItemEventLog( &item, cn, SN_NOT_USER, SILT_BUY_NATION_ITEM_FAME, 100 );
			g_pLogManager->SaveLogChange_NationItem(ch, 1, &item);	// CSD-040224
			MoveEmptyInv( &item, ch );
		}
	}

	t_packet packet;
	packet.h.header.type = CMD_BUY_NATION_ITEM_OK;
		packet.u.kein.default_char = ret;
	packet.h.header.size = sizeof( char );
	QueuePacket( connections, cn, &packet, 1 );	
}

void RecvCheckColossusMap(t_packet *p, short int cn)
{	//< CSD-TW-030701
	CHARLIST* pTarget = CheckServerId(cn);
		
	if (pTarget == NULL)
	{
		return;
	}

	CArenaInfo* pArenaInfo = g_pArenaManager->GetArenaInfo(g_MapPort);

	if (pArenaInfo == NULL)
	{
		return;
	}

	CArenaManager::HASH_GAME mpArena = g_pArenaManager->GetGameSet();

	for (CArenaManager::ITOR_GAME i = mpArena.begin(); i != mpArena.end(); ++i)
	{
		CBaseArena* pGame = i->second;
		pGame->SendGameInfo(pTarget, g_MapPort);
	}
}	//> CSD-TW-030701

// 021107 YGI
void SendPacket2Map( t_packet *p, int port )
{
	if( port != ST_SEND_MAP_ALL	)
	{
		if( port == GetOwnPort() )	// 같은 맵으로 보낼 경우
		{
			HandleCommand( CN_FROM_OTHER_MAPSERVER_MSG, p );
		}
		else
		{
			g_pServerTable->SendRajaPacketToOtherMapServer( port, (char *)p, p->h.header.size+sizeof(t_header) );
		}
	}
	else // all
		g_pServerTable->BroadCastToEveryServer((char *)p, p->h.header.size+sizeof(t_header),SERVER_TYPE_MAP);
}

void RecvInviteColossus( t_packet *p, short int cn )
{	//< CSD-030509
	CHARLIST* ch = CheckServerId(cn);
	
	if (ch == NULL)
	{
		return;
	}

	CBaseArena* pGame = ch->GetJoinArenaGame();

	if (pGame == NULL)
	{
		return;
	}
	// 방장만 사용할 수 있는 명령어
	if (pGame->GetLeader() != ch) 
	{
		return;		
	}
	
	t_packet packet;
	packet.h.header.type = CMD_INVITE_COLOSSUS_REPLY;
	packet.h.header.size = sizeof(k_invite_colossus_map) - 20 + strlen(ch->Name) + 1;
	packet.u.kein.invite_colossus_map.port = g_MapPort;
	strcpy(packet.u.kein.invite_colossus_map.name, ch->Name);
	SendPacket2Map(&packet, ST_SEND_MAP_ALL);
	RecvInviteColossusReply(&packet);
}	//> CSD-030509

void RecvInviteColossusReply( t_packet *p )
{	//< CSD-CN-031213
	g_pUserManager->SendPacket(p);
}	//> CSD-CN-031213

void RecvJoinColossusMap( t_packet *p, short int cn )
{	//< CSD-030519
	CHARLIST* ch = CheckServerId(cn);
	
	if (ch == NULL)
	{
		return;
	}

	const int nPort = p->u.kein.default_short_int;

	if (nPort == g_MapPort)
	{
		return;
	}

	CArenaInfo* pArena = g_pArenaManager->GetArenaInfo(nPort);

	if (pArena == NULL)
	{
		return;
	}
	
	t_MapInfo* pMapInfo = GetMapInfoByMapPort(nPort);

	if (pMapInfo == NULL)
	{
		return;
	}

	int nX = 0, nY = 0;
	pArena->GetWaitPosition(nX, nY);
	MapMove(cn, pMapInfo->mapfile, nX, nY);
}	//> CSD-030519

void RecvAttackColossusItem( t_packet *p, short int cn )
{	//< CSD-030509
	CHARLIST* ch = CheckServerId(cn);
	
	if (ch == NULL) 
	{
		return;
	}

	CBaseArena* pArena = ch->GetJoinArenaGame();

	if (pArena == NULL)
	{
		return;
	}

	if (!pArena->IsPlayGame())
	{
		return;
	}

	CArenaTeam* pTeam = ch->GetJoinArenaTeam();

	if (pTeam == NULL)
	{
		return;
	}

	const int nItem = p->u.kein.default_short_int;
	
	if (nItem < 0 || nItem >= MAX_ITEM_LIST) 
	{
		return;
	}

	SetStoneByTeam(&ItemList[nItem], pTeam->GetTeamNumber());

	t_packet packet;
	packet.h.header.type = CMD_ATTACK_COLOSSUS_STONE_RESULT;
	packet.h.header.size = sizeof(k_attack_colossus_stone_result);
	packet.u.kein.attack_colossus_stone_result.server_id = cn;
	packet.u.kein.attack_colossus_stone_result.item_id = nItem;
	packet.u.kein.attack_colossus_stone_result.attr = ItemList[nItem].attr[IATTR_ATTR];
	QueuePacket(connections, cn, &packet, 1);
	CastMe2Other(cn, &packet);
	// 현재 돌의 수를 카운트 해서 보내준다.
	int team_stone[4] = {0,};
	g_ColossusStone.GetTeamCount( team_stone );
	
	for (int i = 0; i < 2; ++i)
	{	//< CSD-030520
		CBaseArena* pGame = g_pArenaManager->GetGameInfo(i);

		if (pGame == NULL)
		{
			continue;
		}

		pGame->SendGameMessage(i, 2, team_stone[i]);
	}	//> CSD-030520
	// 전체에게 누가 스톤을 차지 했는지 알려준다.
	CBaseArena* pGame = ch->GetJoinArenaGame();

	if (pGame != NULL)
	{	//< CSD-030520
		pGame->SendGameMessage(ch, 5, ItemList[nItem].dumno+1);
	}	//> CSD-030520

	// 전부 차지 했는지 알아본다.
	int time = 0;
	int got_stone_team = 0;		// 돌을 독점한 팀
	for(int i=0; i<2; i++ )
	{
		if( team_stone[i] == g_ColossusStone.GetMaxStone() )
		{
			got_stone_team = i;
			time = g_curr_time;
			break;
		}
	}
	g_ColossusStone.SetWinCondition( time, got_stone_team );
}	//> CSD-030509

void CallMenu( short int menu, short int cn )
{
	SendPacketDefault( CMD_CALL_MAIN_MENU, &menu, sizeof( short int ), cn );
}

void RecvRevivalColossus(t_packet *p, short int cn)
{	//< CSD-030509
	CHARLIST* ch = CheckServerId(cn);
	
	if (ch == NULL)
	{
		return;
	}

	CBaseArena* pGame = ch->GetJoinArenaGame();

	if (pGame == NULL)
	{
		return;
	}

	if (!pGame->IsPlayGame())
	{
		return;
	}

	CArenaTeam* pTeam = ch->GetJoinArenaTeam();

	if (pTeam == NULL)
	{
		return;
	}

	switch (pGame->GetArenaType())
	{
	case AT_STONE:
	case AT_MONSTER:
		{
			if (ch->viewtype != VIEWTYPE_GHOST_ && ch->Hp > 0) 
			{	// 유령이 아니다.
				return;
			}

			int nX = 0, nY = 0;
			pTeam->GetBasePosition(nX, nY);
			MovePc(cn, nX, nY);
			SkillMgr.CharacterToAlive(ch);
			// 전체 공지 메시지
			pGame->SendGameMessage(ch, 4, 0); // CSD-030520
			break;
		}
	}
}	//> CSD-030509

void RecvGuildHouseInfo( t_packet *p, short int cn )
{	//< CSD-030324
	CHARLIST *ch = CheckServerId( cn ); 
	if( !ch ) return;

	//if( !ch->GetGuildCode() ) return;
	int nation = ch->name_status.nation;

	t_packet packet;
	packet.h.header.type = CMD_GUILD_HOUSE_INFO;
	k_guild_house_info *t = (k_guild_house_info *)packet.u.data;
	CGuildHouseUnit *pData;
	t->count=0;
	for( int i=0; i<g_GuildHouse.m_Max; i++ )
	{
		pData = &g_GuildHouse.m_aGuildHouse[i];
		if( !pData->id ) continue;
		if( pData->nation != nation ) continue;
		t->data[t->count].id = pData->id;
		t->data[t->count].guild_code = pData->guild_code;
		t->data[t->count].blue = pData->blue;
		t->data[t->count].red = pData->red;
		t->data[t->count].lv = pData->level;

		t->data[t->count].time = 0;
		// 031009 CI YGI
		int time = 0;
		if( pData->year >= 1900 )
		{
			time += (pData->year-g_year )*365;
			time += (pData->month-g_mon-1)*30;
			time += pData->day-g_day;
			if( time < 0 ) t->data[t->count].time = 0;
			else if( time > 125 ) t->data[t->count].time = 60;
			else
			{
				t->data[t->count].time = time;
			}
		}
		
		strcpy( t->data[t->count].map_name, pData->map_name );
		t->count ++;
	}
	packet.h.header.size = sizeof( k_guild_house_info_unit )*t->count+sizeof( char );
	QueuePacket( connections, cn, &packet, 1 );
	static int check_day = g_day;
	//if( check_day != g_day )
	{
		check_day = g_day;
		CallClient( DB_DEMON, CMD_CHECK_GUILD_HOUSE );		// 없어져야 할 길드가 있는지 체크한다. // 변경 사항을 전 맵에 뿌려준다.
	}
}	//> CSD-030324

void RecvBuyGuildHouse( t_packet *p, short int cn )
{	//< CSD-030324
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int house_id = p->u.kein.default_char;
	int guild_code = ch->GetGuildCode();

	if( !CheckGuildPower( GDP_BUY_GUILD_HOUSE, ch ) ) return;		// 길마만 사용할 수 있다.

	CGuildHouseUnit	*pHouse = g_GuildHouse.GetHouseByIndex( house_id );		// 기존 길드가 있다.
	if( !pHouse  || !pHouse->id ) return;
	if( pHouse->nation != ch->name_status.nation ) return;


	if( GetItemMuchByMuchItem( ch, IN_BLUE_SIGNET ) < pHouse->blue ) return;		// 시그넷이 부족하다.
	if( GetItemMuchByMuchItem( ch, IN_RED_SIGNET ) < pHouse->red ) return;		// 시그넷이 부족하다.

	k_change_guild_house_info data;
	data.guild_code = guild_code;
	data.id = pHouse->id;		// 엠포리아 인덱스
	if( pHouse->guild_code )
	{
		if( pHouse->guild_code != guild_code ) return;

		int time = 0;
		if( pHouse->year >= 1900 )
		{
			time += (pHouse->year-g_year )*365;
			time += (pHouse->month-g_mon-1)*30;
			time += pHouse->day-g_day;
		}
		if( time < 0 || time > 5 )
		{
			// 한번만 연장 할 수 있다.
			SendPutMenuString( KM_FAIL, 174, cn );
			return;
		}
		// 이경우엔 디비 데몬이 모든서버로 보내주게 하자..
		data.day_type = 2;		// 연장
	}
	else
	{
		CGuildHouseUnit	*pData = g_GuildHouse.GetHouseByGuildCode( guild_code );
		if( pData ) return;		// 이미 가지고 있는 엠포리아가 있다.
		// 새로 신청
		data.day_type = 1;		// 신규 신청
	}

	// 삭제 // 취소 됐을 경우 삭제 되므로 DB-demon에서 취소 되면 안된다.
	SendDeleteMuchItemQuantity( ch, IN_BLUE_SIGNET, pHouse->blue );
	SendDeleteMuchItemQuantity( ch, IN_RED_SIGNET, pHouse->red );

	SendPacketDefault( CMD_CHANGE_GUILDHOUSE_INFO, &data, sizeof( k_change_guild_house_info ), DB_DEMON );
}	//> CSD-030324

void RecvChangeGuildHouse( t_packet *p, short int cn )
{
	// 엠포리아 정보 갱신
	int house_id = p->u.kein.change_guild_house_info_db2map.house_id;
	CGuildHouseUnit	*pHouse = g_GuildHouse.GetHouseByIndex( house_id );
	if( !pHouse  || !pHouse->id ) return;

	pHouse->guild_code		=	p->u.kein.change_guild_house_info_db2map.guild_code;
	pHouse->year			=	p->u.kein.change_guild_house_info_db2map.year;
	pHouse->month			=	p->u.kein.change_guild_house_info_db2map.month;
	pHouse->day				=	p->u.kein.change_guild_house_info_db2map.day;
}

void RecvCheckGuildHouse( t_packet *p, short int cn )
{
	int count = p->u.kein.check_guild_house.count;
	char *pHouseId = p->u.kein.check_guild_house.pHouseId;
	if( !count ) return;

	for( int i=0; i<count; i++ )
	{
		CGuildHouseUnit	*pHouse = g_GuildHouse.GetHouseByIndex( pHouseId[i] );
		if( !pHouse ) continue;
		pHouse->guild_code = 0 ;
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// CPotionBox member fuctions
int CPotionBox::PutItem( ItemAttr *item )
{
	//if( !box_type ) return 0;
	CItem *t= ::ItemUnit( item->item_no );
	if( !t ) return 0;

	switch( t->GetItemKind())
	{
	case IK_POTION_BAG:		// BBD 040213	포션백도 포션박스에 들어가믄 않된다
	case IK_POTION_BOX:
	case IK_CANDLE:
	case IK_GOOD_CANDLE:
	case IK_DOUBLE_EXP_UP:
		{	//< CSD-040803
			return 0;
		}	//> CSD-040803
	}
	if( t->GetRbutton() != USE_ITEM ) {return 0;}

	if(item->attr[IATTR_RARE_MAIN])//기능 아이템은 못먹어
	{
		return 0;
	}

	if( much )
	{
		if( item_no != item->item_no ) {return 0;}
		if( 100<= much){return 0;}//020520 lsw 50개로 제한   //coromo 浪彊芎鑒좆
	}
	else
	{
		item_no = item->item_no ;
	}
 
	much++;
	SaveData();
	::DeleteItem(item);
	
	return 1;
}

int CPotionBox::DeleteItem()
{
	if( !much ) return 0;		// 뺄거 없음
	much--;
	SaveData();
	return 1;
}

int CPotionBox::UseItem()
{
	if( !DeleteItem() ) return 0;		// 없다.
	CItem *t = ::ItemUnit( item_no );
	if( !t ) 
	{
		return 0;
	}
	
	switch( t->GetItemKind())//양초나 포션 상자일 경우 지워 버린다
	{
	case IK_POTION_BAG:		// BBD 040213	포션백도 포션박스에 들어가믄 않된다, 당근 사용도 않됨
	case IK_POTION_BOX:
	case IK_CANDLE:
	case IK_GOOD_CANDLE:
	case IK_DOUBLE_EXP_UP:
		{	//< CSD-040803
			item_no = 0;
			much = 0;
			return 0;
		}	//> CSD-040803
	}
	const int add_hp = t->EatItem( hero );
	return add_hp;
}

void CPotionBox::SaveData( )		// 아이템을 사용한후  저장한다.
{
	DWORD temp;
	temp = MAKELONG( item_no, much );
	hero->Sight = temp;
	hero->SendCharInfoBasic( CIB_POTION_BOX, GetData() );		// 변경 사항을 알려준다.////020704 lsw
}

void RecvPkOnOff( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int pk_on_off= p->u.kein.default_char;
	if( ch->pk_on_off != pk_on_off )
	{
		ch->pk_on_off = pk_on_off;
		QueuePacket( connections, cn, p, 1 );
	}
}

void RecvCMD_GET_PK_ON_OFF( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	SendPacketDefault( CMD_GET_PK_ON_OFF, &ch->pk_on_off, sizeof( char ), cn );
}

// 020430 YGI acer 3
//////////////////////////////////////////////////////////////////////
// CItemLog member functions
CItemLog	g_item_log;
//acer4
void CItemLog::Load( int port )
{
	int temp = 0;
	char MapServerConfigFileName[MAX_PATH];
	sprintf(MapServerConfigFileName,"%s/data/MapServerConfig.ini", GameServerDataPath);
	//if( GetOwnPort() == GetManagementMapPort( port ) )
	{
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "DeleteDay",0,MapServerConfigFileName);
		if( !temp ) return;
		m_nDeleteDay = temp;

		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "SaveLevel",0,MapServerConfigFileName);
		if( temp ) m_nSaveLv = temp;

		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "ItemLevel",0,MapServerConfigFileName);
		if( temp ) m_nItemLv = temp;

		MyLog( LOG_NORMAL, "Item Log config, DeleteDay = %d, SaveLevel = %d, ItemLevel = %d", m_nDeleteDay, m_nSaveLv, m_nItemLv );

		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "trade",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_PUT_OTHER_CH] = temp;
			m_aType2Lv[SILT_GET_OTHER_CH] = temp;
		}
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "store",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_GET_STORE] = temp;
			m_aType2Lv[SILT_PUT_STORE] = temp;
		}
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "ground",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_PUT_GROUND] = temp;
			m_aType2Lv[SILT_GET_GROUND] = temp;
			m_aType2Lv[SILT_PUT_GROUND_MAX_INVEN] = temp;
			m_aType2Lv[SILT_DROP_BY_PK] = temp;
		}
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "duration",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_DELETE_BY_DURATION] = temp;
		}
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "stolen",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_GET_OTHER_CH_INVEN] = temp;
			m_aType2Lv[SILT_GET_STOLEN] = temp;
			m_aType2Lv[SILT_PUT_STOLEN] = temp;
		}
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "make_by_skill",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_MAKE_BY_SKILL] = temp;
		}
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "make_by_event",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_MAKE_BY_DB] = temp;
			m_aType2Lv[SILT_MAKE_BY_SCRIPT] = temp;
			m_aType2Lv[SILT_GET_GAMBLE_ITEM] = temp;
			m_aType2Lv[SILT_PUT_GAMBLE_ITEM] = temp;
			m_aType2Lv[SILT_DELETE_BY_SCRIPT] = temp;
		}
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "resource",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_RESOURCE_BY_SKILL] = temp;
			m_aType2Lv[SILT_NEED_ITEM] = temp;
		}
		temp = 0; temp = (int)GetPrivateProfileInt("ItemLog", "man2man",0,MapServerConfigFileName);
		if( temp ) 
		{
			m_aType2Lv[SILT_GET_MEN_TO_MEN] = temp;
			m_aType2Lv[SILT_PUT_MEN_TO_MEN] = temp;
		}
	}
	// 020620 YGI
	sprintf(MapServerConfigFileName,"%s/data/NoItemLog.txt", GameServerDataPath);
	m_NoItemList.Load( MapServerConfigFileName );
}

bool CItemLog::IsSave( ItemAttr *item, int event_type, int lv )
{
	if( lv == 100 ) return true;		// 무조건 저장 // 030506 YGI

	if( !item ) return false;
	if( item->attr[IATTR_RARE_MAIN] ) return true;			// 레어 저장
	
	if( m_NoItemList.Search( item->item_no ) ) return false;		// 저장하지 말아라 목록에 있으면 저장하지 않는다.
	if( item->item_no/1000 == ACCESSORY ) return true;		// 유니크면 저장
	
	CItem *t = ::ItemUnit( item->item_no );
	if( !t ) return false;

	if( t->GetLevel() < m_nItemLv ) return false;			// 아이템 레벨이 낮으면 저장하지 않는다.
	if( !m_aType2Lv[event_type] ) return false;				// 0 일경우 저장하지 않는다.
	if( m_nSaveLv && m_aType2Lv[event_type] > m_nSaveLv ) return false;	// 이벤트 타입이 낮으면 저장하지 않는다.
	
	return true;		// 아이템 레벨과 타입이 높아야지만 저장한다.
}

// 020620 YGI
////////////////////////////////////////////////////////////////////////////
// class C2JinSearch

int C2JinSearch::Load( char *filename )
{
	Remove();

	FILE *fp = fopen( filename, "rt" );
	if( !fp ) return 0;	

	char temp[256];
	int count = 0;
	while( fgets( temp, 256, fp ) )
	{
		if( temp[0] == ' ' || temp[0] == ';' || temp[0] == '\n' ) continue;
		if( !atoi( temp ) ) continue;
		count++;
	}
	fseek( fp, 0, SEEK_SET );
	max = count;
	item = new int[max];
	count = 0;
	while( fgets( temp, 256, fp ) )
	{
		if( temp[0] == ' ' || temp[0] == ';' || temp[0] == '\n' ) continue;
		int value = atoi( temp );
		if( !value ) continue;
		item[count] = value;
		count++;
	}
	Sort();
	fclose( fp );
	return 1;
}

int C2JinSearch::Sort()
{
	if( !max ) return 0;
	for( int i=0; i<max-1; i++ )
	{
		for( int j=i+1; j<max; j++ )
		{
			if( item[i] > item[j] )
			{
				int temp = item[i];
				item[i] = item[j];
				item[j] = temp;
			}
		}
	}
	return 1;
}

int C2JinSearch::Search( int no )
{
	if( !max ) return 0;
	int low=0, high = max-1;
	int mid = 0;
	while( low <= high )
	{
		mid = (low+high)/2;
		if( item[mid] == no ) return 1;
		if( item[mid] < no ) low = mid+1;
		else high = mid-1;
	}

	return 0;
}

int RecvSaveItemLogByEvent( t_packet *p )
{	//< CSD-030806
	if( g_MapPort == GetManagementMapPort( MM_SAVE_LOG_FILE_MAP ) ) 
	{
		k_item_log *pData = &p->u.kein.item_log;
		if( g_item_log.GetSaveLv() )
		{
			//받는데에서는 다 저장하자.
			//if( pData->lv > g_item_log.GetSaveLv()  ) return 0;
		}
		return SaveItemLog( pData );
	}
	return 0;
}	//> CSD-030806

void SendItemEventLog( ItemAttr *item, short int hero_cn, short int target, int event_type, int lv )
{
	if( !g_item_log.IsSave( item, event_type, lv ) ) return;		// 저장 등급이 안되면 리턴하자.. // 030506 YGI

	CHARLIST *hero = CheckServerId( hero_cn );
	if( !hero ) return;

	t_packet packet;
	ZeroMemory(&packet.u.kein.item_log,sizeof(packet.u.kein.item_log));

	k_item_log *pData = &packet.u.kein.item_log;
	packet.h.header.type = CMD_SAVE_ITEM_LOG;
	packet.h.header.size = sizeof( k_item_log );

	if( lv == 0 ) lv = 1;
	pData->item = *item;
	pData->lv = g_item_log.GetTypeLv(event_type)?g_item_log.GetTypeLv(event_type):1;	// 030506 YGI
	pData->port = GetOwnPort();
	pData->type = event_type;
	strcpy( pData->name1, hero->Name );
	if( target > 0 )
	{
		CHARLIST *target_ch;
		if( target > 10000 )	target_ch = CheckNpcId( target-10000 );
		else					target_ch = CheckServerId( target );

		if( target_ch )	
		{
			if( target_ch->IsNpc() )
			{
				strcpy( pData->name2, "<_NPC_>");
			}
			else
			{
				memcpy( pData->name2, target_ch->Name, 20 );//020510 lsw //혹시 모를 Garbage Value
			}
		}
		else
		{
			strcpy( pData->name2, "<_NULL_>" );
		}
	}
	else
	{
		switch( event_type )
		{
			ZeroMemory(pData->name2,sizeof(pData->name2));

			default :	//acer7
			case SILT_GET_STOLEN					:
			case SILT_PUT_STOLEN					:
			case SILT_GET_OTHER_CH_INVEN	:	
			case SILT_PUT_OTHER_CH				:
			case SILT_GET_OTHER_CH				:
				strcpy( pData->name2, _T("<_NULL_>") );
				break;
			case SILT_PUT_GROUND					:
			case SILT_GET_GROUND					:
				strcpy( pData->name2, _T("<_GROUND_>") );
				break;
			case SILT_PUT_BANK						:
			case SILT_GET_BANK						:
				strcpy( pData->name2, _T("<_BANK_>") );
				break;

			case SILT_MAKE_BY_DB					:	
			case SILT_MAKE_BY_SCRIPT			:	
			case SILT_MAKE_BY_SKILL				:
			case SILT_RESOURCE_BY_SKILL		:
				strcpy( pData->name2, _T("<_MAKE_>") );
				break;
			case SILT_PUT_STORE						:
			case SILT_GET_STORE						:
				strcpy( pData->name2, _T("<_STORE_>") );
				break;

			case SILT_GET_GAMBLE_ITEM			:
			case SILT_GET_TODAY_ITEM			:	
			case SILT_GET_GUILD_WAR				:
			case SILT_USE									:
			case SILT_DELETE_BY_DURATION	:
			case SILT_DELETE_BY_ATTR			:
			case SILT_DELETE_BY_RARE_ATTR	:
				strcpy( pData->name2, _T("<_SERVER_>") );
				break;
			//<! BBD 040226	옥션 거래시 로그라는걸 알수 있도록 추가
			case SILT_REGISTER_AUCTION_ITEM		:	// 옥션 등록시
			case SILT_CANCEL_AUCTION_SELL_ITEM	:	// 등록 취소시
			case SILT_TAKE_AUCTION_RESULT_ITEM	:	// 구입한 물건 찾을시
				strcpy( pData->name2, _T("<_AUCTION_>") );
				break;
			//> BBD 040226	옥션 거래시 로그라는걸 알수 있도록 추가
			//<050224_KCH 마일리지몰 작업
			case SILT_RECEIVE_ITEM_MALL	:
				strcpy( pData->name2, _T("<_IM_RECEIVE_>") );
				break;
			case SILT_DELETE_ITEM_MALL	:	//마일리지몰 아이템 제거.
				strcpy( pData->name2, _T("<_IM_DELETE_>") );
				break;
			case SILT_DELETE_ITEM_MALL_DUPE	:	//복사된 마일리지몰 아이템 제거.
				strcpy( pData->name2, _T("<_IM_DELETE_DUPE_>") );
				break;
			//>050224_KCH 마일리지몰 작업
		}
	}
	SendDirectMap( &packet, GetManagementMapPort( MM_SAVE_LOG_FILE_MAP ) );
}

int GetServerSetNum()
{
	return g_pServerTable->GetServerSetNum();
}

// GM 기능, 맵 강제 이동
//acer7
void RecvDirectMapMove( t_packet *p, short int cn )
{
	if( !CheckServerId( cn ) ) return;

	int index = p->u.kein.direct_map_move.map_number;
	int x = p->u.kein.direct_map_move.x;
	int y = p->u.kein.direct_map_move.y;
	char *name = p->u.kein.direct_map_move.name;
	int target_id = cn;

	if( name[0] )
	{
		int temp = ExistHe( name );		// cn 값이 있다면...
		if( temp == -1 ) 
		{
			SendPutMenuString( KM_FAIL, 181, cn );		// 같은 맵에 없는 캐릭터 입니다.
			return;		// 상대편 이름이 잘 못됐다.
		}
		target_id = temp;
	}
	CrackMgr.SendHackingUser( cn, USE_GM_TOOL, " Map Move " );

	GotoUser( MapInfo[index].mapfile, x, y, target_id );			// 020705 YGI
}


// acer4
void RecvSearchChar( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	t_packet packet;
	char *name = p->u.kein.default_name;
	int he = ExistHe( name );
	if( he > 0 )
	{
		CHARLIST *him = CheckServerId( he );
		if( !him ) return;

		PutHimInfo( him, &packet.u.kein.char_info );
		packet.h.header.type = CMD_SEARCH_OTHER_CHAR;
		packet.h.header.size = sizeof( k_char_info );
		QueuePacket( connections, cn, &packet, 1 );
		return;
	}
	else		// 다른 맵으로 요청한다.
	{
		packet.h.header.type = CMD_SEARCH_OTHER_CHAR_MAP;
		packet.u.kein.char_info_map.port = GetOwnPort(); 
		packet.u.kein.char_info_map.cn = cn;
		strcpy( packet.u.kein.char_info_map.name, name );
		packet.h.header.size = sizeof( k_char_info_map );
		SendPacket2Maps( &packet );
	}
}

// acer4
int PutHimInfo( CHARLIST *him, k_char_info *him_info )
{	//< CSD-030324
	him_info->map_number = MapNumber;
	him_info->curr_tactic_kind = him->GetTacticsKind();
	him_info->curr_tactic_lv = him->GetWeaponSkill();
	memcpy( &him_info->name_status, &him->name_status, sizeof( DWORD ) );
	him_info->btLevel = him->GetLevel(); // CSD-030806
	him_info->nk = him->GetNK( him->name_status.nation );
	him_info->x = him->MoveSx;
	him_info->y = him->MoveSy;
	him_info->guild_code = him->GetGuildCode();
	return 1;
}	//> CSD-030324

void RecvSearchCharMap( t_packet *p )
{
	int server_id = p->u.kein.char_info_map.cn;
	int port = p->u.kein.char_info_map.port;
	char *name = p->u.kein.char_info_map.name;

	int he = ExistHe( name );
	if( he > 0 )
	{
		CHARLIST *him = CheckServerId( he );
		if( !him ) return;

		k_char_info data;
		PutHimInfo( him, &data );
		SendDirectClientOtherMap( CMD_SEARCH_OTHER_CHAR, server_id, port, &data, sizeof( k_char_info ) );
	}
}

//acer7
void RecvUseGmTool_SaveLog( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	char *type = p->u.kein.default_msg;
	CrackMgr.SendHackingUser( cn, USE_GM_TOOL, type );
}

void RecvEventNumber( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int event_no = p->u.kein.default_int;
}

void MapServerFree_kein()
{
	SAFE_DELETE(g_pDungeonMap);
	SAFE_DELETE(g_pDungeonMgr);
	
}

void SaveLogChange_Ladder(CHARLIST* pTarget, int old_ladder, int new_ladder, int type)
{	//< CSD-040224
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}

	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}

	char why[MAX_PATH];

	switch (type)
	{
		case CLT_CHANGE_DUAL: 
			{
				strcpy(why, "upgrade dual");
				break;
			}
		case CLT_WIN_COLOSSUS:
			{
				strcpy(why, "win colossus");
				break;
			}
		case CLT_LOSE_COLOSSUS:
			{
				strcpy(why, "loss colossus");
				break;
			}
		default:
			{
				strcpy(why, "default");
				break;
			}
	}

	fprintf(fp,
		    "@ChangeLadderScore	%02d/%02d/%02d/%02d/%02d/%02d	(old ladder)%d(==>)	(new ladder)%d	(why)%s",
			g_year - 2000, g_mon + 1, g_day, g_hour, g_min, g_sec,
			old_ladder, new_ladder, why);
	fprintf( fp, "\n" );
	fclose( fp );
}	//> CSD-040224

/* 040720_KJHuNs g_pLogManager로 이동(정리목적)
void SaveLogChange_ObtainCombat(CHARLIST* pTarget, int nOld, int nNew)
{	//< CSD-040224
}	//> CSD-040224
void SaveLogChange_Combat(CHARLIST* pTarget, int nCombat, int nOld, int nNew)
{	//< CSD-040224
}	//> CSD-040224
*/

// 021117 YGI
void RecvSetInstallVersion ( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	ch->installType = IT_LIGHT_VERSION;
}

//021030 YGI
void RecvResendItem( short int cn )
{
	if( !CheckServerIndex( cn ) ) return;
	SendItemInventory( 0, connections, cn );
	SendItemInventory( 1, connections, cn );
	SendItemInventory( 2, connections, cn );
	SendItemInventory( 3, connections, cn );
	SendItemInventory( 4, connections, cn );
	SendItemInventory( 5, connections, cn );
	SendItemInventory( 6, connections, cn );
	SendItemInventory( 7, connections, cn );
	SendItemInventory( 8, connections, cn );
}

void RecvScriptItem( t_packet *p, short int cn )
{
	k_event_script_item	*pData = (k_event_script_item *)p->u.data;

	CEventNpcScript *pEvent = g_EventMgr.GetEventNpcScriptByEventNo( pData->event_no);
	if( !pEvent ) return;

	CHARLIST *ch = CheckServerId( pData->server_id );
	if( !ch ) return;

	if( strcmp( ch->Name, pData->name ) != 0 ) return ;
	
	if( pData->result )
	{
		SendEventNpcScriptItem( pData->server_id, pData->npc_index, pEvent->m_nEventNpcItemBag, 
			pEvent->m_lpszMessage, pEvent->m_nMessageSize );
	}
	else		// 아이템을 만들지 말고 실패한 이야기를 들려주자
	{
		if( pEvent->m_lpszMessage2 && pEvent->m_nMessageSize2 )		// npc의 대화를 보내준다.
			SendDial( pData->server_id, pData->npc_index, pEvent->m_lpszMessage2, pEvent->m_nMessageSize2 );

	}
}

void RecvEventMoveMap( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int event_no = p->u.kein.default_int;
	CEventMoveMap *pEvent = g_EventMgr.GetEventMoveMapPoint( event_no );
	if( !pEvent ) return;

	// 이벤트 참여 조건 확인 --------------------
	if( !pEvent->m_bAll )
	{
		static int sex2sex[] = { 2, 1 };
		static int class2class[] = { 1, 3, 2, 5, 4 };
		static int nation2nation[] = { 0, 0, 0, 1, 2, 0, 3 };

		if( pEvent->m_nSex )
		{
			if( pEvent->m_nSex != sex2sex[ch->SprNo] ) return;
		}
		if( pEvent->m_nClass )
		{
			if( pEvent->m_nClass != class2class[ch->Class] ) return;
		}
		if( pEvent->m_nNation )
		{
			if( pEvent->m_nNation != nation2nation[ch->name_status.nation] ) return;
		}
	}
	//----------------------------

	t_packet packet;
	packet.h.header.type = CMD_EVENT_MOVE_MAP_CHECK_MAP;
		packet.u.kein.event_move_map_check_map.server_id = cn;
		packet.u.kein.event_move_map_check_map.server_port = GetOwnPort();
		packet.u.kein.event_move_map_check_map.event_no = event_no;
	packet.h.header.size =  sizeof( k_event_move_map_check_map );

	int port = GetMapPortByName( pEvent->m_szMap );
	if( port == GetOwnPort() ) return;
	if( !port )	return;
	SendPacket2Map( &packet, port );
}

void RecvEventMoveMapCheckMap( t_packet *p, short int cn )		// 다른 맵에서 올경우
{
	k_event_move_map_check_map *pData = (k_event_move_map_check_map *)p->u.data;
	CEventMoveMap *pEvent = g_EventMgr.GetEventMoveMapPoint( pData->event_no );
	if( !pEvent ) return;
	if( _stricmp( pEvent->m_szMap, MapName ) != 0 ) return;		// 이벤트 번호가 꼬였다. 

	int user_count = g_pServerTable->GetNumOfUsers();
	if( user_count >= pEvent->m_nMax )
	{
		// 더이상 참가할 수 없다.
		k_put_menu_string msg;
		msg.type = KM_FAIL;
		msg.str_num = 212;

		SendDirectClientOtherMap( CMD_PUT_MENU_STRING, pData->server_id, pData->server_port, &msg, sizeof( k_put_menu_string ) );
	}
	else
	{
		t_packet packet;
		packet.h.header.type = CMD_EVENT_MOVE_MAP_OK;
			strcpy( packet.u.kein.event_move_map_ok.map_name, MapName );
			packet.u.kein.event_move_map_ok.server_id = pData->server_id;
			packet.u.kein.event_move_map_ok.map_x = pEvent->m_nMapX;
			packet.u.kein.event_move_map_ok.map_y = pEvent->m_nMapY;

		packet.h.header.size = sizeof( k_event_move_map_ok );

		SendPacket2Map( &packet, pData->server_port );

		// 서버에 저장한다.
		k_save_event_move_map save_data;
		save_data.event_no = pData->event_no;
		save_data.max = pEvent->m_nMax;	// 021128 YGI
		save_data.now = user_count;
		SendPacketDefault( CMD_SAVE_EVENT_MAPMOVE_NOW, &save_data, sizeof( k_save_event_move_map ), DB_DEMON );
	}
}

void RecvEventMoveMapOk( t_packet *p, short int cn )
{
	k_event_move_map_ok *pData = (k_event_move_map_ok *)p->u.data;
	MapMove( pData->server_id, pData->map_name, pData->map_x, pData->map_y );
}

void CheckDoubleName( CHARLIST *ch, short int cn )
{
	char name[20]={0,};
	::strcpy( name, ch->Name );
	int target_id = ExistHe( ch->Name );
	if( target_id > 0 )		// 누군가 같은 이름을 가지고 있다.
	{
		MyLog(1,"CheckDoubleName() name = %s", ch->Name );
		closeconnection( connections, target_id, -101 );		// 일단 다른 놈을 끊고 --> 저장
		ch->updatable = 0; // CSD-TW-030620
		closeconnection( connections, cn, -102 );				// 동시 접속 한 놈은 저장하지 말면서 끊자...
	}
	else
	{
		target_id = ExistConnectingHe( ch->Name );
		if( target_id > 0 )
		{
			connections[target_id].chrlst.updatable = 1; // CSD-TW-030620
			closeconnection( connections, target_id, -103 );		// 일단 다른 놈을 끊고 --> 저장
			ch->updatable = 0; // CSD-TW-030620
			closeconnection( connections, cn, -104 );				// 동시 접속 한 놈은 저장하지 말면서 끊자...
		}
	}

	// 다른 곳에 있는 놈도 끊자..
	t_packet packet;
	packet.h.header.type = CMD_CHECK_DOUBLE_NAME;
		strcpy( packet.u.kein.invite_colossus_map.name, name );
		packet.u.kein.invite_colossus_map.port = GetOwnPort();
	packet.h.header.size = sizeof( k_invite_colossus_map );
	SendPacket2Map( &packet, ST_SEND_MAP_ALL );
}

// 해당 이름이 있는지 확인한다.
void RecvCheckDoubleName( t_packet *p, short int cn )
{
	char *name = p->u.kein.invite_colossus_map.name;
	int port = p->u.kein.invite_colossus_map.port;

	int target_id = ExistHe( name );
	if( target_id > 0 )
	{
		closeconnection( connections, target_id, -105 );		// 일단 다른 놈을 끊고 --> 저장
		// 그 사실을 알려준다.
		t_packet packet;
		packet.h.header.type = CMD_CHECK_DOUBLE_NAME_OK;
			strcpy( packet.u.kein.default_name, name );
		packet.h.header.size = strlen(name);
		SendPacket2Map( &packet, port );
	}
	else
	{
		target_id = ExistConnectingHe( name );
		if( target_id > 0 )
		{
			connections[target_id].chrlst.updatable = 1; // CSD-TW-030620
			closeconnection( connections, target_id, -106 );		// 일단 다른 놈을 끊고 --> 저장

			// 그 사실을 알려준다.
			t_packet packet;
			packet.h.header.type = CMD_CHECK_DOUBLE_NAME_OK;
				strcpy( packet.u.kein.default_name, name );
			packet.h.header.size = strlen(name);
			SendPacket2Map( &packet, port );
		}
	}
}

void RecvCheckDoubleNameOk( t_packet *p, short int cn )
{
	char *name = p->u.kein.default_name;
	int target_id = ExistHe(name );
	if( target_id > 0 )
	{	// 다른 맵에 같은 놈이 있다는 소식이 왔기 때문에 저장하지 말고 삭제하자
		connections[target_id].chrlst.updatable = 0; // CSD-TW-030620
		closeconnection( connections, target_id, -102 );
	}
	else
	{
		target_id = ExistConnectingHe( name );

		if( target_id > 0 )
		{
			connections[target_id].chrlst.updatable = 0; // CSD-TW-030620
			closeconnection( connections, target_id, -102 );		// 일단 다른 놈을 끊고 --> 저장
		}
	}
}

void RecvCheckDoubleNameDB2Map( t_packet *p, short int cn )
{
	return;
	int target_id;
	int bClose;
	char *pName;  
	for( int i=0; i<4; i++ )
	{
		pName = p->u.kein.login_check_double_name.name[i];
		if( pName[0] )
		{
			EatRearWhiteChar( pName );
			bClose = 0;
			target_id = ExistHe( pName );
			if( target_id > 0 )
			{
				bClose = 1;
			}
			else
			{
				target_id = ExistConnectingHe( pName );
				if( target_id > 0 )
				{
					bClose = 1;
				}
			}

			if( bClose )
			{
				connections[target_id].chrlst.updatable = 1; // CSD-TW-030620
				closeconnection( connections, target_id, -101 );
			}
		}
	}
}

void RecvCheckDoubleNameWhenFirst( t_packet *p, short int cn )
{
	char *pName = p->u.kein.check_double_when_first.name;
	int server_id = p->u.kein.check_double_when_first.server_id;
	int port = p->u.kein.check_double_when_first.port;

	int bClose = 0;
	int target_id = ExistHe( pName );
	if( target_id > 0 )
	{
		bClose = 1;
	}
	else
	{
		target_id = ExistConnectingHe( pName );
		if( target_id > 0 )
		{
			bClose = 1;
		}
	}

	if( bClose )
	{
		connections[target_id].chrlst.updatable = 1; // CSD-TW-030620
		closeconnection( connections, target_id, -101 );

		t_packet packet;
		packet.h.header.type = CMD_CHECK_DOUBLE_NAME_WHERE_OK;
			packet.u.kein.default_short_int = server_id;
		packet.h.header.size = sizeof( short int );
		SendPacket2Map( &packet, port );
	}
}

void RecvCheckDoubleNameWhenFirstOk( t_packet *p, short int cn )
{
	int server_id = p->u.kein.default_short_int;
	if( CheckServerIndex( server_id ) )
	{
		connections[server_id].chrlst.updatable = 0; // CSD-TW-030620
		closeconnection( connections, server_id, -102 );
	}
}

void CheckDoubleNameWhenFirst( char *pName, short int server_id )
{
	int bClose = 0;
	int target_id = ExistHe( pName );
	if( target_id > 0 )
	{
		bClose = 1;
	}
	else
	{
		target_id = ExistConnectingHe( pName );
		if( target_id > 0 )
		{
			bClose = 1;
		}
	}
	if( bClose )
	{
		connections[target_id].chrlst.updatable = 1; // CSD-TW-030620
		closeconnection( connections, target_id, -101 );

		connections[server_id].chrlst.updatable = 0; // CSD-TW-030620	
		closeconnection( connections, server_id, -102 );
	}

	t_packet packet;
	packet.h.header.type = CMD_CHECK_DOUBLE_NAME_WHERE_FIRST;
		strcpy( packet.u.kein.check_double_when_first.name, connections[server_id].name );
		packet.u.kein.check_double_when_first.server_id = server_id;
		packet.u.kein.check_double_when_first.port = GetOwnPort();
	packet.h.header.size = sizeof( k_check_double_when_first );
	SendPacket2Map( &packet, ST_SEND_MAP_ALL );
}

//040720_KJHuNs g_pLogManager로 이동(정리목적)
// 030923 HK YGI
void SaveLogChange_LoginLogoutByKein(CHARLIST* pTarget, int type, int call_function_id )
{
	if (!g_EventFlagMgr.IsSaveLogWhenSaveUserData())
	{
		return;
	}

	char type2str[2][20] = {"LOGOUT", "LOGIN"};
	
	string strPath;

	if (g_pLogManager->GetLogPath(LT_CHANGE, strPath))
	{
		char temp[FILENAME_MAX] = {0,};
		char log[FILENAME_MAX] = {0,};
		::sprintf(temp, "%s/savelog_%s.txt", strPath.c_str(), pTarget->Name);
		::sprintf(log, "%s(%d): [%s][%s] lv=%d, exp=(%d)%d, class=%d",
			      type2str[type], call_function_id, MapName,
				  pTarget->Name, pTarget->GetLevel(), pTarget->GetExperienceStep(), pTarget->Exp, pTarget->Class);
		SaveLogDefault2(temp, log);
	}
}


// 031009 CI YGI
int InitItemLimitCountFromDB( )
{
	char query[512];
	WORD port = GetOwnPort();
	if( !port )
	{
		return 0;
	}
	DWORD limit_number;
	int ret = GetItemLimitCount_SQL( port, &limit_number );
	if( ret < 1 )
	{
		limit_number = 1;
		sprintf( query, "INSERT INTO ItemLimitMgrTable ( port, limit_number ) VALUES ( %d, %d )", port, limit_number );
		Querry_SQL( query );
	}

	ItemMgr.SetLimitCount( limit_number );

	ret = ItemMgr.LoadHaveLimitItem();
	if( !ret )		// 실패
	{
		JustMsg( "Error!!! check please ItemList_HaveLimit table" );
		return 0;
	}
	return 1;
}

// 040105 YGI 보물 ----------------
int GetTreasureMining( int lv )
{
	switch( lv )
	{
		case 1: return 30;
		case 2: return 50;
		case 3: return 70;
		case 4: return 99;
	}
	return 101;
}
void RecvCheckTreasureXY( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	int event_index = -1;

	k_check_treasure_xy *pData = (k_check_treasure_xy *)p->u.data;
	int tile_x = pData->x/TILE_SIZE;
	int tile_y = pData->y/TILE_SIZE;
	int lv = 0;
	bool mining = false;		// 기술이 부족하다

	for( int a=0; a<3; a++ )
	{
		for( int b=0; b<3; b++ )
		{
			for( int c=0; c<8; c++ )
			{
				ItemAttr *pItem = &ch->inv[a][b][c];
				lv = IsTreasureMapItem( pItem->item_no );
				if( !lv ) continue;		// 보물 지도가 아니다.
				CTreasureMapItem *pAttr = (CTreasureMapItem *)&pItem->attr[IATTR_TREASURE_MAP];
				if( !pAttr->isChecked || pAttr->mapnumber != MapNumber ) continue;
				if( ch->skillexp[MINING].skillexp/10000 < GetTreasureMining( lv ) )
				{
					mining = false;
					continue;
				}
				if( pAttr->x != tile_x || pAttr->y != tile_y ) continue;
				event_index = g_EventFindTreasure.CheckXY( tile_x, tile_y );
				if( event_index == -1 ) continue;
				// 마이너 레벨
				mining = true;
				
				// 사용한것은 다시 사용하지 못하게 삭제 한다.
				//pAttr->use = 1;
				POS pos;
				SetItemPos( INV, a, b, c, &pos );
				SendDeleteItem( pItem, &pos, ch );
				//SendServerEachItem( &pos, pItem, cn );
				goto find_stop;
			}
		}
	}

find_stop : 
	if( event_index == -1 ) return;
	if( lv == 0 ) return;
	if( !mining ) 
	{
		SendPutMenuString( KM_FAIL, 215, cn );
		return;
	}

	if( g_TreasureGuardMgr.IsEvent( event_index ) ) return;	// 이미 등장했었는지 확인
	int treasure_class = lv-1;		// 튀어 나와야 할 몹과 나중에 보물 상자의 등급
	int ret = g_TreasureGuardMgr.CreateGuard( event_index, treasure_class, tile_x, tile_y, cn );
	if( ret )
	{
		MyLog( 0, "Treasure Guard Event Start <event_index:%d>, <treasure_class:%d>, (%d,%d) <%s>",
			event_index, treasure_class, tile_x, tile_y, ch->Name );
	}
} //////////////////////////

void RecvGetTreasureMapDetail( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	POS *pPos = &p->u.kein.default_pos;
	ItemAttr *item = GetItemByPOS( ch, *pPos );
	if( !item ) return;

	int lv = IsTreasureMapItem( item->item_no );
	if( !lv ) return;		// 보물 지도가 아니다.

	CTreasureMapItem *pAttr = (CTreasureMapItem *)&item->attr[IATTR_TREASURE_MAP];
	if( pAttr->isChecked) return;

	t_packet packet;
	packet.h.header.type = CMD_GET_TREASURE_MAP_DETAIL_DB;
		packet.u.kein.get_treasure_map_detail_to_db.cn = cn;
		packet.u.kein.get_treasure_map_detail_to_db.pos = *pPos;
	packet.h.header.size = sizeof( k_get_treasure_map_detail_to_db );
	QueuePacket( connections, DB_DEMON, &packet, 1 );
}

void RecvGetTreasureMapDetailDB( t_packet *p, short int cn )
{
	k_get_treasure_map_detail_to_map *pData = &p->u.kein.get_treasure_map_detail_to_map;
	CHARLIST *ch = CheckServerId( pData->cn );
	if( !ch ) return;

	ItemAttr *item = GetItemByPOS( ch, pData->pos );
	if( !item ) return;
	int lv = IsTreasureMapItem( item->item_no );
	if( !lv ) return;		// 보물 지도가 아니다.
	CTreasureMapItem *pAttr = (CTreasureMapItem *)&item->attr[IATTR_TREASURE_MAP];
	if( pAttr->isChecked ) return;

	pAttr->isChecked = 1;
	pAttr->mapnumber = pData->map_number;
	//pAttr->use = 0;
	// lv에 따라 랜덤하게...
	pAttr->x = pData->x;
	pAttr->y = pData->y;
	switch( lv )
	{
		case 1 : pAttr->offset2 = 0; pAttr->offset = 0; break;
		case 2 : pAttr->offset2 = rand()%4; pAttr->offset = rand()%2; break;
		case 3 : pAttr->offset2 = rand()%4; pAttr->offset = rand()%3; break;
		case 4 : pAttr->offset2 = rand()%4; pAttr->offset = rand()%4; break;
	}

	SendServerEachItem( &pData->pos, item, pData->cn );
}

///////////////////////////////////////////////////////////////////////////////////////
// 아이템 몰 관련 함수들 // 040601 YGI

//<050408_KCH
//<050224_KCH 마일리지몰 작업	//해당계정의 받아야하는 아이템몰 아이템의 목록
int GetItemMallItemIndexAndCount( const char* pLogin_Id, int &max, DWORD *index )
{
	char		szQuerry[1024];
	HSTMT		hStmt;
	RETCODE		retCode;
	SDWORD		cbValue;

	char condition[256] = {0,};
	sprintf( condition, "recv_loginid = '%s' AND recv_type = '%d'", pLogin_Id,Wait_ItemMallItem );

	max = 0;	
	GetRowLineOfSQL(TOTAL_DB, "Item_to_Game", "*", &max, condition);
	if( max <= 0 )
	{
//		SendPutMenuString( cn, KM_FAIL, 218, pLogin_Id );	//지급 받을 아이템이 없습니다.
		return -1;		// 아이템 몰에서 구입한 아이템이 없다.
	}

	const int MAX_TOP_GET_ITEM_MALL = 30;
	if( max > MAX_TOP_GET_ITEM_MALL ) max = MAX_TOP_GET_ITEM_MALL; //최대 30개까지 가질수 있다.
	
	sprintf( szQuerry, "SELECT top %d idx FROM Item_to_Game WHERE recv_loginid = '%s' AND recv_type = '%d' ORDER BY idx", MAX_TOP_GET_ITEM_MALL,pLogin_Id,Wait_ItemMallItem );
	SQLAllocStmt(g_hDBC_TotalDB, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		retCode  = SQLFetch(hStmt);
		int count = 0;
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			retCode = SQLGetData(hStmt, 1, SQL_C_LONG, &index[count++], 0, &cbValue);
			retCode  = SQLFetch(hStmt);
		}
		SQLFreeStmt(hStmt, SQL_DROP);
		return 1;
	}

	SQLFreeStmt(hStmt, SQL_DROP);
	return -1;
}

int GetItemMallItem_AB( k_ItemMall *pItemList, const char *pLogin_Id, int nIdx)
{
	SDWORD		cbValue;
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[255];
	int			cItemCount = 0;

	sprintf( szQuerry, "SELECT top 15 [idx], [itemid] FROM Item_to_Game "
		"WHERE recv_loginid = '%s' AND recv_type = '%d' AND idx >= %d ORDER BY idx",
		pLogin_Id, Wait_ItemMallItem, nIdx );

	SQLAllocStmt(g_hDBC_TotalDB, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		retCode = SQLFetch(hStmt);
		while(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			retCode = SQLGetData(hStmt, 1, SQL_C_LONG, &pItemList[cItemCount].itemmall_index, 0, &cbValue);
			retCode = SQLGetData(hStmt, 2, SQL_C_LONG, &pItemList[cItemCount].itemMallItemKind_index, 0, &cbValue);

			retCode = SQLFetch(hStmt);

			++cItemCount;
		}
	}
	SQLFreeStmt(hStmt, SQL_DROP);

	return cItemCount;
}

//050224_KCH 마일리지몰 작업 초기 해당 아이템몰 아이템의 인덱값만 보내준다.
void RecvGetItemMallItem_ABFirst( t_packet *p, short int cn )
{
	//길드 아이템을 참고로 했음.(하지만, DBDemon과 Client의 직접연결은 신뢰할수없는 패킷이므로, Map에서 담당시킴)
	t_packet packet;
	ZeroMemory(&packet.u.kein.get_item_mall_first,sizeof(k_get_item_mall_first));
	k_get_item_mall_first* pData = &packet.u.kein.get_item_mall_first;

	int ret = GetItemMallItemIndexAndCount( connections[cn].id, pData->max, packet.u.kein.get_item_mall_first.item_index );
	if( ret == 1 )
	{
		packet.h.header.type = CMD_GET_ITEM_MALL_AB_FIRST;
		packet.h.header.size = sizeof( k_get_item_mall_first );
		QueuePacket( connections, cn, &packet, 1 );
	}
}

// 특정 인텍스 값에서 해당 아이템몰 아이템의 다음 15개의 아이템을 보내준다.
void RecvGetItemMallItem_AB( t_packet *p, short int cn )
{
	const k_req_guild_item *pItemMallItem_AB = &p->u.kein.req_item_mall_item;
	
	t_packet packet;
	ZeroMemory(&packet.u.kein.get_item_mall_item,sizeof(k_get_item_mall_item));

	k_get_item_mall_item* pData = &packet.u.kein.get_item_mall_item;

	vector<k_ItemMall> vtItemMall;
	vtItemMall.reserve(15);			//ItemMall AccountBox에 한페이지엔 15개의 아이템만 보여줄수 있음.
	//pData->count = GetItemMallItem_AB(vtItemMall.begin(), connections[cn].id, pItemMallItem_AB->index);
	pData->count = GetItemMallItem_AB(vtItemMall.data(), connections[cn].id, pItemMallItem_AB->index);
	_ASSERT(15 >= pData->count);

	if (0 < pData->count)
	{
		for( int i=0; i<pData->count; ++i )
		{
			int nIM_IK_Idx = vtItemMall[i].itemMallItemKind_index;
			if(!nIM_IK_Idx)	{ continue; }	// 인덱스가 0이면 내용없는 아이템이 리턴

			if(nIM_IK_Idx != g_ItemMallItem[nIM_IK_Idx-1].iIndex)	// EventRareItem 테이블의 인덱스가 잘못 정렬되어있다
			{
				// 여기서 테이블이 잘못되었다는 로그를 남겨주자
				MyLog(0,"ItemMall: ItemMallItemKind Table is ERROR!!! Check plz [MakeItemMallItem]");
				continue;		// 역시 빈 아이템 리턴
			}

			LPItemMallItem pEI = &g_ItemMallItem[nIM_IK_Idx-1];
			if( !pEI->iItemNo) 	{ continue; }	//테이블 자체에 값이 없네..

			ItemAttr item = GenerateItem( pEI->iItemNo );//아이템을 만들어 주고
			if( !item.item_no) { continue; }	//아이템 넘버가 없네? ㅡㅡ;;


			// 아이템 몰에서 받아온 속성을 넣어 준다.
			SetAttr2( item.attr[IATTR_ATTR], IA2_ITEMMALL_ITEM);

			// 아이템 몰 특유의 속성 넣어 주기
			if(pEI->iRareKind1)//1번째 속성이 있는 것이라면////레어라면 레어를 주고
			{//레어를 생성해 본다
				//040909_KCH C4800 Warning 제거 
				ItemMgr.MakeRareAttr(item.attr[IATTR_RARE_MAIN],pEI->iGrade,pEI->iRareKind1,pEI->iRareKind2,pEI->iRareKind3,
					pEI->iHighRare,( (pEI->isDynamic) !=0));
			}

			//<050224_KCH 마일리지몰 작업 DB의 인덱스값을 넣어준다.(나중에 Limit값과 같이 복템구별용.)
			item.attr[IATTR_ITEM_MALL_IDX] = vtItemMall[i].itemmall_index;
			pData->index[i] = vtItemMall[i].itemmall_index;

			char szQuerry[2048] = {0,};

			//만약 ItemMallItemKind에 시간이 세팅되어 있다면, Item_to_Game 테이블에 세팅할때, 사용시작,사용종료 시간을 세팅해주어야한다.
			sprintf( szQuerry, "UPDATE Item_to_Game SET item_limit = '%d' WHERE [idx] = '%d'",
				item.attr[IATTR_LIMIT], 
				item.attr[IATTR_ITEM_MALL_IDX]  );

			if (-1 == Querry_SQL(szQuerry, g_hDBC_TotalDB ))
				MyLog(0,"ItemMall: UPDATE Item_to_Game's [Query] ERROR!!! Check plz (RecvGetItemMallItem_AB)");

			// 아이템이 완성되었다
			memcpy(&pData->item[i],&item,sizeof(ItemAttr));
			
		}
		
		packet.h.header.type = CMD_GET_ITEM_MALL_AB;
		packet.h.header.size = sizeof( k_get_item_mall_item );
		QueuePacket( connections, cn, &packet, 1 );
	}
}
//>050408_KCH

void RecvGetItemMalltoDB( t_packet *p, short int cn )
{
	CHARLIST *pCh = CheckServerId( cn );
	if( !pCh ) return;
	const DWORD dwIndex = p->u.kein.default_dword;

	t_packet packet;
	packet.h.header.type = CMD_GET_ITEM_MALL;
		packet.u.kein.get_item_mall.dwItemmall_Index= dwIndex;
		strcpy( packet.u.kein.get_item_mall.login_id, connections[cn].id );
		strcpy( packet.u.kein.get_item_mall.name, pCh->Name );
	packet.h.header.size = sizeof( k_get_item_mall ) - 20+ strlen( pCh->Name); 
	QueuePacket( connections, DB_DEMON, &packet, 1 );

	return;
}

// 040602 YGI
void SendReturnResultForItemMall( k_send_item_mall_item_kind *pData, const char success_type )
{
	// DB TBL에 결과를 저장한다.

	// DB로 보낸다.
	t_packet packet;
	packet.h.header.type = CMD_ITEM_MALL_RESULT;
		packet.u.kein.item_mall_result_to_db.itemmall_index			= pData->itemmall_index;
		packet.u.kein.item_mall_result_to_db.itemMallItemKind_index	= pData->itemMallItemKind_index;
		packet.u.kein.item_mall_result_to_db.itemmall_limit			= (success_type)?pData->item.attr[IATTR_LIMIT]:0;
		packet.u.kein.item_mall_result_to_db.result_type			= success_type;
	packet.h.header.size = sizeof( k_item_mall_result_to_db );

	QueuePacket( connections, DB_DEMON, &packet, 1 );

	// 클라이언트로 보낸다.
}

//050408_KCH 쿼리 에러시 보여준다.
void displaySQLError(SQLHSTMT hStmt, char *szQuery = NULL)
{
	SQLCHAR err[80] = {0, } ;
	SQLINTEGER nep = 0 ;
	SQLCHAR msg[255] = { 0, } ;
	SQLSMALLINT num = 0 ;
	
	int retcode = SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, err, &nep, msg, 254, &num) ;
	
	if(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		MyLog(0, "SQL Error MSG: %s; %s", err, msg);
	}
	
	if( szQuery )
	{
		char query[100];
		strncpy( query, szQuery, 99 );
		query[99] = 0;
		
		MyLog(2, "query Error : %s", query );
	}
}

void RecvItemMallItemList( t_packet *p, short int cn )
{
	k_send_item_mall_item_kind *pData = &p->u.kein.send_item_mall_item_kind;
	
	if (NULL == pData->name) 
	{
		SendReturnResultForItemMall( pData, Wait_ItemMallItem );		// 실패
		return ;
	}	//접속중인 맵에 해당유저가 없다면.. 처리하지 않는다.

	CHARLIST *pCh = CheckServerName(pData->name);
	if( !pCh || strcmp( pData->name, pCh->Name ) != 0 )
	{
		SendReturnResultForItemMall( pData, Wait_ItemMallItem );		// 실패
		return;
	}

	// 아이템 몰에서 받아온 속성을 넣어 준다.
	SetAttr2( pData->item.attr[IATTR_ATTR], IA2_ITEMMALL_ITEM);

	int a, b, c;
	a=b=c=0;
	CItem *pItemCls = ::ItemUnit( pData->item.item_no);
	//인벤토리에 Limit번호와, ItemMall_Idx와 똑같은 녀석이 있는지 체크하자.(복템삭제)
	//빈 인벤토리를 찾아서 반환 한다.
	_eRT_INV_ITEMMALL ret = SearchInvForItemMall( pCh, a, b, c, pData->item );
	if( FULL_INV == ret ) 
	{
		SendReturnResultForItemMall( pData, Wait_ItemMallItem );	// 실패
		SendPutMenuString( KM_FAIL, 216, pCh->GetServerID());		//216	인벤토리에 빈 자리가 없습니다.
		return;
	}
	else if (DUPE_ITEM_INV == ret)	//아이템몰 복템이 인벤에 존재한다면..
	{
		POS pos;
		SetItemPos( INV, a, b, c, &pos );

		SendItemEventLog( &pCh->inv[a][b][c], cn, SN_NOT_USER, SILT_DELETE_ITEM_MALL_DUPE, 100 );
		SendDeleteItem( &pCh->inv[a][b][c], &pos, pCh, 0 );

		SendReturnResultForItemMall( pData, Wait_ItemMallItem );		//아이템 지급 실패(처음부터)
		
		char szLog[256] = {0.};
		sprintf(szLog ,
			"[%d]DupeItem Delete [%s]님의 마일리지 아이템	\
%s[%d](itemmall_index:%d) Is Delete",
			g_pServerTable->GetOwnServerData()->wPort,
			pData->name,
			pItemCls->GetHanName(),
			pData->item.item_no,
			pData->itemmall_index);
		g_pLogManager->ItemMallLog(szLog);
		return;
	}

	//엉뚱한 결과값이 들어오면 처리하지 않는다.
	if (HOLD_ON_SUPPLY_INV_ItemMallItem != pData->result_type && HOLD_ON_Progressive_ItemMallItem != pData->result_type)
	{
		SendReturnResultForItemMall( pData, Wait_ItemMallItem );		// 실패

		char szLog[256] = {0.};
		sprintf(szLog ,
			"[%d][%s]님의 마일리지 아이템	\
%s[%d](itemmall_index:%d)을 [%s] type Is Wrong",
			g_pServerTable->GetOwnServerData()->wPort,
			pItemCls->GetHanName(),
			pData->name,
			pData->item.item_no,
			pData->itemmall_index,
			pData->result_type);
		g_pLogManager->ItemMallLog(szLog);

		return;
	}

	//인벤토리에 아이템을 넣어주자.
	POS pos;
	SetItemPos( INV, a, b, c, &pos );
	pCh->inv[a][b][c] = pData->item;
	SendServerEachItem( &pos, &pData->item, pCh->GetServerID());

	SendItemEventLog( &pData->item, pCh->GetServerID(), SN_NOT_USER , SILT_RECEIVE_ITEM_MALL, 100 ); //YGI acer

	//이 두가지 타입이 들어오게 된다. HOLD_ON_SUPPLY_INV_ItemMallItem,HOLD_ON_Progressive_ItemMallItem
	SendReturnResultForItemMall( pData, pData->result_type);		// 성공
	
	SendPutMenuString( KM_OK, 217, pCh->GetServerID(),pItemCls->GetHanName() );	//아이템 몰에서 구입한 아이템을 받았습니다.

	char szLog[256] = {0.};
	sprintf(szLog ,
		"[%d][%s]님의 마일리지 아이템	\
%s[%d](itemmall_index:%d)을 성공적으로 받았습니다.",
		g_pServerTable->GetOwnServerData()->wPort,
		pData->name,
		pItemCls->GetHanName(),
		pData->item.item_no,
		pData->itemmall_index);
	g_pLogManager->ItemMallLog(szLog);
	
	//050408_KCH 만약 지급후, 맵이 다운되어서 정보를 저장못한다면, 아이템이 날아가버림,그래서 아이템 지급 받는 시점에 정보를 DB에 저장하자.
	if(!::CanSaveUserData(pCh,1))
	{
		char szLog[256] = {0.};
		sprintf(szLog ,
			"[%d][%s]님의 마일리지 아이템	\
	%s[%d](itemmall_index:%d)을 저장시 ERROR[CanSaveUserData 실패.]",
			g_pServerTable->GetOwnServerData()->wPort,
			pData->name,
			pItemCls->GetHanName(),
			pData->item.item_no,
			pData->itemmall_index);
		g_pLogManager->ItemMallLog(szLog);
		return ;
	}

	SQLRETURN retcode;
	SQLHSTMT  hstmt;
	char query[2048] = {0,};

	if( strcmp( pCh->Name, connections[ pCh->GetServerID()].name ) )
	{
		MyLog( LOG_NORMAL, "Error :(a) chr->Name = %s   c[ char_id].name = %s", pCh->Name, connections[ pCh->GetServerID()].name );
		char szLog[256] = {0.};
		sprintf(szLog ,
			"[%d][%s]님의 마일리지 아이템	\
	%s[%d](itemmall_index:%d)을 저장시 ERROR[connection의 캐릭명이 다름]",
			g_pServerTable->GetOwnServerData()->wPort,
			pData->name,
			pItemCls->GetHanName(),
			pData->item.item_no,
			pData->itemmall_index);
		g_pLogManager->ItemMallLog(szLog);
		return;
	}
	
	SQLAllocStmt(g_hDBC_DragonDB, &hstmt);

	retcode = SQLExecDirect(hstmt, (UCHAR *)query, SQL_NTS);
	if(retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) 
	{	
		displaySQLError(hstmt) ;
		//SQLTransact(SQL_NULL_HENV, g_hDBC_DragonDB, SQL_ROLLBACK);
		SQLFreeStmt(hstmt, SQL_DROP);
		return;
	}	
	else {
		//SQLTransact(SQL_NULL_HENV, g_hDBC_DragonDB, SQL_COMMIT);
		SQLFreeStmt(hstmt, SQL_DROP) ;
	}	
		
	if( update_BinaryData_to_Chr_Info(	(UCHAR *)pCh->Ws, 
		(UCHAR *)pCh->Ps, 
		(UCHAR *)pCh->Skill, 
		(UCHAR *)pCh->skillexp, 
		(UCHAR *)pCh->tac_skillEXP,
		(UCHAR *)var[ pCh->GetServerID()],
		(UCHAR *)pCh->inv, 
		(UCHAR *)pCh->equip, 
		(UCHAR *)pCh->quick, 
		(UCHAR *)pCh->party_str, 
		(UCHAR *)pCh->relation_str, 
		(UCHAR *)pCh->employment_str, 
		(UCHAR *)pCh->Item, 
		connections[pCh->GetServerID()].id, connections[pCh->GetServerID()].name) < 0) 
	{		//1219 YGI
		printf("\n Update Character: Update BIN Error") ;
		char szLog[256] = {0.};
		sprintf(szLog ,
			"[%d][%s]님의 마일리지 아이템	\
	%s[%d](itemmall_index:%d)을 저장시 ERROR[Update Character: Update chr_info BIN Error]",
			g_pServerTable->GetOwnServerData()->wPort,
			pData->name,
			pItemCls->GetHanName(),
			pData->item.item_no,
			pData->itemmall_index);
		g_pLogManager->ItemMallLog(szLog);
		return;
	}
	else 
	{
		if( update_BinaryData_to_Chr_Info2(  (UCHAR *)pCh->bank, connections[pCh->GetServerID()].id, connections[pCh->GetServerID()].name) < 0) 
		{
			char szLog[256] = {0.};
			sprintf(szLog ,
				"[%d][%s]님의 마일리지 아이템	\
		%s[%d](itemmall_index:%d)을 저장시 ERROR[Update Character: Update chr_info2 BIN Error]",
				g_pServerTable->GetOwnServerData()->wPort,
				pData->name,
				pItemCls->GetHanName(),
				pData->item.item_no,
				pData->itemmall_index);
			g_pLogManager->ItemMallLog(szLog);
			return;
		}
	}
	
}
//////////////////////////////////////////////////////////////////////////////////////////////

// 040602-3 YGI
void SendItemDropMessage( short int cn, short int item_no )
{
	t_packet packet;
	packet.h.header.type = CMD_ITEM_DROP_MESSAGE;
		packet.u.kein.default_short_int = item_no;
	packet.h.header.size = sizeof( short int );

	QueuePacket( connections, cn, &packet, 1 );
}

// 040621 YGI
void RecvItemRepairByItem( t_packet *p, short int cn )		
{
	CHARLIST *pCh = CheckServerId( cn );
	if( !pCh ) return;

	CItem *pSourceItemUnit;		// 내구도 수리용 아이템 몰 아이템의 속성가져오기 위해서
	CItem *pTargetItemUnit;		// 수리하기 위한 타겟 아이템의 속성을 가져오기 위해서

	// 대상 아이템이 수리 가능한 아이템인지 확인하자
	POS *pPos = &p->u.kein.default_pos;
	ItemAttr *pTargetItem = GetItemByPOS( pCh, *pPos );
	if( !pTargetItem->item_no ) return;

	pTargetItemUnit = ItemUnit( *pTargetItem );
	if( !pTargetItemUnit ) return;
	if( !pTargetItemUnit->GetRepairAble() ) return;		// 수리 가능한 아이템이 아니다.

	// 수리 기능을 사용할 수 있는 아이템을 가지고 있는지 찾기
	bool bFind = false;
	
	_DeleteToExpireItemMallItem _ItemMall;
	_ItemMall.pCharList		= pCh;
	_ItemMall._ItemPosType	= INV;			//구조체에 세팅후에, 호출.

	int a, b, c;
	for( a=0; a<3; a++ )
	{
		for( b=0; b<3; b++ )
		{
			for( c=0; c<8; c++ )
			{
				if( !pCh->inv[a][b][c].item_no ) continue;
				pSourceItemUnit = ItemUnit( pCh->inv[a][b][c].item_no );
				if( !pSourceItemUnit ) continue;
				if( pSourceItemUnit->GetRbutton() != ITEM_MALL_ITEM ) continue;
				if( pSourceItemUnit->Getmouse_Mr_Wave() != IMF_REPAIR_ITEM ) continue;
				

				_ItemMall.PosA	= a;
				_ItemMall.PosB	= b;
				_ItemMall.PosC	= c;		//구조체에 세팅후에, 호출.
				if (DeleteToExpireItemMallItem(_ItemMall)) {	break;	}		//만약 기간만료된 아이템이라면 지운다.

				bFind = true;
				break;
			}
			if( bFind ) break;
		}
		if( bFind ) break;
	}

	if( !bFind ) return;		// 해당 아이템이 없다.


	// 회복용 아이템은 삭제하고
	POS source_pos;
	SetItemPos( INV, a, b, c, &source_pos );
	SendItemEventLog( &pCh->inv[a][b][c], cn, SN_NOT_USER, SILT_DELETE_ITEM_MALL, 100 );
	SendDeleteItem( &pCh->inv[a][b][c], &source_pos, pCh, 0 );
	
	// 대상 아이템의 최대 내구도를 복구 시켜준다.
	pTargetItem->attr[IATTR_DURATION] = MAKELONG( pTargetItemUnit->GetDuration(), pTargetItemUnit->GetDuration() );
	SendServerEachItem( pPos, pTargetItem, cn );

	SendPutMenuString( KM_OK, 224, cn );		//내구도가 회복 됐습니다.
}

const char* ConvertToItemMallKind(int nRet)
{
	BEGIN_SELECT(nRet)
		CASE_SELECT(IK_CP_POTION)
		CASE_SELECT(IK_ABILITY_RESET)
		CASE_SELECT(IK_DOUBLE_EXP_UP)
		CASE_SELECT(IK_CHANGE_GENDER)
	END_SELECT(UNKNOWN_ItemMallItem)
}

//////////////////////////////////////////////////////////////////////////
//	Return value: 0(테이블이 이미 존재) 1(테이블을생성) -1(테이블생성실패)
//	1년을 주기로 테이블을 생성한다.(ex) item_to_game_2005 테이블 생성.
int CreateBackUPatYearTBL(const char *table_name)
{
	char		szQuerry[1024*2];
	HSTMT		hStmt;
	RETCODE		retCode;

	//1년에 한번씩 테이블을 생성하기위해서 테이블이 있는지 확인하는부분.
	sprintf( szQuerry, "select * from dbo.sysobjects where id = object_id(N'[dbo].[%s]') ", table_name);
	
	SQLAllocStmt(g_hDBC_TotalDB, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if (!SQL_SUCCEEDED(retCode)) { SQLFreeStmt(hStmt, SQL_DROP); return -2; }

	retCode=SQLFetch(hStmt);
	SQLFreeStmt(hStmt, SQL_DROP);

	if ( SQL_NO_DATA == retCode)
	{
		sprintf( szQuerry,	\
		"CREATE TABLE [dbo].[%s] (	\
			[idx] [int] NOT NULL ,	\
			[recv_name] [char] (16) COLLATE Korean_Wansung_CI_AS NOT NULL ,	\
			[recv_serv] [int] NOT NULL ,	\
			[recv_time] [datetime] NOT NULL ,	\
			[recv_loginid] [varchar] (16) COLLATE Korean_Wansung_CI_AS NOT NULL ,	\
			[itemid] [int] NOT NULL ,	\
			[in_itemid] [int] NULL ,	\
			[in_itemcnt] [int] NULL ,	\
			[send_name] [char] (16) COLLATE Korean_Wansung_CI_AS NOT NULL ,	\
			[send_serv] [int] NOT NULL ,	\
			[send_loginid] [varchar] (16) COLLATE Korean_Wansung_CI_AS NOT NULL ,	\
			[message] [varchar] (200) COLLATE Korean_Wansung_CI_AS NULL ,	\
			[period] [int] NULL ,	\
			[is_status] [char] (1) COLLATE Korean_Wansung_CI_AS NULL ,	\
			[ip] [varchar] (15) COLLATE Korean_Wansung_CI_AS NULL ,	\
			[recv_time2] [datetime] NULL ,	\
			[recv_type] [int] NULL ,	\
			[item_limit] [int] NULL ,		\
			[StartUseTime] [datetime] NULL ,	\
			[EndUseTime] [datetime] NULL	\
		) ON [PRIMARY] " , table_name );

		retCode = Querry_SQL( szQuerry,g_hDBC_TotalDB );
	}
	return retCode;
}

//050224_KCH 마일리지몰 작업	사용기간이 끝난 아이템은 백업테이블로 옮긴다.
int ExpireToItem_to_GameTBL(const char *pOrginalTBLName,const int& itemmall_index)
{
	char		szQuerry[1024];
	HSTMT		hStmt;
	RETCODE		retCode;
	char		szDestTBLName[20]	= {0,};
	int			nRetResult = -999;

	CTime curr_time;
	curr_time = CTime::GetCurrentTime();
	int nYear = curr_time.GetYear();

	sprintf( szDestTBLName, "%s_%d",pOrginalTBLName, nYear );

	nRetResult	= CreateBackUPatYearTBL(szDestTBLName);
	if (0 > nRetResult)
		return FailedCreateTBL;	//테이블 생성Failed 비정상적인 실패
	
	//1. 원본 테이블의 기간만료임을 세팅하자.
	sprintf( szQuerry, "if (select idx from [dbo].[%s] where idx = '%d') > 0	\
	  BEGIN																		\
		UPDATE [dbo].[%s] SET recv_type = '%d' WHERE idx = '%d'					\
	  END ",
		pOrginalTBLName,itemmall_index,
		pOrginalTBLName,Expire_TIMESTAMP_ItemMallItem,itemmall_index );
	if (!Querry_SQL( szQuerry, g_hDBC_TotalDB ))
		return FailedEditTBL;	//테이블 수정 실패.

	//2. 백업테이블(item_to_game_2005)에 먼저 Insert 하고.
	sprintf( szQuerry, "if (select idx from [dbo].[%s] where idx = '%d') > 0	\
	  BEGIN																		\
		insert  [dbo].[%s] select * from  Item_to_Game where idx = '%d'			\
	  END ",
		pOrginalTBLName,
		itemmall_index,
		szDestTBLName,
		itemmall_index);

	SQLAllocStmt(g_hDBC_TotalDB, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	SQLFreeStmt(hStmt, SQL_DROP);
	if( !SQL_SUCCEEDED(retCode) )
		return FailedMoveTBL;	//사용기간이 만료된 ItemMallItem 이동를 위한 Insert 실패.

	
	//3. 기존 테이블(item_to_game)에서는 삭제한다.
	sprintf( szQuerry, "if (select idx from [dbo].[%s] where idx = '%d') > 0	\
	  BEGIN																		\
		delete  [dbo].[%s] where idx = '%d'										\
	  END ",
		pOrginalTBLName,
		itemmall_index,
		pOrginalTBLName,
		itemmall_index);

	SQLAllocStmt(g_hDBC_TotalDB, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	SQLFreeStmt(hStmt, SQL_DROP);
	if( !SQL_SUCCEEDED(retCode) )
		return FailedDeleteTBL;	//사용기간이 만료된 ItemMallItem 이동를 위한 delete 실패.

	return SucessedTBL;
}

const char* ConvertToRet(int nRet)
{
	BEGIN_SELECT(nRet)
		CASE_SELECT(FailedCreateTBL)
		CASE_SELECT(FailedEditTBL)
		CASE_SELECT(FailedMoveTBL)
		CASE_SELECT(FailedDeleteTBL)
		CASE_SELECT(SucessedTBL)
	END_SELECT(UNKNOWN_TBL)
}

void ExpireToItemMall(const char* pTBLName, const int nItemMall_Index,
					  const char* ItemClsName,const time_t& _EndUseTime, const char* pRecvName )
{
	//TotalDB의 item_to_game TBL에 결과를 세팅한다.
	int nRet = -999;
	//사용기간 만료 아이템 처리
	nRet = ExpireToItem_to_GameTBL(ITEM_TO_GAME,nItemMall_Index);

	//결과는 상관없이, 무조건 로그를 남긴다.
	FILE*	pFP = NULL;
	char szFileName[512] = {0,};
	mkdir("ItemMallItem_Log");
	sprintf(szFileName,"./ItemMallItem_Log/%4d_%2d_%2d_ItemMallItemLog.log",g_year,g_mon+1,g_day);
	pFP = fopen(szFileName,"at+");
	Dbg_Assert(pFP != NULL);

	CTime curTime(_EndUseTime);
	//기간제한 아이템을 처리한 관련로그를 남기자.
	fprintf(pFP ,"[%s]기간만기아이템삭제[%s], \
itemMall[%6d], EndUseTime:[%2d:%2d:%2d] (%s)\n",
		ItemClsName,
		ConvertToRet(nRet),
		nItemMall_Index,
		curTime.GetHour(),
		curTime.GetMinute(),
		curTime.GetSecond(),
		pRecvName);
	fclose(pFP);
}

//<050224_KCH 마일리지몰 작업	유저의 마일리지 아이템의 사용기간을 체크해서 제거한다.
void RecvItemMallItemDelete_Per1Min(t_packet *p, short int cn )
{
	k_Delete_Item_Mall_ARRAY *pData_ARRAY = &p->u.kein.delete_item_mall_array;
	if (0 > pData_ARRAY->nCnt_DelItemMall) return ;	//초기값 -999 인 음수일경우는 값이 없음으로 Skip

	for( int i=0;i<pData_ARRAY->nCnt_DelItemMall;++i)
	{
		k_Delete_Item_Mall* pData = &pData_ARRAY->_DelItemMall_ARRAY[i];
		CHARLIST* pUser = g_pUserManager->GetCharList(pData->recv_name);
		if (NULL == pUser)	//이 맵에서는 없는 유저이다.
		{
			continue;
		}

		CItem *pItemCls = ::ItemUnit( pData->ItemNo );
		//인벤토리를 검색해서 아이템 삭제.	//기간제한 아이템의 정상적인 삭제.
		int a, b, c;
		for( a=0; a<3; ++a )
		for( b=0; b<3; ++b )
		for( c=0; c<8; ++c )
		{
			ItemAttr *pItem = &pUser->inv[a][b][c];
			if ( pData->itemmall_index	== pItem->attr[IATTR_ITEM_MALL_IDX])
				/*pData->ItemNo == pItem->item_no && pData->item_limit == pItem->attr[IATTR_LIMIT] ) */
			{
				POS pos;
				SetItemPos( INV, a, b, c, &pos );
				if (GetAttr2( pItem->attr[IATTR_ATTR], IA2_ITEMMALL_ITEM))
				{
					SendItemEventLog( pItem, g_pUserManager->GetServerID(pData->recv_name), SN_NOT_USER, SILT_DELETE_ITEM_MALL, 100 );
					SendDeleteItem( pItem, &pos, pUser,2 );

					char szLog[256] = {0.};
					sprintf(szLog ,
						"[%d][%s]님의 마일리지 아이템	\
	%s[%d](itemmall_index:%d)의 사용기간이 끝났습니다.",
						g_pServerTable->GetOwnServerData()->wPort,
						pData->recv_name,
						pItemCls->GetHanName(),
						pData->ItemNo,
						pData->itemmall_index);
					g_pLogManager->ItemMallLog(szLog);
				}
				else
				{
					//비정상적인 테이블 세팅값.
					char szLog[256] = {0.};
					sprintf(szLog ,
						"[%d][%s](ItemKind:%d) attr[IATTR_ATTR][%d]이 마일리지 아이템	\
	%s[%d](itemmall_index:%d)이 아닙니다. 아이템번호 중복 CRITICAL ERR",
						g_pServerTable->GetOwnServerData()->wPort,
						ConvertToItemMallKind(pItemCls->GetItemKind()),
						pItemCls->GetItemKind(),
						pItem->attr[IATTR_ATTR],
						pItemCls->GetHanName(),
						pData->ItemNo,
						pData->itemmall_index);
					g_pLogManager->ItemMallLog(szLog);
					MyLog(0,szLog);
					continue;
				}
			}
			//Skip
		}

		//DB의 테이블을 제거및 이동 처리.
		ExpireToItemMall(ITEM_TO_GAME, pData->itemmall_index,
			pItemCls->GetHanName(),pData->EndUseTime,pData->recv_name);
	}
}

bool DeleteToExpireItemMallItem( _DeleteToExpireItemMallItem& _ItemMall )
{
	CHARLIST* pChar = _ItemMall.pCharList;
	ItemAttr *pItem = &pChar->inv[_ItemMall.PosA][_ItemMall.PosB][_ItemMall.PosC];

	if (GetAttr2( pItem->attr[IATTR_ATTR], IA2_ITEMMALL_ITEM) )
	{
		//				'SQl DB TBL에 1부터 순차적으로 인덱스가 Unique하게 세팅되어 있다는 전제.
		if (0 < pItem->attr[IATTR_ITEM_MALL_IDX] )
		{
			CTime curTime	=	CTime::GetCurrentTime();
			LP_ITEM_MALL_TIME lpTime = reinterpret_cast<LP_ITEM_MALL_TIME>(&pItem->attr[IATTR_ITEM_MALL_EndUseTime]);

			if (lpTime->month)	//사용기간이 세팅되어 있는 아이템이라면..
			{
				CTime ItemMallEndTime(2000+lpTime->year,lpTime->month,lpTime->day,lpTime->hour,lpTime->minute,0 );
				CTimeSpan ItemMallUseTime = ItemMallEndTime - curTime;
				
				//시간 세팅이 잘못되어 있거나, 기간이 지난 아이템일 경우, 삭제후, 로그를 남기자.
				if (0 >ItemMallUseTime.GetTotalMinutes())
				{
					CItem *pItemCls = ::ItemUnit( pItem->item_no );

					//아래의 순서 지킬것.(현재 캐릭의 인벤에서 아템정보를 얻어오기 때문에, 먼저 인벤아템을 지우면 안됨)
					//1.DB에도 확인해서 있으면 먼저 TBL을 제거 하자.
					ExpireToItemMall(ITEM_TO_GAME,pItem->attr[IATTR_ITEM_MALL_IDX],
						pItemCls->GetHanName(),ItemMallEndTime.GetTime(),pChar->Name);

					//2.ItemMall_Idx가 같다면 복템이 인벤에 존재한다.(Limit는 지급 시간을 기준으로 하므로 테이블수정에 따라 달라질수도 있다.
					POS pos;
					SetItemPos( INV, _ItemMall.PosA, _ItemMall.PosB, _ItemMall.PosC, &pos );

					SendItemEventLog( pItem, g_pUserManager->GetServerID(pChar->Name), SN_NOT_USER, SILT_DELETE_ITEM_MALL_DUPE, 100 );
					SendDeleteItem( pItem, &pos, pChar,2 );

					char szLog[256] = {0.};
					sprintf(szLog ,	"[%d]DupeItem Delete [%s]님의 마일리지 아이템	\
	%s[%d](itemmall_Idx:%d) Is Delete(Time Stuff Problem)",
						g_pServerTable->GetOwnServerData()->wPort,
						pChar->Name,
						pItemCls->GetHanName(),
						pItem->item_no,
						pItem->attr[IATTR_ITEM_MALL_IDX]);
					g_pLogManager->ItemMallLog(szLog);

					return true;
				}
			}
		}
	}
	return false;
}

void DeleteToExpireItemMallItem_byINV(CHARLIST& _CharList)
{
	//기타 사용기간이 끝난 아이템몰아이템이 인벤에 있을경우에, 지우고 로그를 남긴다.
	_DeleteToExpireItemMallItem _ItemMall;
	_ItemMall.pCharList		= &_CharList;
	_ItemMall._ItemPosType	= INV;			//구조체에 세팅후에, 호출.

	for( int i=0; i<3; ++i)
	for( int j=0; j<3; ++j)
	for( int k=0; k<8; ++k)
	{
		_ItemMall.PosA	= i;
		_ItemMall.PosB	= j;
		_ItemMall.PosC	= k;		//구조체에 세팅후에, 호출.

		DeleteToExpireItemMallItem(_ItemMall);
	}
}

void RecvItemMallItemDelete_Client( t_packet *p, short int cn )		
{
	CHARLIST *pCh = CheckServerId( cn );
	if( !pCh ) return;

	// 대상 아이템이 아이템몰 아이템인지 확인하자
	POS *pPos = &p->u.kein.default_pos;
	ItemAttr *pTargetItem = GetItemByPOS( pCh, *pPos );
	if( !pTargetItem->item_no ) return;

	int a = pPos->p1;
	int b = pPos->p2;
	int c = pPos->p3;

	_DeleteToExpireItemMallItem _ItemMall;
	_ItemMall.pCharList		= pCh;
	_ItemMall._ItemPosType	= static_cast<ItemPosType>(atoi(&pPos->type));	//구조체에 세팅후에, 호출.

	if( GetAttr2( pTargetItem->attr[IATTR_ATTR], IA2_ITEMMALL_ITEM ) ) 
	{
		_ItemMall.PosA	= a;
		_ItemMall.PosB	= b;
		_ItemMall.PosC	= c;		//구조체에 세팅후에, 호출.

		DeleteToExpireItemMallItem(_ItemMall);	//만약 기간만료된 아이템이라면 지운다.
	}
}
//>050224_KCH 마일리지몰 작업
