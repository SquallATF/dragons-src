/**
 * @file	Prison.cpp.
 *			implementation of the CPrison class.
 */
#include "..\stdafx.h"
#include "UserManager.h"
#include "Prison.h"
#include "..\LowerLayers\servertable.h"

extern HDBC g_hDBC_DragonDB;
extern CServerTable* g_pServerTable;

extern int MapMove(int idUser, const char* pMapName, int nX, int nY);
extern int GetNationIndex(int Nation);
extern int GetNationFromIndex(int NationIndex);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrison::CPrison()
{
	if (false == LoadPrisonInfo())
	{
		::MyLog(LOG_FATAL,"CPrison::CPrison(), LoadPrisonInfo() Loading Error!!");
		return;
	}

	m_bIsPrisonMap = IsPrisonMap();
}

CPrison::~CPrison()
{
	int nI;
	for (nI = PN_VYSEUS; nI < PN_MAX; ++nI)
		SAFE_DELETE_ARRAY(m_ppPrisonInfo[nI]);
}

/**
 * @fn		CPrison::LoadPrisonInfo().
 * @brief	DB���� ���� ���� ��ġ ������ ���´�.
 * @return	bool. �ҷ����⿡ �����ϸ� true.
 */
//< LTH-040821-KO.
bool CPrison::LoadPrisonInfo()
{
	HSTMT	hStmt = NULL;
	RETCODE	retCode;
	SDWORD	cbValue;
	TCHAR	szQuery[256] = {0,};

	int nNation = 0;
	int nJ;
	for (nJ = PN_VYSEUS; nJ < PN_MAX; ++nJ)
	{
		nNation = ::GetNationFromIndex(nJ);

		// ������ ������ ������ �о�´�.
		SQLAllocStmt(g_hDBC_DragonDB, &hStmt);
		wsprintf(szQuery, "EXEC up_get_NumOfprison %d", nNation);

		retCode = SQLExecDirect(hStmt, (UCHAR*)szQuery, SQL_NTS);
		if(!SQL_SUCCEEDED(retCode))
		{
			::MyLog(LOG_FATAL,"CPrison::LoadPrisonInfo(), up_get_NumOfprison, prisonInfo Table Query Error!!");
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}

		retCode = SQLFetch( hStmt );
		if(!SQL_SUCCEEDED(retCode))
		{
			::MyLog(LOG_FATAL,"CPrison::LoadPrisonInfo(), up_get_NumOfprison, prisonInfo Table Fetch Error!!");
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}

		retCode = SQLGetData(hStmt, 1, SQL_INTEGER, &m_naNumOfPrisonByNation[nJ], sizeof(int), &cbValue);
		if(!SQL_SUCCEEDED(retCode))
		{
			::MyLog(LOG_FATAL,"CPrison::LoadPrisonInfo(), up_get_NumOfprison, prisonInfo Table SQL Return Error(%d)!!",retCode);
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}

		SQLFreeStmt(hStmt,SQL_DROP);

		// ������ ���� ������ �о�´�.
		m_ppPrisonInfo[nJ] = new tagPrisonInfo[m_naNumOfPrisonByNation[nJ]];

		SQLAllocStmt(g_hDBC_DragonDB, &hStmt);
		wsprintf(szQuery, "EXEC up_get_prison_info %d", nNation);

		retCode = SQLExecDirect(hStmt, (UCHAR*)szQuery, SQL_NTS);
		if(!SQL_SUCCEEDED(retCode))
		{
			::MyLog(LOG_FATAL,"CPrison::LoadPrisonInfo(), up_get_prison_info, prisonInfo Table Query Error!!");
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}

		retCode = SQLFetch( hStmt );
		if(!SQL_SUCCEEDED(retCode))
		{
			::MyLog(LOG_FATAL,"CPrison::LoadPrisonInfo(), up_get_prison_info, prisonInfo Table Fetch Error!!");
			SQLFreeStmt(hStmt,SQL_DROP);
			return false;
		}

		char szMapName[20] = {0,};
		int nX = 0;
		int nY = 0;
		int nI = 0;
		while(SQL_SUCCEEDED(retCode))
		{
			retCode = SQLGetData(hStmt, 1, SQL_C_CHAR, &szMapName, sizeof(szMapName), &cbValue);
			retCode = SQLGetData(hStmt, 2, SQL_C_LONG, &nX, sizeof(int), &cbValue);
			retCode = SQLGetData(hStmt, 3, SQL_C_LONG, &nY, sizeof(int), &cbValue);
			if(!SQL_SUCCEEDED(retCode))
			{
				::MyLog(LOG_FATAL,"CPrison::LoadPrisonInfo(), up_get_prison_info, prisonInfo Table SQL Return Error!!(%d)", retCode);
				SQLFreeStmt(hStmt,SQL_DROP);
				return false;
			}
			strcpy(m_ppPrisonInfo[nJ][nI].szMapName, szMapName);
			::EatRearWhiteChar(m_ppPrisonInfo[nJ][nI].szMapName);
			m_ppPrisonInfo[nJ][nI].xyPosition.x = nX;
			m_ppPrisonInfo[nJ][nI].xyPosition.y = nY;
			
			retCode = SQLFetch( hStmt );
			++nI;
			
			if (nI >= m_naNumOfPrisonByNation[nJ])
				break;
		}
	}

	SQLFreeStmt(hStmt, SQL_DROP);

	int nDay = ::GetPrivateProfileInt("Prison", "Imprisonment", 3, MapServerConfigFileName);	// ������ �ִ� �Ⱓ �о����.
	m_dwImprisonment = (DWORD)((86400 * nDay) / 4);

	return true;
}	//> LTH-040821-KO.

/**
 * @fn		CPrison::GetUserID().
 * @brief	chr_info table���� ĳ���� �̸����� id�� ã�� ��ȯ�Ѵ�.
 * @param	const char* szCharName. ĳ���� �̸�.
 *			char* szUserID. ��ȯ�� ã�� ���� ID.
 * @return	bool. DB���ٿ� Error�� ������ true.
 */
//< LTH-040821-KO.
bool CPrison::GetUserID(const char* szCharName, char* szUserID)
{
	if (NULL == szCharName)
		return false;

	HSTMT	hStmt = NULL;
	RETCODE	retCode;
	SDWORD	cbValue;
	TCHAR	szQuery[256] = {0,};

	//< LTH-040726-KO �ҷ� ������ ���̵� ã�ƺ���.
	SQLAllocStmt(g_hDBC_DragonDB, &hStmt);
	wsprintf(szQuery, "EXEC up_get_login_id '%s'", szCharName);

	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuery, SQL_NTS);
	if(!SQL_SUCCEEDED(retCode))
	{
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	retCode = SQLFetch( hStmt );
	// ���� ���� �ʴ� ĳ���� �̸� �Դϴ�.
	if (retCode == SQL_NO_DATA)
	{
		::MyLog(LOG_FATAL, "CPrison::GetUserID(), up_get_login_id, Not Exist Character Name!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	if(!SQL_SUCCEEDED(retCode))
	{
		::MyLog(LOG_FATAL, "CPrison::GetUserID(), up_get_login_id, chr_info Table SQL Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	char szTemp[20] = {0,};
	retCode = SQLGetData(hStmt, 1, SQL_C_CHAR, &szTemp, sizeof(szTemp), &cbValue);
	if(!SQL_SUCCEEDED(retCode))
	{
		::MyLog(LOG_FATAL,"CPrison::GetUserID(), up_get_login_id, chr_info Table SQL Return Error!!(%d)", retCode);
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	::EatRearWhiteChar(szTemp);
	strcpy(szUserID, szTemp);

	SQLFreeStmt(hStmt, SQL_DROP);
	//> LTH-040726-KO

	return true;
}	//> LTH-040821-KO.

/**
 * @fn		CPrison::GetUserNation().
 * @brief	chr_log_info table���� ĳ���� �̸����� id�� ã�� ��ȯ�Ѵ�.
 * @param	const char* szUserID. ���� ID.
 *			int& nNation. ��ȯ�� �Ҽӱ��� ��.
 * @return	bool. DB���ٿ� Error�� ������ true.
 */
//< LTH-040821-KO.
bool CPrison::GetUserNation(const char *szCharName, int& nNation)
{
	if (NULL == szCharName)
		return false;

	HSTMT	hStmt = NULL;
	RETCODE	retCode;
	SDWORD	cbValue;
	TCHAR	szQuery[256] = {0,};

	//< LTH-040727-KO �ҷ� ������ ���̵�� �Ҽ� ������ ã�ƺ���.
	SQLAllocStmt(g_hDBC_DragonDB, &hStmt);
	wsprintf(szQuery, "EXEC up_get_UserNation '%s'", szCharName);

	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuery, SQL_NTS);
	if(!SQL_SUCCEEDED(retCode))
	{
		::MyLog(LOG_FATAL,"CPrison::GetUserNation(), up_get_UserNation, chr_info Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	retCode = SQLFetch( hStmt );
	if(!SQL_SUCCEEDED(retCode))
	{
		::MyLog(LOG_FATAL,"CPrison::GetUserNation(), up_get_UserNation, chr_info Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	retCode = SQLGetData(hStmt, 1, SQL_C_ULONG, &nNation, sizeof(int), &cbValue);
	if(!SQL_SUCCEEDED(retCode))
	{
		::MyLog(LOG_FATAL,"CPrison::GetUserNation(), up_get_UserNation, chr_info Table SQL Return Error!!(%d)", retCode);
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	SQLFreeStmt(hStmt, SQL_DROP);
	//> LTH-040727-KO

	CCharRank temp_status;
	memcpy(&temp_status, &nNation, sizeof(DWORD));
	nNation = temp_status.nation;

	return true;
}	//> LTH-040821-KO.

/**
 * @fn		CPrison::IsBadUser().
 * @brief	prisonerList table���� User ID�� ��� �Ǿ� �ִ����� ã�� ��ȯ�Ѵ�.
 * @param	const char szUserID[20]. ã�� ���� ID.
 * @return	bool. DB���ٿ� Error�� ���ų� ������ ã���� true.
 */
//< LTH-040821-KO.
bool CPrison::IsBadUser(const char* szUserID)
{
	if (NULL == szUserID)
		return false;

	HSTMT	hStmt = NULL;
	RETCODE	retCode;
	SDWORD	cbValue;
	TCHAR	szQuery[256] = {0,};

	SQLAllocStmt(g_hDBC_DragonDB, &hStmt);
	wsprintf(szQuery, "EXEC up_get_BadUser '%s'", szUserID);

	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuery, SQL_NTS);
	if (!SQL_SUCCEEDED(retCode))
	{
		::MyLog(LOG_FATAL,"CPrison::IsBadUser, up_get_BadUser, prisonerList Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	retCode = SQLFetch( hStmt );
	// ���� ���̵� ������...
	if (retCode == SQL_NO_DATA)
	{
		::MyLog(LOG_FATAL, "User %s is not prisner!!", szUserID);
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}
	
	if (!SQL_SUCCEEDED(retCode))
	{
		::MyLog(LOG_FATAL,"CPrison::IsBadUser, up_get_BadUser, prisonerList Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	char szUserIDTemp[20] = {0,};
	retCode = SQLGetData(hStmt, 1, SQL_C_CHAR, &szUserIDTemp, sizeof(szUserIDTemp), &cbValue);
	if (!SQL_SUCCEEDED(retCode))
	{
		::MyLog(LOG_FATAL,"CPrison::IsBadUser, up_get_BadUser, prisonerList Table SQL Return Error!!(%d)", retCode);
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	SQLFreeStmt(hStmt, SQL_DROP);	

	return true;
}	//> LTH-040821-KO.

/**
 * @fn		CPrison::SetBadUser().
 * @brief	DB�� �ҷ� ������ ����Ѵ�.
 * @param	const char* szUserID. ���� ID.
 *			const char* szCharName. ĳ���� �̸�.
 *			const DWORD dwRemainTime. ������ �־�� �ϴ� �ð�.
 * @return	void.
 */
//< LTH-040821-KO.
bool CPrison::SetBadUser(const char* szUserID, const char* szCharName, const DWORD dwRemainTime)
{
	if ((NULL == szUserID) || (NULL == szCharName))
		return false;

	HSTMT	hStmt = NULL;
	RETCODE	retCode;
	TCHAR	szQuery[256] = {0,};

	wsprintf(szQuery, "EXEC up_set_BadUser '%s', '%s', %d", szUserID, szCharName, dwRemainTime);

	SQLAllocStmt(g_hDBC_DragonDB, &hStmt);
	
	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuery, SQL_NTS);
	if(!SQL_SUCCEEDED(retCode))
	{
		::MyLog(LOG_FATAL,"CPrison::SetBadUser(), PrisonerList Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	SQLFreeStmt(hStmt, SQL_DROP);

	::MyLog(LOG_FATAL, "ID : %s, Character Name : %s, Bad User Setting!!", szUserID, szCharName);

	return true;
}	//> LTH-040821-KO.

/**
 * @fn		CPrison::SendPrisonSystemResult().
 * @brief	���� �ý��� ó���� ���� ����� �����Ѵ�. (MS->C)
 * @param	const CPrison::ePRISON_SYS_RESULT ePrisonSysResult. ����� ���.
 * @return	void.
 */
//< LTH-040727-KO.
void CPrison::SendPrisonSystemResult(const int nCn, const CPrison::ePRISON_SYS_RESULT ePrisonSysResult)
{
	t_packet packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.h.header.type = CMD_PRISON_SYS_PROC_RESULT;
	packet.u.kein.tagPrisonSysProcResult.nReason = (int)(ePrisonSysResult);
	packet.h.header.size = sizeof(t_PrisonSysProcResult);
	QueuePacket(connections, nCn, &packet, 1);
}	//> LTH-040727-KO.

/**
 * @fn		CPrison::GetPrisonPosition().
 * @brief	���� ��ġ�� ��ȯ. ���� ���� Ÿ�� ��ġ�� �������� �Ѵ�.
 * @param	const int nNation. �����ڵ�
 *			char *szMapName. ���̸� ��ȯ
 *			POINT &xyPosition. ��ġ�� ��ȯ
 * @return	void.
 */
//< LTH-040727-KO.
void CPrison::GetPrisonPosition(const int nNation, char *szMapName, POINT &xyPosition)
{
	int nRandomNumber = 0;

	if (m_naNumOfPrisonByNation[nNation] > 0)
	{
		// ���� ���� Ÿ�� �߿� ������ ���� ��ȯ
		srand( (unsigned)time( NULL ) );
		nRandomNumber = rand() % m_naNumOfPrisonByNation[nNation];
	}
	else
		nRandomNumber = 0;
	strcpy(szMapName, m_ppPrisonInfo[nNation][nRandomNumber].szMapName);
	xyPosition = m_ppPrisonInfo[nNation][nRandomNumber].xyPosition;
}	//> LTH-040727-KO.

/**
 * @fn		CPrison::GetVillagePosition().
 * @brief	���� ���� ��ġ�� ��ȯ.
 * @param	const int nNation. �����ڵ�
 *			char *szMapName. ���̸� ��ȯ
 *			POINT &xyPosition. ��ġ�� ��ȯ
 * @return	void.
 */
//< LTH-040805-KO.
void CPrison::GetVillagePosition(const int nNation, char *szMapName, POINT &xyPosition)
{
	switch (nNation)
	{
	case PN_VYSEUS:
		{
			strcpy(szMapName, "MA-IN");
			xyPosition.x = 245;
			xyPosition.y = 115;
		}
		break;
	case PN_ZYPERN: 
		{
			strcpy(szMapName, "RENES_C");
			xyPosition.x = 210;
			xyPosition.y = 175;
		}
		break;
	case PN_YILSE: 
		{
			strcpy(szMapName, "BARANTAN");
			xyPosition.x = 325;
			xyPosition.y = 98;
		}
		break;
	}

}	//> LTH-040805-KO.

/**
 * @fn		CPrison::SetUserStartingMap().
 * @brief	������ ���� �� ��ġ�� ���ڷ� ���� �ʰ� ��ġ�� �����Ѵ�.
 * @param	const char* szUserID. ���� ID.
 *			const char *szMapName. �� �̸�.
 *			const POINT xyPosition. ��ǥ.
 * @return	bool. DB ���ٿ� �����ϸ� false.
 */
//< LTH-040821-KO.
bool CPrison::SetUserStartingMap(const char *szUserID, const char *szMapName, const POINT xyPosition)
{
	if ((NULL == szUserID) || (NULL == szMapName))
		return false;

	char szTempCharName[4][20] = { {0,},};
	GetAllCharName(szUserID, szTempCharName);

	HSTMT	hStmt = NULL;
	RETCODE	retCode;
	TCHAR	szQuery[256] = {0,};
	
	// LTH-040802-KO Ÿ�� �ȼ� ������ ��ȯ
	int nI;
	int nX = (xyPosition.x * TILE_SIZE) + (TILE_SIZE / 2);
	int nY = (xyPosition.y * TILE_SIZE) + (TILE_SIZE / 2);
	for (nI = 0; nI < 4; ++nI)
	{
		::EatRearWhiteChar(szTempCharName[nI]);
		if (strlen(szTempCharName[nI]) != 0)
		{
			wsprintf(szQuery, "EXEC up_set_UserStatingMap '%s', '%s', %d, %d", szTempCharName[nI], szMapName, nX, nY);

			SQLAllocStmt(g_hDBC_DragonDB, &hStmt);
			
			retCode = SQLExecDirect(hStmt, (UCHAR*)szQuery, SQL_NTS);
			if(!SQL_SUCCEEDED(retCode))
			{
				SQLFreeStmt(hStmt,SQL_DROP);
				return false;
			}

			SQLFreeStmt(hStmt, SQL_DROP);

			::MyLog(LOG_FATAL, "Set prisoner ID %s, Character %s, Map : %s (%d, %d)", szUserID, szTempCharName[nI], szMapName, nX, nY);
		}
	}

	return true;
}	//> LTH-040821-KO

/**
 * @fn		CPrison::GetAllCharName().
 * @brief	������ ��� ĳ���� �̸��� �ҷ��´� 
 * @param	const char* szUserID. ���� ID.
 *			const char *szCharName[4]. ĳ���� �̸� �迭.
 * @return	void.
 */
//< LTH-040821-KO.
void CPrison::GetAllCharName(const char* szUserID, char szCharName[4][20])
{
	HSTMT	hStmt = NULL;
	RETCODE	retCode;
	SDWORD	cbValue;
	TCHAR	szQuery[256] = {0,};

	//< �ش� ���̵��� ��� ĳ���� �̸��� �����´�.
	SQLAllocStmt(g_hDBC_DragonDB, &hStmt);
	wsprintf(szQuery, "EXEC up_get_char_all_name '%s'", szUserID);

	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuery, SQL_NTS);
	if(!SQL_SUCCEEDED(retCode))
	{
		::MyLog(LOG_FATAL,"CPrison::GetAllCharName, up_get_char_all_name, chr_info Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return;
	}

	retCode = SQLFetch( hStmt );
	// ĳ���� �ϳ��� ���� ���
	if (retCode == SQL_NO_DATA)
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return;
	}

	if(!SQL_SUCCEEDED(retCode))
	{
		::MyLog(LOG_FATAL,"CPrison::GetAllCharName, up_get_char_all_name, chr_info Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return;
	}

	int nI;
	for (nI = 0; nI < 4; ++nI)
	{
		retCode = SQLGetData(hStmt, nI + 1, SQL_C_CHAR, &szCharName[nI], 20, &cbValue);
		if(!SQL_SUCCEEDED(retCode))
		{
			::MyLog(LOG_FATAL,"CPrison::GetAllCharName, up_get_char_all_name, chr_info Table Get Error!!");
			SQLFreeStmt(hStmt,SQL_DROP);
			return;
		}
		::EatRearWhiteChar(szCharName[nI]);
		if (strlen(szCharName[nI]) != 0)
		{
			::MyLog(LOG_FATAL, "Get %s's Character slot %d : %s", szUserID, nI + 1, szCharName[nI]);
		}
		else
		{
			ZeroMemory(&szCharName[nI], sizeof(szCharName[nI]));
		}
	}

	SQLFreeStmt(hStmt, SQL_DROP);
}	//> LTH-040821-KO.

extern void MovePc(short int cn, int x, int y);		// LTH-040824-KO

/**
 * @fn		CPrison::MoveToPrison().
 * @brief	������ ���� ������ �̵���Ų��. 
 *			���� ���� ������ ���� ������ ���۸��� ��ġ�� �������� �ٲ۴�.
 * @param	const char* szUserID. ���� ID.
 *			const char *szCharName. ĳ���� �̸�.
 *			const int nNation. ���� �ڵ�.
 * @return	void.
 */
//< LTH-040821-KO.
void CPrison::MoveToPrison(const char* szUserID, const int nNation)
{
	char szMapName[20] = {0,};
	POINT xyPosition;

	// ������ �ش� ������ ���� ��ġ�� ������
	GetPrisonPosition(::GetNationIndex(nNation), szMapName, xyPosition);
	::EatRearWhiteChar(szMapName);

	// ���� ���� ��ġ�� �ٲ۴�.
	SetUserStartingMap(szUserID, szMapName, xyPosition);

	char szCharName[4][20] = { {0,},};
	GetAllCharName(szUserID, szCharName);

	bool bIsMove = false;
	LPCHARLIST lpChar = NULL;
	int nI;
	for (nI = 0; nI < 4; ++nI)
	{
		::EatRearWhiteChar(szCharName[nI]);

		if (strlen(szCharName[nI]) != 0)
		{
			lpChar = ::CheckServerName(szCharName[nI]);
			// ������ ���� ���̶��
			if (NULL != lpChar)
			{
				// �ٷ� �̵� ��Ų��.
				short nCn = lpChar->GetServerID();
				//< LTH-040824-KO ���� ���� ��� ��ġ �̵��� ��Ų��.
				if (true != m_bIsPrisonMap)
				{
					::MapMove(nCn, szMapName, xyPosition.x, xyPosition.y);
				}
				else
				{
					::MovePc(nCn, xyPosition.x, xyPosition.y);
				}
				//< LTH-040824-KO
				SendPrisonSystemResult(nCn, CPrison::PSR_MOVE_TO_PRISON);

				::MyLog(LOG_FATAL, "%s Move to prison!! Character : %s", szUserID, szCharName[nI]);
				bIsMove = true;
				break;
			}
		}
	}

	if (false == bIsMove)
	{
		::MyLog(LOG_FATAL, "Current Log Out! or Not Exist current Map!! Prisoner %s, Character : %s", szUserID, szCharName[nI]);
	}
}	//> LTH-040821-KO.

/**
 * @fn		CPrison::RecvBadUserImprison().
 * @brief	client�� ���� �ҷ������� ������ �� ������� ��Ŷ�� �޾� ó�� (C->MS).
 *			CMD_BAD_USER_IMPRISON.
 * @param	t_packet *p. ��Ŷ����.
 * @return	void.
 */
//< LTH-040821-KO.
void CPrison::RecvBadUserImprison(t_packet* p, const int nCn)
{
	// ��Ŷ�� ���� ĳ���� ���� ��������
	char szCharName[20] = {0,};
	strcpy(szCharName, p->u.kein.tagBadUserInfo.szName);
	::EatRearWhiteChar(szCharName);
	
	// ĳ���� ������ ���� ���̵� ��������
	char szUserID[20] = {0,};
	if (false == GetUserID(szCharName, szUserID))
		return;
	::EatRearWhiteChar(szUserID);

	// �ҷ� ���� ���̺� �ִ� �������� ���ϰ� ������
	if (true == IsBadUser(szUserID))
	{
		SendPrisonSystemResult(nCn, PSR_ALREADY_REGISTERED_BAD_USER);
		return;
	}

	// �ҷ� ���� DB���̺� �ִ´�.
	if (false == SetBadUser(szUserID, szCharName, m_dwImprisonment))
		return;

	// �ش� ������ ���� ������ �̵� ��Ų��.
	int nNation = 0;
	if (false == GetUserNation(szCharName, nNation))
		return;

	LPCHARLIST lpChar = ::CheckServerName(szCharName);
	if (NULL == lpChar)
	{
		// ���� �ʿ� ���ٸ�...
		// ���� ���̶�� �ʼ��� ��ü�� ��Ŷ�� ���� ������ �ĳִ´�!!
		t_packet packet;
		ZeroMemory(&packet, sizeof(packet));
		packet.h.header.type = CMD_REQUEST_MOVE_TO_PRISON;
		strcpy(packet.u.kein.tagRequestMoveToPrison.szUserId, szUserID);
		::EatRearWhiteChar(packet.u.kein.tagRequestMoveToPrison.szUserId);
		packet.u.kein.tagRequestMoveToPrison.nNation = nNation;
		packet.h.header.size = sizeof(t_RequestMoveToPrison);
		g_pServerTable->BroadCastToEveryServer((char*)&packet, sizeof(t_header) + packet.h.header.size, SERVER_TYPE_MAP);
		return;
	}

	MoveToPrison(szUserID, nNation);
}	//>LTH-040821-KO.

/**
 * @fn		CPrison::SetDeleteBadUser().
 * @brief	DB�� �ҷ� ������ �����.
 * @param	const char* szUserID. ���� ID.
 * @return	void.
 */
//< LTH-040821-KO.
bool CPrison::SetDeleteBadUser(const char *szUserID)
{
	if (NULL == szUserID)
		return false;

	HSTMT	hStmt = NULL;
	RETCODE	retCode;
	TCHAR	szQuery[256] = {0,};

	wsprintf(szQuery, "EXEC up_DeleteBadUserID '%s'", szUserID);

	SQLAllocStmt(g_hDBC_DragonDB, &hStmt);
	
	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuery, SQL_NTS);
	if(!SQL_SUCCEEDED(retCode))
	{
		::MyLog(LOG_FATAL,"CPrison::SetDeleteBadUser(), PrisonerList Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	SQLFreeStmt(hStmt, SQL_DROP);

	::MyLog(LOG_FATAL, "ID : %s, Bad User Release Setting!!", szUserID);

	return true;
}	//> LTH-040821-KO.

/**
 * @fn		CPrison::MoveToVillage().
 * @brief	������ �������� ���� ���� ���� ������ �̵���Ų��. 
 *			���� ���� ������ ���� ������ ���۸��� ��ġ�� ���θ����� �ٲ۴�.
 * @param	const char* szUserID. ���� ID.
 *			const char *szCharName. ĳ���� �̸�.
 *			const int nNation. ���� �ڵ�.
 * @return	void.
 */
//< LTH-040821-KO.
void CPrison::MoveToVillage(const char *szUserID, const int nNation)
{
	char szMapName[20] = {0,};
	POINT xyPosition;

	// ������ �ش� ������ ���� ���� ��ġ�� ������
	GetVillagePosition(::GetNationIndex(nNation), szMapName, xyPosition);
	::EatRearWhiteChar(szMapName);

	// ���� ���� ��ġ�� �ٲ۴�.
	SetUserStartingMap(szUserID, szMapName, xyPosition);

	char szCharName[4][20] = { {0,},};
	GetAllCharName(szUserID, szCharName);

	bool bIsMove = false;
	LPCHARLIST lpChar = NULL;
	int nI;
	for (nI = 0; nI < 4; ++nI)
	{
		::EatRearWhiteChar(szCharName[nI]);

		if (strlen(szCharName[nI]) != 0)
		{
			lpChar = ::CheckServerName(szCharName[nI]);
			// ������ ���� ���̶��
			if (NULL != lpChar)
			{
				// �ٷ� �̵� ��Ų��.
				short nCn = lpChar->GetServerID();
				::MapMove(nCn, szMapName, xyPosition.x, xyPosition.y);
				SendPrisonSystemResult(nCn, CPrison::PSR_MOVE_TO_VILLAGE);

				::MyLog(LOG_FATAL, "%s Move to village from prison!! Character : %s", szUserID, szCharName[nI]);
				bIsMove = true;
				break;
			}
		}
	}

	if (false == bIsMove)
	{
		::MyLog(LOG_FATAL, "Current Log Out! or Not Exist current Map!! Prisoner %s, Character : %s", szUserID, szCharName[nI]);
	}
}	//> LTH-040821-KO

/**
 * @fn		CPrison::RecvBadUserRelease().
 * @brief	client�� ���� �ҷ������� �������� ������� ��Ŷ�� �޾� ó�� (C->MS).
 *			CMD_BAD_USER_RELEASE.
 * @param	t_packet *p. ��Ŷ����.
 * @return	void.
 */
//< LTH-040821-KO.
void CPrison::RecvBadUserRelease(t_packet* p, const int nCn)
{
	// ��Ŷ�� ���� ĳ���� ���� ��������
	char szCharName[20] = {0,};
	strcpy(szCharName, p->u.kein.tagBadUserInfo.szName);
	::EatRearWhiteChar(szCharName);
	
	// ĳ���� ������ ���� ���̵� ��������
	char szUserID[20] = {0,};
	if (false == GetUserID(szCharName, szUserID))
		return;
	::EatRearWhiteChar(szUserID);

	// �ҷ� ���� ���̺� �ִ� �������� ���ϰ� ������
	if (false == IsBadUser(szUserID))
	{
		SendPrisonSystemResult(nCn, PSR_NOT_BAD_USER);
		return;
	}

	// �ҷ� ���� DB���̺��� �����.
	if (false == SetDeleteBadUser(szUserID))
		return;

	// �ش� ������ ���� ���� ������ �̵� ��Ų��.
	int nNation = 0;
	if (false == GetUserNation(szCharName, nNation))
		return;

	LPCHARLIST lpChar = ::CheckServerName(szCharName);
	if (NULL == lpChar)
	{
		// ���� �ʿ� ���ٸ�...
		// ���� ���̶�� �ʼ��� ��ü�� ��Ŷ�� ���� ���� ������ ����������!!
		t_packet packet;
		ZeroMemory(&packet, sizeof(packet));
		packet.h.header.type = CMD_REQUEST_MOVE_TO_VILLAGE;
		strcpy(packet.u.kein.tagRequestMoveToPrison.szUserId, szUserID);
		::EatRearWhiteChar(packet.u.kein.tagRequestMoveToPrison.szUserId);
		packet.u.kein.tagRequestMoveToPrison.nNation = nNation;
		packet.h.header.size = sizeof(t_RequestMoveToPrison);
		g_pServerTable->BroadCastToEveryServer((char*)&packet, sizeof(t_header) + packet.h.header.size, SERVER_TYPE_MAP);
		return;
	}

	MoveToVillage(szUserID, nNation);
}	//> LTH-040821-KO

/**
 * @fn		CPrison::SetImprisonTerm().
 * @brief	���� �Ⱓ�� DB�� �����Ѵ�.
 * @param	const char* szUserId. ���� ID.
 * @param	const DWORD dwImprisonTerm. ������ ���� �ð�.
 * @return	true. DB ���忡 �����ϸ� true.
 */
//< LTH-040821-KO.
bool CPrison::SetImprisonTerm(const char *szUserId, const DWORD dwImprisonTerm)
{
	if (NULL == szUserId)
		return false;

	HSTMT	hStmt = NULL;
	RETCODE	retCode;
	TCHAR	szQuery[256] = {0,};

	wsprintf(szQuery, "EXEC up_set_Update_Imprison_Term '%s', %d", szUserId, dwImprisonTerm);

	SQLAllocStmt(g_hDBC_DragonDB, &hStmt);
	
	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuery, SQL_NTS);
	if(!SQL_SUCCEEDED(retCode))
	{
		::MyLog(LOG_FATAL,"CPrison::SetImprisonTerm(), PrisonerList Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	SQLFreeStmt(hStmt, SQL_DROP);

	return true;
}	//> LTH-040821-KO.

/**
 * @fn		CPrison::GetImprisonTerm().
 * @brief	���� �Ⱓ�� DB���� �о�´�.
 * @param	const char* szUserId. ���� ID.
 * @return	DWORD. �����Ⱓ.
 */
//< LTH-040821-KO.
DWORD CPrison::GetImprisonTerm(const char *szUserId)
{
	if (NULL == szUserId)
		return 0;

	HSTMT	hStmt = NULL;
	RETCODE	retCode;
	SDWORD	cbValue;
	TCHAR	szQuery[256] = {0,};

	SQLAllocStmt(g_hDBC_DragonDB, &hStmt);
	wsprintf(szQuery, "EXEC up_get_ImprisonTerm '%s'", szUserId);

	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuery, SQL_NTS);
	if(!SQL_SUCCEEDED(retCode))
	{
		::MyLog(LOG_FATAL,"CPrison::GetImprisonTerm(), up_get_ImprisonTerm, prisonerList Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return 0;
	}

	retCode = SQLFetch( hStmt );
	if(!SQL_SUCCEEDED(retCode))
	{
		::MyLog(LOG_FATAL,"CPrison::GetImprisonTerm(), up_get_ImprisonTerm, prisonerList Table Fetch Error!! or Already exist!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return 0;
	}

	DWORD dwImprisonTerm = 0;
	retCode = SQLGetData(hStmt, 1, SQL_C_LONG, &dwImprisonTerm, sizeof(DWORD), &cbValue);
	if(!SQL_SUCCEEDED(retCode))
	{
		::MyLog(LOG_FATAL,"CPrison::GetImprisonTerm(), up_get_ImprisonTerm, prisonerList Table SQL Return Error!!(%d)", retCode);
		SQLFreeStmt(hStmt,SQL_DROP);
		return 0;
	}

	SQLFreeStmt(hStmt, SQL_DROP);

	return dwImprisonTerm;
}	//> LTH-040821-KO.

/**
 * @fn		CPrison::RecvUpdateImprisonTerm().
 * @brief	client�� ���� ������ �ִ� �ð��� �����϶�� ����� ����.
 *			CMD_UPDATE_IMPRISON_TERM.
 * @param	t_packet *p. ��Ŷ����.
 * @return	void.
 */
//< LTH-040810-KO.
void CPrison::RecvUpdateImprisonTerm(const short int cn)
{
	LPCHARLIST lpChar = ::CheckServerId((WORD)cn);
	if (NULL == lpChar)
		return;

	if (false == IsBadUser(connections[cn].id))
		return;

	DWORD dwTerm = GetImprisonTerm(connections[cn].id);
	__int64 n64Temp = dwTerm - 180;		// 3���� ����.

	if (n64Temp <= 0)
	{
		t_packet packet;
		strcpy(packet.u.kein.tagBadUserInfo.szName, connections[cn].name);
		::EatRearWhiteChar(packet.u.kein.tagBadUserInfo.szName);
		RecvBadUserRelease(&packet, cn);
	}
	else
	{
		SetImprisonTerm(connections[cn].id, n64Temp);
	}

	static int nCount = 0;

	if (nCount > 3)
	{
		::MyLog(LOG_NORMAL, "Prisoner %s Update ImprisonTerm %d", connections[cn].name, n64Temp);
		nCount = 0;
	}
	else
	{
		++nCount;
	}
}	//> LTH-040810-KO.

extern WORD g_wMapServerPort;					// LTH-040824-KO
extern int GetMapPortByName(char* map_name);	// LTH-040824-KO

/**
 * @fn		CPrison::IsPrisonMap().
 * @brief	���� �ʼ����� ���� �ʼ������� �Ǻ�.
 * @return	bool. �� �ʼ����� ������ �����ϰ� ������ true.
 */
//< LTH-040824-KO.
bool CPrison::IsPrisonMap()
{
	WORD wMapServerport = 0;
	int nI, nJ;
	for (nJ = 0; nJ < PN_MAX; ++nJ)
	{
		for (nI = 0; nI < m_naNumOfPrisonByNation[nJ]; ++nI)
		{
			wMapServerport = (WORD)(::GetMapPortByName(m_ppPrisonInfo[nJ][nI].szMapName));
			if (g_wMapServerPort == wMapServerport)
			{
				::MyLog(LOG_FATAL, "** This Map is prison Map!! **");
				return true;
			}
		}
	}

	return false;
}	//> LTH-040824-KO.
