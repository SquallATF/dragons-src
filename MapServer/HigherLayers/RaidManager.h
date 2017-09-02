// RaidManager.h: interface for the CRaidManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAIDMANAGER_H__06B94578_16CE_4B2A_BA1A_49FCF0F432C2__INCLUDED_)
#define AFX_RAIDMANAGER_H__06B94578_16CE_4B2A_BA1A_49FCF0F432C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
// BEGIN
class CRaidManager : public TSingleton<CRaidManager>
{
public:
	CRaidManager();
	virtual ~CRaidManager();

public:
	void RecvMonsterRaidStart();
	void RecvMonsterRaidEnd();

public:
	bool IsRunning() const
	{
		return m_bRunning;
	}

private:
	bool m_bRunning;
};
// END
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_RAIDMANAGER_H__06B94578_16CE_4B2A_BA1A_49FCF0F432C2__INCLUDED_)
