#include "pay.h"
//#include "StdAfx.h"

#include "MAIN.H"
//#include "NPC_Pattern.h"
//#include "Map.h"
//#include "Scrp_exe.h"
#include "monitor.h"
#include "servertable.h"
//#include "Id.h"
//商店系统
const int month_tbl[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };	
extern HDBC  hDBC_TotalDB;
		
extern int GetUserAge(const char* szID);	// 030929 kyo
extern bool IsLimitedTime();				// 030929 kyo

COnePass onepass;
		
COnePass::COnePass()
{
}		

COnePass::~COnePass()
{		
}		
		
int COnePass::InsertUsedID_SQL_ForPay( LPSTR szMapName, LPSTR szUID, 
									  char *IP , char *joint_id, 
									  int type, WORD wPort, DWORD dwID )
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode = 0;
	
	char		szQuerry[512] = {0,};

	::SQLAllocStmt(hDBC_TotalDB, &hStmt);
	
	::sprintf(szQuerry, "insert into logintable (servername, user_id, type, ip, joint_id, port, agent_id, server_set_num, d_kyulje) values "
						"('%s','%s',"
						"'%d','%s','%s',"
						"'%d','%d','%d','%d')",
						szMapName, szUID , 
						type , IP, joint_id, 
						wPort, dwID,  g_pServerTable->GetServerSetNum(), this->d_kyulje);
	retCode = ::SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	::SQLFreeStmt(hStmt, SQL_DROP);
			
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		return 1;
	}		

	return 0;
}

//1207 zhh
int COnePass::DeleteUsedID_SQL_ForPay(LPSTR szMapName, LPSTR szUID,int port)
{	
	char	szQuerry[255]={0,};
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	//001218 zhh
	if(port==0)
	{
		if(szUID == NULL)
			sprintf(szQuerry, "delete from logintable where servername='%s' AND server_set_num = %d", szMapName, g_pServerTable->GetServerSetNum() );
		else
			sprintf(szQuerry, "delete from logintable where user_id='%s'", szUID);
	}		
	else	
	{		
		sprintf(szQuerry, "DELETE FROM logintable WHERE port=%d AND server_set_num = %d ", port,  g_pServerTable->GetServerSetNum());
		MyLog( LOG_NORMAL, szQuerry);
	}
			
	SQLAllocStmt(hDBC_TotalDB, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	SQLFreeStmt(hStmt, SQL_DROP);
		
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{	
		return 1;
	}	
	else
	{	
		MyLog( LOG_FATAL, "<< QUERY FAIL >> User '%s's DELETE QUERY has Failed!!!", szUID );
	}	
		
	return 0;
}
	
//2001/02/19 zhh
#include "./China/QueryDBSocket.h"
extern CQueryDBSocket *ConQ;

OUTPUT COnePass::OnePassID(const short nCn,LoginInfoPay &LIP,const bool bIsGMTool)
{
	OUTPUT Output = {0,};
	if(LocalMgr.IsAbleNation(JAPAN) && !bIsGMTool)//老夯牢单 老馆 蜡历老 版快
	{
		Output.nRet		=	CheckLoginIDForJapan(nCn,LIP);
		Output.nType	=	LIP.type;
		Output.dwIndex	=	LIP.index;
	}
	else
	{
		Output.nRet		=	CheckPW_TotalDB_SQL(nCn,LIP);//肺弊牢 啊瓷茄瘤俊 措茄 函荐 涝聪促.
		Output.nType	=	LIP.type;//搬力 鸥涝涝聪促.
		Output.dwIndex	=	LIP.index;//
		// 吝惫狼 苞陛俊辑 IP窜困狼 苞陛捞 甸绢啊搁 捞何盒俊 眠啊等促. 
/* //coromo 2007/5/2  disable pay
		if(LocalMgr.IsAbleNation(TAIWAN|CHINA|HONGKONG))//021007 lsw//吝惫全尼措父 葛滴 酒捞乔甫 眠啊茄促. 
		{
			if( Output.nRet > 0 || Output.nRet == BT_NEED_PAY )//0焊促 努 版快绰 巢篮 朝楼啊 乐绰 巴捞绊
			{
				ConQ->AskCheckLoginWithIP( LIP.id, LIP.ip );
				Output.nRet = COnePass::BT_WAIT_BILLING_MSG;//吝惫狼 版快绰
			}
		}
*/
	}
	return Output;
}


// 橇府 海鸥老 版快 立加 啊瓷茄 酒捞叼 牢瘤 朝磊肺 犬牢 且锭 荤侩
int GetAccessPossableDay( int &year, int &month, int &day )
{
	char szRegistDate[50]= {0,};
	// YYYY/MM/DD 屈侥捞绢具 窃
	if( GetPrivateProfileString( "nation_set", "RegistDate", "" , szRegistDate, 50,DB_DEMON_INI_ ) )
	{
		char *token = strtok( szRegistDate, "/" );
		if( !token ) return 0;
		year = atoi(token);
		
		token = strtok( NULL, "/" );
		if( !token ) return 0;
		month = atoi(token);

		token = strtok( NULL, "/" );
		if( !token ) return 0;
		day = atoi(token);

		return 1;
	}
	
	return 0;
}

// 府畔蔼..
// -1 : ID 绝澜. 
// -2 : PW 撇覆..
// -3 : 捣郴具窃..
//      巢篮 朝楼..
extern bool IsFreeLevel( char *szUID );
int COnePass::CheckPW_TotalDB_SQL(const short nCn,LoginInfoPay &LIP)//荤侩 啊瓷茄 朝楼甫 府畔 钦聪促.
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode= 0;
	SDWORD		cbValue= 0;
	bool		bIsPwRight= 0;	
	int			User_TimeReamin=0;
	int			ip_idx=0;

	TIMESTAMP_STRUCT	date = {0,};

	if( LIP.pw == NULL || ::strlen(LIP.pw) <= 0 ){ return(BT_WRONG_PW); }

	::prepare( connections );

	::EatRearWhiteChar( LIP.id );

	char		szQuerry[ MAX_PATH ] = {0,};
    ::sprintf(szQuerry, "EXEC up_get_user_info2 '%s'", LIP.id);//	sprintf(szQuerry, "select id_index, passwd, d_eday from chr_log_info where login_id='%s'", LIP.id );

	::SQLAllocStmt(hDBC_TotalDB, &hStmt);
	retCode = ::SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{															
		retCode = ::SQLFetch(hStmt);
		if( retCode != SQL_SUCCESS) 
		{
			goto ERROR_NO_ID_;	// 殿废登瘤 臼篮 ID涝聪促. 
		}
		
		while(retCode == SQL_SUCCESS)
		{
			char PW[PW_LENGTH+1]= {0,};
			retCode = ::SQLGetData(hStmt, 1, SQL_C_ULONG,		&LIP.index,		0,		 &cbValue);
			retCode = ::SQLGetData(hStmt, 2, SQL_C_CHAR,		PW,				PW_LENGTH, &cbValue);
			retCode = ::SQLGetData(hStmt, 3, SQL_C_ULONG,		&this->d_kyulje,		0,		 &cbValue);
			retCode = ::SQLGetData(hStmt, 4, SQL_C_TIMESTAMP,	&date,			sizeof( TIMESTAMP_STRUCT ), &cbValue);
			retCode = ::SQLGetData(hStmt, 5, SQL_C_ULONG,		&User_TimeReamin, 0,		 &cbValue);

			::EatRearWhiteChar(PW);
			if(!::strcmp(LIP.pw, PW))
			{
				bIsPwRight = TRUE;
				break;
			}
		}
		::SQLFreeStmt(hStmt, SQL_DROP);

		if(!bIsPwRight){return(BT_WRONG_PW);}// password 撇覆..

		if(LocalMgr.IsFreeBeta())//021007 lsw
		{
			int year = 0, month = 0, day = 0;
			if( ::GetAccessPossableDay( year, month, day ) )//殿废 朝楼肺 橇府海鸥啊 啊瓷 茄瘤 眉农
			{
				sprintf(szQuerry, "login_id = '%s' AND d_regday > '%d-%d-%d' ", LIP.id, year, month, day);
				int nCount = 0;
				::GetRowLineOfSQL( "chr_log_info", "*", &nCount, szQuerry, hDBC_TotalDB );
				if( !nCount ) 
				{
					return BT_WRONG_ID;
				}
			}
			return BT_FREE;
		}

		const int iLimitDay = date.year * 365 + month_tbl[ date.month-1] + date.day; 
		const int iToday = g_year * 365 + g_yday;	
		if( iLimitDay >= iToday )// 1窜拌 : 蜡历朝楼 眉农.
		{
			return 1 + iLimitDay - iToday;// 菊栏肺 荤侩啊瓷茄 朝楼荐 
		}
		//< CSD-040127
		if (LocalMgr.IsAbleNation(TAIWAN|HONGKONG))
		{
			return BT_NEED_PAY;
		}
/* //coromo 2007/5/2  disable pay
		if (LocalMgr.IsAbleNation(CHINA))
		{
			if (IsFreeLevel(LIP.id))
			{
				return BT_FREE;
			}
			return BT_NEED_PAY;
		}
*/
		//> CSD-040127
		DWORD can_use = 0;
		int IP_TimeRemain=0;
		const int ret = CheckGameBangIP_SQL( &can_use, LIP.ip ,LIP.type,IP_TimeRemain,ip_idx);

		LIP.type = 0;
		
		if( ret > 0 )// 2窜拌 : IP朝楼 眉农..
		{	
			this->d_kyulje=0;
			return BT_COMMERCIAL_IP;
		}
		if(IP_TimeRemain>0)// 3窜拌 : IP 急阂辆樊力 眉农..
		{
			this->d_kyulje=0;	//ip客 包访瞪 版快 0栏肺 持绢崔扼绰 碍秦盔 评厘丛狼 夸备俊 蝶扼 0栏肺 技泼.
			LIP.type = ip_idx + 50000;
			return BT_COMMERCIAL_IP;
		}
		else if(User_TimeReamin>0)// 4窜拌 : 蜡历 急阂辆樊力 眉农..
		{
			this->d_kyulje=4000;	
			LIP.type = 4000; // 蜡历 矫埃 辆樊力 鸥涝
			return BT_COMMERCIAL_TIME_REMAIN; // 4老 捞惑巢篮巴栏肺 埃林. 
		}

		if (LocalMgr.IsAbleNation(KOREA))//捣阑 救辰 荤恩捞促
		{	//< CSD-040127
			if( ::IsFreeLevel( LIP.id ) )
			{
				return BT_FREE;
			}// 橇府饭骇蜡历捞骨肺 烹苞
		}	//> CSD-040127

		return BT_NEED_PAY;	// 捣郴具窃..
	}
	else
	{
		displaySQLError( hStmt, szQuerry ); // 030923 HK YGI
	}
ERROR_NO_ID_:
	::SQLFreeStmt(hStmt, SQL_DROP);	
	return BT_WRONG_ID;							// 弊繁 ID绝澜.
}



int COnePass::CheckGameBangIP_SQL( DWORD *can_use, LPSTR ip ,int &type, int &IP_TimeRemain,int &ip_idx)
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	BOOL		bCheck;
	char		szQuerry[255];
	TIMESTAMP_STRUCT	date;
	int month_tbl[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };	
	int rt, ct;
/* //coromo 2007/5/2  disable pay
	if(LocalMgr.IsAbleNation(TAIWAN|CHINA|HONGKONG))//021007 lsw
	{
		return BT_NEED_PAY;
	}
*/
	bCheck = FALSE;
	//010104 zhh
	//sprintf(szQuerry, "select can_use, Billing_eday, ip_type from IP_USE where ip ='%s'", ip );
    /////////////////////////////////////////////////////////////////////////////
    sprintf(szQuerry, "EXEC up_get_ip_info '%s'", ip);
    /////////////////////////////////////////////////////////////////////////////
	SQLAllocStmt(hDBC_TotalDB, &hStmt);

	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		retCode = SQLFetch(hStmt);
			
		if( retCode == SQL_SUCCESS ) // 殿废等 IP啊 粮犁窃. 
		{	
			SQLGetData(hStmt, 1, SQL_C_ULONG, can_use, 0, &cbValue);
			SQLGetData(hStmt, 2, SQL_C_TIMESTAMP, &date,  sizeof( TIMESTAMP_STRUCT ), &cbValue);

			//1207 zhh
			SQLGetData(hStmt, 3, SQL_C_ULONG, &type, 0, &cbValue);
			SQLGetData(hStmt, 4, SQL_C_ULONG, &ip_idx, 0, &cbValue);
			
			SQLFreeStmt(hStmt, SQL_DROP);
			
			this->CheckGameBangIPAccount_SQL( ip_idx, IP_TimeRemain);
			//	朝楼 拌魂.. 
			if( *can_use == 1 ) // 荤侩啊瓷茄 IP..
			{	
				rt = date.year * 365 + month_tbl[ date.month-1] + date.day;
				ct = g_year * 365 + g_yday;
				
				if( rt < ct )	return BT_NEED_PAY;
				else			return 1 + rt - ct;	// 菊栏肺 荤侩啊瓷茄 朝楼俊 
			}	
			else
			{	
				return -2;
			}	
		}		
		else	
		{		
			SQLFreeStmt(hStmt, SQL_DROP);
			return -2;
		}		
	}			
	else
	{
		SQLFreeStmt(hStmt, SQL_DROP);	
		return -1;							// 弊繁 白规IP绝澜.
	}
	SQLFreeStmt(hStmt, SQL_DROP);			// 0414 YGI
	return(-2);
};

int COnePass::CheckLoginIDForJapan(const short nCn, LoginInfoPay &LIP)
{
	HSTMT	hStmt = NULL;
	RETCODE	retCode= 0;
	SDWORD	cbValue= 0;
	char	szQuerry[ MAX_PATH ] = {0,};
	int		rt = 0, ct= 0;

	EatRearWhiteChar( LIP.id );
	EatRearWhiteChar( LIP.pw );

	sprintf(szQuerry, "SELECT   Memidx,Uid FROM NgcTempUser WHERE (Utid = '%s%s')", LIP.id,LIP.pw);//老夯 DB曼炼//TID肺 ID 啊廉坷扁.

	SQLAllocStmt(hDBC_NGCDB, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{															
		retCode = SQLFetch(hStmt);
		if( retCode != SQL_SUCCESS) 
		{
			goto ERROR_NO_ID_;	// 殿废登瘤 臼篮 ID涝聪促. 
		}
		retCode = ::SQLGetData(hStmt, 1, SQL_C_LONG,	&LIP.index,	ID_LENGTH, &cbValue);
		
		char szID[ID_LENGTH]= {0,};
		retCode = ::SQLGetData(hStmt, 2, SQL_C_CHAR,		szID,	ID_LENGTH, &cbValue);
		::EatRearWhiteChar( szID );
		::CharUpper( szID );
		::strcpy(connections[nCn].id,szID);
		::strcpy(LIP.id,szID);
		::SQLFreeStmt(hStmt, SQL_DROP);
		return 100;//老夯牢 版快 傍楼涝聪促. NGC俊辑 苞陛阑 窍扁 锭巩俊.
	}
ERROR_NO_ID_:

	::SQLFreeStmt(hStmt, SQL_DROP);	
	return BT_WRONG_ID;							// 弊繁 ID绝澜.
}

int COnePass::CheckGameBangIPAccount_SQL( DWORD ip_idx, int &IP_TimeRemain)
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;
	char		szQuerry[255];

	if(LocalMgr.IsAbleNation(TAIWAN|CHINA|HONGKONG))//021007 lsw
	{
		IP_TimeRemain = 0;
		return 0;
	}

	sprintf(szQuerry, "select timeremain from IP_ACCOUNT where ip_idx ='%d'", ip_idx );

	SQLAllocStmt(hDBC_TotalDB, &hStmt);

	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		retCode = SQLFetch(hStmt);
			
		if( retCode == SQL_SUCCESS ) // 殿废等 IP啊 粮犁窃. 
		{	
			SQLGetData(hStmt, 1, SQL_C_ULONG, &IP_TimeRemain, 0, &cbValue);
			SQLFreeStmt(hStmt, SQL_DROP);			
			return IP_TimeRemain;
		}
	}
	SQLFreeStmt(hStmt, SQL_DROP);	
	return -1;
}

bool COnePass::CheckLimitedAgeAndTime(const char* szID) // 30929 kyo
{
	if(NULL == szID )
	{
		return false;
	}

	::prepare( connections );
	if( IsLimitedTime() )
	{
		int nAge = GetUserAge( szID );
		int nLimitedAge = ::GetPrivateProfileInt( "Thai Limited", "Age", 18, DB_DEMON_INI_ );
		if( nAge < nLimitedAge || nAge <=0 )
		{
			return true;
		}		
	}
	return false;	
}

