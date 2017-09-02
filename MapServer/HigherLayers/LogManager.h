// LogManager.h: interface for the CLogManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMANAGER_H__564A7676_EFA9_408A_ABA6_9EBB37D1786E__INCLUDED_)
#define AFX_LOGMANAGER_H__564A7676_EFA9_408A_ABA6_9EBB37D1786E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Scrp_exe.h"		//040719_KJHuNs eLDF_TYPE(DualFame에서 사용).
#include "LTSSupport.h"		//040719_KJHuNs eLF_TYPE(ChangeFame에서 사용).

///////////////////////////////////////////////////////////////////////////////
// 로그 관리 위한 관리 클래스
class CLogManager : public TSingleton<CLogManager>
{
	typedef map<int, string>     HASH_LOG;
	typedef HASH_LOG::iterator   ITOR_LOG;
	typedef HASH_LOG::value_type PAIR_LOG;

public:
	CLogManager();
	~CLogManager();

public:
	void CreateMainPath(const char* pPath);
	void CreateSubPath(int nType, const char* pDir);

	//< 040715_KJHuNs Root폴더
	void SaveLogRoot_DeadEventNpc(CHARLIST* pTarget);
	void SaveLogRoot_GambleLogFile(CHARLIST *ch, int house, int class_type,
		int user_count, ItemAttr *item, int resource_type, int lv_band);
	void SaveLogRoot_PK(LPTSTR lpMsg);
	//> 040715_KJHuNs Root폴더

	// 040715_KJHuNs Accelater폴더
	void SaveLogAccelater(CHARLIST* pTarget, const char* pType, const int cn);

	// 040715_KJHuNs AutionLog폴더
	void SaveLogAuction(int type, t_SellerItemDelete * pRecordInfo);	// BBD 040226 옥션로그

	// 040715_KJHuNs Battle폴더
	void SaveLogBattle_Ballancing(LPCHARLIST a, LPCHARLIST d, int magicno, int damage,
		int mana, int attack, int attacktype, int tac);

	//< 040715_KJHuNs Change폴더
	void SaveLogChange_BankMoney(const int idTarget, const DWORD nOldBankMoney,
		const DWORD nNewBankMoney, const eBankMoneyChangeType BMCT);
	void SaveLogChange_BattingMoney(CHARLIST* pTarget, DWORD dwOldMoney, DWORD dwNewMoney);
	void SaveLogChange_ClassQuest(CHARLIST* pTarget, const char* pMsg, int nQuestNo,
		int nQcount, int nStart);
	void SaveLogChange_ClassStep(CHARLIST* pTarget, int nPrevStep, int nNextStep);
	void SaveLogChange_DividendMoney(CHARLIST* pTarget, DWORD dwOldMoney,
		DWORD dwNewMoney, bool bBank);
	void SaveLogChange_DualFame(CHARLIST* pTarget, const int nOldDualFame,
		const int nNewDualFame, eLDF_TYPE type);
	void SaveLogChange_Fame(CHARLIST* pTarget, int old_fame, int new_fame,
		eLF_TYPE type);
	void SaveLogChange_NationItem(CHARLIST* pTarget, int type, ItemAttr* item);
	void SaveLogChange_Ladder(CHARLIST* pTarget, int old_ladder, int new_ladder, int type);
	void SaveLogChange_ObtainCombat(CHARLIST* pTarget, int nOld, int nNew);
	void SaveLogChange_Combat(CHARLIST* pTarget, int nCombat, int nOld, int nNew);
	void SaveLogChange_LoginLogoutByKein(CHARLIST* pTarget, int type, int call_function_id);
	//> 040715_KJHuNs Change폴더

	//< 040721_KJHuNs Check폴더
	void SaveLogCheck_StrikeAttack(CHARLIST* pCaster, CHARLIST* pTarget);
	void SaveLogCheck_ThrowAttack(CHARLIST* pCaster, CHARLIST* pTarget);
	void SaveLogCheck_MagicExecute(BYTE nMagic, CHARLIST* pCaster, CHARLIST* pTarget, int nX, int nY);
	void SaveLogCheck_CombatExecute(BYTE nCombat, CHARLIST* pCaster, CHARLIST* pTarget, int nX, int nY);
	//> 040721_KJHuNs Check폴더	

	// 040721_KJHuNs EventItem폴더
	void SaveLogEventItem_Lost(int type, char * szName, int nIndex);		// BBD 040308 이벤트 아이템 로스트 로그

	// 040721_KJHuNs LevelUp폴더
	void SaveLogLevelUp(LPCHARLIST pTarget);

	//< 040721_KJHuNs LocalWar폴더
	void SaveLogLocalWar_SealStoneInfo(CHARLIST* pSource, CHARLIST* pTarget, int nDamage); // CSD-040316
	void SaveLogLocalWar_Info(CHARLIST* pTarget); // CSD-040407
	void SaveLog_List(const LOG_TYPE eLogType, LPCSTR lpLogMsg, ...);
	//> 040721_KJHuNs LocalWar폴더

	//040721_KJHuNs NeoNationWar
	void SaveLogNeoNationWar(const int nType, char * szLogMsg, ...);	// LTH-040322-KO 1.04 패치 로그. 지하 전쟁터 관련.

	void SaveMoveToWarfield(const int nType, char* szLogMsg, ...);	// LTH-040515-KO 1.04+ 패치 로그. 평화 기간 전쟁터 이동 관련.

	//040721_KJHuNs Script
	void SaveLogScriptLog(CHARLIST* pTarget, const int nQuestNo, const int nQuestStep,
		const char* msg); //kyo
	void ItemMallLog(char* pString);
	void SKILL_RARE_UPGRADE_FAIL_Log(char* pString);
	void ItemMall_Use_Log(char* pString);
	const char* ConvertToDay(int nDay) const;
	const char* ConvertToTactics(int nKind) const;

public:
	bool GetLogPath(string& rPath)
	{
		if (!m_strRoot.empty())
		{
			rPath = m_strRoot;
			return true;
		}

		return false;
	}

	bool GetLogPath(int nType, string& rPath)
	{
		if (m_mpBuffer.find(nType) != m_mpBuffer.end())
		{
			rPath = m_mpBuffer[nType];
			return true;
		}

		return false;
	}
private:
	const char* GetTargetName(CHARLIST* pTarget) const;

private:
	string   m_strRoot;
	HASH_LOG m_mpBuffer;
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_LOGMANAGER_H__564A7676_EFA9_408A_ABA6_9EBB37D1786E__INCLUDED_)
