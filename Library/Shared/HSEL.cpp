#include "HSEL.h"

CHSEL_STREAM::CHSEL_STREAM(void)
{
	iVersion = 3;
	//iHSELType = HSEL_ENCRYPTTYPE_1;
	iCRCValue = 0;
}

CHSEL_STREAM::~CHSEL_STREAM(void)
{

}

__int32 CHSEL_STREAM::Initial(HSEL_INITIAL hselinit)
{
	hselinit = hselinit;
	iCRCValue = 0;
	return unsigned(1);
}

//////////////////////////////////////////////////////////////

// return crc
bool CHSEL_STREAM::Encrypt(char *lpStream, const __int32 iStreamSize/* = 0*/)
{
	return true;
}

//return crc
bool CHSEL_STREAM::Decrypt(char *lpStream, const __int32 iStreamSize/* = 0*/)
{
	return true;
}

char CHSEL_STREAM::GetCRCConvertChar(void)const
{
	return '\0';
}
short CHSEL_STREAM::GetCRCConvertShort(void)const
{
	return (short)0;
}
__int32 CHSEL_STREAM::GetCRCConvertInt(void)const
{
	return (__int32)0;
}

void CHSEL_STREAM::SetKeyCustom(HselKey IntoKey)
{

}

void CHSEL_STREAM::SetNextKey(void)
{

}

void CHSEL_STREAM::GenerateKeys(HselKey &IntoKey)
{

}
