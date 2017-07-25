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

//<050224_KCH 마일리지몰 작업

int GetItemMallItemKind_ItemNo(const int nItemIdx)
{
	for(int i=0;i<static_cast<int>(g_ItemMallItem.size());++i)
	{
		if(nItemIdx == g_ItemMallItem[i].iIndex)
			return g_ItemMallItem[i].iItemNo;
	}
	return -1;
}
//기간제한이 지난 아이템은 사용만기체크후 백업테이블로 옮긴다.(기간제한만기시 제거)
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
	if( nCnt_IsUseItemMallItem <= 0 )	//사용중인 마일리지 기간제한 아이템의 갯수가 몇개나 되나?
	{
//			SendPutMenuString( cn, KM_FAIL, 218, _GetItemToGameTBL.server_id );
		m_IsActiveProcess_byPer1Min = false;
		return;	//아이템 몰에서 구입한 기간 제한 아이템이 없다.	SKIP
	}

	m_IsActiveProcess_byPer1Min = true;	//flag값으로 Process_Item_to_game_TBL 처리중인때, 다시 못들어오게 세팅.

	ZeroMemory(&m_ItemToGame_TBL,sizeof(m_ItemToGame_TBL));
	m_nCnt_ItemToGame_TBL	= 0;
	
	//아이템몰아이템을 소유한 유저라면..시간제 아이템을 확인해서 삭제 처리하도록 하자.
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

	//050304_KCH EndUseTime에 종료시간이 들어 있어야 한다.
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
	}	//만약 다른곳에서 그동안 아이템을 지워버렸다면, Skip
	Dbg_Assert(0 < m_nCnt_ItemToGame_TBL && MAX_GET_ITEM_TO_GAME_TBL >= m_nCnt_ItemToGame_TBL);

	k_Delete_Item_Mall_ARRAY	_DeleteARRAY;
	ZeroMemory(&_DeleteARRAY,sizeof(_DeleteARRAY));
	int nFixCnt = -999;	//배열안에 계속 원형큐처럼 넣어준다.
	
	for (int j=0;j<m_nCnt_ItemToGame_TBL;++j)
	{
		CTime curr_time;
		curr_time = CTime::GetCurrentTime();
		if( m_ItemToGame_TBL[j].EndUseTime > curr_time) { continue; }	//종료시간이 아직 남았다면, Skip
		
		const int nItemNo = GetItemMallItemKind_ItemNo(m_ItemToGame_TBL[j].itemMallItemKind_index);

		if (Size_Delete_Item_Mall_ARRAY> j) { nFixCnt = j; }
		else if (Size_Delete_Item_Mall_ARRAY <= j) 
		{
			nFixCnt = j%Size_Delete_Item_Mall_ARRAY;

			if (0 == nFixCnt) { nFixCnt = Size_Delete_Item_Mall_ARRAY; }		//배열에 최대값까지 채웠음.
		}

		_DeleteARRAY._DelItemMall_ARRAY[nFixCnt].itemmall_index			= m_ItemToGame_TBL[j].itemmall_index;
		_DeleteARRAY._DelItemMall_ARRAY[nFixCnt].itemMallItemKind_index	= m_ItemToGame_TBL[j].itemMallItemKind_index;
		_DeleteARRAY._DelItemMall_ARRAY[nFixCnt].item_limit				= m_ItemToGame_TBL[j].item_limit;
		_DeleteARRAY._DelItemMall_ARRAY[nFixCnt].EndUseTime				= m_ItemToGame_TBL[j].EndUseTime.GetTime();
		_DeleteARRAY._DelItemMall_ARRAY[nFixCnt].ItemNo					= nItemNo;
		strcpy( _DeleteARRAY._DelItemMall_ARRAY[nFixCnt].recv_name, m_ItemToGame_TBL[j].recv_name);
		_DeleteARRAY.nCnt_DelItemMall = nFixCnt+1;		//배열이 0부터 시작하므로.
															//v 패킷을 몇번 보낼것인가.
		if ((Size_Delete_Item_Mall_ARRAY-1) <= j && 0 < (m_nCnt_ItemToGame_TBL /Size_Delete_Item_Mall_ARRAY))	//한번이상 보내야하고, Size_Delete_Item_Mall_ARRAY가 넘어갈때 
		{
			if (0 == (j+1)%Size_Delete_Item_Mall_ARRAY)	//배열이 꽉 찼기 때문에, 패킷을 날린다.
			{
				//Size_Delete_Item_Mall_ARRAY까지 적당히 채운뒤, 한꺼번에 패킷을 날리자.
				//전체 맵서버에 아이템삭제를 요청하고, 사용만기임을 통보하자.
				// 전체 메시지 날리기
				t_packet packet;
				packet.h.header.type = CMD_DELETE_ITEM_MALL_ITEM_PER_1MIN;
				packet.u.kein.delete_item_mall_array = _DeleteARRAY;
				packet.h.header.size = sizeof( k_Delete_Item_Mall_ARRAY );

				//해당맵에서 클라이언트 삭제 처리하고, totalDB item_to_game TBL에도 세팅한다.
				SendPacket2Maps( &packet );
//				g_pUserManager->SendPacket(&packet); // CSD-CN-031213
				RecvItemMallItemDelete_Per1Min(&packet, 0);	//직접 Local Map에도 보낸다. cn은 사용안함
			}
		}

		if(j == m_nCnt_ItemToGame_TBL-1)	//남은패킷을 마지막으로 한번에 보낸다.
		{
			//Size_Delete_Item_Mall_ARRAY까지 적당히 채운뒤, 한꺼번에 패킷을 날리자.
			//전체 맵서버에 아이템삭제를 요청하고, 사용만기임을 통보하자.
			// 전체 메시지 날리기
			t_packet packet;
			packet.h.header.type = CMD_DELETE_ITEM_MALL_ITEM_PER_1MIN;
			packet.u.kein.delete_item_mall_array = _DeleteARRAY;
			packet.h.header.size = sizeof( k_Delete_Item_Mall_ARRAY );

			//해당맵에서 클라이언트 삭제 처리하고, totalDB item_to_game TBL에도 세팅한다.  cn은 사용안함
			SendPacket2Maps( &packet );
//			g_pUserManager->SendPacket(&packet); // CSD-CN-031213
			RecvItemMallItemDelete_Per1Min(&packet, 0);	//직접 Local Map에도 보낸다.
		}
	}

	m_IsActiveProcess_byPer1Min = false;
}