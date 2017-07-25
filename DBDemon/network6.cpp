﻿#include "StdAfx.h"
#include "packed.h"
#include "servertable.h"
#include "MAIN.H"
#include "Scrp_exe.H"
#include "ID.h"
#include "SealStone.h"
#include "Debug.h"
#include "mainheader.h"
#include "monitor.h"
#include "Citem.h"
#include "Pay.h"
#include "ChrLog.h"
#include "dragonloginserver2.h"		// 010406 YGI
#include "hong_sub.h"
#include "auction.h"
#include "MailMgr.h"
#include "MySql.h"

extern HDBC		hDBC_ChrLogDB;
extern HDBC		hDBC_TotalDB;
extern HENV		hEnv ;
extern HDBC		hDBC ;
extern int	g_wday;
//SadonixControl	sadonixcontrol[24];//24시간 이니까
int	CheckGameMakeTool( char *id, char ip[40])		// 020830 YGI
{
	SDWORD cbValue;

	int		oksign=0;
	char	dbip[40];
	
	memset(dbip,0,40);

	char	szQuerry[255];
	HSTMT hStmt= NULL ;
	
	sprintf(szQuerry, "select * from AccessAbleGMTOOLIpAddress where ip='%s'",  ip);    
	SQLAllocStmt(hDBC_ChrLogDB, &hStmt);

	int retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{															
		retCode = SQLFetch(hStmt);
		if( retCode == SQL_NO_DATA )
		{
			MyLog(0,"!!!!----------------------------------!!!!" );
			MyLog(0,"GmTool Stealed  Thief Ip = '%s'" ,ip);
			MyLog(0,"!!!!----------------------------------!!!!" );

			SQLFreeStmt(hStmt, SQL_DROP);
			return oksign;  //coromo 駱聯GM IP
		}
		
		retCode = SQLGetData(hStmt, 1, SQL_C_CHAR, dbip, 40	, &cbValue);
		EatRearWhiteChar(dbip);
		if(!strcmp(ip, dbip))
		{

			SQLFreeStmt(hStmt, SQL_DROP);
			if(LocalMgr.IsAbleNation(CHINA))//021007 lsw
			{
				sprintf(szQuerry, "select * FROM AccessAbleGMTOOLID WHERE id='%s'",  id);
				SQLAllocStmt(hDBC_ChrLogDB, &hStmt);
				retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
				if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
				{
					retCode = SQLFetch(hStmt);
					SQLFreeStmt(hStmt, SQL_DROP);
					if( retCode == SQL_NO_DATA )
					{
						MyLog(0,"!!!!----------------------------------!!!!" );
						MyLog(0,"GmTool Stealed  Thief ID = '%s'" ,id);
						MyLog(0,"!!!!----------------------------------!!!!" );
						return 0;
					}
					return 1;
				}
				else 
				{
					SQLFreeStmt(hStmt, SQL_DROP);
					MyLog(0,"Check error GM_TOOL ID because no exist AccessAbleGMTOOLID table" ,id);
					return 0;
				}
			}
			else
			{
				return 1;
			}
		}
	}
	else
	{
		MyLog(0,"!!!!--------------------------------------------------!!!!");
		MyLog(0,"ChrLogDB in AccessAbleGMTOOLIpAddress Table Error Check DB");
		MyLog(0,"!!!!--------------------------------------------------!!!!");
	}

	SQLFreeStmt(hStmt, SQL_DROP);
	return oksign;
}

int GetNowUpdateSadonixCount(const short nValue)//020822 lsw
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	const int iDay	= nValue/100;
	const int iHour	= nValue%100;

	if(LocalMgr.IsAbleNation(KOREA))//021007 lsw
	{
		SQLAllocStmt(hDBC_ChrLogDB, &hStmt);
	}
	else
	{
		SQLAllocStmt(hDBC, &hStmt);
	}

	int	iSadCount=0;
	char q[255];
	sprintf(q,"select [%d] from SadonixControlOrigin where [no] = %d ",iDay, iHour);//020508 lsw
	retCode = SQLExecDirect(hStmt, (UCHAR *)q, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 0;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			retCode = SQLGetData(hStmt, 1, SQL_C_LONG,	&iSadCount	, 0, &cbValue);
			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
		}	
	}		
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return iSadCount;
}

bool UpdateNowSadonixCount(const int iValue)//020822 lsw
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;

	if(LocalMgr.IsAbleNation(KOREA))//021007 lsw
	{
		SQLAllocStmt(hDBC_ChrLogDB, &hStmt);
	}
	else
	{
		SQLAllocStmt(hDBC, &hStmt);
	}

	int	iSadCount=0;
	char szQuerry[255];
	sprintf(szQuerry,"UPDATE SadonixControl SET No = %d",iValue);
	
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return true;
	}
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	return false;
}

bool RefreshNowSadonixCount(const short nValue)
{
	const int iCount = GetNowUpdateSadonixCount(nValue);
	MyLog(1,"Refresh NOW Update Sadonix Count '%d'",iCount);
	return UpdateNowSadonixCount(iCount);
}

int GetNowSadonixCount()//020822 lsw
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	if(LocalMgr.IsAbleNation(KOREA))//021007 lsw
	{
		SQLAllocStmt(hDBC_ChrLogDB, &hStmt);
	}
	else
	{
		SQLAllocStmt(hDBC, &hStmt);
	}

	int	iSadCount=0;
	char q[255];
//	sprintf(q,"select %d from SadonixControl where [no] = %d ",g_wday , g_hour);
	sprintf(q,"select * from SadonixControl");//이 테이블에는 1행 1열 짜리다
	retCode = SQLExecDirect(hStmt, (UCHAR *)q, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 0;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			retCode = SQLGetData(hStmt, 1, SQL_C_LONG,	&iSadCount	, 0, &cbValue);
			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
		}	
	}		
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return iSadCount;
}

void RecvCMD_CONFORM_SADONIX(const int cn, t_packet &p)
{
	int iSadCount = GetNowSadonixCount();

	if( 0 >= iSadCount )//줄 수 있는 사드오닉스 갯수가 있나?
	{
		MyLog(1,"No More Sadonix Now Count is '%d'",iSadCount);
		return;
	}
	else
	{
		iSadCount--;
		if(UpdateNowSadonixCount(iSadCount))//한개 줬다 
		{
			// 030923 HK YGI
			MyLog(1,"Give to Sadonix '%s' Now Count is '%d' Recommand ItemNo = %d",p.u.ConformSadonix.name, iSadCount, p.u.ConformSadonix.item.item_no);
		}
		else
		{
			MyLog(1,"SadonixControlUpdate Failed Now Count is day ='%d' time = '%d' coount ='%d'", g_wday , g_hour,iSadCount);
			return;
		}
	}
	//허가 패킷 만들어 보낸다.
	//RecvCMD_Give_Sadonix에서 받는다.

	t_packet sp = p;
	sp.h.header.type = CMD_CONFORM_SADONIX;
	sp.h.header.size = sizeof(t_ConformSadonix);
	memcpy(&sp.u.ConformSadonix.name,&p.u.ConformSadonix.name,20);
	sp.u.ConformSadonix.item = p.u.ConformSadonix.item;
	sp.u.ConformSadonix.x = p.u.ConformSadonix.x;
	sp.u.ConformSadonix.y = p.u.ConformSadonix.y;
	QueuePacket(connections,cn,&sp,1);
	return;
}


int IsBlockedId(const char *check_name)
{
	return 0;//지금은 임시로 무조건 블로킹 되지 않았다고 말한다
	SDWORD cbValue;

	int		oksign=0;
	char	dbip[40];
	
	memset(dbip,0,40);

	char	szQuerry[255];
	HSTMT hStmt= NULL ;
	
	sprintf(szQuerry, "select * from Black_List where [Name] ='%s'",  check_name);
    
	SQLAllocStmt(hDBC, &hStmt);
	
	int retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);

	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)//쿼리에 성공 했고
	{															
		retCode = SQLFetch(hStmt);

		switch(retCode)
		{
		case SQL_NO_DATA://그이름이 기록 되어 있지 않네
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return 0;//020802 lsw
			}break;
		default://이름이 기록 되어 있다
			{
				char Name[20];
				retCode = SQLGetData(hStmt, 1, SQL_C_CHAR,		Name,		20	, &cbValue);
				SQLFreeStmt(hStmt, SQL_DROP);
				MyLog(0,"!!!!----------------------------------!!!!" );
				MyLog(0,"Is Blocked Id = '%s'" ,check_name);
				MyLog(0,"!!!!----------------------------------!!!!" );
				return 1;
			}break;
		}
	}
	else
	{
		MyLog(0,"!!!!--------------------------------------------------!!!!");
		MyLog(0,"DragonDB in BlockedId Table Error!!!  PLZ Check DB Table");
		MyLog(0,"!!!!--------------------------------------------------!!!!");
	}

	SQLFreeStmt(hStmt, SQL_DROP);
	return 0;
}

int IsSecretKeyCodeOk(const char *szLogInID,const char *szSecretKeyCode)//020822 lsw
{	
	if(!szLogInID) {return 0;}
	
	char query[256]= {0, } ;

	if(LocalMgr.IsAbleNation(CHINA))//중국은 패스워드 힌트
	{
		::sprintf(query, "SELECT passwd_hint FROM chr_log_info WHERE (login_id = '%s')", szLogInID) ;//계정 선택
	}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//	BBD 031107	1.1과 1.2는 말레이가 패스워드 힌트를 사용한다
//	용오씨가 바꾼 코드로 교체함
/*
	else if( LocalMgr.IsAbleNation(THAI|USA))
	{
		::sprintf(query, "SELECT d_email FROM chr_log_info WHERE (login_id = '%s')", szLogInID);
	}
*/
	else if( LocalMgr.IsAbleNation(THAI))
	{
		::sprintf(query, "SELECT d_email FROM chr_log_info WHERE (login_id = '%s')", szLogInID);
	}
	else if( LocalMgr.IsAbleNation(USA) ) // 030716 kyo malaysia wants using 
	{
		sprintf(query, "SELECT passwd_hint FROM chr_log_info WHERE (login_id = '%s')", szLogInID);
	}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	else if( LocalMgr.IsAbleNation(JAPAN) )
	{
		::sprintf(query, "SELECT uemail FROM member_view WHERE (uid = '%s')", szLogInID);
	}
	else
	{
		::sprintf(query, "SELECT d_jumin FROM chr_log_info WHERE (login_id = '%s')", szLogInID) ;//계정 선택
	}

	SQLRETURN retcode	= 0;
	SQLHSTMT  hstmt		= 0;
	SQLINTEGER  cbvalue	= 0;

	HDBC &hDBCTemp = ((LocalMgr.IsAbleNation(JAPAN))?hDBC_NGCDB:hDBC_TotalDB);

	::SQLAllocStmt(hDBCTemp, &hstmt);//디비선택

	retcode = ::SQLExecDirect(hstmt, (UCHAR *)query, SQL_NTS);

	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
	{
		retcode = ::SQLFetch(hstmt);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
		{
			char szTableKeyCode[51] = {0,};
			retcode= ::SQLGetData(hstmt, 1, SQL_C_CHAR, szTableKeyCode, 50, &cbvalue);
			
			if(!::strcmp(szSecretKeyCode,szTableKeyCode))//키코드가 같다면
			{
				SQLFreeStmt(hstmt, SQL_DROP);//프리후
				return 1;
			}
			else
			{
				::MyLog( LOG_NORMAL, "Failed SecretKeyCode Wrong %s", szSecretKeyCode);
			}
		}
		else 
		{
			::SQLFreeStmt(hstmt, SQL_DROP);
			return 0 ;
		}
	}
	else 
	{
		::SQLFreeStmt(hstmt, SQL_DROP);
		return 0 ;
	}
	::SQLFreeStmt(hstmt, SQL_DROP);//
	return 0 ;
}
//acer7
BOOL DumpException(LPEXCEPTION_POINTERS lpExcep,char* szOutMsg, void *pData, int nSize )
{
	if (!lpExcep)
		return FALSE;

	BOOL		result = TRUE;
	SYSTEMTIME time;
	FILE*		fp = NULL;

	
	const DWORD		dwExceptionAddress = (DWORD)lpExcep->ExceptionRecord->ExceptionAddress;
	const DWORD		dwExceptionCode = lpExcep->ExceptionRecord->ExceptionCode;
	const DWORD		dwExceptionFlags = lpExcep->ExceptionRecord->ExceptionFlags;
	const DWORD		dwNumberParameters = lpExcep->ExceptionRecord->NumberParameters;
	const DWORD		dwDr0 = lpExcep->ContextRecord->Dr0;
	const DWORD		dwDr1 = lpExcep->ContextRecord->Dr1;
	const DWORD		dwDr2 = lpExcep->ContextRecord->Dr2;
	const DWORD		dwDr3 = lpExcep->ContextRecord->Dr3;
	const DWORD		dwDr6 = lpExcep->ContextRecord->Dr6;
	const DWORD		dwDr7 = lpExcep->ContextRecord->Dr7;

	const DWORD		dwSegCs = lpExcep->ContextRecord->SegCs;

	const DWORD		dwSegGs = lpExcep->ContextRecord->SegGs;
	const DWORD		dwSegFs = lpExcep->ContextRecord->SegFs;
	const DWORD		dwSegEs = lpExcep->ContextRecord->SegEs;
	const DWORD		dwEFlags = lpExcep->ContextRecord->EFlags;

	const DWORD		dwEsi = lpExcep->ContextRecord->Esi;
	const DWORD		dwEdi = lpExcep->ContextRecord->Edi;
	const DWORD		dwEbp = lpExcep->ContextRecord->Ebp;
	const DWORD		dwEsp = lpExcep->ContextRecord->Esp;
	const DWORD		dwEip = lpExcep->ContextRecord->Eip;

	const DWORD		dwEax = lpExcep->ContextRecord->Eax;
	const DWORD		dwEbx = lpExcep->ContextRecord->Ebx;
	const DWORD		dwEcx = lpExcep->ContextRecord->Ecx;
	const DWORD		dwEdx = lpExcep->ContextRecord->Edx;
	
	__try
	{
		char szFileName[256+1];
		GetLocalTime(&time);

		sprintf( szFileName, "DBCrash_%d_%d_%d_%d_%d_%d.log", time.wYear,time.wMonth,time.wDay,time.wMinute,time.wSecond );
		fp = fopen( szFileName, "w");

		fprintf(fp,"%d year, %d month, %d day,%d hour, %d minute, %d second\n",
			time.wYear,time.wMonth,time.wDay,time.wMinute,time.wSecond);
		
		fprintf(fp,"%s\n",szOutMsg);
		fprintf(fp,"Crashed address %xh \n",dwExceptionAddress);

		fprintf(fp,"Crashed Map Address %04X:%08X\n",
			dwSegCs,
			dwExceptionAddress);


		fprintf(fp,"Exception Code %u \n",dwExceptionCode);
		fprintf(fp,"Exception Flag %xh \n",dwExceptionFlags);
		fprintf(fp,"NumberParameters %d \n",dwNumberParameters);
		fprintf(fp,"Dr0 %xh \n",dwDr0);
		fprintf(fp,"Dr1 %xh \n",dwDr1);
		fprintf(fp,"Dr2 %xh \n",dwDr2);
		fprintf(fp,"Dr3 %xh \n",dwDr3);
		fprintf(fp,"Dr6 %xh \n",dwDr6);
		fprintf(fp,"Dr7 %xh \n",dwDr7);
		
		fprintf(fp,"SegGs %xh \n",dwSegGs);
		fprintf(fp,"SegFs %xh \n",dwSegFs);
		fprintf(fp,"SegEs %xh \n",dwSegEs);
		fprintf(fp,"EFlags %xh \n",dwEFlags);
		
		fprintf(fp,"Esi %xh \n",dwEsi);
		fprintf(fp,"Edi %xh \n",dwEdi);
		fprintf(fp,"Ebp %xh \n",dwEbp);
		fprintf(fp,"Esp %xh \n",dwEsp);
		fprintf(fp,"Eip %xh \n",dwEip);
		

		fprintf(fp,"Eax %xh \n",dwEax);
		fprintf(fp,"Ebx %xh \n",dwEbx);
		fprintf(fp,"Ecx %xh \n",dwEcx);
		fprintf(fp,"Edx %xh \n",dwEdx);
        
		if( pData ) fprintf(fp,"info : packet_type(PTCL_) =  %d\n",*((char*)pData) );
	
		fprintf(fp,"\n\n");

		fclose(fp);
		fp = NULL;

		if( pData )
		{
			sprintf( szFileName, "DBCrash_info_%d_%d_%d_%d_%d_%d.bin", time.wYear,time.wMonth,time.wDay,time.wMinute,time.wSecond );
			fp = fopen( szFileName, "w");
			fwrite( &nSize, 4, 1, fp );
			fwrite( pData, 1, nSize, fp );
			fclose( fp );
			fp = NULL;
		}
	}
	__except (GetExceptionCode())
	{
		if (fp)
			fclose(fp);
	
		result = FALSE;
	}

	return result;
}

t_EventItem EventItem[MAX_EVENT_ITEM];

int LoadEventRareItem()
{
	memset(EventItem,sizeof(EVENTITEM)*MAX_EVENT_ITEM,0);//클리어

	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	SQLAllocStmt(hDBC, &hStmt);
	retCode = SQLExecDirect(hStmt, (UCHAR *)"Select * from EventRareItem order by IndexNo", SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 0;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			if(i >= MAX_EVENT_ITEM)
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return 0;
			}

			retCode = SQLGetData(hStmt, 1, SQL_C_LONG,	&EventItem[i].iIndex				, 0, &cbValue);
			retCode = SQLGetData(hStmt, 2, SQL_C_LONG,	&EventItem[i].iEvevntType			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 3, SQL_C_LONG,	&EventItem[i].iItemNo				, 0, &cbValue);
			retCode = SQLGetData(hStmt, 4, SQL_C_LONG,	&EventItem[i].iRareKind1			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 5, SQL_C_LONG,	&EventItem[i].iRareKind2			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 6, SQL_C_LONG,	&EventItem[i].iRareKind3			, 0, &cbValue);
			retCode = SQLGetData(hStmt, 7, SQL_C_LONG,	&EventItem[i].iGrade				, 0, &cbValue);
			retCode = SQLGetData(hStmt, 8, SQL_C_LONG,	&EventItem[i].isDynamic				, 0, &cbValue);
			retCode = SQLGetData(hStmt, 9, SQL_C_LONG,	&EventItem[i].iHighRare				, 0, &cbValue);
			
			i++;

			retCode = SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				break;
			}
			else
			{
				SQLFreeStmt(hStmt, SQL_DROP);
				return 0;
			}
		}	
	}		
	SQLFreeStmt(hStmt, SQL_DROP);		// 0308 YGI
	return(1);
}

bool MakeRareAttr(	unsigned int &attr3 ,const int iNowGrade,
					const int iSokSung1,const int iSokSung2,const int iSokSung3,
					const int iHighLevel,const bool bIsDynamicRare)
{
	attr3  = 0;
	if( iNowGrade	>15			||	iNowGrade	<=	0	)	{ return false;}
	if( iSokSung1	>UCHAR_MAX	||	iSokSung1	<=	0	)	{ return false;}
	if( iSokSung2	>UCHAR_MAX	||	iSokSung2	<	0	)	{ return false;}
	if( iSokSung3	>UCHAR_MAX	||	iSokSung3	<	0	)	{ return false;}
	if(	iHighLevel	>7			||	iHighLevel	<	0	)	{ return false;}

	((LPRareMain)&attr3)->grade			= iNowGrade	;
	((LPRareMain)&attr3)->soksung1		= iSokSung1	;
	((LPRareMain)&attr3)->soksung2		= iSokSung2	;
	((LPRareMain)&attr3)->soksung3		= iSokSung3	;
	((LPRareMain)&attr3)->iHighLevel	= iHighLevel;
	((LPRareMain)&attr3)->IsDynamicRare = bIsDynamicRare;
	return true;
}



int MakeDefaultRareItem( ItemAttr *item, const int iItemNo, const int iSok1, const int iGrade, const int iTryMulti)
{
	int iHighLv = 0;
	bool bIsDynamic = false;

	*item = GenerateItem( iItemNo );
	if( !item->item_no ){ return -1;}		// 아이템 번호가 이상하다.

	CItem *t = ItemUnit( *item );
	if( !t ) {	return -1;	}

	if( iSok1 >= 100 )
	{
		iHighLv = H_LV_HIGH_ITEM;
	}
	else
	{
		if( t->GetRbutton() == USE_ITEM )		// 다이나믹 아이템...
		{
			bIsDynamic = true;
		}
	}

	int iSok2=0, iSok3=0;
	if(iSok1)
	{
		switch( iTryMulti )
		{
		case 0 :
		case 1 :
			{
			}break;
		case 2:
			{
				iSok2 = iSok1;
			}break;
		case 3 :
			{
				iSok3 = iSok2 = iSok1;
			}break;
		}

		if( 1 == iHighLv){	iSok3 = iSok2 =0;}

		return (MakeRareAttr( item->attr[IATTR_RARE_MAIN], iGrade, iSok1, iSok2, iSok3, iHighLv, bIsDynamic )?1:0);
	}
	return 1;
}


int SetEventItemEmpty( CHARLIST *ch,  const int iIndex, ItemAttr &GiveItem )		// 빈 인벤토리에 아이템을 생성해서 집어 넣는다. //1215
{
	bool bIsGiveAble = true;//iIndex2 값이 0 이면 절대 안됩니다.
	
	for(int i = 0; MAX_EVENT_ITEM>i;i++)
	{
		if(iIndex == EventItem[i].iIndex )//인덱스가 같으면
		{
			LPEVENTITEM pEI = &EventItem[i];
			if( !pEI->iItemNo){return 0;}//테이블 자체에 값이 없네..
			GiveItem = GenerateItem( pEI->iItemNo );//아이템을 만들어 주고
			if( !GiveItem.item_no){return 0;}//아이템 넘버가 없네? ㅡㅡ;;
			
			if(pEI->iRareKind1)//1번째 속성이 있는 것이라면////레어라면 레어를 주고
			{//레어를 생성해 본다
			bIsGiveAble = MakeRareAttr(GiveItem.attr[IATTR_RARE_MAIN],
				pEI->iGrade,pEI->iRareKind1,pEI->iRareKind2,pEI->iRareKind3,
				pEI->iHighRare,pEI->isDynamic);
			}

			ItemAttr *item = SearchInv( ch );
			if( bIsGiveAble && item )
			{
				*item  = GiveItem;
				return 1;
			}
			else //빈자리가 없다면
			{
				if(bIsGiveAble)
				{
					MyLog(0,"EventRareItemSetting Fault. ReCheck Plz" );
				}
				if(!item)
				{
					MyLog(0,"EventUser Inventory is Full [%s]",ch->Name );						
				}
				return 0;
			}
			break;
		}
	}
	MyLog(0,"EventRareitem Same IndexNo Not Found : Name:[%s] - index:[%d]", ch->Name, iIndex );
	return 0;
}

//<! BBD 040308	넘어온 인덱스에 해당되는 아이템을 만들어 준다
ItemAttr MakeEventItem(int nIndex)
{
	ItemAttr item;
	memset(&item, 0L, sizeof(ItemAttr));
	
	if(!nIndex)		// 인덱스가 0이면 내용없는 아이템이 리턴
	{
		return item;
	}

	if(nIndex != EventItem[nIndex-1].iIndex)	// EventRareItem 테이블의 인덱스가 잘못 정렬되어있다
	{
		// 여기서 테이블이 잘못되었다는 로그를 남겨주자
		MyLog(0,"EventRareItem Table is WRONG!!! Check plz");						
		return item;		// 역시 빈 아이템 리턴
	}


	LPEVENTITEM pEI = &EventItem[nIndex-1];
	if( !pEI->iItemNo)
	{
		return item;
	}//테이블 자체에 값이 없네..
	item = GenerateItem( pEI->iItemNo );//아이템을 만들어 주고
	if( !item.item_no)
	{
		return item;
	}//아이템 넘버가 없네? ㅡㅡ;;
	
	if(pEI->iRareKind1)//1번째 속성이 있는 것이라면////레어라면 레어를 주고
	{//레어를 생성해 본다
		MakeRareAttr(item.attr[IATTR_RARE_MAIN],pEI->iGrade,pEI->iRareKind1,pEI->iRareKind2,pEI->iRareKind3,
			pEI->iHighRare,pEI->isDynamic);
	}

	// 아이템이 완성되었다
	return item;
}
//> BBD 040308	넘어온 인덱스에 해당되는 아이템을 만들어 준다

int HaveAbilityResetList(char *szName,const bool bIsDayCheck)//020822 lsw
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	SQLAllocStmt(hDBC, &hStmt);
	char	szQuerry[255] = {0,};
	if(bIsDayCheck)//쿼리가 날짜 체크 한다
	{
		if(LocalMgr.IsAbleNation(KOREA))//021007 lsw
		{
			sprintf(szQuerry, "Select [name] from AbilityResetDate where [name] = '%s'  AND [date] < (getdate()-30)",  szName);//30일 이전에서 리스트가 있느냐
		}
		else	//한국은 30일 나머지 국가는 90일 마다 한번씩
		{
			sprintf(szQuerry, "Select [name] from AbilityResetDate where [name] = '%s'  AND [date] < (getdate()-90)",  szName);//30일 이전에서 리스트가 있느냐
		}
	}
	else//쿼리가 이름만 있는지 본다
	{
		sprintf(szQuerry, "Select [name] from AbilityResetDate where [name] = '%s'",  szName);//리스트가 있느냐
	}

	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);

	int bIsListed = 0;
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		retCode = SQLFetch(hStmt);
		char szTempName[21] ={0,};
		if( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			retCode = SQLGetData(hStmt, 1, SQL_C_CHAR, szTempName, 20, &cbValue);
			if(szTempName[0]){bIsListed = 1;}
		}
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	return bIsListed;
}

int UpdateAbilityResetList(char *szName, const bool bIsUpdate)
{
	HSTMT		hStmt = NULL;
	RETCODE		retCode;

	SQLAllocStmt(hDBC, &hStmt);
	char	szQuerry[255] = {0,};
	if(bIsUpdate)//쿼리가 Update... 어쩌고
	{
		sprintf(szQuerry, "update AbilityResetDate set [date] = getdate() where name = '%s'",szName);
	}
	else//쿼리가 Insert... 어쩌고
	{
		sprintf(szQuerry, "insert into AbilityResetDate ([name]) values ('%s')",szName);
	}
	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);
	int bIsSuccess = 0;
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		bIsSuccess = 1;
	}
	SQLFreeStmt(hStmt, SQL_DROP);
	return bIsSuccess;
}

void RecvCMD_COMFORM_RESET_ABILITY(const int cn, t_packet &p)//020821-2 lsw
{
	char szName[21] = {0,};
	memcpy(szName, p.u.ComformResetAbililty.szName,20);
	bool bIsAbility = 0;
	//쿼리	//이 이름으로 된 놈의 기록이 있는가. (1)
	if(HaveAbilityResetList(szName,false))//있다면
	{	//지금부터 한달전 보다 과거의 날짜의 이름으로 검색하여 (2)
		if(HaveAbilityResetList(szName,true))
		{//이번에도 기록 검색이 되면 어빌리티 재분배가 가능하다.
			if(UpdateAbilityResetList(szName,true))//업데이트가 성공 하면
			{//재분배 패킷 보내는 플래그 On
				bIsAbility = 1;
			}
			else
			{
				MyLog(0,"AbilityResetList Update Error");
			}
		}
		else
		{
			MyLog(0,"AbilityResetList Time Too Short");
		}
	}
	else//없다면
	{	//Insert 시키고 
		//어빌리티 재분배가 가능하다.//재분배 패킷 보내는 플래그 On
		if(UpdateAbilityResetList(szName,false))
		{//재분배 패킷 보내는 플래그 On
			bIsAbility = 1;
		}
		else
		{
			MyLog(0,"!Warning! Insert AbilityResetList Failed");
		}
	}

	p.u.ComformResetAbililty.iIsAble = bIsAbility;//플래그 저장 하고 
	
	QueuePacket(connections,cn,&p,1);//맵서버로 돌려보낸다
}

bool CheckRookieServerAble(const int iLv)
{
	const int iRookieLv = LocalMgr.GetRookieLv();

	if(	iRookieLv 
	&&  iRookieLv < iLv)
	{//설정한 레벨 보다 높기 때문에 들어 갈 수 없다.
		return false;
	}
	return true;
}

void RecvCMD_SELECTED_CHAR_RESULT_CLIENT_BOTH_DB_DEMON(t_connection c[], DWORD cn, t_packet &p)
{
	const int iMyLv = p.u.Hwoa.rang.SelectedCharResult.iLv;
	if(!CheckRookieServerAble(iMyLv))
	{//초보서버 렙 제한 걸림.
		p.u.Hwoa.rang.SelectedCharResult.iKey = 0;
	}
	else
	{
		p.u.Hwoa.rang.SelectedCharResult.iKey = 1;
	}

	::QueuePacket(c,cn,&p,1);
}

int HandleCommand2(t_connection c[], DWORD dwIndex, t_packet *packet, DWORD dwUserID, int cn)
{
	const int itype	=	packet->h.header.type;

	switch(itype)
	{
	case CMD_CONFORM_SADONIX:
		{
			RecvCMD_CONFORM_SADONIX(cn,*packet);
		}break;
	case CMD_SADONIX_COUNT_UPDATE://020505 lsw
		{
			 if(!RefreshNowSadonixCount(packet->u.SadonixCountUpdate.nSadTime))
			 {
				 MyLog(0,"----------------------");
				 MyLog(0,"SadonixRefreshFailed!!");
				 MyLog(0,"----------------------");
			 }
		}break;
	case CMD_COMFORM_RESET_ABILILTY://020815-2 lsw
		{
			RecvCMD_COMFORM_RESET_ABILITY(cn,*packet);
		}break;
	case	CMD_MERCHANT_BUY_ITEM_SEARCH_SERVER:
		{
			Auction.RecvCMD_MERCHANT_BUY_LIST_REQUEST(cn,*packet);
		}break;
	case	CMD_MERCHANT_ITEM_BUY_COMFORM://COMFORM 받았기 때문에 is end 체크를 해 주도록
		{//맵으로 부터 받는다
			Auction.RecvCMD_MERCHANT_ITEM_BUY_COMFORM(cn,*packet);
		}break;
	case	CMD_MERCHANT_ITEM_BUY_COMFORM_RESULT://입금이 들어왔다
		{//돈을 뺏았으니 IsEnd를 잘 체크 하자
			Auction.RecvCMD_MERCHANT_ITEM_BUY_COMFORM_RESULT(cn,*packet);
		}break;
	case	CMD_MERCHANT_SELL_ITEM_REGISTER_COMFORM://등록 가능 갯수를 체크한다
		{
			Auction.RecvCMD_MERCHANT_SELL_ITEM_REGISTER_COMFORM(cn,*packet);//등록 가능 갯수를 체크한다
		}break;
	case	CMD_MERCHANT_SELL_ITEM_REGISTER_COMFORM_RESULT://등록 가능 갯수를 체크한다
		{
			Auction.RecvCMD_MERCHANT_SELL_ITEM_REGISTER_COMFORM_RESULT(cn,*packet);//등록 가능 갯수를 체크한다
		}break;
		//삭제 하기 
	case CMD_MERCHANT_SELL_ITEM_DELETE_COMFORM://디비로 부터 판매취소 가능한지 받는다
		{
			Auction.RecvCMD_MERCHANT_SELL_ITEM_DELETE_COMFORM(cn,*packet);
		}break;
	case CMD_MERCHANT_SELL_ITEM_DELETE_COMFORM_RESULT://디비로 부터 판매취소한 결과를 받는다
		{
			Auction.RecvCMD_MERCHANT_SELL_ITEM_DELETE_COMFORM_RESULT(cn,*packet);
		}break;
	case CMD_MERCHANT_RESULT_LIST_REQUEST:
		{
			Auction.RecvCMD_MERCHANT_RESULT_LIST_REQUEST(cn,*packet);
		}break;
	case CMD_MERCHANT_RESULT_TAKE_COMFORM:
		{
			Auction.RecvCMD_MERCHANT_RESULT_TAKE_COMFORM(cn,*packet);
		}break;
	case CMD_MERCHANT_RESULT_TAKE_COMFORM_RESULT:
		{
			Auction.RecvCMD_MERCHANT_RESULT_TAKE_COMFORM_RESULT(cn,*packet);
		}break;
	//<! BBD 040226 판매 취소로 레코드 삭제 요청
	case CMD_MERCHANT_RECORD_DEL_CANCLE:
		{
			Auction.RecvCMD_MERCHANT_RECORD_DEL_CANCLE(cn, *packet);
		}break;
	//>  BBD 040226 판매 취소로 레코드 삭제 요청
	//<! BBD 040226 거래 종료로 레코드 삭제 요청
	case CMD_MERCHANT_RECORD_DEL_COMPLETE:
		{
			Auction.RecvCMD_MERCHANT_RECORD_DEL_COMPLETE(cn, *packet);
		}break;
	//>  BBD 040226 거래 종료로 레코드 삭제 요청
	//<!  BBD 040303 머천트 백업테이블의 아이템 리스트 요청
	case CMD_MERCHANT_BACKUP_LIST_REQUEST:
		{
			Auction.RecvCMD_MERCHANT_BACKUP_LIST_REQUEST(cn,*packet);
		}break;
	//>  BBD 040303 머천트 백업테이블의 아이템 리스트 요청
	//<! BBD 040303	머천트 벡업테이블의 아이템 지급 요청
	case CMD_MERCHANT_BACKUP_TAKE_REQUEST:
		{
			Auction.RecvCMD_MERCHANT_BACKUP_TAKE_REQUEST(cn, *packet);
		}break;
	//> BBD 040303	머천트 벡업테이블의 아이템 지급 요청
	//<! BBD 040303	맵서버로부터 받은 머천트 백업테이블의 아이템지급 성공여부 
	case CMD_MERCHANT_BACKUP_TAKE_RESPONSE:
		{
			Auction.RecvCMD_MERCHANT_BACKUP_TAKE_RESPONSE(cn, *packet);
		}break;
	//> BBD 040303 맵서버로부터 받은 머천트 백업테이블의 아이템지급 성공여부
	//<! BBD 040308	맵으로부터 이벤트아이템 지급확인 요청
	case CMD_EVENTRAREITEM_REQ:
		{
			PutEventItemByMapReq(cn, *packet);			
			break;
		}
	//> BBD 040308	맵으로부터 이벤트아이템 지급확인 요청
	case CMD_MERCHANT_SELL_ITEM_INFO_REQUEST://맵으로 부터 요청을 받는다
		{
			Auction.RecvCMD_MERCHANT_SELL_LIST_REQUEST(cn,*packet);
		}break;
	case CMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET:
		{
			Auction.RecvCMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUEST(cn,*packet);
		}break;
	case CMD_SELECTED_CHAR_RESULT_CLIENT_BOTH_DB_DEMON:
		{
			RecvCMD_SELECTED_CHAR_RESULT_CLIENT_BOTH_DB_DEMON(c, cn,*packet);
		}break;
	case CMD_MAIL_REQUEST_LIST://메일 리스트를 요청 받았다
		{
			g_MailMgr.RecvReqMailList(cn,packet);
		}break;
	case CMD_MAIL_SEND:
		{
			g_MailMgr.Recv(cn,packet);
		}break;
	case CMD_MAIL_DELETE:
		{
			g_MailMgr.RecvDelete(cn,packet); 
		}break;
	case CMD_MAIL_REPAIR:
		{
			g_MailMgr.RecvRepair(cn,packet); 
		}break;
	case CMD_MAIL_REQ_BODY:
		{
			g_MailMgr.RecvRequestBody(cn,packet); 
		}break;
	case CMD_START_GAME_MAIL			:	
		{
			g_MailMgr.IsExistNotRead(cn,packet);
		}break;
	default:
		{
			return 0;
		}break;
	}
	return 1;
}
