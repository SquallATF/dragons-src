// DualManager.cpp: implementation of the CDualManager class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "DualManager.h"
#include "Scrp_exe.h"
#include "CItem.h"
#include "LTSSupport.h"

#include "LogManager.h"		//040719_KJHuNs g_pLogManager를 사용하기 위함.

extern inline int GetTotalAbility(LPCHARLIST ch);
extern void AddAbility(CHARLIST *ch, int type, int add);
extern int SendCharInfo(short int cn);
extern inline void	CallClient(short int id, short int type);
extern void UpdateCharacterData(t_connection c[], int cn, bool bDirect = false);

///////////////////////////////////////////////////////////////////////////////
// Global Memeber
///////////////////////////////////////////////////////////////////////////////

static int ADD_ABILITY[5][2] =
{
	{STR,  CON},
	{DEX,  MOVP},
	{DEX,  CON},
	{INT_, WSPS},
	{WIS,  WSPS}
};

static int BASIC_ABILITY[5][11] =
{
	{50, 10, 30, 10, 10, 10, 10, 10, 10, 10, 10},
	{10, 40, 20, 10, 10, 10, 20, 10, 10, 10, 20},
	{10, 50, 20, 10, 10, 10, 20, 10, 10, 10, 10},
	{10, 10, 15, 10, 40, 10, 10, 20, 10, 25, 10},
	{10, 10, 20, 35, 10, 10, 10, 20, 10, 25, 10},
};

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CDualDataInfo::CDualDataInfo()
{
	m_nMaxLevel = 0;
	m_nNeedLadder = 0;
	m_nNeedFame = 0;
	m_nConsumeLadder = 0;
	m_nConsumeFame = 0;
}

CDualDataInfo::~CDualDataInfo()
{

}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CDualClassInfo::CDualClassInfo()
{
	m_ltMember.clear();
}

CDualClassInfo::~CDualClassInfo()
{
	m_ltMember.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CDualClassInfo::AddNew(WORD idMember)
{
	m_ltMember.push_back(idMember);
}

void CDualClassInfo::Delete(WORD idMember)
{
	m_ltMember.remove(idMember);
}

void CDualClassInfo::SendPacket(t_packet* pPacket)
{
	for (ITOR_MEMBER i = m_ltMember.begin(); i != m_ltMember.end(); ++i)
	{
		const WORD idMember = *i;
		CHARLIST* pMember = ::CheckServerId(idMember);

		if (pMember != NULL)
		{
			::QueuePacket(connections, idMember, pPacket, 1);
		}
	}
}

bool CDualClassInfo::IsExist(WORD idMember) const
{
	if (find(m_ltMember.begin(), m_ltMember.end(), idMember) != m_ltMember.end())
	{
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CDualManager::CDualManager()
{
	m_vtDualData.clear();
	m_vtDualData.reserve(MAX_DUAL_STEP);

	m_vtDualClass.clear();
	m_vtDualClass.reserve(MAX_DUAL_STEP);
}

CDualManager::~CDualManager()
{
	for_each(m_vtDualData.begin(), m_vtDualData.end(), TDelete());
	m_vtDualData.clear();

	for_each(m_vtDualClass.begin(), m_vtDualClass.end(), TDelete());
	m_vtDualClass.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CDualManager::AddDualData(CDualDataInfo* pDualData)
{
	m_vtDualData.push_back(pDualData);
}

void CDualManager::AddDualClass(CDualClassInfo* pDualClass)
{
	m_vtDualClass.push_back(pDualClass);
}

bool CDualManager::AddMember(WORD idMember)
{
	CHARLIST* pMember = ::CheckServerId(idMember);

	if (pMember == NULL)
	{
		MyLog(LOG_NORMAL, "CDualManager::AddMember : pMember == NULL");
		return false;
	}

	const int nStep = pMember->GetClassStep();

	if (!IsExist(nStep))
	{
		if (nStep != 0)
		{
			MyLog(LOG_NORMAL, "CDualManager::AddMember : don't exist dual step");
			MyLog(LOG_NORMAL, ">>> dual step : %d", nStep);
		}

		return false;
	}

	if (m_vtDualClass[nStep]->IsExist(idMember))
	{
		MyLog(LOG_NORMAL, "CGuildManager::AddMember : exist user");
		MyLog(LOG_NORMAL, ">>> dual step : %d, user name : %s", nStep, pMember->Name);
		return false;
	}

	m_vtDualClass[nStep]->AddNew(idMember);
	return true;
}

bool CDualManager::DelMember(WORD idMember)
{
	CHARLIST* pMember = ::CheckServerId(idMember);

	if (pMember == NULL)
	{
		MyLog(LOG_NORMAL, "CDualManager::DelMember : pMember == NULL");
		return false;
	}

	const int nStep = pMember->GetClassStep();

	if (!IsExist(nStep))
	{
		if (nStep != 0)
		{	//< CSD-030804
			MyLog(LOG_NORMAL, "CDualManager::DelMember : don't exist dual step");
			MyLog(LOG_NORMAL, ">>> dual step : %d", nStep);
		}	//< CSD-030804

		return false;
	}

	if (!m_vtDualClass[nStep]->IsExist(idMember))
	{
		return false;
	}

	m_vtDualClass[nStep]->Delete(idMember);
	return true;
}

// modify by taniey
void CDualManager::SendDualEnable(WORD idMaster, BYTE nPara, BYTE nX, BYTE nY, BYTE nStep)
{
	t_packet packet;
	packet.h.header.type = CMD_DUAL_ENABLE;
	packet.h.header.size = sizeof(t_server_dual_enable);
	packet.u.dual.server_dual_enable.idMaster = idMaster;
	packet.u.dual.server_dual_enable.nPara = nPara;
	packet.u.dual.server_dual_enable.nPosX = nX;
	packet.u.dual.server_dual_enable.nPosY = nY;
	packet.u.dual.server_dual_enable.nStep = nStep;  // add by taniey
	::QueuePacket(connections, idMaster, &packet, 1);
}

void CDualManager::SendDualChange(WORD idMaster, BYTE nStep)
{
	CHARLIST* pMaster = ::CheckServerId(idMaster);
	if (pMaster == NULL)
		return;

	t_packet packet;
	packet.h.header.type = CMD_DUAL_CHANGE;
	packet.h.header.size = sizeof(t_server_dual_change);
	packet.u.dual.server_dual_change.idMaster = idMaster;
	packet.u.dual.server_dual_change.nStep = nStep;			// add by taniey
	//packet.u.dual.server_dual_change.nStep = pMaster->GetClassStep();			// add by taniey
	packet.u.dual.server_dual_change.nDual = pMaster->GetDualClass();
	packet.u.dual.server_dual_change.dwFame = pMaster->fame;
	packet.u.dual.server_dual_change.wStr = pMaster->Str;
	packet.u.dual.server_dual_change.wCon = pMaster->Con;
	packet.u.dual.server_dual_change.wDex = pMaster->Dex;
	packet.u.dual.server_dual_change.wWis = pMaster->Wis;
	packet.u.dual.server_dual_change.wInt = pMaster->Int;
	packet.u.dual.server_dual_change.wMovep = pMaster->MoveP;
	packet.u.dual.server_dual_change.wCha = pMaster->Char;
	packet.u.dual.server_dual_change.wEndu = pMaster->Endu;
	packet.u.dual.server_dual_change.wMor = pMaster->Moral;
	packet.u.dual.server_dual_change.wLuck = pMaster->Luck;
	packet.u.dual.server_dual_change.wWsPs = pMaster->wsps;
	packet.u.dual.server_dual_change.wPoint = pMaster->GetReservedPoint();
	::QueuePacket(connections, idMaster, &packet, 1);
}

void CDualManager::SendDualDivide(WORD idMaster, BYTE nStep)
{
	CHARLIST* pMaster = ::CheckServerId(idMaster);
	if (pMaster == NULL)
		return;

	t_packet packet;
	packet.h.header.type = CMD_DUAL_DIVIDE;
	packet.h.header.size = sizeof(t_server_dual_divide);
	packet.u.dual.server_dual_divide.idMaster = idMaster;
	packet.u.dual.server_dual_divide.nStep = nStep;			// add by taniey
	packet.u.dual.server_dual_divide.nDual = pMaster->GetDualClass();
	packet.u.dual.server_dual_divide.wStr = pMaster->Str;
	packet.u.dual.server_dual_divide.wCon = pMaster->Con;
	packet.u.dual.server_dual_divide.wDex = pMaster->Dex;
	packet.u.dual.server_dual_divide.wWis = pMaster->Wis;
	packet.u.dual.server_dual_divide.wInt = pMaster->Int;
	packet.u.dual.server_dual_divide.wMovep = pMaster->MoveP;
	packet.u.dual.server_dual_divide.wCha = pMaster->Char;
	packet.u.dual.server_dual_divide.wEndu = pMaster->Endu;
	packet.u.dual.server_dual_divide.wMor = pMaster->Moral;
	packet.u.dual.server_dual_divide.wLuck = pMaster->Luck;
	packet.u.dual.server_dual_divide.wWsPs = pMaster->wsps;
	packet.u.dual.server_dual_divide.wPoint = pMaster->GetReservedPoint();
	::QueuePacket(connections, idMaster, &packet, 1);
}

// add by taniey
void CDualManager::SendResetDualToCC(WORD idMaster, BYTE nPara, BYTE nX, BYTE nY)
{
	t_packet packet;
	packet.h.header.type = CMD_RESET_DUAL_TO_CC;
	packet.h.header.size = sizeof(t_server_reset_dual_to_cc);
	packet.u.dual.server_dual_enable.idMaster = idMaster;
	packet.u.dual.server_dual_enable.nPara = nPara;
	packet.u.dual.server_dual_enable.nPosX = nX;
	packet.u.dual.server_dual_enable.nPosY = nY;
	::QueuePacket(connections, idMaster, &packet, 1);
}


void CDualManager::RecvDualEnable(WORD idMaster, t_client_dual_enable* pPacket)
{	// 듀얼이 가능한지 검사
	CHARLIST* pMaster = ::CheckServerId(idMaster);
	if (pMaster == NULL)
		return;

	// 듀얼 여부 검사
	if (pMaster->IsRightDual(pPacket->nStep))    // modify by taniey
	{
		pMaster->Message(MK_WARNING, 0, 667, pPacket->nStep, pMaster->GetClassStep());  // old value: 383
		return;
	}
	else
	{
		if (pMaster->GetClassStep() + 1 < pPacket->nStep)  // 非当前职转
		{
			pMaster->Message(MK_WARNING, 0, 668, pPacket->nStep, pMaster->GetClassStep());  // no old value
			return;
		}
	}

	const BYTE nPara = pPacket->nPara;
	const BYTE nX = pPacket->nPosX;
	const BYTE nY = pPacket->nPosY;
	const int nStep = pPacket->nStep;     // modify by taniey

	POS pos;
	::SetItemPos(INV, nPara, nY, nX, &pos);
	ItemAttr* pAttr = ::GetItemByPOS(idMaster, pos);
	if (pAttr == NULL)  return;
	CItem* pItem = ::ItemUnit(*pAttr);
	if (pItem == NULL)  return;

	const int ni = pItem->GetRbutton();

	if (pItem->GetRbutton() == DUAL_CHANGE_ITEM)
	{
		if (IsLevelUp(nStep, pMaster))
		{
			SendDualEnable(idMaster, nPara, nX, nY, nStep);    // modify by taniey
		}
		else
		{
			pMaster->Message(MK_WARNING, 0, 665, pPacket->nStep, pMaster->GetClassStep());  // old value: 381
		}
	}
}

void CDualManager::RecvDualChange(WORD idMaster, t_client_dual_change* pPacket)
{	// 듀얼 클래스로 변환 되었다는 Packet를 받음
	CHARLIST* pMaster = ::CheckServerId(idMaster);
	if (pMaster == NULL)  return;
	// 듀얼 여부 검사
	if (pMaster->IsRightDual(pPacket->nStep))    // modify by taniey
	{
		pMaster->Message(MK_WARNING, 0, 667, pPacket->nStep, pMaster->GetClassStep());  // old value: 383

		return;
	}
	// 일반에서 듀얼로 전환하는 경우
	const BYTE nPara = pPacket->nPara;
	const BYTE nX = pPacket->nPosX;
	const BYTE nY = pPacket->nPosY;
	const int nStep = pPacket->nStep;   // modify by taniey

	POS pos;
	::SetItemPos(INV, nPara, nY, nX, &pos);
	ItemAttr* pAttr = ::GetItemByPOS(idMaster, pos);
	if (pAttr == NULL)  return;
	CItem* pItem = ::ItemUnit(*pAttr);
	if (pItem == NULL)  return;

	const int ni = pItem->GetRbutton();

	if (pItem->GetRbutton() == DUAL_CHANGE_ITEM)
	{ // 020428 YGI acer
		if (IsLevelUp(nStep, pMaster))
		{
			::SendItemEventLog(pAttr, idMaster, 0, SILT_USE, 3); //020829 lsw
			::SendDeleteItem(pAttr, &pos, pMaster, 0);
			if (pPacket->nStep <= 1) {
				pMaster->SetDualClass(pPacket->nNext); // 듀얼 클래스 설정
			}

			switch (nStep)  // for NPC sysc
			{
			case 1: s_Dual_NPC_Check = 0; break;
			case 2: s_Dual_NPC_Check = 601; break;
			case 3: s_Dual_NPC_Check = 602; break;
			case 4: s_Dual_NPC_Check = 603; break;
			case 5: s_Dual_NPC_Check = 604; break;
			case 6: s_Dual_NPC_Check = 605; break;
			}
			
			pMaster->DivideAbility(pPacket->nNext);
			Change(nStep, pMaster);
			SendDualChange(idMaster, nStep);		// modify by taniey

			//CharacterAutoUpdate();					// save to db
			UpdateCharacterData(connections, idMaster); // save to db
		}
		else
		{
			pMaster->Message(MK_WARNING, 0, 665, pPacket->nStep, pMaster->GetClassStep());  // old value: 381
		}
	}
}

void CDualManager::RecvDualDivide(WORD idMaster, t_client_dual_divide* pPacket)
{	// 듀얼 클래스 어빌리티 분배를 하겠다는 Packet를 받음
	CHARLIST* pMaster = ::CheckServerId(idMaster);
	if (pMaster == NULL)  return;
	// 듀얼의 단계가 올라가는 경우
	const int nStep = pMaster->GetClassStep() + 1;

	if (pMaster->IsRightDual(pPacket->nStep) && IsLevelUp(nStep, pMaster))		// modify by taniey
	{	//< CSD-030326
		DelMember(idMaster);
		pMaster->DivideAbility(pPacket->nNext);
		Change(nStep, pMaster);
		SendDualDivide(idMaster, nStep);			// modify by taniey
		AddMember(idMaster);
	}	//> CSD-030326
	else
	{
		pMaster->Message(MK_WARNING, 0, 666, pPacket->nStep, pMaster->GetClassStep());  // old value: 382
	}
}

void CDualManager::RecvResetAbility(WORD idMaster, void *data, int size)	//重分点函数
{
	CHARLIST* pMaster = ::CheckServerId(idMaster);
	if (pMaster == NULL)
		return;

	BYTE content[4] = { 0 };
	memcpy(content, data, size);

	const BYTE nPara = content[0];
	const BYTE nX = content[1];
	const BYTE nY = content[2];

	POS pos;
	::SetItemPos(INV, nPara, nY, nX, &pos);
	ItemAttr* pAttr = ::GetItemByPOS(idMaster, pos);
	if (pAttr == NULL)  return;
	CItem* pItem = ::ItemUnit(*pAttr);
	if (pItem == NULL)  return;

	//const int ni = pItem->GetRbutton();

	if (USE_ITEM != pItem->GetRbutton())
		return;

	if (pMaster->GetLevel() >= CROSSING_CLASS_LEVEL) // 101级重分点的限制. 
	{	//< 101分点

		::SendItemEventLog(pAttr, idMaster, 0, SILT_USE, 3); //020829 lsw
		::SendDeleteItem(pAttr, &pos, pMaster, 0);

		const int nTotal = ::GetTotalAbility(pMaster);
		const int nClass = pMaster->Class;
		// 统计点
		pMaster->Str = BASIC_ABILITY[nClass][STR];
		pMaster->Dex = BASIC_ABILITY[nClass][DEX];
		pMaster->Con = BASIC_ABILITY[nClass][CON];
		pMaster->Wis = BASIC_ABILITY[nClass][WIS];
		pMaster->Int = BASIC_ABILITY[nClass][INT_];
		pMaster->Char = BASIC_ABILITY[nClass][CHA];
		pMaster->MoveP = BASIC_ABILITY[nClass][MOVP];
		pMaster->Endu = BASIC_ABILITY[nClass][ENDU];
		pMaster->Moral = BASIC_ABILITY[nClass][MOR];
		pMaster->wsps = BASIC_ABILITY[nClass][WSPS];
		pMaster->Luck = BASIC_ABILITY[nClass][LUCK];
		const int nFirst = 3;
		::AddAbility(pMaster, ADD_ABILITY[nClass][0], nFirst);
		const int nSecond = 2;
		::AddAbility(pMaster, ADD_ABILITY[nClass][1], nSecond);

		int nMinus = 0;

		for (int i = STR; i <= LUCK; ++i)
		{
			nMinus += BASIC_ABILITY[nClass][i];
		}
		// 计算点数并且的结果
		pMaster->SetReservedPoint(nTotal - nMinus - nFirst - nSecond);
		::SendCharInfo(idMaster);
	}	//> 101分点
}

void CDualManager::RecvResetDualToCC(WORD idMaster, t_client_reset_dual_to_cc* pPacket)
{
	CHARLIST* pMaster = ::CheckServerId(idMaster);
	if (pMaster == NULL)
		return;

	if (!pMaster->IsDual()) // not dual don`t deal
		return;

	const BYTE nPara = pPacket->nPara;
	const BYTE nX = pPacket->nPosX;
	const BYTE nY = pPacket->nPosY;

	POS pos;
	::SetItemPos(INV, nPara, nY, nX, &pos);
	ItemAttr* pAttr = ::GetItemByPOS(idMaster, pos);
	if (pAttr == NULL)  return;
	CItem* pItem = ::ItemUnit(*pAttr);
	if (pItem == NULL)  return;

	//const int ni = pItem->GetRbutton();
	if (RESET_DUAL_TO_CC != pItem->GetRbutton())
		return;

	// Start clear the dual , symbol and upgrade info
	::SendItemEventLog(pAttr, idMaster, 0, SILT_USE, 3); //020829 lsw
	::SendDeleteItem(pAttr, &pos, pMaster, 0);

	s_SymBol_Get_NPC_Check_WARRIOR = 0;
	s_SymBol_Get_NPC_Check_THIEF = 0;
	s_SymBol_Get_NPC_Check_ARCHER = 0;
	s_SymBol_Get_NPC_Check_WIZARD = 0;
	s_SymBol_Get_NPC_Check_PRIEST = 0;

	s_SymBol_Upgrade_NPC_Check = 0;
	
	s_Dual_NPC_Check = 0;

	// reset to cc.
	int nGrade = 0;
	// reset to 0
	pMaster->quick[5].item_no = 0;
	pMaster->quick[5].attr[0] = nGrade;
	pMaster->SetDualClass(0); // 듀얼 클래스 설정
	pMaster->SetClassStep(0);

	//1.reset Ability first 
	//RecvResetAbility(idMaster);

	SendResetDualToCC(idMaster, nPara, nX, nY);
	//pMaster->Message(MK_NORMAL, 4, 143, 0, 0);
	//CharacterAutoUpdate();  // save to db
	UpdateCharacterData(connections, idMaster); // save to db

	Send_RareItemMakeLog(pMaster->GetServerID(), 0, -1, nGrade, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

void CDualManager::Change(BYTE nStep, CHARLIST* pMaster)
{	//< CSD-TW-030606 : 듀얼 클래스 단계 상승에 따른 소모 Ladder나 Fame
	CDualDataInfo* pDualData = GetDualDataInfo(nStep);

	if (pDualData == NULL)
	{
		return;
	}

	switch (nStep)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	{
		if (pMaster->LadderScore >= pDualData->GetNeedLadder())
		{
			g_pLogManager->SaveLogChange_ClassStep(pMaster, nStep - 1, nStep);
			const int change = pDualData->GetConsumeLadder();
			g_pLogManager->SaveLogChange_Ladder(pMaster, pMaster->LadderScore, pMaster->LadderScore - change, CLT_CHANGE_DUAL);
			pMaster->LadderScore -= pDualData->GetConsumeLadder();
			return;
		}

		if (pMaster->fame >= pDualData->GetNeedFame())
		{
			g_pLogManager->SaveLogChange_ClassStep(pMaster, nStep - 1, nStep);
			const int DecFame = pDualData->GetConsumeFame();
			::g_pLogManager->SaveLogChange_Fame(pMaster, pMaster->fame, pMaster->fame - DecFame, LF_DUAL);
			pMaster->fame -= DecFame;
			return;
		}

		break;
	}
	case 5:
	{
		g_pLogManager->SaveLogChange_ClassStep(pMaster, nStep - 1, nStep);
		const int change = pDualData->GetConsumeLadder();
		g_pLogManager->SaveLogChange_Ladder(pMaster, pMaster->LadderScore, pMaster->LadderScore - change, CLT_CHANGE_DUAL);
		pMaster->LadderScore -= pDualData->GetConsumeLadder();
		const int DecFame = pDualData->GetConsumeFame();
		::g_pLogManager->SaveLogChange_Fame(pMaster, pMaster->fame, pMaster->fame - DecFame, LF_DUAL);
		pMaster->fame -= DecFame;
		break;
	}
	case 6:  //coromo 转职条件
	{
		g_pLogManager->SaveLogChange_ClassStep(pMaster, nStep - 1, nStep);
		const int change = pDualData->GetConsumeLadder();
		g_pLogManager->SaveLogChange_Ladder(pMaster, pMaster->LadderScore, pMaster->LadderScore - change, CLT_CHANGE_DUAL);
		pMaster->LadderScore -= pDualData->GetConsumeLadder();
		const int DecFame = pDualData->GetConsumeFame();
		::g_pLogManager->SaveLogChange_Fame(pMaster, pMaster->fame, pMaster->fame - DecFame, LF_DUAL);
		pMaster->fame -= DecFame;
		break;
	}
	}
}	//> CSD-TW-030606

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

bool CDualManager::IsLevelUp(BYTE nStep, CHARLIST* pMaster)
{	// 듀얼 클래스 단계 상승이 가능한지 여부
	CDualDataInfo* pDualData = GetDualDataInfo(nStep);

	if (pDualData == NULL)
	{
		return false;
	}

	switch (nStep)
	{	//< CSD-030806
	case 1:
	case 2:
	case 3:
	case 4:
	{
		if (pMaster->GetLevel() >= pDualData->GetMaxLevel())
		{
			if (pMaster->LadderScore >= pDualData->GetNeedLadder())
			{
				return true;
			}

			if (pMaster->fame >= pDualData->GetNeedFame())
			{
				return true;
			}
		}

		break;
	}
	case 5:
	{
		if (pMaster->GetLevel() >= pDualData->GetMaxLevel())
		{
			if (pMaster->LadderScore < pDualData->GetNeedLadder())
			{
				break;
			}

			if (pMaster->fame < pDualData->GetNeedFame())
			{
				break;
			}

			return true;
		}

		break;
	}
	case 6:  //coromo 转职条件
	{
		if (pMaster->GetLevel() >= pDualData->GetMaxLevel())
		{
			if (pMaster->LadderScore < pDualData->GetNeedLadder())
			{
				break;
			}

			if (pMaster->fame < pDualData->GetNeedFame())
			{
				break;
			}

			return true;
		}

		break;
	}
	}	//> CSD-030806

	return false;
}

//< kjy-040804
extern bool IsThereEquipedItem(CHARLIST* pMaster);
void CDualManager::RecvResetAbilityItem(WORD idMaster)
{
	CHARLIST* pMaster = ::CheckServerId(idMaster);
	if (pMaster == NULL)  return;

	// 착용한 아이템이 있는지 확인해야 한다.
	if (!IsThereEquipedItem(pMaster))
	{	//< CSD-030806
		const int nTotal = ::GetTotalAbility(pMaster);
		const int nClass = pMaster->Class;
		// 어빌리티 초기화
		pMaster->Str = BASIC_ABILITY[nClass][STR];
		pMaster->Dex = BASIC_ABILITY[nClass][DEX];
		pMaster->Con = BASIC_ABILITY[nClass][CON];
		pMaster->Wis = BASIC_ABILITY[nClass][WIS];
		pMaster->Int = BASIC_ABILITY[nClass][INT_];
		pMaster->Char = BASIC_ABILITY[nClass][CHA];
		pMaster->MoveP = BASIC_ABILITY[nClass][MOVP];
		pMaster->Endu = BASIC_ABILITY[nClass][ENDU];
		pMaster->Moral = BASIC_ABILITY[nClass][MOR];
		pMaster->wsps = BASIC_ABILITY[nClass][WSPS];
		pMaster->Luck = BASIC_ABILITY[nClass][LUCK];
		const int nFirst = 3;
		::AddAbility(pMaster, ADD_ABILITY[nClass][0], nFirst);
		const int nSecond = 2;
		::AddAbility(pMaster, ADD_ABILITY[nClass][1], nSecond);

		int nMinus = 0;

		for (int i = STR; i <= LUCK; ++i)
		{
			nMinus += BASIC_ABILITY[nClass][i];
		}
		// 어빌리티 포인터 재설정
		pMaster->SetReservedPoint(nTotal - nMinus - nFirst - nSecond);
		::SendCharInfo(idMaster);

		MyLog(0, "%s Ability Reset by item", pMaster->Name);
	}	//> CSD-030806
}
//> kjy-040804
const int RESET_ABILITY_EACH_STAT_ITEM_NO = 1061;   //1061

void CDualManager::RecvResetAbilityEachStat(WORD idMaster, const int Statkind /*STR ~ LUCK*/)
{
	CHARLIST* pMaster = ::CheckServerId(idMaster);
	if (pMaster == NULL)  return;

	// 착용한 아이템이 있는지 확인해야 한다.
	if (!IsThereEquipedItem(pMaster))
	{//스텟을 확인 해 보고//아이템이 있는지 확인 하고 
		short int AfterReserve = 0;
		const short int nClass = pMaster->Class;
		const short int iBase = BASIC_ABILITY[nClass][Statkind];

		short int *pAbility = NULL;

		switch (Statkind)
		{
		case STR: {pAbility = &pMaster->Str;	}break;
		case DEX: {pAbility = &pMaster->Dex;	}break;
		case CON: {pAbility = &pMaster->Con;	}break;
		case WIS: {pAbility = &pMaster->Wis;	}break;
		case INT_: {pAbility = &pMaster->Int;	}break;
		case CHA: {pAbility = &pMaster->Char;	}break;
		case MOVP: {pAbility = &pMaster->MoveP; }break;
		case ENDU: {pAbility = &pMaster->Endu;	}break;
		case MOR: {pAbility = &pMaster->Moral; }break;
		case WSPS: {pAbility = &pMaster->wsps;	}break;
		case LUCK: {pAbility = &pMaster->Luck;	}break;
		default:
		{
			MyLog(LOG_FATAL, "RecvResetAbilityEachStat Incorrect StatKind");
			return;
		}break;
		}

		AfterReserve = (*pAbility) - iBase;//남을 리저브 포인트를 계산하고

		const int nFirst = 3;
		const int nSecond = 2;
		int BonusAddOn = 0;
		if (Statkind == ADD_ABILITY[nClass][0])
		{
			BonusAddOn += nFirst;
		}

		if (Statkind == ADD_ABILITY[nClass][1])
		{
			BonusAddOn += nSecond;
		}

		if (BonusAddOn >= AfterReserve)
		{
			::OutMessage(pMaster, 2, 996);//임시,,미니멈 상태 입니다.
			return;
		}

		//아이템 확인 ->
		const int Lv = pMaster->GetLevel();
		int need_count = 1;
		if (Lv > 104) { need_count = 5; }
		if (104 >= Lv) { need_count = 3; }
		if (80 >= Lv) { need_count = 2; }
		if (50 >= Lv) { need_count = 1; }

		const int iNowCt = ::CheckInventory(pMaster, RESET_ABILITY_EACH_STAT_ITEM_NO, need_count);//내가 원한 값에서 현재 결과를 주니까 -값이 나올 수 있다
		if (0 <= iNowCt)//갯수가 맞으면 지워라
		{
			::SendDeleteItemQuantity(pMaster, RESET_ABILITY_EACH_STAT_ITEM_NO, need_count);		// 특정 아이템을 특정 갯수만큼 삭제 한다.
		}
		else
		{
			::OutMessage(pMaster, 2, 994);//임시,,재분배 아이템이 모자랍니다.
			return;
		}
		//아이템 확인 <-

		(*pAbility) = iBase;//어빌리티 베이스를 셋팅 해준다. 

		if (Statkind == ADD_ABILITY[nClass][0])
		{
			::AddAbility(pMaster, ADD_ABILITY[nClass][0], nFirst);//주사위 굴린 어빌 지급
			AfterReserve -= nFirst;
		}

		if (Statkind == ADD_ABILITY[nClass][1])
		{
			::AddAbility(pMaster, ADD_ABILITY[nClass][1], nSecond);
			AfterReserve -= nSecond;
		}

		const int iBeforeReservePoint = pMaster->GetReservedPoint();//리저브 포인트에 더해줘야 한다.

		pMaster->SetReservedPoint(iBeforeReservePoint + AfterReserve);
		::SendCharInfo(idMaster);

		MyLog(0, "%s Ability Reset Each Stat by item", pMaster->Name);
	}
	else
	{
		::OutMessage(pMaster, 0, 500);//임시,,인벤토리 공간이 충분치 않습니다.
	}//> CSD-030806
}

