#include <windows.h>
#include <stdio.h>
#include "CoreException.h"

BOOL DumpException(LPEXCEPTION_POINTERS lpExcep, char* szOutMsg, void *pData, int nSize)
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
		char szFileName[256 + 1];
		GetLocalTime(&time);

		sprintf(szFileName, "MapCrash_%d_%d_%d_%d_%d_%d.log", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
		fp = fopen(szFileName, "w");

		fprintf(fp, "%d year, %d month, %d day,%d hour, %d minute, %d second\n",
			time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

		fprintf(fp, "%s\n", szOutMsg);
		fprintf(fp, "Crashed address %xh \n", dwExceptionAddress);

		fprintf(fp, "Crashed Map Address %04X:%08X\n",
			dwSegCs,
			dwExceptionAddress);


		fprintf(fp, "Exception Code %u \n", dwExceptionCode);
		fprintf(fp, "Exception Flag %xh \n", dwExceptionFlags);
		fprintf(fp, "NumberParameters %d \n", dwNumberParameters);
		fprintf(fp, "Dr0 %xh \n", dwDr0);
		fprintf(fp, "Dr1 %xh \n", dwDr1);
		fprintf(fp, "Dr2 %xh \n", dwDr2);
		fprintf(fp, "Dr3 %xh \n", dwDr3);
		fprintf(fp, "Dr6 %xh \n", dwDr6);
		fprintf(fp, "Dr7 %xh \n", dwDr7);

		fprintf(fp, "SegGs %xh \n", dwSegGs);
		fprintf(fp, "SegFs %xh \n", dwSegFs);
		fprintf(fp, "SegEs %xh \n", dwSegEs);
		fprintf(fp, "EFlags %xh \n", dwEFlags);

		fprintf(fp, "Esi %xh \n", dwEsi);
		fprintf(fp, "Edi %xh \n", dwEdi);
		fprintf(fp, "Ebp %xh \n", dwEbp);
		fprintf(fp, "Esp %xh \n", dwEsp);
		fprintf(fp, "Eip %xh \n", dwEip);


		fprintf(fp, "Eax %xh \n", dwEax);
		fprintf(fp, "Ebx %xh \n", dwEbx);
		fprintf(fp, "Ecx %xh \n", dwEcx);
		fprintf(fp, "Edx %xh \n", dwEdx);

		if (pData)	fprintf(fp, "info : packet_type(PTCL_) =  %d \n", *((char*)pData));

		fprintf(fp, "\n\n");

		fclose(fp);
		fp = NULL;

		if (pData)
		{
			sprintf(szFileName, "MapCrash_info_%d_%d_%d_%d_%d_%d.bin", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
			fp = fopen(szFileName, "w");
			fwrite(&nSize, 4, 1, fp);
			fwrite(pData, 1, nSize, fp);
			fclose(fp);
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
