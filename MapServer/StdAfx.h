// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

#define WINVER			0x0602
#define _WIN32_WINNT	0x0602

#include <afx.h>
#include <ole2.h>
#include <initguid.h>
#include <winsock2.h>
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

#define rand()	rand_()		// 020707 YGI
extern int rand_();

#include "HigherLayers/DefaultHeader.h"

#include "GameSystem.h"


