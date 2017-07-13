// ItemMallManager.h: interface for the CItemMallManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMMALLMANAGER_H__446DAC05_9DA9_465B_AA88_7C859F256EEE__INCLUDED_)
#define AFX_ITEMMALLMANAGER_H__446DAC05_9DA9_465B_AA88_7C859F256EEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//const int nITEM_MALL_MANAGER_PORT				= 5410;		////050224_KCH ���ϸ����� �۾� ���Ƿ� 5410(DESERT2)Ȳ���� Map�� ItemMallMgr�� ����
#define MAX_GET_ITEM_TO_GAME_TBL	800		//Local ��ǻ�Ϳ��� �׽�Ʈ�غ���� 967���� ������ ó���� ������, 
											//���̻��� INETWORK���� SKIP�ϴ°� ����.(��Ȯ��)
///////////////////////////////////////////////////////////////////////////////
//

class CItemMallManager : public TSingleton<CItemMallManager>  
{
public:
	CItemMallManager();
	virtual ~CItemMallManager();

public:
	void Process_Item_to_game_TBL();								//ȣ���Ͽ�, �Ⱓ��������� ����ó��.
private:
	k_get_item_to_game_TBL	m_ItemToGame_TBL[MAX_GET_ITEM_TO_GAME_TBL];	//�����۸��� �迭�� �־ ��������.(�ʹ����� ���� �������� ó���Ҽ� ���⿡, ������ Fix����)
	mutable	int				m_nCnt_ItemToGame_TBL;						//���� ����ϴ� �����۸� ����.
	bool					m_IsActiveProcess_byPer1Min;
};
//
///////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_ITEMMALLMANAGER_H__446DAC05_9DA9_465B_AA88_7C859F256EEE__INCLUDED_)
