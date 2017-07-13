/**
 * @file	WarfieldInfo.cpp.
 *			implementation of the CWarfieldInfo class.
 */
#include "..\stdafx.h"
#include "LogManager.h"
#include "WarfieldInfo.h"

/**
 * @fn		CWarfieldInfo::LoadSoldierInfo().
 * @brief	전쟁터 부대정보를 MapServerConfig.ini 파일에서 읽어와 세팅.
 * @param	INT nWarfieldNo. 전쟁터 번호.
 * @return	VOID.
 */
//< LTH-040127-KO.
VOID CWarfieldInfo::LoadSoldierInfo(INT nWarfieldNo)
{
	TCHAR szTemp[MAX_PATH];
	sprintf(szTemp, "WarfieldNo%dSoldierInfo", nWarfieldNo);
	m_acSoldierSet[nWarfieldNo].m_tagCommander.nLevelMin = ::GetPrivateProfileInt(szTemp, "CommanderLevelMin", \
		70, MapServerConfigFileName);
	m_acSoldierSet[nWarfieldNo].m_tagCommander.nLadderMin = ::GetPrivateProfileInt(szTemp, "CommanderLadderMin", \
		2000, MapServerConfigFileName);
	m_acSoldierSet[nWarfieldNo].m_tagCommander.nFameMin = ::GetPrivateProfileInt(szTemp, "CommanderFameMin", \
		1200, MapServerConfigFileName);

	INT nI;
	INT nIEnd = m_acSoldierSet[nWarfieldNo].m_nNumOfSquad;
	TCHAR szTemp2[MAX_PATH];
	for (nI = 0; nI < nIEnd; ++nI)
	{
		sprintf(szTemp2, "Squad%dLeaderLevelMin", nI + 1);
		m_acSoldierSet[nWarfieldNo].m_tagaSquad[nI].nLeaderLevelMin = ::GetPrivateProfileInt(szTemp, szTemp2, 0, MapServerConfigFileName);
		sprintf(szTemp2, "Squad%dLeaderLadderMin", nI + 1);
		m_acSoldierSet[nWarfieldNo].m_tagaSquad[nI].nLeaderLadderMin = ::GetPrivateProfileInt(szTemp, szTemp2, 0, MapServerConfigFileName);
		sprintf(szTemp2, "Squad%dLevelMin", nI + 1);
		m_acSoldierSet[nWarfieldNo].m_tagaSquad[nI].nLevelMin = ::GetPrivateProfileInt(szTemp, szTemp2, 0, MapServerConfigFileName);
		sprintf(szTemp2, "Squad%dLevelMax", nI + 1);
		m_acSoldierSet[nWarfieldNo].m_tagaSquad[nI].nLevelMax = ::GetPrivateProfileInt(szTemp, szTemp2, 0, MapServerConfigFileName);
		sprintf(szTemp2, "Squad%dMinJoin", nI + 1);
		m_acSoldierSet[nWarfieldNo].m_tagaSquad[nI].nPersonMin = ::GetPrivateProfileInt(szTemp, szTemp2, 0, MapServerConfigFileName);
		sprintf(szTemp2, "Squad%dMaxJoin", nI + 1);
		m_acSoldierSet[nWarfieldNo].m_tagaSquad[nI].nPersonMax = ::GetPrivateProfileInt(szTemp, szTemp2, 0, MapServerConfigFileName);
		sprintf(szTemp2, "Squad%dStartingPointX", nI + 1);
		m_acSoldierSet[nWarfieldNo].m_tagaSquad[nI].xyStartingPoint.x = ::GetPrivateProfileInt(szTemp, szTemp2, 0, MapServerConfigFileName);
		sprintf(szTemp2, "Squad%dStartingPointY", nI + 1);
		m_acSoldierSet[nWarfieldNo].m_tagaSquad[nI].xyStartingPoint.y = ::GetPrivateProfileInt(szTemp, szTemp2, 0, MapServerConfigFileName);
	}
}	//> LTH-040127-KO.

/**
 * @fn		CWarfieldInfo::LoadMapMoveInfo().
 * @brief	전쟁터 이동정보 MapServerConfig.ini 파일에서 읽어와 세팅.
 * @param	INT nWarfieldNo. 전쟁터 번호.
 * @return	VOID.
 */
//< LTH-040127-KO.
VOID CWarfieldInfo::LoadMapMoveInfo(INT nWarfieldNo)
{
	TCHAR szTemp[MAX_PATH];
	sprintf(szTemp, "WarfieldNo%dMapMove", nWarfieldNo);
	m_acMapSetting[nWarfieldNo].m_nMoveLevelMin = ::GetPrivateProfileInt(szTemp, "LevelMin", 0, MapServerConfigFileName);
	m_acMapSetting[nWarfieldNo].m_nMoveLevelMax = ::GetPrivateProfileInt(szTemp, "LevelMax", 0, MapServerConfigFileName);
	m_acMapSetting[nWarfieldNo].m_nUserEntranceLimitTime = ::GetPrivateProfileInt(szTemp, "UserEntranceLimitTime", 1800, MapServerConfigFileName);	// LTH-040510-KO 유저 전쟁터 이동 제한 시간 읽어오기
}	//> LTH-040127-KO.

extern HDBC g_hDBC_DragonDB;

/**
 * @fn		CWarfieldInfo::LoadWarTimeInfo().
 * @brief	전쟁 시간정보 MapServerConfig.ini 파일에서 읽어와 세팅.
 * @param	INT nWarfieldNo. 전쟁터 번호.
 * @return	VOID.
 */
//< LTH-040127-KO.
VOID CWarfieldInfo::LoadWarTimeInfo(INT nWarfieldNo)
{
	HSTMT	hStmt=NULL;
	RETCODE	ret;
	char	query_stmt[80] = {0,};
	SDWORD	cbValue;

	//< LTH-040414-KO 일주일 간 해당 type의 전쟁이 일어나는 횟수 읽어오기.
	SQLAllocStmt(g_hDBC_DragonDB, &hStmt);

	wsprintf(query_stmt,"select count(*) as NumDayHour from WarStartupTBL where Type=3");   

	ret = SQLExecDirect(hStmt, (UCHAR*)query_stmt, SQL_NTS);
 
	if (ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS)
	{
		::MyLog(LOG_FATAL, "CWarfieldInfo::LoadWarTimeInfo(), WarField Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return;
	}

	ret = SQLFetch(hStmt);
	if (ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS)
	{
		::MyLog(LOG_FATAL,"CWarfieldInfo::LoadWarTimeInfo(), WarField Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return;
	}

	ret = SQLGetData(hStmt, 1, SQL_C_SLONG, &m_btHowManyTimesWeek, sizeof(INT), &cbValue);
	if (ret != SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
	{
		::MyLog(LOG_FATAL,"CWarfieldInfo::LoadWarTimeInfo(), Warfield Table SQL Return Error(%d)!!",ret);
		SQLFreeStmt(hStmt,SQL_DROP);
		return;
	}

	SQLFreeStmt(hStmt,SQL_DROP);
	//> LTH-040414-KO.

	m_atagWarTimeInfo[nWarfieldNo] = new CWarTimeInfo[m_btHowManyTimesWeek];

	//< LTH-040419-KO 일주일간 일어나는 전쟁의 시간을 전쟁터에 따라 로딩.
	SQLAllocStmt(g_hDBC_DragonDB, &hStmt);

	wsprintf(query_stmt,"select * from WarStartupTBL where Type=3 ORDER BY DayofWeek, DHour");   

	ret = SQLExecDirect(hStmt, (UCHAR*)query_stmt, SQL_NTS);
 
	if (ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS)
	{
		::MyLog(LOG_FATAL, "CWarfieldInfo::LoadWarTimeInfo(), WarField Table Query Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return;
	}

	ret = SQLFetch(hStmt);
	if (ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS)
	{
		::MyLog(LOG_FATAL,"CWarfieldInfo::LoadWarTimeInfo(), WarField Table Fetch Error!!");
		SQLFreeStmt(hStmt,SQL_DROP);
		return;
	}

	::MyLog(LOG_NORMAL, "<!-- HADES WARFIELD WAR Startup Information -->");

	INT nIndex = 0, index = 0;
	TCHAR szTemp[MAX_PATH];
	sprintf(szTemp, "WarfieldNo%dLoopTimeInfo", nWarfieldNo);
	
	DWORD dwPrepareTime = 0;
	DWORD dwWarTime = (DWORD)::GetPrivateProfileInt(szTemp, "WarTime", 0, MapServerConfigFileName);
	DWORD dwSecretAreaOpenTime = (DWORD)::GetPrivateProfileInt(szTemp, "SecurityAreaOpenTime", 0, MapServerConfigFileName);

	while (ret == SQL_SUCCESS)
	{
		ret=SQLGetData(hStmt, 1, SQL_C_SLONG, &nIndex, 0, &cbValue);
		ret=SQLGetData(hStmt, 3, SQL_C_SLONG, &m_atagWarTimeInfo[nWarfieldNo][index].m_btStartDay, sizeof(INT), &cbValue);
		ret=SQLGetData(hStmt, 4, SQL_C_SLONG, &m_atagWarTimeInfo[nWarfieldNo][index].m_btStartHour, sizeof(INT), &cbValue);
		if (ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_SUCCESS)
		{
			::MyLog(0,"CWarfieldInfo::LoadWarTimeInfo(), Warfield Table SQL Return Error(%d)!!",ret);
			SQLFreeStmt(hStmt,SQL_DROP);
			return;
		}

		if (m_atagWarTimeInfo[nWarfieldNo][index].m_btStartDay < 0 || \
			m_atagWarTimeInfo[nWarfieldNo][index].m_btStartDay > 6)
		{
			::JustMsg("Failed Day Setting (CWarfieldInfo::LoadWarTimeInfo() DB)");
			return;
		}

		if (m_atagWarTimeInfo[nWarfieldNo][index].m_btStartHour < 0 || \
			m_atagWarTimeInfo[nWarfieldNo][index].m_btStartHour > 23)
		{
			::JustMsg("Failed Time Setting (CWarfieldInfo::LoadWarTimeInfo() DB)");
			return;
		}

		m_atagWarTimeInfo[nWarfieldNo][index].m_dwPrepareTime = dwPrepareTime;
		m_atagWarTimeInfo[nWarfieldNo][index].m_dwWarTime = dwWarTime;
		m_atagWarTimeInfo[nWarfieldNo][index].m_dwSecretAreaOpenTime = dwSecretAreaOpenTime;

		//< LTH-040312-KO 전쟁 끝나는 시간 계산
		DWORD dwWarEndTime = (m_atagWarTimeInfo[nWarfieldNo][index].m_btStartDay * 24 + \
			m_atagWarTimeInfo[nWarfieldNo][index].m_btStartHour + (dwWarTime / 3600));
		//> LTH-040312-KO

		::MyLog(LOG_NORMAL, "[%d]%s(%02d:%02d - %02d:%02d)", 
			  nIndex,
			  (CLogManager::Pointer())->ConvertToDay(m_atagWarTimeInfo[nWarfieldNo][index].m_btStartDay),
			  m_atagWarTimeInfo[nWarfieldNo][index].m_btStartHour, 
			  0, 
			  dwWarEndTime % 24, 
			  0);

		++index;
		ret = SQLFetch(hStmt);
	}

    SQLFreeStmt(hStmt,SQL_DROP);
	//> LTH-040419-KO.
}	//> LTH-040127-KO.

/**
 * @fn		CWarfieldInfo::Init().
 * @brief	전쟁터 초기정보를 MapServerConfig.ini 파일에서 읽어와 세팅.
 * @param	INT nNumOfWarfield. 최대 전쟁터맵의 개수.
 * @return	VOID.
 */
//< LTH-040127-KO.
VOID CWarfieldInfo::Init(INT nNumOfWarfield)
{
	::GetPrivateProfileString("network", "path", "", NetworkDir, MAX_PATH, MAP_SERVER_INI_);
	sprintf(MapServerConfigFileName,"%s/data/MapServerConfig.ini",NetworkDir);

	m_nNumOfWarfield = nNumOfWarfield;

	INT nI;
	for (nI = 0; nI < m_nNumOfWarfield; ++nI)
	{
		switch(nI)
		{
		case WI_HADES_WARFIELD:
			{
				m_acSoldierSet[nI].m_nNumOfSquad = MAX_SQUAD;
				LoadSoldierInfo(nI);
				LoadMapMoveInfo(nI);
			}
			break;
		}
	}
	
	LoadYlseWarfieldMoveMoney();	// LTH-040504-KO 일스의 평화기간 전쟁터 이동 요금 읽어오기
	LoadUserGoOutsideInfo();		// LTH-040514-KO 유저 전쟁터 방출에 관한 정보 로딩
}
//> LTH-040127-KO.

/**
 * @fn		CWarfieldInfo::Release().
 * @brief	전쟁터 정보를 안전하게 제거.
 * @return	VOID.
 */
//< LTH-040127-KO.
VOID CWarfieldInfo::Release()
{
	INT nI;
	for (nI = 0; nI > m_nNumOfWarfield; ++nI)
	{
		SAFE_DELETE_ARRAY(m_atagWarTimeInfo[nI]);
	}
} //> LTH-040127-KO.

/**
 * @fn		CWarfieldInfo::GetThisWarIndex().
 * @brief	현재 진행되는 전쟁의 index가 몬지 알아 보는 함수.
 * @param	INT nWarfieldNo. 전쟁터번호.
 * @param	DWORD dwCurrTime. 현재 시간.
 * @return	INT. 현재 진행되는 전쟁의 index.
 */
//< LTH-040419-KO.
INT CWarfieldInfo::GetThisWarIndex(INT nWarfieldNo, DWORD dwCurrTime)
{
	DWORD dwStartUpTime1 = 0;
	DWORD dwStartUpTime2 = 0;
	INT nI;
	if (m_btHowManyTimesWeek != 1)
	{
		for (nI = 0; nI < m_btHowManyTimesWeek - 1; ++nI)
		{
			dwStartUpTime1 = (m_atagWarTimeInfo[nWarfieldNo][nI].m_btStartDay * 86400) + \
				(m_atagWarTimeInfo[nWarfieldNo][nI].m_btStartHour * 3600);
			dwStartUpTime2 = (m_atagWarTimeInfo[nWarfieldNo][nI + 1].m_btStartDay * 86400) + \
				(m_atagWarTimeInfo[nWarfieldNo][nI + 1].m_btStartHour * 3600);

			if ((dwStartUpTime1 <= dwCurrTime) && (dwStartUpTime2 > dwCurrTime))
				break;
		}
	}
	else
		nI = 0;

	return nI;
} //> LTH-040419-KO.

/**
 * @fn		CWarfieldInfo::LoadYlseWarfieldMoveMoney().
 * @brief	일스국가 유저의 평화 기간 전쟁터 이동 요금 읽어오기.
 * @return	void.
 */
//< LTH-040429-KO.
void CWarfieldInfo::LoadYlseWarfieldMoveMoney()
{
	m_dwYlWarfieldMoveMoney = ::GetPrivateProfileInt("WarfieldMapMove", "YlseWarfieldMoveMoney", 5000000, MapServerConfigFileName);
}	//> LTH-040429-KO.

/**
 * @fn		CWarfieldInfo::LoadUserGoOutsideInfo().
 * @brief	전쟁터에서의 유저 방출 정보 읽어오기.
 * @return	void.
 */
//< LTH-040507-KO.
void CWarfieldInfo::LoadUserGoOutsideInfo()
{
	m_nGoOutsideBBSTime = ::GetPrivateProfileInt("WarfieldMapMove", "UserGoOutsideBBSTime", 60, MapServerConfigFileName);
	m_nNumOfGoOutsideUser = ::GetPrivateProfileInt("WarfieldMapMove", "UserGoOutsideOneTimePersonnel", 20, MapServerConfigFileName);
}	//> LTH-040507-KO.