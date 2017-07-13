/*

*/

#ifndef __MYTYPES_H__
#define	__MYTYPES_H__


#define _MAPOBJECT_DIRECTORY_CHANGE_
//#define BETA_TESTER_
#define	CON_NORMAL	0x00 // 沥惑惑怕	 0000000
#define	CON_CURSE		0x01 // 劝悼利惑怕 0000001
#define	CON_FROZEN	0x02 // 公扁仿惑怕 0000010		
#define	CON_SLOW		0x04 // 浇肺快惑怕 0000100	
#define	CON_DAZE		0x08 // 去鄂惑怕	 0001000	
#define	CON_POISON	0x10 // 吝刀惑怕	 0010000
#define	CON_STONE		0x20 // 籍拳惑怕	 0100000
#define CON_DEATH		0x40 // 磷澜惑怕	 1000000


//< CSD-030419 : NPC 辆练
#define DUMMY	        0
#define HUMAN	        1
#define GUARD			2
#define ORC_DUMMY		3
#define LIFELESS		4
#define HOBBIT			5
#define DWARF	        6
#define DRAGON			7 
#define ANIMAL			8
#define NORM_MON		9
#define SEALSTONE		11
#define GUARDSTONE		12
#define CASTLEGATEWAY	12
#define WARARMS			12
#define GUARDTOWER		13


//> CSD-030419

////////////////////////// 捞痹绕 1230 ///////////////////////////////////////////
#define		SET_LOSTSIGHT	1			//矫仿惑角
//////////////////////////////////////////////////////////////////////////////////
#define		LIGHTNESS		2
/////////////////////////////////////////////////////////////////////////////		


////////////////////////// 0402 捞痹绕 眠啊 //////////////////////////////////////
#define		SET_CUREBLIEND	3
#define		SET_INFORMATION	4

////////////////////////// 0811 YJS 眠啊 //////////////////////////
#define		FIGHT_RESULT	5

#define		FIGHT_WIN		1
#define		FIGHT_LOSE		2
#define		FIGHT_DRAW		3
#define		FIGHT_LOGOUT	4




//////////////// 0311 lkh 眠啊 (捞亥飘侩 澜厩 免仿) //////////////
#define		OPERATOR_PLAYSOUND_EVENT1	851		//青荤 矫累阑 舅府绰 绵啊
#define		OPERATOR_PLAYSOUND_EVENT2	853		//冠荐 家府
#define		OPERATOR_OUTPUT_CELEBRATE	290		//绵器甫 筋酒 棵捞绰 瓤苞

/////////////// 0810 lhs 眠啊 /////////////////
#define		START_QUEST_SOUND			15000		//涅胶飘 矫累 家府
#define		QUEST_SALUTE				15001		//涅胶飘侩 绵器 瓤苞



#define	TILE_SIZE					32
#define	VILLAGE_SIZE				720


#define MAX_LOADSTRING				100
#define	MAX_CHAR_IMAGE_FRAME		60
#define	MAX_ANIMATION				26//24				// LTS ACTION
#define	MAX_PC_ANIMATION_FRAME		40
#define	MAX_SHORTPATH				50
#define	MAX_DIRECTION				5
#define	MAX_CHAT_STRING				256

#define	SCREENMODE_WINDOW			0
#define	SCREENMODE_FULLSCREEN		1


#define	SCREEN_WIDTH				800
#define	SCREEN_HEIGHT				600

#define GAME_SCREEN_XSIZE			800
#define GAME_SCREEN_YSIZE			520// 352//020515 lsw
/*
#define	SCREEN_WIDTH				1024
#define	SCREEN_HEIGHT				768

  #define GAME_SCREEN_XSIZE			1024
  #define GAME_SCREEN_YSIZE			(768-(600-472))// 352
*/


#define RADARXL 640
#define RADARYL 480

// 檬焊磊狼 Level
#define ROOKIE_LEVEL				7



#define GAME_SCREEN_XTILE			(GAME_SCREEN_XSIZE/TILE_SIZE)
#define GAME_SCREEN_YTILE			(GAME_SCREEN_YSIZE/TILE_SIZE)

#define GAME_SCREEN_XTILE_MID		(GAME_SCREEN_XTILE/2)
#define GAME_SCREEN_YTILE_MID		(GAME_SCREEN_YTILE/2)

//////////////////////// 0808 lkh 眠啊 /////////////////////////////
#define OBJECTSOUNDPLAY_XSIZE		1400
#define OBJECTSOUNDPLAY_YSIZE		1000

#define	PC_WIDTH					45
#define	PC_HEIGHT					64
#define	SIZE_AREA_WIDTH				10
#define	SIZE_AREA_HEIGHT			10


#define STATE_BUTTON_STILL			0
#define	STATE_BUTTON_PRESSED		1
#define	STATE_BUTTON_RELEASED		2
#define STATE_BUTTON_DOUBLECLICK	3




#define	CTYPE_PC					 0
#define	CTYPE_NPC					 1
#define	CTYPE_MONSTER				 2


#define	SPRITETYPE_NONE				 0		// 付快胶俊 狼秦 CHARACTER急琶登绰 风凭俊辑 荤侩
#define	SPRITETYPE_CHARACTER		 1 		// 免仿.. Sortting.. 
#define	SPRITETYPE_NPC				 2
#define	SPRITETYPE_MONSTER			 3
#define	SPRITETYPE_OBJECT			 4
#define	SPRITETYPE_OBJECT_BOTTOM	 5  
#define	SPRITETYPE_TILE				 6
#define SPRITETYPE_ITEM				 7
#define SPRITETYPE_ITEM_BOTTOM		 8
#define SPRITETYPE_EFFECT_NORMAL	 9
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
#define PC_STATUS_PEACE 			0
#define PC_STATUS_BATTLE			1


#define SIGHT_PLUS					90000


#define EVENT_TYPE_NPC			0
#define EVENT_TYPE_MAPTILE		1


// 傍拜且锭 某腐磐啊 傍拜裹困俊 乐绰瘤 绝绰瘤俊 措茄 府畔蔼. 
// IsAttackRange()俊辑 荤侩.....
#define ATTACKRANGE_IN_AREA_		1
#define ATTACKRANGE_IN_MIN_			2
#define ATTACKRANGE_OUT_MAX_		3

#define ERR_FILE_OPEN		-1
#define ERR_INVALID_ID		-11

enum ViewTypes
{	//< CSD-030509
	VIEWTYPE_NORMAL_ = 0,
	VIEWTYPE_CAMELEON_ = 1,
	VIEWTYPE_GHOST_	= 2,
	VIEWTYPE_TREE_ = 3,
	VIEWTYPE_OBSERVE_ = 4,
	VIEWTYPE_CONFUSION_	= 5,
	VIEWTYPE_POISON_ = 6,
	VIEWTYPE_STONE_ = 7,
	VIEWTYPE_SLOW_ = 8,
	VIEWTYPE_FROZEN_ = 9,
	VIEWTYPE_CURSE_	= 10,
	VIEWTYPE_TRANSPARENCY_ = 11,
	VIEWTYPE_SELECT_ = 12,
	VIEWTYPE_STAND_UP_ = 13,
};	//> CSD-030509

enum eChatTypes
{
	CHATTYPE_NORMAL				=0,
	CHATTYPE_WHISPER			=1,
	CHATTYPE_PARTY				=2,
	CHATTYPE_GUILD				=3,
	CHATTYPE_COLOSSUS_GLOBAL	=4,
	CHATTYPE_COLOSSUS			=5,
	CHATTYPE_LOCAL_WAR			=6,
	CHATTYPE_NATION_WAR			=7,
	CHATTYPE_GUILD_BBS			=8,
	CHATTYPE_YES_OR_NO			=9,
	CHATTYPE_NOTICE				=10,
	CHATTYPE_MERCHANT_BBS		=11,
	CHATTYPE_TALK_WITH_GM		=12,
};

// 乔纳捞茄 某腐磐狼 倔奔荤柳俊 静老 漠磊惫 Sprite锅龋.
#define PK_BLOOD_EFFECT		272
// 酒贰啊 沥狼登绢 乐栏搁 惫啊俺充捞 利侩等促. 
#define _NATION_APPLY_		
// 酒贰啊 沥狼登绢 乐栏搁 箭磊肺 免仿等促. 
#define DISPLAY_HIT_NUMBER_	

enum 
{
	CLS_STEP = 0,  // 努贰胶 窜拌
	DUAL_CLS = 1,  // 掂倔 努贰胶
	EXP_STEP = 2,  // 版氰摹 窜拌
	T01_STEP = 3,  // 琶平 版氰摹 窜拌
	T02_STEP = 4,
	T03_STEP = 5,
	T04_STEP = 6,
	T05_STEP = 7,
	T06_STEP = 8,
	T07_STEP = 9,
	T08_STEP = 10,
	T09_STEP = 11,
	T10_STEP = 12,
	T11_STEP = 13,
	T12_STEP = 14,
	T13_STEP = 15,
	CSP_STEP = 16, // 傈捧胶懦 器牢磐
	MAX_STEP = 17  // 弥措 单捞鸥荐
};

struct CCharRank
{	//< CSD-030326
	DWORD nation	  :3;  // 加茄唱扼 - 0:绝澜, 1, 2, 3:官捞辑胶, 4:磊捞戚, 5:庆霸葛聪酒, 6:老胶
	DWORD guild_master:3;  // 辨靛 付胶磐
	DWORD king		  :1;  // 唱扼 空
	DWORD dwReserved  :9;  // 荤侩登瘤 臼澜
	DWORD counselor	  :2;  // 0 : 绝澜, 1: 雀荤 款康磊, 2: 霸烙郴 款康磊
	DWORD ultra_nk3	  :1;  // 必厩乔纳捞	1004 YGI
	DWORD ultra_nk4	  :1;  // 必厩乔纳捞	1004 YGI
	DWORD ultra_nk6	  :1;  // 必厩乔纳捞	1004 YGI
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
	DIRECTION_UP		=   4,
	DIRECTION_RIGHTUP	=   5,
	DIRECTION_RIGHT		=   6,
	DIRECTION_RIGHTDOWN	=   7,
	DIRECTION_DOWN		=   0,
	DIRECTION_LEFTDOWN	=   1,
	DIRECTION_LEFT		=   2,
	DIRECTION_LEFTUP	=   3,
	
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
	ACTION_NONE = 0,					//   1 扁夯悼累  1
	ACTION_BASIC_WALK,					//   2 扁夯叭扁  2
	ACTION_BATTLE_WALK_OWNHAND,			//   3 傈捧叭扁  3
	ACTION_BATTLE_NONE,					//   4 傈捧扁夯  4
	ACTION_ATTACK_ONESWORD1,			//   5 茄颊傍拜  5
	ACTION_ATTACK_ONESWORD2,			//   6 茄颊傍拜2  
	ACTION_ATTACK_BONG_DOWN,			//   7 豪, 帮豪,  傍拜( 郴妨摹扁 )
	ACTION_ATTACK_BONG_JJIRKI,			//   8 芒, 豪, 傍 拜( 骂福扁 )
	ACTION_ATTACK_BOW,					//   9 劝筋扁      
	ACTION_ATTACK_PENSWORD2,			//  10 奇教磊技   
	ACTION_ATTACK_PENSING1,				//  11 奇教傍拜 1 
	ACTION_ATTACK_PENSING2,				//  12 奇教傍拜 2 
	ACTION_RUN,						    //  13 顿扁
	ACTION_ATTACKED,				    //  14 嘎扁
	ACTION_DIE,						    //  15 磷扁
	ACTION_MAGIC_BEFORE,				//  16 付过矫傈
	ACTION_MAGIC_CASTING,				//  17 付过某胶泼
	ACTION_SITDOWN,						//  18 臼扁
	ACTION_THROW,					    //  19 带瘤扁傍拜
	ACTION_HORSE,					    //  20 富鸥扁
	ACTION_BATTLE_WALK_TWOHAND,			//  21 傈捧叭扁 2 ( 剧颊俊 八 )
	ACTION_BATTLE_NONE_TWOHAND,			//  22 傈捧扁夯 2( 剧颊 )
	ACTION_ATTACK_BONG_ROUND,			//  23 豪, 芒, 帮豪, 傍拜( 戎滴福扁 )
	ACTION_MAGIC_CASTING2,				//  24 付过某胶泼 2(巢磊 某腐磐俊霸父 利侩 )
	ACTION_ANGER,						//	25 拳郴扁			// LTS ACTION
	ACTION_VICTORY						//  26 铰府器令			// LTS ACTION
} ACTION_PC_WOMAN;

enum ACTION_NPC_MONSTER1
{
	MON1_NONE = 0,			// 扁夯悼累
	MON1_BATTLE_WALK,		// 扁夯叭扁
	MON1_ATTACKED,			// 嘎扁
	MON1_DIE,			    // 磷扁....
	MON1_BATTLE_ATTACK,		// 茄颊傍拜
	MON1_BATTLE_MAGIC,		// 付过矫傈
	MON1_STAND_UP,			// 老绢辑扁
	MON1_CASTING_MAGIC,		// 付过矫傈	// LTS DRAGON
	MON1_BATTLE_ATTACK2,	// 傍拜 2
	MON1_BATTLE_ATTACK3,	// 傍拜 3
	MON1_JUST_ACTION		// 烙狼狼 悼累
		
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
	DWORD		WarNo						:	7;	//	傈里锅龋
	DWORD		WarfieldNo					:	3;	//	傈里磐 锅龋
	DWORD		SquadNo						:	3;	//	何措锅龋
	DWORD		isSquadLeader				:	1;	//	何措厘牢啊?	
	DWORD		isCommander					:	1;	//	荤飞包牢啊?
	DWORD		isCommanderCandidater		:	1;	//	荤飞包饶焊牢啊?
	DWORD		DoWarJoinVote				:	1;	//	傈里磐 曼咯 捧钎甫 沁唱?
	DWORD		DoCommanderVote				:	1;	//	荤飞包 捧钎甫 沁促
	DWORD		YL_JoinNation				:	4;	//	老胶牢版快 磊捞戚俊沁唱? 官捞辑胶俊 沁唱?  // 惫啊锅龋
	DWORD		Reserved					:	10;	//	抗距..
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
	int	mantle;			// 011018 KHS 噶肚
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
	
	NW_Character NWCharacter;		// 010915 LTS		// UNROLL阑 荤侩窍瘤 臼绰 版快, 滚弊狼 惯积捞 快妨 
	
	BYTE nDualCls;		// 掂倔包访
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
	
	BYTE gender;					// 0:咯磊 1 : 巢磊
	BYTE Race;						// 辆练
	BYTE Spiritvalue;				// 啊摹包
	BYTE Class;						// 0:傈荤 1:泵荐 2:档利 3:己流磊 4:付过荤
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
	
	BYTE clothr, clothg, clothb;	// 鹅 祸彬 R.G.B
	BYTE bodyr, bodyg, bodyb;		// 个 祸彬 R.G.B
	
	short Age;
	
	BYTE  Level;
	DWORD Exp;
	
	short Hp;
	short AttackPow;		// 傍拜仿
	
	short Mana;				// 版氰摹
	short VitalPow;			// 积疙仿
	short Hungry;
	short Nk3, Nk4, Nk6;	// 阿 惫啊喊 快龋档  010531 KHS
	
	DWORD Killmon1;			// 1拌凯阁胶磐 懦傅胶
	DWORD Killmon2;			// 2拌凯阁胶磐 懦傅胶
	DWORD Killmon3;			// 3霸凯阁胶磐 懦傅荐
	DWORD Killmon4;			// 4霸凯阁胶磐 懦傅荐
	DWORD Killmon5;			// 5霸凯阁胶磐 懦傅荐
	DWORD Killmon6;			// 6霸凯阁胶磐 懦傅荐
	DWORD Killanimal;		// 悼拱 懦傅荐
	DWORD KillNPC;			// 荤恩,NPC 懦傅荐
	
	DWORD Money;
	
	//弥檬某腐磐 积己且锭狼 荐摹..
	short StrOld;				//  塞
	short ConOld;				//  扒碍
	short DexOld;				//	刮酶己
	short WisOld;				//	瘤驱
	short IntOld;				//	瘤瓷
	short MovePOld;				//	捞悼仿
	short CharOld;				//	概仿
	short EnduOld;				//  历氢仿
	short HealthOld;			//	眉仿
	short MoralOld;				//	荤扁
	short LuckOld;				//	青款
	
	short Str;				//  塞
	short Con;				//  扒碍
	short Dex;				//	刮酶己
	short Wis;				//	瘤驱
	short Int;				//	瘤瓷
	short MoveP;			//	捞悼仿
	short Char;				//	概仿
	short Endu;				//  历氢仿
	short Health;			//	眉仿
	short Moral;			//	荤扁
	short Luck;				//	青款
	short wsps;
	
	
	short Resis_Poision;	//	刀/磷澜 历亲仿
	short Resis_Stone;		//	籍拳 付厚 历亲仿
	short Resis_Magic;		//	付过历亲仿
	short Resis_Fire;		//	阂 历亲仿
	short Resis_Ice;		//	倔澜历亲仿
	short Resis_Elect;		//	傈扁历氢仿
	short Wizardspell;		//	Ws
	short Ws[200];			//	付过
	short Priestspell;		//	Ps
	short Ps[200];			//	脚过
	
	// 郴啊 啊瘤绊 乐绰 酒捞袍..
	DWORD Item[ 13][100];
	
	DWORD bankmoney;
	
}CHARELEMENT, *LPCHARELEMENT;

//const	DWORD	ID_MSK=0x7eb29b9e;	//加密关键字,可以适当修改
///////////////////////////////////////////////////////////////////////////////
//< CSD-031001

//----------------------

//const		DWORD		NAME_MSK=0X9B2BACD6;	//已经不用
//--bu fqyd 060730
//#define		MSK_CA		0x93cf6d //过去的异或数,由于比较容易被破解,所以改成了以下的随机函数
///////////////////////////////////////////////////////////////////这里定义的是随机异或的函数/////////////////////////////////////////////
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
extern int bCheckSalvation_MSK_CA;//无限名声FFH,FFH,FFH,FFH档
//////////////////////////////////////////////*******************新加的*******************
extern int	ready_Magic_MSK_CA;		//0->付过 公 /1->付过 casting 吝/2->付过 casting 蜡瘤 吝/3->付过 惯荤
extern DWORD start_Casting_MSK_CA;		//casting 矫累 矫埃
extern DWORD end_Casting_MSK_CA;		//casting 霖厚 肯丰 矫埃	
extern DWORD end_ReadyMagic_MSK_CA;		//casting 蜡瘤 辆丰瞪 矫埃 	
extern DWORD end_RunMagic_MSK_CA;		//付过 瓤苞 场唱绰 矫埃
extern DWORD castStartingTime_MSK_CA;		//付过 瓤苞 场唱绰 矫埃
////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////随机数字的函数声明//////////////////////////////////////////////////////
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
extern	void bCheckSalvation_cont(void);			//无限名声FFH,FFH,FFH,FFH档
//////////////////////////////////////////////*******************新加的*******************
extern	void ready_Magic_cont(void);		//0->付过 公 /1->付过 casting 吝/2->付过 casting 蜡瘤 吝/3->付过 惯荤
extern	void start_Casting_cont(void);		//casting 矫累 矫埃
extern	void end_Casting_cont(void);		//casting 霖厚 肯丰 矫埃	
extern	void end_ReadyMagic_cont(void);		//casting 蜡瘤 辆丰瞪 矫埃 	
extern	void end_RunMagic_cont(void);		//付过 瓤苞 场唱绰 矫埃
extern	void castStartingTime_cont(void);		//间隔时间?
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*//extern		char		g_name_0[31];			//不用

typedef struct _SChName
{
	char	name[31];							
}SChName;

extern std::vector<SChName>		*pChNames;
//-------------------------*/
class CCharacter     //这个类里面包括了人物名下边的所有信息,晃动值,人物ID,怪物ID等.且都是按顺序
					//排列的.可以改变其顺序就可以改变数据所在位置
{//角色类
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
/*	int id()
	{
		DWORD	temp=xid;
		temp=temp^ID_MSK;
		return(int)temp;
	}
	int set_id(int id)
	{
		//DWORD t=(DWORD)id;
		xid=((DWORD)id)^ID_MSK;	//这里简单的异或加密
		return 1;
	}

	int nCurrentFrame()//人物晃动值
	{
		DWORD	temp=xCF;
		temp=temp^ID_MSK;
		return(int)temp;
	}
	int set_nCurrentFrame(int CF)
	{
		//DWORD t=(DWORD)id;
		nCurrentFrame()=((DWORD)CF)^ID_MSK;	//这里简单的异或加密
		return 1;
	}




char	*get_name()
{
	//std::vector<SChName>::iterator	it;
	return(*pChNames)[this->name_ptr_no].name;
}

int set_name(char *name)
{	
	SChName		temp;
	memcpy(temp.name,name,20);
	//for(int i=0;i<7;i++)
	//((DWORD*)name_0)[i]^NAME_MASK;
	pChNames->push_back(temp);//保存到一个统一的容器里面了

	this->name_ptr_no=pChNames->size()-1;
	return 1;
}*/

private:
//	DWORD		server_19;
	int			m_nGuildCode;
//	DWORD		xid;  //角色或者怪物的ID
//	DWORD		xCF;  //人物晃动值

public:
	CCharRank name_status;  //地位

	char type;
	int id;
	char name[ 31];
//	int name_ptr_no;	//只返回了一个统一的号码
	char namedisplaycount;
	int	sprno;
	int	mutant;   //0810 KHS  //转职,保存
	int	nutantani;	// 0810 KHS
	short age;
	// 010515 KHS
	bool eventnpc;
	
	short nCharacterData[20];
	int	animationType;
	int	AIanimationRest;	//  0 酒聪搁  框流捞瘤 臼绊 弊措肺 辑乐绰促. 
	int	AIattackTargetID;	//	绢恫仇阑 Mon1狼 框流烙俊辑 0 酒聪搁  框流捞瘤 臼绊 弊措肺 辑乐绰促. 
	short movetargetid;
	bool sealstone;
	short sealstoneani;
	char GuardStoneAni;	// 010915 LTS	// LTS SEALSTONE
	char SetAni2Zero;
	
	//DOWRD	reserved_2; //无用.//这里几个变量也是,可以加加
	int x;
	int	restdx;
//	int	movp;//移动力,需要保存
	int	ddx;
	short int tox,toy; //可以改变变量的位置
	bool toflag;
	int y;
	
	int	height;
	int	oldgox, oldgoy;
	int	gox, goy;
	DWORD desttime;
	char destflag;
	int	sex;
	int	walktime;
///	DOWRD	reserved_3; //无用.//这里几个变量也是,可以加加
	POINT position,oldposition;
	POINT offset;
	DIRECTION direction;
	DIRECTION todir; // direction篮 todir肺 官操妨绰己龙捞 乐促. 
	short dirdelay;
	
//	int __movp; //移动力,需要保存
	int	light;  //应该是照明吧...
	int	attackMinRange;
	int	attackMaxRange;
	int	changeActionFlag;// 1捞搁 悼累吝俊 促弗悼累栏肺 官拆荐 绝促.
	
//	int	nAttackedType;//攻击类型,保存
	void* lpAttacked;
	
	short NumberAnimationValue;
	BYTE attackedresult;
	short flagattacked;//
	bool attacksuccess;
	char attackanimationON;
	char pluse_value;
	char willdie; // 捞荐摹啊 1捞登搁 磷绰促. 
	
	int	nCurrentAction;		//人物动作值
	int	movp;//移动力,需要保存
	
	//int	nCurrentFrame;		//人物晃动值
	bool bCanViewGhost;    //看鬼开关,需要保存
	int	nAttackedType;//攻击类型,保存
	int	bow_Type;		// 魔法弓箭,0代表没有,1代表火箭,2代表冰箭.
	int TotalAni;
	PCANIMATIONTABLE AnimationTable[ MAX_ANIMATION ];
	
	Spr	*sp, *sp0, *sp1, *sp2, *sp3, *sp4;
	
	int	pathcount;
	int	oldpathcount;
	int	path[MAX_SHORTPATH][2];
	bool moveFlag; //捞悼矫 Tile狼 困摹甫 扁撅窍咯 沥犬洒 弊镑栏肺 埃促. 
	BYTE movetype; // 0:walk 1:run 2:horse riding.
	
	short basicAction;
	short basicWalk;
	char viewtype;
	BYTE canview;
	short canviewitem;
	//020320 lsw
//	bool bCanViewGhost;    //看鬼开关,需要保存
//	DOWRD	serverd_5;
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
	int	mantle;	// 011018 KHS 噶肚
	int	jjing;	// 011030 KHS JJING
	
	DWORD bodycolor;
	DWORD clothcolor;
	DWORD mantlecolor;	// 011018 KHS 噶肚	
	
	int	hp,mp,Sp;//sp需要保存
	int	hpmax,mpmax,Spmax;
	int	dieani;
	int	diedelay;
	int	dieanitype;
	char boxitembreaktype;
	
	BYTE wisdom;
	BYTE intelligence;
	BYTE health;				//硅绊悄 or Health
	BYTE con;			//扒碍档
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
	
	int	excute_MagicNum;	//泅犁 矫傈磊啊 霖厚窍绊 乐绰 概流锅龋//释放魔法类型,保存
	int	ready_Magic;		//0->付过 公 /1->付过 casting 吝/2->付过 casting 蜡瘤 吝/3->付过 惯荤
	int	ready_Magic_rare;
	DWORD start_Casting;		//casting 矫累 矫埃
	DWORD continue_Time;
	DWORD end_Casting;		//casting 霖厚 肯丰 矫埃
	
	DWORD start_ReadyMagic;	//casting 蜡瘤 矫累等 矫埃
	DWORD end_ReadyMagic;		//casting 蜡瘤 辆丰瞪 矫埃 
	
	DWORD start_RunMagic;		//角力 付过 瓤苞啊 矫累等 矫埃
	DWORD continue_RunMagic; 
	DWORD end_RunMagic;		//付过 瓤苞 场唱绰 矫埃
	
	int	inc_Num;
	
	bool blood_Set;			//辑滚肺何磐 磷菌促绰 皋矫瘤 朝扼坷扁 傈俊 努扼捞攫飘俊辑 固府 乔 免仿
	BYTE condition;			//某腐磐狼 惑怕
	int	arigement;			//某腐磐狼 啊摹包
	int	insertPattern;		// 
	
	int	lostSight;			//某腐磐啊 传刚 惑怕 悸泼(0->乞惑惑怕/1捞惑->矫具惑角 沥档)
	DWORD lostSightTime;		//矫具 惑角 瘤加 矫埃
	
	int shoot_PointX;		//拳混 俊聪皋捞记狼 惯荤矫痢俊 沥犬洒 努腐茄 困摹肺 惯荤困秦 荤侩
	int	shoot_PointY;		
	
	int	eventdelay;
	
	int	_lev;
	int _str;
	int	_Wis;				//	瘤驱
	int	_Int;				//	瘤瓷
	int	_wsps;				//  付过仿->付过 皋葛扼捞令
	
	int _Hp, _HpMax;
	int _Mp, _MpMax;
	int _Hungry,_HungryMax;		
	short int event_no;
	int viewdebugcharinfo;
	
	int destx;
	int desty;		// 格利 Map x,y;
	
	char HostName[ 21];
	//  npc框流烙俊 措茄 函荐 ...
	short patterntype, oldpatterntype;
	int targetid;
	int bossid;
	short attackcount;
	DWORD aitimedelay, aidelayhowmuch;
	int how;
	int attacked;
	DWORD pk;
	DWORD Exp;
	int MoveP;		// NPC狼 框流烙阑 促剧窍霸 窍扁 困茄 函荐肺 静烙..
	bool notcomeinside; //  阁胶磐绰 笼救俊 甸绢棵荐 绝促. 
	
	bool just_ani;	// true捞搁 窜鉴 Animation父阑 茄促. 
	
	short social_status;
	short fame;
	short nk3;
	short nk4;
	short nk6;
	short dummy_pk;  //  喊 包拌 绝捞 版厚捍阑扒甸搁 吝啊..   货肺 area俊 甸绢坷搁 
	
	DWORD killpc;	   // user
	
	short int tame;
	BYTE tame_cmd;
	short int HostId;
	//char HostName[ 21];
	char TargetName[ 31];
	
	int	hpupdown; //  单固瘤 贸府矫埃阑 焊厘窍扁 困茄 函荐.
	
	int	feeldelay;
	int	feeltype;
	
	bool hunter_target;		// 郴啊 殿废茄 泅惑裹捞促 酒聪促, 	// 0807 YGI
	BYTE lv;//Level
	char class_type;// 努饭胶
	
	////////////////////// SoundUp lkh 眠啊(某腐 阿 悼累喊 绊蜡澜 免仿困茄 矫埃函荐) ///////////////////////////
	DWORD naturalSound_Time;
	int	odd_Even;				//娄荐锅-圈荐锅 风橇
	int	attacker_ID;			//唱甫 傍拜茄 措惑狼 ID
	//void		*lpAttacker;			//唱甫 傍拜茄 措惑 器牢磐
	
	BOOL fight_flag;		//措搬 惑怕眉农..
	int	stone_x;		//傈捧倒 x 谅钎..
	int	stone_y;		//傈捧倒 y 谅钎..
	int	fight_id;		//傈捧且 仇..
	int	fight_time;		//傈捧矫埃..
	
	BYTE light_On;		//堡盔咯何(林牢傍 某腐苞 颇萍/老何 NPC父 扁夯利栏肺 堡盔阑 瘤聪绊 乐澜)
	DWORD castStartingTime;
	int	fight_map_team_num;		// 0906 YGI	fightmap
	int	homex, homey;
	
	int	job;			// 免仿且 流诀		1004 YGI
	int	call_job_lv;	// 免仿且 流诀 饭骇
	
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

///////////////*******************新加的2009.7.14月神夜***************************
	int	ready_Magic_res;		//0->付过 公 /1->付过 casting 吝/2->付过 casting 蜡瘤 吝/3->付过 惯荤
	DWORD start_Casting_res;		//casting 矫累 矫埃
	DWORD end_Casting_res;		//casting 霖厚 肯丰 矫埃	
	DWORD end_ReadyMagic_res;		//casting 蜡瘤 辆丰瞪 矫埃 	
	DWORD end_RunMagic_res;		//付过 瓤苞 场唱绰 矫埃
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
/*
	Sp_res^=MSK_CA;
	if(_Sp!=Sp_res)
		return 0;
	Sp_res^=MSK_CA;
	*/
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

///////////////////////////////////**新加的<月神夜>2009.7.14.
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
/*	int set_Sp(int sp)//SP
	{
		Sp=sp;
		Sp_res=sp;
		Sp_res^=Sp_MSK_CA;
		return 1;
	}*/
	
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
///////////////*******************新加的2009.7.14月神夜***************************
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

	DWORD				reserved_1024;//将内存数据推后4位...没什么屁用.关键要改变结构...结构!
	CCharacter* lpPrev;
	CCharacter* lpNext;
};

typedef CCharacter	CHARACTER;
typedef CCharacter*	LPCHARACTER;
//> CSD-031001
///////////////////////////////////////////////////////////////////////////////
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

//#define CHANGE_MONEY_//020730 lsw 捣 烹老
#else

#define CHANGE_MONEY_	

#endif

