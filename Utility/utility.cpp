
//#include "stdafx.h"
#include "utility.h"

#include <tchar.h>
#include <Windows.h>

//get the app path
StdString GetAppPath(void)
{
	static StdString strAppPath(_T(""));
	if (strAppPath.empty()) {
		strAppPath.reserve(MAX_PATH);
		TCHAR bufPath[MAX_PATH] = _T("");
		::GetModuleFileName(NULL, bufPath, MAX_PATH);

		strAppPath = bufPath;
		strAppPath = strAppPath.substr(0, strAppPath.rfind(_T("\\")) + 1);
	}

	return strAppPath;
}

