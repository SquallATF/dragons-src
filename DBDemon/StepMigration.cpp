// StepMigration.cpp: implementation of the CStepMigration class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepMigration.h"

#include "MyLog.h"
#include "Main.h"
#include "CharList.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CStepMigration::CStepMigration()
{
	m_vtBuffer.clear();
}

CStepMigration::~CStepMigration()
{
	m_vtBuffer.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CStepMigration::Load(unsigned char* pData)
{	// µ¥ÀÌÅ¸º£ÀÌ½º¿¡¼­ ¹®ÀÚ¿­ ÀúÀåÀº ¼ýÀÚ 0À» NULL·Î ÀÎ½ÄÇÏ¹Ç·Î 1¾¿ »©¼­ ·Îµå
	unsigned char aBuffer[MAX_STEP];
	memcpy(aBuffer, pData, MAX_STEP);

	for (int i = 0; i < MAX_STEP; ++i)
	{
		*(pData + i) -= 1;
	}

	memcpy(aBuffer, pData, MAX_STEP);
	CheckStepInfo(pData);
}

void CStepMigration::Save(unsigned char* pData)
{	// µ¥ÀÌÅ¸º£ÀÌ½º¿¡¼­ ¹®ÀÚ¿­ ÀúÀåÀº ¼ýÀÚ 0À» NULL·Î ÀÎ½ÄÇÏ¹Ç·Î 1¾¿ ´õÇØ¼­ ÀúÀå
	CheckStepInfo(pData);

	unsigned char aBuffer[MAX_STEP];
	memcpy(aBuffer, pData, MAX_STEP);

	for (int i = 0; i < MAX_STEP; ++i)
	{
		*(pData + i) += 1;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

void CStepMigration::CheckStepInfo(unsigned char* pData)
{	// Ä³¸¯ÅÍÀÇ ´Ü°è Á¤º¸ °Ë»ç
	m_vtBuffer.assign(pData, pData + MAX_STEP);
	
	CheckClassStep(CLS_STEP);
	CheckDualClass(DUAL_CLS);
	CheckCombatPoint(CSP_STEP);
	CheckExpStep(EXP_STEP);

	for (int i = T01_STEP; i <= T13_STEP; ++i)
	{
		CheckTacStep(i);
	}

	copy(m_vtBuffer.begin(), m_vtBuffer.end(), pData);
	*(pData + MAX_STEP) = 0;

	m_vtBuffer.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

void CStepMigration::CheckClassStep(int nStepType)
{	// ×ªÖ°ÉèÖÃ
	const unsigned char cStep = m_vtBuffer[nStepType];

	if (cStep >= 0 && cStep <= 6)   //±£´æ6×ªÐÅÏ¢
	{
		return;
	}

	MyLog(LOG_NORMAL, "[%d]Class Step : %d", nStepType, cStep);
	m_vtBuffer[nStepType] = 0;
}

void CStepMigration::CheckDualClass(int nStepType)
{	// µà¾ó Å¬·¡½º Á¾·ù °Ë»ç
	const unsigned char cStep = m_vtBuffer[nStepType];

	if (cStep >= WARRIOR && cStep <= PRIEST)
	{
		return;
	}

	MyLog(LOG_NORMAL, "[%d]Dual Class : %d", nStepType, cStep);
	m_vtBuffer[nStepType] = WARRIOR;
}

void CStepMigration::CheckExpStep(int nStepType)
{	// °æÇèÄ¡ ´Ü°è °Ë»ç
	const unsigned char cStep = m_vtBuffer[nStepType];

	if (cStep >= 0 && cStep <= NPC_Lev_Ref[MAX_EXP_LEVEL].nStep)
	{	// µ¥ÀÌÅ¸º£ÀÌ½º¿¡¼­ ÀúÀåÀÌ µÇÁö ¾Ê´Â ¹®ÀÚ
		switch (cStep)
		{
		case 31:
			{
				m_vtBuffer[nStepType] = 32;
				break;
			}
		case 38:
			{
				m_vtBuffer[nStepType] = 39;
				break;
			}
		}
		
		return;
	}

	MyLog(LOG_NORMAL, "[%d]Experience Step : %d", nStepType, cStep);
	m_vtBuffer[nStepType] = 0;
}

void CStepMigration::CheckTacStep(int nStepType)
{	// °æÇèÄ¡ ´Ü°è °Ë»ç
	const unsigned char cStep = m_vtBuffer[nStepType];
	
	if (cStep >= 0 && cStep <= NPC_Lev_Ref[MAX_TAC_LEVEL].nStep)
	{	// µ¥ÀÌÅ¸º£ÀÌ½º¿¡¼­ ÀúÀåÀÌ µÇÁö ¾Ê´Â ¹®ÀÚ
		switch (cStep)
		{
		case 31:
			{
				m_vtBuffer[nStepType] = 32;
				break;
			}
		case 38:
			{
				m_vtBuffer[nStepType] = 39;
				break;
			}
		}
		
		return;
	}

	MyLog(LOG_NORMAL, "[%d]Tactic Step : %d", nStepType, cStep);
	m_vtBuffer[nStepType] = 0;
}

void CStepMigration::CheckCombatPoint(int nStepType)
{	// ÀüÅõ½ºÅ³ Æ÷ÀÎÅÍ °Ë»ç
	const unsigned char cStep = m_vtBuffer[nStepType];

	if (cStep >= 0 && cStep <= NPC_Lev_Ref[MAX_EXP_LEVEL].nCspMax)
	{
		return;
	}

	MyLog(LOG_NORMAL, "[%d]Combat Point : %d", nStepType, cStep);
	//m_vtBuffer[nStepType] = 0;
}