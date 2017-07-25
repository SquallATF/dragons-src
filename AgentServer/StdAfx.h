﻿#pragma once


#define WINVER			0x0602
#define _WIN32_WINNT	0x0602

#include <afx.h>
#include <ole2.h>
#include <initguid.h>
#include <windows.h>
#include <winsock2.h>
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

//#if !defined(_DLLEXPORT_)
//	// If _DLLEXPORT_ is NOT defined then the default is to import.
//	#if defined(__cplusplus)
//		#define DLLENTRY extern "C" __declspec(dllimport)
//	#else
//		#define DLLENTRY extern __declspec(dllimport)
//	#endif
//
//	#define STDENTRY DLLENTRY HRESULT WINAPI
//	#define STDENTRY_(type) DLLENTRY type WINAPI
//
//	// Here is the list of server APIs offered by the DLL (using the
//	// appropriate entry API declaration macros just #defined above).
//
//	STDENTRY DllRegisterServer(void);
//	STDENTRY DllUnregisterServer(void);
//#else  // _DLLEXPORT_
//	// Else if _DLLEXPORT_ is indeed defined then we've been told to export.
//	#if defined(__cplusplus)
//		#define DLLENTRY extern "C" __declspec(dllexport)
//	#else
//		#define DLLENTRY __declspec(dllexport)
//	#endif
//
//	#define STDENTRY DLLENTRY HRESULT WINAPI
//	#define STDENTRY_(type) DLLENTRY type WINAPI
//#endif // _DLLEXPORT_

#define GUID_SIZE 128
#define MAX_STRING_LENGTH 256
typedef void**	PPVOID;
//010909 lsw
#include "LocalizingMgr.h"//021007 lsw