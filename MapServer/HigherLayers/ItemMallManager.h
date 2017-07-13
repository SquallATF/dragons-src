// ItemMallManager.h: interface for the CItemMallManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMMALLMANAGER_H__446DAC05_9DA9_465B_AA88_7C859F256EEE__INCLUDED_)
#define AFX_ITEMMALLMANAGER_H__446DAC05_9DA9_465B_AA88_7C859F256EEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//const int nITEM_MALL_MANAGER_PORT				= 5410;		////050224_KCH 마일리지몰 작업 임의로 5410(DESERT2)황무지 Map을 ItemMallMgr로 세팅
#define MAX_GET_ITEM_TO_GAME_TBL	800		//Local 컴퓨터에서 테스트해본결과 967개의 데이터 처리가 가능함, 
											//그이상은 INETWORK에서 SKIP하는것 같음.(불확실)
///////////////////////////////////////////////////////////////////////////////
//

class CItemMallManager : public TSingleton<CItemMallManager>  
{
public:
	CItemMallManager();
	virtual ~CItemMallManager();

public:
	void Process_Item_to_game_TBL();								//호출하여, 기간만기아이템 삭제처리.
private:
	k_get_item_to_game_TBL	m_ItemToGame_TBL[MAX_GET_ITEM_TO_GAME_TBL];	//아이템몰을 배열에 넣어서 보관하자.(너무많은 양의 아이템을 처리할수 없기에, 적당히 Fix했음)
	mutable	int				m_nCnt_ItemToGame_TBL;						//실제 사용하는 아이템몰 갯수.
	bool					m_IsActiveProcess_byPer1Min;
};
//
///////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_ITEMMALLMANAGER_H__446DAC05_9DA9_465B_AA88_7C859F256EEE__INCLUDED_)
