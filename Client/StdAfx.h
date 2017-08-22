﻿// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
// 공유 라이브러리
#include "../Library/Shared/HSEL.h"
#include "../Library/Shared/Shared.h"
#ifdef _DEBUG
#pragma comment (lib, "../Library/HSELD.lib")
#pragma comment (lib, "../Library/SharedD.lib")
#else
#pragma comment (lib, "../Library/HSEL.lib")
#pragma comment (lib, "../Library/Shared.lib")
#endif // !_DEBUG

//////////////////////////////////////////////////010730 lms
#define TAIWAN_LOCALIZING_
//#define HONGKONG_LOCALIZING_
//#define CHINA_LOCALIZING_
//#define USA_LOCALIZING_
//#define THAI_LOCALIZING_
//#define KOREA_LOCALIZING_
//#define JAPAN_LOCALIZING_
//#define MALAYSIA_LOCALIZING_ 

// 030930 kyo<<
#ifdef MALAYSIA_LOCALIZING_ 
#define WAIT_TIME_RESPONSE_DURATION	20000
#else
#define WAIT_TIME_RESPONSE_DURATION	10000
#endif
// 030930 kyo >>

#ifdef CHINA_LOCALIZING_
#ifndef _DEBUG
#define _NO_COMMAND_RESET_ABILITY
//		#define ALT_TAB_BLOCK // CSD-TW-030606
#endif
#endif
// thai2 YGI
#ifdef THAI_LOCALIZING_
#define USA_LOCALIZING_
#endif

// malaysia // 030729 kyo
#ifdef MALAYSIA_LOCALIZING_ 
#define USA_LOCALIZING_
#endif

//#define LIGHT_VERSION_		// 021008 YGI

// TODO: reference additional headers your program requires here
#include <MMSYSTEM.H>

#include "Debuglog.h"
#include "MapInfo.h"

#include "MyTypes.h"
#include "MenuDef.h"
#include "SqlTypes.h"
#include "Ability.h"
#include "language.h"

#include "FileMgr.h"
#include "BattleMgr.h"
#include "DualMgr.h"
#include "CrackMgr.h"
#include "ProFiler.h" 

//#define DWORD_PTR unsigned int * 

//#define D3DVAL(val) float(val)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)

