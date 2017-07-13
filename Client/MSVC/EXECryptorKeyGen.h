#ifndef EXECryptorKeyGenH
#define EXECryptorKeyGenH

#include <windows.h>

#pragma pack(push,1)
typedef struct {
  int LicType;    //0..15
  // if LicType = 1 then we get ExpiryMonth/ExpiryYear
  // otherwise we get UserParam
  int UserParam;  //0..1023
  int ExpiryMonth,//1..12
      ExpiryYear; //2004..2024
  bool F1,F2,F3,F4,F5,F6;
} TSerialNumberInfo;
#pragma pack(pop)

typedef
  void (_stdcall *TCreateSerialNumber) (const char *ProjectFileName,
    const char *RegistrationName, const char *HardwareID, char *SerialNumber);
typedef
  void (_stdcall *TCreateSerialNumberEx) (const char *ProjectFileName,
    const char *RegistrationName, const char *HardwareID,
    const TSerialNumberInfo *SNInfo, char *SerialNumber);

class TKeyGen {
  private:
    HMODULE hLib;
    TCreateSerialNumber FCreateSerialNumber;
    TCreateSerialNumberEx FCreateSerialNumberEx;
  public:
    TKeyGen(const char *DllPath = NULL);
    ~TKeyGen();

    void CreateSerialNumber(const char *PrjName, const char *RegistrationName,
      const char *HardwareID, char *SerialNumber);
    void CreateSerialNumberTimeLimited(const char *PrjName,
      const char *RegistrationName,
      bool F1, bool F2, bool F3, bool F4, bool F5, bool F6,
      int ExpiryMonth, int ExpiryYear,
      const char *HardwareID, char *SerialNumber);
    void CreateSerialNumberEx(const char *PrjName,
      const char *RegistrationName,
      bool F1, bool F2, bool F3, bool F4, bool F5, bool F6,
      int LicType, int UserParam,
      const char *HardwareID, char *SerialNumber);
};

#endif
