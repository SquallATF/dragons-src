// GameFactory.cpp: implementation of the CGameFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameFactory.h"

#include "HigherLayers/LogManager.h"
#include "HigherLayers/UserManager.h"
#include "HigherLayers/DualManager.h"
#include "HigherLayers/GuildManager.h"
#include "HigherLayers/AIManager.h"
#include "HigherLayers/AdventManager.h"
#include "HigherLayers/ArenaManager.h"
#include "HigherLayers/RegenManager.h"
#include "HigherLayers/BattleManager.h"
#include "HigherLayers/RaidManager.h"
#include "HigherLayers/ItemMallManager.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CGameFactory::CGameFactory()
{

}

CGameFactory::~CGameFactory()
{

}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

CLogManager* CGameFactory::CreateLogManager()
{
	return new CLogManager();
}

void CGameFactory::DestroyLogManager(CLogManager* pManager)
{
	SAFE_DELETE(pManager);
}

CUserManager* CGameFactory::CreateUserManager()
{
	return new CUserManager();
}

void CGameFactory::DestroyUserManager(CUserManager* pManager)
{
	SAFE_DELETE(pManager);
}

CDualManager* CGameFactory::CreateDualManager()
{
	return new CDualManager();
}

void CGameFactory::DestroyDualManager(CDualManager* pManager)
{
	SAFE_DELETE(pManager);
}

CGuildManager* CGameFactory::CreateGuildManager()
{
	return new CGuildManager();
}

void CGameFactory::DestroyGuildManager(CGuildManager* pManager)
{
	SAFE_DELETE(pManager);
}

CAIManager* CGameFactory::CreateAIManager()
{
	return new CAIManager();
}

void CGameFactory::DestroyAIManager(CAIManager* pManager)
{
	SAFE_DELETE(pManager);
}

CAdventManager* CGameFactory::CreateAdventManager()
{
	return new CAdventManager();
}

void CGameFactory::DestroyAdventManager(CAdventManager* pManager)
{
	SAFE_DELETE(pManager);
}

CArenaManager* CGameFactory::CreateArenaManager()
{
	return new CArenaManager();
}

void CGameFactory::DestroyArenaManager(CArenaManager* pManager)
{
	SAFE_DELETE(pManager);
}

CRegenManager* CGameFactory::CreateRegenManager()
{
	return new CRegenManager();
}

void CGameFactory::DestroyRegenManager(CRegenManager* pManager)
{
	SAFE_DELETE(pManager);
}

CBattleManager* CGameFactory::CreateBattleManager()
{
	return new CBattleManager();
}

void CGameFactory::DestroyBattleManager(CBattleManager* pManager)
{
	SAFE_DELETE(pManager);
}

CRaidManager* CGameFactory::CreateRaidManager()
{
	return new CRaidManager();
}

void CGameFactory::DestroyRaidManager(CRaidManager* pManager)
{
	SAFE_DELETE(pManager);
}

CItemMallManager* CGameFactory::CreateItemMallManager()
{
	return new CItemMallManager();
}

void CGameFactory::DestroyItemMallManager(CItemMallManager* pManager)
{
	SAFE_DELETE(pManager);
}
