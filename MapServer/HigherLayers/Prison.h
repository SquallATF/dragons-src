/**
 * @file	Prison.h.
 *			interface for the CPrison class.
 */
#pragma once

/**
 * @struct	_tagPrisonInfo.
 * @brief	���� �������� ����.
 */
//< LTH-040723-KO.
typedef struct _tagPrisonInfo
{
	char	szMapName[20];			/**< �� �̸�. */
	POINT	xyPosition;				/**< ��ġ. */
} tagPrisonInfo;
//> LTH-040723-KO.

/**
 * @enum	ePRISON_NATION.
 * @brief	���� �ý��ۿ� ���ǰ� �ϴ� ���� ��ȣ�ε� �򰥸��� ����� �������.
 */
//< LTH-040727-KO.
enum ePRISON_NATION
{
	PN_VYSEUS	= 0,
	PN_ZYPERN	= 1,
	PN_YILSE	= 2,
	PN_MAX		= 3,
};
//> LTH-040727-KO.

/**
 * @class	CPrison.
 * @brief	���� �ý��� class.
 *			���� �ý����� �۵� ��Ű�� ���� class.
 */
//< LTH-040824-KO.
class CPrison  
{
public:
	/**
	 * @def		enum _ePRISON_SYS_RESULT.
	 * @brief	���� �ý��� ó���� ��� ���.
	 */
	//< LTH-040727-KO.
	typedef enum _ePRISON_SYS_RESULT
	{
		PSR_ERROR						= 0,	/**< ����� ����. */
		PSR_ALREADY_REGISTERED_BAD_USER	= 1,	/**< �̹� �ҷ� ������ ��ϵǾ� ���� ���. */
		PSR_MOVE_TO_PRISON				= 2,	/**< �������� �̵� �Ѵ�. */
		PSR_NOT_BAD_USER				= 3,	/**< ������ ���� ������ �ƴϴ�. */
		PSR_MOVE_TO_VILLAGE				= 4,	/**< ���� ������ �̵��Ѵ�. */
		PSR_MAX							= 5,	/**< ����� �ִ� ����. */
	} ePRISON_SYS_RESULT;
	//> LTH-040727-KO.

protected:
	bool SetUserStartingMap(const char* szUserID, const char* szMapName, const POINT xyPosition);
	void GetPrisonPosition(const int nNation, char* szMapName, POINT& xyPosition);
	bool GetUserNation(const char* szCharName, int& nNation);
	void SendPrisonSystemResult(const int nCn, const CPrison::ePRISON_SYS_RESULT ePrisonSysResult);
	bool SetBadUser(const char* szUserID, const char* szCharName, const DWORD dwRemainTime);
	bool IsBadUser(const char* szUserID);
	bool GetUserID(const char* szCharName, char* szUserID);
	bool SetDeleteBadUser(const char* szUserID);
	void GetVillagePosition(const int nNation, char* szMapName, POINT& xyPosition);
	bool SetImprisonTerm(const char* szUserId, const DWORD dwImprisonTerm);
	DWORD GetImprisonTerm(const char* szUserId);
	void GetAllCharName(const char* szUserID, char szCharName[4][20]);
	bool IsPrisonMap();
	bool LoadPrisonInfo();

public:
	void RecvUpdateImprisonTerm(const short int cn);
	void MoveToVillage(const char* szUserID, const int nNation);
	void RecvBadUserRelease(t_packet* p, const int nCn);
	void RecvBadUserImprison(t_packet* p, const int nCn);
	void MoveToPrison(const char* szUserID, const int nNation);

public:
	CPrison();
	virtual ~CPrison();

protected:
	bool m_bIsPrisonMap;					/**< LTH-040824-KO �� ���� ������ �ִ� ���ΰ�?. */
	DWORD m_dwImprisonment;					/**< LTH-040727-KO ������ �ִ� �Ⱓ. */
	tagPrisonInfo* m_ppPrisonInfo[PN_MAX];	/**< LTH-040723-KO ���� ���� ����ü �迭. */
	int m_naNumOfPrisonByNation[PN_MAX];	/**< LTH-040727-KO �� ������ ���� ��ǥ�� ����. */
};
//> LTH-040824-KO.