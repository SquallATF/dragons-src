﻿#include "..\stdafx.h"
#include "..\LowerLayers\servertable.h"
#include "..\LowerLayers\recvmsg.h"
#include "..\LowerLayers\mylog.h"
#include "DefaultHeader.h"
#include "MenuDefine.h"
#include "ItemList.h"
#include "CItem.h"
#include "Scrp_exe.h"
#include "SealStone.h"
#include "SealStonegameserver.h"
#include "op_magic.h"
#include "map.h"
#include "teambattle.h"
#include "gamblerate.h"
#include "LTSSupport.h"
#include "Skill_lsw.h"
#include "GmMgr.h"
#include "eventmgr.h" //021030 YGI
#include "LogManager.h"
#include "UserManager.h"
#include "DualManager.h"
#include "GuildManager.h"
#include "ArenaManager.h"
#include "ItemMallManager.h"

const int SIZE_GR_BS = 50; // CSD-030326
const int SIZE_GR_RS = 100; // CSD-030326

CGambleRateData g_game_rate_data;
CAliveMapZoneAll g_AliveMap;		// 되살아 날수 있는 신전테이블

extern struct CItem_List Item_Ref ;
extern bool exist(int item_no, int char_id, POS &source);
extern void getItemIndex(int item_no, int &type, int &index);
extern int GetItemValueSell( ItemAttr *item );
extern void CleanUpConnection(short idServer);

void SendStoreItemList( int cn, int storelistindex, int menud);
void SendCallSmallMenu( int cn, int storelistindex, int menu );
void SendCallDualMagicLearnMenu( int cn);//soto-030421
int LoadGuildItem( char *filename, int num );

char g_szPassWordName[20];
int		 storelist_attr[ MAX_STORE_LIST][ MAX_STORE_ITEM_LIST];		// 사는 물건 파는 물건 속성
ItemAttr storelist[ MAX_STORE_LIST][ MAX_STORE_ITEM_LIST];
int storelistmax[ MAX_STORE_LIST];
//  상정의 Item 속성은 매일 바뀐다. 
//  이함수는 음.. 1일에 한번정도 호출되어야 한다. 
static inline void swap( int  *a, int *b )
{
	int t = *a;
	*a = *b;
	*b = t;	
}			

void MakeStoreList( void )
{
	char temp[ FILENAME_MAX];
	wsprintf( temp, "%s/data/BLACKSMITH.txt", GameServerDataPath );	storelistmax[ BLACKSMITH ] = LoadGuildItem( temp,	BLACKSMITH	);	//대장간		
	MyLog( LOG_NORMAL, " Store 'BLACKSMITH' itemlist number   : %d", storelistmax[ BLACKSMITH ] );
	wsprintf( temp, "%s/data/FARM.txt", GameServerDataPath );		storelistmax[ FARM		 ] = LoadGuildItem( temp,	FARM		);	//농장		
	MyLog( LOG_NORMAL, " Store 'FARM'       itemlist number   : %d", storelistmax[ FARM] );
	wsprintf( temp, "%s/data/RESTAURANT.txt", GameServerDataPath );	storelistmax[ RESTAURANT ] = LoadGuildItem( temp,	RESTAURANT	);	//요리점		
	MyLog( LOG_NORMAL, " Store 'RESTAURANT' itemlist number   : %d", storelistmax[ RESTAURANT] );
	wsprintf( temp, "%s/data/HERBSHOP.txt", GameServerDataPath );	storelistmax[ HERBSHOP	 ] = LoadGuildItem( temp,	HERBSHOP	);	//약초점		
	MyLog( LOG_NORMAL, " Store 'HERBSHOP'   itemlist number   : %d", storelistmax[ HERBSHOP	 ] );
	wsprintf( temp, "%s/data/SHAMBLES.txt", GameServerDataPath );	storelistmax[ SHAMBLES	 ] = LoadGuildItem( temp,	SHAMBLES	);	//푸줏간		
	MyLog( LOG_NORMAL, " Store 'SHAMBLES'   itemlist number   : %d", storelistmax[ SHAMBLES	 ] );
	wsprintf( temp, "%s/data/CANDLE.txt", GameServerDataPath );		storelistmax[ CANDLE	 ] = LoadGuildItem( temp,	CANDLE		);	//양초점		
	MyLog( LOG_NORMAL, " Store 'CANDLE'     itemlist number   : %d", storelistmax[ CANDLE	 ] );
	wsprintf( temp, "%s/data/SLOPSHOP.txt", GameServerDataPath );	storelistmax[ SLOPSHOP	 ] = LoadGuildItem( temp,	SLOPSHOP	);	//의상실		
	MyLog( LOG_NORMAL, " Store 'SLOPSHOP'   itemlist number   : %d", storelistmax[ SLOPSHOP  ] );
	wsprintf( temp, "%s/data/ALCHEMY.txt", GameServerDataPath );	storelistmax[ ALCHEMY	 ] = LoadGuildItem( temp,	ALCHEMY		);	//연금소		
	MyLog( LOG_NORMAL, " Store 'ALCHEMY'    itemlist number   : %d", storelistmax[ ALCHEMY	 ] );
	wsprintf( temp, "%s/data/WOODWORKING.txt", GameServerDataPath );storelistmax[ WOODWORKING] = LoadGuildItem( temp,	WOODWORKING	);	//목공소		
	MyLog( LOG_NORMAL, " Store 'WOODWORKING'itemlist number   : %d", storelistmax[WOODWORKING] );
	wsprintf( temp, "%s/data/BOWCRAFT.txt", GameServerDataPath);	storelistmax[ BOWCRAFT	 ] = LoadGuildItem( temp,	BOWCRAFT	);		//활과화살	
	MyLog( LOG_NORMAL, " Store 'BOWCRAFT'   itemlist number   : %d", storelistmax[ BOWCRAFT	 ] );
	wsprintf( temp, "%s/data/SHIPYARD.txt", GameServerDataPath );	storelistmax[ SHIPYARD	 ] = LoadGuildItem( temp,	SHIPYARD	);	//조선소		
	MyLog( LOG_NORMAL, " Store 'SHIPYARD'   itemlist number   : %d", storelistmax[ SHIPYARD	 ] );
	wsprintf( temp, "%s/data/PUB.txt", GameServerDataPath );		storelistmax[ PUB		 ] = LoadGuildItem( temp,	PUB			);	//주점		
	MyLog( LOG_NORMAL, " Store 'PUB'        itemlist number   : %d", storelistmax[ PUB		 ] );
	wsprintf( temp, "%s/data/STORE.txt", GameServerDataPath );		storelistmax[ STORE		 ] = LoadGuildItem( temp,	STORE		);	//잡화상		
	MyLog( LOG_NORMAL, " Store 'STORE'      itemlist number   : %d", storelistmax[ STORE	 ] );
	wsprintf( temp, "%s/data/MAGIC_GUILD.txt", GameServerDataPath );storelistmax[MAGIC_GUILD ] = LoadGuildItem( temp,	MAGIC_GUILD );	//마법사		
	MyLog( LOG_NORMAL, " Store 'MAGIC_GUILD'itemlist number   : %d", storelistmax[ MAGIC_GUILD ] );
}

void CallSmallMenu(int cn, int storelistindex, int menu)
{	//< CSD-030509
	switch (menu)
	{
	case MN_SKILLGUILD_REPAIR:
	case MN_SHOP_SELL:
	case MN_SHOP_BUY:
		{
			SendStoreItemList(cn, storelistindex, menu);
			break;
		}
	case MN_SKILLGUILD_BUY:	
	case MN_SKILLGUILD_SELL:	
		{
			SendStoreItemList(cn, storelistindex, menu); 
			SendCallSmallMenu(cn, storelistindex, menu);
			break;
		}
	case MN_ARENA_LOBBY:
		{
			CBaseArena* pGame = g_pArenaManager->GetGameInfo(storelistindex);

			if (pGame != NULL)
			{	//< CSD-TW-030627
				if (!pGame->IsPlayGame())
				{
					SendCallSmallMenu(cn, storelistindex, menu);
					return;
				}
				
				CHARLIST* pTarget = CheckServerId(cn);

				if (pTarget != NULL)
				{
					pGame->SendRemainTime(pTarget, g_curr_time);
				}
			}	//> CSD-TW-030627

			break;
		}
	default:
		{
			SendCallSmallMenu(cn, storelistindex, menu);
			break;
		}
	}
}	//> CSD-030509

// 010815 YGI
void RecvItemBuy( int cn, t_client_item_buy *p )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
//	ch->Money = GetMoneyByItem( ch );

	int store_id = p->store_id;
	int itemno = p->item_no;
	int a,b,c;
	int itempos;
	
	if( store_id >= MAX_STORE_LIST ) return; // Error....
	if( !itemno ) return;

	for( itempos=0; itempos<storelistmax[store_id]; itempos++)
	{
		if( storelist_attr[store_id][itempos] == SIT_EVENT_ITEM ||
			storelist_attr[store_id][itempos] == SIT_NULL ||
			storelist_attr[store_id][itempos] == SIT_ONLY_BUY ) continue;

		if( itemno == storelist[ store_id][itempos].item_no )
		{
			break;
		}
	}
	if( itempos == storelistmax[store_id] ) return;		// 이 상점에 없는 아이템이다.

	ItemAttr *item = &storelist[ store_id][itempos];	
	CItem *t = ItemUnit( *item );
	if( !t ) return;

	const DWORD price = t->GetValue();
	if( ch->Money < price ) return;			// 일단 돈 없으면 안된다.

	//Quick 을 검색하여 IK_POTION_BOX 인가.. 해서
	const int iItemNo = item->item_no;
	bool bHavePotionBox = false;
	for(int i=0; i < ITEM_QUICK_MAX; i++)
	{
		if( !ch->quick[i].item_no ){continue;}//아이템이 없으면 컨티뉴
		CItem *tt = ItemMgr.ConvertItemNoToCItem( ch->quick[i].item_no );
		if( !tt ) continue;
		if(IK_POTION_BOX == tt->GetItemKind())
		{
			bHavePotionBox = true;
			break;
		}
	}
	
	if(bHavePotionBox)
	{
		CPotionBox potion_box( ch );
		if( potion_box.PutItem( item ) ) // 포션 상자에 들어갔다.
		{
			if( SubtractMoney( price, ch ) == -1 )		// 소지금에서 가격을 빼보고
			{
				// 실패하면 포션 다시 뺏는다.
				potion_box.DeleteItem();
			}
			*item = GenerateItem( iItemNo);		// 팔고 난 후 새로운 아이테 생성 ( 이게 상점일 경우만으로 바꾸던지 해야함.. )
			return;	
		}
	}

	int ret = 0;
	if( t->GetRbutton() == DIVIDE_ITEM )	
	{
		ret = SearchItemPosByNumber( item, a, b, c, ch );
	}
	if( !ret )
	{
		if( !SearchInv( ch->inv, a, b, c ) ) return;		// 빈자리가 없다면
	}

	// 이제 실제로 돈을 빼본다. 실패하면 못 산다.
	if( SubtractMoney( price, ch ) == -1 ) return;	
	if( ch->inv[a][b][c].item_no )
	{
		ch->inv[a][b][c].attr[IATTR_MUCH] += item->attr[IATTR_MUCH];
	}
	else
	{
		ch->inv[a][b][c] = *item;		// 여기서 generater 함수를 호출하지 않는 이유는 나중에 특별한 아이템을 팔경울 대비해서..
	}

	POS pos;
	SetItemPos( INV, a, b, c, &pos );
	SendServerEachItem( &pos , &ch->inv[a][b][c], cn );		// 생성된 아이템을 꼭 찝어 보내준다.
	SendItemEventLog( &ch->inv[a][b][c], cn, SN_NOT_USER, SILT_GET_STORE, 3 ); //YGI acer

	*item = GenerateItem( iItemNo);		// 팔고 난 후 새로운 아이테 생성 ( 이게 상점일 경우만으로 바꾸던지 해야함.. )
	return;
}

void RecvItemSell( int cn, t_client_item_sell *p )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int store_id = p->store_id;
	int pos = p->item_position;

	int a = ( pos & 0x0060 ) >> 5;
	int b = ( pos & 0x0018 ) >> 3;
	int c = ( pos & 0x0007 );

	if( a<0 || a>=3 || b<0 || b>=3 || c<0 || c>=8 ) return;
	ItemAttr *item = &ch->inv[a][b][c];

	if( !item->item_no ) return;
	if( store_id > MAX_STORE_LIST) return;
	if( store_id < 0) return;

	int itempos=0;
	for( itempos=0; itempos	<	storelistmax[store_id]; itempos++)
	{
		// 안되는 경우네
		if( storelist_attr[store_id][itempos] == SIT_EVENT_ITEM ||
			storelist_attr[store_id][itempos] == SIT_NULL ||
			storelist_attr[store_id][itempos] == SIT_ONLY_SELL ) 
		{
			continue;
		}
		
		if( item->item_no == storelist[ store_id][itempos].item_no )
		{
			break;
		}
	}
	if( itempos == storelistmax[store_id] ) return;		//거래 불가 판정

	if(::GiveMoney(ch,GetItemValueSell( item )))//021126 lsw// 돈을 올려 준다.
	{//돈 받을 수 없는 상황이면 판매가 안된다
		if(item->attr[IATTR_RARE_MAIN])
		{
			const unsigned int uiRare = item->attr[IATTR_RARE_MAIN];
			Send_RareItemMakeLog(cn,item->item_no,-2,
			((LPRareMain)(&uiRare))->grade,
			((LPRareMain)(&uiRare))->soksung1,
			((LPRareMain)(&uiRare))->soksung2,
			((LPRareMain)(&uiRare))->soksung3,
			0,0,0,0,item->attr[IATTR_LIMIT],item->attr[IATTR_MUCH],0,-2);
		}
		::SendItemEventLog( item, cn, SN_NOT_USER, SILT_PUT_STORE, 3 );	 //YGI acer
		::DeleteItem( item );
	}
}


#define MAX_RUMOR_TO_READ_		5 // 읽더들일 루머의 최대수. 
char RumorText[ MAX_RUMOR_TO_READ_][ TEXT_LENGTH];
void InitRumorText( char *filename )
{
	FILE *fp;
	char  text[ TEXT_LENGTH];
	int   c,cc,i;
	
	fp = fopen( filename, "rt" );
	if( fp == NULL ) return;
	
	c = 0;	
	while( fgets( text, TEXT_LENGTH, fp ))	c++;	
	
	fseek( fp, 0, SEEK_SET );
	for( cc=0, i=0 ; i < c ; i ++ )
	{
		fgets( text, TEXT_LENGTH, fp );
		if( i >= c-5 )
		{
			strcpy( RumorText[cc], text );
			cc++;
		}
	}
	fclose(fp);
}
	
void SendRumor( t_connection c[],  int cn )
{	
	static int count;
	t_packet p;

	p.h.header.type = CMD_MENU_MESSAGE;
	{
		p.u.server_menu_message.type = MM_RUMOR_;	
		strcpy( p.u.server_menu_message.text, RumorText[count] );
	}
	
	p.h.header.size = sizeof( t_server_menu_message ) - TEXT_LENGTH + strlen( RumorText[count] );
	QueuePacket( c, cn, &p, 1 );

	count++;
	count %= MAX_RUMOR_TO_READ_;
}	
	
	
	

//------------------------------------------------------------
/*/참조...
	case MN_BLACKSMITH:			
	case MN_FARM	:	
	case MN_RESTAURANT:
	case MN_HERBSHOP:	
	case MN_SHAMBLES	:
	case MN_CANDLE	:
	case MN_SLOPSHOP	:
	case MN_ALCHEMY	:
	case MN_WOODWORKING:
	case MN_BOWCRAFT	:
	case MN_SHIPYARD	:
	

/********************************************************8
길드 Menu 
#define CMD_GUILD_INFO					7000
typedef struct server_guild_info
{

}t_server_guild_info;

void SendGuildInfo( t_connection c[], int cn, int guildindex, int storeindex, int repairindex )

  int guildindex, :    
  int storeindex, : 
  int repairindex : 

  . 스크립트에서 실행되어진다. 

*********************************************************/
void SendGuildInfo( t_connection c[], int cn  )
{
	t_packet p;

	p.h.header.type = CMD_GUILD_INFO;
	{
	}
	p.h.header.size = sizeof( t_server_guild_info );

	QueuePacket( c, cn, &p, 1 );
}


//////////////////////////////////////////////////////////////////////////////////////
//
//	서버 지원용 메소드
//
//////////////////////////////////////////////////////////////////////////////////////
#define MAX_STR_WRITER		30
#define BBS_STRING_MAX		8192
typedef struct tag_public_notice
{
	int		no;
	int		date;
	char	writer[MAX_STR_WRITER];
	char	title[TEXT_LENGTH];
	char	*text;
}PublicNotice, *lpPublicNotice;


void FreePublicNotice( lpPublicNotice p )
{
	p->no = 0;
	p->date = 0;
	strcpy(p->writer,"");
//	if( p->title )	free(p->title);
//	delete p->text;
}


PublicNotice public_notice[50];
void LoadPublicNotice( void )
{
	for( int i=0; i<50; i++ )		// 갱신을 위한 프리
	{
		if( public_notice[i].no )
		{
			FreePublicNotice( &public_notice[i] );
		}
	}

	char buf[BBS_STRING_MAX];
	
	FILE	*fp;
	char	filename[50];

	sprintf(filename, ".\\data\\공지사항.txt");

	if( fp= fopen(filename, "rt") )
	{
		int i=0; 
		while( !feof(fp) )
		{
			int len;
			fgets(buf, BBS_STRING_MAX, fp);
			if( *buf == ';' ) continue;

			char *token;
			token = strtok(buf, "\t\n\r");
			if( token == NULL ) break;
			public_notice[i].no = atoi( token );

			token = strtok(NULL, "\t\n\r");
			if( token == NULL ) break;
			public_notice[i].date = atoi( token );

			token = strtok(NULL, "\t\n\r");
			if( token == NULL ) break;
			strcpy( public_notice[i].writer, token );

			token = strtok(NULL, "\t\n\r");
			len = strlen( token );
			if( token == NULL ) break;
//			public_notice[i].title = new char[len];
			strcpy( public_notice[i].title, token );
			
//			token = strtok(NULL, "\t\n\r");
//			len = strlen( token );
//			public_notice[i].text = new char[len];
//			strcpy( public_notice[i].text, token );

			i++;
		}
	}
	else 
	{
		MessageBox(NULL, "화일 에러잖아", "에러", MB_OK );
		return;
	}

	fclose(fp);
}


void SendMessageTitle( int type, int count, t_connection c[], int cn )
{
//	char date[10][ FILENAME_MAX];
	char name[10][ FILENAME_MAX];
	char title[10][ FILENAME_MAX];
	int	 ct[10];
	TIMESTAMP_STRUCT date[10];
	
	t_packet p;
	p.h.header.type = type ;
	{
		switch( type )
		{
			case CMD_PUBLIC_NOTICE_TITLE_REQ:	{
													//PublicNotice title[10];
													int max = GetTitleBBS( count, ct, date, name, title );

													for( int i=0; i<max; i++ )
													{
//														if( !public_notice[i].no ) return;

														p.u.server_menu_message_title.no = ct[i];
														p.u.server_menu_message_title.date = date[i].year*10000 + date[i].month*100+date[i].day;
														strncpy( (char *)p.u.server_menu_message_title.name, name[i], NM_LENGTH-2 );
														strncpy( (char *)p.u.server_menu_message_title.title, title[i], NM_LENGTH-2 );
														p.h.header.size = sizeof( t_server_menu_message_title );
														QueuePacket( c, cn, &p, 1 );
													}
													break;
												}
			case CMD_RANK_TITLE_REQ			:
			case CMD_FORUM_TITLE_REQ		:		
			case CMD_EVENT_TITLE_REQ		:			
			case CMD_MYDIARY_TITLE_REQ		:
			case CMD_BBS_TITLE_REQ			:	
			case CMD_ETC_TITLE_REQ			:		;
		}
	}
}



void SendCharInfomation( int type, t_connection c[], int cn )
{
	t_packet p;
	memset( &p, 0, sizeof( t_server_user_db_data ) );
	p.h.header.type = type ;
	{
		PutPacketCharDB( c, cn, &p );
	}
	p.h.header.size = sizeof(t_server_user_db_data);

	QueuePacket( c, cn, &p, 1 );
}

void SendOtherInv( int type, int other_id, t_connection c[], int cn )		// pk 처리는? 
{
	const int GET_ITEM_MAX = 1;

	t_packet p;

	CHARLIST *ch;

	if( other_id < 10000 ) ch = CheckServerId( other_id );
	else ch = CheckNpcId( other_id-10000 );
	if( !ch ) return; 

//soto-tai	for( int i=0; i<15; i++ )	p.u.kein.server_other_ch_inv.item[i] = 0;
	for( int i=0; i<15; i++ )
	{
		memset(&p.u.kein.server_other_ch_inv.item[i],0,sizeof(ItemAttr));
	}

//soto-tai	int item[100];
	ItemAttr	item[100];memset(item,0,sizeof(ItemAttr)*100);


	int ct = 0;
	p.h.header.type = type ;
	{
		for( int a=0; a<3; a++ )
			for( int b=0; b<3; b++ )
				for( int c=0; c<8; c++ )
				{
					if( ch->inv[a][b][c].item_no )
					{
						CItem *t = ItemUnit( ch->inv[a][b][c] );
						if( !t ) continue;
						if( t->GetLevel() >= 100 ) continue;		// 레벨이 높은 아이템은 가져갈수 없게..

						// 040601 YGI
						if( GetAttr2( ch->inv[a][b][c].attr[IATTR_ATTR], IA2_ITEMMALL_ITEM ) ) 
							continue;	// 아이템 몰 아이템은 가져갈 수 없도록

//soto-tai				item[ct++] = ch->inv[a][b][c].item_no;
						item[ct++] = ch->inv[a][b][c];

						if( ct==100 ) goto ok__ ;
					}
				}
	}

	ok__ :

	int count = 0;	// 실질적으로 들어갈 물건들 카운트
	if( ct < GET_ITEM_MAX) 
	{
		for( int i=0; i<ct; i++ )
		{
			p.u.kein.server_other_ch_inv.item[i] = item[i];
			count++;
		}		
	}
	else
	{
		int item_count[GET_ITEM_MAX] = {0,};
		while( count < GET_ITEM_MAX )
		{
			int para = rand() % ct;
			int ret = 0;
			for( int i=0; i<count; i++ )
			{
				if( item_count[i] == para )
				{
					ret = 1;
					break;
				}
			}
			if( !ret ) item_count[count++] = para;
		}
		for( int i=0; i<count; i++ )
		{
			p.u.kein.server_other_ch_inv.item[i] = item[item_count[i]];
		}
	}
		p.u.kein.server_other_ch_inv.target_id = other_id;
		p.u.kein.server_other_ch_inv.item_count = count;
//soto-tai	p.h.header.size = sizeof(kein_server_other_ch_inv) - sizeof( short int )*(15-count);
	p.h.header.size = sizeof(kein_server_other_ch_inv) - sizeof( ItemAttr )*(15-count);

	QueuePacket( c, cn, &p, 1 );
}


int RecvDivideItem( kein_item_attr_pos *item, t_connection c[], int cn )
{
	CHARLIST *ch = &c[cn].chrlst;
	if( !ch ) return 0;

	POS &pos_s = item->item_pos;
	int &much = item->much;

	if( !much ) return OK;		// 값이 없을 경우는 그냥 리턴~

	ItemAttr *source = GetItemByPOS( cn, pos_s );
	if( !source ) return NOT;
	ItemAttr temp = *source;			// 지워지는  아이템 번호와 속성만 가지고 있는다.

	if( source->item_no != item->item_attr.item_no ) return NOT;
	
	if( source->attr[IATTR_MUCH] > MAX_MONEY ) source->attr[IATTR_MUCH] = MAX_MONEY;		// 나눌수 있는 아이템은 100000을 넘지 못한다.
	
	if( source->attr[IATTR_MUCH] > (DWORD) much )
	{
		source->attr[IATTR_MUCH] -= much;
	}
	else
	{
		much = source->attr[IATTR_MUCH];
		DeleteItem( source );
//		SendDeleteItem( pos_s, c, cn );		// 아이템 삭제 전송 함수 
	}

	int x, y, z;
	if( !SearchInv( ch->inv, x, y, z ) ) 
	{
		ItemAttr target = temp;
		target.attr[IATTR_MUCH] = much;
		if( target.item_no == IN_COINS && much <= 5 ) target.item_no = IN_COIN;
		else if( target.item_no == IN_COIN && much > 5 ) target.item_no = IN_COINS;

		if( target.item_no == IN_NEW_COINS && much <= 5 ) target.item_no = IN_NEW_COIN;
		else if( target.item_no == IN_NEW_COIN && much > 5 ) target.item_no = IN_NEW_COINS;

		SendItemEventLog( &target, ch->GetServerID(), ch->GetServerID(), SILT_PUT_GROUND_MAX_INVEN, 1 ); //YGI acer
		DropItem( ch->X, ch->Y, &target );			// 땅바닥으로 떨어뜨린다.
	}
	else
	{
		ItemAttr &target = ch->inv[x][y][z];
		target = temp;								// 아이템 번호와 일반 속성 넘겨 주기 위해
		target.attr[IATTR_MUCH] = much;
		if( target.item_no == IN_COINS && much <= 5 ) target.item_no = IN_COIN;
		else if( target.item_no == IN_COIN && much > 5 ) target.item_no = IN_COINS;

		if( target.item_no == IN_NEW_COINS && much <= 5 ) target.item_no = IN_NEW_COIN;
		else if( target.item_no == IN_NEW_COIN && much > 5 ) target.item_no = IN_NEW_COINS;


		POS pos;
		SetItemPos( INV, x, y, z, &pos );
		SendServerEachItem( &pos, &target, cn );
	}

	//020303 lsw
/*	if( pos_s.type == BOX )
	{
		SendLoginPutBoxItem( ch->box_id, pos_s.p3, source );
	}
*/

	//<050425_KCH (메모리 해킹)에서 Item Divide버그
	CheckItem(cn);	//050331_KCH Memory Direct Hacking에 따른 아이템 체크 강화.(아이템 나눌때 정상적인 아이템인지 먼저 확인)
	//>050425_KCH

	return 1;
}

void RecvBankInfoDeposit( DWORD dwMoney, t_connection c[], int cn )
{
	CHARLIST *ch = ::CheckServerId( cn );
	if( !ch ) 
	{
		return;
	}

	const DWORD dwBankMoney = ch->GetBankMoney();

	if(MAX_BANK_MONEY <= dwBankMoney)
	{//예금액은 40억이 최대 입니다.
		OutMessage(ch,2,930);
	}
	
	if(1==::SubtractMoney( dwMoney, ch )) //전송받은 입력 예금을 더한다.
	{
		::SendSubtractMoney( dwMoney, cn );			// 갱신된 정보를 보내준다.
		ch->IncBankMoney(dwMoney, BMCT_BANK_DEPOSIT); // CSD-030723
	}
}

void RecvBankInfoDefrayal( DWORD dwMoney, t_connection c[], int cn )
{
	CHARLIST	*ch = CheckServerId( cn );
	if( !ch ) return;

	if( ch->GetBankMoney() >= dwMoney )//은행에 돈은 있는가
	{
		if(::GiveMoneyAble(ch,dwMoney))//돈을 받을 수 있는 상황인가
		{
			ch->DecBankMoney(dwMoney, BMCT_BANK_DEFRAYAL); // CSD-030723
			GiveMoney(ch,dwMoney);
		}
	}
}


void SendBankRepayment( t_connection c[], int cn )
{
	CHARLIST	*ch = CheckServerId( cn );
	if( !ch ) return;
	t_packet p;

	p.h.header.type = REP_YOUR_DEPOSIT_INFO;
	{
		p.u.kein.client_bank_repayment.loan_money;
		p.u.kein.client_bank_repayment.last_loan;
		p.u.kein.client_bank_repayment.last_loan_time;
	}
	p.h.header.size = sizeof( k_client_bank_deposit );

	QueuePacket( c, cn, &p, 1 );
}

void RecvCharInfoBasic( t_char_info_basic *p, t_connection c[], int cn )
{
	if( p->kind < 0 ) return;
	if( p->password != 111 ) return; 

	CHARLIST *ch = &c[cn].chrlst;
	if( !ch ) return;

	if( strcmp( g_szPassWordName, ch->Name ) ) return;
	DWORD d = p->data;

	switch (p->kind)
	{	//< CSD-030723
	case STR:
		{
			ch->Str	= short(d);
			break;
		}
    case DEX:
		{
			ch->Dex	= short(d);
			break;
		}
	case CON:
		{
			ch->Con	= short(d);
			break;
		}
	case WIS:
		{
			ch->Wis	= short(d);
			break;
		}
	case INT_:
		{
			ch->Int	= short(d);
			break;
		}
	case MOVP:
		{
			ch->MoveP = short(d);
			break;
		}
	case CHA:
		{
			ch->Char = short(d);
			break;
		}
	case ENDU:
		{
			ch->Endu = short(d);
			break;
		}
	case MOR:
		{
			ch->Moral = short(d);
			break;
		}
	case LUCK:
		{
			ch->Luck = short(d);
			break;
		}
	case WSPS:
		{
			ch->wsps = short(d);
			break;
		}
	case LVPOINT:
		{
			ch->SetReservedPoint(d);
			break;
		}
	case MONEY:
		{
			ch->Money = d;
			break;
		}
	case HP:
		{
			ch->Hp = d;
			break;
		}
	case MAX_HP:
		{
			ch->HpMax = d;
			break;
		}
	case MP:
		{
			ch->Mana = d;
			break;
		}
	case MAX_MP:
		{
			ch->ManaMax	= d;
			break;
		}
	case SP:
        {
			ch->Hungry = d;
			break;	
		}
	case MAX_SP:
		{
			ch->HungryMax = d;
			break;
		}
	case BANKMONEY: 
		{
			ch->SetBankMoney(d, BMCT_RECV_CHAR_INFO_BASIC);
			break;
		}
	case TAC_E1:
	case TAC_E2:
	case TAC_E3:
	case TAC_E4:
	case TAC_E5:
	case TAC_E6:
	case TAC_E7:
	case TAC_E8:
	case TAC_E9:
	case TAC_E10:
	case TAC_E11:
	case TAC_E12:
	case TAC_E13:	  
		{
			ch->ConvertTacticsLevel(p->kind- TAC_E1);
			break;
		}
	case CHANGE_GENDER:	//050413_KCH 캐릭 성전환
		{
			ch->SprNo	= d;
			ch->Gender	= d;
		}
		break;
	}	//> CSD-030723
}

void SendStoreItemList( int cn, int storelistindex, int menu )
{
	t_packet p;
	int count = 0;
	p.h.header.type = CMD_STORE_ITEMLIST;
	{
		p.u.server_store_itemlist.no				= storelistmax[ storelistindex];
		p.u.server_store_itemlist.smallmenu			= menu;
		p.u.server_store_itemlist.storelistindex	= storelistindex;
	
		for( int i = 0 ; i < storelistmax[ storelistindex] ; i ++)
		{
			if( storelist_attr[ storelistindex][ i] == SIT_EVENT_ITEM || 				
				storelist_attr[ storelistindex][ i] == SIT_NULL	) continue;

			switch( menu )
			{
				case MN_SKILLGUILD_REPAIR	:	break;
				case MN_SKILLGUILD_SELL		:	
				case MN_SHOP_SELL			:	if( storelist_attr[ storelistindex][ i] == SIT_ONLY_SELL ) continue;
													break;
				case MN_SKILLGUILD_BUY		:	
				case MN_SHOP_BUY			:	if( storelist_attr[ storelistindex][ i] == SIT_ONLY_BUY ) continue; 
												break;
				default : break;
			}
			p.u.server_store_itemlist.store_item[count++].item	= storelist[ storelistindex][ i].item_no;
		}
	}
	p.h.header.size = sizeof( t_server_store_itemlist ) - sizeof( k_store_list_item )*(MAX_STORE_ITEM_LIST-count) ;
	QueuePacket(connections, cn, &p, 1);
}

void SendCallSmallMenu( int cn, int storelistindex, int menu )
{
	t_packet p;
	p.h.header.type = CMD_CALL_SMALLMENU;
	{
		p.u.server_call_smallmenu.menu = menu;
		p.u.server_call_smallmenu.storelistindex = storelistindex;
	}
	p.h.header.size = sizeof( t_server_call_smallmenu );
	QueuePacket(connections, cn, &p, 1);
}
//<soto-030421
void SendCallDualMagicLearnMenu( int cn)
{
	t_packet p;
	p.h.header.type = CMD_DUAL_MAGIC_LEARN_BOOK;
	p.h.header.size = 0;
	
	QueuePacket(connections, cn, &p, 1);		
}
//>soto-030421

//<soto-030429
void RecvCallLuckyMenuOpen(int cn)
{
	SendCallLuckyMenuOpen(cn);
}
void	SendCallLuckyMenuOpen(int cn)
{
	t_packet p;
	p.h.header.type = CMD_OPEN_LUCKY_MENU;
	p.h.header.size = 0;
	
	QueuePacket(connections, cn, &p, 1);
}



int LoadGuildItem( char *filename, int num )
{
	FILE *fp;
	fp = fopen( filename, "rt" );
	if( fp == NULL ) return 0;
	
	char buf[MAX_STRING];
	int i=0; 

	while( !feof(fp) )
	{
		fgets(buf, BBS_STRING_MAX, fp);
		if( *buf == ';' ) continue;

		char *token;

//		token = strtok(buf, "\t\n\r");		// number skip
//		if( token == NULL ) break;

		token = strtok(buf, "\t\n\r");
//		token = strtok(NULL, "\t\n\r");		// 아이템 번호
		if( token == NULL ) break;
		storelist[num][i].item_no = atoi( token )/100;
		storelist[num][i] = GenerateItem( storelist[num][i].item_no );		// 상점은 디폴트 아이템이 만들어 진다.

		token = strtok(NULL, "\t\n\r");		// 아이템 이름		skip
		if( token == NULL ) break;

		token = strtok(NULL, "\t\n\r");		// 팔기 사기 속성
		if( token == NULL ) break;
		storelist_attr[num][i] = atoi( token );

		i++;		

		// 010522 KHS
		if( i >= MAX_STORE_ITEM_LIST ) 
		{
			MyLog( LOG_NORMAL, "**** Error :  %s ItemList Number is more than 'MAX_STORE_ITEM_LIST' ", filename );
			break;
		}
	}

	fclose( fp );

	SortingItem( storelist[num], storelist_attr[num], i );

	return i;
}

void swap_kein( void *a, void *b, void *t, int size )
{
	memcpy( t, a, size );
	memcpy( a, b, size );
	memcpy( b, t, size );
}

void SortingItem( ItemAttr *item_list, int *list_int, int &max )
{
	ItemAttr temp;	
	for( int i=0; i<max-1; i++ )
	{
		CItem *item_i = ItemUnit( item_list[i] );
		if( !item_i ) 
		{
			item_list[i] = item_list[max-1];
			list_int[i] = list_int[max-1];
			i=-1;
			max--;
			continue;
		}

		for( int j=i; j<max; j++ )
		{
			int type_i = item_list[i].item_no / 1000;
			int type_j = item_list[j].item_no / 1000;

			CItem *item_i = ItemUnit( item_list[i] );

			if( type_i > type_j ) 
			{
				swap_kein( &item_list[i], &item_list[j], &temp, sizeof( ItemAttr ) );
				swap( &list_int[i], &list_int[j] );
				continue;
			}

			if( type_i == type_j )
			{
				CItem *item_j = ItemUnit( item_list[j] );
				if( !item_j ) continue;
				
				//if( item_i->GetWearAble() > item_j->GetWearAble() )
				if( item_i->GetSkill_Ability() > item_j->GetSkill_Ability() )
				{
					swap_kein( &item_list[i], &item_list[j], &temp, sizeof( ItemAttr ) );
					swap( &list_int[i], &list_int[j] );
					continue;					
				}

				//if( item_i->GetWearAble() == item_j->GetWearAble() )
				if( item_i->GetSkill_Ability() == item_j->GetSkill_Ability() )
				{
					if( item_i->GetSkill_Ability() )
					{
						if( item_i->GetLevel() > item_j->GetLevel() )
						{
							swap_kein( &item_list[i], &item_list[j], &temp, sizeof( ItemAttr ) );
							swap( &list_int[i], &list_int[j] );
							continue;					
						}

						if( item_i->GetLevel() == item_j->GetLevel() )
						{
	//						if( list[i].item_no > list[j].item_no )
	//						{
	//							swap_kein( &list[i], &list[j], &temp, sizeof( ItemAttr ) );
	//							swap( &list_int[i], &list_int[j] );
	//							continue;					
	//						}
						}
					} 
				}
			}
		}
	}
}


void SendDecreaseItem( POS item_pos, DWORD dur, t_connection c[], int cn )	// 아이템 감소 내구력 전송 함수
{
	t_packet p;

	p.h.header.type = CMD_DECREASE_ITEM;
	{
		p.u.kein.server_decrease_item.dur = dur;
		p.u.kein.server_decrease_item.item_pos = item_pos;
	}
	p.h.header.size = sizeof( k_server_decrease_item ); 
	QueuePacket(connections, cn, &p, 1);
}
void RecvLvUpPointEach(unsigned char type, t_connection c[], int cn)
{
	t_packet	packet;
	CHARLIST	*ch;
	ch = &c[cn].chrlst;
	if( !ch ) return;

	if (ch->GetReservedPoint() > 0)
	{
    ch->IncAbility(type);
    ResetAbility(ch);//020303 lsw
	}
	else 
	{	
		packet.h.header.type = CMD_LEVELUP_POINT_NO;
		packet.h.header.size = 0;
		QueuePacket(connections, cn, &packet, 1);
	}
}

void RecvLvDownPointEach( unsigned char type, t_connection c[], int cn )
{
	return;
	t_packet	packet;
	CHARLIST	*ch;
	ch = &c[cn].chrlst;

	if( !ch ) return;

	if (ch->GetReservedPoint() > 0)
	{
		if (ch->GetReservedPoint() >= 10)		// 남아있는 레벨업포인트가 10이상이면 빼기는 안된다.
		{
			return;
		}
		switch( type )
		{
			case STR:		ch->Str--;		break;
			case CON:		ch->Con--;		break;
			case DEX:		ch->Dex--;		break;
			case WIS:		ch->Wis--;		break;
			case INT_:		ch->Int--;		break;
			case MOVP:		ch->MoveP--;	break;
			case CHA:		ch->Char--;		break;
			case ENDU:		ch->Endu--;		break;
			case MOR:		ch->Moral--;	break;
			case LUCK:		ch->Luck--;		break;
			case WSPS:		ch->wsps--;		break;
			default : return;
		}

		ch->IncReservedPoint();
		ResetAbility(ch);//020303 lsw

		packet.h.header.type = CMD_LEVELDOWN_OK;
		{
			packet.u.kein.client_lvup_point_each.type = type;
		}
		packet.h.header.size = sizeof( client_lvup_point_each );
		QueuePacket(connections, cn, &packet, 1);
	}
	else 
	{	
		packet.h.header.type = CMD_LEVELUP_POINT_NO;
		packet.h.header.size = 0;
		QueuePacket(connections, cn, &packet, 1);
	}	
}

void RecvLearnItem( k_client_learn_item *i, t_connection c[], int cn )
{
	CHARLIST *ch = &c[cn].chrlst;
	if( !ch ) return;

	int type, num;
	getItemIndex( i->item_no, type, num );
	CItem *t = ItemUnit( type, num );
	if( !t ) return;

	int join_num = GetItemJoinNumber( i->item_no );		// join테이블에서의 파라메타 번호
	if( !item_join[join_num].item_id ) return;					// 없는 아이템
	int value = item_join[join_num].tech_price;
	
//	ch->Money = GetMoneyByItem( ch );
	if( ch->Money < (DWORD)value ) return;				// 돈이 부족함.

	int ct = SkillMgr.FindLearnItem( ch, i->item_no ) ;
	if( ct == -1 ) return ;					// 배운아이템일 경우 0을 리턴한다.
	if( ct == MAX_LEARN_ITEM ) return;	// 모두 다 배웠을 때...

	if( !SkillMgr.CanLearnItem( ch, join_num ) ) return;		// 배울 능력이 부족하다.

	ch->Item[ct] = i->item_no;			// 배운거 등록
	ch->ItemMax++;						// 만들수 있는 아이템 최대치 ++
	SubtractMoney( value, ch );

	t_packet p;
	p.h.header.type = CMD_LEARN_ITEM;		// ok sign
	{
		p.u.kein.item_no.item_no = i->item_no;
	}
	p.h.header.size = sizeof( k_item_no );
	QueuePacket(c, cn, &p, 1);
}

void RecvItemRepair( t_client_item_sell *p, t_connection c[], int cn )
{
	CHARLIST *ch = &c[cn].chrlst;
	if( !ch ) return;
//	ch->Money = GetMoneyByItem( ch );

	int store_id = p->store_id;
	int pos = p->item_position;
	int x,y,z;
	int item_no, type;
	int price;
	int storeitemno;
	int i;

	x = ( pos & 0x0060 ) >> 5;
	y = ( pos & 0x0018 ) >> 3;
	z = ( pos & 0x0007 );
	ItemAttr &item= ch->inv[x][y][z];

	getItemIndex( item.item_no, type, item_no );
	CItem *t = ItemUnit( type, item_no );
	if( !t ) 
	{
		SendServerResult( CM_REPAIR, 3, cn );	// 아이템이 범위를 넘어섬...
		return;
	}

	storeitemno = MAX_STORE_ITEM_LIST;
	for( i = 0 ; i < storeitemno ; i ++)
	{
		if( storelist[store_id][i].item_no == item.item_no ) 
		{
			break;		// 정말로 그 상점에서 취급하는 물품인가
		}
	}

	if( i == storeitemno || !t->GetRepairAble() )
	{
		SendServerResult( CM_REPAIR, 2, cn );	// 취급 물품이 아님
		return;
	}

	WORD d_curr;
	WORD d_max;
	GetItemDuration( item, d_curr, d_max );

	if(!d_max) 
	{
		SendServerResult( CM_REPAIR, 4, cn );		// 내구력이 하나도 없당...
		return;
	}
	price = (int)( (float)t->GetValue()/d_max * ((d_max - d_curr))*DUR_RATIO +.5); // 가격 계산 및 판단 
	if(item.attr[IATTR_RARE_MAIN])// 레어는 수리비 비쌈
	{
		const int iRepairPoint = (int)( (float)((d_max - d_curr))*RARE_DUR_RATIO +.5); // 가격 계산 및 판단 
		const int iRareGrade  = ((LPRareMain)(&item.attr[IATTR_RARE_MAIN]))->grade;
		price += (iRepairPoint/2) * ((iRareGrade*iRareGrade)+10) * 100; // 가격 계산 및 판단 //020815 lsw
	}

	if( !price ) 
	{
		price = 1;
	}
	if(ch->Money < (DWORD) price ) 
	{
		SendServerResult( CM_REPAIR, 1, cn );		// 돈이 없음
		return;
	}
	SubtractMoney( price, ch );

	if(item.attr[IATTR_RARE_MAIN])// 레어는 수리율이 다르다
	{
		d_max = (WORD)(d_curr+(d_max-d_curr)*RARE_DUR_RATIO);
	}
	else
	{
		d_max = (WORD)(d_curr+(d_max-d_curr)*DUR_RATIO);
	}
	
	DWORD d = MAKELONG(d_max, d_max);
	item.attr[IATTR_DURATION] = d;
	{
		t_packet packet;
		packet.h.header.type = CMD_ITEMREPAIR_OK;
		{
			packet.u.kein.server_item_repair_ok.item_pos=pos ;
			packet.u.kein.server_item_repair_ok.money= price;
			packet.u.kein.server_item_repair_ok.dur= d;			
		}
		packet.h.header.size = sizeof(k_server_item_repair_ok);
		QueuePacket(connections, cn, &packet, 1);
	}
}

void SendServerResult( BYTE type, BYTE result, int cn )		// 어떤 결과를 보내줄때..
{
	t_packet packet;
	packet.h.header.type = CMD_RESULT;
	{
		packet.u.kein.server_result.type= type;
		packet.u.kein.server_result.result= result;
	}
	packet.h.header.size = sizeof(k_server_result);
	QueuePacket(connections, cn, &packet, 1);
}

void GetItemDuration( ItemAttr &rItem, WORD &d_curr, WORD &d_max )
{
	d_curr = LOWORD( rItem.attr[IATTR_DURATION] );
	d_max = HIWORD( rItem.attr[IATTR_DURATION] );
}

void SendStatusOpen( t_connection c[], int cn )
{ //< CSD-020821
	CHARLIST *ch = &c[cn].chrlst;
	if (!ch) return;

	t_packet packet;
	packet.h.header.type = CMD_OPEN_STATUS_REQ;
	{
		packet.u.kein.server_status_open.killmon	= ch->killmon;
		packet.u.kein.server_status_open.ac			= ch->GetExtendAc();
		packet.u.kein.server_status_open.fame			= ch->fame;
		packet.u.kein.server_status_open.mov = ch->MoveP;
	}
	packet.h.header.size = sizeof(k_server_status_open);
	QueuePacket(connections, cn, &packet, 1);
} //> CSD-020821

void SendBankInventory( int count, t_connection c[], int cn )		//1225
{
	int i,j;
	
	i=count /3;
	j=count %3;

	t_packet packet;
	packet.h.header.type = CMD_SEND_BANK_INV;
	packet.h.header.size = sizeof(k_server_bank_item);

	packet.u.kein.server_bank_item.first = i;
	packet.u.kein.server_bank_item.second = j;
	memcpy( (unsigned char *)packet.u.kein.server_bank_item.inv, 
			(unsigned char *)c[ cn].chrlst.bank[i][j],	sizeof( ItemAttr ) * 6 );

	QueuePacket( c, cn, &packet, 1);
}


int IsSameCharOfParty( int type, char *name, CHARLIST *ch )		// 같은 이름이 있는지 판단
{
	if( !ch ) return 0;
	int i;
	switch( type )
	{
		case PARTY :		for( i=0; i<6; i++ ) if( !strcmp( ch->party[i].Name, name ) ) return 1;
							break;
		case RELATION :		for( i=0; i<3; i++ ) if( !strcmp( ch->relation[i].Name, name ) ) return 1;
							break;
		case EMPLOYMENT :	for( i=0; i<6; i++ ) if( !strcmp( ch->employment[i].Name, name ) ) return 1;
							break;
	}
	return 0;
}

// 관계를 맺기에 가능한 레벨인지..
int IsCheckPartyLevel( int type, CHARLIST *target_ch, CHARLIST *ch )		
{
	if( !ch ) return 0;
	switch( type )
	{
		case PARTY:
		case RELATION:		
		case EMPLOYMENT:	return 1;
							
	}
	return 0;
}

void RecvDoParty( k_client_do_party *p, t_connection c[], int cn, int party_cn, int check_nation )
{
	CHARLIST *ch = &c[cn].chrlst;
	if( !ch ) return;

	if( !party_cn )
		party_cn = ExistHe( p->name );		// cn 값이 있다면...

	if( party_cn == -1 ) return;
	
	CHARLIST *party_ch = &c[party_cn].chrlst;
	int ct = p->ct;

	if( IsSameCharOfParty( p->type, p->name, ch ) ) return;
	if( !IsCheckPartyLevel( p->type, party_ch, ch ) ) 
	{
		t_packet packet;
		packet.h.header.type = CMD_PARTY_NOT_BY_LEVEL;
			packet.u.kein.default_short_int = p->type;
		packet.h.header.size = sizeof(short int);
		QueuePacket( c, ch->GetServerID(), &packet, 1);
		return;
	}
	//<! BBD 040308		중립맵에서는 나라가 달라도 허용한다
	if( check_nation )
	{
		if(MapInfo[MapNumber].nation)
		{
			if( !IsSameNation( party_ch, ch ) ) return;		// 나라가 달라도 파티를 맺어야 할 경우
		}
	}
	//> BBD 040308		중립맵에서는 나라가 달라도 허용한다

	switch( p->type )				// 삽입할 곳에 다른 파티원이 있으면 그 파티원을 옮기고 삽입한다.
	{
		case PARTY :		if( ch->party[ct].On ) 
							{
								for( int i=0; i<6; i++ )
								{
									if( !ch->party[i].On ) 
									{
										ch->party[i] = ch->party[ct];
										ch->party[i].On = true;
										break;
									}
								}
							}
							strcpy( ch->party[ct].Name, p->name ); 
							ch->party[ct].On = true;
							break;

		case RELATION :		if( *ch->relation[ct].Name ) 
							{
								for( int i=0; i<3; i++ )
								{
									if( !*ch->relation[i].Name ) 
									{
										strcpy( ch->relation[i].Name, ch->relation[ct].Name );
										break;
									}
								}
							}
							strcpy( ch->relation[ct].Name, p->name ); break;
		case EMPLOYMENT :	if( *ch->employment[ct].Name ) 
							{
								for( int i=0; i<6; i++ )
								{
									if( !*ch->employment[i].Name ) 
									{
										strcpy( ch->employment[i].Name, ch->employment[ct].Name );
										break;
									}
								}
							}
							strcpy( ch->employment[ct].Name, p->name ); break;
	}
	// 그 캐릭터의 정보 보내주기
	int ret = SendPartyInfo( p->type, p->ct, c, ch->Name );
	if( ret == -1 )
	{
		switch( p->type )
		{
			case PARTY :		memset( &ch->party[p->ct], 0 , sizeof( CharacterParty ) ); break;
//			case RELATION :		
//			case EMPLOYMENT :		
		}

	}	
	else		// 상대편이 파티 맺었음을 알려준다.
	{
		t_packet packet;
		packet.h.header.type = CMD_DO_PARTY_OK;
			packet.u.kein.do_party_ok.type= p->type;
			strcpy( packet.u.kein.do_party_ok.name, ch->Name );
		packet.h.header.size = sizeof( k_do_party_ok );
		QueuePacket( c, party_ch->GetServerID(), &packet, 1);
	}
}

inline CharacterParty *GetPartyPointer( CHARLIST *ch, int type, int count )
{
	CharacterParty *target_party;
	switch( type )
	{
		case PARTY :		target_party = &ch->party[count]; break;
		case RELATION :		target_party = &ch->relation[count]; break;
		case EMPLOYMENT :	target_party = &ch->employment[count]; break;
		default : return NULL;
	}
	return target_party;
}

int SendPartyInfo( int type, int ct, t_connection c[], const char* szName )	// 파티 종류, 파티 배열 번호		1부분만 전송
{
	CHARLIST *me = CheckServerName(szName);
	if( !me ) return 0;
	
	const int cn = me->GetServerID();	// kyo
	
	if( ct<0 && ct>=6 ) return 0;
	
	CharacterParty *target_party = GetPartyPointer( me, type, ct );
	if( !target_party ||  !target_party->Name[0] ) return 0; // 그 번호에 맺은 파티원이 없다.
	CHARLIST *pa = SearchIdByName(target_party->Name); // CSD-030320
	if( !pa ) return -1; 
	
	t_packet packet;
	memset( &packet, 0, sizeof( t_packet ) );
	//< CSD-030320	
	const int he_exist = ExistHe(pa->Name);
	const int sight = IsMySightInWhoById( he_exist, cn );
	//> CSD-030320
	packet.h.header.type = CMD_SERVER_PARTY;
	{
		packet.u.kein.party_info.type		= (short) type;
		packet.u.kein.party_info.index		= (char) ct;
		
		strcpy( packet.u.kein.party_info.m_Name, target_party->Name);
		packet.u.kein.party_info.m_Gender	= (char) pa->Gender;
		packet.u.kein.party_info.m_ImageNo	= (short) pa->Face;
		packet.u.kein.party_info.m_Class	= (char) pa->Class;
		if( sight && CompPartyBoth( me, pa ) )		// 내시야에 들어왔을 때만 레벨과 힘 정보를 보내준다. 같이 동맹을 맺을 때만..
		{
			packet.u.kein.party_info.m_Lv = pa->GetLevel(); // CSD-030806
			packet.u.kein.party_info.m_Str = pa->Str;
		}
		packet.u.kein.party_info.m_Join		= he_exist?1:0;
	}
	packet.h.header.size = sizeof(k_party_info);
	QueuePacket(connections, cn, &packet, 1);
	
	switch( type )
	{
	case PARTY : {
		for( int j=0; j<6; j++ )
		{
			if( !strcmp( me->Name, pa->party[j].Name ) ) 
			{
				SendPartyJoinOk( target_party->Name, cn );		// 먼저 메시지를 보낸 놈에게 ok를 보내주고
				SendPartyJoinOk( me->Name, pa->GetServerID());	// 상대편도 겜에 접속해 있으면 ok를 보내준다.
				//pa->party[j].Server_id = cn;
				break;
			}
		}
		break;
				 }
		
	}
	return 1;
}

void RecvDeleteParty( k_client_delete_party *p, t_connection c[], int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	CharacterParty *target;
	switch( p->type )
	{
		case PARTY : target = &ch->party[p->ct]; break;
		case RELATION :	target = &ch->relation[p->ct]; break;
		case EMPLOYMENT : target = &ch->employment[p->ct]; break;
		default : return;
	}
	//CHARLIST *party_ch = CheckServerId( target->Server_id );
	CHARLIST *party_ch = CheckServerName( target->Name );

	if( party_ch && strcmp( party_ch->Name, target->Name )==0 )			// 그 놈이 게임상에 존재하는 놈이라면..
	{
		if( IsSameCharOfParty( PARTY, ch->Name, party_ch ) )		// 그놈한테도 
		{
			//SendPacketDefault( CMD_PARTY_JOIN_RELEASE, ch->Name, strlen( ch->Name ), target->Server_id ); 
			CHARLIST *chTar = CheckServerName( target->Name );
			if( !chTar ) return;
			SendPacketDefault( CMD_PARTY_JOIN_RELEASE, ch->Name, strlen( ch->Name ), chTar->GetServerID()); // kyo
		}
	}
	memset( target, 0, sizeof( CharacterParty ));
}

void RecvLearnMagic( short int num, t_connection c[], int cn )
{
	CHARLIST	*ch = &c[cn].chrlst;
	if( !ch ) return;

	if( ch->Spell == WIZARD_SPELL )
	{
		if( ch->Ws[num] )	
		{
			SendServerResult( CM_LEARN_MAGIC, 2, cn );	// 이미 배웠다.
			return;
		}
	}
	else if( ch->Spell == PRIEST_SPELL )
	{
		if( ch->Ps[num] )	
		{
			SendServerResult( CM_LEARN_MAGIC, 2, cn );	// 이미 배웠다.
			return;
		}
	}

//	SendServerResult( CM_LEARN_MAGIC, 5, cn );	// 돈이 부족해
	int ret = CanLearnMagic( num, ch );

	switch( ret )
	{
		case 1  :	SendLearnMagicOk( num, cn );	break;
		case 3  :	SendServerResult( CM_LEARN_MAGIC, 3, cn );	return ;// 능력이 부족해
		case 4  :	SendServerResult( CM_LEARN_MAGIC, 4, cn );	return ;// 그런 마법 없어
		default :	return;
	}

	if( ch->Spell == WIZARD_SPELL )
	{
		SubtractMoney( Magic_Ref[num].Price, ch );
		ch->Ws[num] = true;
	}
	else ch->Ps[num] = true;
}

int	CanLearnMagic( int num, CHARLIST *ch )		// 그 마법을 배울수 있나?
{
	if( !ch ) return false;
//	return true;			// 임시
	int sptype=0;
	int sp;
	unsigned char *spell;
	if( ch->Spell )			// priest
	{
		sptype = 150;
		spell = ch->Ws;
		sp = TACTICS_Orison;
	}
	else 
	{
		spell = ch->Ps;
		sp = TACTICS_Magery;
	}

	int magic_num = num+sptype;
	if( magic_num == -1 ) return 4;
  int Wis  = ch->GetAbility(WIS);
	int Int  = ch->GetAbility(INT_);
	int wsps = ch->GetAbility(WSPS);
	if( Magic_Ref[magic_num].basic_magery> ch->Skill[sp]+1 )	return 3;			// 마법기술이 충분한가?
	if( Magic_Ref[magic_num].require_WP > Wis ) return 3;		// 요구 지혜
	if( Magic_Ref[magic_num].require_IT > Int ) return 3;		// 요구 지능
	if( Magic_Ref[magic_num].exhaust_MS > wsps ) return 3;		// 요구 WSPS
	if( Magic_Ref[magic_num].point_WS > wsps ) return 3;		// 요구 WSPS
	if( (DWORD) Magic_Ref[magic_num].Price > ch->Money ) return 3;		// 요구 WSPS


	return 1;	
}

int GetMagicNumBySerial( int serial )
{
	for( int i=0; i<Num_Of_Magic; i++ )
	{
		if( Magic_Ref[i].serial_Num == serial ) return i;
	}
	
	return -1;
}

void SendLearnMagicOk( int num , int cn)
{
	t_packet packet;
	packet.h.header.type = CMD_LEARN_MAGIC_OK;
	{
		packet.u.kein.learn_magic_ok.magic_num = num;
	}
	packet.h.header.size = sizeof(k_learn_magic_ok);
	QueuePacket(connections, cn, &packet, 1);
}

//
//	질병 예방, 성공여부 클라이언트 전송
//
void SendPreventingOk( int num , int cn)
{
	t_packet packet;
	packet.h.header.type = CMD_PREVENTING;
	{
		packet.u.kein.client_disease.disease_num = num;
	}
	packet.h.header.size = sizeof(k_client_disease);
	QueuePacket(connections, cn, &packet, 1);
}

void RecvPreventing( short int num, t_connection c[], int cn )
{
	CHARLIST *ch = &c[cn].chrlst;
	if( !ch ) return;
//	ch->Money = GetMoneyByItem( ch );

	int money=100;
	money = disease_tbl[num].price_prevent;	// 예방비

	if( num < 0 || num > 5 ) return;
	if( ch->Money < (DWORD)money ) 
	{
		SendServerResult( CM_PREVENTING, 2, cn );			// 돈이 부족해...
		ch->SendCharInfoBasic(MONEY, ch->Money );			// 갱신된 정보를 보내준다.//020704 lsw
		return;
	}
	
// 010531 KHS
//	ch->nut1 = 100;
//	ch->nut2 = 100;
//	ch->nut3 = 100;
	
	SubtractMoney( money, ch );

	SendPreventingOk( num, cn );	// 성공했음.
}

//
//	질병 치료
//
void SendCureDiseaseOk( short int num, int cn )
{
	t_packet packet;
	packet.h.header.type = CMD_CURE_DISEASE;
	{
		packet.u.kein.client_disease.disease_num = num;
	}
	packet.h.header.size = sizeof(k_client_disease);
	QueuePacket(connections, cn, &packet, 1);
}

void RecvCureDisease( short int num, t_connection c[], int cn )
{
	CHARLIST *ch = &c[cn].chrlst;
	if( !ch ) return;
//	ch->Money = GetMoneyByItem( ch );

	int money=100;

	if( num < 0 || num > 5 ) return;
	money = disease_tbl[num].price_heal;
	if( ch->Money < (DWORD)money ) 
	{
		SendServerResult( CM_CURE_DISEASE, 2, cn );
		return;
	}
	SubtractMoney( money, ch );
	SendCureDiseaseOk( num, cn );
}

//
// 아이템 개당 가격 전송 메소드들 		
//
void SendItemValue( int item_no, int cn )
{
	int type, num;
	getItemIndex( item_no, type, num );

	CItem *item = ItemUnit( type, num );
	if( !item ) return;

	t_packet packet;
	packet.h.header.type = CMD_REQ_ITEM_VALUE;
	{
		packet.u.kein.server_item_money.item_no = item_no ;
		packet.u.kein.server_item_money.money = item->GetValue();
	}
	packet.h.header.size = sizeof(k_server_item_money);
	QueuePacket(connections, cn, &packet, 1);
}

void SendItemValue_Sell( POS *pos, int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	ItemAttr *item = GetItemByPOS( cn, *pos );
	if( !item ) return ;

	DWORD price_sell = GetItemValueSell( item );
	if( price_sell == 0 ) 
		SendDeleteItem( item, pos, ch );

	t_packet packet;
	packet.h.header.type = CMD_REQ_ITEM_VALUE_SELL;
	{
		packet.u.kein.server_item_money.item_no = item->item_no ;
		packet.u.kein.server_item_money.money = price_sell;
	}
	packet.h.header.size = sizeof(k_server_item_money);
	QueuePacket(connections, cn, &packet, 1);
}

void SendItemValue_Repair( int item_no, int cn )
{
	int type, num;
	getItemIndex( item_no, type, num );

	CItem *item = ItemUnit( type, num );
	if( !item ) return;

	t_packet packet;
	packet.h.header.type = CMD_REQ_ITEM_VALUE_REPAIR;
	{
		packet.u.kein.server_item_money.item_no = item_no ;
//		packet.u.kein.server_item_money.money = ;
	}
	packet.h.header.size = sizeof(k_server_item_money);
	QueuePacket(connections, cn, &packet, 1);
}

CHARLIST *SearchIdByName(const char* pName)
{	//< CSD-030320
	return g_pUserManager->GetCharList(pName);
}	//> CSD-030320


void RecvClientInn( char type, int cn )
{
	CHARLIST	*ch = CheckServerId( cn );
	if( !ch ) return;
//	ch->Money = GetMoneyByItem( ch );

	switch( type )
	{
		case 0: return;  // 에러..
		
		// 쉴때..
		case INN_SINGLE :	if( ch->Money < 3 ) 
							{
								SendServerResult( CM_INN_REST, 2, cn );		 // 돈이 부족합니다.
								return;
							}
							ch->Status = CS_REST;	
							SubtractMoney( 3, ch );
							break;
							
		case INN_DOUBLE :	ch->Status = CS_REST;	break;
		case INN_MULTI  :	ch->Status = CS_REST;	break;

		// 잠자기
		case INN_SINGLE+1 :	ch->Status = CS_SLEEP;	break;
		case INN_DOUBLE+1 :	ch->Status = CS_SLEEP;	break;
		case INN_MULTI+1  :	ch->Status = CS_SLEEP;	break;
	}
}

int GetItemValueSell( ItemAttr *item )//020925 lsw float 로 바꾸어 나누기 하게끔 했습니다.
{
	int type;
	int num; 
	getItemIndex( item->item_no, type, num );

	CItem *t = ItemUnit( type , num );
	if( t ) 
	{
		int price = 0;
		int base = t->GetValue();
		int much;

		if( type == DISPOSABLE ) 
		{
			much = t->GetItemQuantity();	// dispoable일 경우
			if( !much ) much = 1;
			price = (int)(((float)base * .9+.5)*(((float)item->attr[IATTR_MUCH])/((float)much)));
			return price;
		}
		else if( t->GetRbutton()==DIVIDE_ITEM )		// 수량을 의미...
		{
			much = t->GetItemDuration();		// 10개 단위
			if( !much ) much = 1;
			price = (int)(((float)base * .9+.5)*(((float)item->attr[IATTR_MUCH])/((float)much)));
			return price;
		}

		WORD d_curr, d_max;

		GetItemDuration( *item, d_curr, d_max );
		WORD o_d_max = t->GetDuration();
		if( d_max < o_d_max ) d_max = o_d_max;

		if( d_max )
		{	
			if( (item->attr[IATTR_LIMIT]-g_curr_time) > 0 )		// 유통기한이 남아 있어야 한다.
			{
				price = (int)( (float)(base*.9)*(float)d_curr/(float)d_max+.5 );
				return price;
			}
			// 유통기한을 넣야 한다.
			price = (int)( (float)(base*.9)*(float)d_curr/(float)d_max+.5 );		// 임시 
			return price;
		}
	}
	return 0;
}


ItemAttr *GetItemByPOS( int cn, POS item )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return NULL;
	return GetItemByPOS( ch, item );
}

ItemAttr *GetItemByPOS( CHARLIST *ch, POS item )
{//021030 lsw
	switch( item.type )
	{
		case	HAND	:	return &ch->handheld;
		case	INV		:	return &ch->inv[item.p1][item.p2][item.p3];
		case	EQUIP	:	return &ch->equip[item.p3];
		case	QUICK	:	return &ch->quick[item.p3];
		case	BANK	:	return &ch->bank[item.p1][item.p2][item.p3];
		case	BOX		:
		case	MAKE	:	
		case	AUCTION_DB :
		case	OTHER_CH:	break;
	}
	return NULL;
}


extern void SendCMD_REQUEST_CHECK_WARNO(const int cn);	// 010915 LTS//020903 lsw

void SendServerWeakUp( int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	ch->Status = 0;
	ch->can_memorize_time = g_curr_time;

	t_packet packet;
	packet.h.header.type = CMD_SERVER_WAKE_UP;
	packet.h.header.size = sizeof(0);
	QueuePacket(connections, cn, &packet, 1);
}

void SendJoinGameWho( int cn )		// cn이란 놈이 새로 들어왔을 경우 로그인서버로 내 파티들이 있는지를 요청하고...
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	for( int a=0; MAX_PARTY_MEMBER > a ; a++ )
	{
		if(ch->party[a].Name[0])	
		{
			ReqPartyDB( cn, a, ch->party[a].Name );		// 내가 맺었던 파티 정보를 일단 로그인 서버에 요청한다.
		}
	}
	SendCMD_REQUEST_CHECK_WARNO(cn);// 010915 LTS
}

CHARLIST *CheckServerIdAndName( int cn, char *name )
{
	CHARLIST *ch = CheckServerId( cn );
	if( ch && strcmp(ch->Name, name )==0 )
		return ch;

	return NULL;
}

void SendExitGameWho( int cn )			// 나랑 동맹 맺고 있는 사람 모두를 찾아서 보내줘야 하나 아님 내가 맺고 있는 사람 6명에만 보내주나?
{	//< CSD-030506
	CHARLIST *ch = &connections[cn].chrlst;
	
	if (EXCHANGE_STATE_READY != ch->GetExchangeState())
	{	// 거래중이던걸 취소 시킨다.
		SendExchangeCancel(ch->GetExchangeTargetId(), cn);
	}
	//< CSD-HK-030828
	g_pUserManager->DelMember(cn); 
	g_pDualManager->DelMember(cn);

	const int nGuildCode = ch->GetGuildCode();

	if (nGuildCode > 0)
	{	
		g_pGuildManager->DelMember(nGuildCode, cn);
	}

	if (g_pArenaManager->IsColossusArena())
	{
		g_pArenaManager->Remove(cn);
	}
	//> CSD-HK-030828
	for( int i=0; i<6; i++ )
	{
		if( !ch->party[i].On ) continue;
		//CHARLIST *gammer = CheckServerIdAndName( ch->party[i].Server_id, ch->party[i].Name ); 
		CHARLIST *gammer = CheckServerName( ch->party[i].Name ); // kyo
		if( gammer )
		{
			for( int j=0; j<6; j++ )
			{
				if( gammer->party[j].On && !strcmp(gammer->party[j].Name, ch->Name) )	// 그사람도 나와 동맹을 맺은 사람이라면...
				{
					//gammer->party[j].Server_id = 0;
					t_packet packet;
					packet.h.header.type = CMD_SERVER_EXIT_GAME;
					{
						strcpy( packet.u.kein.client_char_name.name, ch->Name );
					}
					packet.h.header.size = sizeof( k_client_char_name );
					QueuePacket(connections, gammer->GetServerID(), &packet, 1);
					break;
				}
			}
		}
	}
}	//> CSD-030506

void SendMyPartyExist(int cn)			// 새로들어온 캐릭터에게 파티원이 있는 지를 보내준다.
{	//< CSD-030415
	CHARLIST* ch = CheckServerId(cn);
	
	if (!ch)
	{
		return;
	}

	for	(int i = 0; i < MAX_PARTY_MEMBER; ++i)
	{
		if (ch->party[i].On)
		{
			CHARLIST* pParty = CheckServerName(ch->party[i].Name);

			if (pParty == NULL)
			{
				continue;
			}
				
			t_packet packet;
			packet.h.header.type = CMD_REQ_MY_PARTY;
			packet.h.header.size = sizeof(k_client_char_name);
			strcpy(packet.u.kein.client_char_name.name, ch->party[i].Name);
			QueuePacket(connections, cn, &packet, 1);
		}
	}

	SendPartyJoinOkSet( cn );		// 파티원들 전부 조사해서 보내준다.
}	//> CSD-030415

void SendPartyJoinOk( char *name, int cn )
{
	t_packet p;

	p.h.header.type = CMD_PARTY_JOIN_OK;
	{
		strcpy(p.u.kein.client_char_name.name, name);
	}
	p.h.header.size = sizeof( k_client_char_name );
	QueuePacket( connections, cn, &p, 1 );
}

int ExistHe( char *name )//020903 lsw
{	//< CSD-030314
	return g_pUserManager->GetServerID(name);
}	//> CSD-030314

int ExistConnectingHe( char *name )
{	//< CSD-030314
	return -1;
}	//> CSD-030314


void RecvReleaseParty( char *name, int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	int party_id = ExistHe( name);
	if( party_id > 0 ) 
	{
		t_packet p;
		p.h.header.type = CMD_PARTY_JOIN_RELEASE;
		{
			strcpy(p.u.kein.client_char_name.name, ch->Name);
		}
		p.h.header.size = sizeof( k_client_char_name );
		QueuePacket( connections, party_id, &p, 1 );
	}
}
	

void SendPartyJoinOkSet( int cn )		// 접속한 캐릭터의 파티원 셋..
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	for( int i=0; i<6; i++)
	{
		if( !ch->party[i].On ) continue;

		int id = ExistHe( ch->party[i].Name );
		if( id>0 )	// 현재 접속해 있으면
		{
			CHARLIST *pa = &connections[id].chrlst;
			if( !pa ) return;
			
			for( int j=0; j<6; j++ )
			{
				if( !strcmp( ch->Name, pa->party[j].Name ) ) 
				{
					SendPartyJoinOk( ch->party[i].Name, cn );		// 먼저 메시지를 보낸 놈에게 ok를 보내주고
					SendPartyJoinOk( ch->Name, pa->GetServerID());	// 상대편도 겜에 접속해 있으면 ok를 보내준다.
					break;
				}
			}
//			if( j==6 ) RecvReleaseParty( ch->party[i], cn );	// 그 놈은 나와 동맹을 맺고 있지 않다...
		}
//		else		// 현재 접속해 있지 않다면..
//		{
//			char you_party[6][31];
//			get_BinaryData_Party( (UCHAR **)you_party, ch->party[i] );

//			for( int j=0; j<6; j++ )
//			{
//				if( !strcmp( ch->Name, you_party[j] ) ) 
//				{
//					SendPartyJoinOk( ch->party[i], cn );
//					break;
//				}
//			}
//			if( j== 6) RecvReleaseParty( ch->party[i], cn );
//		}
	}
}

void SendServerEachItem( const ItemAttr *item, const CHARLIST *ch )
{
	if( !item ) return;

	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				if( &ch->inv[a][b][c] == item )
				{
					POS pos;
					SetItemPos( INV, a, b, c, &pos );
					SendServerEachItem( &pos, item, ch->GetServerID());
					return;
				}
			}
}

void SendServerEachItem( const POS *pos, const ItemAttr *item, const int cn )			// 그자리에 콕 찝어서 값을 보내준다.
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	t_packet packet;
	packet.h.header.type = CMD_SERVER_EACH_ITEM;
	{
		packet.u.kein.server_item.item_pos  = *pos;
		packet.u.kein.server_item.item_attr = *item;
	}
	packet.h.header.size = sizeof( K_ITEM );
	QueuePacket(connections, cn, &packet, 1);
}

void SendAbilityOpen( const int cn )
{
	CHARLIST *ch = ::CheckServerId(cn);
	if( !ch ) 
	{
		return;
	}
	t_packet packet;
	packet.h.header.type = CMD_OPEN_ABILITY;
	{ 
		packet.u.kein.server_resist.naBasicResist[RT_POISON] = ch->GetBasicResist(RT_POISON);
		packet.u.kein.server_resist.naBasicResist[RT_CURSE] = ch->GetBasicResist(RT_CURSE);
		packet.u.kein.server_resist.naBasicResist[RT_HOLY] = ch->GetBasicResist(RT_HOLY);
		packet.u.kein.server_resist.naBasicResist[RT_FIRE] = ch->GetBasicResist(RT_FIRE);
		packet.u.kein.server_resist.naBasicResist[RT_ICE] = ch->GetBasicResist(RT_ICE);
		packet.u.kein.server_resist.naBasicResist[RT_ELECT] = ch->GetBasicResist(RT_ELECT);

		packet.u.kein.server_resist.naExtentionResist[RT_POISON] = ch->GetExtPoisonByItem();//아이템에 의해 더해지는 마법방어력
		packet.u.kein.server_resist.naExtentionResist[RT_CURSE]	= ch->GetExtCurseByItem() ;
		packet.u.kein.server_resist.naExtentionResist[RT_HOLY]	= ch->GetExtHolyByItem() ;
		packet.u.kein.server_resist.naExtentionResist[RT_FIRE]	= ch->GetExtFireByItem() ;
		packet.u.kein.server_resist.naExtentionResist[RT_ICE]	= ch->GetExtIceByItem() ;
		packet.u.kein.server_resist.naExtentionResist[RT_ELECT]	= ch->GetExtElectByItem(); 
	} 
	packet.h.header.size = sizeof(k_server_resist);
	QueuePacket(connections, cn, &packet, 1);
}

void SendSubtractMoney( DWORD money, int cn )
{
	return;
	t_packet packet;
	packet.h.header.type = CMD_SUBTRACT_MONEY;
	{
		packet.u.kein.server_money.money = money;	
	}
	packet.h.header.size = sizeof(k_server_money);
	QueuePacket(connections, cn, &packet, 1);
}

void SendAddMoney( DWORD money, int cn )
{
	t_packet packet;
	packet.h.header.type = CMD_ADD_MONEY;
	{
		packet.u.kein.server_money.money = money;	
	}
	packet.h.header.size = sizeof(k_server_money);
	QueuePacket(connections, cn, &packet, 1);
}

//////////////////////////////////////////////////////////////
//
// cheat command		  // learn magic, learn skill
//
////////////////////////////////////////////////////////////
void RecvLearnAll( int cn ) 
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	for( int i=0; i<200; i++ )
	{
		ch->Skill[i] = true;
		ch->Ws[i] = true;
		ch->Ps[i] = true;
	}
}

void RecvLearnAllNot( int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	for( int i=0; i<200; i++ )
	{
		ch->Skill[i] = false;
		ch->Ws[i] = false;
		ch->Ps[i] = false;
	}
}

///////////////////////////////////////////////////////////////
//	캐릭터 처음 접속했을때 입고 있는 아이템...및 여러가지 처리
int CheckCurrStats( CHARLIST *ch )		// 010521 YGI
{
	ch->Money = GetMoneyByItem( ch );
	ch->itemweight		= GetTotalItemWeight( ch );	
	ch->Tactics_para	= SetTactics( ch->Spell, ch->Gender, ch->Tactics );

	if (ch->IsDead())
	{	
		ch->bAlive		= DEAD_; 
		ch->SetState(CON_DEATH);
		ch->viewtype	= VIEWTYPE_GHOST_;
	}	
	else 
	{	
		ch->bAlive 		= ALIVE_;
		ch->SetState(CON_NORMAL);
		ch->viewtype	= VIEWTYPE_NORMAL_;
	}

	EquipItem( ch );
	ch->pkid = -1; // 나중에 가치관으로 바뀜...... 최소에 PK를 하면 그의 ID를 가진다. ...
	ch->can_memorize_time = g_curr_time;		// 메모라이즈 할 수 있는 시간 세팅
	return 1;
}

void SendServerItemAttr( short int cn, int type, POS pos, ItemAttr *item )
{	
	short int header_type;
	switch( type )
	{
		case IATTR_DURATION	:		header_type=CMD_IATTR_DURATION	; break;
		case IATTR_LIMIT	:		header_type=CMD_IATTR_LIMIT		; break;
		case IATTR_ATTR		:		header_type=CMD_IATTR_ATTR		; break;
		case IATTR_ID		:		header_type=CMD_IATTR_ID		; break;
		case IATTR_RESERVED0:		header_type=CMD_IATTR_RESERVED0	; break;
		case IATTR_RESERVED1:		header_type=CMD_IATTR_RESERVED1	; break;

		default : return;
	}

	t_packet packet;
	packet.h.header.type = header_type;
	{
		packet.u.kein.server_item_attr.pos = pos;
		packet.u.kein.server_item_attr.attr = item->attr[type];
	}
	packet.h.header.size = sizeof( k_server_item_attr );
	QueuePacket(connections, cn, &packet, 1);
}	

// 020428 YGI acer
// 아이템 삭제 전송 함수 // 아이템이 듀레이션이 제로여서 부서질경운 type을 바꾼다.
int SendDeleteItem( ItemAttr *pItem, POS *pos, CHARLIST *ch, int broken/*=0*/ )
{
	if( pItem )
	{
		if( pos->type == EQUIP )
		{	
			CItem *t = ItemUnit( *pItem );
			if( t ) t->ReleaseItem( ch );
		}
		if( pItem->attr[IATTR_RARE_MAIN])
		{
			const int nRare = pItem->attr[IATTR_RARE_MAIN];
			Send_RareItemMakeLog(ch->GetServerID(), pItem->item_no,-1,
			((LPRareMain)&nRare)->grade,((LPRareMain)&nRare)->soksung1,((LPRareMain)&nRare)->soksung2,
			((LPRareMain)&nRare)->soksung3,0,0,0,pItem->attr[IATTR_RARE_MAIN],pItem->attr[IATTR_LIMIT],0,-1,-1,0,0,0,0,0,0);			//020730 lsw
		}
	}

	t_packet p;
	int type;
	if( 1 == broken ) { type = CMD_DELETE_ITEM_BY_DURATION; }
	else if (2 == broken ) { type = CMD_DELETE_ITEM_MALL_ITEM_PER_1MIN_CLIENT; }
	else { type = CMD_DELETE_ITEM; }
	
	
	p.h.header.type = type;
	{
		p.u.kein.server_delete_item.item_pos = *pos;
	}
	p.h.header.size = sizeof( k_server_delete_item ); 
	QueuePacket(connections, ch->GetServerID(), &p, 1);

	return DeleteItem( pItem );
}

void SendServerAc( int cn )
{ //< CSD-020821
	CHARLIST *ch = CheckServerId( cn );
	if (!ch) return;
  
  t_packet packet;
	packet.h.header.type = CMD_REQ_AC;
  packet.h.header.size = sizeof(k_server_ac);
	packet.u.kein.server_ac.ac = ch->GetExtendAc();
	QueuePacket(connections, cn, &packet, 1);
} //> CSD-020821

void SendServerDmg( int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	t_packet packet;
	packet.h.header.type = CMD_REQ_DMG;
  packet.h.header.size = sizeof(k_server_ac);
	packet.u.kein.default_int = ch->CalcPhysicalStrikingPower();
	QueuePacket(connections, cn, &packet, 1);
}

inline void CallClient( short int id, short int type )
{
	t_packet packet;
	packet.h.header.type = type;
	packet.h.header.size = 0;
	QueuePacket(connections, id, &packet, 1);
}

void SendPacketDefault( int type, void *msg, int size, short int cn )
{
	t_packet packet;
	packet.h.header.type = type;
	packet.h.header.size = 0;
	if( msg && size)
	{
		memcpy( packet.u.data, msg, size );
		packet.h.header.size = size;
	}
	QueuePacket(connections, cn, &packet, 1);
}

void SendServerKillMon( short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	t_packet packet;
	packet.h.header.type = CMD_SERVER_KILLMON;
	{
		packet.u.kein.server_killmon.killmon =  ch->killmon;
	}
	packet.h.header.size = sizeof(k_server_killmon);
	QueuePacket(connections, cn, &packet, 1);
}



/// 0212 YGI /////////
int SetTactics( int spell_type, int sex, int tac_type )		// 캐릭터 메이크할경우 선택한 택틱스를 실제 배열첨자로 바꾸는 루틴..
{
	int tac_con[2][12] = { 
		{ CRAPPLE_, SWORD_, ARCHERY_, FENCING_, MACE_, PIERCE_, WHIRL_, HURL_, PARRYING_, MAGERY_, },		 // 여자
		{ CRAPPLE_, SWORD_, ARCHERY_, PARRYING_, MACE_, PIERCE_, WHIRL_, HURL_, MAGERY_, D_SWORD_, D_MACE_ },	// 남자
	};

	int ret = tac_con[sex][tac_type];
	if( spell_type == PRIEST_SPELL && ret == MAGERY_ ) ret = ORISON_;
	return ret;
}


/*
void LearnSkillOfJob( int inc_job, CHARLIST *ch)
{
	int skill_mother[32][2] = {	{1,3},   {3,5},   {5,8},   {8,10},  {10,16}, {16,19}, {19,21}, {21,25}, {25,28}, {28,32},
								{32,34}, {34,35}, {35,38}, {38,42}, {42,43}, {43,44}, {44,45}, {45,48}, {48,50}, {50,53},
								{53,55}, {55,56}, {56,57}, {57,64}, {64,67}, {67,73}, {73,75}, {75,76}, {76,78}, {78,86},
								{86,87}, {87,101}	};

	int min = skill_mother[inc_job][0];
	int max = skill_mother[inc_job][1];

	for( int i=0; i<max; i++ )	ch->Skill[i] = 1;
}
*/
/*
int SendPartyInfo2( short int type, char ct,  short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return -1;

	t_packet packet;
	packet.h.header.type = CMD_SERVER_PARTY;
	{
		packet.u.kein.party_info.type		= (short) type;
		packet.u.kein.party_info.index		= (char) ct;

		strcpy( packet.u.kein.party_info.m_Name, ch->party[ct].Name);
		packet.u.kein.party_info.m_Gender	= (char) ch->party[ct].Gender;
		packet.u.kein.party_info.m_ImageNo	= (short) ch->party[ct].Face;
		packet.u.kein.party_info.m_Lv		= (char) ch->party[ct].Level;
		packet.u.kein.party_info.m_Str		= ch->party[ct].Str;
		packet.u.kein.party_info.m_Class	= (char) ch->party[ct].Class;
		packet.u.kein.party_info.m_Join		= 1;
	}
	packet.h.header.size = sizeof(k_party_info);
	QueuePacket(connections, cn, &packet, 1);
	return 1;
}
*/
//int SendPartyInfo3( short int type, char ct,  short int cn )		// 맨처음 파티원들의 정보를 보내주는 곳		// 현재는 파티만
int SendPartyInfo3( short int type, char ct,  const char* szName)		// 맨처음 파티원들의 정보를 보내주는 곳		// 현재는 파티만
{
	//CHARLIST *ch = CheckServerId( cn );
	CHARLIST *ch = CheckServerName( szName );
	if( !ch ) return -1;
	int cn = ch->GetServerID();

	t_packet packet;
	memset( &packet, 0, sizeof( t_packet ) );
	packet.h.header.type = CMD_SERVER_PARTY;
	{
		packet.u.kein.party_info.type		= (short) type;
		packet.u.kein.party_info.index		= (char) ct;

		strcpy( packet.u.kein.party_info.m_Name, ch->party[ct].Name);
		packet.u.kein.party_info.m_Gender	= (char) ch->party[ct].Gender;
		packet.u.kein.party_info.m_ImageNo	= (short) ch->party[ct].Face;
		packet.u.kein.party_info.m_Class	= (char) ch->party[ct].Class;
	}
	packet.h.header.size = sizeof(k_party_info);
	QueuePacket(connections, cn, &packet, 1);
	
	//SendPartyInfo( type, ct, connections, cn );				// 서로 동맹관계 여부를 보내준다. 
	SendPartyInfo( type, ct, connections, szName );				// 서로 동맹관계 여부를 보내준다. 
	return 1;
}

void ReqPartyDB( short int client_id, char para, char *name )
{
	t_packet packet;
	packet.h.header.type = CMD_PARTY_ACCESS;
	{
		packet.u.kein.server_req_party_db.server_id = client_id;
		strcpy( packet.u.kein.server_req_party_db.name, name );
		packet.u.kein.server_req_party_db.ct = para;
	}
	packet.h.header.size = sizeof( k_server_req_party_db ) ;
	QueuePacket(connections, DB_DEMON, &packet, 1);
}

void RecvPartyInfoServerToServer( k_server_server_party_info *p )
{
	CHARLIST *ch = CheckServerId( p->Server_id );
	if( !ch ) return;

	if( !(*ch->party[p->ct].Name) ) return;		// 파티원이 없다면

	ch->party[p->ct].Face =		( int ) p->Face;
	ch->party[p->ct].Level =	( int ) p->Level;
	ch->party[p->ct].Gender =	( int ) p->Gender;
	ch->party[p->ct].Str =		( int ) p->Str;
	ch->party[p->ct].Class =	( int ) p->Class;

	if( !strcmp( ch->party[p->ct].Name, ch->Name ) ) return;		// 맺은 파티원이 자기 자신일 경우 더이상 진행을 하지 않는다.

	//SendPartyInfo3( PARTY, p->ct, p->Server_id );	// 새로 들어 온 놈에게 파티원들의 정보를 보내준다.
	SendPartyInfo3( PARTY, p->ct, ch->Name );	// 새로 들어 온 놈에게 파티원들의 정보를 보내준다.

	// 그놈도 나랑 파티를 맺었는지를 알아본다.		// 로그인 서버 요청
	SendReqPartyInfoOfOtherChar( ch->party[p->ct].Name, ch->Name, p->Server_id );
}


int EqualPosByPos( const POS *pos1, const POS *pos2 )
{
	if( pos1->type != pos2->type ) return 0;
	if( pos1->p1 != pos2->p1 ) return 0;
	if( pos1->p2 != pos2->p2 ) return 0;
	if( pos1->p3 != pos2->p3 ) return 0;
	return 1;
}


// 내 시야 안에 그가 있는 지 없는지를 판단.	name->특정 그 사람이름, cn->보낸 클라이언트의 아이디
int IsMySightInWho( char *name, short int cn)		
{
	for(int i = 0; i < MAX_AREA_BUFFER; i ++)
	{
		if(connections[cn].area_tbl[i].bAlive == TRUE && connections[cn].area_tbl[i].Type == 1)
		{
			short int he = connections[cn].area_tbl[i].ID;
			CHARLIST *he_ch = CheckServerId(he);
			if( !he_ch ) continue;
			if( strcmp( he_ch->Name, name )==0 ) return 1;
		}
	}
	return 0;
}

int IsMySightInWhoById( short int check_id, short int cn )
{
	for(int i = 0; i < MAX_AREA_BUFFER; i ++)
	{
		if(connections[cn].area_tbl[i].bAlive && connections[cn].area_tbl[i].Type == 1)
		{
			short int he = connections[cn].area_tbl[i].ID;
			if( he == check_id ) return 1;
		}
	}
	return 0;
}

void SendReqPartyInfoOfOtherChar( char *party_name, char *my_name, short int server_id )
{
	t_packet packet;
	memset( &packet, 0, sizeof( t_packet ) );

	packet.h.header.type = CMD_REQ_PARTY_TOGETHER;
	{
		strcpy( packet.u.kein.server_req_party_together.party_name , party_name );
		strcpy( packet.u.kein.server_req_party_together.my_name, my_name );
		packet.u.kein.server_req_party_together.server_id = server_id;
	}
	packet.h.header.size = sizeof( k_server_req_party_together ) ;
	QueuePacket(connections, DB_DEMON, &packet, 1);
}

void RecvPartyTogetherByLoginServer( short int server_id, char *name )
{
	t_packet packet;
	packet.h.header.type = CMD_REQ_JOIN_GAME;
	{
		strcpy( packet.u.kein.client_char_name.name, name );
	}
	packet.h.header.size = sizeof( k_client_char_name );
	QueuePacket(connections, server_id, &packet, 1);

	CHARLIST *ch = &connections[server_id].chrlst;
	if( !ch ) return;
	for( int i=0; i<6; i++ ) 
		if( !strcmp( ch->party[i].Name, name ) ) 
		{
			//SendPartyInfo( PARTY, i, connections , server_id );	
			SendPartyInfo( PARTY, i, connections , name );
			return;
		}
}		
		


void RecvCharInfoPlus( int type, t_connection c[], int cn )		// 현재는 파티일경우만...
{
	CHARLIST *ch = &c[cn].chrlst;
	if( !ch ) return;

	if( type == PARTY )
	{
		for( int i=0; i<6; i++ )
			if( ch->party[i].Name[0] )
			{
				ch->party[i].On = true;
				//SendPartyInfo( PARTY, i, c, cn );
				SendPartyInfo( PARTY, i, c, ch->party[i].Name );
			}
	}
}

int RecvNoParty(char *name, short int server_id)	//잘 못된 이름을 가지고 있을때... 지운다.
{	//< CSD-HK-030829
	CHARLIST* ch = CheckServerId(server_id);
	
	if (ch == NULL)
	{
		return 0;
	}
		
	for (int i = 0; i < MAX_PARTY_MEMBER; ++i)
	{
		if (strcmp( ch->party[i].Name, name) == 0) 
		{
			memset(&ch->party[i], 0, sizeof(CharacterParty));
			return 1;
		}
	}
		
	return 0;
}	//> CSD-HK-030829

void SendServerMagery( short int cn )			// 클라이이언트에서 요청하는 택틱스를 보내준다.
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	t_packet packet;
	packet.h.header.type = CMD_REQ_MAGERY;
	{
		if( ch->Spell == WIZARD_SPELL )
    {
      packet.u.kein.server_tactics.nStep = ch->GetTacticsStep(MAGERY_);
			packet.u.kein.server_tactics.magery = ch->tac_skillEXP[MAGERY_];
    }
		else if( ch->Spell == PRIEST_SPELL ) 
    {
      packet.u.kein.server_tactics.nStep = ch->GetTacticsStep(ORISON_);
			packet.u.kein.server_tactics.magery = ch->tac_skillEXP[ORISON_];
    }
	}
	packet.h.header.size = sizeof( k_server_tactics );
	QueuePacket(connections, cn, &packet, 1);	
}

void RecvBbsMessageReq( int type, char ct, short int cn )	// 공지사항등 내용 전송
{
	t_packet p;
	p.h.header.type = CMD_GET_SERVER_TEXT ;
	{
		switch( type )
		{
			case CMD_PUBLIC_NOTICE_BBS_REQ:	{
												;
												char *szTemp;
												szTemp = GetMessageBBS_public( ct );
												if( !szTemp )
												{
													szTemp = new char[100];
													sprintf( szTemp, "내용이 없습니다." );
												}
												int len=strlen( szTemp );
												int max = (len / 512)+1;
												for( int i=0; i<max; i++ )
												{
													char *temp = szTemp+512*i;
													strncpy( p.u.kein.get_server_text.text, temp ,512 );
													p.u.kein.get_server_text.ct = ct;
													p.u.kein.get_server_text.len = len;
													
													p.h.header.size = sizeof( k_get_server_text );
													QueuePacket(connections, cn, &p, 1);
												}
												SAFE_DELETE_ARRAY(szTemp);
											}
			case CMD_RANK_BBS_REQ:
			case CMD_FORUM_BBS_REQ:		
			case CMD_EVENT_BBS_REQ:			
			case CMD_MYDIARY_BBS_REQ:
			case CMD_BBS_BBS_REQ:	
			case CMD_ETC_BBS_REQ:		;
		}
	}
}
/*	020303 lsw
void SendLoginPutBoxItem( DWORD box_id, int para, ItemAttr *lpItem )
{
	t_packet packet;

	/////////////////////// DB-DEMON 에서 DB 접근 ////////////////////////
	packet.h.header.type = CMD_PUT_BOX_ITEM;
	{
		packet.u.kein.login_put_box_item.box_id = box_id;
		packet.u.kein.login_put_box_item.para = (char )para;
		packet.u.kein.login_put_box_item.item = *lpItem;
	}
	packet.h.header.size = sizeof( login_put_box_item );
	QueuePacket(connections, DB_DEMON, &packet, 1);		// login_server_connect5`
	////////////////////////////////////////////////////////////////////


	/////////////////////// LOGIN-SERVER ///////////////////////////////
//	packet.h.header.type = CMD_RECALL_BOX_ITEM;
//	{
//		packet.u.kein.game2login_recall_box_item.box_id = box_id;
//	}
//	packet.h.header.size = sizeof( k_game2login_recall_box_item );
//	QueuePacket(connections, LOGIN_SERVER_CONNECT, &packet, 1);
	/////////////////////////////////////////////////////////////////////
}
*/

void SendLoginServerPreachName( k_server_preach_name *p, short int cn )		// 로그인 서버로 보낸다.
{
	t_packet packet;
	packet.h.header.type = CMD_LOGIN_PREACH_NAME;
	{
		packet.u.kein.loginserver_preach_name.server_id= cn;
		strcpy( packet.u.kein.loginserver_preach_name.name, p->name );
		packet.u.kein.loginserver_preach_name.believe_god= p->believe_god;
	}
	packet.h.header.size = sizeof( k_loginserver_preach_name );
	QueuePacket(connections, DB_DEMON, &packet, 1);
}

void RecvAndSendPreachName( k_loginserver_preach_name_result *p)	// 결과를 받아서 클라이언트로 보낸다.
{
	t_packet packet;
	packet.h.header.type = CMD_PREACH_NAME;
	{
		packet.u.kein.server_client_preach_name.believe_god	= p->believe_god;			// 믿는 신
		packet.u.kein.server_client_preach_name.result	= p->result;		// 결과 
		sprintf(packet.u.kein.server_client_preach_name.name, p->name);
	}
	packet.h.header.size = sizeof( k_server_client_preach_name );
	QueuePacket(connections, p->server_id, &packet, 1);
}

void RecvGodRegistCancel( short int cn )	// 파계
{
	CHARLIST *ch = CheckServerId( cn );
	if( !cn || !ch ) return; 

	ch->believe_god = 0;
	ch->faith = 0;

	if( *ch->preach_name )		// 자신을 전도한 사람이 있다면
	{
		CHARLIST* preach_ch = SearchIdByName(ch->preach_name); // CSD-030320

		if( !preach_ch )		
		{
			// 게임내에 없다는 뜻, 로그인 서버로 신앙심 깍으라고 전송
			t_packet packet;
			packet.h.header.type = CMD_LOGIN_PREACH_NAME_CANCEL;
			{
				strcpy( packet.u.kein.server_preach_name.name, ch->preach_name );
				packet.u.kein.server_preach_name.believe_god= ch->believe_god;
			}
			packet.h.header.size = sizeof( k_server_preach_name );
			QueuePacket(connections, DB_DEMON, &packet, 1);
		}
		else
		{
			if( preach_ch->believe_god == ch->believe_god )		// 같은 신전에 등록되어 있어야 한다.
			{
				preach_ch->faith -= 10;
				if( preach_ch->faith < 0 ) preach_ch->faith = 0;
				// 그 사람한테 전송해야 하나?
			}
		}
		SendLoginEvangelistDelete( ch->Name );
	}

	ZeroMemory( ch->preach_name, sizeof( char[20] ) );
	return;
}

void RecvGodRegistOk( char believe_god, short int cn )		// 등록
{
	CHARLIST *ch = CheckServerId( cn );
	if( !cn || !ch ) return; 

	if( !believe_god ) return;
	ch->believe_god = believe_god;
	ch->faith = 10;		//초기 신앙 수치
}

void SendLoginServePreachNameOk( k_server_preach_name *p, short int cn )		// 이 함수는 전도한 사람에게 신앙수치를 올려주는 루틴이다.
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	// 로그인 서버로 SQL문 요청
	t_packet packet;
	
	memset(&packet, 0, sizeof( t_packet));
	packet.h.header.type = CMD_LOGIN_PREACH_NAME_OK;
	packet.h.header.size = sizeof( k_loginserver_preach_name );
	packet.u.kein.loginserver_preach_name.server_id = cn ;
	strcpy( packet.u.kein.loginserver_preach_name.name, p->name );
	packet.u.kein.loginserver_preach_name.believe_god= p->believe_god;
	QueuePacket(connections, DB_DEMON, &packet, 1);

	memset(&packet, 0, sizeof(t_packet));
	packet.h.header.type = CMD_REQ_FAITH_UP; // 혹시 서버에 있는지 확인한다.
	packet.h.header.size = sizeof( k_req_faith_up );
	strcpy( packet.u.kein.req_faith_up.name,p->name ) ;
	packet.u.kein.req_faith_up.up_point = 10; // 10 올려 준다.
	g_pServerTable->BroadCastToEveryServer((char*)&packet, sizeof(t_header) + packet.h.header.size, SERVER_TYPE_MAP);

	strcpy(ch->preach_name, p->name); // 전도자의 이름을 넣고
	SendLoginEvangelistCreate(ch->Name, p->name);	// 전도한 사람 이름 디비에 저장
}


void SendLoginEvangelistCreate( char *my_name, char *eva_name )
{
	t_packet packet;
	packet.h.header.type = CMD_LOGIN_EVANGELIST_CREATE;
	{
		strcpy( packet.u.kein.login_sql_evangelist_create.my_name, my_name );
		strcpy( packet.u.kein.login_sql_evangelist_create.eva_name, eva_name );
	}
	packet.h.header.size = sizeof( k_login_sql_evangelist_create );
	QueuePacket(connections, DB_DEMON, &packet, 1);
}

void SendLoginEvangelistDelete( char *my_name )
{
	t_packet packet;
	packet.h.header.type = CMD_LOGIN_EVANGELIST_DELETE;
	{
		strcpy( packet.u.kein.login_sql_evangelist_delete.my_name, my_name );
	}
	packet.h.header.size = sizeof( k_login_sql_evangelist_delete );
	QueuePacket(connections, DB_DEMON, &packet, 1);
}



struct GodTable g_GodTable[MAX_GOD];
char YearMeetingText[MAX_GOD][4][MAX_GOD_TEXT];
char MonthMeetingText[MAX_GOD][12][MAX_GOD_TEXT];


int g_y, g_mo, g_d, g_h, g_mi, g_se;		// 게임시간
extern void CurrentGameDate( DWORD t, int *y, int *mo, int *d, int *h, int *mi, int *sec );

void LoadGetGodTableForTest( int type )
{
	CurrentGameDate( g_curr_time*6, &g_y, &g_mo, &g_d, &g_h, &g_mi, &g_se );
	switch( type )
	{
		case YEAR_MEETING:
			{
				for( int i=1; i<MAX_GOD; i++ )			// 테스트용		// year
				{
					g_GodTable[i].meet_year_month	= g_mo+1;
				}
			}

		case MONTH_MEETING :
			{
				for( int i=1; i<MAX_GOD; i++ )			// 테스트용		// year
				{
					g_GodTable[i].meet_year_day		= g_d+1;
					g_GodTable[i].meet_year_hour	= g_h;
					g_GodTable[i].meet_month_day	= g_d+1;
					g_GodTable[i].meet_month_hour	= g_h;
				}
			}
		case DAY_MEETING :
			{
				for( int i=1; i<MAX_GOD; i++ )			// 테스트용
					g_GodTable[i].meet_day_hour		= g_h;
				break;
			}
		case MONTH_MEETING_ALAM :
			{
				for( int i=1; i<MAX_GOD; i++ )			// 테스트용		// year
				{
					g_GodTable[i].meet_month_day	= g_d;
					g_GodTable[i].meet_month_hour	= g_h;
				}
				break;
			}
	}
}

void LoadGetGodTable()
{	//< CSD-030506
	TIMESTAMP_STRUCT day, month, year;
	
	for (int i=1; i < MAX_GOD; ++i)
	{
		GetGodMeetingTime( day, month, year, i );

		g_GodTable[i].meet_year_month = (char)year.month;
		g_GodTable[i].meet_year_day = (char)year.day;	
		g_GodTable[i].meet_year_hour = (char)year.hour;
		g_GodTable[i].meet_month_day = (char)month.day;
		g_GodTable[i].meet_month_hour = (char)month.hour;
		g_GodTable[i].meet_day_hour = (char)day.hour;
	}

	LoadMeetingMessage();
}	//> CSD-030506

void LoadMeetingMessage()
{
	for( int a=EDELBLHOY; a<MAX_GOD; a++ )
	{
		for( int i=0; i<4; i++ )
			GetMeetingMessage( a, YEAR_MEETING, i, YearMeetingText[a][i] );

		for(int i=0; i<12; i++ )
			GetMeetingMessage( a, MONTH_MEETING, i, MonthMeetingText[a][i] );
	}
}

char *GetGodMeetMessage( int god_index, int type, int num )
{
	switch( type )
	{
		default : break;
		case YEAR_MEETING  :	return YearMeetingText[god_index][num];
		case MONTH_MEETING : 	return MonthMeetingText[god_index][num];
	}

	return NULL;
}


extern void DugeonMgrProc();// 020620 YGI
extern void CheckDay();

void CheckTimeForMenu_Kein()			//	1분에 한번씩 들어온다.
{	//< CSD-CN-031213
	static DWORD time = g_curr_time;
	if( (g_curr_time - time) < 30 ) return;		// 30초 
	time = g_curr_time;

	CheckDay();
	DugeonMgrProc();   // 020620 YGI
	g_EventMgr.Proc(); // 021030 YGI

	if(LottoSystem())
	{	//<soto - 030517
		LottoSystem()->RunProc();
	}	//>soto - 030517

	//050224_KCH 마일리지몰 작업	(매분마다 사용기간완료된 아이템을 삭제한다.)
	if(nITEM_MALL_MANAGER_PORT == g_pServerTable->GetOwnServerData()->wPort)
	{
		g_pItemMallManager->Process_Item_to_game_TBL();
	}

	CurrentGameDate( g_curr_time*6, &g_y, &g_mo, &g_d, &g_h, &g_mi, &g_se );
	// 잠자기 상태에 대한 처리
	CUserManager::HASH_USER user = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = user.begin(); i != user.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = CheckServerId(idUser);
		
		if (pUser != NULL && pUser->Status)
		{	
			if (pUser->IsFullLife() && pUser->IsFullMana())
			{	// 더이상 쉴필요가 없다면
				SendServerWeakUp(idUser);	// 깨어나라
			}
		}
	}
}	//> CSD-CN-031213

void RecvToolPassword( char *password, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	char *pass = "wkfehlflfk";
	int len = strlen( pass );
	if( !strncmp(password, pass, len) )
	{
		strcpy( g_szPassWordName, ch->Name );
	}
}

void SendCallNoteMeeting( int type, CHARLIST *ch )		// 예배시간 다 됐다고 알리는 루틴
{
	return;		// 다음에...

	t_packet packet;
	packet.h.header.type = CMD_NOTE_MEETING;
	{
		packet.u.kein.note_meeting.meeting_type = type/DAY_MEETING;
		packet.u.kein.note_meeting.believe_god = ch->believe_god;
	}
	packet.h.header.size = sizeof( k_note_meeting );
	

	QueuePacket(connections, ch->GetServerID(), &packet, 1);
}

int CheckGodMeetingTime( CHARLIST *ch )
{
//	return MONTH_MEETING;

	if( !ch ) return 0;
	if( !ch->believe_god ) return 0;
	
	static bool year_time, month_time, day_time;		// 신앙심 체크를 한번만 하기 위해

	/////////////// 예배 메뉴를 띄워야 할지 판단하는 루틴
	if( g_GodTable[ch->believe_god].meet_year_month	== g_mo+1 &&
		g_GodTable[ch->believe_god].meet_year_day	== g_d+1 &&
		g_GodTable[ch->believe_god].meet_year_hour	== g_h ) 
	{
		year_time = 0;
		return YEAR_MEETING;
	}

	else if(	!year_time  &&
				g_GodTable[ch->believe_god].meet_year_month	== g_mo+1 &&
				g_GodTable[ch->believe_god].meet_year_day	== g_d+1 &&
				g_GodTable[ch->believe_god].meet_year_hour+1 == g_h ) 
	{
//		year_time =1;
		SendCallNoteMeeting( YEAR_MEETING, ch );
	}

	if( g_GodTable[ch->believe_god].meet_month_day	== g_d+1 &&
		g_GodTable[ch->believe_god].meet_month_hour	== g_h ) 
	{
		month_time = 0;
		return MONTH_MEETING;
	}
	else if( !month_time &&												//		시간 전에 공고
			g_GodTable[ch->believe_god].meet_month_day	== g_d &&
			g_GodTable[ch->believe_god].meet_month_hour+1	== g_h ) 
	{
//		month_time = 1;
		SendCallNoteMeeting( MONTH_MEETING, ch );
	}

	if( g_GodTable[ch->believe_god].meet_day_hour == g_h )	return DAY_MEETING;
	////////////////////////////////////////////////////////////////////

	/////////////// 예배 시간에 참석한 사람은 게임 시간 1시간 후에 판단하여 신앙심을 올려준다.

	if( ch->is_god_meeting == YEAR_MEETING )
	if( g_GodTable[ch->believe_god].meet_year_month	< g_mo+1 ||
		g_GodTable[ch->believe_god].meet_year_day < g_d+1 || 
		(	g_GodTable[ch->believe_god].meet_year_month	== g_mo+1 &&
			g_GodTable[ch->believe_god].meet_year_day	== g_d+1 &&
			g_GodTable[ch->believe_god].meet_year_hour+1 <= g_h ) )
	{

		if( year_time == false )
		{
			if( (g_curr_time - ch->god_meeting_start_time) >= 60 || ch->i_go_year_meeting )
			{
				ch->faith += 100;
				ch->god_cast_level ++;
				CallClient( ch->GetServerID(), CMD_DIVINE_UP );
			}
			else 
			{
				ch->faith -= 30;
				CallClient( ch->GetServerID(), CMD_DIVINE_DOWN );
			}

			ch->SendCharInfoBasic(FAITH, ch->faith );////020704 lsw
		}

		ch->i_go_year_meeting = false;
		year_time = true;
		return 0;
	}

	if( ch->is_god_meeting == MONTH_MEETING )
	if( g_GodTable[ch->believe_god].meet_year_month	< g_mo+1 ||
		g_GodTable[ch->believe_god].meet_year_day < g_d+1 || 
		( g_GodTable[ch->believe_god].meet_month_day == g_d+1 &&
		  g_GodTable[ch->believe_god].meet_month_hour+1 <= g_h ) )
	{
		if( month_time == false )
		{
			if( ch->i_go_month_meeting ) 
			{
				ch->faith += 20;
				ch->god_cast_level ++;		// 일단 예배 내용은 하나밖에 없으므로...
				CallClient( ch->GetServerID(), CMD_DIVINE_UP );
			}
			else  
			{
				ch->faith -= 10;
				CallClient( ch->GetServerID(), CMD_DIVINE_DOWN );
			}
			ch->SendCharInfoBasic(FAITH, ch->faith );//020704 lsw
		}

		ch->i_go_month_meeting = false;
		month_time = true;
		return 0;
	}

	if( ch->is_god_meeting == DAY_MEETING )
	if( g_GodTable[ch->believe_god].meet_year_month	< g_mo+1 ||
		g_GodTable[ch->believe_god].meet_year_day < g_d+1 || 
		g_GodTable[ch->believe_god].meet_day_hour+1	<= g_h )
	{
		if( day_time == false ) 
			if( ch->i_go_day_meeting ) ch->faith += 5;

		ch->i_go_day_meeting = false;
		day_time = true;
		return 0;
	}
	///////////////////////////////////////////////////////////////////////

//	year_time = false;
//	month_time = false;
//	day_time = false;
	return 0;
}

void SendServerMessage( int type, char *szTemp, short int cn )
{
	if( !szTemp ) return;
	t_packet p;
	int len=strlen( szTemp );
	int max = (len / 512)+1;
	
	p.h.header.type = type;
	for( int i=0; i<max; i++ )
	{
		char *temp = szTemp+512*i;
		strncpy( p.u.kein.server_text_default.text, temp ,512 );
		p.u.kein.server_text_default.len = len;
		
		p.h.header.size = sizeof( k_server_text_default );
		QueuePacket(connections, cn, &p, 1);
	}
}

void SendLoginServerForCharUpdate( short int cn )		// 캐릭터 정보를 업데이트한다.
{
	CHARLIST *ch = &connections[cn].chrlst;
	if( !ch ) return;

	t_packet p;
	p.h.header.type = CMD_CHAR_UPDATE_DATA_BY_KEIN;
	{
		sprintf( p.u.kein.char_update_data.name, "%s", ch->Name );
		p.u.kein.char_update_data.believe_god		= ch->believe_god;
		p.u.kein.char_update_data.faith				= ch->faith;
		p.u.kein.char_update_data.god_cast_level	= ch->god_cast_level;
	}
	p.h.header.size = sizeof( k_char_update_data );
	QueuePacket(connections, DB_DEMON, &p, 1);
}

void  RecvLoginServerGetCharInfoByKein( k_get_char_info *p )
{
	CHARLIST *ch = &connections[p->server_id].chrlst;
	if( strcmp( p->name, connections[p->server_id].name ) != 0 ) return;	// 030923 HK YGI

	ch->believe_god			= p->believe_god;
	ch->faith				= p->faith;
	ch->god_cast_level		= p->god_cast_level;

	t_packet packet;
	packet.h.header.type = CMD_GET_CHARINFO_BY_KEIN;
	{
		packet.u.kein.server_to_client_char_info_plus.believe_god = p->believe_god;
		packet.u.kein.server_to_client_char_info_plus.faith = p->faith;
	}
	packet.h.header.size = sizeof( k_server_to_client_char_info_plus );

	if( p->server_id > DB_DEMON )
		QueuePacket(connections, p->server_id, &packet, 1);
}

void SendReqGetCharInfoEtc( short int server_id, char *name )
{
	t_packet p;

	p.h.header.type = CMD_GET_CHARINFO_BY_KEIN;
	{
		p.u.kein.req_chr_info_etc.server_id	= server_id;
		sprintf( p.u.kein.req_chr_info_etc.name, "%s", name );
	}
	p.h.header.size = sizeof( k_req_chr_info_etc );
	QueuePacket(connections, DB_DEMON, &p, 1);
}

void SendServerKillPc( short int cn )		// 0414 KHS
{
	return;
	if( rand()%3 == 0 )  return;

	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	t_packet packet;
	packet.h.header.type = CMD_SERVER_KILLPC;
	{
		packet.u.kein.server_killpc.id		=  cn;
		packet.u.kein.server_killpc.killpc  =  ch->killpc;
	}
	packet.h.header.size = sizeof(k_server_killpc);

	QueuePacket(connections, cn, &packet, 1);
}

void RecvCanMemorize( t_connection c[], short int cn )
{
	CHARLIST *ch = &c[cn].chrlst;
	if( !ch ) return;

	if( g_hour >= 6 && g_hour < 13 ) CallClient( cn, CMD_CAN_MEMORIZE_OK );
	else
	{
		if( (g_curr_time - ch->can_memorize_time) <= 1200 ) CallClient( cn, CMD_CAN_MEMORIZE_OK );
		else CallClient( cn, CMD_CAN_MEMORIZE_NOT );
	}	
}

void SendEarMessage( char *name, char *message, short int who, int send_game_server, int send_server_id )		// 누군가에게 귓속말을 보낸다.
{
	// 당사자 클라이언트로 메시지를 보내준다.
	t_packet p;
	p.h.header.type = CMD_SEND_EAR_MESSAGE;
	{
		strcpy( p.u.kein.send_ear_message.name, name );
		strcpy( p.u.kein.send_ear_message.message, message);
		p.u.kein.send_ear_message.send_game_server = send_game_server;		// 보낸 사람이 속한 게임 서버 포트번호
		p.u.kein.send_ear_message.send_server_id = send_server_id;			// 보낸 사람의 아이디
	}
	p.h.header.size = sizeof( k_send_ear_message )-200+strlen( message );
	QueuePacket(connections, who, &p, 1);
}

void SendEarMessageResult( int type, char *name, short int cn )
{
	t_packet p;
	p.h.header.type = type;
	{
		strcpy( p.u.kein.send_ear_message_ok.name, name );
	}
	p.h.header.size = sizeof( k_send_ear_message_ok )-20+strlen( name )+1;
	QueuePacket(connections, cn, &p, 1);
}

void SendEarMessageResult2( int type, int ret, short int cn )
{
	t_packet p;
	p.h.header.type = type;
	{
		p.u.kein.default_char = (char )ret;
	}
	p.h.header.size = sizeof( char );
	QueuePacket(connections, cn, &p, 1);
}

inline int GetOwnPort( )
{
	SERVER_DATA *pData = g_pServerTable->GetOwnServerData();
	return pData->wPort;
}

void RecvEarMessage( char *name, char *message, short int cn )		// 1번 
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	ConvString( message );

	int recv_cn = ExistHe( name );
	if( recv_cn > DB_DEMON )
	{
		CHARLIST *pTarget = CheckServerId( recv_cn );
		if(!pTarget){return;}

		if( ch->name_status.nation != pTarget->name_status.nation ) 
		{
			if(!GMMgr.IsMerchant(ch) 
			&& !GMMgr.IsMerchant(pTarget)
			&& !ch->IsCounselor()
			&& !pTarget->IsCounselor())//030114 lsw
			{
				SendEarMessageResult2( CMD_SEND_EAR_MESSAGE_RESULT_ETC, -1, cn);		// 다른 나라 사람이다.
				return;
			}
		}

		SendEarMessage( ch->Name, message, recv_cn, -1, cn );		// 누군가에게 귓속말을 보낸다.
		SendEarMessageResult( CMD_SEND_EAR_MESSAGE_OK, name, cn);
		return;
	}
	
	t_packet p;
	p.h.header.type = CMD_SEND_EAR_MESSAGE_OTHER;				// 다른 맵에 있는지 체크
	{
		p.u.kein.login_to_gameserver_ear_message.game_server = GetOwnPort();
		p.u.kein.login_to_gameserver_ear_message.server_id = cn;
		if(GMMgr.IsMerchant(ch) || ch->IsCounselor())//030129 lsw
		{
			p.u.kein.login_to_gameserver_ear_message.nation = 0;//상인은 국가가 없고 메세지 받을때 검사도 안해
		}
		else
		{
			p.u.kein.login_to_gameserver_ear_message.nation = ch->name_status.nation;
		}
		strcpy( p.u.kein.login_to_gameserver_ear_message.recv_name, name );
		strcpy( p.u.kein.login_to_gameserver_ear_message.send_name, ch->Name );
		strcpy( p.u.kein.login_to_gameserver_ear_message.message, message );
	}
	p.h.header.size = sizeof( k_login_to_gameserver_ear_message )-200+strlen( message );


	// Modified by chan78 at 2000/12/01 BroadCast It to MapServers.
	g_pServerTable->BroadCastToEveryServer( (char*)&p, (sizeof(t_header)+p.h.header.size), SERVER_TYPE_MAP );

	CheckEarChatingData( ch->Name, name, message );
	//	SendEarMessageResult( CMD_SEND_EAR_MESSAGE_NOT, name, cn);
}

void SendEarMessageReturnResult( short int gameserver_port, short int server_id, char *name, int ret  )	// ret 1: 잘 받음, 2: 죽음, 3:거절함
{
	t_packet p;
	p.h.header.type = CMD_SEND_EAR_MESSAGE_OK;
	{
		p.u.kein.send_ear_message_ok_to_login.gameserver_port = gameserver_port;
		p.u.kein.send_ear_message_ok_to_login.server_id	= server_id;
		strcpy( p.u.kein.send_ear_message_ok_to_login.name, name );
		p.u.kein.send_ear_message_ok_to_login.ret = ret;
	}
	p.h.header.size = sizeof( k_send_ear_message_ok_to_login );

	// Modified by chan78 at 2000/12/01 BroadCast It to MapServers.
	g_pServerTable->BroadCastToEveryServer( (char*)&p, (sizeof(t_header)+p.h.header.size), SERVER_TYPE_MAP );
}

void FindWhoEarMessageTarget( k_login_to_gameserver_ear_message *p )// 귀속말을 받을 사람이 이 게임 서버에 존재하는지 판단
{
	const int who = ExistHe( p->recv_name );
	CHARLIST *ch = CheckServerId( who );
	if( !ch ) return;

	if(	!GMMgr.IsMerchant(ch)// he is not Merchant
	&&	!ch->IsCounselor()//He is counselor? Can Recv EarMessage
	&&	p->nation
	&&	ch->name_status.nation 
	&&	p->nation != ch->name_status.nation ) // 국가가 틀리다	// 메시지 출력
	{
		k_send_ear_message_result ret;
		ret.ret = 4;
		ret.send_game_server = p->game_server;
		ret.send_server_id = p->server_id;
		SendLoginEarMessageResult( &ret, -1 );			// 죽어 있다
		return;		
	}

	if( ch->bAlive == TRUE )
	{			
		SendEarMessage( p->send_name, p->message, who, p->game_server, p->server_id );		// 클라이언트에서 결과를 받아온다. 
	}
	else 
	{
		//k_send_ear_message_result 
		k_send_ear_message_result ret;
		ret.ret = 2;
		ret.send_game_server = p->game_server;
		ret.send_server_id = p->server_id;
		SendLoginEarMessageResult( &ret, -1 );			// 죽어 있다
	}
}

void SendLoginEarMessageResult( k_send_ear_message_result *t, short int cn )		
{
	// 클라이언트에서 받은 귓속말 결과 처리 와 다른 서버에서 온 결과물( 제대로 전달이 안됐을경우 )

	if( t->send_server_id == -1 )	// 010607 YGI
	{
		t->send_game_server = -1; 
		t->send_server_id = cn;
	}

	if( t->send_game_server == -1 )		// 같은 맵에 있을 경우
	{
		RecvLoginToGameEarMessageResult( t->send_server_id, t->ret );
		return;		
	}

	t_packet p;
	p.h.header.type = CMD_LOGIN2GAME_EAR_MESSAGE_RESULT;
	{
		p.u.kein.login2game_ear_message_result.send_server_id	= t->send_server_id;
		p.u.kein.login2game_ear_message_result.ret = t->ret;
	}
	int size = p.h.header.size = sizeof( k_login2game_ear_message_result );
	g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)t->send_game_server, (char *)&p, size+sizeof(t_header) );
	//QueuePacket(connections, LOGIN_SERVER_CONNECT, &p, 1);
}

void RecvLoginToGameEarMessageResult( short int server_id, char ret )		// 로그인서버에서 보내온 답장을 클라이언트로 보내준다.
{
	CHARLIST *ch = CheckServerId( server_id );
	if( !ch ) return;

	if( connections[server_id].dwAgentConnectionIndex )
	{
		t_packet p;
		p.h.header.type = CMD_GAME2CLIENT_EAR_RESULT;
		{
			p.u.kein.game2client_ear_result.ret = ret;
		}
		p.h.header.size = sizeof( k_game2client_ear_result );
		QueuePacket(connections, server_id, &p, 1);
	}
}

void SendGameToLoginExistName( char *name, short int cn )
{
	t_packet p;
	p.h.header.type = CMD_GAME2LOGIN_CHECK_NAME;
	{
		strcpy( p.u.kein.loginserver_to_gameserver_ear_message.name, name );
		p.u.kein.loginserver_to_gameserver_ear_message.server_id = cn;
	}
	p.h.header.size = sizeof( k_loginserver_to_gameserver_ear_message);

	// Modified by chan78 at 2000/12/01 BroadCast It to MapServers.
	g_pServerTable->BroadCastToEveryServer( (char*)&p, (sizeof(t_header)+p.h.header.size), SERVER_TYPE_MAP );
}


///////////////////////////////////////////////////////////////////////////
//
//			 여러가지들을 텍스트로 저장하는 함수들의 모임
//
////////////////////////////////////////////////////////////////////////////
void CheckChatingData( char *name, char *message )		// 채팅하는 말 저장 하기
{
	FILE *fp;
	char temp[MAX_PATH];
	sprintf( temp, ".\\logout_chat\\채팅_%s.txt", name);
	fp = fopen( temp, "at+" );
	if( !fp ) return;
	
	fprintf( fp, "%20s>> %s\n", name, message );
	fclose( fp );
}

int ChangePosType( int type )
{
	switch( type )
	{
		case CHARACTER	:	return PT_CHARACTER;
		case GROUND		:	return PT_GROUND;
		case USE		:	return PT_USE;
		default : return type;
	}
}

void CheckBackupItemForCharacter( CHARLIST *ch )
{
	char filename[FILENAME_MAX];
	wsprintf( filename, "./logout_item/%s_item.bin", ch->Name );
	FILE *fp = fopen( filename, "wb+" );
	if( fp )
	{
		fwrite( ch->inv, sizeof( ItemAttr ), 3*3*8, fp );
		fwrite( ch->equip, sizeof( ItemAttr ), 8, fp );
		fclose(fp );
	}
}

void CheckMoveItem( char *name, int source_type, int dest_type, ItemAttr item, DWORD money)
{
	return;

	dest_type = ChangePosType( dest_type );
	source_type = ChangePosType( source_type );

	t_packet p;
	p.h.header.type = CMD_GAME2LOGIN_MOVING_ITEM;
	{
		strcpy( p.u.kein.moving_item.name, name );
		p.u.kein.moving_item.source_type = source_type;
		p.u.kein.moving_item.dest_type = dest_type;
		p.u.kein.moving_item.item = item;
		p.u.kein.moving_item.money = money;
	}
	p.h.header.size = sizeof( k_moving_item );
	QueuePacket(connections, LOGIN_SERVER_CONNECT, &p, 1);	
}

void CheckEarChatingData( char *send_name, char *recv_name, char *message )		// 채팅하는 말 저장 하기
{
	FILE *fp;
	char temp[MAX_PATH];
	sprintf( temp, ".\\logout_ear\\귓말_%s.txt", send_name);
	fp = fopen( temp, "at+" );
	if( !fp ) return;
	
	fprintf( fp, "%20s>>%20s %s\n", send_name, recv_name, message );
	fclose( fp );
}



// 실제적인 Character Data를 다른 MapServer로 이동시키는 함수...
//////////////////////////////////////////////////////////////////////////
//	
//	맵간 이동 메소드 ( 받기 )  
// Rewrote by chan78 at 2000/11/25
//
//////////////////////////////////////////////////////////////////////////
bool NewSendGameToLoginChangeMapData( short int cn, WORD wMyAgentServerID, WORD wTargetServerID )		// 캐릭터 데이타 1
{
	CHARLIST* ch = CheckServerId(cn);
	
	if (!ch)
	{
		return false;
	}
	
	if(!::CanSaveUserData(ch,0)){return false;}

	if (ch->IsPlayer())
	{ // 맵이동을 하면 소환몬을 없앰
		connections[cn].chrlst.m_xSummon.Remove(KillMonster);
	}

	if (connections[cn].dwAgentConnectionIndex == 0)
	{
		return false;
	}

	char szMsg[sizeof(t_header) + sizeof(change_map_protocol) + 1];
	szMsg[0] = BYTE(PTCL_MAP_TO_MAP);
	//ReleaseItem( ch );			// 010521 YGI
	if (EXCHANGE_STATE_READY != ch->GetExchangeState())
	{	// 거래중이던걸 취소 시킨다.
		SendExchangeCancel(ch->GetExchangeTargetId(), cn);
	}

	if (ch->handheld.item_no)
	{
		MoveEmptyInv(&ch->handheld, ch);
		POS pos;
		SetItemPos(HAND, &pos);
		SendServerEachItem(&pos, &ch->handheld, cn);
	}
	
	t_packet* p = (t_packet*)(szMsg + 1);
	// change_map9
	// -------------------------------------------------------------------------------
	{
		p->u.change_map_protocol.mapdata9.game_port = wTargetServerID;
		memcpy(p->u.change_map_protocol.mapdata9.name,	connections[ch->GetServerID()].name, NM_LENGTH);
		p->u.change_map_protocol.mapdata9.Gender = ch->Gender;
		p->u.change_map_protocol.mapdata9.Face = ch->Face;
		p->u.change_map_protocol.mapdata9.Class	= ch->Class;
		p->u.change_map_protocol.mapdata9.Job =	ch->Job;
		p->u.change_map_protocol.mapdata9.Spell = ch->Spell;	
		p->u.change_map_protocol.mapdata9.SprNo = ch->SprNo;	
		p->u.change_map_protocol.mapdata9.Peacests = ch->Peacests;	
		p->u.change_map_protocol.mapdata9.BodyR	= ch->BodyR;	
		p->u.change_map_protocol.mapdata9.BodyG = ch->BodyG;	
		p->u.change_map_protocol.mapdata9.BodyB	= ch->BodyB;	
		p->u.change_map_protocol.mapdata9.ClothR = ch->ClothR;
		p->u.change_map_protocol.mapdata9.ClothG = ch->ClothG;
		p->u.change_map_protocol.mapdata9.ClothB = ch->ClothB;
		p->u.change_map_protocol.mapdata9.total_id = ch->total_id;
		p->u.change_map_protocol.mapdata9.nk3 =	ch->nk[N_VYSEUS];	
		p->u.change_map_protocol.mapdata9.nk4 =	ch->nk[N_ZYPERN];
		p->u.change_map_protocol.mapdata9.nk6 =	ch->nk[N_YILSE];
		// 기억된 이동 장소 정보 구하기
		ch->GetTransMap(p->u.change_map_protocol.mapdata9.nTransPort,
						p->u.change_map_protocol.mapdata9.nTransX,
						p->u.change_map_protocol.mapdata9.nTransY);
		p->u.change_map_protocol.mapdata9.JoinLocalWar = ch->JoinLocalWar;		// LTS LOCALWAR
		p->u.change_map_protocol.mapdata9.GainedFame = ch->GainedFame;
		p->u.change_map_protocol.mapdata9.CurrentPhase = ch->CurrentPhase;
		p->u.change_map_protocol.mapdata9.EventJoin = ch->EventJoin;		// 020115 LTS
		p->u.change_map_protocol.mapdata9.HorseNo = ch->HorseNo;			// LTS HORSERIDER
		p->u.change_map_protocol.mapdata9.HorseGrade = ch->HorseGrade;		// LTS HORSERIDER
		p->u.change_map_protocol.mapdata9.RiderNo = ch->RiderNo;			// LTS HORSERIDER
		p->u.change_map_protocol.mapdata9.RiderGrade = ch->RiderGrade;		// LTS HORSERIDER
		
		p->u.change_map_protocol.mapdata9.bIsTradeOn = ch->IsTradeOn();		//030127 lsw
		p->u.change_map_protocol.mapdata9.nSkillSadCount = ch->nSkillSadCount;		//030127 lsw
		p->u.change_map_protocol.mapdata9.nSkillBlessLeaf = ch->nSkillBlessLeaf;		//030127 lsw

		p->u.change_map_protocol.mapdata9.nBillingType = ch->GetBillingType();		//030127 lsw
		p->u.change_map_protocol.mapdata9.nUserAge	= ch->m_nUserAge;				// 030929 kyo	
	}

	DWORD id = ch->GetServerID();
	// change_map1
	// -------------------------------------------------------------------------------
	{
		p->u.change_map_protocol.mapdata1.game_port = wTargetServerID;

		t_update_char_db* tp = &p->u.change_map_protocol.mapdata1.ch_data;
		strcpy(tp->id, connections[id].id);
		strcpy(tp->name, connections[id].name);

		tp->Level = ch->GetLevel(); // CSD-030806
		tp->nGuildCode = ch->GetGuildCode(); // CSD-030324
		tp->social_status = ch->social_status;
		tp->fame = ch->fame;
		tp->NWCharacter	= ch->NWCharacter; // 010915 LTS	// mapdata9와 중복 

		tp->nk3 = ch->nk[N_VYSEUS];
		tp->nk4	= ch->nk[N_ZYPERN];
		tp->nk6	= ch->nk[N_YILSE];
		
		tp->Str	= ch->Str;
		tp->Con	= ch->Con;
		tp->Dex	= ch->Dex;
		tp->Wis	= ch->Wis;
		tp->Int	= ch->Int;
		tp->MoveP = ch->MoveP;
		tp->Char = ch->Char;
		tp->Endu = ch->Endu;
		tp->Moral = ch->Moral;
		tp->Luck = ch->Luck;
		tp->wsps = ch->wsps;

		tp->Money = ch->Money;
		tp->Tactics	= ch->Tactics;				// 선택한 전투기술 (보여주기 위함)
		memcpy(&tp->nation, &ch->name_status, sizeof(DWORD));		// 1004 YGI

		tp->Hp = ch->Hp;
		tp->HpMax = ch->HpMax ;
		tp->Mana = ch->Mana;					// 마법력
		tp->ManaMax	= ch->ManaMax;
		tp->Hungry = ch->Hungry;	
		tp->HungryMax = ch->HungryMax;

		tp->Condition = ch->GetState();
		memcpy(tp->MapName, ch->MapName, 20);
		tp->Sight = ch->Sight;
		tp->Age = ch->Age;
		tp->bAlive = ch->bAlive;					// 캐릭터의 생사여부(	REMOVE_:0 / ALIVE_:1 / DEAD_:2 / BUFE_:3)

		tp->killmon = ch->killmon;						// 1계열몬스터 킬링스
		tp->killanimal = ch->killanimal;				// 동물 킬링수
		tp->killpc = ch->killpc;						// 사람,NPC 킬링수
		//030227 lsw
		tp->nPoison = ch->GetBasicResist(RT_POISON); // 저주계열 마법에 대한 저항력
		tp->nCurse	= ch->GetBasicResist(RT_CURSE);	 //	저주계열 마법에 대한 저항력
		tp->nHoly	= ch->GetBasicResist(RT_HOLY);    // 신력계열 공격 마법에 대한 저항력
		tp->nFire	= ch->GetBasicResist(RT_FIRE);     //	불계열 공격 마법에 대한 저항력
		tp->nIce	= ch->GetBasicResist(RT_ICE);       //	얼음계열 공격 마법에 대한 저항력
		tp->nElect	= ch->GetBasicResist(RT_ELECT);   //	전격계열 공격 마법에 대한 저항력

		tp->X = ch->X;
		tp->Y = ch->Y;

		tp->accessory[0] = ch->accessory[0];
		tp->accessory[1] = ch->accessory[1];
		tp->accessory[2] = ch->accessory[2];
		tp->accessory[3] = ch->accessory[3];
		tp->mantle		 = ch->mantle; // 011018 KHS 망또
		
		tp->openhouse		= ch->openhouse;
		tp->reserved_point = ch->GetReservedPoint();
		tp->BankMoney		= ch->GetBankMoney();
		
		// 010406 YGI
		memcpy( &tp->win_defeat, &ch->WinLoseScore, sizeof( DWORD ) );
		tp->LadderScore		= ch->LadderScore;
		//tp->LastTotal		= ch->LastTotal;
		tp->LastLoan		= ch->LastLoan;
		//tp->LastLoan_time	= ch->LastLoan_time;

		tp->Exp				= ch->Exp;
		tp->viewtype		= ch->viewtype;
		ch->GetStepInfo(tp->aStepInfo, sizeof(tp->aStepInfo));
		tp->nCombatAmount = ch->Cp;
		//<050413_KCH 성전환 아이템때문에 추가.
		tp->gender			= ch->Gender;
		tp->sprno			= ch->SprNo;
		tp->face			= ch->Face;
		//>050413_KCH 성전환 아이템때문에 추가.
	}	

	// -------------------------------------------------------------------------------
	{
		t_server_update_binary_data0 *tp = &p->u.change_map_protocol.mapdata2.ch_data;
		p->u.change_map_protocol.mapdata2.game_port = wTargetServerID;
		memcpy(tp->name, connections[id].name, NM_LENGTH);
		memcpy(tp->id, connections[id].id,  ID_LENGTH );
		memcpy(tp->Ws,				(UCHAR *)ch->Ws,			SIZE_OF_WS );
		memcpy(tp->Ps,				(UCHAR *)ch->Ps,			SIZE_OF_PS );
		memcpy(tp->Skill,			(UCHAR *)ch->Skill,			SIZE_OF_SKILL );
		memcpy(tp->skillexp,		(UCHAR *)ch->skillexp,		SIZE_OF_SKILL_EXP );
		memcpy(tp->tac_skillEXP,	(UCHAR *)ch->tac_skillEXP,	SIZE_OF_TAC_SKILL_EXP );
	}
	// -------------------------------------------------------------------------------

	// -------------------------------------------------------------------------------
	{
		p->u.change_map_protocol.mapdata3.game_port = wTargetServerID;
		memcpy(p->u.change_map_protocol.mapdata3.ch_data.name, connections[id].name, NM_LENGTH);
		memcpy(p->u.change_map_protocol.mapdata3.ch_data.id, connections[id].id,  ID_LENGTH );
		memcpy(p->u.change_map_protocol.mapdata3.ch_data.script_var,			(UCHAR *)var[ id],			SIZE_OF_SCRIPT_VAR );
	}
	// -------------------------------------------------------------------------------
	
	// -------------------------------------------------------------------------------
	//////////////////////////// 0218 YGI //////////////////////
	{
		EndsetPartyMenberForUpdate( ch );		
		p->h.header.type = CMD_CHANGE_MAP_DATA4;	
		p->u.change_map_protocol.mapdata4.game_port = wTargetServerID;
		memcpy(p->u.change_map_protocol.mapdata4.ch_data.name, connections[id].name, NM_LENGTH);
		memcpy(p->u.change_map_protocol.mapdata4.ch_data.id, connections[id].id,  ID_LENGTH );
		memcpy(p->u.change_map_protocol.mapdata4.ch_data.equip,			(UCHAR *)ch->equip,			SIZE_OF_EQUIP);
		memcpy(p->u.change_map_protocol.mapdata4.ch_data.quick,			(UCHAR *)ch->quick,			SIZE_OF_QUICK);
		memcpy(p->u.change_map_protocol.mapdata4.ch_data.party,			(UCHAR *)ch->party_str,			SIZE_OF_PARTY );
		memcpy(p->u.change_map_protocol.mapdata4.ch_data.relation,			(UCHAR *)ch->relation_str,		SIZE_OF_RELATION);
		memcpy(p->u.change_map_protocol.mapdata4.ch_data.employment,		(UCHAR *)ch->employment_str,	SIZE_OF_EMPLOYMENT);
	}
	// -------------------------------------------------------------------------------
	

	// -------------------------------------------------------------------------------
	{
		p->u.change_map_protocol.mapdata5.game_port = wTargetServerID;
		memcpy(p->u.change_map_protocol.mapdata5.ch_data.name, connections[id].name, NM_LENGTH);
		memcpy(p->u.change_map_protocol.mapdata5.ch_data.id,   connections[id].id,  ID_LENGTH );
		memcpy(p->u.change_map_protocol.mapdata5.ch_data.inv,			(UCHAR *)ch->inv,				SIZE_OF_INV );
	}
	// -------------------------------------------------------------------------------

	// -------------------------------------------------------------------------------
	{
		p->u.change_map_protocol.mapdata6.game_port = wTargetServerID;
		memcpy(p->u.change_map_protocol.mapdata6.ch_data.name, connections[id].name, NM_LENGTH);
		memcpy(p->u.change_map_protocol.mapdata6.ch_data.id,   connections[id].id,  ID_LENGTH );
		memcpy(p->u.change_map_protocol.mapdata6.ch_data.Item,			(UCHAR *)ch->Item,			SIZE_OF_ITEMINDEX );
	}
	// -------------------------------------------------------------------------------

	// -------------------------------------------------------------------------------
	{
		p->u.change_map_protocol.mapdata7.game_port = wTargetServerID;
		memcpy(p->u.change_map_protocol.mapdata7.ch_data.name,	connections[id].name, NM_LENGTH);
		memcpy(p->u.change_map_protocol.mapdata7.ch_data.id,		connections[id].id,  ID_LENGTH );
		memcpy(p->u.change_map_protocol.mapdata7.ch_data.bankitem,		(UCHAR *)ch->bank,		SIZE_OF_BANKITEM );
	}
	// -------------------------------------------------------------------------------

	// -------------------------------------------------------------------------------
	{
		p->u.change_map_protocol.mapdata8.game_port = wTargetServerID;
		memcpy(p->u.change_map_protocol.mapdata8.ch_data.name,	connections[id].name, NM_LENGTH);

		p->u.change_map_protocol.mapdata8.ch_data.believe_god	= ch->believe_god;
		p->u.change_map_protocol.mapdata8.ch_data.faith			= ch->faith;
		p->u.change_map_protocol.mapdata8.ch_data.god_cast_level = ch->god_cast_level;
	}
	// -------------------------------------------------------------------------------
	// Build MAP_CHANGE_REQUEST packet
	p->h.header.type = CMD_MAP_CHANGE_REQUEST;
	p->h.header.size = sizeof(t_change_map_protocol);
	p->h.header.crc = 0;
	p->u.change_map_protocol.wAgentServerID = wMyAgentServerID;
	p->u.change_map_protocol.dwIDForAgent = connections[cn].dwIDForAgent;
	p->u.change_map_protocol.FromServerCn = cn;

	memcpy(&p->u.change_map_protocol.id, connections[cn].id, ID_LENGTH);
	memcpy(&p->u.change_map_protocol.password, connections[cn].pw, PW_LENGTH );
	memcpy(&p->u.change_map_protocol.name, connections[cn].name, NM_LENGTH );
	memcpy(&p->u.change_map_protocol.mapname, connections[cn].mapname, NM_LENGTH);
	memcpy(&p->u.change_map_protocol.ip, connections[cn].ip_address, NM_LENGTH);
	//EquipItem( ch );		// 010525 YGI
	// Write it.
	if (!g_pServerTable->Send(wTargetServerID, szMsg, sizeof(t_header) + p->h.header.size + 1))
	{
		MyLog(LOG_FATAL, "NewSendGameToLoginChangeMapData() :: Can't send REQUEST to Server(ID:%d)", wTargetServerID);
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//	
//	맵간 이동 메소드 ( 받기 )  
//  2000/11/25, chan78
//
//////////////////////////////////////////////////////////////////////////
void NewRecvChangeMapData1( k_change_map1 *p, DWORD cn )		// 캐릭터 데이타 1
{
	CHARLIST *ch = &connections[cn].chrlst;
	if( !ch ) return;

	t_update_char_db *data1 = &p->ch_data;

	ch->SetLevel(data1->Level); // CSD-030806
	ch->SetGuildCode(data1->nGuildCode); // CSD-030806
	ch->social_status	= data1->social_status;
	ch->fame			= data1->fame;
	ch->NWCharacter		= data1->NWCharacter;		// 010915 LTS

	ch->nk[ N_VYSEUS]	= data1->nk3;
	ch->nk[ N_ZYPERN]	= data1->nk4;
	ch->nk[ N_YILSE]	= data1->nk6;

	ch->Str  				= data1->Str;
	ch->Con  				= data1->Con;
	ch->Dex  				= data1->Dex;
	ch->Wis  				= data1->Wis;
	ch->Int  				= data1->Int;
	ch->MoveP				= data1->MoveP;
	ch->Char 				= data1->Char;
	ch->Endu 				= data1->Endu;
	ch->Moral				= data1->Moral;
	ch->Luck 				= data1->Luck;
	ch->wsps 				= data1->wsps;

	ch->Tactics 		= data1->Tactics;				// 선택한 전투기술 (보여주기 위함)
	memcpy( &ch->name_status, &data1->nation, sizeof( DWORD ) );		// 1004 YGI
	ch->Money			= data1->Money;
	ch->Hp				= data1->Hp;
	ch->HpMax			= data1->HpMax ;
	ch->Mana			= data1->Mana;					// 마법력//020214 lsw
	ch->ManaMax			= data1->ManaMax;
    ch->SetState(data1->Condition);
	memcpy( ch->MapName, MapName, 20 );
	ch->Sight = data1->Sight;
	ch->Age = data1->Age;
	ch->bAlive		= data1->bAlive;					// 캐릭터의 생사여부(	REMOVE_:0 / ALIVE_:1 / DEAD_:2 / BUFE_:3)

	ch->Hungry = data1->Hungry;	
	ch->HungryMax = data1->HungryMax;
	ch->killmon = data1->killmon;						// 1계열몬스터 킬링스
	ch->killanimal = data1->killanimal;				// 동물 킬링수
	ch->killpc = data1->killpc;						// 사람,NPC 킬링수

	ch->SetBasicResist(RT_POISON,70); // 저주계열 마법에 대한 저항력//030227 lsw
	ch->SetBasicResist(RT_CURSE,70); //	저주계열 마법에 대한 저항력
	ch->SetBasicResist(RT_HOLY,70); // 신력계열 공격 마법에 대한 저항력
	ch->SetBasicResist(RT_FIRE,70); //	불계열 공격 마법에 대한 저항력
	ch->SetBasicResist(RT_ICE,70); //	얼음계열 공격 마법에 대한 저항력
	ch->SetBasicResist(RT_ELECT,70); //	전격계열 공격 마법에 대한 저항력

	ch->X = data1->X;
	ch->Y = data1->Y;

	ch->accessory[0] = data1->accessory[0];
	ch->accessory[1] = data1->accessory[1];
	ch->accessory[2] = data1->accessory[2];
	ch->accessory[3] = data1->accessory[3];
	ch->mantle		 = data1->mantle; // 011018 KHS 망또 
		
	ch->openhouse		= data1->openhouse;
	ch->SetReservedPoint(data1->reserved_point);
	ch->SetBankMoney(data1->BankMoney, BMCT_RECV_MAP_CHANGE); // CSD-030723
	// 010406 YGI
	memcpy( &ch->WinLoseScore, &data1->win_defeat, sizeof( DWORD ) );
	ch->LadderScore		= data1->LadderScore;
	//ch->LastTotal		= data1->LastTotal;
	ch->LastLoan		= data1->LastLoan;
	//ch->LastLoan_time	= data1->LastLoan_time;

	ch->Exp				= data1->Exp;
	ch->viewtype		= data1->viewtype;
    ch->SetStepInfo(data1->aStepInfo, sizeof(data1->aStepInfo));
    ch->Cp = data1->nCombatAmount;

	//<050413_KCH 성전환 아이템때문에 추가.
	ch->Gender			= data1->gender;
	ch->SprNo			= data1->sprno;
	ch->Face			= data1->face;
	//>050413_KCH 성전환 아이템때문에 추가.

	ch->change_map_confirm[0] = 1;		// 1번째 데이타 받았다
}

void NewRecvChangeMapData2( k_change_map2 *p, DWORD cn )		// 캐릭터 데이타 2 받기
{
	CHARLIST *ch = &connections[cn].chrlst;
	if( !ch ) return;

		memcpy(ch->Ws,			p->ch_data.Ws,			SIZE_OF_WS );
		memcpy(ch->Ps,			p->ch_data.Ps,			SIZE_OF_PS );
		memcpy(ch->Skill,		p->ch_data.Skill,		SIZE_OF_SKILL );
		memcpy(ch->skillexp,	p->ch_data.skillexp,	SIZE_OF_SKILL_EXP );
		memcpy(ch->tac_skillEXP,p->ch_data.tac_skillEXP, SIZE_OF_TAC_SKILL_EXP );

	ch->change_map_confirm[1] = 1;		// 2번째 데이타 받았다
}

void NewRecvChangeMapData3( k_change_map3 *p, DWORD cn )		// 캐릭터 데이타 3 받기
{
	CHARLIST *ch = &connections[cn].chrlst;
	if( !ch ) return;

	memcpy(var[cn], p->ch_data.script_var,	SIZE_OF_SCRIPT_VAR );

	ch->change_map_confirm[2] = 1;		// 3번째 데이타 받았다
}

void NewRecvChangeMapData4( k_change_map4 *p, DWORD cn )		// 캐릭터 데이타 4 받기
{
	CHARLIST *ch = &connections[cn].chrlst;
	if( !ch ) return;

		memcpy(ch->equip,			p->ch_data.equip,			SIZE_OF_EQUIP);
		memcpy(ch->quick,			p->ch_data.quick,			SIZE_OF_QUICK);
		memcpy(ch->party_str,		p->ch_data.party,			SIZE_OF_PARTY );
		memcpy(ch->relation_str,	p->ch_data.relation,		SIZE_OF_RELATION);
		memcpy(ch->employment_str,	p->ch_data.employment,		SIZE_OF_EMPLOYMENT);

	// 011030 KHS JJING
	// 맵이동했을때 JJING에 대해 Setting한다. 
	ch->jjing = ((RareMain*)&(ch->equip[ WT_ARMOR].attr[ IATTR_RARE_MAIN]))->grade;

	InitPartyMemberServerToServer( ch );

	ch->change_map_confirm[3] = 1;		// 4번째 데이타 받았다
}

void NewRecvChangeMapData5( k_change_map5 *p, DWORD cn )		// 캐릭터 데이타 5 받기
{
	CHARLIST *ch = &connections[cn].chrlst;
	if( !ch ) return;

	memcpy(ch->inv, p->ch_data.inv, SIZE_OF_INV );

	ch->change_map_confirm[4] = 1;		// 5번째 데이타 받았다
}

void NewRecvChangeMapData6( k_change_map6 *p, DWORD cn )		// 캐릭터 데이타 6 받기
{
	CHARLIST *ch = &connections[cn].chrlst;
	if( !ch ) return;

	memcpy(ch->Item, p->ch_data.Item, SIZE_OF_ITEMINDEX );

	ch->change_map_confirm[5] = 1;		// 6번째 데이타 받았다
}

void NewRecvChangeMapData7( k_change_map7 *p, DWORD cn )		// 캐릭터 데이타 7 받기
{
	CHARLIST *ch = &connections[cn].chrlst;
	if( !ch ) return;
	
	memcpy(ch->bank, p->ch_data.bankitem, SIZE_OF_BANKITEM );

	int i = 0;
	for(; i < 1000 ; i ++)
	{
		if( ch->Item[i] == 0) break;
	}
	ch->ItemMax = i;
	//010607 lsw
	//	CheckCurrStats( ch );		// 0206 YGI

	ch->change_map_confirm[6] = 1;		// 7번째 데이타 받았다
}

void NewRecvChangeMapData8( k_change_map8 *p, DWORD cn )		// 캐릭터 데이타 8 받기
{
	CHARLIST *ch = &connections[cn].chrlst;
	if( !ch ) return;

		ch->believe_god = p->ch_data.believe_god;
		ch->faith  = p->ch_data.faith;
		ch->god_cast_level = p->ch_data.god_cast_level;

	ch->change_map_confirm[7] = 1;		// 8번째 데이타 받았다
	ch->now_changing_map = 0;

	CalcCharAbilityByChangemap( ch );					// 캐릭터 초기 셋팅
	CallClient( (short int )cn, (short int )CMD_ACCEPT_JOIN_KEIN );				// 모두 받았다.
	memset( ch->change_map_confirm, 0, sizeof( char ) * 9 );
}

void NewRecvChangeMapData9( k_change_map9 *p, DWORD cn )		// 캐릭터 데이타 8 받기
{
	CHARLIST *ch = &connections[cn].chrlst;
	if( !ch ) return;

		ch->Gender			=	p->Gender;
		ch->Face			=	p->Face	;
		ch->Class			=	p->Class;
		ch->Job				=	p->Job	;
		ch->Spell			=	p->Spell;	
		ch->SprNo			=	p->SprNo	;	
		ch->Peacests			=	p->Peacests	;	
		ch->BodyR			=	p->BodyR	;	
		ch->BodyG			=	p->BodyG	;	
		ch->BodyB			=	p->BodyB	;	
		ch->ClothR			=	p->ClothR	;	
		ch->ClothG			=	p->ClothG	;		
		ch->ClothB			=	p->ClothB	;	

		ch->total_id		=	p->total_id	;		

		ch->nk[N_VYSEUS]		=	p->nk3		;	
		ch->nk[N_ZYPERN]		=	p->nk4		;		
		ch->nk[N_YILSE]			=	p->nk6		;		
    ch->SetTransMap(p->nTransPort, p->nTransX, p->nTransY);
	ch->change_map_confirm[8] = 1;		// 9번째 데이타 받았다
	ch->JoinLocalWar		=	p->JoinLocalWar;			// LTS LOCALWAR
	ch->GainedFame			=	p->GainedFame;
	ch->CurrentPhase		=	p->CurrentPhase;
	ch->EventJoin			=	p->EventJoin;				// 020115 LTS

	ch->HorseNo				=	p->HorseNo;					// LTS HORSERIDER
	ch->HorseGrade			=	p->HorseGrade;				// LTS HORSERIDER
	ch->RiderNo				=	p->RiderNo;					// LTS HORSERIDER
	ch->RiderGrade			=	p->RiderGrade;				// LTS HORSERIDER

	ch->SetTradeMode(!p->bIsTradeOn);//030127 lsw		
	ch->nSkillSadCount	= 	p->nSkillSadCount;	//030127 lsw
	ch->nSkillBlessLeaf	=	p->nSkillBlessLeaf;	//030127 lsw
	
	ch->SetBillingType(p->nBillingType);
	ch->m_nUserAge	= p->nUserAge;				// 030929 kyo
}
								
// 040910 YGI								
void closeconnection_kein(t_connection c[], int cn, int errnum)
{								
	CHARLIST *ch = &connections[cn].chrlst;

	if( cn == LOGIN_SERVER_CONNECT || cn == DB_DEMON)
	{
		return;
	}

	if( cn < DRAGON_CONNECTIONS_START || cn >= DRAGON_MAX_CONNECTIONS ) 
	{
		return ;
	}

	QTickData[cn].Func = 0;

	if (ch->IsPlayer())
	{	// 소환된 몬스터가 있다면 전부 제거
		ch->m_xSummon.Remove(KillMonster);

		if (ch->DragonLordWarTeam>0)					// LTS DRAGONLORDWAR
		{
			g_pDragonLordWar.FindAndDeleteMember(cn);
		}
	}

	
	SendExitGameWho( cn );		// 파티원에게 나 나간다고 알린다. // 0116 YGI
	SetArea(REMOVE_PC_AREA, cn);
	ResetManToManItemWhenCloseConnect(ch);		// 0911 YGI
	if( ch->fight_flag )
	{
		CHARLIST *win = CheckServerId( ch->fight_id );
		if( win )
		{
			EndManToManFight( win, ch );
		}
	}
	if( g_pDungeonMap ) 
	{
		g_pDungeonMap->CloseConnect( cn );		// 020701 YGI
	}
	::SetTileOccupied(ch->MoveSx,ch->MoveSy,false);


	for( int i = 0 ; i < MAX_PC_CONTROL_NPC ; i ++)						// LTS NPC BUG
	{
		if( ch->ctrlnpc[i] != -1 )
		{
			NPCList[ ch->ctrlnpc[i]].ctrlpc = -1;
			SetArea(ADD_NPC_AREA, ch->ctrlnpc[i]);	
		}
	}

	//DeleteSquadMember(cn);  // Remove From Warfield Squad Hash;   // 010904 LTS




	g_pServerTable->SetNumOfUsers( g_pServerTable->GetNumOfUsers()-1,110, cn, errnum, connections[cn].name);


	// 040910 YGI
	//if( ch->updateWhenMapmove )	
	{
		updateBinaryDataToLoaginServer( connections, cn );		// YGI 040907
	}
	//050113_KCH_불안정한MapDown에대한 로그및 추적 -> 유저가 맵이동할때, 이전맵에서의 Connection을 끊는다는 의미로써 사용되어진 함수임.
	//MyLog( LOG_NORMAL, "closeconnection_kein cn:%d errnum:%d ChName:%s IP:%d",cn, errnum, connections[cn].name, connections[cn].ip_address);
	//050401_KCH

	//< CSD-HK-030829
	CleanUpConnection(cn);
}	//> CSD-HK-030829
								
int CalcCharAbilityByChangemap( CHARLIST *ch )
{			
	int px, py;
	ch->cur_skillno = 0;
	ch->itemweight		= GetTotalItemWeight( ch ); 
		
	if( ch->Peacests < 0 || ch->Peacests > 1 ) ch->Peacests = 0;

	// 혹시 위치에 장애물이 있으면 옆으로 옮긴다. 
	px = ch->X / TILE_SIZE;
	py = ch->Y / TILE_SIZE;
	NPC_AdjustPosition( ch->SprNo, &px, &py );
	ch->X = px * TILE_SIZE + 16;
	ch->Y = py * TILE_SIZE + 16;
		
	ch->OldMoveSx = ch->MoveSx = px;
	ch->OldMoveSy = ch->MoveSy = py;
	::SetTileOccupied(ch->MoveSx,ch->MoveSy,true);

	sRECALL_SCRP_NO = 0; // 이벤트가 발생하면 이값은 0이 아닌값으로 된다.  들어오면 다시 Setting..
		
	ch->timetoupdate	= g_curr_time;
	
	return 1;
}		
//021030 lsw 박스 아이템은 없어요		
void RecvChangeMapConfirm( short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	for( int i=0; i<9; i++ )
		if( !ch->change_map_confirm[i] ) 
		{
			CallClient( cn, CMD_CHANGE_MAP_FAIL);	// 무슨 문제가 있어 다 넘겨 받지 못했다.
			return;
		}
	
	CallClient( cn, CMD_ACCEPT_JOIN_KEIN );			// 다 넘겨 받았으니깐 계속 진행하라
	memset( ch->change_map_confirm, 0, sizeof( char ) * 9 );
}

////////////////////////////////////////
//
//	채팅 단어를 걸러내는 메소드들
//
///////////////////////////////////////
char FILTER[MAX_WORD][20];			// 단어집합
int filter_count;				// 모든 단어의 개수
int LoadFilterWord( )
{
	filter_count = 0;

	FILE *fp;
	char temp[ MAX_PATH];
	wsprintf( temp, "%s/data/badword.txt", GameServerDataPath );
	fp = fopen( temp, "rt" );
	if( fp == NULL ) return filter_count;

	while( fscanf(fp, "%s", FILTER[filter_count++] ) != EOF );
	
	fclose(fp);

	return filter_count;
}

void ConvString( char *szStr ) // 욕설 필터
{
	return;
	char *cut;
	char str_small[255] = {0,};		// 모두 대문자로 바꾼값...

//	int total_len = strlen( szStr );
//	str_small = new char[total_len];		// 가상으로 바꾼값으로 비교해서 처리하게끔하기 위해

	strcpy( str_small, szStr );
	strlwr( str_small );		// 소문자로 바꾼다.

	int i=0;
	while( *FILTER[i] && *FILTER[i] != ' ' )		// 빈칸이 아닌 문자가 있을때 까지만 
	{
		while( cut = strstr( str_small, FILTER[i] ) )
		{
			char *check_cut = cut;		// 한글의 처음인가 체크 루틴
			int count = 0;
			int ret = 0;
			while( 1 )
			{
				if( str_small >= check_cut )
				{
					if( !(count%2) ) ret = 1; // 한글의 처음이다...
					break;
				}
				if( !(*check_cut & 0x80) )
				{
					if( count%2 ) ret = 1; // 한글의 처음이다...
					break;
				}
				check_cut --;
				count++;
			}
			if( !ret ) 
			{
				*cut = ' ';
				continue;		// 한글의 처음이 아니다.
			}

			int len = strlen( FILTER[i] );
			int point_gab = cut - str_small;			// 주소의 차이
			for( int j=0; j<len; j++ )	
			{
				*(cut+j) = ' ';
				*(szStr+point_gab+j) = ' ';
			}
		}
		i++;
	}
//	delete str_small;
}



int CompPartyBoth( CHARLIST *ch_1, CHARLIST *ch_2 )
{
	for( int i=0; i<6 ; i++ )
	{
		if( !strcmp ( ch_1->party[i].Name, ch_2->Name ) )
		{
			for( int j=0; j<6; j++ )
			{
				if( !strcmp( ch_2->party[j].Name, ch_1->Name) ) return 1;
			}
		}
	}

	return 0;
}

//011022 lsw >
void SendSkillAll( short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	for( int i=0; i<45; i++ ) 
	{
		if( ch->skillexp[i].skillexp )
		{
			SendStartSkillExp( i, ch->skillexp[i], connections, cn );
		}
	}
}
//011022 lsw <

void RecvAndSendFaithUp( char *name, short int up_point )
{
	int cn = ExistHe( name );
	if( cn == -1 ) return ;
	CHARLIST *ch = CheckServerId( cn );
	if( ch )
	{
		ch->faith += up_point;
		ch->SendCharInfoBasic( FAITH, up_point );//020704 lsw
	}
}

void RecvDivineUp( short int cn )			// 기도문에 의한 마나 충족
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	if( ch->Spell == PRIEST_SPELL && ch->believe_god && ch->faith>20 && ch->god_cast_level )		// 50은 임의의 수
	{
		if( ch->Mana < (ch->ManaMax * 0.8) ) 
		{
			ch->Mana += ch->faith;
			if( ch->Mana > ch->ManaMax ) ch->Mana = ch->ManaMax;
			ch->SendCharInfoBasic(MP, ch->Mana ); //020704 lsw
		//	ch->TempMana = ch->Mana;//020214 lsw
		}
	}
}

int GetMeetMessageRandomNumber( int god_type, int meet_type )		// 예배 메시지 번호 가져오기 ( 내용있는 것중에서 랜덤으로 )
{
	int num=0;
	char *message = NULL;
	switch( meet_type )
	{
		case YEAR_MEETING  :	num = rand() % 4;
								message = YearMeetingText[god_type][num];
								break;
		case MONTH_MEETING :	num = rand() % 12;
								message = MonthMeetingText[god_type][num];
								break;
		default : return 0;
	}
	return num;	
}

void RecvOpenSupper( short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	if( !ch->in_temple ) 
	{
		SendServerResult( CM_OPEN_SUPPER, 10, cn );	// 신전안이 아니다
		return;
	}
	if( !ch->is_god_meeting )
	{
		SendServerResult( CM_OPEN_SUPPER, 20, cn );	// 음식 제공 시간이 아니다
		return;
	}
	if( ch->use_supper )
	{
		SendServerResult( CM_OPEN_SUPPER, 30, cn );	// 이미 제공 받았다.
		return;				
	}

	ch->use_supper = true;
	ch->much_supper = 10;		// 제공 받을 수 있는 수량

	t_packet p;
	p.h.header.type = CMD_OPEN_SUPPER;
	{
		p.u.kein.server_to_client_open_supper.much= ch->much_supper;
	}
	p.h.header.size = sizeof( k_server_to_client_open_supper);
	QueuePacket(connections, cn, &p, 1);
}

void RecvGetItemOfSupper( short int item_no, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	
	if( ch->much_supper >= 0)
	{

		int item_kind[] = { 3007, };
		int max = sizeof( item_kind ) / sizeof( int );
		int i = 0;
		for(; i<max; i++ )
		{
			if( item_no == item_kind[i] ) break;
		}
		if( i == max ) return;	// 허락되지 않는 아이템이다. 해킹막기

		ItemAttr item = GenerateItem( item_no );
		if( !item.item_no ) return;
		int a, b, c;
		if( SearchInv( ch->inv, a, b, c ) )
		{
			POS pos;
			SetItemPos( INV, a, b, c, &pos );
			ch->inv[a][b][c] = item;

			SendServerEachItem( &pos, &item, cn );
			ch->much_supper--;
		}
		else return;				// 인벤 창이 꽉 찼다.
	}
}

void CheckInitSkill( CHARLIST *ch )		 // 클레스 스킬은 확실하게...	// 어느정도 들어가면 빼야 하는 루틴...
{
  const int nCombatStart = LIGHTNING_BOOM - 100;
  const int nCombatEnd = WIND_EXTREME - 100;
	int i;
	switch( ch->Class ) 
	{
		case WARRIOR :	for(  i=0; i<MAX_SKILLMAIN;i++ )	
						{
							if( i >= TACTICS_Crapple && i <= TACTICS_Orison ) continue;
              if (i >= nCombatStart && i <= nCombatEnd)  continue;
							if( SkillTbl[i].Class_Warrior == 0 ) ch->Skill[i] = 0;
							else if( SkillTbl[i].Class_Warrior == 1 ) ch->Skill[i] = 1;
						}
						break;

		case THIEF :	for(  i=0; i<MAX_SKILLMAIN;i++ )
						{
							if( i >= TACTICS_Crapple && i <= TACTICS_Orison ) continue;
              if (i >= nCombatStart && i <= nCombatEnd)  continue;
							if( SkillTbl[i].Class_Thief == 1 ) ch->Skill[i] = 1;
							else if( SkillTbl[i].Class_Thief == 0 ) ch->Skill[i] = 0;
						}
						break;
						
		case ARCHER :	for(  i=0; i<MAX_SKILLMAIN;i++ )
						{
							if( i >= TACTICS_Crapple && i <= TACTICS_Orison ) continue;
              if (i >= nCombatStart && i <= nCombatEnd)  continue;
							if( SkillTbl[i].Class_Archery == 1 ) ch->Skill[i] = 1;
							else if( SkillTbl[i].Class_Archery == 0 ) ch->Skill[i] = 0;
						}
						break;
		case WIZARD :	for(  i=0; i<MAX_SKILLMAIN;i++ )
						{
							if( i >= TACTICS_Crapple && i <= TACTICS_Orison ) continue;
              if (i >= nCombatStart && i <= nCombatEnd)  continue;
							if( SkillTbl[i].Class_Wizard == 1 ) ch->Skill[i] = 1;
							else if( SkillTbl[i].Class_Wizard == 0 ) ch->Skill[i] = 0;
						}
						break;
		case PRIEST :	for(  i=0; i<MAX_SKILLMAIN;i++ ) 
						{
							if( i >= TACTICS_Crapple && i <= TACTICS_Orison ) continue;
              if (i >= nCombatStart && i <= nCombatEnd)  continue;
							if( SkillTbl[i].Class_Priest == 1 ) ch->Skill[i] = 1;
							else if( SkillTbl[i].Class_Priest == 0 ) ch->Skill[i] = 0;
						}
						break;
	}

	switch( ch->Job )		
	{
	case J_TAILOR		:	for(  i=0; i<MAX_SKILLMAIN;i++ ) if( SkillTbl[i].Select_Tailoring	== 1 )		{ ch->Skill[i] = 1; makemyjobexpsetting( i, ch ); }break;
	case J_ALCHEMIST	:	for(  i=0; i<MAX_SKILLMAIN;i++ ) 
							{
								if( i >= TACTICS_Crapple && i <= TACTICS_Orison ) continue;
                if (i >= nCombatStart && i <= nCombatEnd)  continue;
								if( SkillTbl[i].Select_Alchemy == 1 )
								{ 
									ch->Skill[i] = 1; 
									makemyjobexpsetting( i, ch ); 
								}
								else if( SkillTbl[i].Select_Alchemy == 0 )
								{
									ch->Skill[i] = 0;
									int kind = SkillTbl[i].MotherSkillType;
									//011012 lsw >
									ch->skillexp[kind].skillexp = 0;
									//011012 lsw <
								}
							}
							break;
	}

//011030 lsw >
	int ok = 0;
	for(  i=0; i<MAX_SKILLMAIN;i++ ) 
	{
		if( SkillTbl[i].Howto == 100 ) 
		{
		switch(i)
			{
			case 106	:	if(ch->skillexp[CUTDOWN].skillexp == 1000000
								&& ch->skillexp[CARPENTRY].skillexp == 1000000) ok = 1;
				break;
			case 107	:	if(ch->skillexp[MINING].skillexp == 1000000
								&& ch->skillexp[BLACKSMITHY].skillexp == 1000000) ok = 1;
				
				break;
			case 108		:	if(ch->skillexp[FARMING].skillexp == 1000000 ||
									ch->skillexp[FISHING].skillexp == 1000000)
									if(ch->skillexp[COOKING].skillexp == 1000000)  ok = 1;
				
				break;
			case 109		:	if(ch->skillexp[BUTCHERING].skillexp == 1000000
									&&ch->skillexp[TAILORING].skillexp == 1000000) ok = 1;
				
				break;
			case 110	:	if(ch->skillexp[CUTDOWN].skillexp == 1000000
								&&ch->skillexp[BOWCRAFTING].skillexp == 1000000) ok = 1;
				
				break;
			case 111	:	if(ch->skillexp[HERBDIGGING].skillexp == 1000000
								&&ch->skillexp[ALCHEMING].skillexp == 1000000) ok = 1;
				
				break;
			case 112	:	if(ch->skillexp[FISHING].skillexp == 1000000
								&&ch->skillexp[CANDLEMAKING].skillexp == 1000000) ok = 1;
				
				break;
			default:
				ok = 0;
				break;
			}
			if(ok)
			{
				ch->Skill[i] = 1;
				ok = 0;
			}
			else 
			{
				ch->Skill[i] = 0;
				ok = 0;
			}
		}
	}
	SendCharInfoSkill( connections, ch->GetServerID());

//011030 lsw <
}

CHARLIST* CheckNpcId(short int cn)
{	//< CSD-031014
	if (cn < NPC_LIST_START || cn > MAX_NPC_LIST)
	{
		return NULL;
	}

	return &NPCList[cn];
}	//> CSD-031014

CHARLIST* CheckNpcName(const char* pName)
{	//< CSD-031014
	for (int i = NPC_LIST_START; i < MAX_NPC_LIST; ++i)
	{
		if (NPCList[i].bAlive == ALIVE_ && strcmp(NPCList[i].Name, pName) == 0)
		{
			return &NPCList[i];
		}
	}

	return NULL;
}	//> CSD-031014

bool CheckServerIndex( short int id )
{
	if (id <= DB_DEMON || id >= DRAGON_MAX_CONNECTIONS)
	{
		return false;
	}

	return true;
}

void RecvReqCharInfo( int cn, int target )
{
	t_packet p;
	LPCHARLIST n;

	p.h.header.type = CMD_REQ_CHAR_INFO;
		p.u.debug_server_req_char_info.id = target;

		if( target >= 10000 )
		{
			n = CheckNpcId( target-10000 );
			if( !n ) return;
		}
		else 
		{
			n = CheckServerId( target );
			if( !n ) return;
		}

		p.u.kein.server_req_char_info.id = target;

//		p.u.kein.server_req_char_info.lv    = n->Level;
//		p.u.kein.server_req_char_info.exp   = n->Exp; 
//		p.u.kein.server_req_char_info.ac    = n->Ac;
//		p.u.kein.server_req_char_info.Str   = n->Str;					//  힘
//		p.u.kein.server_req_char_info.Con   = n->Con;				//  건강
//		p.u.kein.server_req_char_info.Dex   = n->Dex;				//	민첩성
//		p.u.kein.server_req_char_info.Wis   = n->Wis;				//	지혜
//		p.u.kein.server_req_char_info.Int   = n->Int;				//	지능
//		p.u.kein.server_req_char_info.MoveP = n->MoveP;				//	이동력
//		p.u.kein.server_req_char_info.Char  = n->Char;				//	매력
//		p.u.kein.server_req_char_info.Endu  = n->Endu;				//  저향력
//		p.u.kein.server_req_char_info.Moral = n->Moral;				//	사기
//		p.u.kein.server_req_char_info.Luck  = n->Luck;				//	행운
//		p.u.kein.server_req_char_info.wsps  = n->wsps;				//  마법력->마법 메모라이즈
		p.u.kein.server_req_char_info.Hp    = n->Hp;    	
		p.u.kein.server_req_char_info.HpMax = n->HpMax;
//		p.u.kein.server_req_char_info.Mp    = n->Mana;     
//		p.u.kein.server_req_char_info.MpMax = n->ManaMax;
//		p.u.kein.server_req_char_info.Hungry= n->Hungry; 
//		p.u.kein.server_req_char_info.HungryMax = n->HungryMax;
//		p.u.kein.server_req_char_info.event_no = n->eventno;			

	p.h.header.size = sizeof( k_server_req_char_info );
	QueuePacket( connections, cn, &p, 1 );
}


void RecvOhMyParty( char *my_name, char *party_name )			// my_name이 접속했을때 party_name에게 그 사실을 가르쳐준다.
{
	int he = ExistHe( party_name );
	if( he == -1 ) return;
	CHARLIST *party_ch = &connections[he].chrlst;
	if( !party_ch ) return;

	t_packet packet;
	packet.h.header.type = CMD_REQ_JOIN_GAME;
	{
		strcpy( packet.u.kein.client_char_name.name, my_name );
	}
	packet.h.header.size = sizeof( k_client_char_name );
	QueuePacket(connections, he, &packet, 1);
}

void SetShape( unsigned char *equip, short int cn )
{
	memset( equip, 0, 4 ); 
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	static int shape_index[] = { WT_ARMOR, WT_HELMET, WT_WEAPON, WT_SHIELD };
	for( int i=0; i<4; i++ )
	{
		CItem *tt = ItemUnit( ch->equip[shape_index[i]] );
		if( tt )
		{
			equip[i] = tt->GetItemShow( ch->Gender );
		}
	}
}

// 040602-3 YGI
void MoveEmptyInv( ItemAttr *item, CHARLIST *ch )		// 아이템을 비어 있는 곳에 옮기고 지운다.
{
	if( !ch ) return;
	int a, b, c;
	int ret = SearchInv( ch->inv, a, b, c );
	if( ret )
	{
		POS pos;
		SetItemPos( INV, a, b, c, &pos );
		ch->inv[a][b][c] = *item;
		SendServerEachItem( &pos, item, ch->GetServerID());
	}
	else		// 배낭에 물건이 꽉 찼다면 
	{
		if( GetAttr2( item->attr[IATTR_ATTR], IA2_ITEMMALL_ITEM ) )
		{ // 아이템 몰 아이템일 경우 다른 보통의 아이템을 대신 떨구고 그 자리에 넣어 준다.

			// 만약 교환중이였다면 취소 시킨다.	// 040901 YGI
			if (EXCHANGE_STATE_READY != ch->GetExchangeState())
			{
				SendExchangeCancel(ch->GetExchangeTargetId(), ch->GetServerID() );
			}

			for( int a=0; a<3; a++ )
			{
				for( int b=0; b<3; b++ )
				{
					for( int c=0; c<8; c++ )
					{
						if( !GetAttr2( ch->inv[a][b][c].attr[IATTR_ATTR], IA2_ITEMMALL_ITEM ) )
						{
							ItemAttr *pTempItem = &ch->inv[a][b][c];
							SendItemEventLog( pTempItem, ch->GetServerID(), 0, SILT_PUT_GROUND_MAX_INVEN, 1 ); //YGI acer
							SendItemDropMessage( ch->GetServerID(), pTempItem->item_no );
							DropItem( ch->X, ch->Y, pTempItem );	// 일단 일반 아이템을 땅바닥으로 떨어뜨린다.

							POS pos;
							SetItemPos( INV, a, b, c, &pos );
							ch->inv[a][b][c] = *item;
							SendServerEachItem( &pos, item, ch->GetServerID());
							goto END_FUNCTION;
						}
					}
				}
			}
		}

		SendItemEventLog( item, ch->GetServerID(), 0, SILT_PUT_GROUND_MAX_INVEN, 1 ); //YGI acer
		DropItem( ch->X, ch->Y, item );	// 땅바닥으로 떨어뜨린다.
		SendItemDropMessage( ch->GetServerID(), item->item_no );
	}
END_FUNCTION :
	memset( item, 0, sizeof( ItemAttr ) );
}

// 아이템 조인 테이블에서 아이템 번호를 체크하여 그 파라메터를 리턴한다.
int GetItemJoinNumber( int item_num )
{
	for( int i=0; i<ITEM_JOIN_MAX; i++ )
	{
		if( item_join[i].item_id == item_num ) return i;
	}
	return 0;
}

void SetTacSkillLevel( CHARLIST *ch )
{
	for( int i=0; i<= TACTICS_Orison - TACTICS_Crapple ; i++ )
	{
    ch->ConvertTacticsLevel(i);
	}
}


extern	int GetRider(CHARLIST* ch);	// LTS HORSERIDER
extern	int GetRiderGrade(CHARLIST* ch);	// LTS HORSERIDER
extern	int GetHorseGrade(CHARLIST* ch);	// LTS HORSERIDER

// 바꼈는지 확인해서 바꼈으면 보내준다.
void CheckAndSendChangeEquip( CHARLIST *ch, int send  )		// send가 1이면 무조건 보내준다.
{
	int cn = ch->GetServerID();
	if( !cn ) return;
	if( connections[cn].state == CONNECT_CHANGING_MAP ) return;

	CheckEquipItem( ch );		// 0720 YGI

	unsigned char equip[4];
	SetShape( equip, ch->GetServerID());

	int is_change = 0;

	for( int i=0; i<4; i++ )
		if( equip[i] != ch->accessory[i] ) 
		{
			is_change = true;
			break;
		}

	//011018 KHS 망또
	short int mantle = ch->equip[ WT_NECK].item_no;
	if( mantle != ch->mantle )
	{
		ch->mantle = mantle;
		is_change = true;
	}

	short int Chair = ch->equip[ WT_SHOES].item_no;			// LTS SITDOWN BUG
	//CItem* t=ItemUnit(Chair);
	CItem_Etc* tempItem=(CItem_Etc*)ItemUnit(Chair);

	if(tempItem && tempItem->GetItemKind()==IK_CHAIR)
	{
		if (ch->bAlive==1)
		{
			ch->nCurrentAction=1;		
			ch->ChairNum=tempItem->Repair_Skill2;
			is_change = true;
		}
	}
	else
	{
		if (ch->nCurrentAction)
		{
			ch->nCurrentAction=0;		
			ch->ChairNum=0;
			is_change = true;
		}
	}


	short int HorseNo = ch->equip[ WT_SHOES].item_no;			// LTS HORSERIDER

	if(HorseNo>=8041&&HorseNo<=8043)
	{
		if (ch->bAlive==1)
		{
			ch->HorseNo=HorseNo-8040;
			ch->RiderNo=GetRider(ch);
			ch->HorseGrade=0;//GetHorseGrade(ch);//020508 LTS 용량 문제
			ch->RiderGrade=0;//GetRiderGrade(ch);//020508 LTS 용량 문제
		/*	ch->HorseNo=2;
			ch->RiderNo=1;
			ch->HorseGrade=1;
			ch->RiderGrade=1;*/
			is_change = true;
		}
	}
	else
	{
		if (ch->HorseNo>0)
		{
			ch->HorseNo=0;
			ch->RiderNo=0;
			ch->HorseGrade=0;
			ch->RiderGrade=0;
			is_change = true;
		}
	}

	// 011030 KHS JJING
	// 같은 갑옷이더라도 grade가 다르면 바꿔주어야 한다. 
	int cur_jjing = ((RareMain*)&(ch->equip[ WT_ARMOR].attr[ IATTR_RARE_MAIN]))->grade;
	if( ch->jjing != cur_jjing )
	{
		ch->jjing = cur_jjing;
		is_change = true;
	}

	if( is_change || send )
	{
		t_packet packet;
		memcpy(ch->accessory, equip, 4);

		packet.h.header.type = CMD_CHANGE_EQUIP;
		packet.h.header.size = sizeof(t_server_change_equip);
		packet.u.server_change_equip.server_id= ch->GetServerID();
		memcpy(packet.u.server_change_equip.equip, equip, 4);
		packet.u.server_change_equip.mantle = ch->mantle;
		// 011030 KHS JJING
		packet.u.server_change_equip.jjing = ch->jjing;
		packet.u.server_change_equip.ChairNum=ch->ChairNum;		// LTS SITDOWN BUG
		packet.u.server_change_equip.Horse[0]=ch->HorseNo;		// LTS HORSERIDER
		packet.u.server_change_equip.Horse[1]=ch->RiderNo;		// LTS HORSERIDER
		packet.u.server_change_equip.Horse[2]=ch->HorseGrade;	// LTS HORSERIDER
		packet.u.server_change_equip.Horse[3]=ch->RiderGrade;	// LTS HORSERIDER		
		packet.u.server_change_equip.idWeaponItem = ch->equip[WT_WEAPON].item_no; // CSD-031001
		QueuePacket(connections, ch->GetServerID(), &packet, 1);
		CastMe2Other( ch->GetServerID(), &packet);
	}
}




void RecvRepPartyInfo( char *party_name, short int cn )		// 파티원의 xy 좌표와 hp, hpmax값을 보내준다.
{
	int party_id = ExistHe( party_name );
	if( party_id == -1 ) return;
	
	CHARLIST *party = &connections[party_id].chrlst;
	if( !party ) return;

	t_packet packet;

	packet.h.header.type = CMD_REQ_PARTY_INFO;
		strcpy( packet.u.kein.rep_party_info.party_name, party_name);
		packet.u.kein.rep_party_info.x= party->X;
		packet.u.kein.rep_party_info.y= party->Y;
		packet.u.kein.rep_party_info.hp= party->Hp;
	packet.h.header.size = sizeof(k_rep_party_info);
	QueuePacket(connections, cn, &packet, 1);
	//CastMe2Other( cn, &packet);
}

void LoadPublicBbs( )
{
	char *text = NULL;
}

void SendLoginToClientPublicBbsOnlyOne( t_packet *p )
{
	p->h.header.type = CMD_LOGIN_TO_GAME_PUBLIC_BBS;
	QueuePacket(connections, p->u.kein.server_text_only_one.server_id, p, 1);
	return ;
}


void ReturnResultByName( char *name,  int cmd_type )		// 이름으로 아이디를 찾아서 프로토콜을 전송해준다.
{
	int cn = ExistHe( name );
	if( cn == -1 ) return;

	CallClient( cn, cmd_type );
}

void CheckMyMailOnStartGame( short int cn )
{	//< CSD-030324
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	t_packet packet;

	packet.h.header.type = CMD_START_GAME_MAIL;
		strcpy( packet.u.kein.start_game_mail.name, ch->Name);
		packet.u.kein.start_game_mail.server_id =  cn;
		packet.u.kein.start_game_mail.guild_code =  ch->GetGuildCode();
	packet.h.header.size = sizeof(k_start_game_mail);

	QueuePacket(connections, DB_DEMON , &packet, 1);
}	//> CSD-030324

void SendToLoginserver( short int cn )
{
	t_packet packet;

	packet.h.header.type = CMD_CALL_PUBLIC_BBS;
		packet.u.kein.server_id.server_id =  cn;
	packet.h.header.size = sizeof(k_server_id);
	QueuePacket(connections, DB_DEMON , &packet, 1);
}

// Modified by chan78 at 2000/11/28
void SendLoginServerHunterRegist( k_hunter_regist *p, int size, short int cn )		// 001212 YGI
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	if( p->money < 5000000 ) return;		// 돈이 부족함..
	if( ch->Money < p->money ) return;

	if( SubtractMoney( p->money, ch ) != 1 ) return;
	
	t_packet packet;
	packet.h.header.type = CMD_HUNTER_REGIST;
		strcpy( packet.u.kein.hunter_regist2.regist_name, ch->Name);
		strcpy( packet.u.kein.hunter_regist2.target_name, p->name);
		packet.u.kein.hunter_regist2.money = p->money;
		strcpy( packet.u.kein.hunter_regist2.text, p->text);
	packet.h.header.size = sizeof( k_hunter_regist2 )-1000+strlen(p->text)+1;
	QueuePacket(connections, DB_DEMON , &packet, 1);	
}
void SendLoginServerReqHunterList( int id, short int cn )
{
	t_packet packet;
	
	packet.h.header.type = CMD_GET_HUNTER_LIST;
		packet.u.kein.game_login_hunter_list.server_id = cn;
		packet.u.kein.game_login_hunter_list.list_id = id;
	packet.h.header.size = sizeof( k_game_login_hunter_list );
	QueuePacket(connections, DB_DEMON , &packet, 1);
}

void RecvLoginServerAndSendHunterList( k_login_game_hunter_list *login_list )
{
	t_packet packet;
	
	packet.h.header.type = CMD_SEND_HUNTER_LIST;
		packet.u.kein.hunter_list = login_list->list;
	packet.h.header.size = sizeof( k_hunter_list )-1000+strlen( login_list->list.text );
	QueuePacket(connections, login_list->server_id, &packet, 1);
}

void KillWho(CHARLIST*death_ch, CHARLIST *attack_ch)
{	//< CSD-CN-031213
	if (death_ch->IsPlayer())
	{
		CBaseArena* pGame = death_ch->GetJoinArenaGame();

		if (pGame != NULL)
		{
			pGame->SendGameMessage(attack_ch, death_ch);
			
			switch (pGame->GetArenaType())
			{
			case AT_STONE:
			case AT_MONSTER:
				{
					CallMenu(MN_REVIVAL, death_ch->GetServerID());
					break;
				}
			}
		}

		if (attack_ch->IsPlayer())
		{
			SendPacketDefault(CMD_KILL_WHO, death_ch->Name, strlen(death_ch->Name), attack_ch->GetServerID());
		}
		// 030624 YGI
		if( g_EventFlagMgr.IsMoveMapWhenDie() )
		{
			extern bool MoveToVillage(int server_id);
			MoveToVillage(death_ch->GetServerID());
		
			t_packet packet;
			packet.h.header.size = sizeof(t_arena_game_message2);
			packet.h.header.type = CMD_ARENA_GAME_MESSAGE2;
			packet.u.arena.arena_game_message2.nTeamNumber1 = 1;
			packet.u.arena.arena_game_message2.nTeamNumber2 = 1;
			strcpy(packet.u.arena.arena_game_message2.szTeamMember1, attack_ch->Name);
			strcpy(packet.u.arena.arena_game_message2.szTeamMember2, death_ch->Name);
			g_pUserManager->SendPacket(&packet);
		}
	}
}	//> CSD-CN-031213

void RecvHunterList( int id, short int cn )
{ //< CSD-020819
	CHARLIST *ch = CheckServerId( cn );
	if (!ch)              return;
  if (!ch->IsPlayer())  return;

	t_packet packet;
	packet.h.header.type = CMD_KILL_HUNTER_LIST;
	packet.u.kein.kill_hunter_list_login.server_id = cn;
	packet.u.kein.kill_hunter_list_login.list_id= id;
  strcpy( packet.u.kein.kill_hunter_list_login.hunter_name , ch->Name );
	packet.h.header.size = sizeof( k_kill_hunter_list_login );
	QueuePacket(connections, DB_DEMON , &packet, 1);
} //> CSD-020819

// 성공으로 현상범 결과를 마무리 짓는 루틴
// 성공했음을 알려준다.
void RecvHunterListOk( k_send_kill_hunter_list *p )
{
	CHARLIST *ch = ::CheckServerId(p->server_id);
	if(!ch){return;}
	if(!ch->IsPlayer())  {return;}

	::AddMoney(p->money,ch);//021126 lsw// 돈을 올려 준다.

	t_packet packet;				// 성공했다..
	packet.h.header.type = CMD_KILL_HUNTER_LIST_OK;
	packet.u.kein.kill_hunter_ok.money= p->money;
	packet.h.header.size = sizeof( k_kill_hunter_ok );
	QueuePacket(connections, p->server_id, &packet, 1);
} 

void ReturnDangerByHunter( short int target_id, char type)
{
	t_packet packet;				// 성공했다..
	packet.h.header.type = CMD_DANGER_OF_HUNTER;
		packet.u.kein.default_char= type;
	packet.h.header.size = sizeof( char );
	QueuePacket(connections, target_id, &packet, 1);
}

void RecvReqQuestIndex( char page, short int cn )
{
	t_packet packet;
	packet.h.header.type = CMD_REP_QUEST_INDEX;

	int start = page*14, para = 0;
	for( int i=QUEST_START, ct=0; i<_SCRIPT_VARABLE_POINT_ ; i++ )
	{
		if( var[cn][i] ) 
		{
			if( start <= ct )
			{
				packet.u.kein.rep_quest_index.quest[para].index = i;
				packet.u.kein.rep_quest_index.quest[para].ret = var[cn][i];
				para++;
				if( para > 13 ) break;
			}
			ct++;
		}
	}
		packet.u.kein.rep_quest_index.max = para;

	packet.h.header.size = sizeof(k_rep_quest_index)-( (14-para)* sizeof(k_quest_unit) );
	QueuePacket(connections, cn, &packet, 1);
}

void SendLearnedItem( t_connection c[], short int cn )	
{
	t_packet packet;

	packet.h.header.type = CMD_SEND_LEARN_ITEM_INDEX;
	{
		packet.u.kein.server_learn_item.item_count = c[cn].chrlst.ItemMax;
		memcpy( packet.u.kein.server_learn_item.item, c[ cn].chrlst.Item,	sizeof( short int ) * c[cn].chrlst.ItemMax );
	}
	packet.h.header.size = sizeof( k_server_learn_item );
	QueuePacket( c, cn, &packet, 1);
}

void SendDeleteBox( DWORD box_id )
{
	t_packet packet;

	packet.h.header.type = CMD_DELETE_BOX_ITEM;
	{
		packet.u.kein.default_dword = box_id;
	}
	packet.h.header.size = sizeof( DWORD );
	QueuePacket( connections, DB_DEMON, &packet, 1);
}

bool IsConnectLogin(int idUser)
{	//< CSD-030515
	if (connections[idUser].dwAgentConnectionIndex == 0)
	{
		return false;
	}

	if (connections[idUser].state < CONNECT_LOGIN)
	{
		return false;
	}

	return true;
}	//> CSD-030515

bool IsConnectJoin(int idUser)
{	//< CSD-030515
	if (connections[idUser].dwAgentConnectionIndex == 0)
	{
		return false;
	}

	if (connections[idUser].state < CONNECT_JOIN)
	{
		return false;
	}

	return true;
}	//> CSD-030515

CHARLIST* CheckServerId(WORD id)
{	//< CSD-HK-030829
	if (id <= DB_DEMON || id >= DRAGON_MAX_CONNECTIONS)
	{
		return NULL;
	}

	if (connections[id].dwAgentConnectionIndex == 0)
	{
		return NULL;
	}

	if (connections[id].state < CONNECT_LOGIN)
	{
		return NULL;
	}

	return &connections[id].chrlst;
}	//> CSD-HK-030829

CHARLIST* CheckServerName( const char* szName )		// 030224 kyo 이름으로 찾는다. 
{	//< CSD-030320
	return g_pUserManager->GetCharList(szName);
}	//> CSD-030320

CHARLIST* GetCharListPtr(WORD idObject)
{
	if (idObject >= 10000)
	{
		idObject = idObject - 10000;
		
		if (idObject >= NPC_LIST_START && idObject < MAX_NPC_LIST)
		{
			return &NPCList[idObject];
		}
	}
	else
	{
		if (idObject >= DRAGON_CONNECTIONS_START && idObject < DRAGON_MAX_CONNECTIONS)
		{
			return &connections[idObject].chrlst;
		}
	}
	
	return NULL;
}

int SendServerEachItemAuto( CHARLIST *ch, ItemAttr *item )		// 빈곳을 찾아 보내준다. 빈곳이 있으면 1을 리턴 없으면 0을 리턴
{
	int a, b, c;
	int ret;

	if( SearchInv( ch->inv, a, b, c ) )
	{
		POS pos;
		SetItemPos( INV, a, b, c, &pos );
		SendServerEachItem( &pos, item, ch->GetServerID());
		ch->inv[a][b][c] = *item;
		ret = 1;
	}
	else	// 땅에 떨어 뜨리고 0을 리턴
	{
		SendItemEventLog( item, ch->GetServerID(), 0, SILT_PUT_GROUND_MAX_INVEN, 1 ); //YGI acer
		DropItem( ch->X, ch->Y, item );
		ret = 0;
	}
	memset( item, 0, sizeof( ItemAttr ) );
	return ret;
}
void RecvReqManToManRefusal( short int target_id, short int cn )
{
	CHARLIST *me = CheckServerId( cn );
	if( !me ) return;

	RecvReqManToManRefusal( target_id, me );
}

void RecvReqManToManRefusal( short int target_id, CHARLIST *ch )
{
	if( ch->fight_flag != 2 ) return;
	ResultManToMan( ch->GetServerID(), target_id, 3 );
}

///////////////////////////////////////////////////////////////////////
void SendDefaultResult( int type, char ret, short int cn )
{
	t_packet p;
	p.h.header.type = type;
		p.u.kein.default_char = ret;
	p.h.header.size = sizeof( char );
	QueuePacket(connections, cn, &p, 1);
}



int ServerEachItemAuto( CHARLIST *ch, ItemAttr *item )		// 빈곳을 찾아 아이템을 셋팅한다. 클라이언트로 보내주진 않는다.
{
	int a, b, c;
	int ret;
	if( SearchInv( ch->inv, a, b, c ) )
	{
		ch->inv[a][b][c] = *item;
		ret = 1;
	}
	else	// 땅에 떨어 뜨리고 0을 리턴
	{
		DropItem( ch->X, ch->Y, item );
		ret = 0;
	}
	memset( item, 0, sizeof( ItemAttr ) );
	return ret;
}
void ResetManToManItemWhenCloseConnect( CHARLIST *you )
{
	RecvReqManToManRefusal( you->fight_id, you );
}

void RecvChangeScriptValue( t_packet *p, short int cn)
{
	int script_num = p->u.kein.script_change.script_num;
	int value = p->u.kein.script_change.value;

	if( script_num < 0 || script_num > _SCRIPT_VARABLE_POINT_  ) return;
	var[cn][script_num] = value;
}

bool IsSameNation( CHARLIST *party_ch, CHARLIST *ch )
{
	if( party_ch->name_status.nation == ch->name_status.nation ) return true;
	else return false;
}

void CheckBoxAndDownDuration( )
{
	for( int i=DRAGON_CONNECTIONS_START; i<DRAGON_MAX_CONNECTIONS-10; i+=10 )
	{
		int ct=i+rand()%10;
		if( connections[ct].dwAgentConnectionIndex && connections[ct].state >= CONNECT_JOIN )
		{
			for( int a=0; a<3; a++ )
				for( int b=0; b<3; b++ )
					for( int c=0; c<3; c++ )
					{
						ItemAttr &item = connections[ct].chrlst.inv[a][b][c];
						if( item.item_no )
						{
							POS pos;
							SetItemPos( INV, a, b, c, &pos );
							CItem *t = ItemUnit( item );
							if( t )
							{
								if( t->GetItemKind() == IK_BOX )
								{
									WORD d_curr, d_max;
									GetItemDuration( item, d_curr, d_max );
									if( d_curr )
									{
										d_curr--;
										if( d_curr > 100 ) 
											d_curr = d_max = 100;
										
										if( d_curr < 10 ) 
										{
											// 020428 YGI acer
											BreakItemByDuration( &item, &connections[ct].chrlst, pos );
										}
										else
										{
											item.attr[IATTR_DURATION] = MAKELONG( d_curr, d_max );
											SendChangeDurationByKein( &pos, item.attr[IATTR_DURATION], ct );
										}
									}
									// 020428 YGI acer
									else BreakItemByDuration( &item, &connections[ct].chrlst, pos );
								}
							}
						}
					}
		}
	}
}


void SendChangeDurationByKein( POS *pos, DWORD dur, int cn )
{
	t_packet p;
	p.h.header.type = CMD_CHANGE_DURATION;
		p.u.kein.change_dur.pos = *pos;
		p.u.kein.change_dur.dur = dur;
	p.h.header.size = sizeof( k_change_dur );
	QueuePacket(connections, cn, &p, 1);
}

void SendCheckName( short int cn, char *name )		// 디비 데몬으로 캐릭터 존재 여부 요청 루틴
{
	t_packet p;

	p.h.header.type = CMD_EXIST_NAME;
	{
		p.u.kein.exist_name.server_id = cn;
		strcpy( p.u.kein.exist_name.name, name );
	}
	p.h.header.size = sizeof( k_exist_name ) - 20+strlen( name ) +1 ;
	QueuePacket(connections, DB_DEMON, &p, 1);
}

void SendCheckNameForNation( char *name, short int cn )
{
	t_packet p;
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	p.h.header.type = CMD_ISSAME_MY_NATION;
	{
		p.u.kein.exist_name_and_nation.server_id = cn ;
		p.u.kein.exist_name_and_nation.nation= ch->name_status.nation;
		strcpy( p.u.kein.exist_name_and_nation.name, name );
	}
	p.h.header.size = sizeof( k_exist_name_and_nation ) - 20+strlen( name ) +1 ;
	QueuePacket(connections, DB_DEMON, &p, 1);
}



//////////////////////////////////////////////////////////////////////
//
//		 투표 시스템
//

// 나라 별로
int	g_CurrVote[6];		// 현재 투표중인가?	0:아니다.	1:후보 등록기간	2:선거 기간		
int	g_VoteNumber[6];	// 투표 번호
int g_vote_reset;		// 투표에 관련된 값을 다시 가져 올건지를 세팅한다.
typedef struct tag_server_candidate
{
	k_candidate	data;
	int			score;
}k_server_candidate;

k_server_candidate g_Candidate[6][3];

int CanResistCandidate( CHARLIST *ch )
{	//< CSD-030806
	if (ch->GetLevel() < 74)
	{	// level이 부족하다
		return -1;
	}
	
	if (ch->name_status.guild_master != 1)
	{	// 길드 마스터가 아니다.
		return -2;
	}

	if (ch->nk[ch->name_status.nation] > 0)
	{	// 피케이다
		return -3;
	}

	return 1;
}	//> CSD-030806

void RecvRegistCandidate( short int cn )
{	//< CSD-030324
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	if( g_CurrVote[ch->name_status.nation] != 1 ) return;		// 선거 등록을 받고 있지 않다.
	if( CanResistCandidate( ch ) < 0 ) return;		// 자격이 없다.

	t_packet p;
	p.h.header.type = CMD_REGIST_CANDIDATE;
	{
		strcpy( p.u.kein.regist_candidate.candidate.name, ch->Name );
		p.u.kein.regist_candidate.server_id	= cn;
		p.u.kein.regist_candidate.candidate.number		= g_VoteNumber[ch->name_status.nation];
		p.u.kein.regist_candidate.candidate.lv			= ch->GetLevel(); // CSD-030806 
		p.u.kein.regist_candidate.candidate.class_type	= ch->Class;
		p.u.kein.regist_candidate.candidate.gender		= ch->Gender;
		p.u.kein.regist_candidate.candidate.nation		= ch->name_status.nation;
		p.u.kein.regist_candidate.candidate.guild_code	= ch->GetGuildCode();
		p.u.kein.regist_candidate.candidate.face		= ch->Face;
	}
	p.h.header.size = sizeof( k_regist_candidate) - 20+strlen( ch->Name ) +1 ;
	QueuePacket(connections, DB_DEMON, &p, 1);	
}	//> CSD-030324

void RecvAndSendLoginCheckRegistCandidate( short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int ret = CanResistCandidate( ch );
	if( ret < 0 ) 
	{
		CallClient( cn, CMD_THIS_MAN_IS_CANDIDATE_NOT);
		return;
	}

	//////// 투표관련 변수를 다시 읽어 오기 위한 때를 위해
	static DWORD old_time = g_curr_time;
	if( g_vote_reset > 200 ) g_vote_reset= 0;
	if( g_vote_reset==0 || g_curr_time - old_time > 3600 ) 
	{
		old_time = g_curr_time;
		CallClient( DB_DEMON, CMD_SET_VALUE_OF_VOTE );
		g_vote_reset++;
		CallClient( cn, CMD_RETRY );
		return;
	}
	g_vote_reset++;
	///////////////////////////////////////////////////////

	t_packet p;
	p.h.header.type = CMD_IS_THIS_MAN_CANDIDATE;
	{
		p.u.kein.is_this_man_candidate.server_id = cn;
		p.u.kein.is_this_man_candidate.vote_num = g_VoteNumber[ch->name_status.nation];
		strcpy( p.u.kein.is_this_man_candidate.name, ch->Name );
	}
	p.h.header.size = sizeof( k_is_this_man_candidate ) - 20+strlen( ch->Name ) +1 ;
	QueuePacket(connections, DB_DEMON, &p, 1);
}

void CheckVote( short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	//////// 투표관련 변수를 다시 읽어 오기 위한 때를 위해
	static DWORD old_time = g_curr_time;
	if( g_vote_reset > 400 ) g_vote_reset = 0;
	if( g_vote_reset==0 || g_curr_time - old_time > 3600 ) 
	{
		old_time = g_curr_time;
		CallClient( DB_DEMON, CMD_SET_VALUE_OF_VOTE );
		g_vote_reset++;
		CallClient( cn, CMD_RETRY );
		return;
	}
	g_vote_reset++;
	///////////////////////////////////////////////////////

	if( !ch->name_status.nation ) 
	{
		ReturnCheckVoteNot( cn, -2 );	// 나라가 없다.
		return;
	}
	if( g_CurrVote[ch->name_status.nation] != 2 ) 
	{
		ReturnCheckVoteNot( cn, -1 );	// 선거 기간이 아니다.
		return;
	}
	if( ch->vote_num >= g_VoteNumber[ch->name_status.nation] )
	{
		ReturnCheckVoteNot( cn, -3 );// 이미 선거를 했다.
		return;
	}

	// DB-DEMON으로 요청
	t_packet p;
	p.h.header.type = CMD_CHECK_VOTE;
	{
		p.u.kein.check_vote.server_id = cn;
		p.u.kein.check_vote.vote_num = g_VoteNumber[ch->name_status.nation];
		p.u.kein.check_vote.nation = ch->name_status.nation;
		strcpy( p.u.kein.check_vote.login_id, connections[cn].id );
	}
	p.h.header.size = sizeof( k_check_vote )-20+strlen(connections[cn].id)+1;
	QueuePacket(connections, DB_DEMON , &p, 1);
}

int ReturnCheckVoteNot( short int client_id, int ret )
{
	CHARLIST *ch = CheckServerId( client_id );
	if( !ch ) return 0;

	if( ret == -3 )
	{
		ch->vote_num = g_VoteNumber[ch->name_status.nation];
	}

	t_packet p;
	p.h.header.type = CMD_CHECK_VOTE_NOT;
		p.u.kein.default_char = ret;
	p.h.header.size = sizeof( char );
	QueuePacket(connections, client_id, &p, 1);

	return 1;
}

void RecvAndSendCheckVote_Ok( t_packet *p, short int cn )
{
	short int server_id = p->u.kein.check_vote_ok.server_id;

	int count = p->u.kein.check_vote_ok.candidate_count;
	if( count < 0 || count >= 3 ) return;
	int nation = p->u.kein.check_vote_ok.candidate[0].nation;
	
	t_packet packet;
	packet.h.header.type = CMD_CHECK_VOTE_OK;
	{
		packet.u.kein.check_vote_client_ok.candidate_count = count;
		for( int i=0; i<count; i++ )
		{
			g_Candidate[nation][i].data = p->u.kein.check_vote_ok.candidate[i];
			packet.u.kein.check_vote_client_ok.candidate[i] = p->u.kein.check_vote_ok.candidate[i];
		}
		packet.h.header.size = sizeof( k_check_vote_client_ok ) - sizeof( k_candidate )*(3-count );
	}
	QueuePacket(connections, server_id, &packet, 1);
}

void RecvAndSendRegistCandidateResult( t_packet *p )
{
	short int server_id = p->u.kein.login2game_ear_message_result.send_server_id;

	t_packet packet;
	packet.h.header.type = p->h.header.type;
		packet.u.kein.default_char = p->u.kein.login2game_ear_message_result.ret;
	packet.h.header.size = sizeof( char );
	QueuePacket(connections, server_id, &packet, 1);
}

void RecvSetValueOfVote( t_packet *p )
{
	k_set_value_of_vote *target = &p->u.kein.set_value_of_vote;

	for( int i=0; i<6; i++ )
	{
		g_CurrVote[i] = target->is_voting[i];
		g_VoteNumber[i] = target->vote_num[i];
	}
}

void SendVoteGame2Login( t_packet *t, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int nation = ch->name_status.nation;
	if( !nation ) return;		// 나라가 없다

	if( g_CurrVote[nation] < 0 ) return;
	if( g_VoteNumber[nation] < 0 ) return;

	int support = t->u.kein.default_char;

	t_packet p;
	p.h.header.type = CMD_VOTE_GAME2LOGIN;
	{
		p.u.kein.vote_game2login.server_id = cn;
		p.u.kein.vote_game2login.vote = g_VoteNumber[nation];
		p.u.kein.vote_game2login.support = support;
		p.u.kein.vote_game2login.nation = nation;
		strcpy( p.u.kein.vote_game2login.login_id, connections[cn].id );
	}
	p.h.header.size = sizeof( k_vote_game2login ) - 20+strlen( connections[cn].id ) +1 ;
	QueuePacket(connections, DB_DEMON, &p, 1);
}

void SendClientVoteScore( int nation, short int server_id )		// 클라이언트로 값을 보내준다.
{
	int i =0;
	t_packet p;
	p.h.header.type = CMD_REP_VOTE_SCORE;
	{
		for( int x=0; x<3; x++ )
		{
			if( g_Candidate[nation][i].data.name[0] )
			{
				p.u.kein.client_rep_vote_score.name_score[i].score = g_Candidate[nation][i].score;
				strcpy( p.u.kein.client_rep_vote_score.name_score[i].name, g_Candidate[nation][i].data.name );
				i++;
			}
		}
		p.u.kein.client_rep_vote_score.max = i;
	}
	p.h.header.size = sizeof( k_client_rep_vote_score ) - sizeof( k_score_and_name )*(3-i);
	QueuePacket(connections, server_id, &p, 1);
}

void ReqVoteScore( short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int nation = ch->name_status.nation;
	if( !nation ) return;
	if( !g_CurrVote[nation] || !g_VoteNumber[nation] ) return;

	static int old_time = g_curr_time;

	int is_req = 0;		// 디비에서 읽어와야 하는가?
	if( g_curr_time - old_time > 360 ) 
	{
		is_req = 1;
		old_time = g_curr_time;
	}

	if( is_req || !g_Candidate[nation][0].data.name[0] || !g_Candidate[nation][0].score )
	{
		// 디비에서 읽어 온다. ( 요청 )
		t_packet p;
		p.h.header.type = CMD_REQ_VOTE_SCORE;
		{
			p.u.kein.req_vote_score.server_id = cn;
			p.u.kein.req_vote_score.vote_num = g_VoteNumber[nation];
			p.u.kein.req_vote_score.nation = nation;
		}
		p.h.header.size = sizeof( k_req_vote_score );
		QueuePacket(connections, DB_DEMON, &p, 1);		
	}
	else
	{
		// 여기서 그냥 값을 보내준다.
		SendClientVoteScore( nation, cn);
	}
}

void SendClientRepVoteScore( t_packet *p )
{
	k_score_and_name *target = p->u.kein.rep_vote_score.name_score;
	int server_id = p->u.kein.rep_vote_score.server_id;
	int max = p->u.kein.rep_vote_score.max;

	CHARLIST *ch = CheckServerId( server_id );
	if( !ch ) return;

	for( int i=0; i<max; i++ )
	{
		g_Candidate[ch->name_status.nation][i].score = target[i].score;
		strcpy( g_Candidate[ch->name_status.nation][i].data.name, target[i].name );
	}
	SendClientVoteScore( ch->name_status.nation, server_id );
}

////////////////////////////////////////////////////////////
// 보여주기 직업
void RecvGetSkillInfo( t_packet *p, short int cn )
{//021113 lsw
	short int target_id = p->u.kein.default_short_int;
	CHARLIST *target_ch = CheckServerId( target_id );
	if( !target_ch ) return;

	const int iSkillMasterType = GMMgr.IsSkillMaster(target_ch);
	if(iSkillMasterType)//기성일 경우도 무조건 기성이다
	{//우선 순위로 기성이 먼저 올라온다
		::SendSkillInfo( target_id, 1000000, iSkillMasterType, cn );
		return;
	}
	
	if(GMMgr.IsMerchant(target_ch))//상인이면 무조건 상인밖에 안된다
	{
		::SendSkillInfo( target_id, 1000000, GM_QUEST_MERCHANT, cn );
		return;
	}

	if( target_ch->view_job > 0 )	// 이미 정의된 직업이 있다면
	{
		const int view_job			= target_ch->view_job;
		const int iSkillExpIndex	= g_GmMain[view_job].skill;
		const int iExp = target_ch->skillexp[iSkillExpIndex].skillexp;
		if(iExp) 
		{
			::SendSkillInfo( target_id, iExp, view_job, cn );
			return;
		}
	}
	else
	{
		int top_exp = 0;
		int top_jop = 0;
		for( int i=0; i<MAX_GM_MAIN; i++ )
		{
			const int iSkillExpIndex = g_GmMain[i].skill;
			if(	1== g_GmMain[i].job_level
			||	2== g_GmMain[i].job_level)
			{
				if( target_ch->skillexp[iSkillExpIndex].skillexp > top_exp )
				{
					top_exp = target_ch->skillexp[iSkillExpIndex].skillexp;
					top_jop =i;
				}
			}
		}
		target_ch->view_job = top_jop;
		::SendSkillInfo( target_id, top_exp, top_jop, cn );
	}
}

void SendSkillInfo( short int target_id, int top_exp, int top_job, short int cn ,const bool bCast)
{//021113 lsw
	t_packet p;
	int skill_lv = top_exp / 10000;
	p.h.header.type = CMD_GET_SKILL_INFO;
	{
		p.u.kein.get_skill_info.target_id	= target_id;
		p.u.kein.get_skill_info.job			= top_job;		// 1을 더해서 보내고 출력하는데서 1을 빼서 출력한다.
		p.u.kein.get_skill_info.call_lv		= skill_lv;
	}
	p.h.header.size = sizeof( k_get_skill_info );
	QueuePacket(connections, cn, &p, 1);
	if(bCast)
	{
		::CastMe2Other( cn, &p);
	}
}

void RecvClienNationMassage( t_packet *p, short int cn )		// 왕인지 확인이 되면 메시지를 로그인 서버로 보낸다.
{
	char *text = p->u.kein.default_msg;
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

//	if( !ch->name_status.king ) return;  coromo 2005/05/06 혼뇜KING景홈

	t_packet packet;
	packet.h.header.type = CMD_SEND_ALL_MSG;
	{
		packet.u.kein.send_login_nation_msg.nation	= ch->name_status.nation;
		strcpy( packet.u.kein.send_login_nation_msg.msg, text );
	}
	packet.h.header.size = sizeof( k_send_login_nation_msg ) - MAX_MSG + strlen( text )+1;

	// Modified by chan78 at 2000/12/01 BroadCast It to MapServers.
	g_pServerTable->BroadCastToEveryServer( (char*)&packet, (sizeof(t_header)+packet.h.header.size), SERVER_TYPE_MAP );
}

extern void	SendPacket2NationClients(const int Nation,t_packet* p);	// CSD-030415

void RecvAndSendNationMassage( t_packet *p )
{	//< CSD-030415
	const int nation = p->u.kein.send_login_nation_msg.nation;
	if( nation <= 0 || nation >= 6 ) return;
	char *text = p->u.kein.send_login_nation_msg.msg;
	
	t_packet packet;
	packet.h.header.type = CMD_RECV_ALL_MSG;
	packet.h.header.size = strlen(text)+1;
	strcpy(packet.u.kein.default_msg, text);
	SendPacket2NationClients(nation, &packet);
}	//> CSD-030415

void RecvDemonYouAreKing( t_packet *p, int type )
{
	//혹시 이곳에 있으면 5000번 까지 안가도 된다.
	int ret = ExistHe( p->u.kein.default_name );
	CHARLIST *ch = CheckServerId( ret );
	if( ch )
	{
		if( type == CMD_YOU_ARE_KING_LOG2CLIENT )
			ch->name_status.king = 1;
		else ch->name_status.king = 0;
		CallClient( ret, type );
	}
	else QueuePacket(connections, LOGIN_SERVER_CONNECT, p, 1);		// 로그인 서버로 전송
}

void RecvLoginYouAreKing( t_packet *p, int type )
{
	int ret = ExistHe( p->u.kein.default_name );
	CHARLIST *ch = CheckServerId( ret );
	if( ch )
	{
		if( type == CMD_YOU_ARE_KING_LOG2CLIENT )
			ch->name_status.king = 1;
		else ch->name_status.king = 0;
		CallClient( ret, type );
	}
}

void RecvGuildExit( t_packet *p, short int cn )		// 길드 탈퇴 명령
{	//< CSD-030326
	const int target_id = p->u.kein.default_short_int;
	// 길드에 등록할 캐릭터
	CHARLIST* target = CheckServerId(target_id);
	
	if (target == NULL)
	{
		return;
	}
	// 길드 마스터
	CHARLIST* ch = CheckServerId(cn);

	if (ch == NULL)
	{
		return;
	}
	//  같은 길드여야 한다. ( 물론 같은 나라 )
	if (ch->GetGuildCode() <= 0 || ch->name_status.guild_master != 1) 
	{
		return;	// 길드 마스터만 사용할수 있는 명령어다.
	}

	const int nGuildCode = target->GetGuildCode();

	if (ch->GetGuildCode() != nGuildCode) 
	{
		return;
	}
	// 길드 멤버 리스트에서 삭제
	g_pGuildManager->DelMember(nGuildCode, target_id);
	// 길드 정보 초기화
	target->SetGuildCode(0); // CSD-030806
	target->name_status.guild_master = 0;
	// 길드 멤버 리스트 테이블에서 삭제
	SendSaveGuildMemberList(target->Name, nGuildCode, 0, 0);
	// 탈퇴당한놈한테
	CallClient(target_id, CMD_GUILD_EXIT_YOU);
	// 결과 전송
	t_packet packet;					
	packet.h.header.type = CMD_GUILD_EXIT_WHO;
	packet.u.kein.default_short_int = target_id;
	packet.h.header.size = sizeof(short int);
	QueuePacket(connections, cn, &packet, 1); // 길드 마스터에게
	CastMe2Other(target_id, &packet); // 주변사람에게
}	//> CSD-030326

void RecvGuildJoin( t_packet *p, short int cn )
{	//< CSD-030324
	int target_id = p->u.kein.default_short_int;
	if( !target_id ) return;

	CHARLIST *target = CheckServerId( target_id );			// 길드에 등록할 캐릭터
	if( !target ) return;

	CHARLIST *ch = CheckServerId( cn );						// 길드 마스터
	if( !ch ) return;

	if( !CheckGuildPower( GDP_JOINING, ch ) ) return;
	if( target->GetGuildCode() ) return;	// 다른 길드에 가입되어 있으면 안된다.
	if( target->name_status.nation != ch->name_status.nation ) return;		// 같은 나라야 한다.

	t_packet packet;					

	packet.h.header.type = CMD_GUILD_JOIN_SERVER;			// DB_DEMON 으로...
		packet.u.kein.guild_join_server.target_id = target_id;
		packet.u.kein.guild_join_server.server_id = cn;	
		packet.u.kein.guild_join_server.guild_code = ch->GetGuildCode();
		strcpy( packet.u.kein.guild_join_server.id, connections[target_id].id );
	packet.h.header.size = sizeof( k_guild_join_server );
	QueuePacket(connections, DB_DEMON, &packet, 1);
}	//> CSD-030324

void RecvGuildJoinResult( t_packet *p )
{	//< CSD-030324
	int target_id = p->u.kein.guild_join_result.target_id;
	int server_id = p->u.kein.guild_join_result.server_id;

	CHARLIST *target = CheckServerId( target_id );
	CHARLIST *guild_master = CheckServerId( server_id );

	if( !target || !guild_master ) return;

	if( p->h.header.type == CMD_GUILD_JOIN_SERVER_OK )
	{
		// 길드 가입이 됐다.

		t_packet packet;
		packet.h.header.type = CMD_GUILD_JOIN_CONFIRM;
			packet.u.kein.default_short_int = server_id;		// 길마의 아이디를 보내준다.
		packet.h.header.size = sizeof( short int );
		QueuePacket(connections, target_id, &packet, 1);

		CallClient(server_id, CMD_GUILD_JOIN_CONFIRM_GUILMA );	// 확인 중이라는 사실을 길마에게 보내준다.


		//int guild_code = target->GetGuildCode() = guild_master->GetGuildCode();
		//target->name_status.guild_master = 0;

		/*	 당사자에게 먼저 물어 본다.
		t_packet packet;
		packet.h.header.type = CMD_GUILD_JOIN_SERVER_OK;
			packet.u.kein.guild_join_ok.guild_code = guild_code;
			packet.u.kein.guild_join_ok.target_id = target_id;
		packet.h.header.size = sizeof( k_guild_join_ok );
		QueuePacket(connections, target_id, &packet, 1);
		CastMe2Other( target_id, &packet );
		*/
	}
	else		// 길마에게만 보내준다.		// 가입 실패
	{
		t_packet packet;
		packet.h.header.type = CMD_GUILD_JOIN_SERVER_NOT;
			packet.u.kein.default_short_int = target_id;
		packet.h.header.size = sizeof( short int );
		QueuePacket(connections, server_id, &packet, 1);
	}
}	//> CSD-030324

void RecvGuildJoinConfirmOk( t_packet *p, short int cn )
{	//< CSD-030806
	int guild_id = p->u.kein.default_short_int;

	CHARLIST *target = CheckServerId( cn );
	CHARLIST *guild_master = CheckServerId( guild_id );

	if( !target || !guild_master ) return;

	if( !CheckGuildPower( GDP_JOINING, guild_master ) ) return;		// 길드 마스터여야 한다.// 020620 YGI

	const int nGuildCode = guild_master->GetGuildCode();
	target->SetGuildCode(nGuildCode);	
	target->name_status.guild_master = 0;

	t_packet packet;
	packet.h.header.type = CMD_GUILD_JOIN_SERVER_OK;
	packet.u.kein.guild_join_ok.guild_code = nGuildCode;
	packet.u.kein.guild_join_ok.target_id = cn;
	packet.h.header.size = sizeof( k_guild_join_ok );
	QueuePacket(connections, cn, &packet, 1);
	CastMe2Other( cn, &packet );
	// 디비 데몬으로 보내 guild_member_list 테이블에 저장한다.
	SendSaveGuildMemberList( target->Name, nGuildCode, nGuildCode, 0 );
	g_pGuildManager->AddMember(nGuildCode, cn);
}	//> CSD-030806

void RecvGuildJoinConfirmNot( t_packet *p, short int cn )		// 길마에게 그가 거절했다는 걸 알린다.
{
	int guild_id = p->u.kein.default_short_int;

	t_packet packet;
	packet.h.header.type = CMD_GUILD_JOIN_CONFIRM_NOT;
	packet.h.header.size = 0;
	QueuePacket(connections, guild_id, &packet, 1);
}

void RecvScanTarget( short int cn )
{	//< CSD-CN-031213
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	if (!ch->JoinLocalWar) return;				// 011217 LTS
	if( ch->scan_time )
	{
		if( (g_curr_time-ch->scan_time) < 20 ) return;		// 시간을 조금더 보내야 한다.
	}
	ch->scan_time = g_curr_time;

	t_packet packet;
	packet.h.header.type = CMD_SCAN_TARGET;
	char &count = packet.u.kein.scan_target.count;
	count = 0;

	CUserManager::HASH_USER mpUser = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = mpUser.begin(); i != mpUser.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = CheckServerId(idUser); 
		
		if (pUser != NULL)
		{
			if (pUser->name_status.nation != ch->name_status.nation)
			{
				packet.u.kein.scan_target.xy[count].x = pUser->X/TILE_SIZE*200/g_Map.file.wWidth;
				packet.u.kein.scan_target.xy[count].y = pUser->Y/TILE_SIZE*200/g_Map.file.wHeight;
				
				if (++count >= 50)
				{
					break;
				}
			}
		}
	}
	
	packet.h.header.size = sizeof( k_scan_target ) - sizeof( k_x_y )*(250-count);
	QueuePacket(connections, cn, &packet, 1);
}	//> CSD-CN-031213

void RecvGuildAllMsg( t_packet *p, short int cn )
{	//< CSD-030324
	char *msg = p->u.kein.default_msg;
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	
	if( !CheckGuildPower( GDP_BBS, ch ) ) return;

	t_packet packet;
	packet.h.header.type = CMD_GUILD_ALL_MSG_RECV;
		packet.u.kein.guild_msg.guild_code = ch->GetGuildCode();
		strcpy( packet.u.kein.guild_msg.msg, msg );
	packet.h.header.size = sizeof( k_guild_msg) - MAX_MSG + strlen( msg )+1;

	// Modified by chan78 at 2000/12/01 BroadCast It to MapServers.
	g_pServerTable->BroadCastToEveryServer( (char*)&packet, (sizeof(t_header)+packet.h.header.size), SERVER_TYPE_MAP );
	RecvGuildAllMsg_FromServer( &packet );
}	//> CSD-030324

void RecvGuildAllMsg_FromServer( t_packet *p )
{	//< CSD-030415
	char *msg = p->u.kein.guild_msg.msg;
	const int guild_code = p->u.kein.guild_msg.guild_code;

	t_packet packet;
	packet.h.header.type = CMD_GUILD_ALL_MSG_RECV;
	packet.h.header.size = strlen(msg) + 1;
	strcpy(packet.u.kein.default_msg, msg);
	g_pGuildManager->SendPacket(guild_code, &packet);
}	//> CSD-030415

void RecvChangeGuildEachLevel( t_packet *p, short int cn )
{	//< CSD-030324
	int each_level = p->u.kein.guild_each_level.each_level;
	int target_id = p->u.kein.guild_each_level.target_id;

	if( each_level <= 0 || each_level > 5 ) return;		// 1:길마, 5: 일반회원
	if( each_level == 1 ) return;		// 길마를 바꾸는 거라면 이걸 풀어준다.

	CHARLIST *target = CheckServerId( target_id );			// 등급을 바꿀 캐릭터
	if( !target ) return;

	CHARLIST *ch = CheckServerId( cn );						// 길드 마스터
	if( !ch ) return;

//  같은 길드여야 한다. ( 물론 같은 나라 )
	//if( !ch->GetGuildCode() || ch->name_status.guild_master != 1 ) return;	// 길드 마스터만 사용할수 있는 명령어다.
	if( !CheckGuildPower( GDP_CHANGE_DEGREE, ch ) ) return;
	if( ch->GetGuildCode() != target->GetGuildCode() ) return;

	int each_level_to_real_level[] = { 0, 1, 2, 3, 4, 0 };		// 1이 길마 5가 일반
	target->name_status.guild_master = each_level_to_real_level[each_level];		// 실제 값 대입

	int guild_code = target->GetGuildCode();
	SendSaveGuildMemberList( target->Name, guild_code, guild_code, each_level_to_real_level[each_level] );

	QueuePacket(connections, target_id, p, 1);		// 그 자신과 주변 사람에게 모두 그 값을 그대로 돌려준다.
	CastMe2Other( target_id, p );
}	//> CSD-030324

void SendBbsAll( char *name, int type )
{
	t_packet packet;
	packet.h.header.type = CMD_BBS_ALL;
		packet.u.kein.bbs_all.type = type;
		strcpy( packet.u.kein.bbs_all.name, name );
	packet.h.header.size = sizeof( k_bbs_all ) - 20+ strlen( name )+1;

	// Modified by chan78 at 2000/12/01 BroadCast It to MapServers.
	g_pServerTable->BroadCastToEveryServer( (char*)&packet, (sizeof(t_header)+packet.h.header.size) );
}

// 로그인서버에서 받아서 고대로 모든 사람에게 뿌려줄때..
void DirectSend(t_packet *p, int type)
{	//< CSD-CN-031213
	if (type)
	{
		p->h.header.type = type;
	}
	
	g_pUserManager->SendPacket(p);
}	//> CSD-CN-031213

CHARLIST* CheckKingByNation(int nation)
{	//< CSD-CN-031213
	CUserManager::HASH_USER mpUser = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = mpUser.begin(); i != mpUser.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = CheckServerId(idUser);
		
		if (pUser != NULL && pUser->name_status.nation == nation)
		{
			if (pUser->name_status.king == 1)
			{
				return pUser;
			}
		}
	}

	return NULL;
}	//> CSD-CN-031213

// 요청받을 왕에게 정보를 보낸다.
void RecvReqStopWarGame2ClientSetPacket( short int nation, short int game_id, short int target_cn, short int send_cn )
{
	t_packet packet;
	packet.h.header.type = CMD_REQ_STOP_WAR_TO_CLIENT;
		packet.u.kein.send_client_req_stop_war.send_nation	= nation;
		packet.u.kein.send_client_req_stop_war.game_id = game_id;
		packet.u.kein.send_client_req_stop_war.server_id = send_cn;
	packet.h.header.size = sizeof( k_send_client_req_stop_war );
	QueuePacket(connections, target_cn, &packet, 1);
}

// 한 왕이 다른 왕에게 휴전 요청을 해왔을때
void RecvReqStopWarGame2Login( t_packet *p, short int cn )
{
	int nation = p->u.kein.default_char;
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	
//	if( ch->name_status.king != 1 ) return;  //coromo 2005/05/06 혼뇜KING景홈
	if( ch->name_status.nation == nation ) return;
	if( nation < N_VYSEUS || nation > N_YILSE ) return;
	
	CHARLIST *target_king = CheckKingByNation( nation );		 // 상대편 나라의 왕이 현재 맵에 있다면
	if( target_king )
	{
		RecvReqStopWarGame2ClientSetPacket( ch->name_status.nation, DRAGON_PORT, target_king->GetServerID(), cn );		// 바로 정보를 보내서 의향을 물어본다.
	}
	else
	{
		t_packet packet;
		packet.h.header.type = CMD_REQ_STOP_WAR_TO_LOGIN;
			packet.u.kein.send_login_req_stop_war.target_nation	= nation;
			packet.u.kein.send_login_req_stop_war.send_nation	= ch->name_status.nation;
			packet.u.kein.send_login_req_stop_war.server_id		= cn;
		packet.h.header.size = sizeof( k_send_login_req_stop_war );

		// Modified by chan78 at 2000/12/01 BroadCast It to MapServers.
		g_pServerTable->BroadCastToEveryServer( (char*)&packet, (sizeof(t_header)+packet.h.header.size), SERVER_TYPE_MAP );
	}
}

// 맵에 왕이 있는지를 알아봐서 있으면 휴전 협상을 보낸다.
void RecvReqStopWarGame2Client( t_packet *p )
{
	int target_nation	= p->u.kein.send_game_server_req_stop_war.target_nation;
	int send_nation		= p->u.kein.send_game_server_req_stop_war.send_nation;
	short int server_id	= p->u.kein.send_game_server_req_stop_war.server_id;
	short int game_id	= p->u.kein.send_game_server_req_stop_war.game_id;

	CHARLIST *ch = CheckKingByNation( target_nation );
	if( !ch ) return;
	RecvReqStopWarGame2ClientSetPacket( send_nation, game_id, ch->GetServerID(), server_id );
}

void RecvRepStopWarRefusal( t_packet *p )
{
	int game_id = p->u.kein.rep_stop_war_login.game_id;		// gameserver_port;
	if( game_id == DRAGON_PORT )		// 여기 게임 서버라면
	{
		int server_id = p->u.kein.rep_stop_war_login.server_id;
		CHARLIST *ch = CheckServerId( server_id );
		if( !ch ) return;
		CallClient( ch->GetServerID(), CMD_REP_STOP_WAR_REFUSAL_TO_CLIENT );		// 거절 당했다는 것을 알린다.
	}
	else {
		// Modified by chan78 at 2000/12/01 BroadCast It to MapServers.
		g_pServerTable->BroadCastToEveryServer( (char*)p, (sizeof(t_header)+p->h.header.size), SERVER_TYPE_MAP );
	}
}

void RecvRepStopWarAccept( t_packet *p, short int cn )				// 승낙했다.
{	
	int send_nation = p->u.kein.default_char;				// 처음에 휴전을 요청했던 나라
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	if( ch->name_status.king != 1 ) return;  //coromo 2005/05/06 혼뇜KING景홈
	int target_nation = ch->name_status.nation;

	SendNation2NationRelation( target_nation, send_nation, NATION_RELATION_FRIEND_ );
		/*

	t_packet packet;
	packet.h.header.type = CMD_REP_STOP_WAR_ACCEPT_TO_LOGIN;
		packet.u.kein.stop_war_all_msg.target_nation = target_nation;
		packet.u.kein.stop_war_all_msg.send_nation	= send_nation;
	packet.h.header.size = sizeof( k_stop_war_all_msg );
	QueuePacket(connections, LOGIN_SERVER_CONNECT, &packet, 1);*/
}

void RecvRepStopWarGame2Client( t_packet *p )		// 모두에게 휴전의 사실을 보내준다.
{	//< CSD-CN-031213
	const int nation1 = p->u.kein.stop_war_all_msg.send_nation ;
	const int nation2 = p->u.kein.stop_war_all_msg.target_nation;

	t_packet packet;
	packet.h.header.type = CMD_REP_STOP_WAR_ACCEPT_TO_CLIENT;
	packet.h.header.size = sizeof(char);

	CUserManager::HASH_USER mpUser = g_pUserManager->GetUserSet();

	for (CUserManager::ITOR_USER i = mpUser.begin(); i != mpUser.end(); ++i)
	{
		const int idUser = i->second;
		CHARLIST* pUser = CheckServerId(idUser);
		
		if (pUser != NULL)
		{
			if (pUser->name_status.nation == nation1)
			{
				packet.u.kein.default_char = nation2;
				QueuePacket(connections, idUser, &packet, 1);
			}
			
			if (pUser->name_status.nation == nation2)
			{
				packet.u.kein.default_char = nation1;
				QueuePacket(connections, idUser, &packet, 1);
			}
		}
	}
}	//> CSD-CN-031213

void RecvAndReqSalvation( short int cn )		// DB로 기부금을 요청한다.
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	t_packet packet;
	packet.h.header.type = CMD_REQ_SALVATION_MONEY;
		packet.u.kein.req_salvation_money.server_id = cn;
		strcpy( packet.u.kein.req_salvation_money.name, ch->Name );
	packet.h.header.size = sizeof( k_req_salvation_money ) - 20 + strlen( ch->Name )+1;
	QueuePacket(connections, DB_DEMON, &packet, 1);
}

void RecvAndSaveSalvation( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	DWORD money = p->u.kein.default_dword;

	if( !money ) return;
//	ch->Money = GetMoneyByItem( ch );
	if( SubtractMoney( money, ch ) == -1 ) return;
	
	t_packet packet;
	packet.h.header.type = CMD_SALVATION_MONEY;
		packet.u.kein.db_salvation_money.server_id = cn;
		packet.u.kein.db_salvation_money.nation = ch->name_status.nation;
		packet.u.kein.db_salvation_money.money = money;
		strcpy( packet.u.kein.db_salvation_money.name, ch->Name );
	packet.h.header.size = sizeof( k_db_salvation_money ) - 20 + strlen( ch->Name )+1;
	QueuePacket(connections, DB_DEMON, &packet, 1);

	// 011020 YGI
	// 국고로 저장.. 
	SendNationMoney( NM_ADD, ch->name_status.nation, money); // 국고를 변화시킨다.
	// fame 적용
	char add_fame = g_Money_to_Fame.GetFame( money );
	g_pLogManager->SaveLogChange_Fame(ch,ch->fame,ch->fame+add_fame,LF_DONATION);
	ch->fame += add_fame;
	SendDefaultResult( CMD_SALVATION_RESULT_ADD_FAME, add_fame, cn );
}

void Game2ClinetSalvation( t_packet *p )
{
	DWORD money = p->u.kein.db2game_salvation_money.money;
	short int cn = p->u.kein.db2game_salvation_money.server_id;

	t_packet packet;
	packet.h.header.type = CMD_DB2GAME_SALVATION;
		packet.u.kein.default_dword = money;
	packet.h.header.size = sizeof( DWORD );
	QueuePacket(connections, cn, &packet, 1);
}

void RecvDB2MapItem( t_packet *p )	// 디비에서 아이템을 만들어 주라는 명령이 오면 처리
{
	short int item_no = p->u.kein.send_db2map_each_item.item_no;
	short int server_id = p->u.kein.send_db2map_each_item.server_id;

	CHARLIST *ch = CheckServerId( server_id );
	if( !ch ) return;

	ItemAttr item = GenerateItem( item_no );
	if( item.item_no != 0 )
	{
		SendItemEventLog( &item, server_id, SN_NOT_USER,SILT_MAKE_BY_DB, 2 ); //YGI acer
		SendServerEachItemAuto( ch, &item );
	}	
}

void RecvChangeMoney( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	DWORD money = p->u.kein.change_money.money;

	DWORD old_money = 0;
	if(LocalMgr.IsChangeMoney())//030102 lsw
	{
		old_money = GetMoneyByOldItem( ch );
	}
	else
	{
		old_money = GetMoneyByItem( ch );
	}

	if( money > old_money ) return;		// 돈 도 없는 놈이 너무 많이 바꾼다.
	
	DWORD new_money = money * 1;//020730 lsw
	if( new_money )
	{
		if(::GiveMoney(ch,new_money))//지급 되었는가
		{
			SubtractOldMoney( money, ch );
			t_packet packet;
			packet.h.header.type = CMD_CHANGE_MONEY_OK;
			packet.u.kein.default_dword = new_money;
			packet.h.header.size = sizeof( new_money );
			QueuePacket(connections, cn, &packet, 1);
		}	
	}
}	

void SendEatChcolate( char *name, DWORD limit, DWORD cn )
{
	t_packet packet;
	packet.h.header.type = CMD_EAT_CHOCOLATE;
		strcpy(packet.u.kein.eat_chocolate.name, name);
		packet.u.kein.eat_chocolate.server_id = cn ;
		packet.u.kein.eat_chocolate.limit = limit;
	packet.h.header.size = sizeof( k_eat_chocolate ) - 20 + strlen( name )+1;
	QueuePacket(connections, DB_DEMON, &packet, 1);
}


void RecvEatChocolateOk( t_packet *p )
{
	t_packet packet;
	packet.h.header.type = CMD_EAT_CHOCOLATE_OK;
		packet.u.kein.default_int = p->u.kein.eat_chocolate_ok.chocolate_count;
	packet.h.header.size = sizeof( int );
	QueuePacket(connections, p->u.kein.eat_chocolate_ok.server_id, &packet, 1);
}

void RecvPayRumor( short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int ret = SubtractMoney( 500, ch );
	if( ret != 1 ) 
	{
		CallClient( cn, CMD_PAY_RUMOR_NOT );
		return ;
	}

	t_packet packet;			// DB로 요청
	packet.h.header.type = CMD_PAY_RUMOR;
		packet.u.kein.default_dword = cn;
	packet.h.header.size = sizeof( DWORD );
	QueuePacket(connections, DB_DEMON, &packet, 1);
}

void RecvAndSendRumor( t_packet *p )
{
	char *name = p->u.kein.login_check_chocolate.name;
	int count = p->u.kein.login_check_chocolate.count;
	int cn = p->u.kein.login_check_chocolate.server_id;


	t_packet packet;			// DB로 요청
	packet.h.header.type = CMD_PAY_RUMOR_OK;
		strcpy( packet.u.kein.check_chocolate.name, name) ;
		packet.u.kein.check_chocolate.count = count;
	packet.h.header.size = sizeof( k_check_chocolate ) - 20 + strlen( name )+1;
	QueuePacket(connections, cn, &packet, 1);
}


void MovePc( short int cn, int x, int y)
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	ch->X = x*TILE_SIZE + 16;
	ch->Y = y*TILE_SIZE + 16;

	ch->MoveSx = x;
	ch->MoveSy = y;
	ch->OldMoveSx = x;
	ch->OldMoveSy = y;
	ch->MoveGox = x;
	ch->MoveGoy = y;

	ch->MoveLength = ch->MovePathCount = 0;

	t_server_magic_position	smp;

	//클라이언트로 보낼 패킷 셋팅
	smp.nType =	PT_TELEPORT;
	smp.idTarget = WORD(cn);
	smp.nX =	x;
	smp.nY =	y;

	SendCMD_MAGIC_POSITION(&smp);
}

void SendOpenSChoolData( int cn, int item_no, int npc_id )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;
	
	t_packet packet;			// DB로 요청
	packet.h.header.type = CMD_OPENSCHOOL_DATA;
		strcpy( packet.u.kein.openschool_data.name, ch->Name);
		packet.u.kein.openschool_data.item_no = item_no;
		packet.u.kein.openschool_data.nation = ch->name_status.nation;
		packet.u.kein.openschool_data.server_id = cn;
		packet.u.kein.openschool_data.npc_id = npc_id;
	packet.h.header.size = sizeof( k_openschool_data ) - 20 + strlen( ch->Name )+1;
	QueuePacket(connections, DB_DEMON, &packet, 1);
}

void RecvOpenSchoolResult( t_packet *p )
{
	int server_id = p->u.kein.openschool_data_ok.server_id;
	int no = p->u.kein.openschool_data_ok.number;
	int item_no = p->u.kein.openschool_data_ok.item_no;
	int npc_id = p->u.kein.openschool_data_ok.npc_id;

	CHARLIST *ch = CheckServerId( server_id );
	if( !ch ) return;

	if( no == -1 ) // 다시 만들어 준다.
	{
		POS pos;
		int a, b, c;
		if( SearchInv( ch->inv, a, b, c ) )
		{
			ItemAttr *item = &ch->inv[a][b][c];
			*item = GenerateItem( item_no );
			SetItemPos( INV, a, b, c, &pos );
			SendServerEachItem( &pos, item, server_id );
		}
	}

	t_packet packet;
	packet.h.header.type = CMD_OPENSCHOOL_DATA_OK;
		packet.u.kein.openschool_data_result.number = (char) no;
		packet.u.kein.openschool_data_result.npc_id = npc_id;
	packet.h.header.size = sizeof( k_openschool_data_result );
	QueuePacket(connections, server_id, &packet, 1);
}

void RecvGuildExitGlobal( t_packet *p, short int cn )
{	//< CSD-030324
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	if( !CheckGuildPower( GDP_EXIT_GUILD_HIM, ch ) ) return;

	char *name = p->u.kein.default_name;

	t_packet packet;
	packet.h.header.type = CMD_GUILD_EXIT_COMMAND_GLOBAL;
		strcpy( packet.u.kein.guild_exit_global.name, name );
		packet.u.kein.guild_exit_global.server_id = cn;
		packet.u.kein.guild_exit_global.guild_code = ch->GetGuildCode();
	packet.h.header.size = sizeof( k_guild_exit_global )-20+strlen( name )+1;
	QueuePacket(connections, DB_DEMON, &packet, 1);
}	//> CSD-030324

void RecvGuildExitGlobalFromDB( t_packet *p )
{	//< CSD-030326
	char *name = p->u.kein.default_name;
	int cn = ExistHe( name );
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;		// 그런놈 없다.

	g_pGuildManager->DelMember(ch->GetGuildCode(), cn);

	ch->SetGuildCode(0); // CSD-030806
	ch->name_status.guild_master = 0;

	CallClient( cn, CMD_GUILD_EXIT_YOU );			// 탈퇴당한놈한테

	t_packet packet;								// 주변사람에게
	packet.h.header.type = CMD_GUILD_EXIT_WHO;
		packet.u.kein.default_short_int = cn;
		packet.h.header.size = sizeof( short int );
	CastMe2Other( cn, &packet );
}	//> CSD-030326

void SendEatCandy( char *name, DWORD limit, DWORD total_id )
{
	t_packet packet;
	packet.h.header.type = CMD_EAT_CANDY;
		strcpy(packet.u.kein.eat_candy.name, name);
		packet.u.kein.eat_candy.total_id = total_id;
		packet.u.kein.eat_candy.candy_limit = limit;
	packet.h.header.size = sizeof( k_eat_candy) - 20 + strlen( name )+1;
	QueuePacket(connections, DB_DEMON, &packet, 1);
}

// 010314 YGI
void RecvReqLoan( t_packet *p, short int cn )
{	//< CSD-030806
	CHARLIST* ch = CheckServerId(cn);
	
	if (ch == NULL)
	{
		return;
	}

	DWORD loan_money = p->u.kein.default_dword;			//	요청한 금액
	const int nLevel = ch->GetLevel();

	if ((loan_money + ch->LastLoan) > NPC_Lev_Ref[nLevel].bank_loan)
	{	// 요구 금액이 너무 크다.
		return;	
	}

	if (GiveMoney(ch,loan_money))
	{
		ch->LastLoan += loan_money;
		RecvLoanInfo(cn);
	}
}	//> CSD-030806

void RecvLoanInfo( short int cn )
{	//< CSD-030806
	CHARLIST* ch = CheckServerId(cn);

	if (ch == NULL)
	{
		return;
	}

	const int nLevel = ch->GetLevel();
	t_packet packet;
	packet.h.header.type = CMD_REQ_LOAN_INFO;
	packet.u.kein.loan_info.last_loan = ch->LastLoan;
	packet.u.kein.loan_info.max_loan = NPC_Lev_Ref[nLevel].bank_loan;
	packet.h.header.size = sizeof( k_loan_info );
	QueuePacket(connections, cn, &packet, 1);
}	//> CSD-030806

void RecvRepayment( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	DWORD money = p->u.kein.default_dword;
	if( ch->LastLoan < money ) money = ch->LastLoan;

//	ch->Money = GetMoneyByItem( ch );
	if( ch->Money < money ) money = ch->Money;

	SubtractMoney( money, ch );
	ch->LastLoan -= money;
	RecvLoanInfo( cn );
}

static int bbs_money[2][2] = { {100, 1000}, {1500, 15000} };
void RecvMerchantBBS( t_packet *p, short int cn ) // 클라이언트에서 받은걸 이름을 넣어서 현재 맵 모두에게 뿌려준다.
{	//< CSD-CN-031213
	CHARLIST* ch = CheckServerId(cn);
	
	if (ch == NULL)
	{
		return;
	}

	if (ch->Money < bbs_money[1][0])
	{
		return;
	}

	SubtractMoney(bbs_money[1][0], ch);

	t_packet packet;
	packet.h.header.type = p->h.header.type;
	packet.h.header.size = sizeof(k_name_message) - 200 + p->h.header.size;
	strcpy(packet.u.kein.name_message.name, ch->Name);
	strcpy(packet.u.kein.name_message.message, p->u.data);
	g_pUserManager->SendPacket(&packet);
}	//> CSD-CN-031213

void RecvMerchantBBSAllMap( t_packet *p, short int cn )		// 클라이언트로 받아서 타입과 이름을 넣고 모든 서버로 뿌려준다.
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	if( ch->Money < bbs_money[1][1] ) return;
	SubtractMoney( bbs_money[1][1], ch );

	int nation = ch->name_status.nation;
	//g_pServerTable->BroadCastToEveryServer

	t_packet packet;
	packet.h.header.type = CMD_MERCHANT_BBS_MAP_TO_MAP;
		strcpy( packet.u.kein.name_message.name, ch->Name );
		strcpy( packet.u.kein.name_message.message, p->u.data );
	packet.h.header.size = sizeof( k_name_message ) - 200 + p->h.header.size;

	for( int i = 0 ;  i< MAX_MAP_ ; i ++ )
	{
		if( MapInfo[i].nation == nation ) g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)MapInfo[i].port, (char *)&packet, packet.h.header.size+sizeof(t_header) );
	}
	
	packet.h.header.type = CMD_MERCHANT_BBS;
	g_pUserManager->SendPacket(&packet); // CSD-031213
}

void RecvMerchantItemBBS( t_packet *p, short int cn )		// 아이템 공지
{
	CHARLIST *ch = CheckServerId( cn );
	if(!ch ) return;

	if( ch->Money < bbs_money[0][0] ) return;
	SubtractMoney( bbs_money[0][0], ch );

	t_packet packet;
	packet.h.header.type = p->h.header.type;
	packet.h.header.size = sizeof( k_recv_merchant_item_bbs ) - 20+sizeof(ch->Name);
	strcpy( packet.u.kein.recv_merchant_item_bbs.szCharName, ch->Name );
	strcpy( packet.u.kein.recv_merchant_item_bbs.szItemName, p->u.kein.merchant_item_bbs.szItemName );
	packet.u.kein.recv_merchant_item_bbs.dwMoney = p->u.kein.merchant_item_bbs.dwMoney;
	packet.u.kein.recv_merchant_item_bbs.map_number = MapNumber;
	packet.u.kein.recv_merchant_item_bbs.pos_x = ch->X/TILE_SIZE;
	packet.u.kein.recv_merchant_item_bbs.pos_y = ch->Y/TILE_SIZE;

	g_pUserManager->SendPacket(&packet); // CSD-CN-031213
}

void RecvMerchantItemBBSAllMap( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	if( ch->Money < bbs_money[0][1] ) return;
	SubtractMoney( bbs_money[0][1], ch );

	int nation = ch->name_status.nation;

	t_packet packet;
	packet.h.header.type = CMD_MERCHANT_ITEM_BBS_MAP_TO_MAP;

		strcpy( packet.u.kein.recv_merchant_item_bbs.szCharName, ch->Name );
		strcpy( packet.u.kein.recv_merchant_item_bbs.szItemName, p->u.kein.merchant_item_bbs.szItemName );
		packet.u.kein.recv_merchant_item_bbs.dwMoney = p->u.kein.merchant_item_bbs.dwMoney;
		packet.u.kein.recv_merchant_item_bbs.map_number = MapNumber;
		packet.u.kein.recv_merchant_item_bbs.pos_x = ch->X/TILE_SIZE;
		packet.u.kein.recv_merchant_item_bbs.pos_y = ch->Y/TILE_SIZE;
	packet.h.header.size = sizeof( k_recv_merchant_item_bbs )-20+sizeof(ch->Name);

	SendNationPacket( &packet, nation );	
	packet.h.header.type = CMD_MERCHANT_ITEM_BBS;		// 자기 나라
	g_pUserManager->SendPacket(&packet); // CSD-CN-031213
}

inline void SendNationPacket( t_packet *p, int nation )		// 같은 나라의 맵서버에게 모두 보낸다.
{
	for( int i = 0 ;  i< MAX_MAP_ ; i ++ )
	{
		if( MapNumber == i ) continue;

		if( MapInfo[i].nation == nation ) 
			g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)MapInfo[i].port, (char *)p, p->h.header.size+sizeof(t_header) );
	}
}

void RecvPartyAllFree( short int cn )
{
	k_client_delete_party data;
	
	data.type = PARTY;
	for( int i=0; i<6; i++ )
	{
		data.ct = i;
		RecvDeleteParty( &data, connections, cn );		// 모두 해제 시킨다.
	}
}

void InitJoinGameWho(CHARLIST *ch)	// 누군가 이 맵으로 들어왔다.		// 010410 YGI
{	//< CSD-HK-030829
	const WORD idUser = ch->GetServerID();
	g_pUserManager->AddMember(idUser);
	g_pDualManager->AddMember(idUser);

	const int nGuildCode = ch->GetGuildCode();

	if (nGuildCode > 0)
	{
		g_pGuildManager->AddMember(nGuildCode, idUser);
	}
	
	if (g_pDungeonMap)
	{
		g_pDungeonMap->NewJoin(idUser);
	}
}	//> CSD-HK-030829

/////////////////////////////////////////////////////////////////////////////////////////////
// CAliveMapZoneAll
int CAliveMapZoneAll::CheckAlive( int ch_x, int ch_y, int map_number, int word_type )
{
	for( int i=0; i<m_Max; i++ )
	{
		if( m_Map[i].map_number != map_number ) continue;		
		if( m_Map[i].is_all ) return 100;
		if( ! TileMap[ch_x][ch_y].attr_inside ) // 내부 속성이 있는 곳이 아니라면
			continue;

		if( BoxAndDotCrash( m_Map[i].x,m_Map[i].y, m_Map[i].xl, m_Map[i].yl, ch_x, ch_y ) )
		{
			if( !m_Map[i].alive_word_type || word_type == m_Map[i].alive_word_type ) return 100; // 010612 YGI
			else return m_Map[i].alive_word_type;	// 살아나는 곳이긴 하지만 살아 주문이 틀렷다. 그럼 주문을 가르쳐 준다.
		}
	}

	return -1;		// 해당하는 곳이 아니다.
}

/////////////////////////////////////////////////////////////////////////////////////////////


int UpdateLadderScore( CHARLIST *ch )
{
	if( !ch->refresh_ladder ) return 0;

	t_packet packet;
	packet.h.header.type = CMD_UPDATE_LADDER_SCORE;
	packet.h.header.size = sizeof( k_update_ladder_score ) - 20 + strlen( ch->Name )+1;
	packet.u.kein.update_ladder_score.score = ch->LadderScore;
	packet.u.kein.update_ladder_score.naiton = ch->name_status.nation;
	packet.u.kein.update_ladder_score.cls = ch->Class;
	packet.u.kein.update_ladder_score.btLevel = ch->GetLevel(); // CSD-030806
	strcpy( packet.u.kein.update_ladder_score.name, ch->Name );
	QueuePacket( connections, DB_DEMON, &packet,1 );
	return 1;
}

void RecvReqRankLadder( t_packet *p, short int cn )
{
	int page = p->u.kein.default_int;

	t_packet packet;
	packet.h.header.type = CMD_REQ_RANK_LADDER;
		packet.u.kein.req_rank_ladder_to_db.page = page;
		packet.u.kein.req_rank_ladder_to_db.server_id = cn;

	packet.h.header.size = sizeof( k_req_rank_ladder_to_db );
	QueuePacket( connections, DB_DEMON, &packet,1 );	
}

void RecvDirectClient( t_packet *p )
{
	int server_id = p->u.kein.direct_client.server_id;

	t_packet packet;
	packet.h.header.type = p->u.kein.direct_client.type;
	packet.h.header.size = p->h.header.size - sizeof( int );
	if( packet.h.header.size )
		memcpy( packet.u.kein.data, p->u.kein.direct_client.data, packet.h.header.size );

	QueuePacket( connections, server_id, &packet,1 );
}

// 030919 HK YGI
#define CMD_CHECK_SALVATION_NAME			10388
void RecvDirectDB( t_packet *p, short int cn )
{
	//k_send_db_direct_map 구조체와 k_send_db_direct_client 구조체의 형태가 똑같기 때문에 

	int type = p->u.kein.send_db_direct_client.type;
	p->h.header.type = type;
	p->u.kein.send_db_direct_map.server_id = cn;

	// 030919 HK YGI
	switch (type)
	{	//< CSD-031210

	case CMD_GET_GUILD_ITEM : // 040623 YGI
		{
			if( LocalMgr.IsAbleNation( HONGKONG | TAIWAN | CHINA ) )
			{
				k_req_guild_item *pData =  (k_req_guild_item *)p->u.kein.send_db_direct_map.data;
				if( pData->index == 1 )
				{
					CHARLIST* ch = CheckServerId(cn);
					if( !ch ) return;
					if( !ch->IsUseableGuildItem() ) return;
					ch->dwUseTimeOfGuildItemMenu = g_curr_time + MAX_TIME_USE_GUILD_ITEM_MENU;
				}
			}

			break;
		}

	case CMD_CHECK_SALVATION_NAME :
		{
			CHARLIST* ch = CheckServerId(cn);
			
			if (ch != NULL)
			{	
				p->h.header.size = strlen(ch->Name) + 2;
				memcpy( p->u.kein.send_db_direct_map.data, ch->Name, 20 );
			}

			break;
		}
	//< 040331-YGI
	case CMD_REGIST_GUILD_MARK :
		{
			CHARLIST* ch = CheckServerId(cn);
			if( !ch ) 
			{
				MyLog( 0, "Wrong ch[%s], cn[%d] when CMD_REGIST_GUILD_MARK", connections[cn].name, cn );
				return;
			}
			if( !CheckGuildPower( GDP_CHANGE_MARK, ch ) )
			{
				MyLog( 0, "Wrong!! CMD_REGIST_GUILD_MARK : Name[%s], guild_code[%d], guild_degree[%d]", 
					ch->Name, ch->GetGuildCode(), ch->name_status.guild_master );
				return;
			}
			k_regist_guild_mark *pGuildData;
			pGuildData = (k_regist_guild_mark *)p->u.kein.send_db_direct_map.data;
			if( pGuildData->guild_code != ch->GetGuildCode() )
			{
				MyLog( 0, "Wrong!! CMD_REGIST_GUILD_MARK : Name[%s], guild_code[%d], guild_degree[%d], target_guild[%d]", 
					ch->Name, ch->GetGuildCode(), ch->name_status.guild_master, pGuildData->guild_code );
				return;
			}
			MyLog( 0, "Try change guild mark!! Name[%s], guild_code[%d], guild_degree[%d], target_guild[%d]", 
				ch->Name, ch->GetGuildCode(), ch->name_status.guild_master, pGuildData->guild_code );
			
			break;		// LTH-040406-KO 만약을 위해 넣었다. 밑에 또 추가되었을때 빠뜨릴까바...

		}//> 040331-YGI

	}	//> CSD-031210

	QueuePacket( connections, DB_DEMON, p,1 );
}

void RecvReqVisa( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int nation = p->u.kein.default_char;
	int new_nation_code;
	switch( nation )
	{
		case N_VYSEUS	:	new_nation_code = 0; break;
		case N_ZYPERN	:	new_nation_code = 1; break;
		case N_YILSE	:	new_nation_code = 2; break;
		default : return;		// 아직 지원 안되는 나라
	}

	int visa_item[] = { NationInfo[nation].visa_va, NationInfo[nation].visa_zy, NationInfo[nation].visa_ill};
	int visa_mer_item[] = { NationInfo[nation].visa_mer_va, NationInfo[nation].visa_mer_zy, NationInfo[nation].visa_mer_ill };

	int visa_pri_item[] = { NationInfo[nation].visa_pri_va, NationInfo[nation].visa_pri_zy, NationInfo[nation].visa_pri_ill}; 
	int visa_mer_pri_item[] = { NationInfo[nation].visa_mer_pri_va, NationInfo[nation].visa_mer_pri_zy, NationInfo[nation].visa_mer_pri_ill, };

	t_packet packet;
	k_visa_item *send_visa_item = &packet.u.kein.visa_item;
	for( int i=0, ct=0; i<3; i++ )
	{	
		if( i == new_nation_code ) continue;
		send_visa_item->item_no[ct] = visa_item[i];
		send_visa_item->item_no[ct+2] = visa_mer_item[i];

		send_visa_item->item_price[ct] = visa_pri_item[i];
		send_visa_item->item_price[ct+2] = visa_mer_pri_item[i];
		ct++;
	}
	packet.h.header.type = CMD_REQ_VISA;
	packet.h.header.size = sizeof( k_visa_item );
	QueuePacket( connections, cn, &packet,1 );
}

void RecvBuyVisa( t_packet *p, short int cn )
{
	int visa_no = p->u.kein.visa_buy.item_no;
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int nation = p->u.kein.visa_buy.nation;
	int visa_item[] = { NationInfo[nation].visa_va, NationInfo[nation].visa_zy, NationInfo[nation].visa_ill, 
						NationInfo[nation].visa_mer_va, NationInfo[nation].visa_mer_zy, NationInfo[nation].visa_mer_ill };

	int visa_pri_item[] = { NationInfo[nation].visa_pri_va, NationInfo[nation].visa_pri_zy, NationInfo[nation].visa_pri_ill, 
							NationInfo[nation].visa_mer_pri_va, NationInfo[nation].visa_mer_pri_zy, NationInfo[nation].visa_mer_pri_ill, };

	static int of_nation[] = { N_VYSEUS, N_ZYPERN, N_YILSE, N_VYSEUS, N_ZYPERN, N_YILSE };		// 비자의 원래 국가

	for( int i=0; i<6; i++ )
	{
		if( visa_item[i] == visa_no )
		{
			int ret = SubtractMoney( visa_pri_item[i], ch );
			if( ret == 1 )
			{
				ItemAttr item = GenerateItem( visa_item[i] );
				SendServerEachItemAuto( ch, &item );
				// 담당 나라 국고에 ++ 시킨다.
				SendNationMoney( NM_ADD, of_nation[i], visa_pri_item[i] );		// 국고를 변화시킨다.
				CallClient( cn, CMD_BUY_VISA_OK );
			}
			else
			{
				CallClient( cn, CMD_BUY_VISA_FAIL );
			}
			break;
		}
	}
}

void RecvExitGuildHimSelf( short int cn )		// 스스로 길드 탈퇴 명령어		// 010613 YGI
{	//< CSD-030326
	CHARLIST *ch = CheckServerId( cn );
	
	if (ch == NULL)  
	{
		return;
	}

	const int nGuildCode = ch->GetGuildCode(); 
	
	if (nGuildCode <= 0) 
	{
		return;
	}

	if (ch->name_status.guild_master == 1) 
	{
		return;	// 길마일 경우
	}
	// 길드를 탈퇴 시킨다.
	ch->SetGuildCode(0); // CSD-030806 

	t_packet packet;
	packet.h.header.type = CMD_GUILD_EXIT_WHO;
	packet.u.kein.default_short_int = cn;
	packet.h.header.size = sizeof(short int);
	QueuePacket(connections, cn, &packet, 1);	
	CastMe2Other(cn, &packet);		// 메시지를 날린다.
	// 길드 멤버 리스트 테이블에서 삭제
	SendSaveGuildMemberList(ch->Name, nGuildCode, 0, 0); 
	// guild_list 에서 삭제한다.
	g_pGuildManager->DelMember(nGuildCode, cn);
}	//> CSD-030326

int GetItemMuchByMuchItem( CHARLIST *ch, int item_no )
{
	ItemAttr *pItem = (ItemAttr *)ch->inv;
	int count = 0;
	for( int i=0; i<72; i++ ) 
	{
		if( pItem[i].item_no == item_no ) count += pItem[i].attr[IATTR_MUCH];
	}
	return count;
}

void RecvReqistGuild( t_packet *p, short int cn )
{	//< CSD-030326 : 정식 길드 등록
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	const int nGuildCode = ch->GetGuildCode();

	if (nGuildCode <= 0) 
	{
		return;	// 먼저 임시 길드에 등록해야 한다.
	}
	
	CGuildInfo* pGuild = g_pGuildManager->GetGuildInfo(nGuildCode);

	if (pGuild == NULL)
	{
		return;
	}

	if (pGuild->first != 1)
	{
		return;		// 임시 길드가 아니다.
	}

	if( ch->name_status.guild_master != 1 ) return;		// 길마가 아니다.
	//acer7
	if (GetItemMuchByMuchItem( ch, IN_RED_SIGNET ) < SIZE_GR_RS) 
	{
		return;		// 시그넷이 부족하다.
	}
	// 아이템을 지운다. 만들지 못하면 날아가버리는 수가 잇다. --> 복구
	SendDeleteMuchItemQuantity(ch, IN_RED_SIGNET, SIZE_GR_RS);
	p->u.kein.default_short_int = ch->GetGuildCode();		// 길드 코드 해킹을 막기 위해..

	t_packet packet;
	packet.h.header.type = CMD_REGIST_GUILD;
	packet.u.kein.send_db_direct_map.server_id = cn;
	memcpy( packet.u.kein.send_db_direct_map.data, p->u.kein.data, p->h.header.size );
	packet.h.header.size = p->h.header.size + 2;
	QueuePacket(connections, DB_DEMON, &packet,1);
}	//> CSD-030326

void RecvReqistGuildImsi( t_packet *p, short int cn )
{	//< CSD-030326
	CHARLIST *ch = CheckServerId( cn );
	if (!ch) return;
	if (ch->GetGuildCode()) return; // 이미 길드가 있다.
	if (ch->GetLevel() < BUILD_GUILD_LEVEL) return; // CSD-030806
	if (GetItemMuchByMuchItem(ch, IN_BLUE_SIGNET ) < SIZE_GR_BS) return;		// 시그넷이 부족하다.

	t_packet packet;
	packet.h.header.type = CMD_REGIST_GUILD_IMSI;
	packet.u.kein.regist_guild_imsi_db.server_id = cn;
	packet.u.kein.regist_guild_imsi_db.nation = ch->name_status.nation;
	strcpy( packet.u.kein.regist_guild_imsi_db.guild_name, p->u.kein.regist_guild_imsi.guild_name );
	strcpy( packet.u.kein.regist_guild_imsi_db.guild_ma, ch->Name );
	strcpy( packet.u.kein.regist_guild_imsi_db.guild_explain, p->u.kein.regist_guild_imsi.guild_explain);
	packet.h.header.size = sizeof( k_regist_guild_imsi_db ) - 1024 + strlen( p->u.kein.regist_guild_imsi.guild_explain );

	QueuePacket( connections, DB_DEMON, &packet,1 );	
}	//> CSD-030326

void RecvReqistGuildImsiOk( t_packet *p )	// 디비에서 임시 길드등록이 성공했다고 날아왔다.
{	//< CSD-030326
	const int server_id = p->u.kein.regist_guild_imsi_ok.server_id;
	const int nGuildCode = p->u.kein.regist_guild_imsi_ok.guild_code;

	CHARLIST *ch = CheckServerId(server_id);
	
	if (ch == NULL) 
	{
		return;
	}

	if (ch->GetGuildCode() > 0)
	{
		return;
	}
	// 길드 등록
	ch->SetGuildCode(nGuildCode); // CSD-030806
	// 길마 등록
	ch->name_status.guild_master = 1; 
	g_pGuildManager->AddMember(nGuildCode, server_id);
	// 아이템을 지운다. 
	SendDeleteMuchItemQuantity(ch, IN_BLUE_SIGNET, SIZE_GR_BS);
	
	CGuildInfo* pGuild = g_pGuildManager->GetGuildInfo(nGuildCode);

	if (pGuild == NULL)
	{
		return;
	}
	
	// 길마 등록에 관한 클라이언트 패킷 전송
	t_packet packet;
	packet.h.header.type = CMD_REGIST_GUILD_IMSI_OK;
	packet.u.kein.send_regist_imsi_guild_info.server_id = server_id;
	packet.u.kein.send_regist_imsi_guild_info.guild_code = nGuildCode;
	packet.u.kein.send_regist_imsi_guild_info.make = pGuild->make;
	strcpy(packet.u.kein.send_regist_imsi_guild_info.guild_name, pGuild->guild_name);
	packet.h.header.size = sizeof(k_send_regist_imsi_guild_info) - 20 + strlen(pGuild->guild_name);
	QueuePacket(connections, server_id, &packet, 1);
	CastMe2Other(server_id, &packet);
}	//> CSD-030326

void RecvSetGuildInfo(t_packet* p)	// 디비에서 날아온 바뀐 정보를 셋팅한다.
{	//< CSD-030326
	k_set_guild_info *guild_info = &p->u.kein.set_guild_info;
	
	const int nGuildCode = guild_info->guild_code;

	if (g_pGuildManager->IsExist(nGuildCode))
	{	// 존재하는 길드라면
		CGuildInfo* pGuild = g_pGuildManager->GetGuildInfo(nGuildCode);
		pGuild->on = 1;
		pGuild->first = guild_info->first;
		pGuild->active = guild_info->active;
		pGuild->mark_num = guild_info->mark_num;
		pGuild->make = guild_info->make;
		pGuild->nation = guild_info->nation;
		strcpy(pGuild->guild_name, guild_info->guild_name);
		
		for (int i = 0; i < 5; i++)
		{
			strcpy(pGuild->guild_degree[i], guild_info->guild_degree[i]);
		}
	}
	else
	{
		CGuildInfo* pGuild = CGuildInfo::CreateGuildInfo();
		pGuild->on = 1;
		pGuild->first = guild_info->first;
		pGuild->active = guild_info->active;
		pGuild->mark_num = guild_info->mark_num;
		pGuild->make = guild_info->make;
		pGuild->nation = guild_info->nation;
		strcpy(pGuild->guild_name, guild_info->guild_name);
	
		for (int i = 0; i < 5; i++)
		{
			strcpy(pGuild->guild_degree[i], guild_info->guild_degree[i]);
		}

		g_pGuildManager->AddGuild(nGuildCode, pGuild);
		return;
	}
}	//> CSD-030326

void RecvTest( t_packet *p, short int cn )
{
	t_packet packet;
	packet.h.header.type = 2001;
	packet.h.header.size = 0;
	QueuePacket( connections, DB_DEMON, &packet,1 );	
}

//acer7
bool ExitGuild(CHARLIST* ch)
{	//< CSD-030326
	const int nGuildCode = ch->GetGuildCode();
	
	if (nGuildCode > 0)
	{
		return false;
	}
	// 길드 멤버 리스트에서 삭제
	g_pGuildManager->DelMember(nGuildCode, ch->GetServerID());
	// 길드 멤버 리스트 테이블에서 삭제
	SendSaveGuildMemberList(ch->Name, nGuildCode, 0, 0);
	ch->SetGuildCode(0); // CSD-030806
	return true;
}	//> CSD-030326

void RecvGuildInfo( t_packet *p, short int cn )
{	//< CSD-030326
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int guild_code = p->u.kein.req_guild_info1.guild_code;
	CGuildInfo* pGuild = g_pGuildManager->GetGuildInfo(guild_code);

	if (pGuild == NULL)
	{
		return;
	}

	const int make = p->u.kein.req_guild_info1.make;
	const int mark_num = p->u.kein.req_guild_info1.mark_num;
	const int info_num = p->u.kein.req_guild_info1.info_num;
	const int first = p->u.kein.req_guild_info1.first;

	// 비교해서 보내줄 필요가 있으면 보내준다.
	int is_new = 0;

	if (!pGuild->active)	// 삭제된 길드이다.
	{	// 삭제된 길드다
		if( ch->GetGuildCode() == guild_code )
		{	
			if (ExitGuild(ch))
			{	
				SendPacketDefault(CMD_GUILD_INFO_DELETED, &guild_code, sizeof(short int), cn);
			}	
		}
		return;
	}
	// 새로 바뀐 길드이다.
	if (make != pGuild->make || pGuild->first != first)	
	{
		SendGuildName(guild_code, cn);
		is_new = 1;
	}
	// 예비길드는 더이상 정보를 보내주지 않는다.
	if (pGuild->first) 
	{
		return;						
	}
	// 이미지가 업데이트가 된적이 있다.
	if (is_new || mark_num != pGuild->mark_num)	
	{
		t_packet packet;
		packet.h.header.type = CMD_REQ_GUILD_IMAGE;
		packet.u.kein.regist_guild_imsi_ok.guild_code = guild_code;
		packet.u.kein.regist_guild_imsi_ok.server_id = cn;
		packet.h.header.size = sizeof( k_regist_guild_imsi_ok );
		QueuePacket( connections, DB_DEMON, &packet,1 );	
	}
	// 길드의 정보가 바뀐거다.
	if (is_new || info_num != pGuild->info_num)
	{	// 맵서버에 보내준다.
		t_packet packet;
		packet.h.header.type = CMD_GUILD_DEGREE_INFO;
		packet.u.kein.guild_degree_info.guild_code = guild_code;
		packet.u.kein.guild_degree_info.info_num = pGuild->info_num;
		memcpy(packet.u.kein.guild_degree_info.guild_degree, pGuild->guild_degree, sizeof(char)*5*20);

		packet.h.header.size = sizeof( k_guild_degree_info );
		QueuePacket( connections, cn, &packet,1 );
	}
}	//> CSD-030326

void SendGuildName(int guild_code, short int cn)
{	//< CSD-030326
	CGuildInfo* pGuild = g_pGuildManager->GetGuildInfo(guild_code);

	if (pGuild == NULL)
	{
		return;
	}

	t_packet packet;
	packet.h.header.type = CMD_GUILD_INFO_NAME;
	packet.u.kein.guild_info_name.guild_code = guild_code;
	packet.u.kein.guild_info_name.make = pGuild->make;
	packet.u.kein.guild_info_name.first = pGuild->first;
	strcpy(packet.u.kein.guild_info_name.guild_name, pGuild->guild_name);
	packet.h.header.size = sizeof(k_guild_info_name) - 20 + strlen(pGuild->guild_name);
	QueuePacket(connections, cn, &packet, 1);
}	//> CSD-030326

void RecvGuildNumberInfo( t_packet *p )
{	//< CSD-030326
	const int nGuildCode = p->u.kein.set_guild_number_info.guild_code;
	CGuildInfo* pGuild = g_pGuildManager->GetGuildInfo(nGuildCode);
	
	if (pGuild == NULL)
	{
		return;
	}

	const int type = p->u.kein.set_guild_number_info.type;
	const int number = p->u.kein.set_guild_number_info.number;
	
	switch (type)
	{
	case 0:		// 이미지 번호가 바꼈다.
		{
			pGuild->mark_num = number;
			break;
		}
	case 1:		// 메이크 번호가 바꼈다.
		{
			
			pGuild->make = number;
			break;
		}
	case 2:
		{
			pGuild->info_num = number;
			break;
		}
	}
}	//> CSD-030326

void RecvGuildDegreeInfo2( t_packet *p )
{	//< CSD-030326
	const int nGuildCode = p->u.kein.guild_degree_info.guild_code;
	CGuildInfo* pGuild = g_pGuildManager->GetGuildInfo(nGuildCode);

	if (pGuild == NULL)
	{
		return;
	}
	
	memcpy(pGuild->guild_degree, p->u.kein.guild_degree_info.guild_degree, sizeof(char)*5*20);

	const int info_num = p->u.kein.guild_degree_info.info_num;
	pGuild->info_num = info_num;
	// 모든 길드원에게 새로운 정보를 보내준다.
	t_packet packet;
	packet.h.header.type = CMD_REFRESH_GUILD_CODE;
	packet.h.header.size = sizeof(short int);
	packet.u.kein.guild_instroduction_basic.guild_code = nGuildCode;
	g_pGuildManager->SendPacket(nGuildCode, &packet);
}	//> CSD-030326

void RecvGuildDegreeInfo( t_packet *p )
{	//< CSD-030326
	const int nGuildCode = p->u.kein.guild_degree_info.guild_code;
	CGuildInfo* pGuild = g_pGuildManager->GetGuildInfo(nGuildCode);

	if (pGuild == NULL)
	{
		return;
	}

	memcpy(pGuild->guild_degree, p->u.kein.guild_degree_info.guild_degree, sizeof(char)*5*20);
	const int info_num = p->u.kein.guild_degree_info.info_num;	
	pGuild->info_num = info_num;
	pGuild->first = 0;	// 임시길드가 아니다.
	
	// 모든 길드원에게 새로운 정보를 보내준다.
	t_packet packet;
	packet.h.header.type = CMD_REGIST_GUILD_OK;
	packet.h.header.size = sizeof( k_guild_instroduction_basic );
	packet.u.kein.guild_instroduction_basic.guild_code = nGuildCode;
	strcpy(packet.u.kein.guild_instroduction_basic.guild_name, pGuild->guild_name);
	g_pGuildManager->SendPacket(nGuildCode, &packet);
}	//> CSD-030326

void RecvGuildIntroduction( short int cn )
{	//< CSD-030326
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	t_packet packet;

	packet.h.header.type = CMD_REQ_GUILD_INTRODUCTION;
	int count = 0;

	CGuildManager::HASH_GUILD mpGuild = g_pGuildManager->GetGuildSet();
	
	for (CGuildManager::ITOR_GUILD i = mpGuild.begin(); i != mpGuild.end(); ++i)
	{
		CGuildInfo* pGuild = i->second;

		if (pGuild->active && ch->name_status.nation == pGuild->nation)
		{
			packet.u.kein.guild_instroduction.guild[count].guild_code = i->first;
			strcpy(packet.u.kein.guild_instroduction.guild[count].guild_name, pGuild->guild_name);
			count++;
		}

		if( count == 100 )
		{
			packet.u.kein.guild_instroduction.count = count;
			packet.h.header.size = sizeof( short int ) + sizeof( k_guild_instroduction_basic )*count;
			QueuePacket( connections, cn, &packet,1 );
			packet.h.header.type = CMD_REQ_GUILD_INTRODUCTION_PLUS;
			count = 0;
		}
	}
	packet.u.kein.guild_instroduction.count = count;
	packet.h.header.size = sizeof( short int ) + sizeof( k_guild_instroduction_basic )*count;
	QueuePacket( connections, cn, &packet,1 );
}	//> CSD-030326

void SendSaveGuildMemberList( char *target_name, int old_guild_code, int guild_code, int degree )
{
	t_packet packet;
	packet.h.header.type = CMD_SAVE_GUILD_MEMBER_LIST;
		packet.u.kein.save_guild_member_list.guild_code = guild_code;
		packet.u.kein.save_guild_member_list.old_guild_code = old_guild_code;
		packet.u.kein.save_guild_member_list.degree = degree;
		strcpy( packet.u.kein.save_guild_member_list.name, target_name );
	packet.h.header.size = sizeof( k_save_guild_member_list );
	QueuePacket(connections, DB_DEMON, &packet, 1);
}

void TransDbGuildMailId( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	DWORD mail_id = p->u.kein.default_dword;

	t_packet packet;
	packet.h.header.type = p->h.header.type;
		packet.u.kein.guild_mail_id.guild_mail_id= mail_id;
		strcpy( packet.u.kein.guild_mail_id.name, ch->Name );
	packet.h.header.size = sizeof( k_guild_mail_id );
	QueuePacket(connections, DB_DEMON, &packet, 1);
}

void TransDB_DeleteGuildMail( DWORD mail_id, int cn )
{	//< CSD-030324
	LPCHARLIST ch = CheckServerId( cn );
	if( !ch ) return;

	if( !ch->GetGuildCode() ) return;
	//if( !ch->name_status.guild_master ) return;

	t_packet packet;
	packet.h.header.type = CMD_RECV_MAIL_DELETE_GUILD;
		packet.u.kein.delete_guild_mail.server_id = cn;
		packet.u.kein.delete_guild_mail.guild_code = ch->GetGuildCode();
		packet.u.kein.delete_guild_mail.degree = ch->name_status.guild_master;
		packet.u.kein.delete_guild_mail.mail_id = mail_id;
	packet.h.header.size = sizeof( k_delete_guild_mail );
	QueuePacket(connections, DB_DEMON, &packet, 1);
}	//> CSD-030324

void RecvCheckGuild( t_packet *p, short int cn )
{	//< CSD-030324
	int guild_code = p->u.kein.default_short_int;
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	if( ch->GetGuildCode() == guild_code ) return;
	
	k_guild_mail_unit guild_info;
	guild_info.guild_code = ch->GetGuildCode();
	guild_info.degree = ch->name_status.guild_master;
	SendPacketDefault( CMD_CHECK_MY_GUILD, &guild_info, sizeof( k_guild_mail_unit ), cn );
}	//> CSD-030324

void RecvGuildChatMap( t_packet *p )
{	//< CSD-030326
	const int nGuildCode = p->u.kein.guild_chat_map.guild_code;
	const char* name =  p->u.kein.guild_chat_map.name;
	const char* msg = p->u.kein.guild_chat_map.msg;

	t_packet packet;
	packet.h.header.type = CMD_GUILD_CHAT;
	packet.h.header.size = sizeof(k_guild_chat) - 1024 + strlen(msg);
	strcpy(packet.u.kein.guild_chat.name, name);
	strcpy(packet.u.kein.guild_chat.msg, msg);
	g_pGuildManager->SendPacket(nGuildCode, &packet);
}	//> CSD-030326

void RecvGuildChat( t_packet *p, short int cn )
{	//< CSD-030324
	LPCHARLIST ch = CheckServerId( cn );
	if( !ch ) return;

	if( !ch->GetGuildCode() ) return;

	char *msg = (char *)p->u.data;
	t_packet packet;
	packet.h.header.type = CMD_GUILD_CHAT_MAP;
	packet.h.header.size = sizeof( k_guild_chat_map ) - 1024 + strlen(msg );
	packet.u.kein.guild_chat_map.guild_code = ch->GetGuildCode();
	strcpy( packet.u.kein.guild_chat_map.name, ch->Name );
	strcpy( packet.u.kein.guild_chat_map.msg, msg );

	g_pServerTable->BroadCastToEveryServer( (char*)&packet, (sizeof(t_header)+packet.h.header.size), SERVER_TYPE_MAP );
	RecvGuildChatMap( &packet );
}	//> CSD-030324

////////////////////////////////////////////////////////////////////////////////////
// 어빌 재분배 루틴

extern inline int GetTotalAbility(LPCHARLIST ch);
inline int GetTotalAbility( LPCHARLIST ch )
{
	return ( ch->Str+ch->Con+ch->Dex+ch->Wis+ch->Int+ch->MoveP+
		ch->Char+ch->Endu+ch->Moral+ch->Luck+ch->wsps+ch->GetReservedPoint());
}

void AddAbility( CHARLIST *ch, int type, int add )
{
	short int *ability;
	switch( type )
	{
		case STR :  ability = &ch->Str; break;
		case DEX :  ability = &ch->Dex; break;
		case CON :  ability = &ch->Con; break;
		case WIS :  ability = &ch->Wis; break;
		case INT_ : ability = &ch->Int; break;
		case CHA :  ability = &ch->Char; break;
		case MOVP : ability = &ch->MoveP; break;
		case ENDU : ability = &ch->Endu; break;
		case MOR :  ability = &ch->Moral; break;
		case WSPS : ability = &ch->wsps; break;
		case LUCK : ability = &ch->Luck; break;
		default : return;
	}
	(*ability) += add;
	return;
}

void RecvReqResetAbility( short int cn )
{
	LPCHARLIST ch = CheckServerId( cn ); 
	if( !ch ) return;
	
	static int class_ability[5][5] = { 
								{ STR,	CON }, 
								{ DEX,	MOVP},
								{ DEX,	CON },
								{ INT_,	WSPS},
								{ WIS,	WSPS} 
	};

	static int basic_ability[5][11] = {
			{ 50, 10, 30, 10, 10, 10, 10, 10, 10, 10, 10 },
			{ 10, 40, 20, 10, 10, 10, 20, 10, 10, 10, 20 },
			{ 10, 50, 20, 10, 10, 10, 20, 10, 10, 10, 10 },
			{ 10, 10, 15, 10, 40, 10, 10, 20, 10, 25, 10 },
			{ 10, 10, 20, 35, 10, 10, 10, 20, 10, 25, 10 },
	};

	int total = GetTotalAbility( ch );
	int ability = 0;
	ability += ch->Str = basic_ability[ch->Class][0];
	ability += ch->Dex = basic_ability[ch->Class][1];
	ability += ch->Con = basic_ability[ch->Class][2];
	ability += ch->Wis = basic_ability[ch->Class][3];
	ability += ch->Int = basic_ability[ch->Class][4];
	ability += ch->Char = basic_ability[ch->Class][5];
	ability += ch->MoveP = basic_ability[ch->Class][6];
	ability += ch->Endu = basic_ability[ch->Class][7];
	ability += ch->Moral = basic_ability[ch->Class][8];
	ability += ch->wsps = basic_ability[ch->Class][9];
	ability += ch->Luck = basic_ability[ch->Class][10];

	//for( int i=0; i<11; i++ ) ability += basic_ability[ch->Class][i];

	AddAbility( ch, class_ability[ch->Class][0], 3 );
	AddAbility( ch, class_ability[ch->Class][1], 2 );
	ability+=5;
	ch->SetReservedPoint(total-ability);
	SendCharInfo( cn );
}

int SendCharInfo( short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return 0;

	t_packet packet;
	packet.h.header.type = CMD_REQ_RESET_ABILITY;
		packet.u.kein.reset_ability.Str  = ch->Str  ;
		packet.u.kein.reset_ability.Con  = ch->Con  ;
		packet.u.kein.reset_ability.Dex  = ch->Dex  ;
		packet.u.kein.reset_ability.Wis  = ch->Wis  ;
		packet.u.kein.reset_ability.Int  = ch->Int  ;
		packet.u.kein.reset_ability.MoveP= ch->MoveP;
		packet.u.kein.reset_ability.Char = ch->Char ;
		packet.u.kein.reset_ability.Endu = ch->Endu ;
		packet.u.kein.reset_ability.Moral= ch->Moral;
		packet.u.kein.reset_ability.Luck = ch->Luck ;
		packet.u.kein.reset_ability.wsps = ch->wsps ;
    packet.u.kein.reset_ability.reserved_point	= ch->GetReservedPoint();
	packet.h.header.size = sizeof( k_reset_ability );
	QueuePacket(connections, cn, &packet, 1);
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////

void RecvDeleteGuild( t_packet *p, short int cn )
{	//< CSD-030324
	CHARLIST *ch = CheckServerId( cn ); 
	if( !ch ) return;
	if( ch->name_status.guild_master != 1 ) return;
	if( !ch->GetGuildCode() ) return;

	// 021008 YGI
	t_packet packet;
	packet.h.header.type = p->h.header.type;
	packet.u.kein.guild_instroduction_basic.guild_code = ch->GetGuildCode();
	strcpy( packet.u.kein.guild_instroduction_basic.guild_name, ch->Name );
	packet.h.header.size = sizeof( k_guild_instroduction_basic );
	QueuePacket(connections, DB_DEMON, &packet, 1);
}	//> CSD-030324

// 길드가 삭제 됐다는 것을 모든 유저에게 알려 준다.
void RecvDeleteGuildOk( t_packet *p )
{	//< CSD-CN-031213
	const int nGuildCode = p->u.kein.default_short_int;
	g_pGuildManager->DelGuild(nGuildCode);

	t_packet packet;
	packet.h.header.type = CMD_GUILD_INFO_DELETED;
	packet.u.kein.default_short_int = nGuildCode;
	packet.h.header.size = sizeof(short int);
	g_pUserManager->SendPacket(&packet);
}	//> CSD-CN-031213

void RecvGetGhostQuestInfo( short int cn )
{
	t_packet packet;
	packet.h.header.type = CMD_GET_GHOST_QUEST_INFO;
	packet.u.data[0] =	var[cn][s_GhostLvDef];
	packet.u.data[1] =	var[cn][s_GhostEndCountDef];
	packet.h.header.size = sizeof( short int );
	QueuePacket(connections, cn, &packet, 1);
}

//////////////////////////////////////////////////////////////
// 겜블
int CGambleRateData::LoadData()
{
	char file[MAX_PATH];
	sprintf( file, "%s/data/GambleRateData.bin", GameServerDataPath );
	FILE *fp = fopen( file, "rb" );
	if( fp )
	{
		fread( &g_game_rate_data, sizeof( k_GambleRateDataBasic ), 1, fp );
		fclose(fp);
	}
	else
	{
		memcpy( rune2band, g_rune_band, sizeof( int )*8 );
		memcpy( lv2band, g_lv2band, sizeof( int )*10 );
		memcpy( choice_house, g_choice_house, sizeof( int )*10*5 );
		memcpy( house_band_class, g_house_band_class, sizeof( int )*5*11*6);
		memcpy( item2house_type, g_item2house_type, sizeof( int )*10*6);
		memcpy( give_gamble_item_rate, g_give_gamble_item_rate, sizeof( int )*16*2);
		memcpy( give_gamble_item_rate, g_give_gamble_item_rate, sizeof( int )*16*2);
		memcpy( map2band, g_map2band, sizeof( int ) * 6*2 );
		memcpy( create_today_item, g_create_today_item, sizeof( int ) * 8 );
		get_today_item = g_get_today_item;
	}
	return 1;
}

int CGambleRateData::SaveData()
{
	FILE *fp = fopen( "./output/GambleRateData.bin", "wb" );
	if( fp )
	{
		fwrite( &g_game_rate_data, sizeof( k_GambleRateDataBasic ), 1, fp );
		fclose(fp);
	}
	return 1;
}
/////////////////////////////////////////////////


int GetLevelBand( int lv, int map_count, int rune )		// 자신의 레벨과 그 맵에 접속한 수에 따른 등급 결정
{
	int para = (lv-1)/10;
	if( para > 9 ) para = 9;
	int basic = g_game_rate_data.lv2band[para];

	para = 0;
	for( int i=0; i<6; i++ )
	{
		if( map_count < g_game_rate_data.map2band[i][0] )
		{
			para = g_game_rate_data.map2band[i][1];
			break;
		}
	}
	basic += para;
	basic += g_game_rate_data.rune2band[rune];	

	if( basic < 0 ) basic = 0;
	if( basic > 10 ) basic = 10;
	return basic;
}

// 1차원 배열을 넘기고 그 한계를 넘겨 주면 값을 비교하여 그 값을 넘지 않는 가장 최대
// 의 첨자 값을 반환한다.
inline int ChoiceParam( int *arg, int arg_count, int cmp_value )
{
	int count = 0;		// 누적 변수
	for( int i=0; i<arg_count; i++ )
	{
		count += arg[i];
		if( cmp_value < count ) return i;
	}
	return 0;
}

void ProcGambleItem( int house_type, CHARLIST *ch )
{
	int lv = ch->GetLevel(); // CSD-030806
	int user_count = (g_pServerTable)?g_pServerTable->GetNumOfUsersInServerSet():0;

	//int house_max = sizeof(choice_house) / sizeof(int) * 5;

	// 1. house_type 에 따른 방선택
	int house_rand = rand()%1000;
	int house = ChoiceParam( g_game_rate_data.choice_house[house_type], 5, house_rand );

	// 2. 레벨과 접속 수에 따른 높은 선택율을 가리키는 band선택
	int lv_band = GetLevelBand( lv, user_count, (int)ch->select_rune );

	int cls_rand = rand()%100;
	int cls_param = ChoiceParam( g_game_rate_data.house_band_class[house][lv_band], 6, cls_rand );

	// house 변수와 cls_param 변수를 이용한 도박
	const int item_no = g_GambleItem.GetRandItem( house, cls_param );

	if (item_no == -1)
	{	//< CSD-040223
		MyLog(LOG_NORMAL, "[Gamble Item] house(%d), cls_param(%d)", house, cls_param);
		return;
	}	//> CSD-040223
	
	ItemAttr item = MakeGambleItem( house, cls_param, item_no );
	g_pLogManager->SaveLogRoot_GambleLogFile( ch, house, cls_param, user_count, &item, house_type, lv_band );
	SendMessageGambleItem( ch, &item );

	SendItemEventLog( &item, ch->GetServerID(), SN_NOT_USER, SILT_GET_GAMBLE_ITEM, 2 ); //YGI acer
	MoveEmptyInv( &item, ch );
}

void RecvSearchDualCharacter( t_packet *p, short int cn )
{	//< CSD-030415
	CHARLIST *ch = CheckServerId( cn ); 
	if( !ch ) return;

	t_packet packet;
	char &count = packet.u.kein.scan_target.count;

	char type = 0;
	if	( !ch->IsDual() ) 
	{
		type = 1;		// 듀얼이 아닙니다.
		return;
	}
	if( ch->GetClassStep() == 1 )
	{
		if( !var[cn][s_GhostEndCountDef] ) 
		{
			type = 2;	// 퀘스트 진행중이 아닙니다.
			return;
		}
	}

	packet.h.header.type = CMD_SCAN_TARGET;	
	count = 0;

	const int nDualStep = ch->GetClassStep();
	CDualClassInfo* pDualClass = g_pDualManager->GetDualClassInfo(nDualStep);

	if (pDualClass == NULL)
	{
		return;
	}

	DWORD idMember = 0;
	CDualClassInfo::LIST_MEMBER ltMember = pDualClass->GetDualClassList();

	for (CDualClassInfo::ITOR_MEMBER i = ltMember.begin(); i != ltMember.end(); ++i)
	{	//< CSD-HK-030829
		const WORD idTarget = *i;
		CHARLIST* pTarget = CheckServerId(idTarget);

		if (pTarget == NULL)
		{
			continue;
		}

		if (cn == idTarget)
		{
			continue;
		}

		packet.u.kein.scan_target.xy[count].x = pTarget->X/TILE_SIZE*200/g_Map.file.wWidth;
		packet.u.kein.scan_target.xy[count].y = pTarget->Y/TILE_SIZE*200/g_Map.file.wHeight;
		
		if (++count >= 250)
		{
			break;
		}
	}	//> CSD-HK-030829
	
	packet.h.header.size = sizeof( k_scan_target ) - sizeof( k_x_y )*(250-count); ;
	QueuePacket(connections, cn, &packet, 1);
}	//> CSD-030415

// << 011019 ygi
void RecvChangeColor( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int type1 = p->u.kein.change_color.type1;		// 돈을 내는가 아닌가
	int type2 = p->u.kein.change_color.type2;		// 바디칼라인지 옷 칼라 인지
	int r = p->u.kein.change_color.r;
	int g = p->u.kein.change_color.g;
	int b = p->u.kein.change_color.b;

	if( type1 )		// 돈을 내야 한다면
	{	//< CSD-030806
		const int nLevel = ch->GetLevel();
		
		if (ch->Money < NPC_Lev_Ref[nLevel].nDyeingPrice)
		{
			return;
		}
		
		SubtractMoney(NPC_Lev_Ref[nLevel].nDyeingPrice, ch);
	}	//> CSD-030806
	if( type2 )		// 옷색깔 바꾸기
	{
		ch->BodyR = r;
		ch->BodyG = g;
		ch->BodyB = b;
	}
	else
	{
		ch->ClothR = r;
		ch->ClothG = g;
		ch->ClothB = b;
	}

	t_packet packet;		// DB 로 저장한다. 
	packet.h.header.type = CMD_CHANGE_COLOR;
		packet.u.kein.change_color_db.type = type2;		// 몸인지 띠 인지 선택
		packet.u.kein.change_color_db.r = r;
		packet.u.kein.change_color_db.g = g;
		packet.u.kein.change_color_db.b = b;
		strcpy( packet.u.kein.change_color_db.name, ch->Name );
	packet.h.header.size = sizeof( k_change_color_db );
	QueuePacket(connections, DB_DEMON, &packet, 1);

	// 유저에게 알려 준다.
	packet.h.header.type = CMD_CHANGE_COLOR;
		packet.u.kein.change_color_map2client.type = type2;		// 몸인지 띠 인지 선택
		packet.u.kein.change_color_map2client.r = r;
		packet.u.kein.change_color_map2client.g = g;
		packet.u.kein.change_color_map2client.b = b;
		packet.u.kein.change_color_map2client.server_id = cn;
	packet.h.header.size = sizeof( k_change_color_map2client );
	QueuePacket(connections, cn, &packet, 1);
	CastMe2Other( cn, &packet );
}

void RecvGetNationMoney( short int cn, int nation )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	if( nation <= 0 )
	{
		nation = ch->name_status.nation;
	}

	t_packet packet;		// DB 로 저장한다. 
	packet.h.header.type = CMD_REP_NATION_MONEY;
		packet.u.kein.nation_money.nation_type = nation;

		if( nation >= N_NATION_ALL )		// 모든 국가의 정보를 요청했을 경우 
		{
			packet.u.kein.nation_money.nation_money[0] = NationInfo[3].nation_money;
			packet.u.kein.nation_money.nation_money[1] = NationInfo[4].nation_money;
			packet.u.kein.nation_money.nation_money[2] = NationInfo[6].nation_money;
			packet.h.header.size = sizeof( k_nation_money );
		}
		else
		{
			packet.u.kein.nation_money.nation_money[0] = NationInfo[nation].nation_money;
			packet.h.header.size = sizeof( k_nation_money ) - sizeof(DWORD)*2;
		}

	QueuePacket(connections, cn, &packet, 1);
}

inline void SendDirectMap( t_packet *p, short int port )
{
	if( port == g_MapPort )
		HandleCommand( CN_FROM_OTHER_MAPSERVER_MSG, p );
	else 
		g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)port, (char *)p, p->h.header.size+sizeof(t_header) );
}

int InitRefreshMenu()
{
	if( g_Money_to_Fame.LoadData() )
	{
		MyLog( LOG_NORMAL, "%s/data/money_fame.txt loding............ ok", GameServerDataPath  );
	}
	else
	{
		MyLog( LOG_NORMAL, "%s/data/money_fame.txt loding............ Fail !!!!!!", GameServerDataPath  );
	}
	g_game_rate_data.LoadData();
	RefreshTodayItem();
	LoadMapPotal();
	LoadNationItem(g_wday );
	LoadGuildHouse();
	g_EventMgr.LoadEvent();
	InitItemLog( GetManagementMapPort( MM_SAVE_LOG_FILE_MAP ) );

	// 021117 YGI
	g_LightVersion.LoadLightVersionMap();

	// 030624 YGI
	extern bool InitEventFlag();
	InitEventFlag();
	//< CSD-031229
	//ItemMgr.LoadHaveLimitItem();	// 031009 CI YGI
	//> CSD-031229
	return 1;
}

int InitOnlyStart_kein()		// 맨처음에만 로딩할 것들
{
	LoadDungeonMap();
	LoadDungeonMapAll();
	
	return 1;
}

// DB에 넣어 두기 힘든 데이타를 소스에서 생성시킬때
void __stdcall MakeMapDataFile(DWORD dwValue)//020511 lsw	// VK_F7
{
	g_game_rate_data.SaveData();
	MyLog( LOG_NORMAL, " " );
	MyLog( LOG_NORMAL, "Make Server data files.... ok" );
	MyLog( LOG_NORMAL, " " );

	// 011130 YGI
	extern void LoadDRAGON_MAX_CONNECTIONS();
	LoadDRAGON_MAX_CONNECTIONS();

}

void RecvGambleItem( t_packet *p, short int cn )
{
	int count = p->u.kein.gamble_item.count;
	POS *pos =  p->u.kein.gamble_item.pos;

	if( !count ) return;

	CHARLIST *ch = CheckServerId( cn );
	if(!ch ) return;

	ItemAttr *item;
	CItem	*t;

	int gen1=0;			// 일반 겜블 아이템
	int gen2=0;			// 일반 겜블 아이템 2
	int grade1=0;		// 그래이드 겜블 1
	int grade2=0;		// 그래이드 겜블 2
	int gen_item=0;		// 일반 아이템
	int grade_item=0;	// 레어 아이템
	for( int i=0; i<count; i++ )
	{
		item = GetItemByPOS( cn, pos[i] );
		if( !item ) continue;

		t = ItemUnit( *item );
		if( !t ) continue;

		// 040601 YGI
		if( GetAttr2( item->attr[IATTR_ATTR], IA2_ITEMMALL_ITEM ) )
		{
			//아이템 몰 아이템은 겜블에 사용하지 못한다.
			continue;
		}

		if( item->attr[IATTR_RARE_MAIN] )	grade_item++;// 레어다..
		else if( t->GetItemKind() == IK_GRADE_GAMBLE ) grade1++;
		else if( t->GetItemKind() == IK_GENERAL_GAMBLE ) gen1++;
		else gen_item++;

		SendItemEventLog( item, cn, SN_NOT_USER, SILT_PUT_GAMBLE_ITEM, 4 ); //YGI acer
		DeleteItem( item );
	}

	ItemAttr today_item = g_TodayGambleItem.GetTodayItem();
	if( grade1 >= 5 && today_item.item_no )
	{
		if( WinOfRandFunction( g_game_rate_data.get_today_item ) )
		{
			g_pLogManager->SaveLogRoot_GambleLogFile( ch, 100, 100, 100, &today_item, 100, 100);
			SendMessageGambleItem( ch, &today_item );
			
			// 전체 메시지 날리기
			t_packet packet;
			packet.h.header.type = CMD_TODAY_ITEM_GET_WHO;
			packet.u.kein.today_gamble_item.item = today_item;
			packet.u.kein.today_gamble_item.nation = ch->name_status.nation;
			strcpy( packet.u.kein.today_gamble_item.name, ch->Name );
			packet.h.header.size = sizeof( k_today_gamble_item ) - 20 +sizeof( ch->Name );
			SendPacket2Maps( &packet );
 			g_pUserManager->SendPacket(&packet); // CSD-CN-031213

			SendItemEventLog( &today_item, cn, SN_NOT_USER, SILT_GET_TODAY_ITEM, 2 ); //YGI acer
			MoveEmptyInv( &today_item, ch );
			RecvDeleteTodayGambleItem();
			return;
		}
	}

	int house_type = -1;
	for(int i=9; i>=0; i-- )
	{
		if(	( gen1 >= g_game_rate_data.item2house_type[i][0] ) &&
			( gen2 >= g_game_rate_data.item2house_type[i][1] ) &&
			( grade1 >= g_game_rate_data.item2house_type[i][2] ) &&
			( grade2 >= g_game_rate_data.item2house_type[i][3] ) &&
			( gen_item >= g_game_rate_data.item2house_type[i][4] ) &&
			( grade_item >= g_game_rate_data.item2house_type[i][5] ) )
		{
			house_type = i; 
			break;
		}
	}
	if( house_type == -1 )
	{
		char result = 1;		// 겜블용 아이템이 없다.
		SendDefaultResult( CMD_GAMBLE_ITEM_RESULT, result, cn );
		return;
	}
	ProcGambleItem( house_type, ch );
}

ItemAttr MakeGambleItem( int house, int grade, int item_no )
{
	ItemAttr item;
	RareMain RareAttr;
	switch( house )
	{
		default :
			item = GenerateItem( item_no );
			break;
		case 4:	
			item = ItemMgr.GiveRareItem( item_no, 0, RARE_ABLE|RARE_ABLE_GAMBLE_ROOM_4, 2, grade, grade, RareAttr ,H_LV_HIGH_ITEM);//020725 lsw
			break;
		case 3:
			item = ItemMgr.GiveRareItem( item_no, 0, RARE_ABLE|RARE_ABLE_GAMBLE_ROOM_5, 1, grade, grade, RareAttr ,H_LV_HIGH_ITEM);//020725 lsw
			break;
	}

	return item;
}

/* 040715_KJHuNs 겜블로그를 g_pLogManager로 옮겨서 사용.(정리목적)
void SaveGambleLogFile( CHARLIST *ch, int house, int class_type, int user_count, ItemAttr *item, int resource_type, int lv_band )
	//> CSD-TW-030622
*/

void RecvGambleStart( short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	ch->select_rune = rand()%8;	
	SendPacketDefault( CMD_GAMBLE_START, &ch->select_rune, sizeof( BYTE ), cn );
}

void SendMessageGambleItem( CHARLIST *ch, ItemAttr *item )
{
	t_packet packet;
	packet.h.header.type = CMD_GAMBLE_ITEM_RESULT;
		packet.u.kein.gamble_item_result.result_type = 2;		// 어떤 메시지 인지 결과 타입 
		packet.u.kein.gamble_item_result.item_no = item->item_no;
		strcpy( packet.u.kein.gamble_item_result.name, ch->Name );
	packet.h.header.size = sizeof( k_gamble_item_result );
	QueuePacket(connections, ch->GetServerID(), &packet, 1);
}

void RecvLogoutChar( t_packet *p, short int cn )
{
	char *name = p->u.kein.default_name;
	int who = ExistHe( name );
	CHARLIST *target = CheckServerId( who );
	if( target )
	{
		// 강제 로그 아웃
		SendPutMenuString( KM_FAIL, 87, who );		// 강제 종료 당하는 사람한테
		closeconnection( connections, who, -33 );	// 운영자에 의한 강제 종료

		if( cn == CN_FROM_OTHER_MAPSERVER_MSG )		// 다른 맵에서 온거라면
		{
			int server_id = p->u.kein.focuse_logout_map.cn;
			int port = p->u.kein.focuse_logout_map.map_port;
			
			k_put_menu_string msg;
			msg.type = KM_INFO;
			msg.str_num = 88;		// 강제 종료 한 사람한테
			
			SendDirectClientOtherMap( CMD_PUT_MENU_STRING, server_id, port, &msg, sizeof( k_put_menu_string ) );
		}
		else
		{
			SendPutMenuString( KM_OK, 88, cn );		// 강제 종료 당하는 사람한테
			//acer7
			CrackMgr.SendHackingUser( cn, USE_GM_TOOL, "Absolute LogOut by name" );
		}
		return;
	}
	else
	{
		if( cn == CN_FROM_OTHER_MAPSERVER_MSG )		// 다른 맵에서 온거라면
		{
			return;
		}
		else
		{
			CHARLIST *ch = CheckServerId( cn );
			if( !ch ) return;

			// 다른 맵으로 보낸다.
			t_packet packet;
			packet.h.header.type = p->h.header.type;
				strcpy( packet.u.kein.focuse_logout_map.name, name );
				packet.u.kein.focuse_logout_map.map_port=g_MapPort;
				packet.u.kein.focuse_logout_map.cn = cn;
			packet.h.header.size = sizeof( k_focuse_logout_map );
			g_pServerTable->BroadCastToEveryServer( (char*)&packet, (sizeof(t_header)+packet.h.header.size), SERVER_TYPE_MAP );

			//acer7
			CrackMgr.SendHackingUser( cn, USE_GM_TOOL, "Absolute LogOut by name" );
		}
	}
}

void SendPutMenuString( int type, int str_num, short int cn, char* pName /*=NULL*/ )
{
	t_packet packet;
	packet.h.header.type = CMD_PUT_MENU_STRING;
		packet.u.kein.put_menu_string.type = type;
		packet.u.kein.put_menu_string.str_num = str_num;
		if (NULL==pName) { ZeroMemory(packet.u.kein.put_menu_string.szName,sizeof(packet.u.kein.put_menu_string.szName)); }
		else
		{
			strcpy(packet.u.kein.put_menu_string.szName,pName); 
		}
	packet.h.header.size = sizeof( k_put_menu_string);
	QueuePacket( connections, cn, &packet, 1 );
}

// 021107 YGI
void SendDirectClientOtherMap( int type, short int server_id, short int port, void *msg, int size )
{
	t_packet packet;
	packet.h.header.type = CMD_SEND_DIRECT_CLIENT_TO_MAP;
		packet.u.kein.direct_map2user.type = type;
		packet.u.kein.direct_map2user.server_id = server_id;
		if( msg )
		{
			memcpy( packet.u.kein.direct_map2user.data, msg, size );
		}		
	packet.h.header.size = size+4;
	if( port == GetOwnPort() )		// 같은 맵으로 보낼 경우
	{
		HandleCommand( CN_FROM_OTHER_MAPSERVER_MSG, &packet );
	}
	else
	{
		g_pServerTable->SendRajaPacketToOtherMapServer( (WORD)port, (char *)&packet, packet.h.header.size+sizeof(t_header) );	
	}
}

int RecvSEND_DIRECT_CLIENT_TO_MAP( t_packet *p )
{
	int type = p->u.kein.direct_map2user.type;
	int cn = p->u.kein.direct_map2user.server_id;
	void *msg = (void *)p->u.kein.direct_map2user.data;
	t_packet packet;
	packet.h.header.type = type;
	packet.h.header.size = p->h.header.size-4;
	memcpy( packet.u.data, msg, packet.h.header.size );
	QueuePacket( connections, cn, &packet, 1 );
	return 1;
}

void RecvReporterMode( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn ); 
	if( !ch ) return;
	
	if( !ch->IsReporter() ) return;		// 기자가 아니다.
	if( !ch->reporter_mode && ch->viewtype == VIEWTYPE_GHOST_ ) return;		// 유령상태에서 유령상태로 가려고 할경우
	ch->reporter_mode = 1 - ch->reporter_mode;

	if( ch->reporter_mode )		// 기자단 모드이면
	{
		// 유령으로 만들고.. 
		ch->Hp = 1;
		ch->bAlive = DEAD_;
		ch->viewtype = VIEWTYPE_GHOST_;
		ch->InitCurse(); 
		SendViewTypeChange( cn, VIEWTYPE_GHOST_ );
		ch->SendCharInfoBasic( HP, 0 );////020704 lsw
	}
	else
	{
		// 유령에서 정상으로 돌려 준다.
		SkillMgr.CharacterToAlive(ch,1);// 살려준다.//020501 lsw
	}
	SendPacketDefault( CMD_REPORTER_MODE, &ch->reporter_mode, sizeof( char ), cn );
}

// 011130 YGI
void LoadDRAGON_MAX_CONNECTIONS()
{
	int max = 0;
	max = GetPrivateProfileInt("Option","DRAGON_MAX_CONNECTIONS",0,MAP_SERVER_INI_);	
	if( !max ) return;
	if( max > DRAGON_MAX_CONNECTIONS_ )
	{	
		MyLog( LOG_IMPORTANT, "DRAGON_MAX_CONNECTIONS over %d", DRAGON_MAX_CONNECTIONS_ );
		return;
	}
	if( max < DRAGON_MAX_CONNECTIONS )
	{
		MyLog( LOG_IMPORTANT, "ERROR!!!  new DRAGON_MAX_CONNECTIONS: %d, current DRAGON_MAX_CONNECTIONS: %d", max, DRAGON_MAX_CONNECTIONS );
		return;
	}
	
	DRAGON_MAX_CONNECTIONS = max;
	MyLog( LOG_NORMAL, "DRAGON_MAX_CONNECTIONS change OK!!!  DRAGON_MAX_CONNECTIONS number = %d", DRAGON_MAX_CONNECTIONS );
	return;
}

void RecvSetTodayGambleItem( t_packet *p )
{
	ItemAttr *item = &p->u.kein.default_item;
	g_TodayGambleItem.SetTodayItem( item );
}

void RecvDeleteTodayGambleItem()
{
	g_TodayGambleItem.DeleteTodyaItem();
}

void RecvGetTodayGambleItem( short int cn )
{
	SendPacketDefault( CMD_GET_TODAY_GAMBLE_ITEM, 
		&g_TodayGambleItem.GetTodayItem(), sizeof(ItemAttr ), cn );
}

void RecvChangePartySelectExp( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	ch->party_select = p->u.kein.default_char;
}

// 020111 YGI
int WinOfRandFunction( int count )
{
	// 몇분의 1 인 확률 계산
	if( !count ) return 0;
	if( rand()%count == count-1 ) return 1;
	return 0;
}
/*
void RecvGetPotalMap( short int cn )
{
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int nation = ch->name_status.nation;
	int max = g_MapPotal[nation].GetCount();

	t_packet packet;
	packet.h.header.type = CMD_GET_POTAL_MAP;
	packet.h.header.size = sizeof( k_potal_map_unit	) * max+1;
		packet.u.kein.potal_map.count = max;
	
	if( !max ) return;
	for( int i=0; i<max; i++ )
	{
		//패킷에 내용을 담는다.
		packet.u.kein.potal_map.map[i].map_number = g_MapPotal[nation].GetMapNumber( i );
		packet.u.kein.potal_map.map[i].map_money = g_MapPotal[nation].GetMapMoney( i );
	}

	QueuePacket( connections, cn, &packet, 1 );
}*/

void RecvGotoPotal( t_packet *p, short int cn )
{
	int para = p->u.kein.default_char;
	CHARLIST *ch = CheckServerId( cn );
	if( !ch ) return;

	int nation = MapInfo[MapNumber].nation;
	if( !nation ) nation = ch->name_status.nation;

	int money = g_MapPotal[nation].GetMapMoney( para );
	if( ch->Money < money ) return;// 돈이 부족

	int level = g_MapPotal[nation].GetMapLevel( para );
	if (ch->GetLevel() < level) return; // CSD-030806

	// 맵 이동
	int map_num = g_MapPotal[nation].GetMapNumber( para );
	char *map_name = MapInfo[map_num].mapfile;
	int x, y;
	g_MapPotal[nation].GetMapXY( para, x, y );

	char temp_name[30];
	strcpy( temp_name, MapName );
	CharUpper( temp_name );
	if( strcmp( map_name, temp_name) == 0 )
	{
		// 같은 지역으로는 이동을 할 수 없다.
		SendPutMenuString( KM_FAIL, 128, cn );
		return;
	}

	SubtractMoney( money, ch );
	if( !MapMove( cn, map_name, x, y ) )
	{	// 실패 햇을 경우 
		::AddMoney(money,ch);//021126 lsw
	}
}

void RecvSetMapType( t_packet *p, short int cn )
{
	int map_num;
	if( cn == CN_FROM_OTHER_MAPSERVER_MSG )		// 맵에서 왔다.
	{
		map_num = p->u.kein.set_map_type.map_num;
		MapInfo[map_num].m_MoveType = p->u.kein.set_map_type.type;

		// 030811
		MyLog( 0, "Change MapMove Type [%s]'s type = %d", MapInfo[map_num].mapfile, MapInfo[map_num].m_MoveType );
	}
	else	// 클라이언트로 패킷이 왔을 경우 
	{
		// 이 맵이 마을 맵이면 설정이 되지 않게 한다.
		if( !strcmp( MapName, "MA-IN" )
			|| !strcmp( MapName, "RENES_C" )
			|| !strcmp( MapName, "BARANTAN" ) )
			return;		// 중요 마을 맵이다.

		int type = p->u.kein.default_char;
		int real_type = MMT_NORMAL;
		switch( type )
		{
			case 1 : // 시작
				{
					real_type = MMT_NORMAL;
					
					// 이벤트 시작을 알린다. // 참여 유도	// 맵 오픈
					t_packet packet;
					packet.h.header.type = CMD_PUT_MENU_STRING;
						packet.u.kein.put_menu_string.type = KM_BBS;
						packet.u.kein.put_menu_string.str_num = 117;
					packet.h.header.size = sizeof( k_put_menu_string);
					SendPacket2Maps( &packet );	// 모든 맵에 뿌려준다.
					g_pUserManager->SendPacket(&packet); // CSD-CN-031213
					break;
				}
			case 2 :
				{
					real_type = MMT_NOT_MOVE;
					
					// 이벤트 시작됐음을 알린다.	// 참여 불가
					t_packet packet;
					packet.h.header.type = CMD_PUT_MENU_STRING;
						packet.u.kein.put_menu_string.type = KM_BBS;
						packet.u.kein.put_menu_string.str_num = 118;
					packet.h.header.size = sizeof( k_put_menu_string);
					SendPacket2Maps( &packet );	// 모든 맵에 뿌려준다.
					g_pUserManager->SendPacket(&packet); // CSD-CN-031213
					break;
				}
			case 3:
				{
					real_type = MMT_NOT_MOVE;		// 참여 불가
					// 운영자만 남기고, 모두 팅긴다.
					UserAllMapMove( 1, "G_BATTLE", 111, 116 );
					break;
				}
			case 4:
				{
					real_type = MMT_NOT_MOVE;		// 참여 불가 설정
					break;
				}

			case 0 :
				break;
		}
		MapInfo[MapNumber].m_MoveType = real_type ;
		
		p->h.header.size = sizeof( k_set_map_type );
			p->u.kein.set_map_type.map_num = MapNumber;
			p->u.kein.set_map_type.type = real_type ;
		SendPacket2Maps( p );	// 모든 맵에 뿌려준다.
		// 성공한 사실을 알려준다.
		// 이벤트 맵으로 선포했습니다. 이제부터는 이 맵은 이벤트 맵이 되고 맵이동을 하지 못합니다.
	}
}
void RecvCheckGuildChange( t_packet *p, short int cn )
{
	CHARLIST *ch = CheckServerId( cn ); 
	if( !ch) return;

	t_packet packet;
	packet.h.header.type = CMD_CHECK_CHANGE_GUILD_INFO;
	packet.h.header.size= sizeof( k_check_guild_change );


	int type = p->u.kein.default_char;
	packet.u.kein.check_guild_change.type = type;
	switch( type )
	{
		case 1 : // 길드 마크 변경
			{	// 파란 그넷 10개, 빨간 시그넷 5개, 돈 200000
				//< 040331-YGI 
				if( !CheckGuildPower( GDP_CHANGE_MARK, ch ) )
				{
					packet.u.kein.check_guild_change.ret = 0;		// 실패
					QueuePacket( connections, cn, &packet, 1 );
					return;
				}
				//> 040331-YGI

				int ret = 0;
				if( ( GetItemMuchByMuchItem( ch, IN_RED_SIGNET) < 5 )		// 빨강
					|| ( GetItemMuchByMuchItem( ch, IN_BLUE_SIGNET) < 10 )		// 파란
					|| ( ch->Money < 200000 ) )
				{
					packet.u.kein.check_guild_change.ret = 0;		// 실패
					QueuePacket( connections, cn, &packet, 1 );
					return;
				}

				SendDeleteMuchItemQuantity( ch, IN_RED_SIGNET, 5 );
				SendDeleteMuchItemQuantity( ch, IN_BLUE_SIGNET, 10 );
				SubtractMoney( 200000, ch );
				
				packet.u.kein.check_guild_change.ret = 1;		// 성공
				QueuePacket( connections, cn, &packet, 1 );
				break;
			}
		case 2 :	// 길드 직책 변경
			{	// 파란 시그넷 5개, 빨간 시그넷 3개, 돈 100000
				//< 040331-YGI
				if( !CheckGuildPower( GDP_CHANGE_DEGREE_INFO, ch ) )
				{
					packet.u.kein.check_guild_change.ret = 0;		// 실패
					QueuePacket( connections, cn, &packet, 1 );
					return;
				}
				//> 040331-YGI

				int ret = 0;
				if( ( GetItemMuchByMuchItem( ch, IN_RED_SIGNET ) < 3 )		// 빨강
					|| ( GetItemMuchByMuchItem( ch, IN_BLUE_SIGNET) < 5 )		// 파란
					|| ( ch->Money < 100000 ) )
				{
					packet.u.kein.check_guild_change.ret = 0;		// 실패
					QueuePacket( connections, cn, &packet, 1 );
					return;
				}

				SendDeleteMuchItemQuantity( ch, IN_RED_SIGNET, 3 );
				SendDeleteMuchItemQuantity( ch, IN_BLUE_SIGNET, 5 );
				SubtractMoney( 100000, ch );
				
				packet.u.kein.check_guild_change.ret = 1;		// 성공
				QueuePacket( connections, cn, &packet, 1 );
				break;
			}
	}
}


// 020701 YGI
extern void ScenarioServerReset( WORD port );
void DisconnectServer_kein( WORD port )
{
}

void ConnectServer_kein( WORD port )
{
	if( !port ) return;
	ScenarioServerReset( port );
}
