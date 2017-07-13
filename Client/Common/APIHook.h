// APIHook.h: interface for the CAPIHook class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APIHOOK_H__C3592DA0_92F1_4592_B103_6B9C788AC974__INCLUDED_)
#define AFX_APIHOOK_H__C3592DA0_92F1_4592_B103_6B9C788AC974__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "stdafx.h"

#include <windows.h>

class CAPIHook  
{
public:
     CAPIHook(LPSTR pszModName, LPSTR pszFuncName, PROC pfnHook);

	virtual ~CAPIHook();

	operator PROC() {return (m_pfnOrig);}
public:
	//Unhook a function from all modules
	void UnHookFunc();
	//hook a function in all modules
	void HookFunc(PSTR pszCalleeModName,PSTR pszFuncName,PROC pfnHook,BOOL fExcludeAPIHookMod);
	//call the real GetProcAddress
	static FARPROC WINAPI GetProcAddressRaw(HMODULE hmod, PCSTR pszProcName);

private:
	static PVOID sm_pvMaxAppAddr;	//maxumum private memory address
	static CAPIHook *sm_pHead;		//Address of first Object
	CAPIHook *m_pNext;				//Address of next Object

	PCSTR m_pszCalleeModName;		//Module cintaining he function (ANSI)
	PCSTR m_pszFuncName;			//Function name in callee (ANSI)
	PROC m_pfnOrig;					//Original function aaddress in callee
	PROC m_pfnHook;					//Hook function address
	BOOL m_fExcludeAPIHookMod;		//Hook module w/CAPIHook implementation?

private:
	static void WINAPI ReplaceIATEntryInAllMods(PCSTR pszCalleeModName,PROC pfnOrig,PROC pfnHook,BOOL bExcludeAPIHookMod);
	static void WINAPI ReplaceIATEntryInOneMod(PCSTR pszCalleeModName, PROC pfnOrig, PROC pfnHook, HMODULE hmodCaller);

private:
	//use when a dll is newly loaded after hooking a function
	static void WINAPI FixupNewlyLoadedModule(HMODULE hmod,DWORD dwFlags);
	static HMODULE WINAPI LoadLibraryA(PCSTR pszModulePath);
	static HMODULE WINAPI LoadLibraryW(PCWSTR pszModulePath);
	static HMODULE WINAPI LoadLibraryExA(PCSTR pszModulePath,HANDLE hFile,DWORD dwFlags);
	static HMODULE WINAPI LoadLibraryExW(PCWSTR pszModulePath, HANDLE hFile, DWORD dwFlags);
	static FARPROC WINAPI GetProcAddress(HMODULE hmod ,PCSTR pszProcName);

private:
	//Instance hooks in these functions 
	static CAPIHook sm_LoadLibraryA;
	static CAPIHook sm_LoadLibraryW;
	static CAPIHook sm_LoadLibraryExA;
	static CAPIHook sm_LoadLibraryExW;
	static CAPIHook sm_GetProcAddress;
};

#endif // !defined(AFX_APIHOOK_H__C3592DA0_92F1_4592_B103_6B9C788AC974__INCLUDED_)
