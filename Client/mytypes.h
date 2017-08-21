/*

*/

#ifndef __MYTYPES_H__
#define	__MYTYPES_H__

#pragma pack()

#define _MAPOBJECT_DIRECTORY_CHANGE_
//#define BETA_TESTER_
#define	CON_NORMAL						0x00 // 정상상태	 0000000
#define	CON_CURSE						0x01 // 활동적상태 0000001
#define	CON_FROZEN						0x02 // 무기력상태 0000010		
#define	CON_SLOW						0x04 // 슬로우상태 0000100	
#define	CON_DAZE						0x08 // 혼란상태	 0001000	
#define	CON_POISON						0x10 // 중독상태	 0010000
#define	CON_STONE						0x20 // 석화상태	 0100000
#define CON_DEATH						0x40 // 죽음상태	 1000000
//< CSD-030419 : NPC 종족
#define DUMMY							0
#define HUMAN							1
#define GUARD							2
#define ORC_DUMMY						3
#define LIFELESS						4
#define HOBBIT							5
#define DWARF							6
#define DRAGON							7 
#define ANIMAL							8
#define NORM_MON						9
#define SEALSTONE						11
#define GUARDSTONE						12
#define CASTLEGATEWAY					12
#define WARARMS							12
#define GUARDTOWER						13


//> CSD-030419

////////////////////////// 이규훈 1230 ///////////////////////////////////////////
#define SET_LOSTSIGHT					1			//시력상실
//////////////////////////////////////////////////////////////////////////////////
#define LIGHTNESS						2
/////////////////////////////////////////////////////////////////////////////		


////////////////////////// 0402 이규훈 추가 //////////////////////////////////////
#define SET_CUREBLIEND					3
#define SET_INFORMATION					4

////////////////////////// 0811 YJS 추가 //////////////////////////
#define FIGHT_RESULT					5

#define FIGHT_WIN						1
#define FIGHT_LOSE						2
#define FIGHT_DRAW						3
#define FIGHT_LOGOUT					4


//////////////// 0311 lkh 추가 (이벤트용 음악 출력) //////////////
#define OPERATOR_PLAYSOUND_EVENT1		851		//행사 시작을 알리는 축가
#define OPERATOR_PLAYSOUND_EVENT2		853		//박수 소리
#define OPERATOR_OUTPUT_CELEBRATE		290		//축포를 쏘아 올이는 효과

/////////////// 0810 lhs 추가 /////////////////
#define START_QUEST_SOUND				15000		//퀘스트 시작 소리
#define QUEST_SALUTE					15001		//퀘스트용 축포 효과



#define	TILE_SIZE						32
#define	VILLAGE_SIZE					720


#define MAX_LOADSTRING					100
#define	MAX_CHAR_IMAGE_FRAME			60
#define	MAX_ANIMATION					26//24				// LTS ACTION
#define	MAX_PC_ANIMATION_FRAME			40
#define	MAX_SHORTPATH					50
#define	MAX_DIRECTION					5
#define	MAX_CHAT_STRING					256

#define	SCREENMODE_WINDOW				0
#define	SCREENMODE_FULLSCREEN			1

#define	SCREEN_WIDTH					800
#define	SCREEN_HEIGHT					600

#define GAME_SCREEN_XSIZE				800
#define GAME_SCREEN_YSIZE				520// 352//020515 lsw
//
//#define	SCREEN_WIDTH				1024
//#define	SCREEN_HEIGHT				768
//
//  #define GAME_SCREEN_XSIZE			1024
//  #define GAME_SCREEN_YSIZE			(768-(600-472))// 352


#define RADARXL							640
#define RADARYL							480

// 초보자의 Level
#define ROOKIE_LEVEL					7


#define GAME_SCREEN_XTILE			(GAME_SCREEN_XSIZE/TILE_SIZE)
#define GAME_SCREEN_YTILE			(GAME_SCREEN_YSIZE/TILE_SIZE)

#define GAME_SCREEN_XTILE_MID		(GAME_SCREEN_XTILE/2)
#define GAME_SCREEN_YTILE_MID		(GAME_SCREEN_YTILE/2)

//////////////////////// 0808 lkh 추가 /////////////////////////////
#define OBJECTSOUNDPLAY_XSIZE			1400
#define OBJECTSOUNDPLAY_YSIZE			1000

#define	PC_WIDTH						45
#define	PC_HEIGHT						64
#define	SIZE_AREA_WIDTH					10
#define	SIZE_AREA_HEIGHT				10


#define STATE_BUTTON_STILL				0
#define	STATE_BUTTON_PRESSED			1
#define	STATE_BUTTON_RELEASED			2
#define STATE_BUTTON_DOUBLECLICK		3


#define	CTYPE_PC						0
#define	CTYPE_NPC						1
#define	CTYPE_MONSTER					2


#define	SPRITETYPE_NONE					0		// 마우스에 의해 CHARACTER선택되는 루틴에서 사용
#define	SPRITETYPE_CHARACTER			1 		// 출력.. Sortting.. 
#define	SPRITETYPE_NPC					2
#define	SPRITETYPE_MONSTER				3
#define	SPRITETYPE_OBJECT				4
#define	SPRITETYPE_OBJECT_BOTTOM		5  
#define	SPRITETYPE_TILE					6
#define SPRITETYPE_ITEM					7
#define SPRITETYPE_ITEM_BOTTOM			8
#define SPRITETYPE_EFFECT_NORMAL		9
#define SPRITETYPE_EFFECT_CEILING		10
#define SPRITETYPE_EFFECT_BOTTOM		11
#define SPRITETYPE_NUMBER				12
#define SPRITETYPE_FOOT					13
#define SPRITETYPE_DEST_POINT			14	
#define SPRITETYPE_DEST_POINT_BOTTOM	15
#define SPRITETYPE_SKILLCURSOR			16
#define SPRITETYPE_DETECT_TILE			17
#define SPRITETYPE_SKILLGAUGE			18
#define	SPRITETYPE_CLOCKNIDDLE			19	
#define SPRITETYPE_ON_THE_CEILING_CHAR	20
#define SPRITETYPE_ALPHAOBJECT			21	


#define CHARACTER_SPRITE_FRAME_MAX_		75
#define CHARACTER_ATTACKRANGE_MAX_		181 // CSD-031007
#define CHARACTER_ACCESSORY_MAX_		241		// 011030 KHS JJING
//#define CHARACTER_ACCESSORY_MAX		220		// NEW_CHAR YGI


// Hero->peacests
#define PC_STATUS_PEACE 				0
#define PC_STATUS_BATTLE				1


#define SIGHT_PLUS						90000


#define EVENT_TYPE_NPC					0
#define EVENT_TYPE_MAPTILE				1


// 공격할때 캐릭터가 공격범위에 있는지 없는지에 대한 리턴값. 
// IsAttackRange()에서 사용.....
#define ATTACKRANGE_IN_AREA_			1
#define ATTACKRANGE_IN_MIN_				2
#define ATTACKRANGE_OUT_MAX_			3

#define ERR_FILE_OPEN					-1
#define ERR_INVALID_ID					-11

enum ViewTypes
{	//< CSD-030509
	VIEWTYPE_NORMAL_ = 0,
	VIEWTYPE_CAMELEON_,
	VIEWTYPE_GHOST_,
	VIEWTYPE_TREE_,
	VIEWTYPE_OBSERVE_,
	VIEWTYPE_CONFUSION_,
	VIEWTYPE_POISON_,
	VIEWTYPE_STONE_,
	VIEWTYPE_SLOW_,
	VIEWTYPE_FROZEN_,
	VIEWTYPE_CURSE_,
	VIEWTYPE_TRANSPARENCY_,
	VIEWTYPE_SELECT_,
	VIEWTYPE_STAND_UP_,
};	//> CSD-030509

enum eChatTypes
{
	CHATTYPE_NORMAL = 0,
	CHATTYPE_WHISPER,
	CHATTYPE_PARTY,
	CHATTYPE_GUILD,
	CHATTYPE_COLOSSUS_GLOBAL,
	CHATTYPE_COLOSSUS,
	CHATTYPE_LOCAL_WAR,
	CHATTYPE_NATION_WAR,
	CHATTYPE_GUILD_BBS,
	CHATTYPE_YES_OR_NO,
	CHATTYPE_NOTICE,
	CHATTYPE_MERCHANT_BBS,
	CHATTYPE_TALK_WITH_GM,
};

// 피케이한 캐릭터의 얼굴사진에 쓰일 칼자국 Sprite번호.
#define PK_BLOOD_EFFECT		272
// 아래가 정의되어 있으면 국가개념이 적용된다. 
#define _NATION_APPLY_		
// 아래가 정의되어 있으면 숫자로 출력된다. 
#define DISPLAY_HIT_NUMBER_	

enum 
{
	CLS_STEP = 0,  // 클래스 단계
	DUAL_CLS,  // 듀얼 클래스
	EXP_STEP,  // 경험치 단계
	T01_STEP,  // 택틱 경험치 단계
	T02_STEP,
	T03_STEP,
	T04_STEP,
	T05_STEP,
	T06_STEP,
	T07_STEP,
	T08_STEP,
	T09_STEP,
	T10_STEP,
	T11_STEP,
	T12_STEP,
	T13_STEP,
	CSP_STEP, // 전투스킬 포인터
	MAX_STEP  // 최대 데이타수
};

struct CCharRank
{	//< CSD-030326
	DWORD nation	  :3;  // 속한나라 - 0:없음, 1, 2, 3:바이서스, 4:자이펀, 5:헤게모니아, 6:일스
	DWORD guild_master:3;  // 길드 마스터
	DWORD king		  :1;  // 나라 왕
	DWORD dwReserved  :9;  // 사용되지 않음
	DWORD counselor	  :2;  // 0 : 없음, 1: 회사 운영자, 2: 게임내 운영자
	DWORD ultra_nk3	  :1;  // 극악피케이	1004 YGI
	DWORD ultra_nk4	  :1;  // 극악피케이	1004 YGI
	DWORD ultra_nk6	  :1;  // 극악피케이	1004 YGI
	DWORD reporter	  :1;
	DWORD imsi		  :10; // 1004 YGI
};	//> CSD-030326


typedef enum	enumAPPEARANCE_PC
{
	enumBODY = 0,
	enumHEAD,
	enumMAIL,
	enumLEFTHAND,
	enumRIGHTHAND,
	enumFOOT
} APPEARANCE_PC;


typedef enum	enumDIRECTION
{
	DIRECTION_SAME		=   -1,
	DIRECTION_DOWN		=   0,
	DIRECTION_LEFTDOWN	=   1,
	DIRECTION_LEFT		=   2,
	DIRECTION_LEFTUP	=   3,
	DIRECTION_UP		=   4,
	DIRECTION_RIGHTUP	=   5,
	DIRECTION_RIGHT		=   6,
	DIRECTION_RIGHTDOWN	=   7,
	
	DIRECTION_LEFTDOWNDOWN	=	8,
	DIRECTION_LEFTLEFTDOWN	=	9,
	DIRECTION_LEFTLEFTUP    =	10,
	DIRECTION_LEFTUPUP		=	11,
	DIRECTION_RIGHTUPUP		=	12,
	DIRECTION_RIGHTRIGHTUP	=	13,
	DIRECTION_RIGHTRIGHTDOWN=	14,
	DIRECTION_RIGHTDOWNDOWN	=	15
		
} DIRECTION;

typedef enum	enumACTION_PC_WOMAN
{	
	ACTION_NONE = 0,					//   1 기본동작  1
	ACTION_BASIC_WALK,					//   2 기본걷기  2
	ACTION_BATTLE_WALK_OWNHAND,			//   3 전투걷기  3
	ACTION_BATTLE_NONE,					//   4 전투기본  4
	ACTION_ATTACK_ONESWORD1,			//   5 한손공격  5
	ACTION_ATTACK_ONESWORD2,			//   6 한손공격2  
	ACTION_ATTACK_BONG_DOWN,			//   7 봉, 곤봉,  공격( 내려치기 )
	ACTION_ATTACK_BONG_JJIRKI,			//   8 창, 봉, 공 격( 찌르기 )
	ACTION_ATTACK_BOW,					//   9 활쏘기      
	ACTION_ATTACK_PENSWORD2,			//  10 펜싱자세   
	ACTION_ATTACK_PENSING1,				//  11 펜싱공격 1 
	ACTION_ATTACK_PENSING2,				//  12 펜싱공격 2 
	ACTION_RUN,						    //  13 뛰기
	ACTION_ATTACKED,				    //  14 맞기
	ACTION_DIE,						    //  15 죽기
	ACTION_MAGIC_BEFORE,				//  16 마법시전
	ACTION_MAGIC_CASTING,				//  17 마법캐스팅
	ACTION_SITDOWN,						//  18 않기
	ACTION_THROW,					    //  19 던지기공격
	ACTION_HORSE,					    //  20 말타기
	ACTION_BATTLE_WALK_TWOHAND,			//  21 전투걷기 2 ( 양손에 검 )
	ACTION_BATTLE_NONE_TWOHAND,			//  22 전투기본 2( 양손 )
	ACTION_ATTACK_BONG_ROUND,			//  23 봉, 창, 곤봉, 공격( 휘두르기 )
	ACTION_MAGIC_CASTING2,				//  24 마법캐스팅 2(남자 캐릭터에게만 적용 )
	ACTION_ANGER,						//	25 화내기			// LTS ACTION
	ACTION_VICTORY						//  26 승리포즈			// LTS ACTION
} ACTION_PC_WOMAN;

enum ACTION_NPC_MONSTER1
{
	MON1_NONE = 0,			// 기본동작
	MON1_BATTLE_WALK,		// 기본걷기
	MON1_ATTACKED,			// 맞기
	MON1_DIE,			    // 죽기....
	MON1_BATTLE_ATTACK,		// 한손공격
	MON1_BATTLE_MAGIC,		// 마법시전
	MON1_STAND_UP,			// 일어서기
	MON1_CASTING_MAGIC,		// 마법시전	// LTS DRAGON
	MON1_BATTLE_ATTACK2,	// 공격 2
	MON1_BATTLE_ATTACK3,	// 공격 3
	MON1_JUST_ACTION		// 임의의 동작
		
};

typedef enum	enumACTION_NPC
{
} ACTION_NPC;

typedef enum	enumACTION_MONSTER
{
	
	
} ACTION_MONSTER;

typedef struct	tagPOSITION
{
	short	x, y;
} POSITION, *LPPOSITION;

typedef struct	tagORIGIN
{
	short	x, y;
} ORIGIN, *LPORIGIN;

typedef struct	tagWORDSIZE
{
	WORD	cx;
	WORD	cy;
} WORDSIZE, *LPWORDSIZE;




typedef struct { 
	short xl, yl;
	short ox, oy;
	DWORD	size;
	char    *img;
}Spr;


typedef struct { 
	Spr sp[8][CHARACTER_SPRITE_FRAME_MAX_];	 
}CharacterSprite;

//_____________________________________________________	
struct NW_Character						// 010915 LTS		// LTS CHAR
{
	DWORD		WarNo						:	7;	//	전쟁번호
	DWORD		WarfieldNo					:	3;	//	전쟁터 번호
	DWORD		SquadNo						:	3;	//	부대번호
	DWORD		isSquadLeader				:	1;	//	부대장인가?	
	DWORD		isCommander					:	1;	//	사령관인가?
	DWORD		isCommanderCandidater		:	1;	//	사령관후보인가?
	DWORD		DoWarJoinVote				:	1;	//	전쟁터 참여 투표를 했나?
	DWORD		DoCommanderVote				:	1;	//	사령관 투표를 했다
	DWORD		YL_JoinNation				:	4;	//	일스인경우 자이펀에했나? 바이서스에 했나?  // 국가번호
	DWORD		Reserved					:	10;	//	예약..
};
//_____________________________________________________

typedef struct	tagGPCHARINFO
{							
	char type;
	DWORD id;
	char name[30];
	int	sprno;
	int mutant;  // 0810 KHS
	POINT position;
	DIRECTION direction;
	int	basicAction;
	int	basicWalk;
	int	nCurrentAction;
	int	animationType;
	int	accessory[4];
	int	mantle;			// 011018 KHS 망또
	int	jjing;			// 011030 KHS JJING
	
	int	hp,hpmax, mp,mpmax, sp, spmax;
	int	sex;
	char spell;
	short int face;
	int sight;
	
	BYTE bodyr, bodyg, bodyb;
	BYTE clothr, clothg, clothb;
	
	char viewtype;
	
	BYTE peacests;
	short Age;
	DWORD killpc;
	short openhouse;
	BYTE goodbad;
	
	short social_status;
	CCharRank name_status;				// 1004 YGI
	short fame;
	short nk3;
	short nk4;
	short nk6;
	
	char class_type;	// 0807 YGI
	char lv;			// 0807 YGI
	
	int	homex;
	int homey;
	
	bool eventnpc;
	bool bRecallNpc; // CSD-021119
	
	NW_Character NWCharacter;		// 010915 LTS		// UNROLL을 사용하지 않는 경우, 버그의 발생이 우려 
	
	BYTE nDualCls;		// 듀얼관련
	BYTE nClsStep;
	char JoinLocalWar;			// LTS LOCALWAR
	char ChairNum;				// LTS SITDOWN BUG
	
	char HorseNo;					// LTS HORSERIDER
	char RiderNo;				// LTS HORSERIDER
	char HorseGrade;				// LTS HORSERIDER
	char RiderGrade;				// LTS HORSERIDER
	char DragonLordWarTeam;		// LTS DRAGONLORD
	int nAttr;
	int nRace; // CSD-030419        
	short int guild_code; // CSD-030324 thai2 YGI
	WORD idWeaponItem; // CSD-031001
}	GPCHARINFO, *LPGPCHARINFO;




typedef struct	tagPCANIMATIONTABLE
{	
	int		nCount;
	BYTE	cFrame[ MAX_PC_ANIMATION_FRAME ];
	
	char  	dx[ MAX_PC_ANIMATION_FRAME ];
	char  	dy[ MAX_PC_ANIMATION_FRAME ];
	char    attackf[ MAX_PC_ANIMATION_FRAME ];
	short   wave[  MAX_PC_ANIMATION_FRAME ];
	short 	height[ MAX_PC_ANIMATION_FRAME ];
	short	effect[ MAX_PC_ANIMATION_FRAME ];
} PCANIMATIONTABLE, *LPPCANIMATIONTABLE;


#define PC_IN_MAP_MAX	


typedef struct tagCHARELEMENT
{	
	char Name[ 20];
	
	BYTE gender;					// 0:여자 1 : 남자
	BYTE Race;						// 종족
	BYTE Spiritvalue;				// 가치관
	BYTE Class;						// 0:전사 1:궁수 2:도적 3:성직자 4:마법사
	BYTE class_sprituality;	
	BYTE class_poisioning;	
	BYTE class_bombplay;		
	BYTE class_entrapment;	
	BYTE class_scrolling;	
	BYTE class_cunning1;	
	BYTE class_cunning2;	
	BYTE class_cunning3;	
	BYTE class_stealing;	
	
	BYTE job;
	
	BYTE clothr, clothg, clothb;	// 띠 색깔 R.G.B
	BYTE bodyr, bodyg, bodyb;		// 몸 색깔 R.G.B
	
	short Age;
	
	BYTE  Level;
	DWORD Exp;
	
	short Hp;
	short AttackPow;		// 공격력
	
	short Mana;				// 경험치
	short VitalPow;			// 생명력
	short Hungry;
	short Nk3, Nk4, Nk6;	// 각 국가별 우호도  010531 KHS
	
	DWORD Killmon1;			// 1계열몬스터 킬링스
	DWORD Killmon2;			// 2계열몬스터 킬링스
	DWORD Killmon3;			// 3게열몬스터 킬링수
	DWORD Killmon4;			// 4게열몬스터 킬링수
	DWORD Killmon5;			// 5게열몬스터 킬링수
	DWORD Killmon6;			// 6게열몬스터 킬링수
	DWORD Killanimal;		// 동물 킬링수
	DWORD KillNPC;			// 사람,NPC 킬링수
	
	DWORD Money;
	
	//최초캐릭터 생성할때의 수치..
	short StrOld;				//  힘
	short ConOld;				//  건강
	short DexOld;				//	민첩성
	short WisOld;				//	지혜
	short IntOld;				//	지능
	short MovePOld;				//	이동력
	short CharOld;				//	매력
	short EnduOld;				//  저향력
	short HealthOld;			//	체력
	short MoralOld;				//	사기
	short LuckOld;				//	행운
	
	short Str;				//  힘
	short Con;				//  건강
	short Dex;				//	민첩성
	short Wis;				//	지혜
	short Int;				//	지능
	short MoveP;			//	이동력
	short Char;				//	매력
	short Endu;				//  저향력
	short Health;			//	체력
	short Moral;			//	사기
	short Luck;				//	행운
	short wsps;
	
	
	short Resis_Poision;	//	독/죽음 저항력
	short Resis_Stone;		//	석화 마비 저항력
	short Resis_Magic;		//	마법저항력
	short Resis_Fire;		//	불 저항력
	short Resis_Ice;		//	얼음저항력
	short Resis_Elect;		//	전기저향력
	short Wizardspell;		//	Ws
	short Ws[200];			//	마법
	short Priestspell;		//	Ps
	short Ps[200];			//	신법
	
	// 내가 가지고 있는 아이템..
	DWORD Item[ 13][100];
	
	DWORD bankmoney;
	
}CHARELEMENT, *LPCHARELEMENT;

const	DWORD	ID_MSK=0x7eb29b9e;	//加密关键字,可以适当修改
///////////////////////////////////////////////////////////////////////////////
//< CSD-031001

//----------------------

const		DWORD		NAME_MSK=0X9B2BACD6;	//已经不用
//--bu fqyd 060730
//#define		MSK_CA		0x93cf6d //过去的异或数,由于比较容易被破解,所以改成了以下的随机函数
/////////////////////////////////这里定义的是随机异或的函数/////////////////////////////////////////////
//extern int 	MSK_CA;	//过去的异或数,由于比较容易被破解,所以改成了以下的随机函数
extern int mutant_MSK_CA;//转职
extern int movp_MSK_CA;//移动力
extern int nAttackedType_MSK_CA;//攻击类型
extern int bCanViewGhost_MSK_CA;//看鬼
extern int sight_MSK_CA;//视野
extern int excute_MagicNum_MSK_CA;//释放魔法类型,保存
extern int continue_RunMagic_MSK_CA;//魔法持续时间
extern int nActiveCombat_MSK_CA;//弓盗技能地址.E4 00 00 00 00 00 00 00 00 00 00 00 01冰暴什么的.
extern int nActiveLevel_MSK_CA;//弓盗技能等级地址.
extern int nCurrentAction_MSK_CA;//人物动作行为
extern int nCurrentFrame_MSK_CA;//人物晃动值
extern int bow_Type_MSK_CA;// 魔法冰箭，0是没有，1是火箭，2是冰箭
extern int is_mainmenu_active_MSK_CA;//窗口基址变量
extern int bCheckSalvation_MSK_CA;//无限名 ?FH,FFH,FFH,FFH档
//////////////////////////////////////////////*******************新加的*******************
extern int	ready_Magic_MSK_CA;		//0->마법 무 /1->마법 casting 중/2->마법 casting 유지 중/3->마법 발사
extern DWORD start_Casting_MSK_CA;		//casting 시작 시간
extern DWORD end_Casting_MSK_CA;		//casting 준비 완료 시간	
extern DWORD end_ReadyMagic_MSK_CA;		//casting 유지 종료될 시간 	
extern DWORD end_RunMagic_MSK_CA;		//마법 효과 끝나는 시간
extern DWORD castStartingTime_MSK_CA;		//마법 효과 끝나는 시간
////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////随机数字的函数 ?/////////////////////////////////////////////////////
extern	void mutant_cont(void);			//转职
extern	void movp_cont(void);			//移动力
extern	void nAttackedType_cont(void);			//攻击类型
extern	void bCanViewGhost_cont(void);			//看鬼
extern	void sight_cont(void);			//视野
extern	void excute_MagicNum_cont(void);			//释放魔法类型
extern	void continue_RunMagic_cont(void);			//魔法持续时间
extern	void nActiveCombat_cont(void);			//弓盗技能类型
extern	void nActiveLevel_cont(void);			//弓盗技能等级
extern	void nCurrentAction_cont(void);			//人物动作行为
extern	void nCurrentFrame_cont(void);			//晃动值
extern	void bow_Type_cont(void);			//魔法箭
extern	void is_mainmenu_active_cont(void);			//窗口基址变量
extern	void bCheckSalvation_cont(void);			//无限名 ?FH,FFH,FFH,FFH档
//////////////////////////////////////////////*******************新加的*******************
extern	void ready_Magic_cont(void);		//0->마법 무 /1->마법 casting 중/2->마법 casting 유지 중/3->마법 발사
extern	void start_Casting_cont(void);		//casting 시작 시간
extern	void end_Casting_cont(void);		//casting 준비 완료 시간	
extern	void end_ReadyMagic_cont(void);		//casting 유지 종료될 시간 	
extern	void end_RunMagic_cont(void);		//마법 효과 끝나는 시간
extern	void castStartingTime_cont(void);		//间隔时间?
////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

extern		char		g_name_0[31];			//不用

typedef struct _SChName
{
	char	name[31];							
}SChName;

extern std::vector<SChName>		*pChNames;
//-------------------------
class CCharacter     //这个类里面包括了人物名下边的所有信息,晃动值,人物ID,怪物ID等.且都是按顺序
					//排列的.可以改变其顺序就可以改变数据所在位置
{//角 ?
public:
	CCharacter() : lpPrev(NULL), lpNext(NULL)
	{
		m_nGuildCode = 0;
	}

	~CCharacter()
	{
	
	}

public:
	void SetGuildCode(int nGuild)
	{
		m_nGuildCode = nGuild;
	}

	int GetGuildCode() const 
	{
		return m_nGuildCode;
	}

	bool IsCounselor() const 
	{ 
		return (name_status.counselor) ? true:false;
	}
//	int id()
//	{
//		DWORD	temp=xid;
//		temp=temp^ID_MSK;
//		return(int)temp;
//	}
//	int set_id(int id)
//	{
//		//DWORD t=(DWORD)id;
//		xid=((DWORD)id)^ID_MSK;	//这里简单的异或加密
//		return 1;
//	}
//
//	int nCurrentFrame()//人物晃动值
//	{
//		DWORD	temp=xCF;
//		temp=temp^ID_MSK;
//		return(int)temp;
//	}
//	int set_nCurrentFrame(int CF)
//	{
//		//DWORD t=(DWORD)id;
//		nCurrentFrame()=((DWORD)CF)^ID_MSK;	//这里简单的异或加密
//		return 1;
//	}
//
//
//
//
//char	*get_name()
//{
//	//std::vector<SChName>::iterator	it;
//	return(*pChNames)[this->name_ptr_no].name;
//}
//
//int set_name(char *name)
//{	
//	SChName		temp;
//	memcpy(temp.name,name,20);
//	//for(int i=0;i<7;i++)
//	//((DWORD*)name_0)[i]^NAME_MASK;
//	pChNames->push_back(temp);//保存到一个统一的容器里面了
//
//	this->name_ptr_no=pChNames->size()-1;
//	return 1;
//}

private:
	DWORD		server_19;
	int			m_nGuildCode;
	DWORD		xid;  //角 蛘吖治锏?D
	// DWORD		xCF;  //人物晃动值

public:
	CCharRank name_status;  //地位

	char type;
	int id;
	char name[ 31];
	int name_ptr_no;	//只返回了一个统一的号码
	char namedisplaycount;
	int	sprno;

	int	mutant;   //0810 KHS  //转职,保存
	int	nutantani;	// 0810 KHS
	short age;
	// 010515 KHS
	bool eventnpc;
	
	//DOWRD	reserved_6; //无用.//这里几个变量也是,可以加加
	short nCharacterData[20];
	int	animationType;
	int	AIanimationRest;	//  0 아니면  움직이지 않고 그대로 서있는다. 
	int	AIattackTargetID;	//	어떤놈을 Mon1의 움직임에서 0 아니면  움직이지 않고 그대로 서있는다. 
	short movetargetid;
	bool sealstone;
	short sealstoneani;
	char GuardStoneAni;	// 010915 LTS	// LTS SEALSTONE
	char SetAni2Zero;
	
	//DOWRD	reserved_2; //无用.//这里几个变量也是,可以加加
	int x;
	int	restdx;
	// int	movp;//移动力,需要保存
	int	ddx;
	short int tox,toy; //可以改变变量的位置
	bool toflag;
	int y;

	short NumberAnimationValue;
	BYTE attackedresult;
	short flagattacked;//
	bool attacksuccess;
	char attackanimationON;
	char pluse_value;
	char willdie; // 이수치가 1이되면 죽는다.

	int	height;
	int	oldgox, oldgoy;
	int	gox, goy;
	DWORD desttime;
	char destflag;
	int	sex;
	int	walktime;
	// DOWRD	reserved_3; //无用.//这里几个变量也是,可以加加
	POINT position,oldposition;
	POINT offset;
	DIRECTION direction;
	DIRECTION todir; // direction은 todir로 바꾸려는성질이 있다. 
	short dirdelay;
	
	// int __movp; //移动力,需要保存

	int	light;  //应该是照明吧...
	int	attackMinRange;
	int	attackMaxRange;
	int	changeActionFlag;// 1이면 동작중에 다른동작으로 바뀔수 없다.
	
	// int	nAttackedType;//攻击类型,保存
	void* lpAttacked;
	
 
	
	int	nCurrentAction;		//人物动作值
	int	movp;//移动力,需要保存

	//int	nCurrentFrame;		//人物晃动值
	bool bCanViewGhost;    //看鬼开关,需要保存
	// int	excute_MagicNum;	//현재 시전자가 준비하고 있는 매직번호//姦렴침랬잚謹,괏닸
	int	nAttackedType;	// 攻击类型,保存
	int	bow_Type;		// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.
	int TotalAni;
	PCANIMATIONTABLE AnimationTable[ MAX_ANIMATION ];
	
	Spr	*sp, *sp0, *sp1, *sp2, *sp3, *sp4;
	
	int	pathcount;
	int	oldpathcount;
	int	path[MAX_SHORTPATH][2];
	bool moveFlag; //이동시 Tile의 위치를 기억하여 정확히 그곳으로 간다. 
	BYTE movetype; // 0:walk 1:run 2:horse riding.
	
	short basicAction;
	short basicWalk;
	char viewtype;
	BYTE canview;
	short canviewitem;
	//020320 lsw
	// bool bCanViewGhost;    //看鬼开关,需要保存
	// DOWRD	serverd_5;
	DWORD dwCanViewGhostTime;
	
	bool bCanViewMagicTrap;
	DWORD dwCanViewMagicTrapTime;
	
	bool bCanViewTrap;
	DWORD dwCanViewTrapTime;
	
	bool bCanViewBomb;
	DWORD dwCanViewBombTime;
	
	BYTE ghostani;
	BYTE ghostanito;
	
	int	accessory[4];
	int	mantle;	// 011018 KHS 망또
	int	jjing;	// 011030 KHS JJING
	
	DWORD bodycolor;
	DWORD clothcolor;
	DWORD mantlecolor;	// 011018 KHS 망또	
	
	int	hp,mp,Sp;//sp需要保存
	int	hpmax,mpmax,Spmax;
	int	dieani;
	int	diedelay;
	int	dieanitype;
	char boxitembreaktype;
	
	BYTE wisdom;
	BYTE intelligence;
	BYTE health;				//배고픔 or Health
	BYTE con;					//건강도
	char spell;
	short face;
	
	bool stolen;
	
	int	sight;//视野,保存
	BYTE peacests;
	BYTE goodbad;
	
	CHARELEMENT* ce;
	
	int ChatDelay;
	int	nChatType;
	char szChatMessage[MAX_CHAT_STRING];
	
	char smileno;
	short int smiledelay;
	
	
	int	excute_MagicNum;	//현재 시전자가 준비하고 있는 매직번호//姦렴침랬잚謹,괏닸
	int	ready_Magic;		//0->마법 무 /1->마법 casting 중/2->마법 casting 유지 중/3->마법 발사
	int	ready_Magic_rare;
	DWORD start_Casting;		//casting 시작 시간
	DWORD continue_Time;
	DWORD end_Casting;		//casting 준비 완료 시간
	
	DWORD start_ReadyMagic;	//casting 유지 시작된 시간
	DWORD end_ReadyMagic;		//casting 유지 종료될 시간 
	
	DWORD start_RunMagic;		//실제 마법 효과가 시작된 시간
	DWORD continue_RunMagic; 
	DWORD end_RunMagic;		//마법 효과 끝나는 시간
	
	int	inc_Num;
	
	bool blood_Set;			//서버로부터 죽었다는 메시지 날라오기 전에 클라이언트에서 미리 피 출력
	BYTE condition;			//캐릭터의 상태
	int	arigement;			//캐릭터의 가치관
	int	insertPattern;		// 
	
	DWORD	serverd_13;
	int	lostSight;			//캐릭터가 눈먼 상태 셋팅(0->평상상태/1이상->시야상실 정도)
	DWORD lostSightTime;		//시야 상실 지속 시간
	
	int shoot_PointX;		//화살 에니메이션의 발사시점에 정확히 클릭한 위치로 발사위해 사용
	int	shoot_PointY;		
	
	int	eventdelay;
	
	int	_lev;
	int _str;
	int	_Wis;				//	지혜
	int	_Int;				//	지능
	int	_wsps;				//  마법력->마법 메모라이즈
	
	int _Hp, _HpMax;
	int _Mp, _MpMax;
	int _Hungry,_HungryMax;		
	short int event_no;
	int viewdebugcharinfo;
	
	DWORD serverd_14;
	int destx;
	int desty;		// 목적 Map x,y;

	char HostName[ 21];
	//  npc움직임에 대한 변수 ...
	short patterntype, oldpatterntype;
	int targetid;
	int bossid;
	short attackcount;
	DWORD aitimedelay, aidelayhowmuch;
	int how;
	int attacked;
	DWORD pk;
	DWORD Exp;
	int MoveP;		// NPC의 움직임을 다양하게 하기 위한 변수로 쓰임..
	bool notcomeinside; //  몬스터는 집안에 들어올수 없다. 
	
	bool just_ani;	// true이면 단순 Animation만을 한다. 
	
	short social_status;
	short fame;
	short nk3;
	short nk4;
	short nk6;
	short dummy_pk;  //  별 관계 없이 경비병을건들면 중가..   새로 area에 들어오면 
	
	DWORD killpc;	   // user
	
	short int tame;
	BYTE tame_cmd;
	short int HostId;
	//char HostName[ 21];
	char TargetName[ 31];
	
	int	hpupdown; //  데미지 처리시간을 보장하기 위한 변수.
	
	int	feeldelay;
	int	feeltype;
	
	bool hunter_target;		// 내가 등록한 현상범이다 아니다, 	// 0807 YGI
	BYTE lv;//Level
	char class_type;// 클레스
	
	////////////////////// SoundUp lkh 추가(캐릭 각 동작별 고유음 출력위한 시간변수) ///////////////////////////
	DWORD naturalSound_Time;
	int	odd_Even;				//짝수번-홀수번 루프
	int	attacker_ID;			//나를 공격한 대상의 ID
	//void		*lpAttacker;			//나를 공격한 대상 포인터
	
	BOOL fight_flag;		//대결 상태체크..
	int	stone_x;		//전투돌 x 좌표..
	int	stone_y;		//전투돌 y 좌표..
	int	fight_id;		//전투할 놈..
	int	fight_time;		//전투시간..
	
	BYTE light_On;		//광원여부(주인공 캐릭과 파티/일부 NPC만 기본적으로 광원을 지니고 있음)
	DWORD castStartingTime;
	int	fight_map_team_num;		// 0906 YGI	fightmap
	int	homex, homey;
	
	int	job;			// 출력할 직업		1004 YGI
	int	call_job_lv;	// 출력할 직업 레벨
	
	int	candle;
	
	NW_Character NWCharacter;
	
	char JoinLocalWar;
	char GainedFame;	
	char ChairNum;
	
	char HorseNo;
	char RiderNo;
	char HorseGrade;
	char RiderGrade;
	char DragonLordWarTeam;
	
	BYTE aStepInfo[20];
	char reporter_mode;
	int nAttr;
	int nRace;
	int nActiveCombat;//弓盗技能地址.E4 00 00 00 00 00 00 00 00 00 00 00 01冰暴什么的.
	int nPassiveCombat;
	int nRecoveryCombat;
	int nActiveLevel;//弓盗技能等级地址.                                 
	int nPassiveLevel;
	int nRecoveryLevel;
	bool bCrash;
	bool bShoot;
	bool bSelect;
	bool bLock;	
	WORD idWeaponItem; // CSD-031001
	int	nCurrentFrame;		//人物晃动值

	//-----------fqyd 060730--
	//------关键信息在这里备份------
	int mutant_res;//转职
	int movp_res;	//移动力
	int nAttackedType_res;//攻击类型
	int bCanViewGhost_res;//看鬼开关
	int Sp_res;				//SP...就是SP喽
	int sight_res;			//视野
	int excute_MagicNum_res;//释放魔法类型,保存
	int nActiveCombat_res;//弓盗技能地址.E4 00 00 00 00 00 00 00 00 00 00 00 01冰暴什么的.
	int nActiveLevel_res;//弓盗技能等级地址.
	int	nCurrentAction_res;		//人物动作行为
	int	nCurrentFrame_res;		//人物晃动值
	int	bow_Type_res;		// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.

///////////////*******************新加的2009.7.14月 褚?**************************
	int	ready_Magic_res;		//0->마법 무 /1->마법 casting 중/2->마법 casting 유지 중/3->마법 발사
	DWORD start_Casting_res;		//casting 시작 시간
	DWORD end_Casting_res;		//casting 준비 완료 시간	
	DWORD end_ReadyMagic_res;		//casting 유지 종료될 시간 	
	DWORD end_RunMagic_res;		//마법 효과 끝나는 시간
	DWORD castStartingTime_res; //魔法间隔时间?

////////////////////////////////////////////////////////////////////////////////////////////////
	//char	pluse_value_res;
	DWORD	continue_RunMagic_res;//魔法持续时间

	int Check()
	{//对应的检查函数
	mutant_res^=mutant_MSK_CA;//转职
	if(mutant!=mutant_res)
		return 0;
	mutant_cont();				//随机函数
	mutant_res^=mutant_MSK_CA;

	movp_res^=movp_MSK_CA;//移动
	if(movp!=movp_res)
		return 1;
	movp_cont();				//随机函数
	movp_res^=movp_MSK_CA;

	nAttackedType_res^=nAttackedType_MSK_CA;//攻击类型
	if(nAttackedType!=nAttackedType_res)
		return 2;
	nAttackedType_cont();				//随机函数
	nAttackedType_res^=nAttackedType_MSK_CA;
	
	bCanViewGhost_res^=bCanViewGhost_MSK_CA;//看鬼
	if(bCanViewGhost!=bCanViewGhost_res)
		return 3;
	bCanViewGhost_cont();				//随机函数
	bCanViewGhost_res^=bCanViewGhost_MSK_CA;

	//Sp_res^=MSK_CA;
	//if(_Sp!=Sp_res)
	//	return 0;
	//Sp_res^=MSK_CA;
	//
	sight_res^=sight_MSK_CA;//视野
	if(sight!=sight_res)
		return 4;
	sight_cont();				//随机函数
	sight_res^=sight_MSK_CA;

	excute_MagicNum_res^=excute_MagicNum_MSK_CA;//释放魔法类型
	if(excute_MagicNum!=excute_MagicNum_res)
		return 5;
	excute_MagicNum_cont();				//随机函数
	excute_MagicNum_res^=excute_MagicNum_MSK_CA;
	
	continue_RunMagic_res^=continue_RunMagic_MSK_CA;//魔法持续时间
	if(continue_RunMagic_res!=continue_RunMagic)
		return 6;
	continue_RunMagic_cont();				//随机函数
	continue_RunMagic_res^=continue_RunMagic_MSK_CA;

	nActiveCombat_res^=nActiveCombat_MSK_CA;//弓盗技能地址.E4 00 00 00 00 00 00 00 00 00 00 00 01冰暴什么的.
	if(nActiveCombat!=nActiveCombat_res)
		return 7;
	nActiveCombat_cont();				//随机函数
	nActiveCombat_res^=nActiveCombat_MSK_CA;
	
	nActiveLevel_res^=nActiveLevel_MSK_CA;////弓盗技能等级地址.
	if(nActiveLevel!=nActiveLevel_res)
		return 8;
	nActiveLevel_cont();				//随机函数
	nActiveLevel_res^=nActiveLevel_MSK_CA;
	
	nCurrentAction_res^=nCurrentAction_MSK_CA;//人物动作行为
	if(nCurrentAction!=nCurrentAction_res)
		return 9;
	nCurrentAction_cont();				//随机函数
	nCurrentAction_res^=nCurrentAction_MSK_CA;
	
	nCurrentFrame_res^=nCurrentFrame_MSK_CA;//人物晃动值
	if(nCurrentFrame!=nCurrentFrame_res)
		return 10;
	nCurrentFrame_cont();				//随机函数
	nCurrentFrame_res^=nCurrentFrame_MSK_CA;
	
	bow_Type_res^=bow_Type_MSK_CA;// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.
	if(bow_Type!=bow_Type_res)
		return 11;
	bow_Type_cont();				//随机函数
	bow_Type_res^=bow_Type_MSK_CA;

/////////////////////**新加的<月 褚?2009.7.14.//////////////
	ready_Magic_res^=ready_Magic_MSK_CA;// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.
	if(ready_Magic!=ready_Magic_res)
		return 12;
	ready_Magic_cont();				//随机函数
	ready_Magic_res^=ready_Magic_MSK_CA;
				
	start_Casting_res^=start_Casting_MSK_CA;// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.
	if(start_Casting!=start_Casting_res)
		return 13;
	start_Casting_cont();				//随机函数
	start_Casting_res^=start_Casting_MSK_CA;
	
	end_Casting_res^=end_Casting_MSK_CA;// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.
	if(end_Casting!=end_Casting_res)
		return 14;
	end_Casting_cont();				//随机函数
	end_Casting_res^=end_Casting_MSK_CA;
							
	end_ReadyMagic_res^=end_ReadyMagic_MSK_CA;// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.
	if(end_ReadyMagic!=end_ReadyMagic_res)
		return 15;
	end_ReadyMagic_cont();				//随机函数
	end_ReadyMagic_res^=end_ReadyMagic_MSK_CA;

	end_RunMagic_res^=end_RunMagic_MSK_CA;// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.
	if(end_RunMagic!=end_RunMagic_res)
		return 16;
	end_RunMagic_cont();				//随机函数
	end_RunMagic_res^=end_RunMagic_MSK_CA;
	
	castStartingTime_res^=castStartingTime_MSK_CA;// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.
	if(castStartingTime!=castStartingTime_res)
		return 17;
	castStartingTime_cont();				//随机函数
	castStartingTime_res^=castStartingTime_MSK_CA;
	
		return 18;
}

	//下面这些都是截获程序调用,并保存备份的函数.
	int set_mutant(int m)//转职
	{
		mutant=m;
		mutant_res=m;
		mutant_res^=mutant_MSK_CA;
		return 1;
	}
	int set_movp(int	m)//移动
	{
		movp=m;
		movp_res=m;
		movp_res^=movp_MSK_CA;
		return 1;
	}
	int set_nAttackedType(int n)//攻击类型
	{
		nAttackedType=n;
		nAttackedType_res=n;
		nAttackedType_res^=nAttackedType_MSK_CA;
		return 1;
	}
	int set_bCanViewGhost(bool c)//看鬼
	{
		bCanViewGhost=c;
		bCanViewGhost_res=c;
		bCanViewGhost_res^=bCanViewGhost_MSK_CA;
		return 1;
	}
	//int set_Sp(int sp)//SP
	//{
	//	Sp=sp;
	//	Sp_res=sp;
	//	Sp_res^=Sp_MSK_CA;
	//	return 1;
	//}
	
	int set_sight(int	s)//视野
	{
		sight=s;
		sight_res=s;
		sight_res^=sight_MSK_CA;
		return 1;
	}
	int set_excute_MagicNum(int e)//魔法类型
	{
		excute_MagicNum=e;
		excute_MagicNum_res=e;
		excute_MagicNum_res^=excute_MagicNum_MSK_CA;
		return 1;
	}
	int set_continue_RunMagic(DWORD cr)//魔法持续时间
	{
		continue_RunMagic=cr;
		continue_RunMagic_res=cr;
		continue_RunMagic_res^=continue_RunMagic_MSK_CA;
		return 1;
	}
	int set_nActiveCombat(int ac)//弓盗技能
	{
		nActiveCombat=ac;
		nActiveCombat_res=ac;
		nActiveCombat_res^=nActiveCombat_MSK_CA;
		return 1;
	}
	int set_nActiveLevel(int al)//弓盗技能等级
	{
		nActiveLevel=al;
		nActiveLevel_res=al;
		nActiveLevel_res^=nActiveLevel_MSK_CA;
		return 1;
	}
	int set_nCurrentAction(int ca)//人物动作行为
	{
		nCurrentAction=ca;
		nCurrentAction_res=ca;
		nCurrentAction_res^=nCurrentAction_MSK_CA;
		return 1;
	}
	int set_nCurrentFrame(int cf)//人物晃动值
	{
		nCurrentFrame=cf;
		nCurrentFrame_res=cf;
		nCurrentFrame_res^=nCurrentFrame_MSK_CA;
		return 1;
	}
	int set_bow_Type(int bw)//// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.
	{
		bow_Type_res=bw;
		bow_Type_res=bw;
		bow_Type_res^=bow_Type_MSK_CA;
		return 1;
	}
///////////////*******************新加的2009.7.14月 褚?**************************
	int set_ready_Magic(int rm)//// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.
	{
		ready_Magic_res=rm;
		ready_Magic_res=rm;
		ready_Magic_res^=ready_Magic_MSK_CA;
		return 1;
	}
	int set_start_Casting(int sc)//// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.
	{
		start_Casting_res=sc;
		start_Casting_res=sc;
		start_Casting_res^=start_Casting_MSK_CA;
		return 1;
	}
	int set_end_Casting(int ec)//// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.
	{
		end_Casting_res=ec;
		end_Casting_res=ec;
		end_Casting_res^=end_Casting_MSK_CA;
		return 1;
	}
	int set_end_ReadyMagic(int er)//// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.
	{
		end_ReadyMagic_res=er;
		end_ReadyMagic_res=er;
		end_ReadyMagic_res^=end_ReadyMagic_MSK_CA;
		return 1;
	}
	int set_end_RunMagic(int er)//// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.
	{
		end_RunMagic_res=er;
		end_RunMagic_res=er;
		end_RunMagic_res^=end_RunMagic_MSK_CA;
		return 1;
	}
	int set_castStartingTime(int cst)//// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.
	{
		castStartingTime_res=cst;
		castStartingTime_res=cst;
		castStartingTime_res^=castStartingTime_MSK_CA;
		return 1;
	}

	
////////////////////////////////////////////////////////////////////////////////////////////////

	DWORD reserved_1024;	//将内存数据推后4位...没什么屁用.关键要改变结构...结构!
	CCharacter* lpPrev;
	CCharacter* lpNext;

};

typedef CCharacter	CHARACTER;
typedef CCharacter*	LPCHARACTER;
//> CSD-031001
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct	tagCHARACTERLIST
{
	int				nCount;
	LPCHARACTER		lpFirst, lpLast;
	tagCHARACTERLIST()
	{
		nCount = 0;
		lpFirst = NULL;
		lpLast = NULL;

	}
} CHARACTERLIST, *LPCHARACTERLIST;

#endif	//__MYTYPES_H__

// 010505 KHS
#if defined(TAIWAN_LOCALIZING_ ) || defined( CHINA_LOCALIZING_ ) || defined (HONGKONG_LOCALIZING_)//010730 lms

//#define CHANGE_MONEY_//020730 lsw 돈 통일
#else

#define CHANGE_MONEY_	

#endif

