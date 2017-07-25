// GameSystem.h: interface for the CGameSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMESYSTEM_H__569DF8D0_3814_4DA0_A71C_472AC4C33A94__INCLUDED_)
#define AFX_GAMESYSTEM_H__569DF8D0_3814_4DA0_A71C_472AC4C33A94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDefine.h"


///////////////////////////////////////////////////////////////////////////////
// 게임 관련 Manager 생성 관리 클래스
class CGameSystem : public IGameSystem
{
public:
	CGameSystem(IGameFactory* pFactory);
	virtual ~CGameSystem();

public:
	virtual void CreateManager(eGAME_SYSTEM _GameSystem);

	virtual CLogManager* GetLogManager();
	virtual CUserManager* GetUserManager();
	virtual CDualManager* GetDualManager();
	virtual CGuildManager* GetGuildManager();
	virtual CAIManager* GetAIManager();
	virtual CAdventManager* GetAdventManager();
	virtual CArenaManager* GetArenaManager();
	virtual CRegenManager* GetRegenManager();
	virtual CBattleManager* GetBattleManager();
	virtual CRaidManager* GetRaidManager();
	virtual CItemMallManager* GetItemMallManager();

protected:
	IGameFactory*   m_pFactory;
	const char* ConvertToMgrType(eGAME_SYSTEM _GameSystem);

private:
	CLogManager* m_pLogManager;
	CUserManager* m_pUserManager;
	CDualManager* m_pDualManager;
	CGuildManager* m_pGuildManager;
	CAIManager* m_pAIManager;
	CAdventManager* m_pAdventManager;
	CArenaManager* m_pArenaManager;
	CRegenManager* m_pRegenManager;
	CBattleManager* m_pBattleManager;
	CRaidManager* m_pRaidManager;
	CItemMallManager* m_pItemMallManager;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 전역 참조자
#define g_pLogManager CLogManager::Pointer()
#define g_pUserManager CUserManager::Pointer()
#define g_pDualManager CDualManager::Pointer()
#define g_pGuildManager CGuildManager::Pointer()
#define g_pAIManager CAIManager::Pointer()
#define g_pAdventManager CAdventManager::Pointer()
#define g_pArenaManager CArenaManager::Pointer()
#define g_pRegenManager CRegenManager::Pointer()
#define g_pBattleManager CBattleManager::Pointer()
#define g_pRaidManager CRaidManager::Pointer()
#define g_pItemMallManager CItemMallManager::Pointer()
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_GAMESYSTEM_H__569DF8D0_3814_4DA0_A71C_472AC4C33A94__INCLUDED_)
