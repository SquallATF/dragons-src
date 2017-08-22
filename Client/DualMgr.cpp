﻿// DualMgr.cpp: implementation of the CDualMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DualMgr.h"

#include "Char.h"
#include "Effect.h"
#include "Network.h"
#include "CharDataTable.h"
#include "SmallMenuSet.h"
#include "SymbolMgr_Client.h"

extern CHARACTERLIST g_CharacterList;
extern int UseItemByRbutton(POS pos_s, ItemAttr &item_attr);
extern CSymbolMgr_Client	g_SymBolMgr;

//class	CSymbolMgr_Client;

CDualMgr g_mgrDual;
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CDualMgr::CDualMgr()
{
	m_nPara = 0;
	m_nPosX = 0;
	m_nPosY = 0;
}

CDualMgr::~CDualMgr()
{

}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CDualMgr::Load(const char* pFile)
{

	//ifstream fin(pFile, ios::in|ios::binary);

	//for (int i = 0; i < 5; ++i)
	//{
	//	for  (int j = 0; j < 15; ++j)
	//	{
	//		fin.read((char*)(&m_aBank[i][j]), sizeof(HERO_NAME));
	//	}
	//}
	//fin.close();

	ifstream fin(pFile);
	string strLine;
	int i = 0, j = 0;

	while (getline(fin, strLine, fin.widen('\n')))
	{
		int nBegin = 0, nEnd = strLine.find("\t", 0);

		if (i != atoi(strLine.substr(nBegin, nEnd - nBegin).c_str()))
		{
			i = atoi(strLine.substr(nBegin, nEnd - nBegin).c_str());
			j = 0;
		}
		// 현재 클래스 읽어오기
		m_aBank[i][j].nBefore = i;
		// 변경할 클래스 읽어오기
		nBegin = nEnd + 1;
		nEnd = strLine.find("\t", nBegin);
		if (nEnd == string::npos)  continue;
		m_aBank[i][j].nAfter = atoi(strLine.substr(nBegin, nEnd - nBegin).c_str());
		// 클래스 단계 읽어오기
		nBegin = nEnd + 1;
		nEnd = strLine.find("\t", nBegin);
		if (nEnd == string::npos)  continue;
		m_aBank[i][j].nStep = atoi(strLine.substr(nBegin, nEnd - nBegin).c_str());
		// 명칭을 사용 가능한 레벨 읽어오기
		nBegin = nEnd + 1;
		nEnd = strLine.find("\t", nBegin);
		if (nEnd == string::npos)  continue;
		m_aBank[i][j].nLimit = atoi(strLine.substr(nBegin, nEnd - nBegin).c_str());
		// 클래스 명칭 읽어오기
		nBegin = nEnd + 1;
		nEnd = strLine.find("\t", nBegin);
		if (nEnd == string::npos)  continue;
		m_aBank[i][j].strName = string(strLine, nBegin, nEnd - nBegin);
		// 명칭에 관한 설명 읽어오기
		nBegin = nEnd + 1;
		m_aBank[i][j].strExplain = string(strLine, nBegin, strLine.length() - nBegin);
		++j;
	}

	fin.close();
}

void CDualMgr::Save(const char* pFile)
{
	//ofstream fout(pFile, ios::out|ios::binary);
	//for (int i = 0; i < 5; ++i)
	//{
	//	for (int j = 0; j < 15; ++j)
	//	{
	//		fout.write((char*)(&m_aBank[i][j]), sizeof(HERO_NAME));
	//	}
	//}
	//fout.close();

	int i = 12, j = 8920;
	string strTemp = "Test best!";
	ofstream fout(pFile);
	fout << i << j << strTemp << endl;
	fout.close();
}

string CDualMgr::GetClassName(int nBefore, int nAfter, int nStep, int nLevel) const
{
	switch (nStep)
	{
	case 0: return (nLevel >= 100) ? "贱人" : m_aBank[nBefore][(nLevel - 1) / 10].strName;
	case 1:
	case 2:
	case 3:
	case 4: return m_aBank[nBefore][nAfter + 10].strName;
	case 5: return "扑街";
	case 6:

		if (m_aBank[nBefore][nAfter + 10].strName == "至尊骑士") { return "屠龙王者"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "狂战士") { return "叛逆战神"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "武圣") { return "逐风骑士"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "魔战神") { return "魔尊騎士"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "圣武士") { return "圣堂武神"; }


		if (m_aBank[nBefore][nAfter + 10].strName == "狼牙") { return "暗影狂徒"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "夜鹰") { return "偷天换日"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "侠盗") { return "疾走幻神"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "幻影魔盗") { return "暗影之刺"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "神逐者") { return "天使神偷"; }

		if (m_aBank[nBefore][nAfter + 10].strName == "征服之翼") { return "射日箭神"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "浪人箭神") { return "追命箭神"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "神鹰之眼") { return "丛林箭神"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "魔箭圣") { return "追影射神"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "神临箭士") { return "烈火赤帝"; }


		if (m_aBank[nBefore][nAfter + 10].strName == "魔战巫师") { return "魔尊灭世"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "大占卜师") { return "暗影仲裁"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "超法师") { return "亡灵之怒"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "大贤者") { return "元素之魂"; }

		if (m_aBank[nBefore][nAfter + 10].strName == "圣殿骑士") { return "光明仲裁"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "堕天使") { return "天使之翼"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "神之怒") { return "无心天使"; }
		if (m_aBank[nBefore][nAfter + 10].strName == "圣者") { return "神之觉醒"; }

		return "大恶魔";

	}

	return "";
}

string CDualMgr::GetDulaExplain(int nBefore, int nAfter) const
{
	return m_aBank[nBefore][nAfter + 10].strExplain;
}

void CDualMgr::SendDualEnable(LPCHARACTER pMaster, BYTE nPara, BYTE nX, BYTE nY)
{
	if (pMaster == NULL)  return;

	POS pos;
	// 전투스킬 포인터 획득 아이템 위치 구하기
	::SetItemPos(INV, nPara, nY, nX, &pos);
	// 전투스킬 포인터 획득 아이템 확인
	ItemAttr& rItemAttr = InvItemAttr[nPara][nY][nX];

	if (rItemAttr.item_no == 4028)
	{ // Packet 전송
		t_packet packet;
		packet.h.header.type = CMD_DUAL_ENABLE;
		packet.h.header.size = sizeof(t_client_dual_enable);
		packet.u.combat.client_combat_obtain.idMaster = WORD(pMaster->id);
		packet.u.combat.client_combat_obtain.nPara = nPara;
		packet.u.combat.client_combat_obtain.nPosX = nX;
		packet.u.combat.client_combat_obtain.nPosY = nY;
		::QueuePacket(&packet, 1);
	}
}

void CDualMgr::SendDualChange(LPCHARACTER pMaster, BYTE nNext)
{
	if (pMaster == NULL)  return;

	POS pos;
	// 전투스킬 포인터 획득 아이템 위치 구하기
	::SetItemPos(INV, m_nPara, m_nPosY, m_nPosX, &pos);
	// 전투스킬 포인터 획득 아이템 확인
	ItemAttr& rItemAttr = InvItemAttr[m_nPara][m_nPosY][m_nPosX];

	if (rItemAttr.item_no == 4028)
	{ // Packet 전송
		t_packet packet;
		packet.h.header.type = CMD_DUAL_CHANGE;
		packet.h.header.size = sizeof(t_client_dual_change);
		packet.u.dual.client_dual_change.idMaster = WORD(pMaster->id);
		packet.u.dual.client_dual_change.nPara = m_nPara;
		packet.u.dual.client_dual_change.nPosX = m_nPosX;
		packet.u.dual.client_dual_change.nPosY = m_nPosY;
		packet.u.dual.client_dual_change.nNext = nNext;
		::QueuePacket(&packet, 1);
	}
}

void CDualMgr::SendDualDivide(LPCHARACTER pMaster, BYTE nNext)
{
	if (pMaster == NULL)  return;

	t_packet packet;
	packet.h.header.type = CMD_DUAL_DIVIDE;
	packet.h.header.size = sizeof(t_client_dual_divide);
	packet.u.dual.client_dual_divide.idMaster = WORD(pMaster->id);
	packet.u.dual.client_dual_divide.nNext = nNext;
	::QueuePacket(&packet, 1);
}

void CDualMgr::RecvDualEnable(t_server_dual_enable* pPacket)
{
	LPCHARACTER pDual = ::ReturnCharacterPoint(pPacket->idMaster);
	if (pDual == NULL)  return;

	const BYTE nPara = pPacket->nPara;
	const BYTE nX = pPacket->nPosX;
	const BYTE nY = pPacket->nPosY;
	// 전투스킬 포인터 획득 아이템 위치 구하기
	POS pos;
	::SetItemPos(INV, nPara, nY, nX, &pos);
	// 전투스킬 포인터 획득 아이템 확인
	ItemAttr& rItemAttr = InvItemAttr[nPara][nY][nX];

	if (rItemAttr.item_no == 4028)
	{ // Packet 전송
		m_nPara = nPara;
		m_nPosX = nX;
		m_nPosY = nY;
		::CallDualInterFace(MT_DUAL_CHANGE);
	}
}

void CDualMgr::RecvDualChange(t_server_dual_change* pPacket)
{
	LPCHARACTER pDual = ::ReturnCharacterPoint(pPacket->idMaster);
	if (pDual == NULL)  return;

	if (pDual == Hero)
	{
		m_nPara = 0;
		m_nPosX = 0;
		m_nPosY = 0;

		pDual->aStepInfo[CLS_STEP] = 1;              // 듀얼 클래스 단계 상승
		pDual->aStepInfo[DUAL_CLS] = pPacket->nDual; // 듀얼 클래스 설정
		SCharacterData.nCharacterAbility[FAME] = pPacket->dwFame; // 국가전 fame 점수 재설정
		SCharacterData.nCharacterAbility[STR] = pPacket->wStr;
		SCharacterData.nCharacterAbility[DEX] = pPacket->wDex;
		SCharacterData.nCharacterAbility[CON] = pPacket->wCon;
		SCharacterData.nCharacterAbility[WIS] = pPacket->wWis;
		SCharacterData.nCharacterAbility[INT_] = pPacket->wInt;
		SCharacterData.nCharacterAbility[CHA] = pPacket->wCha;
		SCharacterData.nCharacterAbility[MOVP] = pPacket->wMovep;
		SCharacterData.nCharacterAbility[ENDU] = pPacket->wEndu;
		SCharacterData.nCharacterAbility[MOR] = pPacket->wMor;
		SCharacterData.nCharacterAbility[LUCK] = pPacket->wLuck;
		SCharacterData.nCharacterAbility[WSPS] = pPacket->wWsPs;
		SCharacterData.LvUpPoint = pPacket->wPoint;


	}

	::InsertMagic(pDual, pDual, 443, -1, 0, 0, 0, 0);
}

void CDualMgr::RecvDualDivide(t_server_dual_divide* pPacket)
{
	LPCHARACTER pDual = ::ReturnCharacterPoint(pPacket->idMaster);
	if (pDual == NULL)  return;

	if (pDual == Hero)
	{

		++Hero->aStepInfo[CLS_STEP];                // 듀얼 클래스 단계 상승
		Hero->aStepInfo[DUAL_CLS] = pPacket->nDual; // 듀얼 클래스 설정
		SCharacterData.nCharacterAbility[STR] = pPacket->wStr;
		SCharacterData.nCharacterAbility[DEX] = pPacket->wDex;
		SCharacterData.nCharacterAbility[CON] = pPacket->wCon;
		SCharacterData.nCharacterAbility[WIS] = pPacket->wWis;
		SCharacterData.nCharacterAbility[INT_] = pPacket->wInt;
		SCharacterData.nCharacterAbility[CHA] = pPacket->wCha;
		SCharacterData.nCharacterAbility[MOVP] = pPacket->wMovep;
		SCharacterData.nCharacterAbility[ENDU] = pPacket->wEndu;
		SCharacterData.nCharacterAbility[MOR] = pPacket->wMor;
		SCharacterData.nCharacterAbility[LUCK] = pPacket->wLuck;
		SCharacterData.nCharacterAbility[WSPS] = pPacket->wWsPs;
		SCharacterData.LvUpPoint = pPacket->wPoint;

	}

	::InsertMagic(pDual, pDual, 443, -1, 0, 0, 0, 0);
}

int CDualMgr::GetAbility(BYTE nType)
{	// 어빌리티 최대값 가져오기
	const int nClass = Hero->class_type;
	const int nStep = Hero->aStepInfo[CLS_STEP];
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(nClass, nStep);

	switch (nType)
	{
	case STR:  return pLimit->GetStr();  // 힘
	case CON:	 return pLimit->GetCon();  // 체력
	case DEX:	 return pLimit->GetDex();  // 민첩
	case WIS:	 return pLimit->GetWis();  // 지능
	case INT_: return pLimit->GetInt();	 // 지혜
	case CHA:	 return pLimit->GetCha();  // 매력
	case MOVP: return pLimit->GetMovp(); // 이동력
	case ENDU: return pLimit->GetEndu(); // 인내
	case MOR:	 return pLimit->GetMor();  // 사기
	case LUCK: return pLimit->GetLuck(); // 행운
	case WSPS:
	{
		switch (SCharacterData.nCharacterData[SPELL])
		{
		case WIZARD_SPELL: return pLimit->GetWs(); // 마법사
		case PRIEST_SPELL: return pLimit->GetPs(); // 성직자
		}
	}
	}

	return 0;
}