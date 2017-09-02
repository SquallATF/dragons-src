#include "..\stdafx.h"
#include "DefaultHeader.h"
#include "Sealstone.h"
#include "..\\lowerlayers\\servertable.h"
#include "..\\lowerlayers\\servertable2.h"
#include "scrp_exe.h"
extern CServerTable* g_pServerTable;
void SendProxyWantChangeDBDEMON()
{
	//	g_pServerTable->SendToProxyServer( (char*)pPacket, dwLength );
	g_pServerTable->RequestToSetDBDemon();
}

void MoveToHomeTown(CHARLIST *Caster)
{
	switch (Caster->name_status.nation)
	{
	case NW_BY: {MapMove(Caster->GetServerID(), "MA-IN", 245, 115);	}break;
	case NW_ZY: {MapMove(Caster->GetServerID(), "RENES_C", 210, 175);	}break;
	case NW_YL: {MapMove(Caster->GetServerID(), "BARANTAN", 325, 98);	}break;
	default:		break;
	}
}

void MoveToHomeTownWithAilve(CHARLIST *Caster)
{
	SkillMgr.CharacterToAlive(Caster);
	MoveToHomeTown(Caster);
}

extern DWORD GetAbilityRedistributionMoney(t_connection c[], int cn);		// 020115 LTS

void SendCMD_COMFORM_RESET_ABILITY(CHARLIST *ch)//020820 lsw//020901 lsw
{
	if (!ch) { return; }

	for (int i = 0; i < 8; i++)
	{
		if (ch->equip[i].item_no)//장착 된게 있다면//020701 lsw
		{
			return;
		}
	}

	const int cn = ch->GetServerID();

	const DWORD tempAbilityRedistributionMoney = GetAbilityRedistributionMoney(connections, cn);
	if (ch->GetBankMoney() >= tempAbilityRedistributionMoney)	// 크거나 같다면  //돈검사
	{
		//캐릭터 이름을 날린다
		t_packet p;
		p.h.header.type = CMD_COMFORM_RESET_ABILILTY;
		p.h.header.size = sizeof(t_ComformResetAbililty);
		memcpy(p.u.ComformResetAbililty.szName, ch->Name, 20);
		p.u.ComformResetAbililty.szName[20] = 0;
		p.u.ComformResetAbililty.cn = ch->GetServerID();
		QueuePacket(connections, DB_DEMON, &p, 1);
	}
	else//돈없잖아
	{
		t_packet packet;
		packet.h.header.type = CMD_CHECK_ABILITY_CHANGE_MONEY;
		packet.h.header.size = sizeof(t_CommonDataC);
		packet.u.NationWar.CommonDataC.Data = 0;	// 실패
		QueuePacket(connections, cn, &packet, 1);
	}
}

void RecvCMD_COMFORM_RESET_ABILITY(t_packet &packet)//020820 lsw//020901 lsw
{
	CHARLIST *ch = CheckServerId(packet.u.ComformResetAbililty.cn);

	if (!ch) { return; }

	if (!packet.u.ComformResetAbililty.iIsAble)//020827 lsw
	{
		OutMessage(ch, 2, 710);
		return;
	}

	if (strcmp(ch->Name, packet.u.ComformResetAbililty.szName))
	{
		{return; }
	}
	DWORD tempAbilityRedistributionMoney;
	packet.h.header.type = CMD_CHECK_ABILITY_CHANGE_MONEY;
	packet.h.header.size = sizeof(t_CommonDataC);
	for (int i = 0; i < 8; i++)
	{
		if (ch->equip[i].item_no)//장착 된게 있다면//020701 lsw
		{
			return;
		}
	}

	const int cn = ch->GetServerID();
	tempAbilityRedistributionMoney = GetAbilityRedistributionMoney(connections, cn);

	if (ch->DecBankMoney(tempAbilityRedistributionMoney, BMCT_RESET_ABILITY)) // 돈을 없엔다
	{	//< CSD-030723
		packet.u.NationWar.CommonDataC.Data = 1;	// 성공
		RecvReqResetAbility(cn);									// 어빌재분배 루틴을 호출한다.
	}	//> CSD-030723
	else
	{
		packet.u.NationWar.CommonDataC.Data = 0;	// 실패
	}
	QueuePacket(connections, cn, &packet, 1);
}

//< kjy-040804
bool IsThereEquipedItem(CHARLIST* pMaster)
{
	for (int i = 0; i < 8; i++)
	{
		if (pMaster->equip[i].item_no)
		{
			return true;
		}
	}

	return false;
}
//> kjy-040804

//<050224_KCH 마일리지몰 작업
vector<t_ItemMallItem> g_ItemMallItem;
int LoadItemMallItemKind()	//처음에 아이템몰 매니저에서 읽어와서 참고로 하자.(DBdemon의 것을 복사.)
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	int nRowCount = 0;
	GetRowLineOfSQL(TOTAL_DB, "ItemMallItemKind", "*", &nRowCount, NULL);
	if (nRowCount == 0)
	{
		MyLog(0, "Fail, LoadItemMallItemKind() : none record");
		return 0;
	}
	g_ItemMallItem.reserve(nRowCount);

	SQLAllocStmt(g_hDBC_TotalDB, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)"Select * from ItemMallItemKind order by IndexNo", SQL_NTS);
	if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		retCode = SQLFetch(hStmt);
		while (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			t_ItemMallItem _ItemMallItem;

			retCode = SQLGetData(hStmt, 1, SQL_C_LONG, &_ItemMallItem.iIndex, 0, &cbValue);
			retCode = SQLGetData(hStmt, 2, SQL_C_LONG, &_ItemMallItem.iEvevntType, 0, &cbValue);
			retCode = SQLGetData(hStmt, 3, SQL_C_LONG, &_ItemMallItem.iItemNo, 0, &cbValue);
			retCode = SQLGetData(hStmt, 4, SQL_C_LONG, &_ItemMallItem.iRareKind1, 0, &cbValue);
			retCode = SQLGetData(hStmt, 5, SQL_C_LONG, &_ItemMallItem.iRareKind2, 0, &cbValue);
			retCode = SQLGetData(hStmt, 6, SQL_C_LONG, &_ItemMallItem.iRareKind3, 0, &cbValue);
			retCode = SQLGetData(hStmt, 7, SQL_C_LONG, &_ItemMallItem.iGrade, 0, &cbValue);
			retCode = SQLGetData(hStmt, 8, SQL_C_LONG, &_ItemMallItem.isDynamic, 0, &cbValue);
			retCode = SQLGetData(hStmt, 9, SQL_C_LONG, &_ItemMallItem.iHighRare, 0, &cbValue);
			retCode = SQLGetData(hStmt, 10, SQL_C_LONG, &_ItemMallItem.idayRemain, 0, &cbValue);	//050224_KCH 마일리지몰 작업 기간제한 아이템속성으로 시단위로 기입
			retCode = SQLGetData(hStmt, 11, SQL_C_LONG, &_ItemMallItem.ihourRemain, 0, &cbValue);	//050224_KCH 마일리지몰 작업 기간제한 아이템속성으로 분단위로 기입
			retCode = SQLGetData(hStmt, 12, SQL_C_LONG, &_ItemMallItem.iminuteRemain, 0, &cbValue);	//050224_KCH 마일리지몰 작업 기간제한 아이템속성으로 초단위로 기입

			retCode = SQLFetch(hStmt);

			MyLog(LOG_NORMAL, "ItemMall: Index(6%d)\t EventType(3%d)\t ItemNo(%6d)\t TimeRemain(%2d/%2d:%2d)",
				_ItemMallItem.iIndex, _ItemMallItem.iEvevntType, _ItemMallItem.iItemNo, _ItemMallItem.idayRemain, _ItemMallItem.ihourRemain, _ItemMallItem.iminuteRemain);

			if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
				g_ItemMallItem.push_back(_ItemMallItem);
			}
			else if (retCode == SQL_NO_DATA)
			{
				g_ItemMallItem.push_back(_ItemMallItem);	//마지막 데이터 읽어올때, 위의 SQLFetch때문에, Nodata로 가기때문에 읽어들인거 마지막으로 저장후에 종료.
				break;
			}
			else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return 0;
			}
		}
	}
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return(1);
}
//>050224_KCH 마일리지몰 작업
