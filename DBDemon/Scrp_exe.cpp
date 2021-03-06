﻿#include "StdAfx.h"
#include "MAIN.H"

#include "Scrp_int.h"
#include "Map.h"
#include "MenuDefine.h"
#include "MenuServer.h"

#define __SCRP_EXE_H__
#include "Scrp_exe.h"


int var[DRAGON_MAX_CONNECTIONS][_SCRIPT_VARABLE_POINT_];
char String[MAX_STRING_];

int EventPC, EventNPC;

#ifdef _GAME_SERVER_

void ReCallScript(void)
{
	int i;
	static DWORD time;

	for (i = 0; i < DRAGON_MAX_CONNECTIONS; i++)
	{
		if (var[i][1])
		{
			if (connections[i].socket && connections[i].state >= CONNECT_JOIN)
			{
				if (var[i][3] < global_time)
				{
					Script(var[i][1]);
				}
			}
		}
	}
}





//
//窃荐疙 : void sDialogD( void )
//汲  疙 : 措拳甫 窍绰单 措拳窍绰 磊狼 规氢捞 林牢傍阑 氢茄促.
//牢  磊 :
//府  畔 :
//
void sDialOnlyPC(void)// 0
{
	int length;
	t_packet packet;

	// Client俊 措拳甫 焊郴霖促....
	GetString(String);

	length = strlen(String);
	packet.h.header.type = CMD_CHAT_DATA;
	{
		packet.u.server_chat_data.server_id = EventNPC + 10000;
		packet.u.server_chat_data.length = (unsigned char)length;
		memcpy(packet.u.server_chat_data.text, String, length);
	}
	packet.h.header.size = sizeof(t_server_chat_data) - TEXT_LENGTH + length;

	if (connections[EventPC].socket && connections[EventPC].state >= CONNECT_JOIN)
		QueuePacket(connections, EventPC, &packet, 1);

	return;
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
	int itemno, x, y;

	itemno = GetInt();
	x = GetInt();
	y = GetInt();

	AddItemList(itemno, 0, 0, 0, x, y);
}


void sAddNpc(void)// 2
{
	int npcindex, x, y;

	npcindex = GetInt();
	x = GetInt();
	y = GetInt();

	AddNPCList(npcindex, x, y);

	//int i,j;
	//for( i = 0 ; i < 360 ; i += 20 )
	//{
	//	for( j = 0 ; j < 360 ; j += 15 )
	//	{								
	//		AddNPCList( npcindex, i, j);
	//	}
	//}
}



void sMapName(void)// 3
{
	GetString(MapName);
}


void sMapMove(void)// 4
{
	t_packet packet;
	char mapname[20];
	int	 x, y;

	GetString(mapname);
	x = GetInt();
	y = GetInt();

	if (connections[EventPC].socket && connections[EventPC].state >= CONNECT_JOIN)
	{
		memcpy(connections[EventPC].mapname, mapname, 20);

		memset(&packet, 0, sizeof(t_packet));

		packet.h.header.type = CMD_CHANGE_MAP;
		packet.h.header.size = sizeof(t_server_change_map);
		memcpy(&packet.u.server_change_map.mapname[0], mapname, 20);
		packet.u.server_change_map.x = x;
		packet.u.server_change_map.y = y;

		if (GetChangeMapInfo_SQL(connections, EventPC, &packet) == 1)
			QueuePacket(connections, EventPC, &packet, 1);
	}
}


void sIsLevel(void)
{
	int lev = GetInt();

	// EventPC狼 饭骇捞 lev焊促 臭栏搁 1甫 sYES俊 持绊 弊犯瘤 臼栏搁 0阑 持绰促. 

	if (connections[EventPC].chrlst.Level >= lev)
	{
		sYES = 1;
	}
	else
	{
		sYES = 0;
	}

}


// Monster啊 惯积且 厘家客 Sprite锅龋甫 沥狼茄促. 
void sInitMonsterGeneratedPosition(void)
{
	int x = GetInt();
	int y = GetInt();
	int npcindex = GetInt();

	NPCGeneratePosition(npcindex, x, y);
}




int callmenudef[72] = { MN_GUILD_INTRODUCTION	,
MN_GUILD_OPERATION		,
MN_GUILD_GUILD1		,
MN_GUILD_GUILD2		,
MN_GUILD_DONE		,
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
MN_SHAMBLES	,
MN_CANDLE	,
MN_SLOPSHOP	,
MN_ALCHEMY	,
MN_WOODWORKING,
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
MN_SHOP_MAIN,
MN_SHOP_SELL,
MN_SHOP_BUY	,
MN_HEAL_MAIN,
MN_HEAL_HEALING,
MN_HEAL_PREVENTING,
MN_HEAL_LEARN,
MN_INN_MAIN,
MN_INN_REST,
MN_INN_SLEEP,			// 69
MN_SYSTEM_LOGOUT };		// 70



void sCallSmallMenu(void)
{
	int index = GetInt();	// 惑痢 List Index...
	int menu = GetInt();	// SmallMenu Index..

	CallSmallMenu(EventPC, index, callmenudef[menu]);
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
	aftertime = GetInt() * 100;

	sRECALL_SCRP_NO = NScriptNo; //    (var[EventPC][1])
	sRECALL_CURTIME = global_time;
	sRECALL_AFTTIME = global_time + aftertime;
}
void sRecallScriptEnd(void)
{
	sRECALL_SCRIPT_COUNT = 0;
	sRECALL_SCRP_NO = 0;
}


//////////////////////////////////////////////////////////////////////////////////////////
void(*SFunction[_SCRIPT_FUNCTION_POINT_])(void)
= {
	sDialOnlyPC,		// 0
	sDialInAreaPC,
	sDialEveryone,
	sAddItem,
	sAddNpc,
	sMapName,
	sMapMove,
	sIsLevel,
	sInitMonsterGeneratedPosition,
	sCallSmallMenu,
	sWeatherOn,				// 10 				// 朝揪俊 包秦 BroadCast茄促. 
	sWeatherOff,			// 11	// 朝揪俊 包秦 BroadCast窍瘤 臼绰促.  
	sRecallScriptStart,		// 12
	sRecallScriptEnd,		// 13


}; // 荤侩磊 沥狼 窃荐甸狼 器牢磐


#else 

void(*SFunction[_SCRIPT_FUNCTION_POINT_])(void);

#endif

