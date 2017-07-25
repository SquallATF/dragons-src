// LogManager.cpp: implementation of the CLogManager class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "LogManager.h"

#include "eventmgr.h" //040720_KJHuNs g_EventFlagMgr사용하기 위함.

#include <direct.h>

//莖샙
#define BEGIN_SELECT(e) switch (e) {
#define CASE_SELECT(e)  case(e): return CS(e);
#define END_SELECT(e)   } return CS(e);
//040715_KJHuNs SaveLogBattle_Ballancing, Ballancing2에서 사용하기 위함.
#include "CItem.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CLogManager::CLogManager()
{
	m_mpBuffer.clear();
}

CLogManager::~CLogManager()
{
	m_mpBuffer.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CLogManager::CreateMainPath(const char* pPath)
{	// 기본 디렉토리 생성
	mkdir(pPath);

	time_t nowTime;
	time(&nowTime);

	tm* pNow = localtime(&nowTime);
	const int nYear = pNow->tm_year + 1900;
	const int nMon = pNow->tm_mon + 1;
	// 월별 디렉토리 생성
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH);
	sprintf(szBuffer,"%s\\%04d_%02d", pPath, nYear, nMon);
	mkdir(szBuffer);
	//
	m_strRoot = szBuffer;
}

void CLogManager::CreateSubPath(int nType, const char* pPath)
{
	string strTemp = m_strRoot;
	strTemp += "\\";
	strTemp += pPath;
	mkdir(strTemp.c_str());
	//
	m_mpBuffer.insert(PAIR_LOG(nType, strTemp));
}

//< 040715_KJHuNs Root 폴더 저장
void CLogManager::SaveLogRoot_DeadEventNpc(CHARLIST* pTarget)
{	//< CSD-031013
	string strPath;

	if (!GetLogPath(strPath))
	{
		return;
	}
	
	const char* pPath = VA("%s\\DeadEventNpc.txt", strPath.c_str());
	FILE* pFile = fopen(pPath, "at+");
	
	if (pFile == NULL)
	{
		return;
	}

	fprintf(pFile, "[%02d.%02d.%02d %02d:%02d:%02d]", g_year-2000, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "%s(%3d, %3d), EventNo(%d), Killer(%s)\n", MapName, 
		                                                      pTarget->MoveSx, 
															  pTarget->MoveSy, 
															  pTarget->eventno,
															  pTarget->KillerName);
	fclose(pFile);
}	//> CSD-031013

// 040715_KJHuNs Menuserver.cpp 의 GambleLog함수를 여기로 이동(정리목적)
void CLogManager::SaveLogRoot_GambleLogFile( CHARLIST *ch, int house, int class_type,
											int user_count, ItemAttr *item, int resource_type, 
											int lv_band )
{	//< CSD-TW-030622
	string strPath;

	if (!GetLogPath(strPath))
	{
		return;
	}

	char file[MAX_PATH]= {0,};
	::sprintf( file, "%s/gamble_item%d.txt", strPath.c_str(), house );	// 030506 YGI
	FILE *fp = ::fopen( file, "at+" );
	if(!fp){return;}

	fprintf( fp, "---%02d.%02d %02d:%02d:%02d ---\n",  g_mon+1, g_day, g_hour, g_min, g_sec );
	fprintf( fp, "item_no[%d]	house:%d	room:%d		%s(lv:%d)\n", item->item_no, house, class_type, ch->Name, ch->GetLevel()); // CSD-030806
	fprintf( fp, "resource_type:%d	user:%d	lv_band%d\n\n", resource_type, user_count, lv_band );	

	fclose( fp );
}

// 040715_KJHuNs Op_Battle.cpp 의 pk루틴을 여기로 이동(정리목적)
void CLogManager::SaveLogRoot_PK(LPTSTR lpMsg)
{
	// 030506 YGI
	string strPath;
	if (GetLogPath(strPath))
	{
		//< CSD-TW-030701
		char temp_path[80];
		sprintf( temp_path, "%s/pk.txt", strPath.c_str());

		SaveLogDefault(temp_path, lpMsg, 0);

	}	//> CSD-TW-030701
}
//> 040715_KJHuNs Root 폴더 저장

//< 040715_KJHuNs Accelater폴더에 저장
void CLogManager::SaveLogAccelater(CHARLIST* pTarget, const char* pType, const int cn)
{	//< CSD-030808
	string strPath;

	if (!GetLogPath(LT_ACCELATER, strPath))
	{
		return;
	}
	//< CSD-040224

	//050328_KCH 캐릭명으로만 남으니까, 문제가 생겨서, 로그가 너무 많이 생겨서 해당 날짜별로 남게함.
//	FILE* pFile = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");
	FILE* pFile = fopen(VA("%s/%02d_%02d_%02d.txt", strPath.c_str(), g_year-2000, g_mon + 1, g_day), "at+");
	Dbg_Assert(NULL != pFile);
	Dbg_Assert(0 <= cn && cn <= DRAGON_MAX_CONNECTIONS_+1);

	if (NULL != connections[cn].id && 0 < strlen(connections[cn].id) )
	{
		//> CSD-040224
		fprintf(pFile, "[%02d:%02d:%02d](%s) ", g_hour, g_min, g_sec,pTarget->Name);
		fprintf(pFile, "ID(%s) MapName(%s), IP(%s), Type(%s)\n", 
														  connections[cn].id,
														  connections[cn].mapname, 
														  connections[cn].ip_address,
														  pType);
	}
	else
	{
		//> CSD-040224
		fprintf(pFile, "[%02d:%02d:%02d](%s) ", g_hour, g_min, g_sec,pTarget->Name);
		fprintf(pFile, "ID(%s) MapName(%s), IP(%s), Type(%s)\n",
														  connections[pTarget->GetServerID()].id,
														  connections[pTarget->GetServerID()].mapname, 
														  connections[pTarget->GetServerID()].ip_address,
														  pType);
	}
	fclose(pFile);
}	//> CSD-030808
//> 040715_KJHuNs Accelater폴더에 저장

//< 040715_KJHuNs Auction폴더에 저장
//<! BBD 040226	옥션로그를 위해생성한 함수
void CLogManager::SaveLogAuction(int type, t_SellerItemDelete *pRecordInfo)
{

	// 로그 타입 분류
	char szType[64];
	switch(type)
	{
	case ACLT_SELLER_REGIST:
		sprintf(szType, "<<SELLER_REGIST>>");
		break;
	case ACLT_SELLER_CANCLE:
		sprintf(szType, "<<SELLER_CANCLE>>");
		break;
	case ACLT_BUYER_TAKE:
		sprintf(szType, "<<BUYER_TAKE>>");
		break;
	case ACLT_SELLER_TAKE:
		sprintf(szType, "<<SELLER_TAKE>>");
		break;
	default:
		return;
	}

	// 타입이 정상임
	string strPath;

	if (!GetLogPath(LT_AUCTION, strPath))
	{
		return;
	}

	// 경로에 파일을 연다
	const char* pPath = VA("%s\\AuctionLog_%02d_%02d_%02d.txt", strPath.c_str(), g_year-2000, g_mon + 1, g_day);
	FILE* pFile = fopen(pPath, "at+");
	
	if (pFile == NULL)
	{
		return;
	}


	//로그를 한줄씩 남긴다
	fprintf(pFile, "[%02d.%02d.%02d %02d:%02d:%02d]\t", g_year-2000, g_mon + 1, g_day, g_hour, g_min, g_sec);

	// 지저분하니깐 여기서 받아오자
	char * Seller = pRecordInfo->szSellerName;
	char * Buyer = pRecordInfo->szName;
	int money = pRecordInfo->dwSellValue;
	int item_no = pRecordInfo->SellItem.item_no;
	int limit = pRecordInfo->SellItem.attr[1];
	RareMain *pRare = (RareMain *)&(pRecordInfo->SellItem.attr[3]);

	// 본격적으로 로그를 남겨보자
	fprintf(pFile, "Type:%s\tSeller:%s\tBuyer:%s\tMoney:%d\tItemNo:%d\tLimit:%d\tGrade:%d\tSok1:%d\tSok2:%d\tSok3:%d\tHigh:%d\tDynamic:%d\t\n",
		szType, Seller, Buyer, money, item_no, limit, pRare->grade, pRare->soksung1, pRare->soksung2, pRare->soksung3, pRare->iHighLevel, pRare->IsDynamicRare);

	fclose(pFile);
}
//> BBD 040226	옥션로그를 위해생성한 함수
//> 040715_KJHuNs Auction폴더에 저장

//< 040715_KJHuNs Battle폴더에 저장
void SaveBallancing2( LPCHARLIST a, LPCHARLIST d, int magicno, int damage, int mana, int attack, int attacktype, int tac  )
{   //< CSD-030324
	if(a->IsNpc()){return;}
	if( a->balancingtest/20  == (a->balancingtest-1)/20)//20 번에 한번
	{
		return;
	}
	
	char temp[ FILENAME_MAX] = {0,};
	::sprintf( temp, "./SimpleLog/%s.txt", a->Name );
	FILE *fp = fopen( temp, "at+");
	if( NULL == fp )
	{
		sprintf( temp, "./SimpleLog/%s.txt", connections[a->GetServerID()].id );
		fp = fopen( temp, "at+" );
		if( fp == NULL ) return;
		fprintf( fp, " %s \n", a->Name );
	}
	
	if( magicno )
	{
		if (magicno >= 215)
		{
			fprintf( fp, "Combat Skill< %3d:%30s >  --> %20s   \n", magicno, Magic_Ref[magicno].Han_Name, d->Name );
		}
		else
		{
			fprintf( fp, "Magic< %3d:%30s >  --> %20s   \n", magicno, Magic_Ref[magicno].Han_Name, d->Name );
		}
		
		fprintf( fp, "---%s(%3d,%3d) %02d.%02d %02d:%02d:%02d ---\n",  MapName, a->MoveSx, a->MoveSy, g_mon+1, g_day,g_hour, g_min, g_sec );
		
		fprintf( fp, " Attack(Lv:%3d/Exp:%9d/ladder:%3d/fame:%3d)\n", a->GetLevel(), a->Exp, a->LadderScore, a->fame); // CSD-030806
		fprintf( fp, " Defence(Lv:%3d/Exp:%9d)\n", d->GetLevel(), d->Exp); // CSD-030806

		fprintf( fp, " Hp %4d/%4d ->  Hp %4d/%4d \n", a->Hp, a->HpMax, d->Hp, d->HpMax );
		fprintf( fp, " Mana %4d/%4d -> Mana %4d/%4d \n", a->Mana, a->ManaMax, d->Mana, d->ManaMax );  
		fprintf( fp, " Hungry %4d/%4d -> Hngy %4d/%4d \n", a->Hungry, a->HungryMax, d->Hungry, d->HungryMax );
		fprintf( fp, " Damage : %4d        \n", damage );
		fprintf( fp, " Used Mana: %4d        \n",   mana );

		switch(a->Spell)
		{
		case WIZARD_SPELL:
			{
				::fprintf(fp, " Magery               Tactics(%3d:%d)\n", 
					a->Skill[TACTICS_Magery], a->tac_skillEXP[11]);
			}break;
		case PRIEST_SPELL:
			{
				::fprintf(fp, " Orison               Tactics(%3d:%d)\n", 
					a->Skill[TACTICS_Orison], a->tac_skillEXP[12]);
			}break;
		}
	}	
	else
	{
		fprintf( fp, "Weapon-Attack< %d:%s >  --> %s\n", attacktype, magicno, Magic_Ref[magicno].Han_Name,  d->Name  );
		fprintf( fp, "---%s(%3d,%3d) %02d.%02d %02d:%02d:%02d ---\n",  MapName, a->MoveSx, a->MoveSy, g_mon+1, g_day,g_hour, g_min, g_sec );
		
		fprintf(fp, " Attack(Lv:%3d/Exp:%9d/ladder:%3d/fame:%3d)\n", a->GetLevel(), a->Exp, a->LadderScore, a->fame); // CSD-030806
		fprintf(fp, " Defence(Lv:%3d/Exp:%9d)\n", d->GetLevel(), d->Exp); // CSD-030806
				
		const int ino = a->equip[ WT_WEAPON].item_no;
		CItem *ii = ItemUnit(ino);
		
		int itemkind = IK_NONE;

		if( ii )	{itemkind = ii->GetItemKind();}
		else		{itemkind = IK_NONE;}
		
		char *at = NULL;

		switch( itemkind )
		{			
			//	장거리 무기....
		case IK_DUAL_BOW:		at = "DUAL_BOW";	break;//030102 lsw Dual BOW
		case IK_SHORT_BOW	:	at = "SHORT_BOW";	break;
		case IK_TWO_ARROW	:	at = "TWO_ARROW";	break;
		case IK_DUAL_ARROW	:	at = "DUAL_ARROW";	break;//030102 lsw
		case IK_LONG_BOW	:	at = "LONG_BOW";	break;
		case IK_ARROW_POINT	:	at = "ARROW_POINT";	break;
		case IK_FEATHER		:	at = "FEATHER";	break;
			// 중장거리.. 
		case IK_HURL		:	at = "THROWING WEAPON"; break;
			//	단거리무기..
		default             :   at = "MELEE WEAPON";    break;
		}			
		
		char temp[ FILENAME_MAX];
		
		if( ii ) 
		{
			strcpy( temp, ii->GetHanName() );
		}
		else 
		{
			strcpy( temp, " NO WEAPON " );
		}
		fprintf( fp, " Hp %4d/%4d ->  Hp %4d/%4d \n", a->Hp, a->HpMax, d->Hp, d->HpMax );
		fprintf( fp, " Mana %4d/%4d ->Mana %4d/%4d \n", a->Mana, a->ManaMax, d->Mana, d->ManaMax );
		fprintf( fp, " Hungry %4d/%4d ->Hngy %4d/%4d \n", a->Hungry, a->HungryMax, d->Hungry, d->HungryMax );
		fprintf( fp, " Ac  %3d    ->       %3d   \n", 	a->Ac, d->Ac );
		fprintf( fp, " Damage  :    %4d     \n",   damage );
		fprintf( fp, " WEAPON :  %s( %d )  \n",    temp, a->equip[ WT_WEAPON].item_no );
		fprintf( fp, " AttackType :    %s      \n",    at );
	}
//	
	const int iTacKind = a->GetTacticsKind();// == SKILL_UNKNOWN 이면 기록 하면 안되지.
	switch(iTacKind)
	{
	case TACTICS_Crapple						:
		{
			::fprintf(fp, " Crapple              Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[0]);
		}break;
	case TACTICS_swordmanship					:
		{
			::fprintf(fp, " Swordmanship         Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[1]);
		}break;
	case TACTICS_Archery						:
		{
			::fprintf(fp, " Archery              Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[2]);
		}break;
	case TACTICS_Fencing						:
		{
			::fprintf(fp, " Fencing              Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[3]);
		}break;
	case TACTICS_Mace_fighting					:
		{
			::fprintf(fp, " Mace Fighting        Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[4]);
		}break;
	case TACTICS_Pierce							:
		{
			::fprintf(fp, " Pierce               Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[5]);
		}break;
	case TACTICS_Whirl							:
		{
			::fprintf(fp, " Whirl                Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[6]);
		}break;
	case TACTICS_Hurl							:
		{
			::fprintf(fp, " Hurl                 Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[7]);
		}break;
	case TACTICS_Parrying						:
		{
			::fprintf(fp, " Parrying             Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[8]);
		}break;
	case TACTICS_Double_Swordmanship			:
		{
			::fprintf(fp, " Double Swordmanship  Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[9]);
		}break;
	case TACTICS_Double_MaceFighting			:
		{
			::fprintf(fp, " Double Mace Fighting Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[10]);
		}break;
	case TACTICS_Magery							:
		{
			::fprintf(fp, " Magery               Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[11]);
		}break;
	case TACTICS_Orison							:
		{
			::fprintf(fp, " Orison               Tactics(%3d:%d)\n", 
				a->Skill[iTacKind], a->tac_skillEXP[12]);
		}break;
	default:
		{
			::fprintf(fp, " TACTIC UNKNOWN \n");
		}break;
	}
	::fprintf(fp, "\n");
	fclose(fp);
}
void CLogManager::SaveLogBattle_Ballancing( LPCHARLIST a, LPCHARLIST d, 
										   int magicno, int damage, int mana, 
										   int attack, int attacktype, int tac )
{   //< CSD-TW-030622
	if (a->IsNpc())
	{
		return;
	}

	//char temp[ FILENAME_MAX];	// LTH-040227-KO 사용되지 않아서 삭제
	a->balancingtest --;
	if( a->balancingtest > 0 )
	{
		::SaveBallancing2( a, d, magicno, damage, mana, attack, attacktype, tac  );
		return;
	}
	a->balancingtest = 100;
	
	string strPath;

	if (!GetLogPath(LT_BATTLE, strPath))
	{
		return;
	}

	//< CSD-040224
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), a->Name), "at+");

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[a->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	
	if( magicno )
	{
		if (magicno >= 215)
		{
			fprintf( fp, "Combat Skill< %3d:%30s >  --> %20s   \n", magicno, Magic_Ref[magicno].Han_Name, d->Name );
		}
		else
		{
			fprintf( fp, "Magic< %3d:%30s >  --> %20s   \n", magicno, Magic_Ref[magicno].Han_Name, d->Name );
		}
		
		fprintf( fp, "---%s(%3d,%3d) %02d.%02d %02d:%02d:%02d ---\n",  MapName, a->MoveSx, a->MoveSy, g_mon+1, g_day,g_hour, g_min, g_sec );
		
		fprintf(fp, " Attack(Lv:%3d/Exp:%9d/ladder:%3d/fame:%3d)\n", a->GetLevel(), a->Exp, a->LadderScore, a->fame); // CSD-030806
		fprintf(fp, " Defence(Lv:%3d/Exp:%9d)\n", d->GetLevel(), d->Exp); // CSD-030806
		fprintf( fp, " Reserved Point %3d -> Reserved Point %3d\n", a->GetReservedPoint(), d->GetReservedPoint());
		fprintf( fp, " Str   %3d    ->  Str   %3d                Hp %4d/%4d ->  Hp %4d/%4d \n", a->Str, d->Str, a->Hp, a->HpMax, d->Hp, d->HpMax );
		fprintf( fp, " Con   %3d    ->  Con   %3d              Mana %4d/%4d ->Mana %4d/%4d \n", a->Con, d->Con, a->Mana, a->ManaMax, d->Mana, d->ManaMax );  
		fprintf( fp, " Dex   %3d    ->  Dex   %3d            Hungry %4d/%4d ->Hngy %4d/%4d \n", a->Dex, d->Dex, a->Hungry, a->HungryMax, d->Hungry, d->HungryMax );
		fprintf( fp, " Wis   %3d    ->  Wis   %3d                                   \n", 	a->Wis, d->Wis );
		fprintf( fp, " Int   %3d    ->  Int   %3d                 Damage : %4d        \n", 	a->Int, d->Int, damage );
		fprintf( fp, " MoveP %3d    ->  MoveP %3d             Used Mana: %4d        \n",    a->MoveP, d->MoveP, mana );
		fprintf( fp, " Char  %3d    ->  Char  %3d                                   \n",    a->Char , d->Char );
		fprintf( fp, " Endu  %3d    ->  Endu  %3d                                   \n",    a->Endu , d->Endu ); 
		fprintf( fp, " Moral %3d    ->  Moral %3d                                   \n",    a->Moral, d->Moral); 
		fprintf( fp, " Luck  %3d    ->  Luck  %3d                                   \n",    a->Luck , d->Luck ); 
		fprintf( fp, " wsps  %3d    ->  wsps  %3d                                   \n",    a->wsps , d->wsps ); 
		
		fprintf( fp, "방어자의 마법저항치 \n" );
		fprintf(fp, "Resist Poison  %3d\n", d->GetBasicResist(RT_POISON));//030227 lsw
		fprintf(fp, "Resist Curse   %3d\n", d->GetBasicResist(RT_CURSE));
		fprintf(fp, "Resist Fire    %3d\n", d->GetBasicResist(RT_FIRE));
		fprintf(fp, "Resist Ice     %3d\n", d->GetBasicResist(RT_ICE));
		fprintf(fp, "Resist Elect   %3d\n", d->GetBasicResist(RT_ELECT));
		fprintf(fp, "Resist Holy    %3d\n", d->GetBasicResist(RT_HOLY));
	}	
	else
	{	
		fprintf( fp, "Weapon-Attack< %d:%s >  --> %s\n", attacktype, magicno, Magic_Ref[magicno].Han_Name,  d->Name  );
		fprintf( fp, "---%s(%3d,%3d) %02d.%02d %02d:%02d:%02d ---\n",  MapName, a->MoveSx, a->MoveSy, g_mon+1, g_day,g_hour, g_min, g_sec );
		
		fprintf(fp, " Attack(Lv:%3d/Exp:%9d/ladder:%3d/fame:%3d)\n", a->GetLevel(), a->Exp, a->LadderScore, a->fame); // CSD-030806
		fprintf(fp, " Defence(Lv:%3d/Exp:%9d)\n", d->GetLevel(), d->Exp); // CSD-030806
		
		int itemkind;
		char *at;
		const int ino = a->equip[ WT_WEAPON].item_no;
		CItem *ii = ItemUnit( ino/1000, ino%1000);
		
		if( ii )	itemkind = ii->GetItemKind();
		else		itemkind = IK_NONE;
		
		switch( itemkind )
		{			
			//	장거리 무기....
		case IK_DUAL_BOW:		at = "DUAL_BOW";	break;//030102 lsw Dual BOW
		case IK_SHORT_BOW	:	at = "SHORT_BOW";	break;
		case IK_TWO_ARROW	:	at = "TWO_ARROW";	break;
		case IK_DUAL_ARROW	:	at = "DUAL_ARROW";	break;//030102 lsw
		case IK_LONG_BOW	:	at = "LONG_BOW";	break;
		case IK_ARROW_POINT	:	at = "ARROW_POINT";	break;
		case IK_FEATHER		:	at = "FEATHER";	break;
			// 중장거리.. 
		case IK_HURL		:	at = "THROWING WEAPON"; break;
			//	단거리무기..
		default             :   at = "MELEE WEAPON";    break;
		}			
		
		char temp[ FILENAME_MAX];
		
		if( ii ) 
		{
			strcpy( temp, ii->GetHanName() );
		}
		else 
		{
			strcpy( temp, " NO WEAPON " );
		}
		
		fprintf( fp, " Reserved Point %3d -> Reserved Point %3d\n", a->GetReservedPoint(), d->GetReservedPoint());
		fprintf( fp, " Str   %3d	->  Str   %3d                Hp %4d/%4d ->  Hp %4d/%4d \n", a->Str, d->Str, a->Hp, a->HpMax, d->Hp, d->HpMax );
		fprintf( fp, " Con   %3d	->  Con   %3d              Mana %4d/%4d ->Mana %4d/%4d \n", a->Con, d->Con, a->Mana, a->ManaMax, d->Mana, d->ManaMax );
		fprintf( fp, " Dex   %3d	->  Dex   %3d            Hungry %4d/%4d ->Hngy %4d/%4d \n", a->Dex, d->Dex, a->Hungry, a->HungryMax, d->Hungry, d->HungryMax );
		fprintf( fp, " Wis   %3d	->  Wis   %3d                Ac  %3d    ->       %3d   \n", 	a->Wis, d->Wis, a->Ac, d->Ac );
		fprintf( fp, " Int   %3d	->  Int   %3d                                   \n",    a->Int, d->Int);
		fprintf( fp, " MoveP %3d	->  MoveP %3d              Damage  :    %4d     \n",    a->MoveP, d->MoveP, damage );
		fprintf( fp, " Char  %3d	->  Char  %3d             WEAPON :  %s( %d )  \n",    a->Char , d->Char,  temp, a->equip[ WT_WEAPON].item_no );
		fprintf( fp, " Endu  %3d	->  Endu  %3d           AttackType :    %s      \n",    a->Endu , d->Endu,  at );
		fprintf( fp, " Moral %3d	->  Moral %3d                                   \n",    a->Moral, d->Moral );
		fprintf( fp, " Luck  %3d	->  Luck  %3d                                   \n",    a->Luck , d->Luck );
		fprintf( fp, " wsps  %3d	->  wsps  %3d                                   \n",    a->wsps , d->wsps );
	}
	
	fprintf(fp, " [Tactic Info]\n");
	fprintf(fp, " Crapple              Tactics(%3d:%d)\n", a->Skill[TACTICS_Crapple], a->tac_skillEXP[0]);
	fprintf(fp, " Swordmanship         Tactics(%3d:%d)\n", a->Skill[TACTICS_swordmanship], a->tac_skillEXP[1]);
	fprintf(fp, " Archery              Tactics(%3d:%d)\n", a->Skill[TACTICS_Archery], a->tac_skillEXP[2]);
	fprintf(fp, " Fencing              Tactics(%3d:%d)\n", a->Skill[TACTICS_Fencing], a->tac_skillEXP[3]);
	fprintf(fp, " Mace Fighting        Tactics(%3d:%d)\n", a->Skill[TACTICS_Mace_fighting], a->tac_skillEXP[4]);
	fprintf(fp, " Pierce               Tactics(%3d:%d)\n", a->Skill[TACTICS_Pierce], a->tac_skillEXP[5]);
	fprintf(fp, " Whirl                Tactics(%3d:%d)\n", a->Skill[TACTICS_Whirl], a->tac_skillEXP[6]);
	fprintf(fp, " Hurl                 Tactics(%3d:%d)\n", a->Skill[TACTICS_Hurl], a->tac_skillEXP[7]);
	fprintf(fp, " Parrying             Tactics(%3d:%d)\n", a->Skill[TACTICS_Parrying], a->tac_skillEXP[8]);
	fprintf(fp, " Double Swordmanship  Tactics(%3d:%d)\n", a->Skill[TACTICS_Double_Swordmanship], a->tac_skillEXP[9]);
	fprintf(fp, " Double Mace Fighting Tactics(%3d:%d)\n", a->Skill[TACTICS_Double_MaceFighting], a->tac_skillEXP[10]);
	fprintf(fp, " Magery               Tactics(%3d:%d)\n", a->Skill[TACTICS_Magery], a->tac_skillEXP[11]);
	fprintf(fp, " Orison               Tactics(%3d:%d)\n", a->Skill[TACTICS_Orison], a->tac_skillEXP[12]);

	fprintf(fp, " [NK Info]\n");
	fprintf(fp, "N_NEUTRAL  = %d	N_NONSTER	= %d	N_VYSEUS = %d\n", a->nk[N_NEUTRAL], a->nk[N_NONSTER], a->nk[N_VYSEUS]);
	fprintf(fp, "N_ZYPERN	= %d	N_HEGEMONIA	= %d	N_YILSE	 =%d\n", a->nk[N_ZYPERN	], a->nk[N_HEGEMONIA], a->nk[N_YILSE]);
	
	fprintf(fp, " [Party Info]\n");
	
	for (int i = 0; i < MAX_PARTY_MEMBER; ++i)
	{
		if (a->party[i].On && a->party[i].Name)
		{ 
			fprintf(fp, " [%d]%20s%s", i, a->party[i].Name, (i==2)?"\n":"  ");
		}
	}

	fprintf(fp, "\n");
	fclose(fp);
}	//> CSD-TW-030622
//> 040715_KJHuNs Battle폴더에 저장

//< 040715_KJHuNs Change폴더에 저장
void CLogManager::SaveLogChange_BankMoney(const int idTarget, 
											const DWORD nOldBankMoney, 
											const DWORD nNewBankMoney ,
											const eBankMoneyChangeType BMCT)
{	//< CSD-040324
	string strPath;

	if (!GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}

	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[idTarget].name), "at+");

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[idTarget].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}

	string strWhy = "";

	switch(BMCT)
	{
	case BMCT_ARENA_GAMBLE_DIVIDEND:
		{
			strWhy = "Arena Gamble Dividend";
			break;
		}
	case BMCT_ARENA_GAMBLE_BETTING:
		{
			strWhy = "Arena Gamble Betting";
			break;
		}
	case BMCT_BANK_DEPOSIT:
		{
			strWhy = "Bank Deposit";
			break;
		}
	case BMCT_BANK_DEFRAYAL:
		{
			strWhy = "Bank Gamble Defrayal";
			break;
		}
	case BMCT_AUCTION_ITEM_BUY:
		{
			strWhy = "Auction Item Buy ";
			break;
		}
	case BMCT_AUCTION_ITEM_REGISTER:
		{
			strWhy = "Auction Item Register";
			break;
		}
	case BMCT_BUY_LOTTO:
		{
			strWhy = "Buy Lotto";
			break;
		}
	case BMCT_RESET_ABILITY:
		{
			strWhy = "Reset Ability";
			break;
		}
	case BMCT_CHAR_LOGIN:
		{
			strWhy = "User Login";
			break;
		}
	case BMCT_RECV_MAP_CHANGE:
		{
			strWhy = "User MapMove";
			break;
		}
	case BMCT_RECV_CHAR_INFO_BASIC:
		{
			strWhy = "Recv CharInfo Basic";
			break;
		}
	default:
		{
			strWhy = "UnKnown";
			break;
		}
	}

	::fprintf( fp, "@ChangeBankMoney	%d-%02d-%02d %02d:%02d:%02d	(Old)%d	(New)%d %s\n",
			g_year-2000, g_mon+1, g_day, 
			g_hour, g_min, g_sec,
			nOldBankMoney, nNewBankMoney, strWhy.c_str());
	::fclose( fp );
}	//> CSD-040324

void CLogManager::SaveLogChange_BattingMoney(CHARLIST* pTarget, DWORD dwOldMoney, DWORD dwNewMoney)
{	//< CSD-TW-030624
	string strPath;

	if (!GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	::fprintf(fp, "@ChangeBattingMoney	%d-%02d-%02d %02d:%02d:%02d	BankMoney (Before)%d (After)%d\n",
			  g_year-2000, g_mon+1, g_day, g_hour, g_min, g_sec,
			  dwOldMoney, dwNewMoney);
	::fclose( fp );
}	//> CSD-TW-030624

void CLogManager::SaveLogChange_ClassQuest(CHARLIST* pTarget, const char* pMsg, 
										   int nQuestNo, int nQcount, int nStart)
{	//< CSD-TW-030701
	string strPath;

	if (!GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	::fprintf(fp, "@ChangeQuest[%d]	%02d-%02d-%02d %02d:%02d:%02d	Msg=%s, Qcount=%d, Start=%d\n",
			  nQuestNo,       
		      g_year - 2000, g_mon + 1, g_day, g_hour, g_min, g_sec,
			  pMsg, nQcount, nStart);
	::fclose(fp);
}	//> CSD-TW-030701

void CLogManager::SaveLogChange_ClassStep(CHARLIST* pTarget, int nPrevStep, int nNextStep)
{	//< CSD-TW-030622
	string strPath;

	if (!GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	//> CSD-040224	
	::fprintf(fp, "@ChangeStep	%02d-%02d-%02d %02d:%02d:%02d	(old)%d	(new)%d\n",
		      g_year - 2000, g_mon + 1, g_day, g_hour, g_min, g_sec,
			  nPrevStep, nNextStep);
	::fclose(fp);
}	//> CSD-TW-030622

void CLogManager::SaveLogChange_DividendMoney(CHARLIST* pTarget, DWORD dwOldMoney, DWORD dwNewMoney, bool bBank)
{	//< CSD-TW-030624
	string strPath;

	if (!GetLogPath(LT_CHANGE, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(fp, "@ChangeDividendMoney	%d-%02d-%02d %02d:%02d:%02d	",
			  g_year-2000, g_mon+1, g_day, g_hour, g_min, g_sec);

	if (bBank)
	{
		fprintf(fp, "BankMoney (Before)%d (After)%d\n", dwOldMoney, dwNewMoney);
	}
	else
	{
		fprintf(fp, "InventoryMoney (Before)%d (After)%d\n", dwOldMoney, dwNewMoney);
	}

	::fclose( fp );
}	//> CSD-TW-030624

void CLogManager::SaveLogChange_DualFame(CHARLIST* pTarget, const int nOldDualFame, 
										 const int nNewDualFame, eLDF_TYPE type)
{	//< CSD-040224
	string strPath;

	if (!GetLogPath(LT_CHANGE, strPath))
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

	char why[MAX_PATH] = {0,};
	switch (type)
	{
	case LDF_LOCALWAR: 
		{	// 듀얼 소비
			::strcpy(why, "LocalWar");
			break;
		}
	case LDF_QUEST:
		{	// 국지전
			::strcpy(why, "Quest");
			break;
		}
	case LDF_NEOWAR:	//< LTH-040226-KO
		{
			::strcpy( why, "NeoWar" );			// 1.4 신규국가전 
		}
		break;			//> LTH-040226-KO
	default : 	
		{
			::strcpy( why, "default" );
		}break;
	}

	::fprintf( fp, "@ChangeDualFame	why=%s %02d-%02d-%02d %02d:%02d:%02d	(x:%d y:%d)	(old)%d	(new)%d\n",
			why, 
			g_year-2000, g_mon+1, g_day, 
			g_hour, g_min, g_sec,
			pTarget->X, pTarget->Y, nOldDualFame, nNewDualFame);
	::fclose( fp );
}	//> CSD-040224

void CLogManager::SaveLogChange_Fame(CHARLIST* pTarget, int old_fame, int new_fame, 
									 eLF_TYPE type)
{	//< CSD-040224
	string strPath;

	if (!GetLogPath(LT_CHANGE, strPath))
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
	
	char why[MAX_PATH] = {0,};

	switch (type)
	{
	case LF_DUAL: 
		{	// 듀얼 소비
			strcpy(why, "upgrade dual");
			break;
		}
	case LF_LOCALWAR:
		{	// 국지전
			strcpy(why, "LocalWar");
			break;
		}
	case LF_ITEMBUY:
		{	// 국가 고급 아이템
			strcpy(why, "Nation Item Buy");
			break;
		}
	case LF_DONATION:
		{	// 국가 기부
			strcpy(why, "Nation Doantion");
			break;
		}
	case LF_NATIONWAR:
		{	// 국가전
			strcpy(why, "Nation War");
			break;
		}
	case LF_SCRIPT:
		{	// 스크립트 펑션
			strcpy(why, "Script Func");
			break;
		}
	default:
		{
			strcpy(why, "default");
			break;
		}
	}

	::fprintf(fp, "@ChangeFame	why=%s %02d-%02d-%02d %02d:%02d:%02d	(x:%d y:%d)	(old)%d	(new)%d\n",
			  why, 
			  g_year-2000, g_mon + 1, g_day,
			  g_hour, g_min, g_sec,
			  pTarget->X, pTarget->Y, old_fame, new_fame);
	::fclose( fp );
}	//> CSD-040224

void CLogManager::SaveLogChange_NationItem(CHARLIST* pTarget, int type, ItemAttr* item)
{	//< CSD-040224
	string strPath;

	if (!GetLogPath(LT_CHANGE, strPath))
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

	CItem *t = ItemUnit( item->item_no );
	
	::fprintf( fp, "@ChangeNationItem	%d-%02d-%02d %02d:%02d:%02d	(Type)%s	(item:%s)%d	%d	%d	%d\n",
			g_year-2000, g_mon+1, g_day, 
			g_hour, g_min, g_sec, type?"FAME":"MONEY", t?t->GetHanName():"WRONG ITEM", item->item_no,
			item->attr[0], item->attr[1], item->attr[2] );
	::fclose( fp );
}	//> CSD-040224

void CLogManager::SaveLogChange_Ladder(CHARLIST* pTarget, int old_ladder, int new_ladder, int type)
{	//< CSD-040224
	string strPath;

	if (!GetLogPath(LT_CHANGE, strPath))
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

void CLogManager::SaveLogChange_ObtainCombat(CHARLIST* pTarget, int nOld, int nNew)
{	//< CSD-040224
	string strPath;

	if (!GetLogPath(LT_CHANGE, strPath))
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
	
	fprintf(fp ,
		    "@ObtainCombatPoint	%02d/%02d/%02d/%02d/%02d/%02d	(old point)%d(==>)	(new point)%d\n",
		    g_year - 2000, g_mon + 1, g_day, g_hour, g_min, g_sec, nOld, nNew);
	fprintf(fp, ";Lightning Boom    %3d  Thunder Blow      %3d\n", pTarget->GetCombatLevel(LIGHTNING_BOOM), pTarget->GetCombatLevel(THUNDER_BLOW));
	fprintf(fp, ";Lightning Shock   %3d  Thunder Strike    %3d\n", pTarget->GetCombatLevel(LIGHTNING_SHOCK), pTarget->GetCombatLevel(THUNDER_STRIKE));
	fprintf(fp, ";Lightning Shield  %3d  Lightning Extreme %3d\n", pTarget->GetCombatLevel(LIGHTNING_SHIELD), pTarget->GetCombatLevel(LIGHTNING_EXTREME));
	fprintf(fp, ";Blood Earth       %3d  Ground Attack     %3d\n", pTarget->GetCombatLevel(BLOOD_EARTH), pTarget->GetCombatLevel(GROUND_ATTACK));
	fprintf(fp, ";Blood Will        %3d  Stone Armor       %3d\n", pTarget->GetCombatLevel(BLOOD_WILL), pTarget->GetCombatLevel(STONE_ARMOR));
	fprintf(fp, ";Simunian Bosom    %3d  Earth Extreme     %3d\n", pTarget->GetCombatLevel(SIMUNIAN_BOSOM), pTarget->GetCombatLevel(EARTH_EXTREME));
	fprintf(fp, ";Sword N'Roses     %3d  Multiple Fire     %3d\n", pTarget->GetCombatLevel(SWORD_N_ROSES), pTarget->GetCombatLevel(MULTIPLE_FIRE));
	fprintf(fp, ";Fleet Missile     %3d  Converting Armor  %3d\n", pTarget->GetCombatLevel(FLEET_MISSILE), pTarget->GetCombatLevel(CONVERTING_ARMOR));
	fprintf(fp, ";Burning Bloody	%3d  Fire Extreme      %3d\n", pTarget->GetCombatLevel(BURNING_BLOODY), pTarget->GetCombatLevel(FIRE_EXTREME)); // 030415 kyo 
	fprintf(fp, ";Poisoning Nova    %3d  Cherroy Shade     %3d\n", pTarget->GetCombatLevel(POISONING_NOVA), pTarget->GetCombatLevel(CHERROY_SHADE));
	fprintf(fp, ";Dark Blade        %3d  Tybern Gift       %3d\n", pTarget->GetCombatLevel(DARK_BLADE), pTarget->GetCombatLevel(TYBERN_GIFT));
	fprintf(fp, ";AcuquireDark      %3d  Dark Extreme      %3d\n", pTarget->GetCombatLevel(ACUQUIRE_DARK), pTarget->GetCombatLevel(DARK_EXTREME));// 030415 kyo 
	fprintf(fp, ";Icing Blast       %3d  Critical Hiding   %3d\n", pTarget->GetCombatLevel(ICING_BLAST), pTarget->GetCombatLevel(CRITICAL_HIDING));
	fprintf(fp, ";Shark Missile     %3d  Horn Of Iceberg   %3d\n", pTarget->GetCombatLevel(SHARK_MISSILE), pTarget->GetCombatLevel(HORN_OF_ICEBERG));
	fprintf(fp, ";Ice Shield        %3d  Ice Extreme       %3d\n", pTarget->GetCombatLevel(ICE_SHIELD), pTarget->GetCombatLevel(ICE_EXTREME));
	fprintf(fp, ";Charging          %3d  Double Attack     %3d\n", pTarget->GetCombatLevel(CHARGING), pTarget->GetCombatLevel(DOUBLE_ATTACK));
	fprintf(fp, ";Whilwind          %3d  Twister           %3d\n", pTarget->GetCombatLevel(WHILWIND), pTarget->GetCombatLevel(TWISTER));
	fprintf(fp, ";Gust              %3d  Wind Extreme      %3d\n", pTarget->GetCombatLevel(GUST), pTarget->GetCombatLevel(WIND_EXTREME));
	fprintf(fp, ";Reserved Combat Skill Point %3d \n", pTarget->GetCombatPoint());
	
	fclose(fp);
}	//> CSD-040224

void CLogManager::SaveLogChange_Combat(CHARLIST* pTarget, int nCombat, int nOld, int nNew)
{	//< CSD-040224
	string strPath;
	
	if (!GetLogPath(LT_CHANGE, strPath))
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
	
	fprintf(fp,
		    "@ChangeCombatPoint	%02d/%02d/%02d/%02d/%02d/%02d	(old level)%d(==>)	(new level)%d	(combat skill)%d\n",
		    g_year - 2000, g_mon + 1, g_day, g_hour, g_min, g_sec, nOld, nNew, nCombat);
	fprintf(fp, ";Lightning Boom    %3d  Thunder Blow      %3d\n", pTarget->GetCombatLevel(LIGHTNING_BOOM), pTarget->GetCombatLevel(THUNDER_BLOW));
	fprintf(fp, ";Lightning Shock   %3d  Thunder Strike    %3d\n", pTarget->GetCombatLevel(LIGHTNING_SHOCK), pTarget->GetCombatLevel(THUNDER_STRIKE));
	fprintf(fp, ";Lightning Shield  %3d  Lightning Extreme %3d\n", pTarget->GetCombatLevel(LIGHTNING_SHIELD), pTarget->GetCombatLevel(LIGHTNING_EXTREME));
	fprintf(fp, ";Blood Earth       %3d  Ground Attack     %3d\n", pTarget->GetCombatLevel(BLOOD_EARTH), pTarget->GetCombatLevel(GROUND_ATTACK));
	fprintf(fp, ";Blood Will        %3d  Stone Armor       %3d\n", pTarget->GetCombatLevel(BLOOD_WILL), pTarget->GetCombatLevel(STONE_ARMOR));
	fprintf(fp, ";Simunian Bosom    %3d  Earth Extreme     %3d\n", pTarget->GetCombatLevel(SIMUNIAN_BOSOM), pTarget->GetCombatLevel(EARTH_EXTREME));
	fprintf(fp, ";Sword N'Roses     %3d  Multiple Fire     %3d\n", pTarget->GetCombatLevel(SWORD_N_ROSES), pTarget->GetCombatLevel(MULTIPLE_FIRE));
	fprintf(fp, ";Fleet Missile     %3d  Converting Armor  %3d\n", pTarget->GetCombatLevel(FLEET_MISSILE), pTarget->GetCombatLevel(CONVERTING_ARMOR));
	fprintf(fp, ";Burning Bloody	  %3d  Fire Extreme      %3d\n", pTarget->GetCombatLevel(BURNING_BLOODY), pTarget->GetCombatLevel(FIRE_EXTREME)); // 030415 kyo 
	fprintf(fp, ";Poisoning Nova    %3d  Cherroy Shade     %3d\n", pTarget->GetCombatLevel(POISONING_NOVA), pTarget->GetCombatLevel(CHERROY_SHADE));
	fprintf(fp, ";Dark Blade        %3d  Tybern Gift       %3d\n", pTarget->GetCombatLevel(DARK_BLADE), pTarget->GetCombatLevel(TYBERN_GIFT));
	fprintf(fp, ";AcuquireDark      %3d  Dark Extreme      %3d\n", pTarget->GetCombatLevel(ACUQUIRE_DARK), pTarget->GetCombatLevel(DARK_EXTREME)); // 030415 kyo 
	fprintf(fp, ";Icing Blast       %3d  Critical Hiding   %3d\n", pTarget->GetCombatLevel(ICING_BLAST), pTarget->GetCombatLevel(CRITICAL_HIDING));
	fprintf(fp, ";Shark Missile     %3d  Horn Of Iceberg   %3d\n", pTarget->GetCombatLevel(SHARK_MISSILE), pTarget->GetCombatLevel(HORN_OF_ICEBERG));
	fprintf(fp, ";Ice Shield        %3d  Ice Extreme       %3d\n", pTarget->GetCombatLevel(ICE_SHIELD), pTarget->GetCombatLevel(ICE_EXTREME));
	fprintf(fp, ";Charging          %3d  Double Attack     %3d\n", pTarget->GetCombatLevel(CHARGING), pTarget->GetCombatLevel(DOUBLE_ATTACK));
	fprintf(fp, ";Whilwind          %3d  Twister           %3d\n", pTarget->GetCombatLevel(WHILWIND), pTarget->GetCombatLevel(TWISTER));
	fprintf(fp, ";Gust              %3d  Wind Extreme      %3d\n", pTarget->GetCombatLevel(GUST), pTarget->GetCombatLevel(WIND_EXTREME));
	fprintf(fp, ";Reserved Combat Skill Point %3d \n", pTarget->GetCombatPoint());
	
	fclose(fp);
}	//> CSD-040224

// 030923 HK YGI
void CLogManager::SaveLogChange_LoginLogoutByKein(CHARLIST* pTarget, int type, int call_function_id )
{
	if (!g_EventFlagMgr.IsSaveLogWhenSaveUserData())
	{
		return;
	}

	char type2str[2][20] = {"LOGOUT", "LOGIN"};
	
	string strPath;

	if (GetLogPath(LT_CHANGE, strPath))
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
//> 040715_KJHuNs Change폴더에 저장

//< 040715_KJHuNs Check폴더에 저장
void CLogManager::SaveLogCheck_StrikeAttack(CHARLIST* pCaster, CHARLIST* pTarget)
{	//< CSD-030804
	return;

	string strPath;

	if (!GetLogPath(LT_CHECK, strPath))
	{
		return;
	}

	//< CSD-040224
	FILE* pFile = fopen(VA("%s/%s.txt", strPath.c_str(), pCaster->Name), "at+");

	if (pFile == NULL)
	{
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pCaster->GetServerID()].id), "at+");

		if (pFile == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	
	fprintf(pFile, "[%02d.%02d.%02d %02d:%02d:%02d]", g_year-2000, g_mon + 1, g_day, g_hour, g_min, g_sec);
	
	const int nCombat = pCaster->GetActiveCombat();
	bitset<MAX_CLASS> bsClass(Magic_Ref[nCombat].nClass);
	
	const int nClass = pCaster->Class;

	if (!bsClass[nClass])
	{
		fprintf(pFile, "Strike Attack, ");
	}
	else
	{
		fprintf(pFile, "Strike(%s), ", Magic_Ref[nCombat].Han_Name);
	}

	fprintf(pFile, "%s(%3d, %3d), ", MapName, pCaster->MoveSx, pCaster->MoveSy);
	fprintf(pFile, "%s(%d, %d), ", GetTargetName(pTarget), pTarget->MoveSx, pTarget->MoveSy);
	fprintf(pFile, "exp(%d/%d/%d), ", pCaster->GetExperienceStep(), pCaster->GetLevel(), pCaster->Exp); // CSD-030806

	const int nKind = pCaster->GetTacticsKind();
	fprintf(pFile, "%s(%d/%d/%d)\n", ConvertToTactics(nKind),
		                             pCaster->GetTacticsStep(nKind - TACTICS_Crapple),
					  			     pCaster->Skill[nKind],
									 pCaster->tac_skillEXP[nKind - TACTICS_Crapple]);
	fclose(pFile);
}	//> CSD-030804

void CLogManager::SaveLogCheck_ThrowAttack(CHARLIST* pCaster, CHARLIST* pTarget)
{	//< CSD-030804
	return;

	string strPath;

	if (!GetLogPath(LT_CHECK, strPath))
	{
		return;
	}
	
	//< CSD-040224
	FILE* pFile = fopen(VA("%s/%s.txt", strPath.c_str(), pCaster->Name), "at+");

	if (pFile == NULL)
	{
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pCaster->GetServerID()].id), "at+");

		if (pFile == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	
	fprintf(pFile, "[%02d.%02d.%02d %02d:%02d:%02d]", g_year-2000, g_mon + 1, g_day, g_hour, g_min, g_sec);
	
	const int nCombat = pCaster->GetActiveCombat();
	bitset<MAX_CLASS> bsClass(Magic_Ref[nCombat].nClass);
	
	const int nClass = pCaster->Class;

	if (!bsClass[nClass])
	{
		fprintf(pFile, "Throw Attack, ");
	}
	else
	{
		fprintf(pFile, "Throw(%s), ", Magic_Ref[nCombat].Han_Name);
	}

	fprintf(pFile, "%s(%d, %d), ", MapName, pCaster->MoveSx, pCaster->MoveSy);
	fprintf(pFile, "%s(%d, %d), ", GetTargetName(pTarget), pTarget->MoveSx, pTarget->MoveSy);
	fprintf(pFile, "exp(%d/%d/%d), ", pCaster->GetExperienceStep(), pCaster->GetLevel(), pCaster->Exp); // CSD-030806

	const int nKind = pCaster->GetTacticsKind();
	fprintf(pFile, "%s(%d/%d/%d)\n", ConvertToTactics(nKind),
		                             pCaster->GetTacticsStep(nKind - TACTICS_Crapple),
					  			     pCaster->Skill[nKind],
									 pCaster->tac_skillEXP[nKind - TACTICS_Crapple]);
	fclose(pFile);
}	//> CSD-030804

void CLogManager::SaveLogCheck_MagicExecute(BYTE nMagic, CHARLIST* pCaster, CHARLIST* pTarget, int nX, int nY)
{	//< CSD-030804
	return;

	string strPath;

	if (!GetLogPath(LT_CHECK, strPath))
	{
		return;
	}
	
	//< CSD-040224
	FILE* pFile = fopen(VA("%s/%s.txt", strPath.c_str(), pCaster->Name), "at+");

	if (pFile == NULL)
	{
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pCaster->GetServerID()].id), "at+");

		if (pFile == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	
	fprintf(pFile, "[%02d.%02d.%02d %02d:%02d:%02d]", g_year-2000, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "%s, ", Magic_Ref[nMagic].Han_Name);
	fprintf(pFile, "%s(%3d, %3d), ", MapName, pCaster->MoveSx, pCaster->MoveSy);
	fprintf(pFile, "%s(%d, %d), ", GetTargetName(pTarget), nX/32, nY/32);
	fprintf(pFile, "exp(%d/%d/%d), ", pCaster->GetExperienceStep(), pCaster->GetLevel(), pCaster->Exp); // CSD-030806

	int nKind = TACTICS_Crapple;

	switch (pCaster->Spell)
	{
	case WIZARD_SPELL: nKind = TACTICS_Magery; break;
	case PRIEST_SPELL: nKind = TACTICS_Orison; break;
	}
	
	fprintf(pFile, "%s(%d/%d/%d)\n", ConvertToTactics(nKind),
		                             pCaster->GetTacticsStep(nKind - TACTICS_Crapple),
					  			     pCaster->Skill[nKind],
									 pCaster->tac_skillEXP[nKind - TACTICS_Crapple]);
	fclose(pFile);
}	//> CSD-030804

void CLogManager::SaveLogCheck_CombatExecute(BYTE nCombat, CHARLIST* pCaster, CHARLIST* pTarget, int nX, int nY)
{	//< CSD-030804
	return;

	string strPath;

	if (!GetLogPath(LT_CHECK, strPath))
	{
		return;
	}
	//< CSD-040224
	FILE* pFile = fopen(VA("%s/%s.txt", strPath.c_str(), pCaster->Name), "at+");

	if (pFile == NULL)
	{
		pFile = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pCaster->GetServerID()].id), "at+");

		if (pFile == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(pFile, "[%02d.%02d.%02d %02d:%02d:%02d]", g_year-2000, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "%s(%d), ", Magic_Ref[nCombat].Han_Name, Magic_Ref[nCombat].nCombatCount);
	fprintf(pFile, "%s(%3d, %3d), ", MapName, pCaster->MoveSx, pCaster->MoveSy);
	fprintf(pFile, "%s(%d, %d), ", GetTargetName(pTarget), nX/32, nY/32);
	fprintf(pFile, "exp(%d/%d/%d), ", pCaster->GetExperienceStep(), 
									  pCaster->GetLevel(),
									  pCaster->Exp); // CSD-030806
	const int nKind = pCaster->GetTacticsKind();
	fprintf(pFile, "%s(%d/%d/%d)\n", ConvertToTactics(nKind),
		                             pCaster->GetTacticsStep(nKind - TACTICS_Crapple),
					  			     pCaster->Skill[nKind],
									 pCaster->tac_skillEXP[nKind - TACTICS_Crapple]);
	fclose(pFile);
}	//> CSD-030804

//<! BBD 040308 이벤트 아이템 로스트 로그
// 맵서버가 이벤트 아이템 지급에 실패했을때 남기는 로그
void CLogManager::SaveLogEventItem_Lost(int type, char * szName, int nIndex)
{
	// 로그 타입 분류
	char szReason[64];
	switch(type)
	{
	case EILT_INVALID_CONNECTION:
		sprintf(szReason, "<<Disconnected>>");
		break;
	case EILT_INVALID_NAME:
		sprintf(szReason, "<<Invalid name>>");
		break;
	case EILT_NOTENOUGH_INVENTORY:
		sprintf(szReason, "<<Not Enough Inven>>");
		break;
	default:
		sprintf(szReason, "");
		break;
	}

	string strPath;
	if (!GetLogPath(LT_EVENTITEM, strPath))
	{
		return;
	}

	// 경로에 파일을 연다
	const char* pPath = VA("%s\\EventItemLog_%02d_%02d_%02d.txt", strPath.c_str(), g_year-2000, g_mon + 1, g_day);
	FILE* pFile = fopen(pPath, "at+");
	
	if (pFile == NULL)
	{
		return;
	}

	//로그를 한줄씩 남긴다
	fprintf(pFile, "[%02d.%02d.%02d %02d:%02d:%02d]\t", g_year-2000, g_mon + 1, g_day, g_hour, g_min, g_sec);

	fprintf(pFile, "Reason : %s\tName : %s\tItemIndex : %d\t\n", szReason, szName, nIndex);
	
	fclose(pFile);
}
//> BBD 040308 이벤트 아이템 로스트 로그

// 040721_KJHuNs LevelUp폴더
void CLogManager::SaveLogLevelUp(LPCHARLIST pTarget)
{	//< CSD-TW-030622
	string strPath;

	if (!GetLogPath(LT_LEVEL_UP, strPath))
	{
		return;
	}

	//< CSD-040224
	FILE* fp = fopen(VA("%s/%s.txt", strPath.c_str(), pTarget->Name), "at+");

	if (fp == NULL)
	{
		fp = fopen(VA("%s/%s.txt", strPath.c_str(), connections[pTarget->GetServerID()].id), "at+");

		if (fp == NULL)
		{
			return;
		}
	}
	//> CSD-040224
	fprintf(fp, " %02d.%02d.%02d %02d:%02d:%02d\n", g_year-2000, g_mon + 1, g_day, g_hour, g_min, g_sec); // CSD-040202
	
	fprintf(fp, " [Ability Info]\n");
	fprintf(fp, " Level %3d     Exp   %3d     Step  %3d\n", pTarget->GetLevel(), pTarget->Exp, pTarget->GetExperienceStep()); // CSD-030806
	fprintf(fp, " Class %3d     Step  %3d     Dual  %3d\n", pTarget->Class, pTarget->GetClassStep(), pTarget->GetDualClass());
	fprintf(fp, " Str   %3d     Con   %3d     Dex   %3d\n", pTarget->Str, pTarget->Con, pTarget->Dex);
	fprintf(fp, " Wis   %3d     Int   %3d     MoveP %3d\n", pTarget->Wis, pTarget->Int, pTarget->MoveP);
	fprintf(fp, " Char  %3d     Endu  %3d     Moral %3d\n", pTarget->Char, pTarget->Endu, pTarget->Moral);
	fprintf(fp, " Luck  %3d     wsps  %3d     Reserved Point %3d\n", pTarget->Luck, pTarget->wsps, pTarget->GetReservedPoint());
	
	fprintf(fp, " [Tactic Info]\n");
	fprintf(fp, " Crapple              Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Crapple], pTarget->tac_skillEXP[0]);
	fprintf(fp, " Swordmanship         Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_swordmanship], pTarget->tac_skillEXP[1]);
	fprintf(fp, " Archery              Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Archery], pTarget->tac_skillEXP[2]);
	fprintf(fp, " Fencing              Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Fencing], pTarget->tac_skillEXP[3]);
	fprintf(fp, " Mace Fighting        Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Mace_fighting], pTarget->tac_skillEXP[4]);
	fprintf(fp, " Pierce               Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Pierce], pTarget->tac_skillEXP[5]);
	fprintf(fp, " Whirl                Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Whirl], pTarget->tac_skillEXP[6]);
	fprintf(fp, " Hurl                 Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Hurl], pTarget->tac_skillEXP[7]);
	fprintf(fp, " Parrying             Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Parrying], pTarget->tac_skillEXP[8]);
	fprintf(fp, " Double Swordmanship  Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Double_Swordmanship], pTarget->tac_skillEXP[9]);
	fprintf(fp, " Double Mace Fighting Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Double_MaceFighting], pTarget->tac_skillEXP[10]);
	fprintf(fp, " Magery               Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Magery], pTarget->tac_skillEXP[11]);
	fprintf(fp, " Orison               Tactics(%3d:%d)\n", pTarget->Skill[TACTICS_Orison], pTarget->tac_skillEXP[12]);
	
	fprintf(fp, " [Resist Info]\n");
	fprintf(fp, " Resist Poison  %3d\n", pTarget->GetBasicResist(RT_POISON));//030227 lsw
	fprintf(fp, " Resist Curse   %3d\n", pTarget->GetBasicResist(RT_CURSE));
	fprintf(fp, " Resist Fire    %3d\n", pTarget->GetBasicResist(RT_FIRE));
	fprintf(fp, " Resist Ice     %3d\n", pTarget->GetBasicResist(RT_ICE));
	fprintf(fp, " Resist Elect   %3d\n", pTarget->GetBasicResist(RT_ELECT));
	fprintf(fp, " Resist Holy    %3d\n", pTarget->GetBasicResist(RT_HOLY));
	
	fprintf(fp, " [Combat Skill Info]\n");
	fprintf(fp, " Lightning Boom    %3d  Thunder Blow      %3d\n", pTarget->GetCombatLevel(LIGHTNING_BOOM), pTarget->GetCombatLevel(THUNDER_BLOW));
	fprintf(fp, " Lightning Shock   %3d  Thunder Strike    %3d\n", pTarget->GetCombatLevel(LIGHTNING_SHOCK), pTarget->GetCombatLevel(THUNDER_STRIKE));
	fprintf(fp, " Lightning Shield  %3d  Lightning Extreme %3d\n", pTarget->GetCombatLevel(LIGHTNING_SHIELD), pTarget->GetCombatLevel(LIGHTNING_EXTREME));
	fprintf(fp, " Blood Earth       %3d  Ground Attack     %3d\n", pTarget->GetCombatLevel(BLOOD_EARTH), pTarget->GetCombatLevel(GROUND_ATTACK));
	fprintf(fp, " Blood Will        %3d  Stone Armor       %3d\n", pTarget->GetCombatLevel(BLOOD_WILL), pTarget->GetCombatLevel(STONE_ARMOR));
	fprintf(fp, " Simunian Bosom    %3d  Earth Extreme     %3d\n", pTarget->GetCombatLevel(SIMUNIAN_BOSOM), pTarget->GetCombatLevel(EARTH_EXTREME));
	fprintf(fp, " Sword N'Roses     %3d  Multiple Fire     %3d\n", pTarget->GetCombatLevel(SWORD_N_ROSES), pTarget->GetCombatLevel(MULTIPLE_FIRE));
	fprintf(fp, " Fleet Missile     %3d  Converting Armor  %3d\n", pTarget->GetCombatLevel(FLEET_MISSILE), pTarget->GetCombatLevel(CONVERTING_ARMOR));
	fprintf(fp, " Burning Bloody	%3d  Fire Extreme      %3d\n", pTarget->GetCombatLevel(BURNING_BLOODY), pTarget->GetCombatLevel(FIRE_EXTREME)); // 030415 kyo 
	fprintf(fp, " Poisoning Nova    %3d  Cherroy Shade     %3d\n", pTarget->GetCombatLevel(POISONING_NOVA), pTarget->GetCombatLevel(CHERROY_SHADE));
	fprintf(fp, " Dark Blade        %3d  Tybern Gift       %3d\n", pTarget->GetCombatLevel(DARK_BLADE), pTarget->GetCombatLevel(TYBERN_GIFT));
	fprintf(fp, " AcuquireDark      %3d  Dark Extreme      %3d\n", pTarget->GetCombatLevel(ACUQUIRE_DARK), pTarget->GetCombatLevel(DARK_EXTREME)); // 030415 kyo 
	fprintf(fp, " Icing Blast       %3d  Critical Hiding   %3d\n", pTarget->GetCombatLevel(ICING_BLAST), pTarget->GetCombatLevel(CRITICAL_HIDING));
	fprintf(fp, " Shark Missile     %3d  Horn Of Iceberg   %3d\n", pTarget->GetCombatLevel(SHARK_MISSILE), pTarget->GetCombatLevel(HORN_OF_ICEBERG));
	fprintf(fp, " Ice Shield        %3d  Ice Extreme       %3d\n", pTarget->GetCombatLevel(ICE_SHIELD), pTarget->GetCombatLevel(ICE_EXTREME));
	fprintf(fp, " Charging          %3d  Double Attack     %3d\n", pTarget->GetCombatLevel(CHARGING), pTarget->GetCombatLevel(DOUBLE_ATTACK));
	fprintf(fp, " Whilwind          %3d  Twister           %3d\n", pTarget->GetCombatLevel(WHILWIND), pTarget->GetCombatLevel(TWISTER));
	fprintf(fp, " Gust              %3d  Wind Extreme      %3d\n", pTarget->GetCombatLevel(GUST), pTarget->GetCombatLevel(WIND_EXTREME));
	fprintf(fp, " Combat Skill Point %3d \n", pTarget->GetCombatPoint());
	
	fprintf(fp, " [NK Info]\n");
	fprintf(fp, " N_NEUTRAL %d	N_NONSTER   %d	N_VYSEUS %d\n", pTarget->nk[N_NEUTRAL], pTarget->nk[N_NONSTER], pTarget->nk[N_VYSEUS]);
	fprintf(fp, " N_ZYPERN	%d	N_HEGEMONIA	%d	N_YILSE  %d\n", pTarget->nk[N_ZYPERN], pTarget->nk[N_HEGEMONIA], pTarget->nk[N_YILSE]);
	
	fprintf(fp, " [Party Info]\n");
	for (int i = 0; i < MAX_PARTY_MEMBER; ++i)
	{
		if (pTarget->party[i].On && pTarget->party[i].Name)
		{ 
			fprintf(fp, " [%d]%20s%s", i, pTarget->party[i].Name, (i==2)?"\n":"  ");
		}
	}
	
	fprintf(fp, "\n");
	fclose(fp);
}	//> CSD-TW-030622

//< 04721_KJHuNs LocalWar폴더
void CLogManager::SaveLogLocalWar_SealStoneInfo(CHARLIST* pSource, CHARLIST* pTarget, int nDamage)
{	//< CSD-040316
	if (nDamage <= 0)
	{
		return;
	}

	string strPath;

	if (!GetLogPath(LT_LOCAL_WAR,strPath))
	{
		return;
	}
	
	// 040331-YGI
	const char* pPath = VA("%s\\%d%02d%02d%02d_SealStone.txt", strPath.c_str(), g_MapPort, g_year-2000, g_mon+1, g_day);
	FILE* pFile = fopen(pPath, "at+");
	
	if (pFile == NULL)
	{
		return;
	}

	fprintf(pFile, "[%02d.%02d.%02d %02d:%02d:%02d]", g_year-2000, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "%s(%3d, %3d), %s(%3d, %3d), Damage(%d)\n", pSource->Name,
		                                                       pSource->X,
											                   pSource->Y, 
															   pTarget->Name,
		                                                       pTarget->MoveSx,
											                   pTarget->MoveSy, 
												               nDamage);
	fclose(pFile);
}	//> CSD-040316

void CLogManager::SaveLogLocalWar_Info(CHARLIST* pTarget)
{	//< CSD-040407

	/* 040721_KJHuNs SaveLog_List()에서 불러들이는 것으로 통일(정리목적)
		//%s\\%02d%02d%02d.txt 같은 파일에 저장된다.
	string strPath;

	if (!GetLogPath(LT_LOCAL_WAR,strPath))
	{
		return;
	}
	
	// 040331-YGI
	const char* pPath = VA("%s\\%02d%02d%02d.txt", strPath.c_str(), g_year - 2000, g_mon + 1, g_day);
	
	FILE* pFile = fopen(pPath, "at+");

	if (pFile == NULL)
	{
		return;
	}
	*/

	string strNation;
	switch (pTarget->name_status.nation)
	{
	case NW_BY:	
		{
			strNation = "NW_BY";
			break;
		}
	case NW_ZY:	
		{
			strNation = "NW_ZY";
			break;
		}
	case NW_YL:	
		{
			strNation = "NW_YL";
			break;
		}
	default:
		{
			strNation = "UNKNOWN";
			break;
		}
	}
	
	/* 040721_KJHuNs SaveLog_List()에서 불러들이는 것으로 통일(정리목적)
	//파일을 열고닫기때문에 비효율적이지만, 통일시켜서 이해하기 편하게 수정하였다.
	fprintf(pFile, "[%02d.%02d.%02d %02d:%02d:%02d]", g_year-2000, g_mon + 1, g_day, g_hour, g_min, g_sec);
	fprintf(pFile, "Nation(%s), Map(%d), %s(%3d, %3d)\n", strNation.c_str(),
		                                                  g_MapPort,
										                  pTarget->Name,
		                                                  pTarget->X,
											              pTarget->Y);
	fclose(pFile);
	*/

	SaveLog_List(LT_LOCAL_WAR,"[%02d.%02d.%02d %02d:%02d:%02d]",	\
					g_year-2000, g_mon + 1, g_day, g_hour, g_min, g_sec);
	SaveLog_List(LT_LOCAL_WAR,"Nation(%s), Map(%d), %s(%3d, %3d)\n",\
					strNation.c_str(),g_MapPort, pTarget->Name, pTarget->X, pTarget->Y);
}	//> CSD-040407

void CLogManager::SaveLog_List(const LOG_TYPE eLogType, LPCSTR lpLogMsg, ... )
{

	string strPath;
	if (!GetLogPath(eLogType,strPath)) { return; }

	LPCSTR lpPath = VA("%s\\%02d%02d%02d.txt", strPath.c_str(), g_year - 2000, g_mon + 1, g_day);
	FILE* pFile = fopen(lpPath, "at+");
	if (pFile == NULL) { return; }


	va_list vargs;
	// Argument Processing
	va_start( vargs, lpLogMsg );
	vfprintf(pFile,lpLogMsg,vargs);
	va_end(vargs);
	
	fclose(pFile);

}

const char* CLogManager::ConvertToDay(int nDay) const
{	//< CSD-030804
	BEGIN_SELECT(nDay)
		CASE_SELECT(SUNDAY)
		CASE_SELECT(MONDAY)
		CASE_SELECT(TUESDAY)
		CASE_SELECT(WEDNESDAY)
		CASE_SELECT(THURSDAY)
		CASE_SELECT(FRIDAY)
		CASE_SELECT(SATURDAY)
	END_SELECT(UNKNOWNDAY)
}	//> CSD-030804

const char* CLogManager::ConvertToTactics(int nKind) const
{	//< CSD-030804
	BEGIN_SELECT(nKind)
		CASE_SELECT(TACTICS_Crapple)
		CASE_SELECT(TACTICS_swordmanship)
		CASE_SELECT(TACTICS_Archery)
		CASE_SELECT(TACTICS_Fencing)
		CASE_SELECT(TACTICS_Mace_fighting)
		CASE_SELECT(TACTICS_Pierce)
		CASE_SELECT(TACTICS_Whirl)
		CASE_SELECT(TACTICS_Hurl)
		CASE_SELECT(TACTICS_Parrying)
		CASE_SELECT(TACTICS_Double_Swordmanship)
		CASE_SELECT(TACTICS_Double_MaceFighting)
		CASE_SELECT(TACTICS_Magery)
		CASE_SELECT(TACTICS_Orison)
	END_SELECT(SKILL_UNKNOWN)
}	//> CSD-030804

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

const char* CLogManager::GetTargetName(CHARLIST* pTarget) const
{	//< CSD-030804
	if (pTarget->IsPlayer())
	{
		return pTarget->Name;
	}

	return NPC_Gen_Ref[pTarget->npc_index].Name;
}	//> CSD-030804

//< LTH-040322-KO 1.4 패치. 신규 국가전 관련 로그
void CLogManager::SaveLogNeoNationWar(const int nType, char *szLogMsg, ...)
{
	va_list vargs;
	struct tm *now;
	time_t nowTime;

	int year = 0, mon = 0, day = 0;
	int hour = 0, min = 0, sec = 0;

	// Get nowtime
	time( &nowTime );
	now = localtime(&nowTime);

	// Make it usable.
	year = now->tm_year + 1900;
	mon  = now->tm_mon + 1;
	day  = now->tm_mday;
	hour = now->tm_hour;
	min  = now->tm_min;
	sec  = now->tm_sec;

	char szTime[64] = {0,};
	sprintf(szTime, "[%02d:%02d:%02d] ", hour, min, sec);

	// 로그 타입 분류
	char szType[64] = {0,};
	switch(nType)
	{
	case NNT_TIME_INFO:
		sprintf(szType, "[Time Info] ");
		break;
	case NNT_STATE_INFO:
		sprintf(szType, "[State Info] ");
		break;
	case NNT_FAME_INFO:
		sprintf(szType, "[Fame Info] ");
		break;
	case NNT_CHARACTER_INFO:
		sprintf(szType, "[User Info] ");
		break;
	case NNT_WAR_INFO:
		sprintf(szType, "[War Info] ");
		break;
	default:
		sprintf(szType, "");
		break;
	}

	va_start( vargs, szLogMsg );

	if (strlen(szLogMsg) > 2048 - (strlen(szType) + strlen(szTime) + 1))
	{
		MyLog( LOG_FATAL, "Log Too long string - This log will be lost" );
		va_end( vargs );
		return;
	}

	string strPath;
	if (!GetLogPath(LT_NEONATIONWAR, strPath))
	{
		return;
	}

	// 경로에 파일을 연다
	const char* pPath = VA("%s\\NeoNationWarLog_%02d_%02d_%02d.txt", strPath.c_str(), year-2000, mon, day);
	FILE* pFile = fopen(pPath, "at+");
	
	if (pFile == NULL)
	{
		return;
	}

	char szLog[2048] = {0,};
	strcpy(szLog, szTime);
	strcat(szLog, szType);
	char szTemp[2048] = {0,};
	vsprintf( szTemp, szLogMsg, (vargs) );
	strcat(szLog, szTemp);
	strcat(szLog, "\n");
	
	fprintf(pFile, szLog);
	fclose(pFile);
	// Finish Func
	va_end( vargs );

	::MyLog(LOG_JUST_DISPLAY, szLog);
}
//> LTH-040322-KO

/**
 * @fn		CLogManager::SaveMoveToWarfield().
 * @brief	평화 기간에 전쟁터 이동 관련 로그.
 * @param	const int nType. 로그 타입.
 * @param	char * szLogMsg. 로그 메시지.
 * @param	... .... 가변 인자.
 * @return	void.
 */
//< LTH-040515-KO.
void CLogManager::SaveMoveToWarfield(const int nType, char *szLogMsg, ...)
{
	va_list vargs;
	struct tm *now;
	time_t nowTime;

	int year = 0, mon = 0, day = 0;
	int hour = 0, min = 0, sec = 0;

	// Get nowtime
	time( &nowTime );
	now = localtime(&nowTime);

	// Make it usable.
	year = now->tm_year + 1900;
	mon  = now->tm_mon + 1;
	day  = now->tm_mday;
	hour = now->tm_hour;
	min  = now->tm_min;
	sec  = now->tm_sec;

	char szTime[64] = {0,};
	sprintf(szTime, "[%02d:%02d:%02d] ", hour, min, sec);

	// 로그 타입 분류
	char szType[64] = {0,};
	switch(nType)
	{
	case MTWT_GUARD_INFO:
		sprintf(szType, "[Guard Info] ");
		break;
	case MTWT_STATE_INFO:
		sprintf(szType, "[State Info] ");
		break;
	case MTWT_MONSTER_INFO:
		sprintf(szType, "[Monster Info] ");
		break;
	case MTWT_CHARACTER_INFO:
		sprintf(szType, "[User Info] ");
		break;
	default:
		sprintf(szType, "");
		break;
	}

	va_start( vargs, szLogMsg );

	if (strlen(szLogMsg) > 2048 - (strlen(szType) + strlen(szTime) + 1))
	{
		MyLog( LOG_FATAL, "Log Too long string - This log will be lost" );
		va_end( vargs );
		return;
	}

	string strPath;
	if (!GetLogPath(LT_MOVETOWARFIELD, strPath))
	{
		return;
	}

	// 경로에 파일을 연다
	const char* pPath = VA("%s\\MoveToWarfieldLog_%04d_%02d_%02d.txt", strPath.c_str(), year, mon, day);
	FILE* pFile = fopen(pPath, "at+");
	
	if (pFile == NULL)
	{
		return;
	}

	char szLog[2048] = {0,};
	strcpy(szLog, szTime);
	strcat(szLog, szType);
	char szTemp[2048] = {0,};
	vsprintf( szTemp, szLogMsg, (vargs) );
	strcat(szLog, szTemp);
	strcat(szLog, "\n");
	
	fprintf(pFile, szLog);
	fclose(pFile);
	// Finish Func
	va_end( vargs );

	::MyLog(LOG_JUST_DISPLAY, szLog);
}
//> LTH-040515-KO.

//040721_KJHuNs Script폴더
void CLogManager::SaveLogScriptLog(CHARLIST* pTarget, const int nQuestNo, 
								   const int nQuestStep, const char* msg) //kyo
{	//< CSD-040224
	string strPath;

	if (!g_pLogManager->GetLogPath(LT_SCRIPT, strPath))
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

	::fprintf( fp, "@ScriptLog[%d]:%d %02d-%02d-%02d %02d:%02d:%02d	(x:%d y:%d)	(Msg)%s	\n",
			nQuestNo, nQuestStep,
			g_year-2000, g_mon+1, g_day, 
			g_hour, g_min, g_sec,
			pTarget->X, pTarget->Y, msg);
	::fclose( fp );
}	//> CSD-040224

void CLogManager::ItemMallLog(char* pString)
{	//<050224_KCH 마일리지몰 작업

	SaveLog_List(KCH_ITEM_MALL_ITEM,"[%02d.%02d.%02d %02d:%02d:%02d] ",	\
					g_year-2000, g_mon + 1, g_day, g_hour, g_min, g_sec);
	SaveLog_List(KCH_ITEM_MALL_ITEM,"%s \n",	pString);
}	//>050224_KCH 마일리지몰 작업

void CLogManager::SKILL_RARE_UPGRADE_FAIL_Log(char* pString)
{	//<050224_KCH 마일리지몰 작업

	SaveLog_List(KCH_SKILL_RARE_UPGRADE_FAIL,"[%02d.%02d.%02d %02d:%02d:%02d] ",	\
					g_year-2000, g_mon + 1, g_day, g_hour, g_min, g_sec);
	SaveLog_List(KCH_SKILL_RARE_UPGRADE_FAIL,"%s \n",	pString);
}

void CLogManager::ItemMall_Use_Log(char* pString)
{	//<050224_KCH 마일리지몰 작업

	SaveLog_List(KCH_ITEM_MALL_ITEM_USE,"[%02d.%02d.%02d %02d:%02d:%02d] ",	\
					g_year-2000, g_mon + 1, g_day, g_hour, g_min, g_sec);
	SaveLog_List(KCH_ITEM_MALL_ITEM_USE,"%s \n",	pString);
}