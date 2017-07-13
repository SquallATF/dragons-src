/**
 * @file	TeamInfo.cpp.
 *			implementation of the CTeamInfo class.
 */
#include "..\stdafx.h"
#include "TeamInfo.h"
#include "WarfieldInfo.h"
#include "LogManager.h"

CSquad::CSquad()
: m_nSquadLeader(-1)
, m_nSquadNo(-1)
{
	m_ltSquad.clear();
	m_xyLoadingPoint.x = 0;
	m_xyLoadingPoint.y = 0;
	INT nI;
	for (nI = 0; nI < 3; ++nI)
	{
		m_naNumOfJoinNation[nI] = 0;
		m_naNumOfDeathNation[nI] = 0;
	}
}

/**
 * @fn		CSquad::GetNationMemberCount().
 * @brief	�� ������ �ο� ���� ��ȯ �Ѵ�.
 * @param	INT nNationNo. Ư�� ����.
 * @return	INT. Ư�� ������ �ο� ��.
 */
//< LTH-040419-KO.
INT CSquad::GetNationMemberCount(INT nNationNo)
{
	switch(nNationNo)
	{
	case NW_BY:
		return m_naNumOfJoinNation[0];

	case NW_ZY:
		return m_naNumOfJoinNation[1];

	case NW_YL:
		return m_naNumOfJoinNation[2];
	}
	
	return 0;
}	//> LTH-040419-KO.

/**
 * @fn		CSquad::GetMemberCount().
 * @brief	�� �δ��� �� �ο� �� ��ȯ.
 * @return	INT. �� �δ��� �� �ο� ��.
 */
//< LTH-040419-KO.
INT CSquad::GetMemberCount()
{
	INT nMemberCount = m_ltSquad.size();
	return nMemberCount;
}	//> LTH-040419-KO.

/**
 * @fn		CSquad::InsertMember().
 * @brief	�δ�� �߰��ϴ� �Լ�.
 * @param	INT nCn. ���� �ν� ��ȣ.
 * @return	VOID.
 */
//< LTH-040419-KO.
VOID CSquad::InsertMember(INT nCn)
{
	SquadMember* lpSquadMember = new SquadMember;
	lpSquadMember->SetData(nCn);
	m_ltSquad.push_back(lpSquadMember);

	switch (connections[nCn].chrlst.name_status.nation)
	{
	case NW_BY:
		++m_naNumOfJoinNation[0];
		break;

	case NW_ZY:
		++m_naNumOfJoinNation[1]; 
		break;

	case NW_YL:
		++m_naNumOfJoinNation[2];
		break;
	}
	connections[nCn].chrlst.NWCharacter.SquadNo = (DWORD)m_nSquadNo;
	//< LTH-040419-KO �α� ��ȭ.
	g_pLogManager->SaveLogNeoNationWar(NNT_CHARACTER_INFO, "%s Join in Squad %d, Nation %d", connections[nCn].name, \
		m_nSquadNo, connections[nCn].chrlst.name_status.nation);
	g_pLogManager->SaveLogNeoNationWar(NNT_CHARACTER_INFO, "Number Of %d Squad Member : %d", m_nSquadNo, \
		m_ltSquad.size());
	//> LTH-040419-KO.
}	//> LTH-040419-KO.

/**
 * @fn		CSquad::DeleteMember().
 * @brief	�δ�� ����.
 * @param	INT nCn. ���� �ν� ��ȣ.
 * @return	VOID.
 */
//< LTH-040419-KO.
VOID CSquad::DeleteMember(INT nCn)
{
	INT nCheckID;
	SquadMember* lpSquadMember;
	LTSQUADIT ltSquadIt = m_ltSquad.begin();
	LTSQUADIT ltSquadItEnd = m_ltSquad.end();
	while (ltSquadIt != ltSquadItEnd)
	{ 
		nCheckID = (*ltSquadIt)->GetData();
		if (nCheckID == nCn)
		{
			switch (connections[nCn].chrlst.name_status.nation)
			{
			case NW_BY:
				--m_naNumOfJoinNation[0];
				break;

			case NW_ZY:
				--m_naNumOfJoinNation[1];
				break;

			case NW_YL:
				--m_naNumOfJoinNation[2];
				break;
			}
			//< LTH-040419-KO �α� �߰�!!
			connections[nCn].chrlst.NWCharacter.SquadNo = 0;
			g_pLogManager->SaveLogNeoNationWar(NNT_CHARACTER_INFO, "%s Withdrew from Squad %d, Nation %d", connections[nCn].name, \
				m_nSquadNo, connections[nCn].chrlst.name_status.nation);
			g_pLogManager->SaveLogNeoNationWar(NNT_CHARACTER_INFO, "Number Of %d Squad Member : %d", m_nSquadNo, m_ltSquad.size() - 1);
			//> LTH-040419-KO

			lpSquadMember = (*ltSquadIt);
			m_ltSquad.remove(lpSquadMember);
			SAFE_DELETE(lpSquadMember);
			connections[nCn].chrlst.NWCharacter.SquadNo = 0;
			break;
		}
		else
			ltSquadIt++; 
	}
}	//> LTH-040419-KO.

/**
 * @fn		CSquad::SendMessageAllMember().
 * @brief	�� �δ������ ä�� �޽��� ������.
 * @param	t_packet * p. ��Ŷ ����.
 * @return	VOID.
 */
//< LTH-040419-KO.
VOID CSquad::SendMessageAllMember(t_packet *p)
{
	INT nCn = -1;
	LTSQUADIT ltSquadIt = m_ltSquad.begin();
	LTSQUADIT ltSquadItEnd = m_ltSquad.end();
	for (; ltSquadIt != ltSquadItEnd; ltSquadIt++)
	{
		nCn = (*ltSquadIt)->GetData();
		QueuePacket(connections, nCn, p, 1);
	}
}	//> LTH-040419-KO.

/**
 * @fn		CSquad::FindMember().
 * @brief	�δ�� ã��.
 * @param	INT nCn. ���� �ν� ��ȣ.
 * @return	BOOL. ã������ TRUE.
 */
//< LTH-040419-KO.
BOOL CSquad::FindMember(INT nCn)
{
	INT nID = -1;
	LTSQUADIT ltSquadIt = m_ltSquad.begin();
	LTSQUADIT ltSquadItEnd = m_ltSquad.end();
	BOOL bFind = FALSE;
	for (; ltSquadIt != ltSquadItEnd; ltSquadIt++)
	{
		nID = (*ltSquadIt)->GetData();
		if (nID == nCn)
		{
			bFind = TRUE;
			break;
		}
	}

	return bFind;
}	//> LTH-040419-KO.

/**
 * @fn		CSquad::GetNationDeathCount().
 * @brief	Ư�� ������ ���� ��� �� ��ȯ.
 * @param	INT nNationNo. Ư�� ���� ��ȣ.
 * @return	INT. ���� ��� ��.
 */
//< LTH-040419-KO.
INT CSquad::GetNationDeathCount(INT nNationNo)
{
	switch(nNationNo)
	{
	case NW_BY:
		return m_naNumOfDeathNation[0];

	case NW_ZY:
		return m_naNumOfDeathNation[1];

	case NW_YL:
		return m_naNumOfDeathNation[2];
	}
	
	return 0;
}	//> LTH-040419-KO.

extern void SendFameChange(LPCHARLIST lpChar,int AddFame);

/**
 * @fn		CSquad::GiveFame().
 * @brief	Ÿ�� �������κ��� ���� �Ÿ��� �ִ� ������� �ش� ��ġ�� �ش�.
 * @param	LPCHARLIST lpCaster. ��ġ�� �޴µ� ���� �� ���,
 * @param	LPCHARLIST lpTarget. Ÿ��.
 * @param	INT nRange. ���� �Ÿ�
 * @param	INT nAddFame. ���� ��ġ.
 * @return	VOID.
 */
//< LTH-040419-KO.
VOID CSquad::GiveFame(LPCHARLIST lpCaster, LPCHARLIST lpTarget, INT nRange, INT nAddFame)
{
	INT nCn;
	DWORD dwTileRange = (nRange * TILE_SIZE) * (nRange * TILE_SIZE);
	DWORD dwTargetRange = 0;
	INT nSx = lpTarget->X;
	INT nSy = lpTarget->Y;
	INT nDx, nDy;
	LPCHARLIST	lpChar;
	LTSQUADIT ltSquadIt = m_ltSquad.begin();
	LTSQUADIT ltSquadItEnd = m_ltSquad.end();
	for (; ltSquadIt != ltSquadItEnd; ltSquadIt++)
	{
		nCn = (*ltSquadIt)->GetData();
		if (lpCaster->GetServerID() != nCn)
		{
			lpChar = &connections[nCn].chrlst;
			nDx = lpChar->X;
			nDy = lpChar->Y;
			dwTargetRange = (nSx - nDx) * (nSx - nDx) + (nSy - nDy) * (nSy - nDy);
			if (dwTileRange > dwTargetRange)
			{
				g_pLogManager->SaveLogChange_Fame(lpChar, lpChar->fame, lpChar->fame + nAddFame, LF_NATIONWAR);
				g_pLogManager->SaveLogNeoNationWar(NNT_FAME_INFO, "Fame Change, Name : %s, OldFame : %d, NewFame : %d, Gap : %d", \
					lpChar->Name, lpChar->fame, lpChar->fame + nAddFame, nAddFame);
				lpChar->fame += nAddFame;
				::SendFameChange(lpChar, nAddFame);
			}
		}
	}
}	//> LTH-040419-KO.

#include "Scrp_exe.h"
/*040719_KJHuNs g_pLogManager�� �̵�(��������)
extern void SaveLogChange_DualFame( CHARLIST *ch, const int nOldDualFame, const int nNewDualFame, eLDF_TYPE type);
*/

/**
 * @fn		CSquad::GiveFame2Dual().
 * @brief	��� ��ġ�� �ش�.
 * @param	INT nAddFame. ���� ��ġ.
 * @return	VOID.
 */
//< LTH-040419-KO.
VOID CSquad::GiveFame2Dual(INT nAddFame)
{
	INT nCn = 0;
	LPCHARLIST lpChar = NULL;

	LTSQUADIT ltSquadIt = m_ltSquad.begin();
	LTSQUADIT ltSquadItEnd = m_ltSquad.end();
	for (; ltSquadIt != ltSquadItEnd; ltSquadIt++)
	{
		nCn = (*ltSquadIt)->GetData();
		lpChar = &connections[nCn].chrlst;
		if (NULL == lpChar)
			continue;
		if (lpChar->IsDual())
		{
			g_pLogManager->SaveLogChange_DualFame(lpChar, lpChar->fame, lpChar->fame + nAddFame, LDF_NEOWAR);
			g_pLogManager->SaveLogNeoNationWar(NNT_FAME_INFO, "Fame Change, Name : %s, OldFame : %d, NewFame : %d, Gap : %d", \
				lpChar->Name, lpChar->fame, lpChar->fame + nAddFame, nAddFame);
			lpChar->fame += nAddFame;
			::SendFameChange(lpChar, nAddFame);
		}
	}
}	//> LTH-040419-KO.

/**
 * @fn		CSquad::AddDeathCount().
 * @brief	Ư�� ������ ���� ����� �߰�.
 * @param	INT nNationNo. Ư�� ���� ��ȣ.
 * @return	VOID.
 */
//< LTH-040419-KO.
VOID CSquad::AddDeathCount(INT nNationNo)
{
	switch(nNationNo)
	{
	case NW_BY:
		++m_naNumOfDeathNation[0];
		break;

	case NW_ZY:
		++m_naNumOfDeathNation[1];
		break;

	case NW_YL:
		++m_naNumOfDeathNation[2];
		break;
	}
}	//> LTH-040419-KO.

/**
 * @fn		CSquad::DeleteAllMember().
 * @brief	��� �δ�� ����.
 * @return	VOID.
 */
//< LTH-040419-KO.
VOID CSquad::DeleteAllMember()
{
	SquadMember* lpSquadMember;
	LTSQUADIT ltSquadIt = m_ltSquad.begin();
	LTSQUADIT ltSquadItEnd = m_ltSquad.end();
	while (ltSquadIt != ltSquadItEnd)
	{
		lpSquadMember = (*ltSquadIt);
		m_ltSquad.remove(lpSquadMember);
		SAFE_DELETE(lpSquadMember);

		ltSquadIt++;
	}

	m_nSquadLeader = -1;
	m_ltSquad.clear();
	INT nI;
	for (nI = 0; nI < 3; ++nI)
	{
		m_naNumOfJoinNation[nI] = 0;
		m_naNumOfDeathNation[nI] = 0;
	}
}	//> LTH-040419-KO.

/**
 * @fn		CSquad::RemoveArmor().
 * @brief	������ �����.
 * @return	VOID.
 */
//< LTH-040419-KO.
VOID CSquad::RemoveArmor()
{
	INT nID;
	LTSQUADIT ltSquadIt = m_ltSquad.begin();
	LTSQUADIT ltSquadItEnd = m_ltSquad.end();

	while (ltSquadIt != ltSquadItEnd)
	{
		nID = (*ltSquadIt)->GetData();
		::CheckAndSendChangeEquip(&connections[nID].chrlst, 1);
		ltSquadIt++;
	}
}	//> LTH-040419-KO.

//----------------------------------------------------------------------------------

/**
 * @fn		CTeamInfo::GetNationMemberCount().
 * @brief	Ư�� ������ ������� ��ȯ.
 * @param	INT nNationNo. Ư�� ���� ��ȣ.
 * @return	INT. Ư�� ������ ��� ��.
 */
//< LTH-040419-KO.
INT CTeamInfo::GetNationMemberCount(INT nNationNo)
{
	INT nNationMember = 0;
	INT nI;
	for (nI = 0; nI < m_nNumOfSquad; ++nI)
	{
		nNationMember += m_lpSquad[nI].GetNationMemberCount(nNationNo);
	}

	return nNationMember;
}	//> LTH-040419-KO.

/**
 * @fn		CTeamInfo::GetSquadMemberCount().
 * @brief	Ư�� �δ��� �δ�� ��.
 * @param	INT nSquadNo. Ư�� �δ� ��ȣ.
 * @return	INT. Ư�� �δ��� �δ�� ��.
 */
//< LTH-040419-KO.
INT CTeamInfo::GetSquadMemberCount(INT nSquadNo)
{
	//< LTH-040304-KO �δ밡 ���� ���� ���� �Ͱ� �������� �̹Ƿ�...
	if (m_nNumOfSquad == 0)
		return 0;
	//> LTH-040304-KO

	INT nSquadMember = m_lpSquad[nSquadNo].GetMemberCount();
	return nSquadMember;
}	//> LTH-040419-KO.

extern LPWARFIELDINFO g_pcWarfieldInfo;

/**
 * @fn		CTeamInfo::CheckSquadOfCharacter().
 * @brief	Ư�� ������ ������ �ش� ���￡���� ��� �δ��ΰ� ��ȯ.
 * @param	INT nWarfieldNo. ���� ������ ��ȣ.
 * @param	INT nCharLv. Ư�� ������ ����.
 * @return	INT. ��� �δ�.
 */
//< LTH-040419-KO.
INT CTeamInfo::CheckSquadOfCharacter(INT nWarfieldNo, INT nCharLv)
{
	CSoldierSet cSoldier = g_pcWarfieldInfo->GetSoldierSet(nWarfieldNo);
	INT nSquadNo = -1;
	INT nI;
	for (nI = 0; nI < m_nNumOfSquad; ++nI)
	{
		if ((nCharLv >= cSoldier.m_tagaSquad[nI].nLevelMin) && (nCharLv <= cSoldier.m_tagaSquad[nI].nLevelMax))
			break;
	}
	nSquadNo = nI;

	return nSquadNo;
}	//> LTH-040419-KO.

/**
 * @fn		CTeamInfo::Init().
 * @brief	CTeamInfo class �ʱ�ȭ.
 * @param	INT nNumOfSquad. �� �δ� ��.
 * @return	HRESULT.
 */
//< LTH-040419-KO.
HRESULT CTeamInfo::Init(INT nNumOfSquad)
{
	m_nNumOfSquad = nNumOfSquad;
	//< LTH-040212-KO ��� �� �δ�� �־���Ѵ�
	if (m_nNumOfSquad == 0)
		return E_FAIL;
	//> LTH-040212-KO
	m_lpSquad = new CSquad[m_nNumOfSquad];
	Dbg_Assert(NULL != m_lpSquad);

	INT nI;
	for (nI = 0; nI < m_nNumOfSquad; ++nI)
		m_lpSquad[nI].SetSquadNo(nI);
	return S_OK;
}	//> LTH-040419-KO.

/**
 * @fn		CTeamInfo::InsertSquadMember().
 * @brief	�δ�� �߰�.
 * @param	INT nWarfieldNo. ������ ��ȣ.
 * @param	INT nCn. ���� �ν� ��ȣ.
 * @return	VOID.
 */
//< LTH-040419-KO.
VOID CTeamInfo::InsertSquadMember(INT nWarfieldNo, INT nCn)
{
	//< LTH-040505-KO ��� ĳ���� �δ뿡 ���� ��Ű�� �������� �߰�
	LPCHARLIST lpChar = ::CheckServerId(nCn);
	if (lpChar == NULL)
		return;
	if (lpChar->IsCounselor()) 
		return;
	//> LTH-040505-KO

	INT nCharLv = connections[nCn].chrlst.GetLevel();
	INT nSquadNo = CheckSquadOfCharacter(nWarfieldNo, nCharLv);
	m_lpSquad[nSquadNo].InsertMember(nCn);
}	//> LTH-040419-KO.

/**
 * @fn		CTeamInfo::DeleteSquadMember().
 * @brief	�δ�� ����.
 * @param	INT nWarfieldNo. �����͹�ȣ.
 * @param	INT nCn. ���� �ν� ��ȣ.
 * @return	VOID.
 */
//< LTH-040419-KO.
VOID CTeamInfo::DeleteSquadMember(INT nWarfieldNo, INT nCn)
{
	//< LTH-040505-KO ��� ĳ���� �δ뿡 ���� ��Ű�� �������� �߰�
	LPCHARLIST lpChar = ::CheckServerId(nCn);
	if (lpChar == NULL)
		return;
	if (lpChar->IsCounselor()) 
		return;
	//> LTH-040505-KO
	
	INT nCharLv = connections[nCn].chrlst.GetLevel();
	INT nSquadNo = CheckSquadOfCharacter(nWarfieldNo, nCharLv);
	m_lpSquad[nSquadNo].DeleteMember(nCn);
}	//> LTH-040419-KO.

/**
 * @fn		CTeamInfo::SendMessageAllMember().
 * @brief	�δ�� ä�� ������ ����.
 * @param	t_packet * p. ��Ŷ ����.
 * @return	VOID.
 */
//< LTH-040419-KO.
VOID CTeamInfo::SendMessageAllMember(t_packet *p)
{
	INT nI;
	for (nI = 0; nI < m_nNumOfSquad; ++nI)
		m_lpSquad[nI].SendMessageAllMember(p);
}	//> LTH-040419-KO.

/**
 * @fn		CTeamInfo::GetLivePoint().
 * @brief	Ư�� ������ ���� �δ��� �ε� ����Ʈ ������.
 * @param	LPCHARLIST lpChar. Ư�� ����.
 * @return	POINT. �ε� ����Ʈ.
 */
//< LTH-040419-KO.
POINT CTeamInfo::GetLivePoint(LPCHARLIST lpChar)
{
	INT nI;
	POINT xyTemp = {0, 0};
	for (nI = 0; nI < m_nNumOfSquad; ++nI)
	{
		if (m_lpSquad[nI].FindMember(lpChar->GetServerID()))
		{
			xyTemp = m_lpSquad[nI].GetLivePoint();
			break;
		}
	}

	return xyTemp;
}	//> LTH-040419-KO.

/**
 * @fn		CTeamInfo::KickUser().
 * @brief	Ư�� �δ�� ����.
 * @param	INT nCn. ����
 * @return	BOOL. ������ ������ FALSE.
 */
//< LTH-040419-KO.
BOOL CTeamInfo::KickUser(INT nCn)
{
	LPCHARLIST lpChar = ::CheckServerId((WORD)nCn);
	if (NULL == lpChar)
		return FALSE;

	BOOL bIsUser = FALSE;
	INT nI;
	for (nI = 0; nI < m_nNumOfSquad; ++nI)
	{
		//< LTH-040322-KO ���ڷ� ���� ���� ���̵� ���ڳ� �����!! �Լ��� �Ǿ��� -_-;
		if (m_lpSquad[nI].FindMember(nCn))
		{
			m_lpSquad[nI].DeleteMember(nCn);
			bIsUser = TRUE;
			break;
		}
		//> LTH-040322-KO
	}

	return bIsUser;
}	//> LTH-040419-KO.

/**
 * @fn		CTeamInfo::GetNationDeathCount().
 * @brief	Ư�� ������ ���� ��� ���� ��ȯ.
 * @param	INT nNation. Ư�� ���� ��ȣ.
 * @return	INT. ���� ��� ��.
 */
//< LTH-040419-KO.
INT CTeamInfo::GetNationDeathCount(INT nNation)
{
	INT nNationDeathMember = 0;
	INT nI;
	for (nI = 0; nI < m_nNumOfSquad; ++nI)
	{
		nNationDeathMember += m_lpSquad[nI].GetNationDeathCount(nNation);
	}

	return nNationDeathMember;
}	//> LTH-040419-KO.

/**
 * @fn		CTeamInfo::GiveFame2Dual().
 * @brief	��� ���� �ش�.
 * @param	INT nAddFame. ���� ��ġ.
 * @return	VOID.
 */
//< LTH-040419-KO.
VOID CTeamInfo::GiveFame2Dual(INT nAddFame)
{
	INT nI;
	for (nI = 0; nI < m_nNumOfSquad; ++nI)
		m_lpSquad[nI].GiveFame2Dual(nAddFame);
}	//> LTH-040419-KO.

/**
 * @fn		CTeamInfo::GiveFame().
 * @brief	Ư�� ĳ���� �����ϰ� ���� �Ÿ��� ĳ������ ��ġ�� �ش�.
 * @param	LPCHARLIST lpCaster. ��ġ�� ���� ���� ĳ��.
 * @param	LPCHARLIST lpTarget. Ÿ��.
 * @param	INT nRange. ���� �Ÿ�.
 * @param	INT nAddFame. ���� ��ġ.
 * @return	VOID.
 */
//< LTH-040419-KO.
VOID CTeamInfo::GiveFame(LPCHARLIST lpCaster, LPCHARLIST lpTarget, INT nRange, INT nAddFame)
{
	INT nI;
	for (nI = 0; nI < m_nNumOfSquad; ++nI)
	{
		m_lpSquad[nI].GiveFame(lpCaster, lpTarget, nRange, nAddFame);
	}
}	//> LTH-040419-KO.

/**
 * @fn		CTeamInfo::Clear().
 * @brief	��� �δ�� ����.
 * @return	VOID.
 */
//< LTH-040419-KO.
VOID CTeamInfo::Clear()
{
	INT nI;
	for (nI = 0; nI < m_nNumOfSquad; ++nI)
		m_lpSquad[nI].DeleteAllMember();
}	//> LTH-040419-KO.

/**
 * @fn		CTeamInfo::RemoveArmor().
 * @brief	���� �����.
 * @return	VOID.
 */
//< LTH-040419-KO.
VOID CTeamInfo::RemoveArmor()
{
	INT nI;
	for (nI = 0; nI < m_nNumOfSquad; ++nI)
		m_lpSquad[nI].RemoveArmor();
}	//> LTH-040419-KO.