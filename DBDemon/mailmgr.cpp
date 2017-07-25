// MailMgr.cpp: implementation of the CMailMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
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
#include "MailMgr.h"
#include "MySQL.h"
////////////////////////////////////////////////////////////
#include <string.h>
#include <stdio.h>
char SQLerror[200];/////////////////////////////////////定义发送外挂错误的字符串
extern void HackLog( int type, char *logmsg, ... );//记录外挂的LOG
////////////////////////////////////截获声明
int i,len1,len2,len3,len4;
char s1[100],s2[100],s3[100],s4[100],*str;
//char hackID[100],hackname[100];
///////////////////////////////////
////////////////////////////////////////////////////////////
extern HDBC		hDBC_ChrLogDB;
extern HDBC		hDBC_TotalDB;
extern HENV		hEnv ;
extern HDBC		hDBC ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern void ConvertQueryString(const char* szStr, char* szResult, const int iMaxString = MAX_STEP);
CMailMgr g_MailMgr;
CMailMgr::CMailMgr()
{

}

CMailMgr::~CMailMgr()
{

}

void CMailMgr::RecvReqMailList(const int cn,t_packet *p)//府胶飘 夸没阑 罐澜
{
	//罐篮 甘辑滚肺 登倒覆	//捞抚苞 构.. 构.. 肺 八祸窍磊
	REQESTMAILLIST *pMailList = (LPREQESTMAILLIST)&p->u.Hwoa.rang.ReqestMailList;
	pMailList->szName[MAX_NAME-1] = 0;
	const char *szName = pMailList->szName;//弊荤恩捞 乐阑 版快 叼厚俊霸 夸没阑 茄促	

	const int iFindType = pMailList->iWantMailType;//罐篮祈瘤窃 瘤款 祈瘤窃
	const int iPageIndex = pMailList->iPageIndex;

	if(0 > iPageIndex || MAX_MAIL_PAGE_COUNT <= iPageIndex)//其捞瘤 牢郸胶啊 捞惑窍促搁
	{
		return;
	}
	//	CMD_MAIL_LIST_FROM_DBDEMON//菩哦 鸥涝
//	MAILDATA[MAX_MAIL_PAGE];
	
	t_packet packet;
	packet.h.header.type = CMD_MAIL_LIST_FROM_DBDEMON;
	packet.h.header.size = sizeof(MAILLISTRESULT);

	LPMAILLISTRESULT pMLR = &packet.u.Hwoa.rang.MailListResult;
	ZeroMemory(pMLR,sizeof(MAILLISTRESULT));

	pMLR->iWantViewType = iFindType;
	pMLR->iWantViewPage = iPageIndex;
	::memcpy(pMLR->szName, pMailList->szName, MAX_NAME);
	
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	SQLAllocStmt(hDBC, &hStmt);
	
	char szmailQuerry[MAX_PATH] = {0,};
	::sprintf(szmailQuerry, "EXEC MailGetList '%s', %d", pMLR->szName,iFindType);
	
	retCode = SQLExecDirect(hStmt, (UCHAR *)szmailQuerry, SQL_NTS);
	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		int i = 0;
		int j = 0;
		retCode = SQLFetch(hStmt);
		while( retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			if(i < (iPageIndex)*MAX_MAIL_PAGE)//郴啊 焊绊磊 窍绰 其捞瘤 焊促 累促.
			{
				i++;
				retCode = ::SQLFetch(hStmt);
				if( retCode == SQL_NO_DATA ) 
				{
					::SQLFreeStmt(hStmt, SQL_DROP);
					break;
				}
				continue;
			}
			if(i > (iPageIndex+1)*MAX_MAIL_PAGE)//郴啊 焊绊磊 窍绰 其捞瘤甫 逞绢脊促.
			{
				::SQLFreeStmt(hStmt, SQL_DROP);
				break;
			}

			if(j >= MAX_MAIL_PAGE)//其捞瘤 荐 逞菌澜 曼炼 窍瘤 富巴
			{
				::SQLFreeStmt(hStmt, SQL_DROP);
				break;
			}

			int column = 0;
			retCode = ::SQLGetData(hStmt, ++column, SQL_C_LONG,	&pMLR->MailData[j].iIndex,		0, &cbValue);
			retCode = ::SQLGetData(hStmt, ++column, SQL_C_LONG,	&pMLR->MailData[j].iType,		0, &cbValue);
			retCode = ::SQLGetData(hStmt, ++column, SQL_BIT	,	&pMLR->MailData[j].bIsRead,		0, &cbValue);
			retCode = ::SQLGetData(hStmt, ++column, SQL_C_CHAR,	pMLR->MailData[j].szSender,		MAX_NAME, &cbValue);
			::EatRearWhiteChar(pMLR->MailData[j].szSender);
			retCode = ::SQLGetData(hStmt, ++column, SQL_C_CHAR,	pMLR->MailData[j].szReceiver,	MAX_NAME, &cbValue);
			::EatRearWhiteChar(pMLR->MailData[j].szReceiver);
			retCode = ::SQLGetData(hStmt, ++column, SQL_C_CHAR,	pMLR->MailData[j].szTitle,		60, &cbValue);
			::EatRearWhiteChar(pMLR->MailData[j].szTitle);
			retCode = ::SQLGetData(hStmt, ++column, SQL_C_LONG,	&pMLR->MailData[j].iBodyIndex,	0, &cbValue);
			retCode = ::SQLGetData(hStmt, ++column, SQL_C_TIMESTAMP, &pMLR->MailData[j].date,	sizeof( TIMESTAMP_STRUCT ), &cbValue);

			i++;
			j++;
			retCode = ::SQLFetch(hStmt);
			if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
				continue;
			}
			else if( retCode == SQL_NO_DATA ) 
			{
				::SQLFreeStmt(hStmt, SQL_DROP);
				break;
			}
			else
			{
				::SQLFreeStmt(hStmt, SQL_DROP);
				return;
			}
		}	
	}		
	else 
	{
		::SQLFreeStmt(hStmt, SQL_DROP);
		return;
	}
	//府矫滚啊 szName 捞绊
	::QueuePacket(connections,cn,&packet,1);//甘辑滚肺 倒妨焊辰促
}

void CMailMgr::Recv(const int cn, t_packet *p)//这里是收到EMAIL的地方
{//insert 甫 窍绊 翠阑 焊郴霖促


	MAILSEND *pMailSend = &p->u.Hwoa.rang.MailSend;//收到信件
	pMailSend->szSender[MAX_NAME-1]			= 0; //发送人
	pMailSend->szTarget[MAX_NAME-1]			= 0; //接收人
	pMailSend->szTitle[60-1]				= 0; //这里是主旨
	pMailSend->szBody[MAX_MAIL_BODY_LEN-1]	= 0; //这里是内容

////////////////////////////////////////////////////////////////////////////////////////截获非法字符	
	strcpy(s1,pMailSend->szSender);	//发送人
	strcpy(s2,pMailSend->szTarget);	//接收人
	strcpy(s3,pMailSend->szTitle);	//主旨
	strcpy(s4,pMailSend->szBody);	//内容
//gets(pMailSend->szSender);
len1 = (int)strlen(s1);
len2 = (int)strlen(s2);
len3 = (int)strlen(s3);
len4 = (int)strlen(s4);

str="发现SQL漏洞攻击!非法内容:[%s],  发送人:[%s], 收信人:[%s],  主旨:[%s],  内容:[%s]";
for(i = 0; i < len1; i++)		//发件人
{
/*   if(!(input[i] > 'z' && input[i] < '==' || 
      input[i] > 'A' && input[i] < 'Z' ||
      input[i] == '.' || input[i] == ' '))*/
 //     printf("error! you input %c\n", input[i]);
	if(s1[i]==39 || s1[i]=='-')
	{
//	MessageBox(0,pMailSend->szSender,pMailSend->szBody,0);
//		pMailSend->szBody[i]==' ';
		if(i==0)
		{
		sprintf(SQLerror,str,pMailSend->szSender,pMailSend->szSender,pMailSend->szTarget,pMailSend->szTitle,pMailSend->szBody);
		HackLog(0,SQLerror);
		}
	pMailSend->szSender[i] = 32;
	}
} 

for(i = 0; i < len2; i++)		//收件人
{
	if(s2[i]==39 || s2[i]=='-')
	{
		if(i==0)
		{
		sprintf(SQLerror,str,pMailSend->szTarget,pMailSend->szSender,pMailSend->szTarget,pMailSend->szTitle,pMailSend->szBody);
		HackLog(0,SQLerror);
		}
	pMailSend->szTarget[i] = 32;
	}
} 

for(i = 0; i < len3; i++)		//主旨
{
	if(s3[i]==39 || s3[i]=='-')
	{
		if(i==0)
		{
		sprintf(SQLerror,str,pMailSend->szTitle,pMailSend->szSender,pMailSend->szTarget,pMailSend->szTitle,pMailSend->szBody);
		HackLog(4,SQLerror);
		}
	pMailSend->szTitle[i] = 32;
	}
} 

for(i = 0; i < len4; i++)		//内容
{
	if(s4[i]==39 || s4[i]=='-')
	{
		if(i==0)
		{
		sprintf(SQLerror,str,pMailSend->szBody,pMailSend->szSender,pMailSend->szTarget,pMailSend->szTitle,pMailSend->szBody);
		HackLog(0,SQLerror);
		}
	pMailSend->szBody[i] = 32;
	}
} 

 /*   if(pMailSend->szSender == '月神夜')
	{
		pMailSend->szSender = '月神夜';
	}*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if( InsertMail( pMailSend->iType, pMailSend->szSender ,pMailSend->szTarget, pMailSend->szTitle, pMailSend->szBody ) ) 
	{
		SendInsertMailResult(  pMailSend->szSender, cn, CMD_SEND_MAIL_OK );	// 己傍
		if(LocalMgr.IsAbleNation(CHINA|TAIWAN|HONGKONG))//措父俊 mail2
		{
			InsertMail2( pMailSend->iType, pMailSend->szSender ,pMailSend->szTarget, pMailSend->szTitle, pMailSend->szBody );
		}
	}
	else 
	{
		SendInsertMailResult(  pMailSend->szSender, cn, CMD_SEND_MAIL_FAIL );	// 己傍
	}
}

bool CMailMgr::InsertMail( const int iType, const char *szSender, const char *szReceiver, char *szTitle, char *szBody )
{		
	char szMailQuerry[3072] = {0,};
	char szMailBody[2048] = {0,};
	
	::ConvertQueryString(szBody,szMailBody,1000);

	switch(iType)
	{
	case GUILD_MAIL:
		{
			int iGuildCode = 0;
			if(!::GetGuildCode(szSender, iGuildCode)){return false;}
		
			::sprintf(szMailQuerry, "EXEC MailInsertGuild %d, %d, %d, '%s', '%s', '%s'",
				MVT_RECV_LIST,	iType,	iGuildCode,
				szSender, szTitle, szMailBody );
		}break;
	default:
		{
			::sprintf(szMailQuerry, "EXEC MailInsert %d, %d, '%s', '%s', '%s', '%s'",
				MVT_RECV_LIST,	iType,	
				szSender, szReceiver, szTitle, szMailBody );
		}break;
	}

	if( 1 == ::Querry_SQL( szMailQuerry ))
	{
		SendAlert(szReceiver);//皋老 吭澜阑 舅赴促.
		return true;
	}
	return false;
}

// 020808 YGI ----------------------------------------------------
bool CMailMgr::InsertMail2(const int iType, const char *szSender, const char *szReceiver, char *szTitle, char *szBody )
{	
	char szMailQuerry[3072] = {0,};
	char szMailBody[2048] = {0,};
	
	::ConvertQueryString(szBody,szMailBody,1000);

	switch(iType)
	{
	case GUILD_MAIL:
		{
			int iGuildCode = 0;
			::GetGuildCode(szSender, iGuildCode);

			if(iGuildCode){return false;}
		
			::sprintf(szMailQuerry, "EXEC MailInsertGuild2 %d, %d, %d, '%s', '%s', '%s'",
				MVT_RECV_LIST,	iType,	iGuildCode,
				szSender, szTitle, szMailBody );
		}break;
	default:
		{
			::sprintf(szMailQuerry, "EXEC MailInsert2 %d, %d, '%s', '%s', '%s', '%s'",
				MVT_RECV_LIST,	iType,	
				szSender, szReceiver, szTitle, szMailBody );
		}break;
	}

	::Querry_SQL( szMailQuerry );
	return true;
}

void CMailMgr::SendAlert( const char *szName )		// 穿焙啊 皋老阑 焊陈阑锭 罐绰 荤恩俊霸 舅妨霖促.
{
	t_packet packet;
	packet.h.header.type = CMD_MAIL_ALERT;
	{
		strcpy( packet.u.kein.who_send_mail.name, szName );
	}
	packet.h.header.size = sizeof( k_who_send_mail );
	
	g_pServerTable->BroadCastToEveryServer( (char *)&packet, (sizeof(t_header)+packet.h.header.size ), SERVER_TYPE_MAP );
}

void CMailMgr::SendInsertMailResult( const char *szName, const int cn, const int cmd_type )
{
	t_packet packet;
	packet.h.header.type = cmd_type;
	
	strcpy( packet.u.kein.who_send_mail.name, szName );
	
	packet.h.header.size = sizeof(k_who_send_mail) ;
	QueuePacket( connections, cn, &packet, 1 );
}

bool CMailMgr::RecvDelete(const int cn, t_packet *p)//皋老捞 瘤况咙 
{//皋老 昏力
	//快急 瘤款 祈瘤茄俊 持绊 瘤款祈瘤窃俊 乐绰 局甸篮 昏力 登档废 贸府 窍磊.
	char		szQuerry[255]= {0,};

	for (int i = 0; MAX_MAIL_PAGE>i; i++)
	{
		const int iDeleteTarget = p->u.Hwoa.rang.MailDelete.aiMailIndex[i];
		if(!iDeleteTarget){continue;}

		::sprintf(szQuerry, "EXEC MailDelete %d, '%s'",
			iDeleteTarget,
			p->u.Hwoa.rang.MailDelete.szName);
		
		::Querry_SQL( szQuerry );
	}
	return true;
}

bool CMailMgr::RecvRepair(const int cn, t_packet *p)//皋老捞 瘤况咙 
{//皋老 汗盔 // Delete 客 鞍篮 菩哦阑 竟聪促. 
	//快急 瘤款 祈瘤茄俊 持绊 瘤款祈瘤窃俊 乐绰 局甸篮 昏力 登档废 贸府 窍磊.
	
	char		szQuerry[255]= {0,};
	for (int i = 0; MAX_MAIL_PAGE>i; i++)
	{
		const int iDeleteTarget = p->u.Hwoa.rang.MailDelete.aiMailIndex[i];
		if(!iDeleteTarget){continue;}

		::sprintf(szQuerry, "EXEC MailRepair %d, '%s'",
			iDeleteTarget,
			p->u.Hwoa.rang.MailDelete.szName);
		
		::Querry_SQL( szQuerry );
	}
	return true;
}

void CMailMgr::RecvRequestBody(const int cn,t_packet *p)
{//官叼甫 茫嚼聪促.
	t_packet packet;
	packet.h.header.type = CMD_MAIL_REQ_BODY_RESULT;
	packet.h.header.size = sizeof(REQESTMAILBODYRESULT);
	ZeroMemory(&packet.u.Hwoa.rang.ReqestMailBodyResult,sizeof(REQESTMAILBODYRESULT));

	LPREQESTMAILBODYRESULT pRMBR= &packet.u.Hwoa.rang.ReqestMailBodyResult;
		 
	const int iMainIndex = p->u.Hwoa.rang.ReqestMailBody.iMailIndex;
	if(!iMainIndex){return;}
	
	char		szQuerry[MAX_PATH] = {0,};
	
	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	::sprintf(szQuerry, "EXEC MailGetData %d, '%s'",
	iMainIndex,	p->u.Hwoa.rang.ReqestMailBody.szName);

	::SQLAllocStmt(hDBC, &hStmt);

	retCode = ::SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);

	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		retCode = ::SQLFetch(hStmt);
		int column = 0;
	
		retCode = ::SQLGetData(hStmt, ++column, SQL_C_LONG,	&pRMBR->iMailIndex,	0, &cbValue);
		retCode = ::SQLGetData(hStmt, ++column, SQL_C_LONG,	&pRMBR->iType,		0, &cbValue);
		retCode = ::SQLGetData(hStmt, ++column, SQL_C_CHAR,	pRMBR->szSender,	MAX_NAME, &cbValue);
		::EatRearWhiteChar(pRMBR->szSender);
		retCode = ::SQLGetData(hStmt, ++column, SQL_C_CHAR,	pRMBR->szReceiver,	MAX_NAME, &cbValue);
		::EatRearWhiteChar(pRMBR->szReceiver);
		retCode = ::SQLGetData(hStmt, ++column, SQL_C_CHAR,	pRMBR->szTitle,		60, &cbValue);
		::EatRearWhiteChar(pRMBR->szTitle);
		retCode = ::SQLGetData(hStmt, ++column, SQL_C_TIMESTAMP, &pRMBR->date,	sizeof( TIMESTAMP_STRUCT ), &cbValue);
		::SQLFreeStmt(hStmt, SQL_DROP);
	}
	else 
	{
		::SQLFreeStmt(hStmt, SQL_DROP);
		return;
	}
	
	::sprintf(szQuerry, "EXEC MailGetBody %d, '%s'",
	iMainIndex,	p->u.Hwoa.rang.ReqestMailBody.szName);	

	::SQLAllocStmt(hDBC, &hStmt);

	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);

	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		retCode = SQLFetch(hStmt);
		int column = 0;
	
		retCode = SQLGetData(hStmt, ++column, SQL_C_CHAR,	pRMBR->szBody,		MAX_MAIL_BODY_LEN, &cbValue);
		::EatRearWhiteChar(pRMBR->szBody);
		SQLFreeStmt(hStmt, SQL_DROP);
		const int iBodyLength = strlen(pRMBR->szBody);
		packet.h.header.size = sizeof(REQESTMAILBODYRESULT)+iBodyLength+1-MAX_MAIL_BODY_LEN;
	}
	else 
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return;
	}

	::QueuePacket(connections,cn,&packet,1);//甘辑滚肺 倒妨焊辰促
}


bool CMailMgr::IsExistNotRead(const int cn,t_packet *p)//救佬篮 皋老捞 乐绰啊?
{
	char		szQuerry[MAX_PATH] = {0,};

	k_start_game_mail *pSGM = &p->u.kein.start_game_mail;
	
	::sprintf(szQuerry, "EXEC  MailGetNotReadCount '%s'",
	pSGM->name);

	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	::SQLAllocStmt(hDBC, &hStmt);

	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);

	int iCount = 0;

	if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{		
		retCode = SQLFetch(hStmt);
		int column = 0;
		
		retCode = ::SQLGetData(hStmt, ++column, SQL_C_LONG, &iCount, 0, &cbValue);
		
		::SQLFreeStmt(hStmt, SQL_DROP);
		
		if(iCount)
		{
			SendAlert(pSGM->name);//倔返阑 锭赴促
		}
		return ((iCount)?true:false);
	}
	else 
	{
		::SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	return false;
}
