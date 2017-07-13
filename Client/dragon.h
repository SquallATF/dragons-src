/*

*/

#ifndef	__DRAGON_H__
#define	__DRAGON_H__

#include <windows.h>
#include <windowsx.h>
#include <time.h>

#include "directdraw.h"
#include "directinput.h"
//#include "MenuDef.h"
#include "Network.h"
#include "item.h"

#define	CM_COMMAND			0
#define	CM_MESSAGE			1

#define CONNECT_REFUSE		10101010

////////////// 0427 lkh Ãß°¡ /////////////////
#define	DISPLAY_EARTHQUAKE	10
#define	DISPLAY_WAVEEARTH	1

extern HWND					g_hwndMain;
extern HINSTANCE			g_hInstance;

extern TCHAR				g_szTitle[ MAX_LOADSTRING ];
extern TCHAR				g_szWindowClass[ MAX_LOADSTRING ];
extern TCHAR				g_szCWD[ _MAX_PATH ];

extern BOOL					g_bIsActive;
extern BOOL					g_bCommandMode;

extern DIRECTDRAWINFO		g_DirectDrawInfo;

extern POINT				g_pointMouse, g_pointFuzz;
extern long					g_pointMouseX, g_pointMouseY;
extern int					g_nLButtonState, g_nRButtonState;
extern int					g_nLDButtonState, g_nRDButtonState;
extern int					g_nOldLButtonState, g_nOldRButtonState;
extern int					g_nSensitivity;

extern int					LButtonDownIng, RButtonDownIng;
//extern int					LButtonDown, RButtonDown;
extern DWORD				g_CurrentTime;
extern DWORD				g_ClientTime;	// GameProc()¿¡¼­ °è¼ÓÀûÀ¸·Î Check ÇÑ´Ù. 
extern DWORD				g_ServerClientTimeGap;

extern DWORD				g_curr_time;  //  ¼­¹ö¿¡¼­º¸³»ÁÖ´Â ÇöÀçÀÇ ½Ã°£. ÃÊ´ÜÀ§..
extern DWORD				g_packet_recv_send_checktime;
////////////////////// 0128 lkh /////////////////////////
extern bool					g_Volume_Off;	//¿ÏÀüÈ÷ ¼Ò¸®¸¦ Á×ÀÏ±î?

extern int					g_Operator_Function;

// ----------------------- khs 0804

extern char				  *g_DestBackBuf, *g_OldDestBackBuf;
extern int					Mox, Moy;
extern int					Mapx, Mapy;
extern LPCHARACTER			Hero;

//-----------------------------------

extern int					QuitFlag;

extern DWORD				g_RecvBytes, g_SendBytes;

extern bool					MouseCursorOnOff;

extern DWORD				g_Inside;

extern int					NowLoading;




/////////////////// 0126 ÀÌ±ÔÈÆ //////////////////////
extern short int	g_Current_Volume;	//ÇöÀçÀÇ »ç¿îµå º¼·ý(0~100»çÀÌ°ª)
extern short int	g_To_Volume		;	//º¯È¯µÇ¾î¾ß ÇÏ´Â (ÃÖÁ¾ÀûÀ¸·Î) ¸ñÇ¥ »ç¿îµå º¼·ý
extern short int	g_Change_Volume	;		//º¼·ý ¼öÄ¡ º¯È¯Ä¡(-+ º¯È­°ª->Fade Out/In)

extern char IIDD[MAX_PATH];
extern char PPWW[MAX_PATH];

extern int	CheckSumError;	

extern int recvAttackResultCount;
extern FILE *keyplayfp;
extern FILE *keykeyplayfp;

extern bool		g_bIsChristmas;
extern bool		g_bIsChristmas;
//----2006-7-30----
//	DWORD			MSK_SelectType;
//	DWORD			MSK_LBS;
//	DWORD			MSK_RBS;

#define			MSK_LS	0x38ab5
#define			MSK_RS	0x8a4f4
#define			MSK_SST	0x38ab5
#define			MSK_OPF	0x7b3da

typedef struct	tagGAMEINFO
{
	char		proxy_ip[3][ 18 ];
	int			proxy_port;
	char		agent_ip[18];
	int			agent_port;
	int			version;

	int			map_port; // ¸Ê¼­¹öÀÇ port¹øÈ£ Port¸¦ ¾Ë¸é MapNameµµ ¾Ë¼ö ÀÖ´Ù. 

	time_t		timeGame;
	BYTE		cWeather;
	BYTE		cWindVelocity;
	BYTE		cHumidity;

	POINT		pointMouse, pointFuzz;
	int			nLButtonState, nRButtonState;
	int			nOldLButtonState, nOldRButtonState;
	int			nSensitivity;

	
	DWORD		dwFrameRate;
	DWORD		dwFrameCount, dwFrameCount0;
	DWORD		dwFrameTime, dwFrameTime0;

	RECT		rectView, rectScreen;

	LPCHARACTER  lpcharacter;
	LPCHARACTER  lpoldcharacter;

	int			nSelectedSpriteType;
	LPVOID		lpvSelectedSprite;

	int			 PopupFaceCount;
	int			 checkcount; // ¸¶¿ì½º¿Í Ãæµ¹ÇÏ¸é Count°í 5»óÀÌµÇ¸é È­¸é¿¡ ÀÌ¸§À» º¸¿©ÁØ´Ù. 
	
	// ¹è°æÀÇ ´Ù¾çÇÑ Ãâ·ÂÀ» À§ÇÑ ÇÃ·¹±×
	int			displayOption;		
	int			displayOptionDelay;
	//< 040326 kjy
	int			preventSelectYilse;
	//> 040326 kjy
	
	int		LS_res;
	int		RS_res;
	int		SST_res;

	tagGAMEINFO()
	{
		Set_LS(0);
		Set_RS(0);
		Set_SST(0);
	}

	//--½Ø»ñº¯Êý
	int		Set_LS(int	ls)
	{
	// const	int	MSK_LS=0X38ab5;


		LS_res=ls;
		LS_res=MSK_LS^LS_res;
		nLButtonState=ls;
		return 1;
	}
	int			Set_RS(int rs)
	{
		//const int MSK_RS=0x8a4f4;

		RS_res=rs;
		RS_res=MSK_RS^RS_res;
		nRButtonState=rs;
		return 1;
	}
	
	int		Set_SST(int sst)
	{
		//const int MSK_SST=0x98bd;
		SST_res=sst;
		SST_res^=MSK_SST;//Õâ¸öÐèÒªµÄ»°×Ô¼º¸Ä,ÉÏÃæ
		nSelectedSpriteType=sst;
		return 1;
	}
	//--Ìá¹©µÄ¼ì²éº¯Êý
	int		Check()
	{
//		const	int MSK_LS=0x38ab5;

/*		LS_res^=MSK_LS;
		if(LS_res!=__nLButtonState)
		{
			return 0;
		}
		LS_res^=MSK_LS;
		RS_res^=MSK_RS;
		if(RS_res!=__nRButtonState)
		{
			return 0;
		}
		RS_res^=MSK_RS;*/
///Ö»¼ì²éÁËÒ»¸ö
		SST_res^=MSK_SST;
		if(SST_res!=nSelectedSpriteType)
		{
			return 0;
		}
		SST_res^=MSK_SST;
		
		return 1;
	}
} GAMEINFO, *LPGAMEINFO;

extern GAMEINFO		g_GameInfo;

#ifdef __cplusplus
extern "C"
{
#endif

extern void		SetCurrentWorkingDirectory( void );
extern char*	GetCurrentWorkingDirectory( void );
extern void		SetInfoFile( void );
extern char*	GetInfoFile( void );
extern char*	EatFrontWhiteChar( char* );
extern char*	EatRearWhiteChar( char* );
extern BOOL		ShowErrorMessage( char* lpszMessage );

enum eExitAppType
{
	EA_NORMAL = 0,//Á¤»óÀûÀÎ Á¾·á.
	EA_CHECK_ACCEL= 1,
	EA_BEFORE_EXE= 2,
	EA_BEFORE_EXE2= 3,
	EA_LOAD_CHAR_SPR_DATA_SUB= 4,
	EA_LOAD_RIDER_SPR_DATA_SUB= 5,
	EA_LOAD_RIDER_BAND_SPR_DATA_SUB= 6,
	EA_LOAD_RIDER_MANTLE_SPR_DATA_SUB= 7,
	EA_LOAD_WEAPON_SPR_DATA= 8,
	EA_LOAD_GUARD_SPR_DATA= 9,
	EA_LOAD_LOACL_WAR_INFO_SPR_DATA= 10,
	EA_LOAD_SPRITE_INFO= 11,
	EA_LOAD_RIDER_GRADE_SPR_DATA_SUB= 12,
	EA_LOAD_HORSE_SPR_DATA_SUB= 13,
	EA_LOAD_HORSE_SADDEL_SPR_DATA_SUB= 14,
	EA_LOAD_HORSE_GRADE_SPR_DATA_SUB= 15,
	EA_LOAD_LOACL_CHAIR_SPR_DATA= 16,
	EA_LOAD_LOACL_SQUAD_SPR_DATA= 17,
	EA_RECV_CONNECT_INFO= 18,
	EA_CPU_CLOCK_ERROR = 19,
	EA_CRACK_THREAD_FIND_CRACK = 20,
	EA_LANGUAGE_ERROR = 21,
	EA_JAPAN_LOGIN_ERROR = 22,
};

extern void		ExitApplication( const eExitAppType eEAType );

extern char*	EatRearWhiteChar( char* pStr );

enum eLoadingGameDataStep
{
	LGDS_FIRST = 0,
	LGDS_STEP1 = 1,
	LGDS_STEP2 = 2,
	LGDS_STEP3 = 3,
	LGDS_STEP4 = 4,
	LGDS_STEP5 = 5,
	LGDS_STEP6 = 6,
	LGDS_STEP7 = 7,
	LGDS_STEP8 = 8,
	LGDS_STEP9 = 9,
	LGDS_STEP10 = 10,
	LGDS_STEP11 = 11,
	LGDS_STEP12 = 12,
	LGDS_END	= 13,
};
extern int		LoadingGameData();

extern bool		chinese_input;
extern void		CheckChineseInput( HWND hwnd, LPARAM lParam );
// lsw
extern bool IsChatBoxActive();
extern void SetChatMode(const int iMode, bool bIsForce = false);

enum eCHATTARGETNORMAL
{
	CHAT_TARGET_NORMAL		= 0,//º¸Åë Ã¤ÆÃ ¸ðµå
	CHAT_TARGET_GUILD		= 1,//±æµå Ã¤ÆÃ ¸ðµå
	SELECT_WISPER_MODE		= 2,//±Ó¸»»ó´ë ¼±ÅÃ ÇÏ±â
	CHAT_TARGET_WISPER1		= 3,//±Ó¸»»ó´ë ¼±ÅÃ ÇÏ±â	//ÇÊµå³Ñ¹ö¿Í ÀÏÄ¡
	CHAT_TARGET_WISPER2		= 4,//±Ó¸»»ó´ë ¼±ÅÃ ÇÏ±â
	CHAT_TARGET_WISPER3		= 5,//±Ó¸»»ó´ë ¼±ÅÃ ÇÏ±â
	CHAT_TARGET_WISPER4		= 6,//±Ó¸»»ó´ë ¼±ÅÃ ÇÏ±â
	CHAT_TARGET_WISPER5		= 7,//±Ó¸»»ó´ë ¼±ÅÃ ÇÏ±â
	CHAT_TARGET_WISPER6		= 8,//±Ó¸»»ó´ë ¼±ÅÃ ÇÏ±â
	CHAT_TARGET_WISPER7		= 9,//±Ó¸»»ó´ë ¼±ÅÃ ÇÏ±â
	CHAT_TARGET_WISPER8		= 10,//±Ó¸»»ó´ë ¼±ÅÃ ÇÏ±â
	CHAT_TARGET_WISPER9		= 11,//±Ó¸»»ó´ë ¼±ÅÃ ÇÏ±â
	CHAT_TARGET_WISPER10	= 12,//±Ó¸»»ó´ë ¼±ÅÃ ÇÏ±â
	
};
const int CHAT_TARGET_WISPER_START	= CHAT_TARGET_WISPER1;//±Ó¸»»ó´ë ¼±ÅÃ ÇÏ±â ½ÃÀÛÁ¡
const int CHAT_TARGET_WISPER_END	= CHAT_TARGET_WISPER5;//±Ó¸»»ó´ë ¼±ÅÃ ÇÏ±â ³¡Á¡

#ifdef __cplusplus
}
#endif

#endif	// __DRAGON_H__


