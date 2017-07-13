/**
 * @file	HWState.cpp.
 */
#include "..\stdafx.h"
#include "HWState.h"
#include "WarfieldInfo.h"
#include "LogManager.h"

extern void UpdateWarTime();
extern void SendCMD_UPDATE_WARTIME(const int WarfieldNo, const int Status, const DWORD RemainTime);

extern LPWARFIELDINFO g_pcWarfieldInfo;
extern DWORD g_dwCurrWeekElapsedSec;

//--------------------------------------------
CHWPeaceState::CHWPeaceState()
{
	m_dwTerm = 0;
	m_n64RemainTime = 0;
	m_dwLoopTime = 0;
}

/**
 * @fn		CHWPeaceState::Init().
 * @brief	초기화.
 * @param	INT nWarfieldNo. 전쟁터 번호.
 * @return	STDMETHODIMP_(VOID).
 */
//< LTH-040419-KO.
STDMETHODIMP_(VOID) CHWPeaceState::Init(INT nWarfieldNo)
{
	m_nWarfieldNo = nWarfieldNo;
	::UpdateWarTime();
	INT nIndex = g_pcWarfieldInfo->GetThisWarIndex(m_nWarfieldNo, g_dwCurrWeekElapsedSec);
	CWarTimeInfo cWarTime = g_pcWarfieldInfo->GetWarTimeInfo(m_nWarfieldNo, nIndex);
	m_dwTerm = 0;
	m_dwLoopTime = g_dwCurrWeekElapsedSec + m_dwTerm;
	m_n64RemainTime = m_dwLoopTime - g_dwCurrWeekElapsedSec;
	::SendCMD_UPDATE_WARTIME(m_nWarfieldNo, NW_PEACE, GetRemainTime());

	//< LTH-040323-KO 로그 강화
	g_pLogManager->SaveLogNeoNationWar(NNT_STATE_INFO, "Peace State!! Active!! RemainTime : %ld", m_n64RemainTime);
	//> LTH-040323-KO
}	//> LTH-040419-KO.

/**
 * @fn		CHWPeaceState::SetRemainTime().
 * @brief	남은 시간 설정.
 * @param	__int64 dwRemainTime. 설정 할 남은 시간.
 * @return	STDMETHODIMP_(VOID).
 */
//< LTH-040419-KO.
STDMETHODIMP_(VOID) CHWPeaceState::SetRemainTime(__int64 dwRemainTime)
{
	m_n64RemainTime = dwRemainTime;
}	//> LTH-040419-KO.

/**
 * @fn		CHWPeaceState::GetRemainTime().
 * @brief	남은시간 반환.
 * @return	STDMETHODIMP_(__int64).
 */
//< LTH-040419-KO.
STDMETHODIMP_(__int64) CHWPeaceState::GetRemainTime()
{
	::UpdateWarTime();
	//< LTH-040419-KO 현재 시간이 현재 상태 종료 시간을 지났다면 인위적으로 0을 세팅해 끝내도록 한다.
	if (m_dwLoopTime < g_dwCurrWeekElapsedSec)
		m_dwLoopTime = 0;
	//> LTH-040419-KO.

	return m_dwLoopTime - g_dwCurrWeekElapsedSec;
}	//> LTH-040419-KO.

/**
 * @fn		CHWPeaceState::SetLoopTime().
 * @brief	인위적으로 종료시간을 세팅한다.
 * @param	DWORD dwLoopTime. 현재 상태 종료시간.
 * @return	STDMETHODIMP_(VOID).
 */
//< LTH-040419-KO.
STDMETHODIMP_(VOID) CHWPeaceState::SetLoopTime(DWORD dwLoopTime)
{
	//< LTH-040310-KO
	::UpdateWarTime();
	m_dwLoopTime = g_dwCurrWeekElapsedSec + dwLoopTime;
	//> LTH-040310-KO
}	//> LTH-040419-KO.

/**
 * @fn		CHWPeaceState::GetLoopTime().
 * @brief	현재 상태의 종료시간을 반환한다.
 * @return	STDMETHODIMP_(DWORD).
 */
//< LTH-040419-KO.
STDMETHODIMP_(DWORD) CHWPeaceState::GetLoopTime()
{
	return m_dwLoopTime;
}	//> LTH-040419-KO.

/**
 * @fn		CHWPeaceState::Update().
 * @brief	현재 남은 시간을 다시 계산하며 현재 상태에 해야할일을 업데이트 한다 남은시간이 마이너스이면 현재상태를 종료한다.
 * @return	STDMETHODIMP.
 */
//< LTH-040419-KO.
STDMETHODIMP CHWPeaceState::Update()
{
	return S_OK;
}	//> LTH-040419-KO.

//----- 이하 상태 class의 멤버변수는 위와 동일한 기능이다. -----
CHWWarState::CHWWarState()
{
	m_dwTerm = 0;
	m_n64RemainTime = 0;
	m_dwLoopTime = 0;
}

STDMETHODIMP_(VOID) CHWWarState::Init(INT nWarfieldNo)
{
	m_nWarfieldNo = nWarfieldNo;
	::UpdateWarTime();
	INT nIndex = g_pcWarfieldInfo->GetThisWarIndex(m_nWarfieldNo, g_dwCurrWeekElapsedSec);
	CWarTimeInfo cWarTime = g_pcWarfieldInfo->GetWarTimeInfo(m_nWarfieldNo, nIndex);
	m_dwTerm = cWarTime.m_dwWarTime;
	m_dwLoopTime = g_dwCurrWeekElapsedSec + m_dwTerm;
	m_n64RemainTime = m_dwLoopTime - g_dwCurrWeekElapsedSec;
	::SendCMD_UPDATE_WARTIME(m_nWarfieldNo, NW_WAR, GetRemainTime());

	//< LTH-040323-KO 로그 강화
	g_pLogManager->SaveLogNeoNationWar(NNT_STATE_INFO, "War State!! Active!! RemainTime : %ld", m_n64RemainTime);
	//> LTH-040323-KO
}

STDMETHODIMP_(VOID) CHWWarState::SetRemainTime(__int64 dwRemainTime)
{
	m_n64RemainTime = dwRemainTime;
}

STDMETHODIMP_(__int64) CHWWarState::GetRemainTime()
{
	::UpdateWarTime();
	if (m_dwLoopTime < g_dwCurrWeekElapsedSec)
		m_dwLoopTime = 0;

	return m_dwLoopTime - g_dwCurrWeekElapsedSec;
}

STDMETHODIMP_(VOID) CHWWarState::SetLoopTime(DWORD dwLoopTime)
{
	//< LTH-040310-KO
	::UpdateWarTime();
	m_dwLoopTime = g_dwCurrWeekElapsedSec + dwLoopTime;
	//> LTH-040310-KO
}

STDMETHODIMP_(DWORD) CHWWarState::GetLoopTime()
{
	return m_dwLoopTime;
}

STDMETHODIMP CHWWarState::Update()
{
	::UpdateWarTime();
	m_n64RemainTime = m_dwLoopTime - g_dwCurrWeekElapsedSec;

	if (m_n64RemainTime <= 0)
		return E_FAIL;

	if (m_n64RemainTime % NW_REMAIN_TIME_UPDATE == 0) 
		::SendCMD_UPDATE_WARTIME(m_nWarfieldNo, NW_WAR, GetRemainTime());

	//< LTH-040322-KO 10분간격으로 남은 시간을 로그로 남기자구!!
	if ((m_n64RemainTime % 600) == 0)
		g_pLogManager->SaveLogNeoNationWar(NNT_TIME_INFO, "[Current RemainTime] = LoopTime(%ld) - CurrTime(%ld) = %ld", \
		m_dwLoopTime, g_dwCurrWeekElapsedSec, m_n64RemainTime);
	//> LTH-040322-KO

	return S_OK;
}

//--------------------------------------------
CHWSOpenState::CHWSOpenState()
{
	m_dwTerm = 0;
	m_n64RemainTime = 0;
	m_dwLoopTime = 0;
}

STDMETHODIMP_(VOID) CHWSOpenState::Init(INT nWarfieldNo)
{
	m_nWarfieldNo = nWarfieldNo;
	::UpdateWarTime();
	INT nIndex = g_pcWarfieldInfo->GetThisWarIndex(m_nWarfieldNo, g_dwCurrWeekElapsedSec);
	CWarTimeInfo cWarTime = g_pcWarfieldInfo->GetWarTimeInfo(m_nWarfieldNo, nIndex);
	m_dwTerm = cWarTime.m_dwSecretAreaOpenTime;
	m_dwLoopTime = g_dwCurrWeekElapsedSec + m_dwTerm;
	m_n64RemainTime = m_dwLoopTime - g_dwCurrWeekElapsedSec;
	::SendCMD_UPDATE_WARTIME(m_nWarfieldNo, NW_SOPEN, GetRemainTime());

	//< LTH-040323-KO 로그 강화
	g_pLogManager->SaveLogNeoNationWar(NNT_STATE_INFO, "Secret Area Open State!! Active!! RemainTime : %ld", \
		m_n64RemainTime);
	//> LTH-040323-KO
}

STDMETHODIMP_(VOID) CHWSOpenState::SetRemainTime(__int64 dwRemainTime)
{
	m_n64RemainTime = dwRemainTime;
}

STDMETHODIMP_(__int64) CHWSOpenState::GetRemainTime()
{
	::UpdateWarTime();
	if (m_dwLoopTime < g_dwCurrWeekElapsedSec)
		m_dwLoopTime = 0;

	return m_dwLoopTime - g_dwCurrWeekElapsedSec;
}

STDMETHODIMP_(VOID) CHWSOpenState::SetLoopTime(DWORD dwLoopTime)
{
	//< LTH-040310-KO
	::UpdateWarTime();
	m_dwLoopTime = g_dwCurrWeekElapsedSec + dwLoopTime;
	//> LTH-040310-KO
}

STDMETHODIMP_(DWORD) CHWSOpenState::GetLoopTime()
{
	return m_dwLoopTime;
}

STDMETHODIMP CHWSOpenState::Update()
{
	::UpdateWarTime();
	m_n64RemainTime = m_dwLoopTime - g_dwCurrWeekElapsedSec;

	if (m_n64RemainTime <= 0)
		return E_FAIL;
	
	if (m_n64RemainTime % NW_REMAIN_TIME_UPDATE == 0) 
		::SendCMD_UPDATE_WARTIME(m_nWarfieldNo, NW_SOPEN, GetRemainTime());

	//< LTH-040322-KO 10분간격으로 남은 시간을 로그로 남기자구!!
	if ((m_n64RemainTime % 600) == 0)
		g_pLogManager->SaveLogNeoNationWar(NNT_TIME_INFO, "[Current RemainTime] = LoopTime(%ld) - CurrTime(%ld) = %ld", \
		m_dwLoopTime, g_dwCurrWeekElapsedSec, m_n64RemainTime);
	//> LTH-040322-KO

	return S_OK;
}