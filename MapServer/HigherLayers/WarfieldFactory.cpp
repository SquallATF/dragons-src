/**
 * @file	WarfieldFactory.cpp.
 *			implementation of the CWarfieldFactory class.
 */

#include "..\stdafx.h"
#include "WarfieldFactory.h"
#include "HadesWarfield.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWarfieldFactory::CWarfieldFactory()
{
}

CWarfieldFactory::~CWarfieldFactory()
{
}

/**
 * @fn		CWarfieldFactory::Create().
 * @brief	IWarfieldDefine interface�� ���̵�(nObjectID)�� ���� Ư�� �����ͷ� ������Ű�� �Լ�.
 * @param	INT nObjectID. ������ �ĺ� ID.
 * @param	VOID** ppVoid. �����͸� �����Ͽ� ��ȯ �� ������.
 * @return	STDMETHODIMP. ���� ���� ���� ��ȯ.
 */
//< LTH-040419-KO.
STDMETHODIMP CWarfieldFactory::Create(INT nObjectID, VOID** ppVoid)
{
	LPWARFIELDDEFINE lpWarfield = NULL;
	
	switch(nObjectID)
	{
	case WI_HADES_WARFIELD:		// LTH-040419-KO ���� ������.
		lpWarfield = new CHadesWarfield;
		break;

	default:					// LTH-040419-KO ���̵� Ʋ���� ���� ����.
		::JustMsg("NO Warfield ID!! %d", nObjectID);
		return E_FAIL;
	}

	Dbg_Assert(NULL != lpWarfield);

	*ppVoid = (LPVOID)lpWarfield;

	return S_OK;
} //> LTH-040419-KO.