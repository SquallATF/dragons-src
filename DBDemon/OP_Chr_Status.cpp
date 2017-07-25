#include "StdAfx.h"

#include "main.h"
#include "CItem.h"
#include "Map.h"
#include "scrp_exe.h"
#ifdef _GAME_SERVER_
#include "OP_Magic.h"
#endif

#define NUM_OF_RACE	9
#define NUM_OF_KIND 8

extern CItem_Weapon			CWeapon[] ;

//////////////////////////////////////////////////////////////
///
///
extern int  calcAC( CHARLIST *ch );

#ifdef _GAME_SERVER_
extern int IsHePK( int id );
#endif

/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
extern char* SQLerrorS;/////////////////////////////////////定义发送外挂错误的字符串
extern void HackLog( int type, char *logmsg, ... );//记录外挂的LOG
////////////////////////////////////截获声明
extern char *str;
//char hackID[100],hackname[100];
///////////////////////////////////

//////////////////////////////////////////////////////////////
///
///

// SP 家厚 包访 抛捞喉
// 辆练喊, 惑怕(傈捧, 厚傈捧, 泪 殿狼 惑怕俊 蝶扼)
char sp_consume_tbl[NUM_OF_RACE][NUM_OF_KIND]= { -5, -3, 1, 15, -8, -5, 1, 20,
-6, -3, 1, 15, -6, -2, 2, 18,
-4, -2, 1, 15, -6, -3, 1, 15,
-8, -5, 1, 20, -5, -3, 1, 15,
-4, -4, 2, 15, -4, -4, 3, 15,
-5, -3, 1, 20, -6, -4, 3, 25,
-1, -1, 1,  5, -1, -1, 1,  6,
-3,  0, 2, 10, -4,  0, 2, 10,
-1,  0, 1,  3, -1,  0, 1,  3 	} ;						



// SP焊沥 包访 抛捞喉
char sp_ttt_corr_tbl[11]= {6, 8, 1, 13, 16, 19, 22, 25, 28, 31, 34 } ; // use the result of divide it by 10
char sp_weight_corr_tbl[10]= {1, 1, 1, 1, 1, 12, 14, 16, 18, 2 } ; // use the result of divide it by 10

// 付过狼 某胶泼 焊沥 抛捞喉 DAMAGE(%)
// 某胶泼 矫埃 - 泅饭骇 + 夸备饭骇
char spell_efficent_tbl[10]= {6, 7, 8, 9, 10, 11, 12, 13, 14, 15 } ; // use the result of divide it by 10

// 氓栏肺 嚼垫啊瓷茄 付过 俺荐.
// index= (int)(INT-10) / 8
char spell_per_int_tbl[9]= {8, 17, 37, 48, 60, 73, 87, 102, 116 } ;

// 巩凯扁
char break_lock_tbl[20]= {1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 } ;
// 艰芒混 戎扁
char break_bar_tbl[20]= {0, 0, 0, 1, 2, 3, 7, 9, 10, 12, 18, 24, 30, 36, 42, 48, 54, 60, 66, 72 } ;

// 捞悼加档 0~25, 26~50, 51~75, 76~99 
// index= (int)((MoveP-1) / 25)
char walk_per_round[4]= {2, 3, 4, 5 } ;
char run_per_round[4]= {4, 5, 6, 7 } ;




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///		User Functions Declaration...
///
int InitPartyMemberServerToServer( CHARLIST *ch );		//  捞抚父 叼厚俊 历厘窍绊 捞抚父 波郴柯促.
int EndsetPartyMenberForUpdate( CHARLIST *ch );
int SetMySkill( CHARLIST *ch );		// 老窜 扁夯利栏肺 舅酒具窍绰 胶懦 悸

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///		User Functions.
///

void SendLevelUpMsgAndPoint( CHARLIST *chr )		// 1217
{
	t_packet p;
	
	p.h.header.type= CMD_LEVELUP_POINT;
	{
		//< CSD-010907
		p.u.server_levelup_point.idServer	= chr->server_id;
		p.u.server_levelup_point.nPoint	= (BYTE )chr->reserved_point;
		p.u.server_levelup_point.nLife = chr->Hp;
		//> CSD-010907
	}
	p.h.header.size= sizeof( t_server_levelup_point );
	QueuePacket(connections, chr->server_id, &p, 1);
	
	
	
#ifdef _GAME_SERVER_
	CastMe2Other( chr->server_id, &p );
#endif
	
}

// 矫埃寸 1锅 龋免
void calcHungry(CHARLIST *chr)
{
	double lev_corr, disease_corr, t;
	int h= 0, weight_index ;
	
	if(chr->Peacests) { // 傈捧矫
		if(chr->Level < 20) {
			lev_corr= 1 + (chr->Level - 20) * 0.02 ;
		}
		else {
			lev_corr= 1 + (chr->Level - 20) * 0.03 ;
		}
		
		weight_index= (int)chr->Weight / chr->MaxWeight * 10 ;
		
		disease_corr= 1.0 ;
		
		t = sp_consume_tbl[chr->Race][0] * lev_corr * sp_weight_corr_tbl[weight_index] / 10 + disease_corr ;
		
		chr->Hungry-= (short)t;
		if( chr->Hungry < 0 )  chr->Hungry = 0;
		
	}
	else { // 扁贱荤侩? 绒侥? 荐搁?
	}
}	


int calcMovP( CHARLIST *ch )
{
	int t = ch->MoveP;
	
	return t;
}

// 唱捞俊 蝶弗 阿 公扁扁贱喊 胶懦 槛访档狼 惑铰加档狼 瞒捞 10栏肺 唱穿绢 荤侩
char tac_inc_by_age_tbl[12][37]= {	1, 13, 12, 13, 13, 12, 12, 14, 13, 15, 14, 12, 10, 12, 10, 10, 10, 10, 11, 10, 10, 10, 10, 11, 10, 10, 10, 10, 10, 11, 10, 10, 10, 10, 10, 10, 1,
1, 10, 10, 11, 11, 12, 12, 13, 13, 12, 14, 13, 11, 10, 10, 10, 10, 10, 10, 11, 10, 10, 10, 10, 11, 12, 12, 11, 13, 11, 14, 10, 10, 10, 10, 10, 1,
1, 10, 10, 10, 10, 11, 13, 14, 15, 15, 15, 11, 12, 10, 10, 13, 10, 10, 10, 11, 10, 11, 10, 10, 10, 10, 10, 10, 11, 10, 10, 10, 10, 10, 10, 10, 1,
1, 10, 10, 10, 10, 11, 10, 10, 10, 10, 10, 10, 11, 15, 13, 12, 14, 11, 11, 12, 10, 10, 10, 10, 10, 13, 11, 12, 10, 10, 10, 11, 10, 10, 10, 10, 1,
1, 10, 10, 10, 10, 12, 10, 10, 10, 10, 10, 10, 10, 15, 14, 11, 13, 10, 12, 11, 10, 10, 10, 10, 10, 14, 10, 13, 10, 10, 10, 10, 10, 11, 11, 10, 1,
1, 12, 10, 10, 11, 13, 12, 10, 10, 10, 10, 10, 10, 11, 10, 10, 10, 10, 10, 12, 14, 10, 12, 13, 12, 12, 13, 13, 12, 10, 10, 10, 10, 10, 11, 10, 1,
1, 10, 11, 10, 11, 10, 10, 10, 10, 11, 10, 10, 10, 10, 10, 12, 10, 10, 10, 14, 15, 14, 15, 15, 14, 14, 15, 10, 10, 10, 11, 10, 10, 10, 10, 10, 1,
1, 10, 11, 10, 10, 10, 10, 10, 11, 10, 12, 10, 10, 10, 10, 10, 10, 10, 10, 11, 10, 10, 11, 10, 12, 13, 15, 14, 14, 15, 15, 11, 10, 10, 10, 11, 1,
1, 10, 10, 11, 10, 10, 10, 10, 11, 10, 13, 13, 12, 15, 14, 12, 10, 10, 10, 10, 10, 10, 11, 10, 10, 11, 13, 12, 12, 11, 10, 10, 11, 10, 10, 10, 1,
1, 10, 10, 10, 10, 11, 10, 10, 11, 10, 10, 14, 12, 13, 14, 15, 14, 13, 12, 11, 10, 10, 10, 11, 10, 10, 10, 10, 10, 11, 10, 10, 10, 10, 10, 10, 1,
1, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 10, 10, 11, 12, 13, 12, 12, 14, 13, 12, 11, 12, 13, 11, 10, 10, 10, 10, 11, 11, 10, 11, 10, 1,
1, 10, 10, 10, 10, 11, 10, 10, 10, 11, 10, 10, 13, 12, 14, 13, 14, 11, 10, 10, 10, 10, 10, 12, 13, 12, 13, 14, 12, 10, 10, 10, 10, 11, 10, 10, 1
} ;

// 流诀俊 蝶弗 公扁扁贱喊 槛访档 惑铰沥档 (10栏肺 唱穿绢 荤侩)

char tac_inc_by_class_tbl[5][12]= { 15, 15, 15, 15, 1, 1, 1, 1, 15, 15, 15, 1,
8, 10, 8, 8, 8, 8, 15, 15, 8, 8, 1, 8,
1, 1, 8, 8, 15, 15, 15, 15, 1, 8, 8, 1,
8, 8, 8, 8, 15, 1, 1, 8, 8, 8, 8, 1,
5, 8, 5, 5, 15, 1, 1, 8, 5, 5, 5, 5
} ;

void increaseTacticsSkill(CHARLIST *chr)
{
	int skill_index ;
	int k ;
	int age_index, weapon_type_index ;
	double skill_corr ;
	
	k= 1 ;
	skill_index= 0 ;
	
	// 胶懦急琶 焊沥
	if(chr->Tactics == skill_index) skill_corr= 1.2 ;
	else skill_corr= 1.0 ;
	
	if(chr->Age < 15) {
		age_index= 0 ;
	} else age_index= chr->Age-15;
	
	if(CWeapon[chr->equip[0].item_no].Item_kind < 0)
	{
		weapon_type_index= 0;
	} else weapon_type_index= CWeapon[chr->equip[0].item_no].Item_kind;
	
	double t = k * tac_inc_by_age_tbl[skill_index][age_index] 
		* tac_inc_by_class_tbl[chr->Race][weapon_type_index] * skill_corr;
	
	chr->tac_skillEXP[skill_index]+= (int)t;
}



void increaseJobSkill(CHARLIST *chr)
{	
	
}	

void decreaseJobSkill(CHARLIST *chr)
{	
	
}	

void decreaseClassSkill(CHARLIST *chr)
{	
	
}	

void increaseClassSkill(CHARLIST *chr)
{	
	
}	

int increaseAbility(CHARLIST *chr, t_packet *packet)
{
	if(packet->h.header.size < packet->u.mass_data.num * 3 + 1) {
		printf("\n increase ability: Size doesn't matching!") ;
		return -1 ;
	}
	int total_inc= 0 ;	
	int i= 0 ;
	
	for(i= 0 ; i < packet->u.mass_data.num ; i++) 
		total_inc+= packet->u.mass_data.elements[i].value ;
	// 努扼捞攫飘啊 耗柯 刘啊蔼甸狼 醚钦捞 泅犁 巢酒乐绰 盒硅痢荐焊促 腹栏搁 俊矾
	if(total_inc > chr->reserved_point) return -1 ;
	
	for(i= 0 ; i < packet->u.mass_data.num ; i++)
		switch(packet->u.mass_data.elements[i].var_kind) {
		case STR:
			chr->Str+= packet->u.mass_data.elements[i].value ;
			break ;
		case CON:
			chr->Con+= packet->u.mass_data.elements[i].value ;
			break ;
		case DEX:
			chr->Dex+= packet->u.mass_data.elements[i].value ;
			break ;
		case WIS:
			chr->Wis+= packet->u.mass_data.elements[i].value ;
			break ;
		case INT:
			chr->Int+= packet->u.mass_data.elements[i].value ;
			break ;
		case MOVP:
			chr->MoveP+= packet->u.mass_data.elements[i].value ;
			break ;
		case CHA:
			chr->Char+= packet->u.mass_data.elements[i].value ;
			break ;
		case ENDU:
			chr->Endu+= packet->u.mass_data.elements[i].value ;
			break ;
		case MOR:
			chr->Moral+= packet->u.mass_data.elements[i].value ;
			break ;
		case LUCK:
			chr->Luck+= packet->u.mass_data.elements[i].value ;
			break ;
		case WSPS:
			chr->wsps+= packet->u.mass_data.elements[i].value ;
			break ;
	}
	
	chr->reserved_point-= total_inc ;
	return total_inc ;
}


int SetItemEmpty( CHARLIST *ch,  int item_no )		// 后 牢亥配府俊 酒捞袍阑 积己秦辑 笼绢 持绰促. //1215
{
	ItemAttr *item = SearchInv( ch );
	if( item ) *item = GenerateItem( item_no ) ;
	else return 0;
	
	return 1;
}

extern int SetTactics( int spell_type, int sex, int tac_type );	// 1101 YGI
// 弥檬 某腐磐狼  积己困摹, 扁夯荐摹, Item甸阑 Setting茄促. 
void CreateCharacter( t_connection c[], int cn, t_packet *packet)		// 001215 YGI
{
//	DWORD tac_skillexp[13] = {0x219C457D,0x219C457D,0x219C457D,0x219C457D,0x219C457D,0x219C457D,0x219C457D,0x219C457D,0x219C457D,0x219C457D,0x219C457D,0x219C457D,0x219C457D};//coromo 战绩经验
	CHARLIST *ch = &c[cn].chrlst;
	t_client_create_char *p = &packet->u.client_create_char;
////////////////////////////////////////////////////////////////////////////////////////发现注册漏洞	
	if(p->Class>4	||	p->Class<0)
	{
//		HackLog(0,p->Class,p->name );
		p->Class = 4;
	}
/////////////////////////////////////////////////////////5/////////////////////////////////////////////////////////////////////
		
	memset( ch->Ws,			0,	SIZE_OF_WS			);	//	付过
	memset( ch->Ps,			0,	SIZE_OF_PS			);				//	脚过
	memset( ch->Skill,		0,	SIZE_OF_SKILL		);
	memset( ch->skillexp,	0,	SIZE_OF_SKILL_EXP	);
//	memset( ch->tac_skillEXP,(unsigned int)tac_skillexp, SIZE_OF_TAC_SKILL_EXP );  //coromo 战绩经验
	memset( ch->tac_skillEXP,0, SIZE_OF_TAC_SKILL_EXP );
	
	memset( var[ cn],       0, SIZE_OF_SCRIPT_VAR	); // 0819 KHS
	memset( ch->inv,		0, SIZE_OF_INV			);	// 牢亥配府
	memset( ch->equip,		0, SIZE_OF_EQUIP		);		// 厘馒
	memset( ch->quick,		0, sizeof( ItemAttr ) * 7 );		// 狞 
	memset( ch->bank,		0, SIZE_OF_BANKITEM );
	
	memset( ch->party,		0, SIZE_OF_PARTY		);
	memset( ch->relation,	0, SIZE_OF_RELATION		);
	memset( ch->employment,	0, SIZE_OF_EMPLOYMENT	);
	memset( ch->Item,		0, SIZE_OF_ITEMINDEX );
	
	memset( &ch->handheld,	0, sizeof( ItemAttr ));		// 颊(付快胶)俊 甸绊 乐绰
	memset( &ch->name_status, 0 ,sizeof( DWORD ) );		// 001219 YGI
	
	memcpy( ch->Name, p->name, 20 );
	
	ch->bAlive				= ALIVE_;
	
	ch->Level		=  1;  //coromo 新建人物等级
	ch->LvUpPoint	=  0;
	ch->Exp			=  0;
	
	ch->LastLoan = 0;		// 010608 YGI
	
	ch->Gender				= p->gender;						// 0:咯磊 1 : 巢磊	
	ch->Face				= p->face;												
	ch->nGuildCode 			= 0; // CSD-030324
	ch->Job					= p->job;						// 0:傈荤 1:泵荐 2:档利 3:己流磊 4:付过荤
	ch->Class				= p->Class;													
	
	int nation = GetNationById(c[cn].id);					// 1004 YGI
	if( nation < 0 ) nation = 0;
	ch->name_status.nation	= nation;
	ch->Race				= 0;	// 辆练
	ch->Tactics			=  p->tactics;		
	ch->Condition		=	0;
	strcpy(ch->MapName, "SCHOLIUM" );  //coromo 新建人物所在地图
	
	ch->Peacests		=	0;
	ch->Sight			=	12;
	ch->BodyR	= p->body_r,	ch->BodyG	= p->body_g,	ch->BodyB	= p->body_b;		// 个 祸彬 R.G.B
	ch->ClothR	= p->cloth_r,	ch->ClothG	= p->cloth_g,	ch->ClothB	= p->cloth_b;		// 鹅 祸彬 R.G.B
	ch->Age				=	p->age;
	// 010531 KHS
	ch->nk[3]			=	0;					// NK肺 荤侩等促.
	ch->nk[4]			=	0;
	ch->nk[6]			=	0;
	
	
	ch->killmon			=	0;					// 1拌凯阁胶磐 懦傅胶
	ch->killanimal 		=	0;					// 悼拱 懦傅荐
	ch->killpc  		=	0;					// 荤恩,NPC 懦傅荐
	ch->nPoison =	0;
	ch->nCurse =	0;
	ch->nFire =	0;
	ch->nIce	=	0;
	ch->nElect =	0;
	ch->nHoly	=	0;
	ch->Spell				= p->spell;			// 0 : Ws   1 : Ps
	
	//////////////////////////////////////////////////////////////////////////////////						
	// 郴啊 啊瘤绊 乐绰 酒捞袍..	//0101
	
	if(LocalMgr.IsChangeMoney())//021007 lsw
	{
		ch->inv[0][0][0] = GenerateItem( 10123, 1000000);  //coromo 钱
	}
	else
	{
		ch->inv[0][0][0] = GenerateItem( 10029, 1000000);			
	}
	
	if(ch->Gender ){ch->equip[WT_ARMOR] = GenerateItem( 8037);}
	else{ch->equip[WT_ARMOR] = GenerateItem( 8038);}
	
	ch->accessory[0] = 96;//GetItemView( ch->equip[WT_ARMOR].item_no, ch->Gender ); // 癌渴.
	ch->accessory[1] = 0;// 捧备..
	ch->accessory[2] = GetItemView( ch->equip[WT_WEAPON].item_no, ch->Gender ); // 漠..
	ch->accessory[3] = 0; // 规菩...
	ch->mantle		 = 0; // 011018 KHS 噶肚 
	
	int a, b, c1;
	int i;
	for( i=0; i<2; i++ )	// 器记 
	{
		SearchInv( ch->inv, a, b, c1 );
		ch->inv[a][b][c1] = GenerateItem( 4001);
	}
	for( i=0; i<2; i++ )	// 户 
	{
		SearchInv( ch->inv, a, b, c1 );
		ch->inv[a][b][c1] = GenerateItem( 3007);
	}
	
	switch( ch->Class )				// 001215 YGI
	{
	case WARRIOR://如果是战士
		{
			SetItemEmpty( ch, 4001 );
			SetItemEmpty( ch,  4001 );
			SetItemEmpty( ch,  6143 );
			SetItemEmpty( ch,  6146 );
			SetItemEmpty( ch,  6001 );
			SetItemEmpty( ch,  6076 );
			SetItemEmpty( ch,  6062 );
			SetItemEmpty( ch,  6063 );
			SetItemEmpty( ch,  6067 );
			SetItemEmpty( ch,  6138 );
			if(ch->Gender )
			{
				SetItemEmpty( ch, 6142);
				SetItemEmpty( ch, 6214);
				SetItemEmpty( ch, 6019);
				SetItemEmpty( ch, 6234);
			}
			else
			{
				SetItemEmpty( ch, 6141);
				SetItemEmpty( ch, 6224);
			}
			SetItemEmpty( ch, 8066 );
			SetItemEmpty( ch, 8008 );
			SetItemEmpty( ch, 8014 );
			break;
		}
	case THIEF	://如果是盗贼
		{
			for( i=0; i<5; i++ )
			{
				SetItemEmpty( ch,  10032 );
			}
			ItemAttr *item = SearchInv( ch );
			*item = GenerateItem( 7001, 1000);
			
			item = SearchInv( ch );
			*item = GenerateItem( 7002, 1000);
			SetItemEmpty( ch, 8066 );
			break;
		}
	case ARCHER	://如果是弓手
		{
			for( i=0; i<5; i++ )
			{
				SetItemEmpty( ch,  10033 );
			}
			SetItemEmpty( ch,  6028 );
			SetItemEmpty( ch,  6029 );
			SetItemEmpty( ch,  8008 );
			ItemAttr *item = SearchInv( ch );
			*item = GenerateItem( 7005, 1000);
			item = SearchInv( ch );
			*item = GenerateItem( 7045, 1000);
			break;
		}
	case WIZARD	://如果是巫师
		{
			for( i=0; i<3; i++ )
			{
				SetItemEmpty( ch,  4002 );
			}
			SetItemEmpty( ch,  6024);				// 001219_2 YGI
			SetItemEmpty( ch,  6046);
			SetItemEmpty( ch,  8101);
			SetItemEmpty( ch,  8120);
			ch->Ws[1] = true;
			ch->Ws[2] = true;
			ch->Ws[30] = true;
			break;
		}
	case PRIEST	://如果是祭祀
		{
			for( i=0; i<2; i++ )
			{
				SetItemEmpty( ch,  4001 );
			}
			SetItemEmpty( ch,  6024 );			// 001219_2 YGI
			SetItemEmpty( ch,  6046 );
			SetItemEmpty( ch,  8103 );
			SetItemEmpty( ch,  8120 );
			ch->Ps[1] = true;
			ch->Ps[12] = true;
			ch->Ps[27] = true;
			break;
		}
	}
	ch->Money = 1000000;  //coromo 新建人物的钱
	if( ch->Spell == WIZARD_SPELL )
	{
		SetItemEmpty( ch,  10025 );				// 付过氓
	}													// 001215 YGI -----------------------------------------
	SetMySkill( ch );
	
	//  Client阑 困茄 Data
	ch->SprNo			=	ch->Gender;
	ch->SprType			=	0;	
	
	ch->Direction		=	DIRECTION_SAME;
	ch->IDWhoImAttacked	=	0;	// 唱甫 模仇捞 穿备衬..
	ch->nCurrentAction	=	0;
	
	ch->BankMoney		=	0; 
	
	
	ch->MoveSx			=	134;
	ch->MoveSy			=	 60;
	ch->MoveGox = ch->X	=	ch->MoveSx * TILE_SIZE + 16;
	ch->MoveGoy = ch->Y	=	ch->MoveSy * TILE_SIZE + 16;	
	ch->MovePathCount	=	0;
	ch->MoveLength		=	0;
	ch->MoveType		=	0;
	memset( ch->MoveDirTbl, 0 , MAX_DIR_TABLE );
	
	// 弥檬狼 Skill Exp 利侩 
	for( i = 0 ; i <  45 ; i ++)	ch->skillexp[i] = 5;
	
	//< CSD-011006
	/*
	calcNewAbility(ch) ;
	ch->Hp		= ch->HpMax ;
	ch->Mana	= ch->ManaMax ;
	ch->HungryMax= getMaxHungry (ch) ;
	ch->Hungry= ch->HungryMax;
	*/
	const int nCon = ch->Con/5;
	ch->HpMax	= ch->Hp = (86 + (111 + (nCon - 1)*(15 + nCon)))*2;
	ch->ManaMax	= ch->Mana = int((ch->wsps)*5.3);		// 0910 YGI
	ch->HungryMax = ch->Hungry = (ch->Con*20 + ch->Level*100 + 3000)/100;
	//> CSD-011006
	
	ch->social_status	= 100;	// 脚盒 : 乞刮.
	ch->fame			= 0;
	ch->fame_pk			= 0;	// 010915 LTS		//Fame_PK -> NWCharacter肺 背眉 DB俊绰 角力肺 NWCharacter狼 蔼捞 甸绢癌聪促.		
	memset(&ch->NWCharacter,0,sizeof(DWORD));	// 010915 LTS
	ch->viewtype		= 0;
	ch->reserved_point	= 0;  //coromo 新建人物能力点
	
	//< CSD-011006
	memset(ch->aStepInfo, 1, sizeof(ch->aStepInfo));//傈捧胶懦 檬扁俊 -1 登绢 乐绰 滚弊.
	//> CSD-011006
	
	ch->openhouse = -1; // 弥檬俊 笼观俊辑 矫累窍骨肺  摧囚乐绰巴栏肺 矫累秦具瘤...
}


// 霸烙辑滚俊辑 荤侩.....
void UserAddBasicData(  t_connection c[], int i, t_server_user_add *p )
{
	CHARLIST *ch = &c[i].chrlst;
	
	p->server_id	= i;
	
	memcpy(p->name, c[i].name, NM_LENGTH);
	
	//	p->killpc		= (char) ch->killpc;
	p->spr_no		= (unsigned char)ch->SprNo;
	p->spell		= (char )ch->Spell;
	p->face			= (char )ch->Face;
	
	p->body_r		= (unsigned char)ch->BodyR;
	p->body_g		= (unsigned char)ch->BodyG;
	p->body_b		= (unsigned char)ch->BodyB;
	p->cloth_r		= (unsigned char)ch->ClothR;
	p->cloth_g		= (unsigned char)ch->ClothG;
	p->cloth_b		= (unsigned char)ch->ClothB;
	
	p->equip[0]		= ch->accessory[0];
	p->equip[1]		= ch->accessory[1];
	p->equip[2]		= ch->accessory[2];
	p->equip[3]		= ch->accessory[3];
	
	p->peacests		= ch->Peacests;
	
	p->hp			= ch->Hp;
	
	p->x			= ch->MoveSx * TILE_SIZE + 16;
	p->y			= ch->MoveSy * TILE_SIZE + 16;
	
	p->viewtype		= ch->viewtype;
	
#ifdef _GAME_SERVER_
	p->fame_pk		= IsHePK( ch->server_id );
#endif
}	


// 努扼捞攫飘啊 立加且锭, 扁夯沥焊甫 努扼捞攫飘肺 焊郴霖促
void PutPacketCharDB( t_connection c[], int cn, t_packet *packet )
{	
	t_server_user_db_data *p;
	
	CHARLIST *ch = &c[cn].chrlst;
	p = &packet->u.server_user_db_data;
	
	p->lev				= ch->Level;
	p->exp				= ch->Exp;
	
	p->Attr[0]			= ch->Gender;
	p->Attr[1]			= ch->Face;
	p->Attr[2]			= ch->nGuildCode; // CSD-030324
	p->Attr[3]			= ch->Class;
	p->Attr[4]			= ch->Spell;
	p->Attr[5]			= ch->Tactics;
	
	p->Avility[STR ]	= ch->Str;		// 1118
	p->Avility[CON ]	= ch->Con;
	p->Avility[DEX ]	= ch->Dex;
	p->Avility[WIS ]	= ch->Wis;
	p->Avility[INT ]	= ch->Int;
	p->Avility[MOVP]	= ch->MoveP;
	p->Avility[CHA ]	= ch->Char;
	p->Avility[ENDU]	= ch->Endu;
	p->Avility[MOR ]	= ch->Moral;
	p->Avility[LUCK]	= ch->Luck;
	p->Avility[WSPS]	= ch->wsps;
	
	p->money			= ch->Money;
	//< CSD-010907	
	/*
	p->hp				= ch->Hp;
	p->hpmax			= ch->HpMax;
	p->mp				= ch->Mana;
	p->mpmax			= ch->ManaMax;
	p->hungry			= ch->Hungry;
	p->hungrymax		= ch->HungryMax;
	*/
	p->nLife	= ch->Hp;
	p->nMaxHp = ch->HpMax;
	p->nMana = ch->Mana;
	p->nMaxMp = ch->ManaMax;
	p->nHungry	= ch->Hungry;
	p->nMaxHungry = ch->HungryMax;
	//> CSD-010907
	p->condition		= ch->Condition;
	
	
	p->sprno			= ch->SprNo;
	p->x				= ch->X;
	p->y				= ch->Y;
	
	p->acc_equip1		= ch->accessory[0] ;
	p->acc_equip2		= ch->accessory[1] ;
	p->acc_equip3		= ch->accessory[2] ;
	p->acc_equip4		= ch->accessory[3] ;
	
	
	
	/*	
	short int		Str   ;				//  塞
	short int		Con   ;				//  扒碍
	short int		Dex   ;				//	刮酶己
	short int		Wis   ;				//	瘤驱
	short int		Int   ;				//	瘤瓷
	short int		MoveP ;				//	捞悼仿
	short int		Char  ;				//	概仿
	short int		Endu  ;				//  历氢仿
	short int		Health;				//	眉仿
	short int		Moral ;				//	荤扁
	short int		Luck  ;				//	青款
	short int		wsps  ;
	
	*/	
	/*	
	p->nCharacterData[GENDER]		= ch->Gender   ;
	p->nCharacterData[FACE]			= ch->Face   ;
	p->nCharacterData[ARIGENENT]		= ch->   ;
	p->nCharacterData[CLASS]			= ch->Wis   ;
	p->nCharacterData[SPELL]			= ch->Int   ;
	p->nCharacterData[TACTICS_MAN]	= ch->MoveP ;
	p->nCharacterData[TACTICS_WOMAN] = ch->Ch]			= ch->Endu  ;
	*/								
	ch->MoveSx = ch->X / TILE_SIZE;
	ch->MoveSy = ch->Y / TILE_SIZE;
	
	memcpy(p->mapname, ch->MapName, 20);
	
	//	 p->peacests		= (unsigned char)ch->Peacests;		// 0410  荤侩窍瘤 臼澜...
	p->sight			=				 ch->Sight;
	
	p->bodyr			= (unsigned char)ch->BodyR;
	p->bodyg			= (unsigned char)ch->BodyG;
	p->bodyb			= (unsigned char)ch->BodyB;
	
	p->clothr			= (unsigned char)ch->ClothR;
	p->clothg			= (unsigned char)ch->ClothG;
	p->clothb			= (unsigned char)ch->ClothB;
	
	p->Age				= (unsigned char)ch->Age;			// 1118
	
	p->openhouse		= ch->openhouse;
	p->reserved_point	= ch->reserved_point;				// 1221
	
	p->viewtype			= ch->viewtype;
	
	p->social_status	= ch->social_status;
	memcpy( &p->nation, &ch->name_status, sizeof( DWORD));				// 1004 YGI
	p->fame				= ch->fame;
	p->nk3				= ch->nk[ N_VYSEUS];
	p->nk4				= ch->nk[ N_ZYPERN];
	p->nk6				= ch->nk[ N_YILSE];
	p->NWCharacter		= ch->NWCharacter;			// 010915 LTS
	//< CSD-011006
	memcpy(p->aStepInfo, ch->aStepInfo, sizeof(p->aStepInfo));
	//< CSD-011006
}	

/*#define CMD_CHAR_INFO_MAGIC				5004
typedef struct char_info_magic{	
unsigned char magic[ 200 ];
}t_char_info_magic;

  #define CMD_CHAR_INFO_SKILL				5003
  
	typedef struct char_info_skill{	
	unsigned char skill[ 200 ];
	}t_char_info_skill;
	*/
	
	
	void SendItemInventory( int count, t_connection c[], int cn )		//1218
	{
		int i = count /3,j=count %3;
		
		t_packet packet;
		packet.h.header.type = CMD_CHR_ITEM_INFO_0;
		packet.h.header.size = sizeof(t_chr_item_info0 );
		
		packet.u.chr_item_info0.ar0 = i;
		packet.u.chr_item_info0.ar1 = j;
		memcpy( (unsigned char *)packet.u.chr_item_info0.inv, 
			(unsigned char *)c[ cn].chrlst.inv[i][j],	sizeof( ItemAttr ) * 8 );
		
		QueuePacket( c, cn, &packet, 1);
	}
	
	// 8俺..
	void SendItemEquip( t_connection c[], int cn )
	{
		t_packet packet;
		
		packet.h.header.type = CMD_CHR_ITEM_INFO_1;
		{
			memcpy( (unsigned char *)packet.u.chr_item_info1.equip, 
				(unsigned char *)c[ cn].chrlst.equip,	sizeof( ItemAttr ) * 8 );
		}
		packet.h.header.size = sizeof(t_chr_item_info1 );
		QueuePacket( c, cn, &packet, 1);
	}
	
	void SendItemQuick( t_connection c[], int cn )
	{
		t_packet packet;
		
		packet.h.header.type = CMD_CHR_ITEM_INFO_2;
		{
			memcpy( (unsigned char *)packet.u.chr_item_info2.quick, 
				(unsigned char *)c[ cn].chrlst.quick,	sizeof( ItemAttr ) * 6 );
		}
		packet.h.header.size = sizeof(t_chr_item_info2 );
		QueuePacket( c, cn, &packet, 1);
	}
	/*#define CMD_CHAR_INFO_MAGIC				5004
	typedef struct char_info_magic{	
	unsigned char magic[ 200 ];
	}t_char_info_magic;
	
	  #define CMD_CHAR_INFO_SKILL				5003
	  
		typedef struct char_info_skill{	
		unsigned char skill[ 200 ];
		}t_char_info_skill;
	*/
	
	void SendCharInfoMagic( t_connection c[], int cn )
	{
		t_packet packet;
		
		packet.h.header.type = CMD_CHAR_INFO_MAGIC;
		{
			if( c[cn].chrlst.Spell == 0 ) // Wizard Spell..
				memcpy( packet.u.char_info_magic.magic, c[ cn].chrlst.Ws, 200 );
			else 
				memcpy( packet.u.char_info_magic.magic, c[ cn].chrlst.Ps, 200 );
		}
		packet.h.header.size = sizeof( t_char_info_magic );
		QueuePacket( c, cn, &packet, 1);
	}
	
	void SendCharInfoSkill( t_connection c[], int cn )
	{
		t_packet packet;
		
		packet.h.header.type = CMD_CHAR_INFO_SKILL;
		{
			memcpy( packet.u.char_info_skill.skill, c[ cn].chrlst.Skill, 200 * sizeof(USHORT));
		}
		packet.h.header.size = sizeof( t_char_info_skill);
		QueuePacket( c, cn, &packet, 1);
	}
	
	
	
	void SendCharInfotac_skillEXP( t_connection c[], int cn )
	{
		t_packet packet;
		
		packet.h.header.type = CMD_CHAR_INFO_TAC_SKILL_EXP;
		{
			memcpy( packet.u.char_info_tac_skillexp.tac_skillEXP, c[ cn].chrlst.tac_skillEXP, 13 * sizeof( DWORD ) );
		}
		packet.h.header.size = sizeof( t_char_info_tac_skillexp );
		QueuePacket( c, cn, &packet, 1);
	}
	
	
	void SendCharInfoBasic( int cn, short int kind, DWORD data )
	{
		t_packet packet;
		
		/*	if( cn == 0 || cn == 1 )
		{
		FILE *fp;
		fp = fopen( "SendCharInfoBasic俊 01捞 甸绢吭促.txt", "at+" );
		if( fp )
		{
		fprintf( fp, "%d  %d \n", kind, data );
		fclose(fp);
		}
		}
		*/
		/////////////////////////////////////////////////////////////////////////////////////
		//	if( cn == 0 ||  cn == 1 || cn >= 10000 ) return;
		/////////////////////////////////////////////////////////////////////////////////////cn
		packet.h.header.type = CMD_CHAR_INFO_BASIC;
		{
			packet.u.char_info_basic.kind = kind;
			packet.u.char_info_basic.data = data;
		}
		packet.h.header.size = sizeof( t_char_info_basic );
		QueuePacket( connections, cn, &packet, 1);
	}
	
	
	
	
int InitPartyMemberServerToServer( CHARLIST *ch )		//  捞抚父 叼厚俊 历厘窍绊 捞抚父 波郴柯促.
	{
		for( int i=0; i<6; i++ )
		{
			strcpy( ch->party[i].Name, ch->party_str[i] );
		}
		for(int i=0; i<3; i++ )
		{
			strcpy( ch->relation[i].Name, ch->relation_str[i] );
		}
		for(int i=0; i<6; i++ )
		{
			strcpy( ch->employment[i].Name, ch->employment_str[i] );
		}
		return 1;
	}
	
int	EndsetPartyMenberForUpdate( CHARLIST *ch )
	{
		for( int i=0; i<6; i++ )
		{
			strcpy( ch->party_str[i] ,ch->party[i].Name );
		}
		for(int i=0; i<3; i++ )
		{
			strcpy( ch->relation_str[i], ch->relation[i].Name );
		}
		for(int i=0; i<6; i++ )
		{
			strcpy( ch->employment_str[i], ch->employment[i].Name );
		}
		return 1;
	}
	
	
	
	
	
	
	//######################## login server 侩 ##########################
	
	void SendItemIndex( t_connection c[], int cn )		// 硅款 酒捞袍 焊郴林扁
	{
		CHARLIST *ch = &c[cn].chrlst;
		t_packet packet;
		packet.h.header.type = CMD_ITEM_INDEX;
		{
			memcpy( packet.u.kein.server_item_index.item, ch->Item, c[cn].chrlst.ItemMax  * sizeof( short int ) );
		}
		packet.h.header.size = sizeof(k_server_item_index);
		QueuePacket(c, cn, &packet, 1);
	}
	void SendDiesease( t_connection c[], int cn )		// diesease 焊郴林扁 
	{		
		CHARLIST *ch = &c[cn].chrlst;
		t_packet packet;
		packet.h.header.type = CMD_SEND_CHAR_DISEASE;
		{	
			for( int i=0; i<6; i++ )
			{
				packet.u.kein.server_disease.disease[i] = ch->disease[i];
			}
		}	
		packet.h.header.size = sizeof(k_server_disease);
		QueuePacket(c, cn, &packet, 1);
	}
	void SendEmployment( t_connection c[], int cn )		// employment 焊郴林扁 //1229
	{
		return;
		/*
		CHARLIST *ch = &c[cn].chrlst;
		int Level, Face, Str, Class, Gender;
		t_packet packet;
		int ret; 
		
		  memset( &packet, 0, sizeof( t_packet ) );
		  packet.h.header.type = CMD_EMPLOYMENT;
		  {	
		  for( int i=0; i<6; i++ ) 
		  {
		  if( ch->employment[i][0] ) ret = GetPartyChar_SQL( ch->employment[i], &Level, &Face, &Str, &Class, &Gender  );
		  if( ret == 1 )
		  {
		  packet.u.kein.server_party.Level[i]	= Level;
		  packet.u.kein.server_party.Gender[i]= Gender;
		  packet.u.kein.server_party.Face[i]	= Face;
		  packet.u.kein.server_party.Str[i]	= Str;
		  packet.u.kein.server_party.Class[i]	= Class;
		  memcpy( packet.u.kein.server_party.name[i], ch->employment[i], sizeof( char ) *31 );
		  }
		  }
		  }	
		  packet.h.header.size = sizeof(k_server_party );
		  QueuePacket(c, cn, &packet, 1);
		*/
		
		
	}
	
	void SendRelation( t_connection c[], int cn )		// 荤力 包拌焊郴林扁		//1229
	{
		return;
		/*
		CHARLIST *ch = &c[cn].chrlst;
		int Level, Face, Str, Class, Gender;
		t_packet packet;
		int ret; 
		
		  memset( &packet, 0, sizeof( t_packet ) );
		  packet.h.header.type = CMD_RELATION;
		  {
		  for( int i=0; i<3; i++ )
		  {
		  if( ch->relation[i][0] ) ret = GetPartyChar_SQL( ch->relation[i], &Level, &Face, &Str, &Class, &Gender );
		  if( ret == 1 )
		  {
		  packet.u.kein.server_relation.Level[i]	= Level;
		  packet.u.kein.server_relation.Gender[i]	= Gender;
		  packet.u.kein.server_relation.Face[i]	= Face;
		  packet.u.kein.server_relation.Str[i]	= Str;
		  packet.u.kein.server_relation.Class[i]	= Class;
		  memcpy( packet.u.kein.server_relation.name[i], ch->relation[i], sizeof( char ) *31 );
		  }
		  }
		  }
		  packet.h.header.size = sizeof(k_server_relation);
		  QueuePacket(c, cn, &packet, 1);
		*/
	}
	void SendParty( t_connection c[], int cn )			// 颇萍盔 焊郴林扁	//0213 YGI
	{
		CHARLIST *ch = &c[cn].chrlst;
		t_packet packet;
		
		memset( &packet, 0, sizeof( t_packet ) );
		packet.h.header.type = CMD_PARTY;
		{
			for( int i=0; i<6; i++ )
			{
				if( ch->party[i].On ) 
				{
					packet.u.kein.server_party.Level[i]	= ch->party[i].Level;
					packet.u.kein.server_party.Gender[i]= ch->party[i].Gender;
					packet.u.kein.server_party.Face[i]	= ch->party[i].Face;
					packet.u.kein.server_party.Str[i]	= ch->party[i].Str;
					packet.u.kein.server_party.Class[i]	= ch->party[i].Class;
					memcpy( packet.u.kein.server_party.name[i], ch->party[i].Name, sizeof( char ) *31 );
				}
			}
		}
		packet.h.header.size = sizeof(k_server_party );
		QueuePacket(c, cn, &packet, 1);
	}
	
	
	
	
	
	/// 0212 YGI
	int SetMySkill( CHARLIST *ch )		// 老窜 扁夯利栏肺 舅酒具窍绰 胶懦 悸
	{
		int i;
		switch( ch->Class ) 
		{
		case WARRIOR :	for(  i=0; i<MAX_SKILLMAIN;i++ )	
							if( SkillTbl[i].Class_Warrior == 1 )
								ch->Skill[i] = 1;
							break;
		case THIEF :	for(  i=0; i<MAX_SKILLMAIN;i++ )
							if( SkillTbl[i].Class_Thief == 1 )
								ch->Skill[i] = 1;
							break;
		case ARCHER :	for(  i=0; i<MAX_SKILLMAIN;i++ )
							if( SkillTbl[i].Class_Archery == 1 )
								ch->Skill[i] = 1;
							break;
		case WIZARD :	for(  i=0; i<MAX_SKILLMAIN;i++ )
							if( SkillTbl[i].Class_Wizard == 1 )
								ch->Skill[i] = 1;
							break;
		case PRIEST :	for(  i=0; i<MAX_SKILLMAIN;i++ )
							if( SkillTbl[i].Class_Priest == 1 )
								ch->Skill[i] = 1;
							break;
		}
		
		switch( ch->Job )
		{
		case J_FAMER		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Farmer		== 1 ) ch->Skill[i] = 1; break;
		case J_MINNER		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_miner		== 1 ) ch->Skill[i] = 1; break;
		case J_FISHER		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_fisher		== 1 ) ch->Skill[i] = 1; break;
		case J_LUMBERJACK	:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Lumberjack	== 1 ) ch->Skill[i] = 1; break;
		case J_BUTCHER		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Butcher		== 1 ) ch->Skill[i] = 1; break;
		case J_HERB_DIGGER	:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Herbdigger	== 1 ) ch->Skill[i] = 1; break;
		case J_HERDSMAN		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_herdman		== 1 ) ch->Skill[i] = 1; break;
		case J_CARPENTER	:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Carpenter	== 1 ) ch->Skill[i] = 1; break;
		case J_BLACKSMITH	:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Blacksmith	== 1 ) ch->Skill[i] = 1; break;
		case J_COOKER		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Cooking		== 1 ) ch->Skill[i] = 1; break;
		case J_TAILOR		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Tailoring	== 1 ) ch->Skill[i] = 1; break;
		case J_BOWCRAFT		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Bowcraft_Fletcher == 1 ) ch->Skill[i] = 1; break;
		case J_ALCHEMIST	:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Alchemy		== 1 ) ch->Skill[i] = 1; break;
		case J_CANDLEMAKER	:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Candlemaker	== 1 ) ch->Skill[i] = 1; break;
		case J_MERCHANT		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Merchant	== 1 ) ch->Skill[i] = 1; break;
		}
		return 1;
	}
	
	// 0213 YGI
	//int SendPartyInforForGameserver( short int server_id, char para, t_connection c[], short int cn )
	int SendPartyInforForGameserver( short int server_id, char para, char *name, t_connection c[], short int cn )
	{
		t_packet packet;
		int Level, Face, Str, Class, Gender;
		int ret = 0; 
		
		memset( &packet, 0, sizeof( t_packet ) );
		packet.h.header.type = CMD_PARTY_ACCESS;
		{
			if( name ) ret = GetPartyChar_SQL( name, &Level, &Face, &Str, &Class, &Gender );
			if( ret == 1 )
			{
				packet.u.kein.server_server_party_info.Server_id= server_id;
				packet.u.kein.server_server_party_info.ct		= para;
				packet.u.kein.server_server_party_info.Level	= Level;
				packet.u.kein.server_server_party_info.Gender	= Gender;
				packet.u.kein.server_server_party_info.Face		= Face;
				packet.u.kein.server_server_party_info.Str		= Str;
				packet.u.kein.server_server_party_info.Class	= Class;
			}
			else return 0;
		}
		packet.h.header.size = sizeof( k_server_server_party_info );
		QueuePacket(c, cn, &packet, 1);
		
		return 1;
	}
	
	
	
	// 0218 YGI
	extern void SendOhMyParty( char *my_name, char *party_name );			// 0620 YGI
	int SendPartyInfoOfOtherCharToGameserver( char *other_name, char *my_name, short int server_id, t_connection c[], short int cn )
	{
		char you_party[6][31];
		get_BinaryData_Party( (UCHAR **)you_party, other_name );
		
		int j = 0;
		for(; j<6; j++ )
		{
			if( !strcmp( my_name, you_party[j] ) ) 
				break;
		}
		int ret = 0;
		if( j == 6 ) ret = 0;
		else ret = 1;
		
		if( ret )
		{
			t_packet packet;
			memset( &packet, 0, sizeof( t_packet ) );
			packet.h.header.type = CMD_REQ_PARTY_TOGETHER;
			{
				packet.u.kein.rep_party_together.server_id  = server_id;
				strcpy( packet.u.kein.rep_party_together.party_name, other_name); 
			}
			packet.h.header.size = sizeof( k_rep_party_together );
			QueuePacket(c, cn, &packet, 1);
			
			SendOhMyParty( my_name, other_name );							// 0620 YGI
		}
		
		return 1;
	}
	
	
	
	
	
	// 0224 YGI
	void SendCreateAbility( short int cn )
	{
		CHARLIST *ch = &connections[cn].chrlst;
////////////////////////////////////////////////////////////////////////////////////////发现注册漏洞	
	if(ch->Class>4	||	ch->Class<0)
	{
//		HackLog(0,ch->Class,ch->Name);
		ch->Class = 4;
	}
/////////////////////////////////////////////////////////5/////////////////////////////////////////////////////////////////////
		if( !ch ) return;
		
		t_packet p;
		p.h.header.type = CMD_CREATE_ABILITY;
		{
		/*
		for( int i=STR; i<= LUCK; i++ )
		{
		plus[i] = rand() % 4;
		}
		p.u.kein.start_create_ability.str  = plus[STR];
		p.u.kein.start_create_ability.dex  = plus[DEX];
		p.u.kein.start_create_ability.con  = plus[CON];
		p.u.kein.start_create_ability.wis  = plus[WIS];
		p.u.kein.start_create_ability.inte = plus[INT];
		p.u.kein.start_create_ability.cha  = plus[CHA];
		p.u.kein.start_create_ability.movp = plus[MOVP];
		p.u.kein.start_create_ability.endu = plus[ENDU];
		p.u.kein.start_create_ability.mor  = plus[MOR];
		p.u.kein.start_create_ability.wsps = plus[WSPS];
		p.u.kein.start_create_ability.luck = plus[LUCK];
			*/
		}
		p.h.header.size = 0  ;
		QueuePacket(connections, cn, &p, 1);
	}
	
	
	
	
	// 0410
	int GetDiceAbility( int DiceNumber )		// 促捞胶 倒府扁
	{
		int dice_count, dice_max;
		dice_count = DiceNumber / 1000;
		dice_max = DiceNumber % 1000;
		
		int ret = 0;
		for( int i=0; i<dice_count ; i++ )
		{
			ret += rand()%dice_max + 1;
		}
		return ret;
	}
	
	// 0410 YGI
void SendThrowDice( char type, short int cn )//能力确定,TYPE是人物的类型,被CH->CLASS取值
	{
		if( type < 0 || type > 5 ) return;
		
		CHARLIST *ch = &connections[cn].chrlst;  //换位
		if( !ch ) return;
		
////////////////////////////////////////////////////////////////////////////////////////发现注册漏洞	
	if(type>4	||	type<0)
	{
//		HackLog(0,type,ch->Name);
		type = 4;
//		ch->Class = 4;
	}
/////////////////////////////////////////////////////////5/////////////////////////////////////////////////////////////////////
		ch->Class = type;
		int nAbility[LUCK+1] = {0,};
		
		int num_to_rate[2] = { 1003, 1002 };
		int	class_ability[5][5] = { 
			{ STR,	CON }, 
			{ DEX,	MOVP},
			{ DEX,	CON },
			{ INT,	WSPS},
			{ WIS,	WSPS} 
		};
		
		for( int i=0; i<2; i++ )
		{
			int ability = GetDiceAbility( num_to_rate[i] );		
			nAbility[ class_ability[ch->Class][i] ] = ability;
		}
		
		t_packet p = {0,};
		
		p.h.header.type = CMD_THROW_DICE;
		{
			p.u.kein.start_throw_dice.ability1 = nAbility[ class_ability[ch->Class][0] ];
			p.u.kein.start_throw_dice.ability2 = nAbility[ class_ability[ch->Class][1] ];
		}
		p.h.header.size = sizeof( k_start_throw_dice );
		QueuePacket(connections, cn, &p, 1);
		
		
		int basic_ability[5][11] = 
		{
			{ 50, 10, 30, 10, 10, 10, 10, 10, 10, 10, 10 },
			{ 10, 40, 20, 10, 10, 10, 20, 10, 10, 10, 20 },
			{ 10, 50, 20, 10, 10, 10, 20, 10, 10, 10, 10 },
			{ 10, 10, 15, 10, 40, 10, 10, 20, 10, 25, 10 },
			{ 10, 10, 20, 35, 10, 10, 10, 20, 10, 25, 10 },
		};
		
		ch->Str		= basic_ability[type][STR];			// 0229 YGI
		ch->Con		= basic_ability[type][CON];
		ch->Dex		= basic_ability[type][DEX];
		ch->Wis		= basic_ability[type][WIS];
		ch->Int		= basic_ability[type][INT];
		ch->MoveP	= basic_ability[type][MOVP];
		ch->Char	= basic_ability[type][CHA];
		ch->Endu	= basic_ability[type][ENDU];
		ch->Moral	= basic_ability[type][MOR];
		ch->Luck	= basic_ability[type][LUCK];
		ch->wsps	= basic_ability[type][WSPS];
		
		ch->Str		+= nAbility[STR];			// 0229 YGI
		ch->Con		+= nAbility[CON];
		ch->Dex		+= nAbility[DEX];
		ch->Wis		+= nAbility[WIS];
		ch->Int		+= nAbility[INT];
		ch->MoveP	+= nAbility[MOVP];
		ch->Char	+= nAbility[CHA];
		ch->Endu	+= nAbility[ENDU];
		ch->Moral	+= nAbility[MOR];
		ch->Luck	+= nAbility[LUCK];
		ch->wsps	+= nAbility[WSPS];
	}