#pragma once

#include <xstring>

#ifndef _UNICODE
#define StdString	std::string
#else
#define StdString	std::wstring
#endif // _UNICODE


StdString GetAppPath(void);
