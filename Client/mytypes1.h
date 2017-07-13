/*

*/

#ifndef __MYTYPES_H__
#define	__MYTYPES_H__


#define _MAPOBJECT_DIRECTORY_CHANGE_
//#define BETA_TESTER_
#define	CON_NORMAL	0x00 // �������	 0000000
#define	CON_CURSE		0x01 // Ȱ�������� 0000001
#define	CON_FROZEN	0x02 // ����»��� 0000010		
#define	CON_SLOW		0x04 // ���ο���� 0000100	
#define	CON_DAZE		0x08 // ȥ������	 0001000	
#define	CON_POISON	0x10 // �ߵ�����	 0010000
#define	CON_STONE		0x20 // ��ȭ����	 0100000
#define CON_DEATH		0x40 // ��������	 1000000


//< CSD-030419 : NPC ����
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

////////////////////////// �̱��� 1230 ///////////////////////////////////////////
#define		SET_LOSTSIGHT	1			//�÷»��
//////////////////////////////////////////////////////////////////////////////////
#define		LIGHTNESS		2
/////////////////////////////////////////////////////////////////////////////		


////////////////////////// 0402 �̱��� �߰� //////////////////////////////////////
#define		SET_CUREBLIEND	3
#define		SET_INFORMATION	4

////////////////////////// 0811 YJS �߰� //////////////////////////
#define		FIGHT_RESULT	5

#define		FIGHT_WIN		1
#define		FIGHT_LOSE		2
#define		FIGHT_DRAW		3
#define		FIGHT_LOGOUT	4




//////////////// 0311 lkh �߰� (�̺�Ʈ�� ���� ���) //////////////
#define		OPERATOR_PLAYSOUND_EVENT1	851		//��� ������ �˸��� �డ
#define		OPERATOR_PLAYSOUND_EVENT2	853		//�ڼ� �Ҹ�
#define		OPERATOR_OUTPUT_CELEBRATE	290		//������ ��� ���̴� ȿ��

/////////////// 0810 lhs �߰� /////////////////
#define		START_QUEST_SOUND			15000		//����Ʈ ���� �Ҹ�
#define		QUEST_SALUTE				15001		//����Ʈ�� ���� ȿ��



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

// �ʺ����� Level
#define ROOKIE_LEVEL				7



#define GAME_SCREEN_XTILE			(GAME_SCREEN_XSIZE/TILE_SIZE)
#define GAME_SCREEN_YTILE			(GAME_SCREEN_YSIZE/TILE_SIZE)

#define GAME_SCREEN_XTILE_MID		(GAME_SCREEN_XTILE/2)
#define GAME_SCREEN_YTILE_MID		(GAME_SCREEN_YTILE/2)

//////////////////////// 0808 lkh �߰� /////////////////////////////
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


#define	SPRITETYPE_NONE				 0		// ���콺�� ���� CHARACTER���õǴ� ��ƾ���� ���
#define	SPRITETYPE_CHARACTER		 1 		// ���.. Sortting.. 
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


// �����Ҷ� ĳ���Ͱ� ���ݹ����� �ִ��� �������� ���� ���ϰ�. 
// IsAttackRange()���� ���.....
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

// �������� ĳ������ �󱼻����� ���� Į�ڱ� Sprite��ȣ.
#define PK_BLOOD_EFFECT		272
// �Ʒ��� ���ǵǾ� ������ ���������� ����ȴ�. 
#define _NATION_APPLY_		
// �Ʒ��� ���ǵǾ� ������ ���ڷ� ��µȴ�. 
#define DISPLAY_HIT_NUMBER_	

enum 
{
	CLS_STEP = 0,  // Ŭ���� �ܰ�
	DUAL_CLS = 1,  // ��� Ŭ����
	EXP_STEP = 2,  // ����ġ �ܰ�
	T01_STEP = 3,  // ��ƽ ����ġ �ܰ�
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
	CSP_STEP = 16, // ������ų ������
	MAX_STEP = 17  // �ִ� ����Ÿ��
};

struct CCharRank
{	//< CSD-030326
	DWORD nation	  :3;  // ���ѳ��� - 0:����, 1, 2, 3:���̼���, 4:������, 5:��Ը�Ͼ�, 6:�Ͻ�
	DWORD guild_master:3;  // ��� ������
	DWORD king		  :1;  // ���� ��
	DWORD dwReserved  :9;  // ������ ����
	DWORD counselor	  :2;  // 0 : ����, 1: ȸ�� ���, 2: ���ӳ� ���
	DWORD ultra_nk3	  :1;  // �ؾ�������	1004 YGI
	DWORD ultra_nk4	  :1;  // �ؾ�������	1004 YGI
	DWORD ultra_nk6	  :1;  // �ؾ�������	1004 YGI
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
	ACTION_NONE = 0,					//   1 �⺻����  1
	ACTION_BASIC_WALK,					//   2 �⺻�ȱ�  2
	ACTION_BATTLE_WALK_OWNHAND,			//   3 �����ȱ�  3
	ACTION_BATTLE_NONE,					//   4 �����⺻  4
	ACTION_ATTACK_ONESWORD1,			//   5 �Ѽհ���  5
	ACTION_ATTACK_ONESWORD2,			//   6 �Ѽհ���2  
	ACTION_ATTACK_BONG_DOWN,			//   7 ��, ���,  ����( ����ġ�� )
	ACTION_ATTACK_BONG_JJIRKI,			//   8 â, ��, �� ��( ��� )
	ACTION_ATTACK_BOW,					//   9 Ȱ���      
	ACTION_ATTACK_PENSWORD2,			//  10 ����ڼ�   
	ACTION_ATTACK_PENSING1,				//  11 ��̰��� 1 
	ACTION_ATTACK_PENSING2,				//  12 ��̰��� 2 
	ACTION_RUN,						    //  13 �ٱ�
	ACTION_ATTACKED,				    //  14 �±�
	ACTION_DIE,						    //  15 �ױ�
	ACTION_MAGIC_BEFORE,				//  16 ��������
	ACTION_MAGIC_CASTING,				//  17 ����ĳ����
	ACTION_SITDOWN,						//  18 �ʱ�
	ACTION_THROW,					    //  19 ���������
	ACTION_HORSE,					    //  20 ��Ÿ��
	ACTION_BATTLE_WALK_TWOHAND,			//  21 �����ȱ� 2 ( ��տ� �� )
	ACTION_BATTLE_NONE_TWOHAND,			//  22 �����⺻ 2( ��� )
	ACTION_ATTACK_BONG_ROUND,			//  23 ��, â, ���, ����( �ֵθ��� )
	ACTION_MAGIC_CASTING2,				//  24 ����ĳ���� 2(���� ĳ���Ϳ��Ը� ���� )
	ACTION_ANGER,						//	25 ȭ����			// LTS ACTION
	ACTION_VICTORY						//  26 �¸�����			// LTS ACTION
} ACTION_PC_WOMAN;

enum ACTION_NPC_MONSTER1
{
	MON1_NONE = 0,			// �⺻����
	MON1_BATTLE_WALK,		// �⺻�ȱ�
	MON1_ATTACKED,			// �±�
	MON1_DIE,			    // �ױ�....
	MON1_BATTLE_ATTACK,		// �Ѽհ���
	MON1_BATTLE_MAGIC,		// ��������
	MON1_STAND_UP,			// �Ͼ��
	MON1_CASTING_MAGIC,		// ��������	// LTS DRAGON
	MON1_BATTLE_ATTACK2,	// ���� 2
	MON1_BATTLE_ATTACK3,	// ���� 3
	MON1_JUST_ACTION		// ������ ����
		
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
	DWORD		WarNo						:	7;	//	�����ȣ
	DWORD		WarfieldNo					:	3;	//	������ ��ȣ
	DWORD		SquadNo						:	3;	//	�δ��ȣ
	DWORD		isSquadLeader				:	1;	//	�δ����ΰ�?	
	DWORD		isCommander					:	1;	//	��ɰ��ΰ�?
	DWORD		isCommanderCandidater		:	1;	//	��ɰ��ĺ��ΰ�?
	DWORD		DoWarJoinVote				:	1;	//	������ ���� ��ǥ�� �߳�?
	DWORD		DoCommanderVote				:	1;	//	��ɰ� ��ǥ�� �ߴ�
	DWORD		YL_JoinNation				:	4;	//	�Ͻ��ΰ�� �����ݿ��߳�? ���̼����� �߳�?  // ������ȣ
	DWORD		Reserved					:	10;	//	����..
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
	int	mantle;			// 011018 KHS ����
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
	
	NW_Character NWCharacter;		// 010915 LTS		// UNROLL�� ������� �ʴ� ���, ������ �߻��� ��� 
	
	BYTE nDualCls;		// ������
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
	
	BYTE gender;					// 0:���� 1 : ����
	BYTE Race;						// ����
	BYTE Spiritvalue;				// ��ġ��
	BYTE Class;						// 0:���� 1:�ü� 2:���� 3:������ 4:������
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
	
	BYTE clothr, clothg, clothb;	// �� ���� R.G.B
	BYTE bodyr, bodyg, bodyb;		// �� ���� R.G.B
	
	short Age;
	
	BYTE  Level;
	DWORD Exp;
	
	short Hp;
	short AttackPow;		// ���ݷ�
	
	short Mana;				// ����ġ
	short VitalPow;			// �����
	short Hungry;
	short Nk3, Nk4, Nk6;	// �� ������ ��ȣ��  010531 KHS
	
	DWORD Killmon1;			// 1�迭���� ų����
	DWORD Killmon2;			// 2�迭���� ų����
	DWORD Killmon3;			// 3�Կ����� ų����
	DWORD Killmon4;			// 4�Կ����� ų����
	DWORD Killmon5;			// 5�Կ����� ų����
	DWORD Killmon6;			// 6�Կ����� ų����
	DWORD Killanimal;		// ���� ų����
	DWORD KillNPC;			// ���,NPC ų����
	
	DWORD Money;
	
	//����ĳ���� �����Ҷ��� ��ġ..
	short StrOld;				//  ��
	short ConOld;				//  �ǰ�
	short DexOld;				//	��ø��
	short WisOld;				//	����
	short IntOld;				//	����
	short MovePOld;				//	�̵���
	short CharOld;				//	�ŷ�
	short EnduOld;				//  �����
	short HealthOld;			//	ü��
	short MoralOld;				//	���
	short LuckOld;				//	���
	
	short Str;				//  ��
	short Con;				//  �ǰ�
	short Dex;				//	��ø��
	short Wis;				//	����
	short Int;				//	����
	short MoveP;			//	�̵���
	short Char;				//	�ŷ�
	short Endu;				//  �����
	short Health;			//	ü��
	short Moral;			//	���
	short Luck;				//	���
	short wsps;
	
	
	short Resis_Poision;	//	��/���� ���׷�
	short Resis_Stone;		//	��ȭ ���� ���׷�
	short Resis_Magic;		//	�������׷�
	short Resis_Fire;		//	�� ���׷�
	short Resis_Ice;		//	�������׷�
	short Resis_Elect;		//	���������
	short Wizardspell;		//	Ws
	short Ws[200];			//	����
	short Priestspell;		//	Ps
	short Ps[200];			//	�Ź�
	
	// ���� ������ �ִ� ������..
	DWORD Item[ 13][100];
	
	DWORD bankmoney;
	
}CHARELEMENT, *LPCHARELEMENT;

//const	DWORD	ID_MSK=0x7eb29b9e;	//���ܹؼ���,�����ʵ��޸�
///////////////////////////////////////////////////////////////////////////////
//< CSD-031001

//----------------------

//const		DWORD		NAME_MSK=0X9B2BACD6;	//�Ѿ�����
//--bu fqyd 060730
//#define		MSK_CA		0x93cf6d //��ȥ�������,���ڱȽ����ױ��ƽ�,���Ըĳ������µ��������
///////////////////////////////////////////////////////////////////���ﶨ�����������ĺ���/////////////////////////////////////////////
//extern int 	MSK_CA;	//��ȥ�������,���ڱȽ����ױ��ƽ�,���Ըĳ������µ��������
extern int mutant_MSK_CA;//תְ
extern int movp_MSK_CA;//�ƶ���
extern int nAttackedType_MSK_CA;//��������
extern int bCanViewGhost_MSK_CA;//����
extern int sight_MSK_CA;//��Ұ
extern int excute_MagicNum_MSK_CA;//�ͷ�ħ������,����
extern int continue_RunMagic_MSK_CA;//ħ������ʱ��
extern int nActiveCombat_MSK_CA;//�������ܵ�ַ.E4 00 00 00 00 00 00 00 00 00 00 00 01����ʲô��.
extern int nActiveLevel_MSK_CA;//�������ܵȼ���ַ.
extern int nCurrentAction_MSK_CA;//���ﶯ����Ϊ
extern int nCurrentFrame_MSK_CA;//����ζ�ֵ
extern int bow_Type_MSK_CA;// ħ��������0��û�У�1�ǻ����2�Ǳ���
extern int is_mainmenu_active_MSK_CA;//���ڻ�ַ����
extern int bCheckSalvation_MSK_CA;//��������FFH,FFH,FFH,FFH��
//////////////////////////////////////////////*******************�¼ӵ�*******************
extern int	ready_Magic_MSK_CA;		//0->���� �� /1->���� casting ��/2->���� casting ���� ��/3->���� �߻�
extern DWORD start_Casting_MSK_CA;		//casting ���� �ð�
extern DWORD end_Casting_MSK_CA;		//casting �غ� �Ϸ� �ð�	
extern DWORD end_ReadyMagic_MSK_CA;		//casting ���� ����� �ð� 	
extern DWORD end_RunMagic_MSK_CA;		//���� ȿ�� ������ �ð�
extern DWORD castStartingTime_MSK_CA;		//���� ȿ�� ������ �ð�
////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////������ֵĺ�������//////////////////////////////////////////////////////
extern	void mutant_cont(void);			//תְ
extern	void movp_cont(void);			//�ƶ���
extern	void nAttackedType_cont(void);			//��������
extern	void bCanViewGhost_cont(void);			//����
extern	void sight_cont(void);			//��Ұ
extern	void excute_MagicNum_cont(void);			//�ͷ�ħ������
extern	void continue_RunMagic_cont(void);			//ħ������ʱ��
extern	void nActiveCombat_cont(void);			//������������
extern	void nActiveLevel_cont(void);			//�������ܵȼ�
extern	void nCurrentAction_cont(void);			//���ﶯ����Ϊ
extern	void nCurrentFrame_cont(void);			//�ζ�ֵ
extern	void bow_Type_cont(void);			//ħ����
extern	void is_mainmenu_active_cont(void);			//���ڻ�ַ����
extern	void bCheckSalvation_cont(void);			//��������FFH,FFH,FFH,FFH��
//////////////////////////////////////////////*******************�¼ӵ�*******************
extern	void ready_Magic_cont(void);		//0->���� �� /1->���� casting ��/2->���� casting ���� ��/3->���� �߻�
extern	void start_Casting_cont(void);		//casting ���� �ð�
extern	void end_Casting_cont(void);		//casting �غ� �Ϸ� �ð�	
extern	void end_ReadyMagic_cont(void);		//casting ���� ����� �ð� 	
extern	void end_RunMagic_cont(void);		//���� ȿ�� ������ �ð�
extern	void castStartingTime_cont(void);		//���ʱ��?
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*//extern		char		g_name_0[31];			//����

typedef struct _SChName
{
	char	name[31];							
}SChName;

extern std::vector<SChName>		*pChNames;
//-------------------------*/
class CCharacter     //���������������������±ߵ�������Ϣ,�ζ�ֵ,����ID,����ID��.�Ҷ��ǰ�˳��
					//���е�.���Ըı���˳��Ϳ��Ըı���������λ��
{//��ɫ��
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
		xid=((DWORD)id)^ID_MSK;	//����򵥵�������
		return 1;
	}

	int nCurrentFrame()//����ζ�ֵ
	{
		DWORD	temp=xCF;
		temp=temp^ID_MSK;
		return(int)temp;
	}
	int set_nCurrentFrame(int CF)
	{
		//DWORD t=(DWORD)id;
		nCurrentFrame()=((DWORD)CF)^ID_MSK;	//����򵥵�������
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
	pChNames->push_back(temp);//���浽һ��ͳһ������������

	this->name_ptr_no=pChNames->size()-1;
	return 1;
}*/

private:
//	DWORD		server_19;
	int			m_nGuildCode;
//	DWORD		xid;  //��ɫ���߹����ID
//	DWORD		xCF;  //����ζ�ֵ

public:
	CCharRank name_status;  //��λ

	char type;
	int id;
	char name[ 31];
//	int name_ptr_no;	//ֻ������һ��ͳһ�ĺ���
	char namedisplaycount;
	int	sprno;
	int	mutant;   //0810 KHS  //תְ,����
	int	nutantani;	// 0810 KHS
	short age;
	// 010515 KHS
	bool eventnpc;
	
	short nCharacterData[20];
	int	animationType;
	int	AIanimationRest;	//  0 �ƴϸ�  �������� �ʰ� �״�� ���ִ´�. 
	int	AIattackTargetID;	//	����� Mon1�� �����ӿ��� 0 �ƴϸ�  �������� �ʰ� �״�� ���ִ´�. 
	short movetargetid;
	bool sealstone;
	short sealstoneani;
	char GuardStoneAni;	// 010915 LTS	// LTS SEALSTONE
	char SetAni2Zero;
	
	//DOWRD	reserved_2; //����.//���Ｘ������Ҳ��,���ԼӼ�
	int x;
	int	restdx;
//	int	movp;//�ƶ���,��Ҫ����
	int	ddx;
	short int tox,toy; //���Ըı������λ��
	bool toflag;
	int y;
	
	int	height;
	int	oldgox, oldgoy;
	int	gox, goy;
	DWORD desttime;
	char destflag;
	int	sex;
	int	walktime;
///	DOWRD	reserved_3; //����.//���Ｘ������Ҳ��,���ԼӼ�
	POINT position,oldposition;
	POINT offset;
	DIRECTION direction;
	DIRECTION todir; // direction�� todir�� �ٲٷ��¼����� �ִ�. 
	short dirdelay;
	
//	int __movp; //�ƶ���,��Ҫ����
	int	light;  //Ӧ����������...
	int	attackMinRange;
	int	attackMaxRange;
	int	changeActionFlag;// 1�̸� �����߿� �ٸ��������� �ٲ�� ����.
	
//	int	nAttackedType;//��������,����
	void* lpAttacked;
	
	short NumberAnimationValue;
	BYTE attackedresult;
	short flagattacked;//
	bool attacksuccess;
	char attackanimationON;
	char pluse_value;
	char willdie; // �̼�ġ�� 1�̵Ǹ� �״´�. 
	
	int	nCurrentAction;		//���ﶯ��ֵ
	int	movp;//�ƶ���,��Ҫ����
	
	//int	nCurrentFrame;		//����ζ�ֵ
	bool bCanViewGhost;    //������,��Ҫ����
	int	nAttackedType;//��������,����
	int	bow_Type;		// ħ������,0����û��,1������,2�������.
	int TotalAni;
	PCANIMATIONTABLE AnimationTable[ MAX_ANIMATION ];
	
	Spr	*sp, *sp0, *sp1, *sp2, *sp3, *sp4;
	
	int	pathcount;
	int	oldpathcount;
	int	path[MAX_SHORTPATH][2];
	bool moveFlag; //�̵��� Tile�� ��ġ�� ����Ͽ� ��Ȯ�� �װ����� ����. 
	BYTE movetype; // 0:walk 1:run 2:horse riding.
	
	short basicAction;
	short basicWalk;
	char viewtype;
	BYTE canview;
	short canviewitem;
	//020320 lsw
//	bool bCanViewGhost;    //������,��Ҫ����
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
	int	mantle;	// 011018 KHS ����
	int	jjing;	// 011030 KHS JJING
	
	DWORD bodycolor;
	DWORD clothcolor;
	DWORD mantlecolor;	// 011018 KHS ����	
	
	int	hp,mp,Sp;//sp��Ҫ����
	int	hpmax,mpmax,Spmax;
	int	dieani;
	int	diedelay;
	int	dieanitype;
	char boxitembreaktype;
	
	BYTE wisdom;
	BYTE intelligence;
	BYTE health;				//����� or Health
	BYTE con;			//�ǰ���
	char spell;
	short face;
	
	bool stolen;
	
	int	sight;//��Ұ,����
	BYTE peacests;
	BYTE goodbad;
	
	CHARELEMENT* ce;
	
	int ChatDelay;
	int	nChatType;
	char szChatMessage[MAX_CHAT_STRING];
	
	char smileno;
	short int smiledelay;
	
	int	excute_MagicNum;	//���� �����ڰ� �غ��ϰ� �ִ� ������ȣ//�ͷ�ħ������,����
	int	ready_Magic;		//0->���� �� /1->���� casting ��/2->���� casting ���� ��/3->���� �߻�
	int	ready_Magic_rare;
	DWORD start_Casting;		//casting ���� �ð�
	DWORD continue_Time;
	DWORD end_Casting;		//casting �غ� �Ϸ� �ð�
	
	DWORD start_ReadyMagic;	//casting ���� ���۵� �ð�
	DWORD end_ReadyMagic;		//casting ���� ����� �ð� 
	
	DWORD start_RunMagic;		//���� ���� ȿ���� ���۵� �ð�
	DWORD continue_RunMagic; 
	DWORD end_RunMagic;		//���� ȿ�� ������ �ð�
	
	int	inc_Num;
	
	bool blood_Set;			//�����κ��� �׾��ٴ� �޽��� ������� ���� Ŭ���̾�Ʈ���� �̸� �� ���
	BYTE condition;			//ĳ������ ����
	int	arigement;			//ĳ������ ��ġ��
	int	insertPattern;		// 
	
	int	lostSight;			//ĳ���Ͱ� ���� ���� ����(0->������/1�̻�->�þ߻�� ����)
	DWORD lostSightTime;		//�þ� ��� ���� �ð�
	
	int shoot_PointX;		//ȭ�� ���ϸ��̼��� �߻������ ��Ȯ�� Ŭ���� ��ġ�� �߻����� ���
	int	shoot_PointY;		
	
	int	eventdelay;
	
	int	_lev;
	int _str;
	int	_Wis;				//	����
	int	_Int;				//	����
	int	_wsps;				//  ������->���� �޸������
	
	int _Hp, _HpMax;
	int _Mp, _MpMax;
	int _Hungry,_HungryMax;		
	short int event_no;
	int viewdebugcharinfo;
	
	int destx;
	int desty;		// ���� Map x,y;
	
	char HostName[ 21];
	//  npc�����ӿ� ���� ���� ...
	short patterntype, oldpatterntype;
	int targetid;
	int bossid;
	short attackcount;
	DWORD aitimedelay, aidelayhowmuch;
	int how;
	int attacked;
	DWORD pk;
	DWORD Exp;
	int MoveP;		// NPC�� �������� �پ��ϰ� �ϱ� ���� ������ ����..
	bool notcomeinside; //  ���ʹ� ���ȿ� ���ü� ����. 
	
	bool just_ani;	// true�̸� �ܼ� Animation���� �Ѵ�. 
	
	short social_status;
	short fame;
	short nk3;
	short nk4;
	short nk6;
	short dummy_pk;  //  �� ���� ���� ������ǵ�� �߰�..   ���� area�� ������ 
	
	DWORD killpc;	   // user
	
	short int tame;
	BYTE tame_cmd;
	short int HostId;
	//char HostName[ 21];
	char TargetName[ 31];
	
	int	hpupdown; //  ������ ó���ð��� �����ϱ� ���� ����.
	
	int	feeldelay;
	int	feeltype;
	
	bool hunter_target;		// ���� ����� ������̴� �ƴϴ�, 	// 0807 YGI
	BYTE lv;//Level
	char class_type;// Ŭ����
	
	////////////////////// SoundUp lkh �߰�(ĳ�� �� ���ۺ� ������ ������� �ð�����) ///////////////////////////
	DWORD naturalSound_Time;
	int	odd_Even;				//¦����-Ȧ���� ����
	int	attacker_ID;			//���� ������ ����� ID
	//void		*lpAttacker;			//���� ������ ��� ������
	
	BOOL fight_flag;		//��� ����üũ..
	int	stone_x;		//������ x ��ǥ..
	int	stone_y;		//������ y ��ǥ..
	int	fight_id;		//������ ��..
	int	fight_time;		//�����ð�..
	
	BYTE light_On;		//��������(���ΰ� ĳ���� ��Ƽ/�Ϻ� NPC�� �⺻������ ������ ���ϰ� ����)
	DWORD castStartingTime;
	int	fight_map_team_num;		// 0906 YGI	fightmap
	int	homex, homey;
	
	int	job;			// ����� ����		1004 YGI
	int	call_job_lv;	// ����� ���� ����
	
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
	int nActiveCombat;//�������ܵ�ַ.E4 00 00 00 00 00 00 00 00 00 00 00 01����ʲô��.
	int nPassiveCombat;
	int nRecoveryCombat;
	int nActiveLevel;//�������ܵȼ���ַ.                                 
	int nPassiveLevel;
	int nRecoveryLevel;
	bool bCrash;
	bool bShoot;
	bool bSelect;
	bool bLock;	
	WORD idWeaponItem; // CSD-031001
	int	nCurrentFrame;		//����ζ�ֵ

//-----------fqyd 060730--
	//------�ؼ���Ϣ�����ﱸ��------
	int mutant_res;//תְ
	int movp_res;	//�ƶ���
	int nAttackedType_res;//��������
	int bCanViewGhost_res;//������
	int Sp_res;				//SP...����SP�
	int sight_res;			//��Ұ
	int excute_MagicNum_res;//�ͷ�ħ������,����
	int nActiveCombat_res;//�������ܵ�ַ.E4 00 00 00 00 00 00 00 00 00 00 00 01����ʲô��.
	int nActiveLevel_res;//�������ܵȼ���ַ.
	int	nCurrentAction_res;		//���ﶯ����Ϊ
	int	nCurrentFrame_res;		//����ζ�ֵ
	int	bow_Type_res;		// ħ������,0����û��,1������,2�������.

///////////////*******************�¼ӵ�2009.7.14����ҹ***************************
	int	ready_Magic_res;		//0->���� �� /1->���� casting ��/2->���� casting ���� ��/3->���� �߻�
	DWORD start_Casting_res;		//casting ���� �ð�
	DWORD end_Casting_res;		//casting �غ� �Ϸ� �ð�	
	DWORD end_ReadyMagic_res;		//casting ���� ����� �ð� 	
	DWORD end_RunMagic_res;		//���� ȿ�� ������ �ð�
	DWORD castStartingTime_res; //ħ�����ʱ��?

////////////////////////////////////////////////////////////////////////////////////////////////
	//char	pluse_value_res;
	DWORD	continue_RunMagic_res;//ħ������ʱ��

	int Check()
	{//��Ӧ�ļ�麯��
	mutant_res^=mutant_MSK_CA;//תְ
	if(mutant!=mutant_res)
		return 0;
	mutant_cont();				//�������
	mutant_res^=mutant_MSK_CA;

	movp_res^=movp_MSK_CA;//�ƶ�
	if(movp!=movp_res)
		return 1;
	movp_cont();				//�������
	movp_res^=movp_MSK_CA;

	nAttackedType_res^=nAttackedType_MSK_CA;//��������
	if(nAttackedType!=nAttackedType_res)
		return 2;
	nAttackedType_cont();				//�������
	nAttackedType_res^=nAttackedType_MSK_CA;
	
	bCanViewGhost_res^=bCanViewGhost_MSK_CA;//����
	if(bCanViewGhost!=bCanViewGhost_res)
		return 3;
	bCanViewGhost_cont();				//�������
	bCanViewGhost_res^=bCanViewGhost_MSK_CA;
/*
	Sp_res^=MSK_CA;
	if(_Sp!=Sp_res)
		return 0;
	Sp_res^=MSK_CA;
	*/
	sight_res^=sight_MSK_CA;//��Ұ
	if(sight!=sight_res)
		return 4;
	sight_cont();				//�������
	sight_res^=sight_MSK_CA;

	excute_MagicNum_res^=excute_MagicNum_MSK_CA;//�ͷ�ħ������
	if(excute_MagicNum!=excute_MagicNum_res)
		return 5;
	excute_MagicNum_cont();				//�������
	excute_MagicNum_res^=excute_MagicNum_MSK_CA;
	
	continue_RunMagic_res^=continue_RunMagic_MSK_CA;//ħ������ʱ��
	if(continue_RunMagic_res!=continue_RunMagic)
		return 6;
	continue_RunMagic_cont();				//�������
	continue_RunMagic_res^=continue_RunMagic_MSK_CA;

	nActiveCombat_res^=nActiveCombat_MSK_CA;//�������ܵ�ַ.E4 00 00 00 00 00 00 00 00 00 00 00 01����ʲô��.
	if(nActiveCombat!=nActiveCombat_res)
		return 7;
	nActiveCombat_cont();				//�������
	nActiveCombat_res^=nActiveCombat_MSK_CA;
	
	nActiveLevel_res^=nActiveLevel_MSK_CA;////�������ܵȼ���ַ.
	if(nActiveLevel!=nActiveLevel_res)
		return 8;
	nActiveLevel_cont();				//�������
	nActiveLevel_res^=nActiveLevel_MSK_CA;
	
	nCurrentAction_res^=nCurrentAction_MSK_CA;//���ﶯ����Ϊ
	if(nCurrentAction!=nCurrentAction_res)
		return 9;
	nCurrentAction_cont();				//�������
	nCurrentAction_res^=nCurrentAction_MSK_CA;
	
	nCurrentFrame_res^=nCurrentFrame_MSK_CA;//����ζ�ֵ
	if(nCurrentFrame!=nCurrentFrame_res)
		return 10;
	nCurrentFrame_cont();				//�������
	nCurrentFrame_res^=nCurrentFrame_MSK_CA;
	
	bow_Type_res^=bow_Type_MSK_CA;// ħ������,0����û��,1������,2�������.
	if(bow_Type!=bow_Type_res)
		return 11;
	bow_Type_cont();				//�������
	bow_Type_res^=bow_Type_MSK_CA;

///////////////////////////////////**�¼ӵ�<����ҹ>2009.7.14.
	ready_Magic_res^=ready_Magic_MSK_CA;// ħ������,0����û��,1������,2�������.
	if(ready_Magic!=ready_Magic_res)
		return 12;
	ready_Magic_cont();				//�������
	ready_Magic_res^=ready_Magic_MSK_CA;
				
	start_Casting_res^=start_Casting_MSK_CA;// ħ������,0����û��,1������,2�������.
	if(start_Casting!=start_Casting_res)
		return 13;
	start_Casting_cont();				//�������
	start_Casting_res^=start_Casting_MSK_CA;
	
	end_Casting_res^=end_Casting_MSK_CA;// ħ������,0����û��,1������,2�������.
	if(end_Casting!=end_Casting_res)
		return 14;
	end_Casting_cont();				//�������
	end_Casting_res^=end_Casting_MSK_CA;
							
	end_ReadyMagic_res^=end_ReadyMagic_MSK_CA;// ħ������,0����û��,1������,2�������.
	if(end_ReadyMagic!=end_ReadyMagic_res)
		return 15;
	end_ReadyMagic_cont();				//�������
	end_ReadyMagic_res^=end_ReadyMagic_MSK_CA;

	end_RunMagic_res^=end_RunMagic_MSK_CA;// ħ������,0����û��,1������,2�������.
	if(end_RunMagic!=end_RunMagic_res)
		return 16;
	end_RunMagic_cont();				//�������
	end_RunMagic_res^=end_RunMagic_MSK_CA;
	
	castStartingTime_res^=castStartingTime_MSK_CA;// ħ������,0����û��,1������,2�������.
	if(castStartingTime!=castStartingTime_res)
		return 17;
	castStartingTime_cont();				//�������
	castStartingTime_res^=castStartingTime_MSK_CA;
	
		return 18;
}

	//������Щ���ǽػ�������,�����汸�ݵĺ���.
	int set_mutant(int m)//תְ
	{
		mutant=m;
		mutant_res=m;
		mutant_res^=mutant_MSK_CA;
		return 1;
	}
	int set_movp(int	m)//�ƶ�
	{
		movp=m;
		movp_res=m;
		movp_res^=movp_MSK_CA;
		return 1;
	}
	int set_nAttackedType(int n)//��������
	{
		nAttackedType=n;
		nAttackedType_res=n;
		nAttackedType_res^=nAttackedType_MSK_CA;
		return 1;
	}
	int set_bCanViewGhost(bool c)//����
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
	
	int set_sight(int	s)//��Ұ
	{
		sight=s;
		sight_res=s;
		sight_res^=sight_MSK_CA;
		return 1;
	}
	int set_excute_MagicNum(int e)//ħ������
	{
		excute_MagicNum=e;
		excute_MagicNum_res=e;
		excute_MagicNum_res^=excute_MagicNum_MSK_CA;
		return 1;
	}
	int set_continue_RunMagic(DWORD cr)//ħ������ʱ��
	{
		continue_RunMagic=cr;
		continue_RunMagic_res=cr;
		continue_RunMagic_res^=continue_RunMagic_MSK_CA;
		return 1;
	}
	int set_nActiveCombat(int ac)//��������
	{
		nActiveCombat=ac;
		nActiveCombat_res=ac;
		nActiveCombat_res^=nActiveCombat_MSK_CA;
		return 1;
	}
	int set_nActiveLevel(int al)//�������ܵȼ�
	{
		nActiveLevel=al;
		nActiveLevel_res=al;
		nActiveLevel_res^=nActiveLevel_MSK_CA;
		return 1;
	}
	int set_nCurrentAction(int ca)//���ﶯ����Ϊ
	{
		nCurrentAction=ca;
		nCurrentAction_res=ca;
		nCurrentAction_res^=nCurrentAction_MSK_CA;
		return 1;
	}
	int set_nCurrentFrame(int cf)//����ζ�ֵ
	{
		nCurrentFrame=cf;
		nCurrentFrame_res=cf;
		nCurrentFrame_res^=nCurrentFrame_MSK_CA;
		return 1;
	}
	int set_bow_Type(int bw)//// ħ������,0����û��,1������,2�������.
	{
		bow_Type_res=bw;
		bow_Type_res=bw;
		bow_Type_res^=bow_Type_MSK_CA;
		return 1;
	}
///////////////*******************�¼ӵ�2009.7.14����ҹ***************************
	int set_ready_Magic(int rm)//// ħ������,0����û��,1������,2�������.
	{
		ready_Magic_res=rm;
		ready_Magic_res=rm;
		ready_Magic_res^=ready_Magic_MSK_CA;
		return 1;
	}
	int set_start_Casting(int sc)//// ħ������,0����û��,1������,2�������.
	{
		start_Casting_res=sc;
		start_Casting_res=sc;
		start_Casting_res^=start_Casting_MSK_CA;
		return 1;
	}
	int set_end_Casting(int ec)//// ħ������,0����û��,1������,2�������.
	{
		end_Casting_res=ec;
		end_Casting_res=ec;
		end_Casting_res^=end_Casting_MSK_CA;
		return 1;
	}
	int set_end_ReadyMagic(int er)//// ħ������,0����û��,1������,2�������.
	{
		end_ReadyMagic_res=er;
		end_ReadyMagic_res=er;
		end_ReadyMagic_res^=end_ReadyMagic_MSK_CA;
		return 1;
	}
	int set_end_RunMagic(int er)//// ħ������,0����û��,1������,2�������.
	{
		end_RunMagic_res=er;
		end_RunMagic_res=er;
		end_RunMagic_res^=end_RunMagic_MSK_CA;
		return 1;
	}
	int set_castStartingTime(int cst)//// ħ������,0����û��,1������,2�������.
	{
		castStartingTime_res=cst;
		castStartingTime_res=cst;
		castStartingTime_res^=castStartingTime_MSK_CA;
		return 1;
	}

	
////////////////////////////////////////////////////////////////////////////////////////////////

	DWORD				reserved_1024;//���ڴ������ƺ�4λ...ûʲôƨ��.�ؼ�Ҫ�ı�ṹ...�ṹ!
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

//#define CHANGE_MONEY_//020730 lsw �� ����
#else

#define CHANGE_MONEY_	

#endif

