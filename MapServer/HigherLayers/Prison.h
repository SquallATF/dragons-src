/**
 * @file	Prison.h.
 *			interface for the CPrison class.
 */
#pragma once

/**
 * @struct	_tagPrisonInfo.
 * @brief	감옥 맵정보를 저장.
 */
//< LTH-040723-KO.
typedef struct _tagPrisonInfo
{
	char	szMapName[20];			/**< 맵 이름. */
	POINT	xyPosition;				/**< 위치. */
} tagPrisonInfo;
//> LTH-040723-KO.

/**
 * @enum	ePRISON_NATION.
 * @brief	감옥 시스템에 사용되곤 하는 국가 번호인데 헷갈리지 말라고 만들었다.
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
 * @brief	감옥 시스템 class.
 *			감옥 시스템을 작동 시키기 위한 class.
 */
//< LTH-040824-KO.
class CPrison  
{
public:
	/**
	 * @def		enum _ePRISON_SYS_RESULT.
	 * @brief	감옥 시스템 처리의 결과 상수.
	 */
	//< LTH-040727-KO.
	typedef enum _ePRISON_SYS_RESULT
	{
		PSR_ERROR						= 0,	/**< 결과값 에러. */
		PSR_ALREADY_REGISTERED_BAD_USER	= 1,	/**< 이미 불량 유저로 등록되어 있을 경우. */
		PSR_MOVE_TO_PRISON				= 2,	/**< 감옥으로 이동 한다. */
		PSR_NOT_BAD_USER				= 3,	/**< 감옥에 갇힌 유저가 아니다. */
		PSR_MOVE_TO_VILLAGE				= 4,	/**< 메인 마을로 이동한다. */
		PSR_MAX							= 5,	/**< 결과값 최대 개수. */
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
	bool m_bIsPrisonMap;					/**< LTH-040824-KO 이 맵이 감옥이 있는 맵인가?. */
	DWORD m_dwImprisonment;					/**< LTH-040727-KO 감옥에 있는 기간. */
	tagPrisonInfo* m_ppPrisonInfo[PN_MAX];	/**< LTH-040723-KO 감옥 정보 구조체 배열. */
	int m_naNumOfPrisonByNation[PN_MAX];	/**< LTH-040727-KO 각 국가별 감옥 좌표의 개수. */
};
//> LTH-040824-KO.