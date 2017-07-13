#include "stdafx.h"
#include "EXECryptorKeyGen.h"
#include <string.h>

TKeyGen::TKeyGen(const char *DllPath)
{
  char buf[1024];
  if (DllPath == NULL) buf[0]=0;
  else strcpy(buf,DllPath);
  strcat(buf,"eckeygen.dll");
  hLib = LoadLibrary(buf);
  if (hLib != NULL) {
    FCreateSerialNumber = (TCreateSerialNumber)GetProcAddress(hLib,"CreateSerialNumber");
    FCreateSerialNumberEx = (TCreateSerialNumberEx)GetProcAddress(hLib,"CreateSerialNumberEx");
  }
}

TKeyGen::~TKeyGen()
{
  FreeLibrary(hLib);
}

void TKeyGen::CreateSerialNumber(const char *PrjName, const char *RegistrationName,
  const char *HardwareID, char *SerialNumber)
{
  if (FCreateSerialNumber != NULL)
    FCreateSerialNumber(PrjName, RegistrationName, HardwareID, SerialNumber);
}

void TKeyGen::CreateSerialNumberTimeLimited(const char *PrjName,
  const char *RegistrationName,
  bool F1, bool F2, bool F3, bool F4, bool F5, bool F6,
  int ExpiryMonth, int ExpiryYear,
  const char *HardwareID, char *SerialNumber)
{
  TSerialNumberInfo li;
  li.F1=F1; li.F2=F2; li.F3=F3; li.F4=F4; li.F5=F5; li.F6=F6;
  li.LicType=1;
  li.ExpiryMonth=ExpiryMonth;
  li.ExpiryYear=ExpiryYear;
  FCreateSerialNumberEx(PrjName,RegistrationName,HardwareID,&li,SerialNumber);
}

void TKeyGen::CreateSerialNumberEx(const char *PrjName,
  const char *RegistrationName,
  bool F1, bool F2, bool F3, bool F4, bool F5, bool F6,
  int LicType, int UserParam,
  const char *HardwareID, char *SerialNumber)
{
  TSerialNumberInfo li;
  li.F1=F1; li.F2=F2; li.F3=F3; li.F4=F4; li.F5=F5; li.F6=F6;
  li.LicType=LicType;
  li.UserParam=UserParam;
  FCreateSerialNumberEx(PrjName,RegistrationName,HardwareID,&li,SerialNumber);
}

