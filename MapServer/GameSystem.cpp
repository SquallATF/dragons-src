// GameSystem.cpp: implementation of the CGameSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameSystem.h"

//莖샙
#define BEGIN_SELECT(e) switch (e) {
#define CASE_SELECT(e)  case(e): return CS(e);
#define END_SELECT(e)   } return CS(e);


///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CGameSystem::CGameSystem(IGameFactory* pFactory)
{
	m_pFactory = NULL;
	m_pLogManager = NULL;
	m_pUserManager = NULL;
	m_pDualManager = NULL;
	m_pGuildManager = NULL;
	m_pAIManager = NULL;
	m_pAdventManager = NULL;
	m_pArenaManager = NULL;
	m_pRegenManager = NULL;
	m_pBattleManager = NULL;
	m_pRaidManager = NULL;
	m_pItemMallManager	= NULL;

	Dbg_Assert(pFactory != NULL);
	m_pFactory = pFactory;
}

CGameSystem::~CGameSystem()
{
	m_pFactory->DestroyLogManager(m_pLogManager);
	m_pFactory->DestroyUserManager(m_pUserManager);
	m_pFactory->DestroyDualManager(m_pDualManager);
	m_pFactory->DestroyGuildManager(m_pGuildManager);
	m_pFactory->DestroyAIManager(m_pAIManager);
	m_pFactory->DestroyAdventManager(m_pAdventManager);
	m_pFactory->DestroyArenaManager(m_pArenaManager);
	m_pFactory->DestroyRegenManager(m_pRegenManager);
	m_pFactory->DestroyBattleManager(m_pBattleManager);
	m_pFactory->DestroyRaidManager(m_pRaidManager);
	m_pFactory->DestroyItemMallManager(m_pItemMallManager);
	
	m_pFactory = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CGameSystem::CreateManager(eGAME_SYSTEM _eGameSystem)
{
	switch (_eGameSystem)
	{
	case LogManager:
		m_pLogManager = m_pFactory->CreateLogManager();
		Dbg_Assert(m_pLogManager != NULL);
		break;
	case UserManager:
		m_pUserManager = m_pFactory->CreateUserManager();
		Dbg_Assert(m_pUserManager != NULL);
		break;
	case DualManager:
		m_pDualManager = m_pFactory->CreateDualManager();
		Dbg_Assert(m_pDualManager != NULL);
		break;
	case GuildManager:
		m_pGuildManager = m_pFactory->CreateGuildManager();
		Dbg_Assert(m_pGuildManager != NULL);
		break;
	case AIManager:
		m_pAIManager = m_pFactory->CreateAIManager();
		Dbg_Assert(m_pAIManager != NULL);
		break;
	case AdventManager:
		m_pAdventManager = m_pFactory->CreateAdventManager();
		Dbg_Assert(m_pAdventManager != NULL);
		break;
	case ArenaManager:
		m_pArenaManager = m_pFactory->CreateArenaManager();
		Dbg_Assert(m_pArenaManager != NULL);
		break;
	case RegenManager:
		m_pRegenManager = m_pFactory->CreateRegenManager();
		Dbg_Assert(m_pRegenManager != NULL);
		break;
	case BattleManager:
		m_pBattleManager = m_pFactory->CreateBattleManager();
		Dbg_Assert(m_pBattleManager != NULL);
		break;
	case RaidManager:
		m_pRaidManager = m_pFactory->CreateRaidManager();
		Dbg_Assert(m_pRaidManager != NULL);
		break;
	case ItemMallManager:
		m_pItemMallManager = m_pFactory->CreateItemMallManager();
		Dbg_Assert(m_pItemMallManager != NULL);
		break;
	default:
		MyLog(LOG_IMPORTANT,"CGameSystem::CreateManager 에서 해당하는 Type이 없습니다.[%s]",ConvertToMgrType(_eGameSystem));
		break;
	}
}

const char* CGameSystem::ConvertToMgrType(eGAME_SYSTEM _GameSystem)
{
	BEGIN_SELECT(_GameSystem)
		CASE_SELECT(LogManager)
		CASE_SELECT(UserManager)
		CASE_SELECT(DualManager)
		CASE_SELECT(GuildManager)
		CASE_SELECT(AIManager)
		CASE_SELECT(AdventManager)
		CASE_SELECT(ArenaManager)
		CASE_SELECT(RegenManager)
		CASE_SELECT(BattleManager)
		CASE_SELECT(RaidManager)
		CASE_SELECT(ItemMallManager)
	END_SELECT(UNKNOWN_Manager)
}

CLogManager* CGameSystem::GetLogManager()
{
	Dbg_Assert(m_pLogManager != NULL);
	return m_pLogManager;
}

CUserManager* CGameSystem::GetUserManager()
{
	Dbg_Assert(m_pUserManager != NULL);
	return m_pUserManager;
}

CDualManager* CGameSystem::GetDualManager()
{
	Dbg_Assert(m_pDualManager != NULL);
	return m_pDualManager;
}

CGuildManager* CGameSystem::GetGuildManager()
{
	Dbg_Assert(m_pGuildManager != NULL);
	return m_pGuildManager;
}

CAIManager* CGameSystem::GetAIManager()
{
	Dbg_Assert(m_pAIManager != NULL);
	return m_pAIManager;
}

CAdventManager* CGameSystem::GetAdventManager()
{
	Dbg_Assert(m_pAdventManager != NULL);
	return m_pAdventManager;
}

CArenaManager* CGameSystem::GetArenaManager()
{
	Dbg_Assert(m_pArenaManager != NULL);
	return m_pArenaManager;
}
	
CRegenManager* CGameSystem::GetRegenManager()
{
	Dbg_Assert(m_pRegenManager != NULL);
	return m_pRegenManager;
}

CBattleManager* CGameSystem::GetBattleManager()
{
	Dbg_Assert(m_pBattleManager != NULL);
	return m_pBattleManager;
}

CRaidManager* CGameSystem::GetRaidManager()
{
	Dbg_Assert(m_pRaidManager != NULL);
	return m_pRaidManager;
}

CItemMallManager* CGameSystem::GetItemMallManager()
{
	Dbg_Assert(m_pItemMallManager != NULL);
	return m_pItemMallManager;
}