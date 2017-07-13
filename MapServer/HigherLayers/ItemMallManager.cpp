// ItemMallManager.cpp: implementation of the CItemMallManager class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "ItemMallManager.h"

#include "UserManager.h"
#include "Kein_item.h"
#include "CItem.h"

///////////////////////////////////////////////////////////////////////////////
// Static Member
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CItemMallManager::CItemMallManager()
{
	m_IsActiveProcess_byPer1Min	= false;
	ZeroMemory(&m_ItemToGame_TBL,sizeof(m_ItemToGame_TBL));
	m_nCnt_ItemToGame_TBL	= 0;
}

CItemMallManager::~CItemMallManager()
{
	m_IsActiveProcess_byPer1Min	= false;

	ZeroMemory(&m_ItemToGame_TBL,sizeof(m_ItemToGame_TBL));
	m_nCnt_ItemToGame_TBL	= 0;
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

//<050224_KCH ���ϸ����� �۾�

int GetItemMallItemKind_ItemNo(const int nItemIdx)
{
	for(int i=0;i<static_cast<int>(g_ItemMallItem.size());++i)
	{
		if(nItemIdx == g_ItemMallItem[i].iIndex)
			return g_ItemMallItem[i].iItemNo;
	}
	return -1;
}
//�Ⱓ������ ���� �������� ��븸��üũ�� ������̺�� �ű��.(�Ⱓ���Ѹ���� ����)
void CItemMallManager::Process_Item_to_game_TBL()
{

	if (true == m_IsActiveProcess_byPer1Min)
	{ 
		return; 
	}
//		k_get_item_to_game_TBL _GetItemToGameTBL;
	char condition[256];
	sprintf( condition, "recv_type = '%d'", Progressive_TIMESTAMP_ItemMallItem);

	int	nCnt_IsUseItemMallItem = 0;	
	GetRowLineOfSQL(TOTAL_DB, ITEM_TO_GAME, "*", &nCnt_IsUseItemMallItem, condition);
	if( nCnt_IsUseItemMallItem <= 0 )	//������� ���ϸ��� �Ⱓ���� �������� ������ ��� �ǳ�?
	{
//			SendPutMenuString( cn, KM_FAIL, 218, _GetItemToGameTBL.server_id );
		m_IsActiveProcess_byPer1Min = false;
		return;	//������ ������ ������ �Ⱓ ���� �������� ����.	SKIP
	}

	m_IsActiveProcess_byPer1Min = true;	//flag������ Process_Item_to_game_TBL ó�����ζ�, �ٽ� �������� ����.

	ZeroMemory(&m_ItemToGame_TBL,sizeof(m_ItemToGame_TBL));
	m_nCnt_ItemToGame_TBL	= 0;
	
	//�����۸��������� ������ �������..�ð��� �������� Ȯ���ؼ� ���� ó���ϵ��� ����.
	SDWORD		cbValue;
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	char		szQuerry[2048] = {0,};
//	CString		strString;
	TIMESTAMP_STRUCT	/*start,*/ end;
	
	int nRandomFind = rand()%nCnt_IsUseItemMallItem;
	if (MAX_GET_ITEM_TO_GAME_TBL > nCnt_IsUseItemMallItem-nRandomFind)
	{
		nRandomFind = 1;
	}

	//050304_KCH EndUseTime�� ����ð��� ��� �־�� �Ѵ�.
	sprintf(szQuerry,"select top %d idx,itemid,item_limit,recv_loginid,recv_name,EndUseTime from Item_to_Game where recv_type='%d' AND EndUseTime IS NOT NULL AND getdate()>=EndUseTime AND idx>(select max(idx) from (select top %d idx from item_to_game)s)",
MAX_GET_ITEM_TO_GAME_TBL,
Progressive_TIMESTAMP_ItemMallItem,
nRandomFind);

	SQLAllocStmt(g_hDBC_TotalDB, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		retCode = SQLFetch(hStmt);
		while(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			retCode = SQLGetData(hStmt, 1, SQL_C_LONG, &m_ItemToGame_TBL[m_nCnt_ItemToGame_TBL].itemmall_index, 0, &cbValue);
			retCode = SQLGetData(hStmt, 2, SQL_C_LONG, &m_ItemToGame_TBL[m_nCnt_ItemToGame_TBL].itemMallItemKind_index, 0, &cbValue);
			retCode = SQLGetData(hStmt, 3, SQL_C_LONG, &m_ItemToGame_TBL[m_nCnt_ItemToGame_TBL].item_limit, 0, &cbValue);
			retCode = SQLGetData(hStmt, 4, SQL_C_CHAR, &m_ItemToGame_TBL[m_nCnt_ItemToGame_TBL].recv_loginid, sizeof(m_ItemToGame_TBL[m_nCnt_ItemToGame_TBL].recv_loginid), &cbValue);
			EatRearWhiteChar( m_ItemToGame_TBL[m_nCnt_ItemToGame_TBL].recv_loginid );
			retCode = SQLGetData(hStmt, 5, SQL_C_CHAR, &m_ItemToGame_TBL[m_nCnt_ItemToGame_TBL].recv_name, sizeof(m_ItemToGame_TBL[m_nCnt_ItemToGame_TBL].recv_name), &cbValue);
			EatRearWhiteChar( m_ItemToGame_TBL[m_nCnt_ItemToGame_TBL].recv_name );
			retCode = SQLGetData(hStmt, 6, SQL_C_TIMESTAMP, &end,sizeof( TIMESTAMP_STRUCT ), &cbValue);
			CTime _TempTime(end.year,end.month,end.day,end.hour, end.minute, end.second);
			m_ItemToGame_TBL[m_nCnt_ItemToGame_TBL].EndUseTime = _TempTime;
			retCode = SQLFetch(hStmt);

			++m_nCnt_ItemToGame_TBL;
		}
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	if (0 == m_nCnt_ItemToGame_TBL)	
	{ 
		m_IsActiveProcess_byPer1Min = false;
		return; 
	}	//���� �ٸ������� �׵��� �������� �������ȴٸ�, Skip
	Dbg_Assert(0 < m_nCnt_ItemToGame_TBL && MAX_GET_ITEM_TO_GAME_TBL >= m_nCnt_ItemToGame_TBL);

	k_Delete_Item_Mall_ARRAY	_DeleteARRAY;
	ZeroMemory(&_DeleteARRAY,sizeof(_DeleteARRAY));
	int nFixCnt = -999;	//�迭�ȿ� ��� ����ťó�� �־��ش�.
	
	for (int j=0;j<m_nCnt_ItemToGame_TBL;++j)
	{
		CTime curr_time;
		curr_time = CTime::GetCurrentTime();
		if( m_ItemToGame_TBL[j].EndUseTime > curr_time) { continue; }	//����ð��� ���� ���Ҵٸ�, Skip
		
		const int nItemNo = GetItemMallItemKind_ItemNo(m_ItemToGame_TBL[j].itemMallItemKind_index);

		if (Size_Delete_Item_Mall_ARRAY> j) { nFixCnt = j; }
		else if (Size_Delete_Item_Mall_ARRAY <= j) 
		{
			nFixCnt = j%Size_Delete_Item_Mall_ARRAY;

			if (0 == nFixCnt) { nFixCnt = Size_Delete_Item_Mall_ARRAY; }		//�迭�� �ִ밪���� ä����.
		}

		_DeleteARRAY._DelItemMall_ARRAY[nFixCnt].itemmall_index			= m_ItemToGame_TBL[j].itemmall_index;
		_DeleteARRAY._DelItemMall_ARRAY[nFixCnt].itemMallItemKind_index	= m_ItemToGame_TBL[j].itemMallItemKind_index;
		_DeleteARRAY._DelItemMall_ARRAY[nFixCnt].item_limit				= m_ItemToGame_TBL[j].item_limit;
		_DeleteARRAY._DelItemMall_ARRAY[nFixCnt].EndUseTime				= m_ItemToGame_TBL[j].EndUseTime.GetTime();
		_DeleteARRAY._DelItemMall_ARRAY[nFixCnt].ItemNo					= nItemNo;
		strcpy( _DeleteARRAY._DelItemMall_ARRAY[nFixCnt].recv_name, m_ItemToGame_TBL[j].recv_name);
		_DeleteARRAY.nCnt_DelItemMall = nFixCnt+1;		//�迭�� 0���� �����ϹǷ�.
															//v ��Ŷ�� ��� �������ΰ�.
		if ((Size_Delete_Item_Mall_ARRAY-1) <= j && 0 < (m_nCnt_ItemToGame_TBL /Size_Delete_Item_Mall_ARRAY))	//�ѹ��̻� �������ϰ�, Size_Delete_Item_Mall_ARRAY�� �Ѿ�� 
		{
			if (0 == (j+1)%Size_Delete_Item_Mall_ARRAY)	//�迭�� �� á�� ������, ��Ŷ�� ������.
			{
				//Size_Delete_Item_Mall_ARRAY���� ������ ä���, �Ѳ����� ��Ŷ�� ������.
				//��ü �ʼ����� �����ۻ����� ��û�ϰ�, ��븸������ �뺸����.
				// ��ü �޽��� ������
				t_packet packet;
				packet.h.header.type = CMD_DELETE_ITEM_MALL_ITEM_PER_1MIN;
				packet.u.kein.delete_item_mall_array = _DeleteARRAY;
				packet.h.header.size = sizeof( k_Delete_Item_Mall_ARRAY );

				//�ش�ʿ��� Ŭ���̾�Ʈ ���� ó���ϰ�, totalDB item_to_game TBL���� �����Ѵ�.
				SendPacket2Maps( &packet );
//				g_pUserManager->SendPacket(&packet); // CSD-CN-031213
				RecvItemMallItemDelete_Per1Min(&packet, 0);	//���� Local Map���� ������. cn�� ������
			}
		}

		if(j == m_nCnt_ItemToGame_TBL-1)	//������Ŷ�� ���������� �ѹ��� ������.
		{
			//Size_Delete_Item_Mall_ARRAY���� ������ ä���, �Ѳ����� ��Ŷ�� ������.
			//��ü �ʼ����� �����ۻ����� ��û�ϰ�, ��븸������ �뺸����.
			// ��ü �޽��� ������
			t_packet packet;
			packet.h.header.type = CMD_DELETE_ITEM_MALL_ITEM_PER_1MIN;
			packet.u.kein.delete_item_mall_array = _DeleteARRAY;
			packet.h.header.size = sizeof( k_Delete_Item_Mall_ARRAY );

			//�ش�ʿ��� Ŭ���̾�Ʈ ���� ó���ϰ�, totalDB item_to_game TBL���� �����Ѵ�.  cn�� ������
			SendPacket2Maps( &packet );
//			g_pUserManager->SendPacket(&packet); // CSD-CN-031213
			RecvItemMallItemDelete_Per1Min(&packet, 0);	//���� Local Map���� ������.
		}
	}

	m_IsActiveProcess_byPer1Min = false;
}