// RaidManager.cpp: implementation of the CRaidManager class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "RaidManager.h"

#include "RegenManager.h"
#include "UserManager.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CRaidManager::CRaidManager()
{
	m_bRunning = false;
}

CRaidManager::~CRaidManager()
{

}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CRaidManager::RecvMonsterRaidStart()
{
	if (IsRunning())
	{
		return;
	}
	else
	{
		m_bRunning = true;
	}

	g_pRegenManager->Remove(CGroupInfo::ET_NORMAL);
	MyLog(LOG_NORMAL, "<!-- Raid System Start -->");
	g_pUserManager->Message(MK_WARNING, 0, 409);
	g_pRegenManager->Ready(CGroupInfo::ET_RAID);
}

void CRaidManager::RecvMonsterRaidEnd()
{
	if (!IsRunning())
	{
		return;
	}
	else
	{
		m_bRunning = false;	
	}

	g_pRegenManager->Remove(CGroupInfo::ET_RAID);
	g_pUserManager->Message(MK_NORMAL, 0, 410);
	MyLog(LOG_NORMAL, "<!-- Raid System End -->");
	g_pRegenManager->Ready(CGroupInfo::ET_NORMAL);
}
