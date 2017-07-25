﻿// MapInfo.h: interface for the CMapInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPINFO_H__6A59C12B_1C45_495B_958A_D5A51EFE0646__INCLUDED_)
#define AFX_MAPINFO_H__6A59C12B_1C45_495B_958A_D5A51EFE0646__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_MAP_		200		// 서버하고 똑같아야 한다. // 031013 kyo 100 -> 200으로 
#define MAP_INFO_MYCODE	040503	// YGI 040510

// map_move_type

#define MMT_FAIL		-1		// 없는 맵이다.
#define MMT_NORMAL		0		// 이동하는데 아무런 제약이 없다.
#define MMT_NOT_MOVE	1		// 이동 못하게 막아둔 맵이다.

#pragma pack(push,1)
typedef struct MAPINFO
{
	char mapfile[30];
	char mapname[30];
	char cPosType; // CSD-030509
	int  nSaveX;   // CSD-030509
	int  nSaveY;   // CSD-030509
	char daylightcontrol;	// 지붕이 있으면 0     // 없으면  ( 아래것과 거의 같다 ) 
	char rain;		//  1 :이면 비가 오는곳이다.	0 : 비가 오지 않는 동굴같은 곳이다. 
	char nation;	//  국가코드. 
	char forrookie;//0 -> PK 되고 HP회복 정상 1-> PK안됨 2->PK 안되고 HP 빠른 회복 3->PK 되고 HP 빨리 회복
	int  ghostguide3x, ghostguide3y;	// nation code 3
	int  ghostguide4x, ghostguide4y;	// nation code 4..

	int	 port;
	int  group;//020420 lsw
	int  Class;//021113 lsw
	int  LvMin;//020420 lsw
	int  LvMax;//021113 lsw
	int  nDisableMagicType; // CSD-030516
	int  bEncode;		// 암호화 유무	// YGI-040510

	int	 m_MoveType;//GM이 셋팅 하는 데이타, 테이블에는 이런 필드가 없음

	MAPINFO()
	{
		cPosType = 0; // CSD-030509
		nSaveX= 0;    // CSD-030509
		nSaveY= 0;    // CSD-030509
		daylightcontrol= 0;	// 지붕이 있으면 0     // 없으면  ( 아래것과 거의 같다 ) 
		rain= 0;		//  1 :이면 비가 오는곳이다.	0 : 비가 오지 않는 동굴같은 곳이다. 
		nation= 0;	//  국가코드. 
		forrookie= 0;
		ghostguide3x= 0, ghostguide3y= 0;	// nation code 3
		ghostguide4x= 0, ghostguide4y= 0;	// nation code 4..

		port = 0;
		group= 0;//020420 lsw
		Class= 0;//021113 lsw
		LvMin= 0;//020420 lsw
		LvMax= 0;//021113 lsw
		nDisableMagicType = 0; // CSD-030516
		bEncode = 0;	// YGI-040510

		m_MoveType = 0;
	}
}t_MapInfo,*LPMAPINFO;
#pragma pack(pop,1)

extern t_MapInfo MapInfo[MAX_MAP_];
t_MapInfo* GetMapInfoByMapName(const char *szMapName); // CSD-030506
t_MapInfo* GetMapInfoByMapPort(int nPort);             // CSD-030506

#endif // !defined(AFX_MAPINFO_H__6A59C12B_1C45_495B_958A_D5A51EFE0646__INCLUDED_)
