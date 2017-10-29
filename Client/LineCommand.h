﻿#if !defined(AFX_LINECOMMAND_H__589944BD_DCE5_4B05_9D6B_82C08591AC88__INCLUDED_)
#define AFX_LINECOMMAND_H__589944BD_DCE5_4B05_9D6B_82C08591AC88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSmallTipCount
{
public:
	char m_szKey[50];		// 020925 YGI
	int	m_nPage;
	CSmallTipCount *next;

public:
	CSmallTipCount() { next = NULL; m_nPage = 0; }
	~CSmallTipCount() { SAFE_DELETE(next); }

	int AddData(int number, char *str);
	int GetPage(char *key);
};

//enum LineCommandIndex
//{
//	LC_NO_COMMAND = 0,
//	LC_GO_NORTH = 1,
//	LC_GO_WEST = 2,
//	LC_GO_EAST = 3,
//	LC_GO_SOUTH = 4,
//	LC_EAT_ITEM = 5,
//	LC_NOT_GO = 6,
//	LC_CHAT_WHISPER = 7,
//	LC_CHAT_PARTY = 8,
//	LC_CHAT_GUILD = 9,
//	LC_TOTAL_MAP_CONNECTIONS = 10,
//	LC_TOTAL_CONNECTIONS = 11,
//	LC_MAKE_PARTY = 12,
//	LC_DEFAULT_WALK = 13,
//	LC_DEFAULT_RUN = 14,
//	LC_HI = 15,
//	LC_HELLO = 16,
//	LC_ENTERHOUSE = 17,
//	LC_TAME_STOP = 18,
//	LC_TAME_ATTACK = 19,
//	LC_TAME_FOLLOWME = 20,
//	LC_TAME_NAME = 21,
//	LC_GO_STOP = 22,
//	LC_GO_RANDOM = 23,
//	LC_OPEN_TIMEMENU = 24,	// 0328 YGI
//	LC_CLOSE_TIMEMENU = 25,
//	LC_CLOSE_ALL = 26,
//	LC_EAR_MESSAGE_REFUSE = 27,	// 특정사람 귓속말 듣기 거부
//	LC_EAR_MESSAGE_OK = 28,	// 귀속말 허락
//	LC_REPRINT_EAR_MESSAGE = 29,	// 받은 메모 다시 보기
//	LC_EAR_MESSAGE_REFUSE_WHO = 30,	// 귓속말거부 상대
//	LC_GREETING = 40,
//	LC_EXPLAIN_OPEN = 41,
//	LC_EXPLAIN_CLOSE = 42,
//	LC_REQ_MAN_TO_MAN = 43,
//	LC_RESET_MAGIC_ARRAY = 44,
//	LC_MSG_ALL_NATION = 45,
//	LC_GUILD_EXIT = 46,	// 길드원 탈퇴
//	LC_GUILD_JOIN = 47,	// 길드원 가입
//	LC_GUILD_ALL_MSG = 48,	// 길드 전체 메시지
//	LC_GUILD_CHANGE_EACH_LEVEL = 49,	// 길드내 등급을 변환 시킨다.
//	LC_GUILD_CMD_INFO = 50,	// 길드 정보 출력
//	LC_CHANGE_JOB = 51,	// 보여주는 직업을 바꾼다.
//	LC_STOP_WAR = 52,	// 휴전 요청한다.
//	LC_SCAN_TARGET_VYSEUS = 53,	// 적을 스켄한다. ( 바이서스 )
//	LC_SCAN_TARGET_ZYPERN = 54,	// 적을 스켄한다. ( 자이펀 )
//	LC_KILL_CHARACTER = 55,	// 길드 정보 출력
//	LC_DIRECTIONS = 56,
//	LC_OPEN_DISPLAY_SKILL_EXP = 57,
//	LC_CLOSE_DISPLAY_SKILL_EXP = 58,
//	LC_OPEN_DISPLAY_TAC_EXP = 59,
//	LC_CLOSE_DISPLAY_TAC_EXP = 60,
//	LC_LIGHTOFRESCUE = 61,	// '구원의 빛'
//	LC_AUTOLEVELUP_CLOSE = 62,	// 001130 YGI
//	LC_AUTOLEVELUP_OPEN = 63,	// 001130 YGI
//	LC_ANNONCE_ON = 64,
//	LC_ANNONCE_OFF = 65,
//	LC_EAR_COMMAND_INFO = 66,
//	LC_FIGHTMAP_ALL_CHAT = 68,
//	LC_SMALL_TIPS = 69,
//	LC_EMOTION = 70,	// 감정
//	LC_EXIT_GUILD = 71,	// 길드 탈퇴
//	LC_VIEW_NK = 72,
//	LC_SQUAD_LEADER = 73,	// 사령관 -> 예하 부대장
//	LC_SQUAD = 74,	// 사령관 -> 부대원 
//	LC_SQUAD_MEMBER = 75,	// 부대장 -> 부대원 
//	LC_SQUAD_MEMBER_TO_MEMBER = 76,	// 부대원 -> 부대원 
//	LC_WAR_MENU_OPEN = 77,	// 전쟁관리소 메뉴 오픈 
//	LC_WAR_MENU_NOTICE_OPEN = 78,	// 전쟁 공지 // LTS TEMP
//	LC_GUILD_MAIL = 79,	// 길드메일
//	LC_SQUAD_CHAT = 80,
//	LC_GUILD_CHAT = 81,	// 길드 체팅
//	LC_COMMANDER_INFO = 82,
//	LC_SQUAD_INFO = 83,
//	LC_GUILD_CHAT_ON = 84,
//	LC_GUILD_CHAT_OFF = 85,
//	LC_RESET_ABILITY = 86,
//	LC_WAR_GIVE_LIFE = 87,	// 011015 LTS
//	LC_PARTY_ALL_FREE = 88,
//	LC_SEND_GUILD_MAIL = 89,
//	LC_SEND_MAIL = 90,
//	LC_SHOW_ME_DUAL = 91,
//	LC_REPORTER_MODE = 92,
//	LC_FRIEND_MENU = 93,
//	LC_WAR_GIVE_LIFE2 = 94,	// 011213 LTS
//	LC_SCAN_TARGET_YILSE = 95,	// 011217 LTS
//	LC_GETCOLOSSUS_INFO = 96,
//	LC_INVITE_COLOSSUS = 97,
//	LC_JOIN_COLOSSUS = 98,
//	LC_PK_ON_OFF = 99,
//	LC_SET_NORMAL_CHAT_MODE = 109, // lsw 020620
//	LC_COMBAT_SKILL_ON_OFF = 100,
//	LC_LOCALWAR_CHAT = 111,		// LTS NEW LOCALWAR
//	LC_QUESTMODE = 112,		// LTS DRAGON MODIFY
//	LC_WEATHER_ON = 113, // CSD-CN-031215
//	LC_WEATHER_OFF = 114, // CSD-CN-031215
//	LC_COMBAT_RESET = 115, // CSD-020611
//	LC_MAGIC_DISPLAY_ON = 116,// CSD-020620
//	LC_MAGIC_DISPLAY_OFF = 117, // CSD-020620
//	LC_REPRINT_NORMAL_CHAT = 118, // lsw 020620
//	LC_REPRINT_GUILD_CHAT = 119,// lsw 020620
//	LC_REPRINT_GUILD_BBS = 120,
//	LC_TRADE = 121,	//020808 YGI 
//	LC_CHECK_DUAL_FAME = 122,		// LTS DUAL FAME
//	LC_WHAT_QUEST_IN_MAP = 123,		// 021022 kyo network7에서 이쪽으로
//	LC_MERCHANT_BBS = 124,		// 021022 직접거래
//	LC_MERCHANT_EXCHANGE = 125,		// 021022 직접거래
//	LC_EVENT_MOVE_MAP = 126,		// 021107 YGI
//	//<< 031021 kyo
//	LC_EMOTI_LOVE = 127,
//	LC_EMOTI_SMILE,
//	LC_EMOTI_HAPPY,
//	LC_EMOTI_SHY,
//	LC_EMOTI_CRY,
//	LC_EMOTI_GOOD,
//	LC_EMOTI_SORRY,
//	LC_EMOTI_ANGER,
//	LC_EMOTI_THANKS,
//	LC_EMOTI_TELL,
//	LC_GAME_SCISSOR = 137,
//	LC_GAME_ROCK,
//	LC_GAME_PAPER,
//	LC_GAME_GO,
//	LC_GAME_HELP,
//	LC_GAME_PARTY,
//	LC_GAME_HEAL,
//	LC_GAME_TRANSACTION,
//	LC_GAME_BEG,
//	LC_GAME_CONGRAGULATION = 146,
//
//	LC_BBS_LIMITED_USER_ADD = 147,
//	LC_BBS_ON = 148,
//	LC_BBS_OFF = 149,
//	//>> 031021 kyo
//	LC_RAID_START = 150, // CSD-040310
//	LC_RAID_END = 151,	 // CSD-040310
//	LC_LINERESET_POINT = 153, //月神夜090822
//	LC_MINIWINDOW_MODE = 154, //月神夜090822
//	LC_MAXWINDOW_MODE = 155, //月神夜090822
//	LC_INC_EXP //月神夜090822
//};

enum LineCommandIndex
{
	LC_NO_COMMAND = 0,
	LC_GO_NORTH,
	LC_GO_WEST,
	LC_GO_EAST,
	LC_GO_SOUTH,
	LC_EAT_ITEM,
	LC_NOT_GO,
	LC_CHAT_WHISPER,
	LC_CHAT_PARTY,
	LC_CHAT_GUILD,
	LC_TOTAL_MAP_CONNECTIONS,
	LC_TOTAL_CONNECTIONS,
	LC_MAKE_PARTY,
	LC_DEFAULT_WALK,
	LC_DEFAULT_RUN,
	LC_HI,
	LC_HELLO,
	LC_ENTERHOUSE,
	LC_TAME_STOP,
	LC_TAME_ATTACK,
	LC_TAME_FOLLOWME,
	LC_TAME_NAME,
	LC_GO_STOP,
	LC_GO_RANDOM,
	LC_OPEN_TIMEMENU,				// 0328 YGI
	LC_CLOSE_TIMEMENU,
	LC_CLOSE_ALL,
	LC_EAR_MESSAGE_REFUSE,			// 특정사람 귓속말 듣기 거부
	LC_EAR_MESSAGE_OK,				// 귀속말 허락
	LC_REPRINT_EAR_MESSAGE,			// 받은 메모 다시 보기
	LC_EAR_MESSAGE_REFUSE_WHO,		// 귓속말거부 상대
	LC_GREETING,
	LC_EXPLAIN_OPEN,
	LC_EXPLAIN_CLOSE,
	LC_REQ_MAN_TO_MAN,
	LC_RESET_MAGIC_ARRAY,
	LC_MSG_ALL_NATION,
	LC_GUILD_EXIT,					// 길드원 탈퇴
	LC_GUILD_JOIN,					// 길드원 가입
	LC_GUILD_ALL_MSG,				// 길드 전체 메시지
	LC_GUILD_CHANGE_EACH_LEVEL,		// 길드내 등급을 변환 시킨다.
	LC_GUILD_CMD_INFO,				// 길드 정보 출력
	LC_CHANGE_JOB,					// 보여주는 직업을 바꾼다.
	LC_STOP_WAR,					// 휴전 요청한다.
	LC_SCAN_TARGET_VYSEUS,			// 적을 스켄한다. ( 바이서스 )
	LC_SCAN_TARGET_ZYPERN,			// 적을 스켄한다. ( 자이펀 )
	LC_KILL_CHARACTER,				// 길드 정보 출력
	LC_DIRECTIONS,
	LC_OPEN_DISPLAY_SKILL_EXP,
	LC_CLOSE_DISPLAY_SKILL_EXP,
	LC_OPEN_DISPLAY_TAC_EXP,
	LC_CLOSE_DISPLAY_TAC_EXP,
	LC_LIGHTOFRESCUE,				// '구원의 빛'
	LC_AUTOLEVELUP_CLOSE,			// 001130 YGI
	LC_AUTOLEVELUP_OPEN,			// 001130 YGI
	LC_ANNONCE_ON,
	LC_ANNONCE_OFF,
	LC_EAR_COMMAND_INFO,
	LC_FIGHTMAP_ALL_CHAT,
	LC_SMALL_TIPS,
	LC_EMOTION,						// 감정
	LC_EXIT_GUILD,					// 길드 탈퇴
	LC_VIEW_NK,
	LC_SQUAD_LEADER,				// 사령관 -> 예하 부대장
	LC_SQUAD,						// 사령관 -> 부대원 
	LC_SQUAD_MEMBER,				// 부대장 -> 부대원 
	LC_SQUAD_MEMBER_TO_MEMBER,		// 부대원 -> 부대원 
	LC_WAR_MENU_OPEN,				// 전쟁관리소 메뉴 오픈 
	LC_WAR_MENU_NOTICE_OPEN,		// 전쟁 공지 // LTS TEMP
	LC_GUILD_MAIL,					// 길드메일
	LC_SQUAD_CHAT,
	LC_GUILD_CHAT,					// 길드 체팅
	LC_COMMANDER_INFO,
	LC_SQUAD_INFO,
	LC_GUILD_CHAT_ON,
	LC_GUILD_CHAT_OFF,
	LC_RESET_ABILITY,
	LC_WAR_GIVE_LIFE,				// 011015 LTS
	LC_PARTY_ALL_FREE,
	LC_SEND_GUILD_MAIL,
	LC_SEND_MAIL,
	LC_SHOW_ME_DUAL,
	LC_REPORTER_MODE,
	LC_FRIEND_MENU,
	LC_WAR_GIVE_LIFE2,				// 011213 LTS
	LC_SCAN_TARGET_YILSE,			// 011217 LTS
	LC_GETCOLOSSUS_INFO,
	LC_INVITE_COLOSSUS,
	LC_JOIN_COLOSSUS,
	LC_PK_ON_OFF,
	LC_SET_NORMAL_CHAT_MODE,		// lsw 020620
	LC_COMBAT_SKILL_ON_OFF,
	LC_LOCALWAR_CHAT,				// LTS NEW LOCALWAR
	LC_QUESTMODE,					// LTS DRAGON MODIFY
	LC_WEATHER_ON,					// CSD-CN-031215
	LC_WEATHER_OFF,					// CSD-CN-031215
	LC_COMBAT_RESET,				// CSD-020611
	LC_MAGIC_DISPLAY_ON,			// CSD-020620
	LC_MAGIC_DISPLAY_OFF,			// CSD-020620
	LC_REPRINT_NORMAL_CHAT,			// lsw 020620
	LC_REPRINT_GUILD_CHAT,			// lsw 020620
	LC_REPRINT_GUILD_BBS,
	LC_TRADE,						// 020808 YGI 
	LC_CHECK_DUAL_FAME,				// LTS DUAL FAME
	LC_WHAT_QUEST_IN_MAP,			// 021022 kyo network7에서 이쪽으로
	LC_MERCHANT_BBS,				// 021022 직접거래
	LC_MERCHANT_EXCHANGE,			// 021022 직접거래
	LC_EVENT_MOVE_MAP,				// 021107 YGI
							
	LC_EMOTI_LOVE,
	LC_EMOTI_SMILE,
	LC_EMOTI_HAPPY,
	LC_EMOTI_SHY,
	LC_EMOTI_CRY,
	LC_EMOTI_GOOD,
	LC_EMOTI_SORRY,
	LC_EMOTI_ANGER,
	LC_EMOTI_THANKS,
	LC_EMOTI_TELL,
	LC_GAME_SCISSOR,
	LC_GAME_ROCK,
	LC_GAME_PAPER,
	LC_GAME_GO,
	LC_GAME_HELP,
	LC_GAME_PARTY,
	LC_GAME_HEAL,
	LC_GAME_TRANSACTION,
	LC_GAME_BEG,
	LC_GAME_CONGRAGULATION,

	LC_BBS_LIMITED_USER_ADD,
	LC_BBS_ON,
	LC_BBS_OFF,
	//>> 031021 kyo
	LC_RAID_START,					// CSD-040310
	LC_RAID_END,					// CSD-040310
	LC_LINERESET_POINT,				//月神夜090822
	LC_MINIWINDOW_MODE,				//月神夜090822
	LC_MAXWINDOW_MODE,				//月神夜090822
	//LC_INC_EXP_OR_NOT,				// add by taniey
	LC_INC_EXP_STOP,				// add by taniey
	LC_INC_EXP_START,				// add by taniey
};

#define TAME_FAIL_CHANGE_NAME		0 // 이름바꾸기를  실패. 
#define TAME_NO_CHAR				1 // 그런이름으로 길들인 동물이 없습니다.  
#define TAME_NO_NAME_CHAR			2 // 그런 이름을 가진 대상이 없습니다. 
#define TAME_FOLLOW					3 // 따라갑니다. 
#define TAME_STOP					4 // 멈춰 섭니다. 
#define TAME_ATTACK					5 // 공격합니다. 
#define TAME_SUCCESS_CHANGE_NAME	6 // 이름을 바꾸었습니다. 
// 문자열의 리스트
class CTextCmd
{
public:
	char *m_txt;
	CTextCmd *next;

public:
	CTextCmd() { m_txt = NULL; next = NULL; }
	~CTextCmd()
	{
		SAFE_DELETE(next);
		SAFE_DELETE(m_txt);
	}
	void AddText(char *txt);
	char *GetLastText();
	char *GetFirstText();
};

class CEmotionCmd		// 감정표현 명령어들
{
public:
	char *m_Cmd;
	CEmotionCmd *next;

public:
	CEmotionCmd() { m_Cmd = NULL; next = NULL; }
	~CEmotionCmd()
	{
		SAFE_DELETE(next);
		SAFE_DELETE(m_Cmd);
	}
	void AddCmd(char *cmd);
	int GetCmd(char *txt);
};

/////////////////////////////////////////////////////////////////////////////////////
extern char	Lc_ChatType;
extern char	Lc_TempChatName[31];
extern char	Lc_DefaultMoveType;
extern char GreetingStr[FILENAME_MAX];
extern int IsLineCommand(char *message, int nsize);
extern void SendTotalMapConnections(void);
extern void SendTotalConnections(void);
extern void RecvTotalMapConnections(int no);
extern void RecvTotalConnections(int no);
extern void RecvTameCommandResult(int result);
extern void RecvTameNameChange(t_tame_name_change *p);
extern void RecvHostName(t_tame_host_name *p);
extern void RecvHostEnd(tame_end *p);

extern void SendBBS(char *bbs);		// 공지사항 공고...
extern void SendBBSAll(char *bbs);

extern void RecvBbs_quest(char *bbs);					// 0810 lhs 퀘스트 도움말

extern void SendChatDataGlobalFightMap(char *lpStr);
extern void CallViewSmallTips(char *txt);
extern void SendLcEmotion(char *txt);

extern int LoadEmotionCmd(CEmotionCmd *lpEmotionCmd);
extern void SendExitGuild();
extern void ViewNk(char *name);
extern bool SendChatGuild(char *msg);
extern void RecvGuildChatData(t_packet *p);
extern void CheckShowMeDual();
extern void SendJoinColossusMap();
extern void TradeONOFF(char *t);//020808 YGI 
extern int LoadSmallTipsPage();
#endif //AFX_LINECOMMAND_H__589944BD_DCE5_4B05_9D6B_82C08591AC88__INCLUDED_