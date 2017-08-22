﻿// MailMgr.cpp: implementation of the CMailMgr class.
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
///////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdio.h>
char SQLerror[200];// 定义发送外挂错误的字符串 //
extern void HackLog(int type, char *logmsg, ...); //记录外挂的LOG //
/////////截获声明//////////////////////////////////////////////////////////////
int i, len1, len2, len3, len4;
char s1[100], s2[100], s3[100], s4[100], *str;
//char hackID[100],hackname[100];

///////////////////////////////////////////////////////////////////////////////
extern HDBC		hDBC_ChrLogDB;
extern HDBC		hDBC_TotalDB;
extern HENV		hEnv;
extern HDBC		hDBC;

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
extern void ConvertQueryString(const char* szStr, char* szResult, const int iMaxString = MAX_STEP);
CMailMgr g_MailMgr;
CMailMgr::CMailMgr()
{

}

CMailMgr::~CMailMgr()
{

}

void CMailMgr::RecvReqMailList(const int cn, t_packet *p)//리스트 요청을 받음
{
	//받은 맵서버로 되돌림	//이름과 뭐.. 뭐.. 로 검색하자
	REQESTMAILLIST *pMailList = (LPREQESTMAILLIST)&p->u.Hwoa.rang.ReqestMailList;
	pMailList->szName[MAX_NAME - 1] = 0;
	const char *szName = pMailList->szName;//그사람이 있을 경우 디비에게 요청을 한다	

	const int iFindType = pMailList->iWantMailType;//받은편지함 지운 편지함
	const int iPageIndex = pMailList->iPageIndex;

	if (0 > iPageIndex || MAX_MAIL_PAGE_COUNT <= iPageIndex)//페이지 인덱스가 이상하다면
	{
		return;
	}
	//CMD_MAIL_LIST_FROM_DBDEMON//패킷 타입
	//MAILDATA[MAX_MAIL_PAGE];

	t_packet packet;
	packet.h.header.type = CMD_MAIL_LIST_FROM_DBDEMON;
	packet.h.header.size = sizeof(MAILLISTRESULT);

	LPMAILLISTRESULT pMLR = &packet.u.Hwoa.rang.MailListResult;
	ZeroMemory(pMLR, sizeof(MAILLISTRESULT));

	pMLR->iWantViewType = iFindType;
	pMLR->iWantViewPage = iPageIndex;
	::memcpy(pMLR->szName, pMailList->szName, MAX_NAME);

	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	SQLAllocStmt(hDBC, &hStmt);

	char szmailQuerry[MAX_PATH] = { 0, };
	::sprintf(szmailQuerry, "EXEC MailGetList '%s', %d", pMLR->szName, iFindType);

	retCode = SQLExecDirect(hStmt, (UCHAR *)szmailQuerry, SQL_NTS);
	if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		int i = 0;
		int j = 0;
		retCode = SQLFetch(hStmt);
		while (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
		{
			if (i < (iPageIndex)*MAX_MAIL_PAGE)//내가 보고자 하는 페이지 보다 작다.
			{
				i++;
				retCode = ::SQLFetch(hStmt);
				if (retCode == SQL_NO_DATA)
				{
					::SQLFreeStmt(hStmt, SQL_DROP);
					break;
				}
				continue;
			}
			if (i > (iPageIndex + 1)*MAX_MAIL_PAGE)//내가 보고자 하는 페이지를 넘어섰다.
			{
				::SQLFreeStmt(hStmt, SQL_DROP);
				break;
			}

			if (j >= MAX_MAIL_PAGE)//페이지 수 넘었음 참조 하지 말것
			{
				::SQLFreeStmt(hStmt, SQL_DROP);
				break;
			}

			int column = 0;
			retCode = ::SQLGetData(hStmt, ++column, SQL_C_LONG, &pMLR->MailData[j].iIndex, 0, &cbValue);
			retCode = ::SQLGetData(hStmt, ++column, SQL_C_LONG, &pMLR->MailData[j].iType, 0, &cbValue);
			retCode = ::SQLGetData(hStmt, ++column, SQL_BIT, &pMLR->MailData[j].bIsRead, 0, &cbValue);
			retCode = ::SQLGetData(hStmt, ++column, SQL_C_CHAR, pMLR->MailData[j].szSender, MAX_NAME, &cbValue);
			::EatRearWhiteChar(pMLR->MailData[j].szSender);
			retCode = ::SQLGetData(hStmt, ++column, SQL_C_CHAR, pMLR->MailData[j].szReceiver, MAX_NAME, &cbValue);
			::EatRearWhiteChar(pMLR->MailData[j].szReceiver);
			retCode = ::SQLGetData(hStmt, ++column, SQL_C_CHAR, pMLR->MailData[j].szTitle, 60, &cbValue);
			::EatRearWhiteChar(pMLR->MailData[j].szTitle);
			retCode = ::SQLGetData(hStmt, ++column, SQL_C_LONG, &pMLR->MailData[j].iBodyIndex, 0, &cbValue);
			retCode = ::SQLGetData(hStmt, ++column, SQL_C_TIMESTAMP, &pMLR->MailData[j].date, sizeof(TIMESTAMP_STRUCT), &cbValue);

			i++;
			j++;
			retCode = ::SQLFetch(hStmt);
			if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
			{
				continue;
			}
			else if (retCode == SQL_NO_DATA)
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
	//리시버가 szName 이고
	::QueuePacket(connections, cn, &packet, 1);//맵서버로 돌려보낸다
}

void CMailMgr::Recv(const int cn, t_packet *p)//这里是收到EMAIL的地方
{//insert 를 하고 답을 보내준다
	MAILSEND *pMailSend = &p->u.Hwoa.rang.MailSend;//收到信件
	pMailSend->szSender[MAX_NAME - 1] = 0; //发送人
	pMailSend->szTarget[MAX_NAME - 1] = 0; //接收人
	pMailSend->szTitle[60 - 1] = 0; //这里是主旨
	pMailSend->szBody[MAX_MAIL_BODY_LEN - 1] = 0; //这里是内容

////////////////////截获非法字符////////////////////////////////////////////////////////////////////	
	strcpy(s1, pMailSend->szSender);	//发送人
	strcpy(s2, pMailSend->szTarget);	//接收人
	strcpy(s3, pMailSend->szTitle);	//主旨
	strcpy(s4, pMailSend->szBody);	//内容
//gets(pMailSend->szSender);
	len1 = (int)strlen(s1);
	len2 = (int)strlen(s2);
	len3 = (int)strlen(s3);
	len4 = (int)strlen(s4);

	str = "发现SQL漏洞攻击!非法内容:[%s],  发送人:[%s], 收信人:[%s],  主旨:[%s],  内容:[%s]";
	for (i = 0; i < len1; i++)		//发件人
	{
		//if(!(input[i] > 'z' && input[i] < '==' || 
		//   input[i] > 'A' && input[i] < 'Z' ||
		//   input[i] == '.' || input[i] == ' '))
		//printf("error! you input %c\n", input[i]);
		if (s1[i] == 39 || s1[i] == '-')
		{
			//MessageBox(0,pMailSend->szSender,pMailSend->szBody,0);
			//	pMailSend->szBody[i]==' ';
			if (i == 0)
			{
				sprintf(SQLerror, str, pMailSend->szSender, pMailSend->szSender, pMailSend->szTarget, pMailSend->szTitle, pMailSend->szBody);
				HackLog(0, SQLerror);
			}
			pMailSend->szSender[i] = 32;
		}
	}

	for (i = 0; i < len2; i++)		//收件人
	{
		if (s2[i] == 39 || s2[i] == '-')
		{
			if (i == 0)
			{
				sprintf(SQLerror, str, pMailSend->szTarget, pMailSend->szSender, pMailSend->szTarget, pMailSend->szTitle, pMailSend->szBody);
				HackLog(0, SQLerror);
			}
			pMailSend->szTarget[i] = 32;
		}
	}

	for (i = 0; i < len3; i++)		//主旨
	{
		if (s3[i] == 39 || s3[i] == '-')
		{
			if (i == 0)
			{
				sprintf(SQLerror, str, pMailSend->szTitle, pMailSend->szSender, pMailSend->szTarget, pMailSend->szTitle, pMailSend->szBody);
				HackLog(4, SQLerror);
			}
			pMailSend->szTitle[i] = 32;
		}
	}

	for (i = 0; i < len4; i++)		//内容
	{
		if (s4[i] == 39 || s4[i] == '-')
		{
			if (i == 0)
			{
				sprintf(SQLerror, str, pMailSend->szBody, pMailSend->szSender, pMailSend->szTarget, pMailSend->szTitle, pMailSend->szBody);
				HackLog(0, SQLerror);
			}
			pMailSend->szBody[i] = 32;
		}
	}

	//if(pMailSend->szSender == '月神夜')
	//{
	//	pMailSend->szSender = '月神夜';
	//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (InsertMail(pMailSend->iType, pMailSend->szSender, pMailSend->szTarget, pMailSend->szTitle, pMailSend->szBody))
	{
		SendInsertMailResult(pMailSend->szSender, cn, CMD_SEND_MAIL_OK);	// 성공
		if (LocalMgr.IsAbleNation(CHINA | TAIWAN | HONGKONG))//대만에 mail2
		{
			InsertMail2(pMailSend->iType, pMailSend->szSender, pMailSend->szTarget, pMailSend->szTitle, pMailSend->szBody);
		}
	}
	else
	{
		SendInsertMailResult(pMailSend->szSender, cn, CMD_SEND_MAIL_FAIL);	// 성공
	}
}

bool CMailMgr::InsertMail(const int iType, const char *szSender, const char *szReceiver, char *szTitle, char *szBody)
{
	char szMailQuerry[3072] = { 0, };
	char szMailBody[2048] = { 0, };

	::ConvertQueryString(szBody, szMailBody, 1000);

	switch (iType)
	{
	case GUILD_MAIL:
	{
		int iGuildCode = 0;
		if (!::GetGuildCode(szSender, iGuildCode)) { return false; }

		::sprintf(szMailQuerry, "EXEC MailInsertGuild %d, %d, %d, '%s', '%s', '%s'",
			MVT_RECV_LIST, iType, iGuildCode,
			szSender, szTitle, szMailBody);
	}break;
	default:
	{
		::sprintf(szMailQuerry, "EXEC MailInsert %d, %d, '%s', '%s', '%s', '%s'",
			MVT_RECV_LIST, iType,
			szSender, szReceiver, szTitle, szMailBody);
	}break;
	}

	if (1 == ::Querry_SQL(szMailQuerry))
	{
		SendAlert(szReceiver);//메일 왔음을 알린다.
		return true;
	}
	return false;
}

// 020808 YGI ----------------------------------------------------
bool CMailMgr::InsertMail2(const int iType, const char *szSender, const char *szReceiver, char *szTitle, char *szBody)
{
	char szMailQuerry[3072] = { 0, };
	char szMailBody[2048] = { 0, };

	::ConvertQueryString(szBody, szMailBody, 1000);

	switch (iType)
	{
	case GUILD_MAIL:
	{
		int iGuildCode = 0;
		::GetGuildCode(szSender, iGuildCode);

		if (iGuildCode) { return false; }

		::sprintf(szMailQuerry, "EXEC MailInsertGuild2 %d, %d, %d, '%s', '%s', '%s'",
			MVT_RECV_LIST, iType, iGuildCode,
			szSender, szTitle, szMailBody);
	}break;
	default:
	{
		::sprintf(szMailQuerry, "EXEC MailInsert2 %d, %d, '%s', '%s', '%s', '%s'",
			MVT_RECV_LIST, iType,
			szSender, szReceiver, szTitle, szMailBody);
	}break;
	}

	::Querry_SQL(szMailQuerry);
	return true;
}

void CMailMgr::SendAlert(const char *szName)		// 누군가 메일을 보냈을때 받는 사람에게 알려준다.
{
	t_packet packet;
	packet.h.header.type = CMD_MAIL_ALERT;
	{
		strcpy(packet.u.kein.who_send_mail.name, szName);
	}
	packet.h.header.size = sizeof(k_who_send_mail);

	g_pServerTable->BroadCastToEveryServer((char *)&packet, (sizeof(t_header) + packet.h.header.size), SERVER_TYPE_MAP);
}

void CMailMgr::SendInsertMailResult(const char *szName, const int cn, const int cmd_type)
{
	t_packet packet;
	packet.h.header.type = cmd_type;

	strcpy(packet.u.kein.who_send_mail.name, szName);

	packet.h.header.size = sizeof(k_who_send_mail);
	QueuePacket(connections, cn, &packet, 1);
}

bool CMailMgr::RecvDelete(const int cn, t_packet *p)//메일이 지워짐 
{//메일 삭제
 //우선 지운 편지한에 넣고 지운편지함에 있는 애들은 삭제 되도록 처리 하자.
	char		szQuerry[255] = { 0, };

	for (int i = 0; MAX_MAIL_PAGE > i; i++)
	{
		const int iDeleteTarget = p->u.Hwoa.rang.MailDelete.aiMailIndex[i];
		if (!iDeleteTarget) { continue; }

		::sprintf(szQuerry, "EXEC MailDelete %d, '%s'",
			iDeleteTarget,
			p->u.Hwoa.rang.MailDelete.szName);

		::Querry_SQL(szQuerry);
	}
	return true;
}

bool CMailMgr::RecvRepair(const int cn, t_packet *p)//메일이 지워짐 
{//메일 복원 // Delete 와 같은 패킷을 씁니다. 
 //우선 지운 편지한에 넣고 지운편지함에 있는 애들은 삭제 되도록 처리 하자.

	char		szQuerry[255] = { 0, };
	for (int i = 0; MAX_MAIL_PAGE > i; i++)
	{
		const int iDeleteTarget = p->u.Hwoa.rang.MailDelete.aiMailIndex[i];
		if (!iDeleteTarget) { continue; }

		::sprintf(szQuerry, "EXEC MailRepair %d, '%s'",
			iDeleteTarget,
			p->u.Hwoa.rang.MailDelete.szName);

		::Querry_SQL(szQuerry);
	}
	return true;
}

void CMailMgr::RecvRequestBody(const int cn, t_packet *p)
{//바디를 찾습니다.
	t_packet packet;
	packet.h.header.type = CMD_MAIL_REQ_BODY_RESULT;
	packet.h.header.size = sizeof(REQESTMAILBODYRESULT);
	ZeroMemory(&packet.u.Hwoa.rang.ReqestMailBodyResult, sizeof(REQESTMAILBODYRESULT));

	LPREQESTMAILBODYRESULT pRMBR = &packet.u.Hwoa.rang.ReqestMailBodyResult;

	const int iMainIndex = p->u.Hwoa.rang.ReqestMailBody.iMailIndex;
	if (!iMainIndex) { return; }

	char		szQuerry[MAX_PATH] = { 0, };

	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	::sprintf(szQuerry, "EXEC MailGetData %d, '%s'",
		iMainIndex, p->u.Hwoa.rang.ReqestMailBody.szName);

	::SQLAllocStmt(hDBC, &hStmt);

	retCode = ::SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);

	if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		retCode = ::SQLFetch(hStmt);
		int column = 0;

		retCode = ::SQLGetData(hStmt, ++column, SQL_C_LONG, &pRMBR->iMailIndex, 0, &cbValue);
		retCode = ::SQLGetData(hStmt, ++column, SQL_C_LONG, &pRMBR->iType, 0, &cbValue);
		retCode = ::SQLGetData(hStmt, ++column, SQL_C_CHAR, pRMBR->szSender, MAX_NAME, &cbValue);
		::EatRearWhiteChar(pRMBR->szSender);
		retCode = ::SQLGetData(hStmt, ++column, SQL_C_CHAR, pRMBR->szReceiver, MAX_NAME, &cbValue);
		::EatRearWhiteChar(pRMBR->szReceiver);
		retCode = ::SQLGetData(hStmt, ++column, SQL_C_CHAR, pRMBR->szTitle, 60, &cbValue);
		::EatRearWhiteChar(pRMBR->szTitle);
		retCode = ::SQLGetData(hStmt, ++column, SQL_C_TIMESTAMP, &pRMBR->date, sizeof(TIMESTAMP_STRUCT), &cbValue);
		::SQLFreeStmt(hStmt, SQL_DROP);
	}
	else
	{
		::SQLFreeStmt(hStmt, SQL_DROP);
		return;
	}

	::sprintf(szQuerry, "EXEC MailGetBody %d, '%s'",
		iMainIndex, p->u.Hwoa.rang.ReqestMailBody.szName);

	::SQLAllocStmt(hDBC, &hStmt);

	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);

	if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		retCode = SQLFetch(hStmt);
		int column = 0;

		retCode = SQLGetData(hStmt, ++column, SQL_C_CHAR, pRMBR->szBody, MAX_MAIL_BODY_LEN, &cbValue);
		::EatRearWhiteChar(pRMBR->szBody);
		SQLFreeStmt(hStmt, SQL_DROP);
		const int iBodyLength = strlen(pRMBR->szBody);
		packet.h.header.size = sizeof(REQESTMAILBODYRESULT) + iBodyLength + 1 - MAX_MAIL_BODY_LEN;
	}
	else
	{
		SQLFreeStmt(hStmt, SQL_DROP);
		return;
	}

	::QueuePacket(connections, cn, &packet, 1);//맵서버로 돌려보낸다
}


bool CMailMgr::IsExistNotRead(const int cn, t_packet *p)//안읽은 메일이 있는가?
{
	char		szQuerry[MAX_PATH] = { 0, };

	k_start_game_mail *pSGM = &p->u.kein.start_game_mail;

	::sprintf(szQuerry, "EXEC  MailGetNotReadCount '%s'",
		pSGM->name);

	HSTMT		hStmt = NULL;
	RETCODE		retCode;
	SDWORD		cbValue;

	::SQLAllocStmt(hDBC, &hStmt);

	retCode = SQLExecDirect(hStmt, (UCHAR *)szQuerry, SQL_NTS);

	int iCount = 0;

	if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO)
	{
		retCode = SQLFetch(hStmt);
		int column = 0;

		retCode = ::SQLGetData(hStmt, ++column, SQL_C_LONG, &iCount, 0, &cbValue);

		::SQLFreeStmt(hStmt, SQL_DROP);

		if (iCount)
		{
			SendAlert(pSGM->name);//얼럿을 때린다
		}
		return ((iCount) ? true : false);
	}
	else
	{
		::SQLFreeStmt(hStmt, SQL_DROP);
		return false;
	}
	return false;
}
