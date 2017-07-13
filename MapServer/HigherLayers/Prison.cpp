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
 * @brief	DB에서 직접 감옥 위치 정보를 얻어온다.
 * @return	bool. 불러오기에 성공하면 true.
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

		// 국가별 감옥의 개수를 읽어온다.
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

		// 국가별 감옥 정보를 읽어온다.
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

	int nDay = ::GetPrivateProfileInt("Prison", "Imprisonment", 3, MapServerConfigFileName);	// 감옥에 있는 기간 읽어오기.
	m_dwImprisonment = (DWORD)((86400 * nDay) / 4);

	return true;
}	//> LTH-040821-KO.

/**
 * @fn		CPrison::GetUserID().
 * @brief	chr_info table에서 캐릭터 이름으로 id를 찾아 반환한다.
 * @param	const char* szCharName. 캐릭터 이름.
 *			char* szUserID. 반환될 찾은 유저 ID.
 * @return	bool. DB접근에 Error가 없으면 true.
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

	//< LTH-040726-KO 불량 유저의 아이디를 찾아본다.
	SQLAllocStmt(g_hDBC_DragonDB, &hStmt);
	wsprintf(szQuery, "EXEC up_get_login_id '%s'", szCharName);

	retCode = SQLExecDirect(hStmt, (UCHAR*)szQuery, SQL_NTS);
	if(!SQL_SUCCEEDED(retCode))
	{
		SQLFreeStmt(hStmt,SQL_DROP);
		return false;
	}

	retCode = SQLFetch( hStmt );
	// 존재 하지 않는 캐릭터 이름 입니다.
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
 * @brief	chr_log_info table에서 캐릭터 이름으로 id를 찾아 반환한다.
 * @param	const char* szUserID. 유저 ID.
 *			int& nNation. 반환될 소속국가 값.
 * @return	bool. DB접근에 Error가 없으면 true.
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

	//< LTH-040727-KO 불량 유저의 아이디로 소속 국가를 찾아본다.
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
 * @brief	prisonerList table에서 User ID로 등록 되어 있는지를 찾아 반환한다.
 * @param	const char szUserID[20]. 찾을 유저 ID.
 * @return	bool. DB접근에 Error가 없거나 유저를 찾으면 true.
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
	// 유저 아이디가 없으면...
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
 * @brief	DB에 불량 유저를 등록한다.
 * @param	const char* szUserID. 유저 ID.
 *			const char* szCharName. 캐릭터 이름.
 *			const DWORD dwRemainTime. 감옥에 있어야 하는 시간.
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
 * @brief	감옥 시스템 처리에 대한 결과를 전송한다. (MS->C)
 * @param	const CPrison::ePRISON_SYS_RESULT ePrisonSysResult. 결과값 상수.
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
 * @brief	감옥 위치를 반환. 감옥 내에 타일 위치는 랜덤으로 한다.
 * @param	const int nNation. 국가코드
 *			char *szMapName. 맵이름 반환
 *			POINT &xyPosition. 위치값 반환
 * @return	void.
 */
//< LTH-040727-KO.
void CPrison::GetPrisonPosition(const int nNation, char *szMapName, POINT &xyPosition)
{
	int nRandomNumber = 0;

	if (m_naNumOfPrisonByNation[nNation] > 0)
	{
		// 감옥 내에 타일 중에 랜덤한 값을 반환
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
 * @brief	메인 마을 위치를 반환.
 * @param	const int nNation. 국가코드
 *			char *szMapName. 맵이름 반환
 *			POINT &xyPosition. 위치값 반환
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
 * @brief	유저의 시작 맵 위치를 인자로 받은 맵과 위치로 세팅한다.
 * @param	const char* szUserID. 유저 ID.
 *			const char *szMapName. 맵 이름.
 *			const POINT xyPosition. 좌표.
 * @return	bool. DB 접근에 실패하면 false.
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
	
	// LTH-040802-KO 타일 픽셀 값으로 변환
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
 * @brief	유저의 모든 캐릭터 이름을 불러온다 
 * @param	const char* szUserID. 유저 ID.
 *			const char *szCharName[4]. 캐릭터 이름 배열.
 * @return	void.
 */
//< LTH-040821-KO.
void CPrison::GetAllCharName(const char* szUserID, char szCharName[4][20])
{
	HSTMT	hStmt = NULL;
	RETCODE	retCode;
	SDWORD	cbValue;
	TCHAR	szQuery[256] = {0,};

	//< 해당 아이디의 모든 캐릭터 이름을 가져온다.
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
	// 캐릭이 하나도 없을 경우
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
 * @brief	유저를 감옥 맵으로 이동시킨다. 
 *			현재 접속 중이지 않은 유저는 시작맵의 위치를 감옥으로 바꾼다.
 * @param	const char* szUserID. 유저 ID.
 *			const char *szCharName. 캐릭터 이름.
 *			const int nNation. 국가 코드.
 * @return	void.
 */
//< LTH-040821-KO.
void CPrison::MoveToPrison(const char* szUserID, const int nNation)
{
	char szMapName[20] = {0,};
	POINT xyPosition;

	// 유저의 해당 국가의 감옥 위치를 얻어오고
	GetPrisonPosition(::GetNationIndex(nNation), szMapName, xyPosition);
	::EatRearWhiteChar(szMapName);

	// 시작 맵의 위치를 바꾼다.
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
			// 유저가 접속 중이라면
			if (NULL != lpChar)
			{
				// 바로 이동 시킨다.
				short nCn = lpChar->GetServerID();
				//< LTH-040824-KO 같은 맵일 경우 위치 이동만 시킨다.
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
 * @brief	client로 부터 불량유저를 감옥에 쳐 넣으라는 패킷을 받아 처리 (C->MS).
 *			CMD_BAD_USER_IMPRISON.
 * @param	t_packet *p. 패킷변수.
 * @return	void.
 */
//< LTH-040821-KO.
void CPrison::RecvBadUserImprison(t_packet* p, const int nCn)
{
	// 패킷의 유저 캐릭터 명을 가져오고
	char szCharName[20] = {0,};
	strcpy(szCharName, p->u.kein.tagBadUserInfo.szName);
	::EatRearWhiteChar(szCharName);
	
	// 캐릭터 명으로 유저 아이디를 가져오고
	char szUserID[20] = {0,};
	if (false == GetUserID(szCharName, szUserID))
		return;
	::EatRearWhiteChar(szUserID);

	// 불량 유저 테이블에 있는 유저인지 비교하고 없으면
	if (true == IsBadUser(szUserID))
	{
		SendPrisonSystemResult(nCn, PSR_ALREADY_REGISTERED_BAD_USER);
		return;
	}

	// 불량 유저 DB테이블에 넣는다.
	if (false == SetBadUser(szUserID, szCharName, m_dwImprisonment))
		return;

	// 해당 국가의 감옥 맵으로 이동 시킨다.
	int nNation = 0;
	if (false == GetUserNation(szCharName, nNation))
		return;

	LPCHARLIST lpChar = ::CheckServerName(szCharName);
	if (NULL == lpChar)
	{
		// 같은 맵에 없다면...
		// 접속 중이라면 맵서버 전체에 패킷을 날려 감옥에 쳐넣는다!!
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
 * @brief	DB에 불량 유저를 지운다.
 * @param	const char* szUserID. 유저 ID.
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
 * @brief	유저를 감옥에서 각자 메인 마을 맵으로 이동시킨다. 
 *			현재 접속 중이지 않은 유저는 시작맵의 위치를 메인마을로 바꾼다.
 * @param	const char* szUserID. 유저 ID.
 *			const char *szCharName. 캐릭터 이름.
 *			const int nNation. 국가 코드.
 * @return	void.
 */
//< LTH-040821-KO.
void CPrison::MoveToVillage(const char *szUserID, const int nNation)
{
	char szMapName[20] = {0,};
	POINT xyPosition;

	// 유저의 해당 국가의 메인 마을 위치를 얻어오고
	GetVillagePosition(::GetNationIndex(nNation), szMapName, xyPosition);
	::EatRearWhiteChar(szMapName);

	// 시작 맵의 위치를 바꾼다.
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
			// 유저가 접속 중이라면
			if (NULL != lpChar)
			{
				// 바로 이동 시킨다.
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
 * @brief	client로 부터 불량유저를 감옥에서 빼내라는 패킷을 받아 처리 (C->MS).
 *			CMD_BAD_USER_RELEASE.
 * @param	t_packet *p. 패킷변수.
 * @return	void.
 */
//< LTH-040821-KO.
void CPrison::RecvBadUserRelease(t_packet* p, const int nCn)
{
	// 패킷의 유저 캐릭터 명을 가져오고
	char szCharName[20] = {0,};
	strcpy(szCharName, p->u.kein.tagBadUserInfo.szName);
	::EatRearWhiteChar(szCharName);
	
	// 캐릭터 명으로 유저 아이디를 가져오고
	char szUserID[20] = {0,};
	if (false == GetUserID(szCharName, szUserID))
		return;
	::EatRearWhiteChar(szUserID);

	// 불량 유저 테이블에 있는 유저인지 비교하고 없으면
	if (false == IsBadUser(szUserID))
	{
		SendPrisonSystemResult(nCn, PSR_NOT_BAD_USER);
		return;
	}

	// 불량 유저 DB테이블에서 지운다.
	if (false == SetDeleteBadUser(szUserID))
		return;

	// 해당 국가의 메인 마을 맵으로 이동 시킨다.
	int nNation = 0;
	if (false == GetUserNation(szCharName, nNation))
		return;

	LPCHARLIST lpChar = ::CheckServerName(szCharName);
	if (NULL == lpChar)
	{
		// 같은 맵에 없다면...
		// 접속 중이라면 맵서버 전체에 패킷을 날려 메인 마을로 보내버린다!!
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
 * @brief	수감 기간을 DB에 저장한다.
 * @param	const char* szUserId. 유저 ID.
 * @param	const DWORD dwImprisonTerm. 저장할 수감 시간.
 * @return	true. DB 저장에 성공하면 true.
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
 * @brief	수감 기간을 DB에서 읽어온다.
 * @param	const char* szUserId. 유저 ID.
 * @return	DWORD. 수감기간.
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
 * @brief	client로 부터 감옥에 있는 시간을 갱신하라는 명령을 받음.
 *			CMD_UPDATE_IMPRISON_TERM.
 * @param	t_packet *p. 패킷변수.
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
	__int64 n64Temp = dwTerm - 180;		// 3분을 뺀다.

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
 * @brief	현재 맵서버가 감옥 맵서버인지 판별.
 * @return	bool. 이 맵서버가 감옥을 포함하고 있으면 true.
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
