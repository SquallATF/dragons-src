﻿/*****************************************************************************\
* Copyright (c), Future Entertainment World / Seoul, Republic of Korea        *
* All Rights Reserved.                                                        *
*                                                                             *
* This document contains proprietary and confidential information.  No        *
* parts of this document or the computer program it embodies may be in        *
* any way copied, duplicated, reproduced, translated into a different         *
* programming language, or distributed to any person, company, or             *
* corporation without the prior written consent of Future Entertainment World *
\*****************************************************************************/

#include "stdafx.h"
#include <Ddraw.h>
#include <Math.h>


#include "sys/stat.h"

#include "Dragon.h"	
#include "Hong_Sprite.h"
#include "Hong_Sub.h"
#include "char.h"  
#include "map.h"
#include "Path.h"


#include "DirectSound.h"
#include "CharDataTable.h"

#include "Menu.h"
#include "Effect.h"

//#include "MenuDef.h"
#include "NumberAniMgr.h"
#include "ItemTable.h"

#include "NetWork.h"
#include "GameProc.h"
#include "Hangul.h"
#include "Item.h"

#include "MenuSet.h"//无限名声头文件
#include "Tool.h"
#include "Hong_Light.h"
#include "House.h"
#include "Skill.h"
#include "kh_cpp.h"
#include "Horse.h"		// LTS HORSERIDER
#include "StrikeSys.h"
#include "CombatSys.h"
using namespace battle;

#define COLOR_FROZEN ReturnBlendRGB(100, 100, 255)
#define	COLOR_SLOW   ReturnBlendRGB(255, 255, 100)
#define COLOR_DAZE   ReturnBlendRGB(255, 100, 100)
#define	COLOR_POISON ReturnBlendRGB(100, 255, 100)
#define COLOR_STONE  ReturnBlendRGB(100, 100, 100)

extern cHorseRider		g_HorseRider;		// LTS HORSERIDER
extern int				g_ActionAnger;		// LTS ACTION



extern	void	ChangeItem( int start );	// 烙矫 酒捞袍 官操扁 

////////////////////// 0531 lkh 荐沥 ////////////////////////
extern  int		GetItemAttr(int item_id, int attr);
extern  int		GetItemAttr2(int sprno, int item_id, int attr);		//011018 KHS 噶肚

extern int GetIndexOfCondition( BYTE );


int IDCount;

///////////////////////////////////////////////////////////////////////////////
//	
extern	SCharacter			SCharacterData;	
extern 	SCharacter			SCharSource;

extern CHARACTERLIST		g_CharacterList;
extern int		set_is_mainmenu_active(bool m);//窗口基址
extern	CMenuVariable g_Menu_Variable;			//无限名声CLASS
//extern int aItemInventory[3][3][8];

extern void SendReqGuildInfo1( int guild_code );		// 010719 YGI

BOOL			BuildCharacterList( LPCHARACTERLIST lpCharacterList );
void			DestroyCharacterList( LPCHARACTERLIST lpCharacterList );
void			DestroyCharacterListAll( LPCHARACTERLIST lpCharacterList );

void			DisplayPC( LPCHARACTER ch, int tag );
BOOL			AddCharacter( LPCHARACTERLIST lpCharacterList, LPCHARACTER ch, LPGPCHARINFO gpch );
void			DeleteCharacter( LPCHARACTERLIST lpCharacterList, LPCHARACTER ch );
LPCHARACTER		FindCharacter( LPCHARACTERLIST lpCharacterList, DWORD dwID );

void			SetCharacterPosition( LPCHARACTER ch, POINT position );
LPPOINT			GetCharacterPosition( LPCHARACTER ch );
BOOL			ChangeDirection( DIRECTION* lpDirection, int sx, int sy, int dx, int dy );


BYTE			SpriteOrder[ 4][ 8][ 75];
int				EndOfAnimationFlag;
int				AttackRangeTable[2][ CHARACTER_ACCESSORY_MAX_][ 10];


void  GetAttacked( LPCHARACTER ch );

NPC_INFO    g_infNpc[MAX_CHARACTER_SPRITE_];
GPCHARINFO	gpCharInfo;	


BOOL BuildCharacterList( LPCHARACTERLIST lpCharacterList )
{	
	gpCharInfo.type			= SPRITETYPE_CHARACTER;	
	gpCharInfo.id			= 0;
	
	gpCharInfo.sprno		= 0;
	
	//	gpCharInfo.position.x	= gpCharInfo.position.y = 0; 
	gpCharInfo.position.x	= 195* TILE_SIZE; 
	gpCharInfo.position.y	= 151* TILE_SIZE;
	
	CharUpper( MapName );
	if( strcmp( "FIREDUN1", MapName ) == 0 )
	{
		gpCharInfo.position.x	= 195* TILE_SIZE; 
		gpCharInfo.position.y	= 151* TILE_SIZE;
	}
	else if( strcmp( "K_SUNG2", MapName ) == 0 )
	{
	}else if( strcmp( "MA-IN", MapName ) == 0 )
	{
	}else if( strcmp( "SUNG_TILE_00", MapName ) == 0 )
	{
		gpCharInfo.position.x = 52* TILE_SIZE; 
		gpCharInfo.position.y = 22* TILE_SIZE;
	}else if( strcmp( "SOURCE", MapName ) == 0 )
	{
		gpCharInfo.position.x	= 200* TILE_SIZE; 
		gpCharInfo.position.y	= 130* TILE_SIZE;
	}else if( strcmp( "ICE-W01", MapName ) == 0 )
	{
		gpCharInfo.position.x	= 91* TILE_SIZE; 
		gpCharInfo.position.y	= 119* TILE_SIZE;
	}
	
	gpCharInfo.direction	= DIRECTION_DOWN;
	
	gpCharInfo.nCurrentAction = ACTION_NONE;
	gpCharInfo.basicAction	= ACTION_BATTLE_NONE;
	gpCharInfo.basicWalk	= ACTION_BATTLE_WALK_OWNHAND;
	gpCharInfo.animationType= ANIMATION_TYPE_WOMAN;
	
	gpCharInfo.bodyr = 255;
	gpCharInfo.bodyg = 255;
	gpCharInfo.bodyb = 255;
	
	gpCharInfo.clothr = 255;
	gpCharInfo.clothg = 0;
	gpCharInfo.clothb = 0;
	
	SCharacterData.nCharacterHP = gpCharInfo.hp			= 2000;
	SCharacterData.nCharacterMP = gpCharInfo.mp			= 3000;
	SCharacterData.nCharacterSP = gpCharInfo.sp			= 4000;
	SCharacterData.nCharacterMAXHP = gpCharInfo.hpmax	= 2100;
	SCharacterData.nCharacterMAXMP = gpCharInfo.mpmax	= 3100;
	SCharacterData.nCharacterMAXSP = gpCharInfo.spmax	= 4100;


	SCharSource.nCharacterData[0] = 1;
	gpCharInfo.sex			= SCharSource.nCharacterData[0]; 
	gpCharInfo.sight		= 300;
	
	switch( gpCharInfo.sprno )
	{			
	case 0 :	gpCharInfo.accessory[0] = 11;
		gpCharInfo.accessory[1] =  0;
		gpCharInfo.accessory[2] = 61;
		gpCharInfo.accessory[3] =  0;
		break;
	case 1 :	gpCharInfo.accessory[0] =  7;
		gpCharInfo.accessory[1] =  0;
		gpCharInfo.accessory[2] = 61;
		gpCharInfo.accessory[3] =  0;
		break;
	}			
	
	int tx = gpCharInfo.position.x - 10;
	if( tx < 0 ) tx = 0;		
	int ty = gpCharInfo.position.y - 7;
	if( ty < 0 ) ty = 0;		
	
	RECT	rect;				
	
	rect.left	= tx;			
	rect.top	= ty;			
	rect.right	= tx + 20;		
	rect.bottom = ty + 11;		
	
	int id = 0;
	int i,j;
	
	for( i = 0 ; i < 5 ; i ++ )
		for( j = 0 ; j < 5 ; j ++)
		{
			gpCharInfo.position.x	= (195 + i*2 )* TILE_SIZE; 
			gpCharInfo.position.y	= (151 + j*2 )* TILE_SIZE;
			gpCharInfo.id			= id++;
			AddCharacter( lpCharacterList, g_CharacterList.lpLast, &gpCharInfo );
		}
		
		
		
		///////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////  Another PC 
		///////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////
		
		
		///////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////  NPC  Monster...
		///////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////
		
		
		gpCharInfo.type		 = SPRITETYPE_MONSTER;	//SPRITETYPE_MONSTER;	
		gpCharInfo.id		 = 7;	
		gpCharInfo.sprno	 = 16;	
		
		//	gpCharInfo.position.x = gpCharInfo.position.y = 0; 
		gpCharInfo.position.x = 192* TILE_SIZE; 
		gpCharInfo.position.y = 152* TILE_SIZE;
		
		
		if( strcmp( "FIREDUN1", MapName ) == 0 )
		{
			gpCharInfo.position.x	= 193* TILE_SIZE; 
			gpCharInfo.position.y	= 151* TILE_SIZE;
		}
		else if( strcmp( "K_SUNG2", MapName ) == 0 )
		{
			
		}else if( strcmp( "MA-IN", MapName ) == 0 )
		{
			
		}else if( strcmp( "SUNG_TILE_00", MapName ) == 0 )
		{
			gpCharInfo.position.x = 53* TILE_SIZE; 
			gpCharInfo.position.y = 27* TILE_SIZE;
		}else if( strcmp( "SOURCE", MapName ) == 0 )
		{
			gpCharInfo.position.x	= 200* TILE_SIZE; 
			gpCharInfo.position.y	= 133* TILE_SIZE;
		}else if( strcmp( "ICE-W01", MapName ) == 0 )
		{
			gpCharInfo.position.x	= 94* TILE_SIZE; 
			gpCharInfo.position.y	= 121* TILE_SIZE;
		}
		
		
		gpCharInfo.direction = DIRECTION_DOWN;
		
		gpCharInfo.nCurrentAction	= MON1_NONE;
		gpCharInfo.basicAction		= MON1_NONE;
		gpCharInfo.basicWalk		= MON1_BATTLE_WALK;
		gpCharInfo.animationType	= ANIMATION_TYPE_MON1;
		
		gpCharInfo.sex				= 0;
		
		gpCharInfo.hp			= 2000;
		gpCharInfo.mp			= 3000;
		gpCharInfo.sp			= 4000;
		gpCharInfo.hpmax		= 2100;
		gpCharInfo.mpmax		= 3100;
		gpCharInfo.spmax		= 4100;
		
		if ( !AddCharacter( lpCharacterList, g_CharacterList.lpLast, &gpCharInfo ) )
		{
			return	FALSE;	
		}
		
		return	TRUE;
}	

void DestroyCharacterList(  LPCHARACTERLIST lpCharacterList )
{		
	while ( lpCharacterList->lpLast != NULL )
		DeleteCharacter( lpCharacterList, lpCharacterList->lpLast );
	
	lpCharacterList->nCount = 0;	
}		

void DestroyCharacterListExceptHero( void )
{		
	LPCHARACTER temp = NULL;
	
	if(!g_CharacterList.lpFirst)
	{
		return;
	}
	
	LPCHARACTER ch = g_CharacterList.lpFirst->lpNext;
	
	while( ch != NULL )
	{	
		temp = ch->lpNext;
		DeleteCharacter( &g_CharacterList, ch );
		ch = temp;
	}	
}		

void PutWeapon(CHARACTER* pTarget, int nX, int nY)
{	//< CSD-031029
	if (pTarget == NULL)
	{
		return;
	}

	if (pTarget->peacests == 0)
	{
		return;
	}

	if (pTarget->idWeaponItem/1000 != 6)
	{	//< CSD-031101
		PutCompressedImage(nX, nY, pTarget->sp2);
		return;
	}	//> CSD-031101

	CItem_Weapon* pWeapon = static_cast<CItem_Weapon*>(ItemUnit(pTarget->idWeaponItem));

	if (pWeapon == NULL)
	{
		return;
	}

	const int nEffect = pWeapon->effect;
	const int nFrame = pTarget->AnimationTable[pTarget->nCurrentAction].cFrame[pTarget->nCurrentFrame];
	CharacterSprite* pAccessorySprite = CharAccessorySpr[pTarget->sprno];
	Spr* pWeaponSprite = &(pAccessorySprite + nEffect)->sp[pTarget->direction][nFrame];

	switch (pWeapon->nRange)
	{
	case 0:
		{
			if (nEffect > 0)
			{
				PutCompressedImageFX(nX, nY, pWeaponSprite, 10 + Random(4), 2);
			}

			PutCompressedImage(nX, nY, pTarget->sp2);
			break;
		}
	case 1:
		{
			PutCompressedImage(nX, nY, pTarget->sp2);
			
			if (nEffect > 0)
			{
				PutCompressedImageFX(nX, nY, pWeaponSprite, 10 + Random(4), 2);
			}

			break;
		}
	}
}	//> CSD-031014

void PutWeapon(CHARACTER* pTarget, int nX, int nY, DWORD dwColor)
{	//< CSD-031014
	if (pTarget == NULL)
	{
		return;
	}

	if (pTarget->peacests == 0)
	{
		return;
	}

	if (pTarget->idWeaponItem/1000 != 6)
	{	//< CSD-031101
		PutCompressedImageCharRGB(nX, nY, pTarget->sp2, dwColor);
		return;
	}	//> CSD-031101

	CItem_Weapon* pWeapon = static_cast<CItem_Weapon*>(ItemUnit(pTarget->idWeaponItem));

	if (pWeapon == NULL)
	{
		return;
	}

	const int nEffect = pWeapon->effect;

	if (nEffect > 0)
	{
		CharacterSprite* pAccessorySprite = CharAccessorySpr[pTarget->sprno];

		const int nFrame = pTarget->AnimationTable[pTarget->nCurrentAction].cFrame[pTarget->nCurrentFrame];
		Spr* pWeaponSprite = &(pAccessorySprite + nEffect)->sp[pTarget->direction][nFrame];
		PutCompressedImageFX(nX, nY, pWeaponSprite, 10 + Random(4), 2);
	}

	PutCompressedImageCharRGB(nX, nY, pTarget->sp2, dwColor);
}	//> CSD-031029

void PutBody( LPCHARACTER ch, int x, int y, Spr *sp, DWORD bodycolor )
{
	PutCompressedImageCharRGB( x,y, sp, bodycolor);
}		

void PutHead(int ac1, CharacterSprite *cas, int dir, int sprc, int x, int y)		// 010412 YGI
{	//< CSD-031029
	switch (ac1)
	{
	case 0:
		{
			PutCompressedImage(x, y, &(cas + 104)->sp[dir][sprc]);
			break;		// default 捧备
		}
	case 1:
		{
			break;
		}
	default:
		{
			PutCompressedImage(x, y, &(cas + ac1)->sp[dir ][sprc]);
			break;
		}
	}
}	//> CSD-031029

void PutHead(int ac1, CharacterSprite *cas, int dir, int sprc, int x, int y, DWORD dwColor)
{	//< CSD-031029
	switch (ac1)
	{
	case 0:
		{
			PutCompressedImageCharRGB(x, y, &(cas + 104)->sp[dir][sprc], dwColor);
			break;		// default 捧备
		}
	case 1:
		{
			break;
		}
	default:
		{
			PutCompressedImageCharRGB(x, y, &(cas + ac1)->sp[dir ][sprc], dwColor);
			break;
		}
	}
}	//> CSD-031029

void PutMantle(CHARACTER* pTarget, int nX, int nY, Spr* pSprite)
{	//< CSD-040331
	switch (pTarget->mantle)
	{
	case 8305:
		{
			PutCompressedImageCharRGB(nX, nY, pSprite, pTarget->mantlecolor);  //调披风正常颜色  上下代码替换 原来代码: PutCompressedImage(nX, nY, pSprite);
			break;
		}
	default:
		{
			PutCompressedImage(nX, nY, pSprite);   //调披风正常颜色  上下代码替换 原来代码:  PutCompressedImageCharRGB(nX, nY, pSprite, pTarget->mantlecolor);
			break;
		}
	}
}	//> CSD-040331

void PutMantle(CHARACTER* pTarget, int nX, int nY, Spr* pSprite, DWORD dwColor)
{	//< CSD-040331
	PutCompressedImageCharRGB(nX, nY, pSprite, dwColor);
}	//> CSD-040331

int g_bToolViewPlusEquip = 1;	// NEW_CHAR YGI
extern void DrawChair(LPCHARACTER ch);		// LTS SITDOWN

void BeforeDrawChair(LPCHARACTER ch)		// 011214 LTS
{
	switch (ch->direction)
	{
	case 0 :
	case 1 :
	case 2 :
	case 6 :
	case 7 : DrawChair(ch);	break;
	}
}

void AfterDrawChair(LPCHARACTER ch)
{
	switch (ch->direction)
	{
	case 3 :
	case 4 :
	case 5 : DrawChair(ch);	break;
	}
}

void PutCharacter(int spriteorder, LPCHARACTER ch, CharacterSprite *cas, int x, int y, int dir, int sprc, int ac0, int ac1, int ac2, int ac3, int ac4)
{	//< CSD-034331
	int dflag;
	if( ch->peacests && ( ch->nCurrentAction == ACTION_BATTLE_NONE_TWOHAND	|| ch->nCurrentAction == ACTION_BATTLE_NONE ) )
	{
		dflag= (( ch->direction >= 1 && ch->direction <= 5 ) ?true:false);  // KHS ; 噶肚狼 版快 困规亲( dir == 5 )阑 焊绊 乐阑锭俊绰 盖付瘤阜俊 嘛囚具 茄促. 
	}
	else
	{
		dflag= (( ch->direction >= 2 && ch->direction <= 6 ) ?true:false);  // KHS ; 噶肚狼 版快 困规亲( dir == 5 )阑 焊绊 乐阑锭俊绰 盖付瘤阜俊 嘛囚具 茄促. 
	}
	
	if (ch->nCurrentAction == ACTION_SITDOWN)
	{	// 011214 LTS
		BeforeDrawChair(ch);		
	}

	switch (spriteorder)
	{			
	case 0:
		{	//	个.公扁.规菩
			if (ac4 && !dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc]);
			}

			PutBody(ch, x, y, ch->sp, ch->bodycolor);
			
			if (ac0)  // 癌渴..
			{	
				PutCompressedImage( x,y, ch->sp0);
				PutCompressedImageCharRGB( x,y, &(cas + ac0 + 1)->sp[dir][sprc], ch->clothcolor);
				// 011030 KHS JJING
				if (0 < ac0 && ac0 <= 40)		// LTS LOCALWAR
				{
					if (ch->jjing >= 5)
					{
						PutCompressedImage(x,y, &(cas + ac0 + 200)->sp[dir][sprc]);
					}

					if (ch->jjing >= 9)
					{
						PutCompressedImage( x,y, &(cas + ac0 + 201)->sp[dir][sprc]);
					}
				}
			}

			if (!ac4 || !dflag)
			{
				PutHead(ac1, cas, dir, sprc, x, y); // 捧备.
			}

			PutWeapon(ch, x, y); // CSD-031014

			if (ch->peacests && ac3)
			{
				PutCompressedImage(x, y, ch->sp3);  // 规菩.
			}

			if (ch->nCurrentAction == ACTION_SITDOWN)
			{
				AfterDrawChair(ch);		// 011214 LTS
			}

			if (ac4 && dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc]);
				PutHead(ac1, cas, dir, sprc, x, y); // 捧备.
			}

			break;
		}
	case 1: 
		{	//	个. 规菩, 漠
			if (ac4 && !dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc]);
			}

			PutBody(ch, x, y, ch->sp, ch->bodycolor);	// 漠..
			
			if (ac0)  // 癌渴.					
			{	
				PutCompressedImage( x,y, ch->sp0 );
				PutCompressedImageCharRGB( x,y, &(cas+ac0+1)->sp[ dir ][ sprc], ch->clothcolor );
				// 011030 KHS JJING
				if (0 < ac0 && ac0 <= 40)		// LTS LOCALWAR
				{
					if (ch->jjing >= 5)
					{
						PutCompressedImage(x, y, &(cas + ac0+200)->sp[dir ][sprc]);
					}

					if (ch->jjing >= 9)
					{
						PutCompressedImage( x,y, &(cas+ac0+201)->sp[dir][sprc]);
					}
				}
			}

			if (!ac4 || !dflag)
			{
				PutHead( ac1, cas, dir, sprc, x, y); // 捧备..
			}
			
			if (ch->peacests && ac3)
			{
				PutCompressedImage(x, y, ch->sp3);  // 规菩..
			}

			PutWeapon(ch, x, y); // CSD-031014
			
			if (ch->nCurrentAction == ACTION_SITDOWN)
			{
				AfterDrawChair(ch);		// 011214 LTS
			}

			if (ac4 && dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc]);
				PutHead( ac1, cas, dir, sprc, x, y); // 捧备..
			}

			break;
		}
	case 2: 
		{	//	漠. 个. 规菩
			if (ac4 && !dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc]);
			}

			PutWeapon(ch, x, y); // CSD-031014
			PutBody(ch, x, y, ch->sp, ch->bodycolor);

			if (ac0)  // 癌渴..				
			{	
				PutCompressedImage(x, y, ch->sp0);
				PutCompressedImageCharRGB(x, y, &(cas+ac0 + 1)->sp[dir][sprc], ch->clothcolor);
				// 011030 KHS JJING
				if (0 < ac0 && ac0 <= 40)		// LTS LOCALWAR
				{
					if (ch->jjing >= 5)
					{
						PutCompressedImage( x,y, &(cas + ac0 + 200)->sp[dir][sprc]);
					}

					if (ch->jjing >= 9)
					{
						PutCompressedImage( x,y, &(cas+ac0+201)->sp[dir][sprc]);
					}
				}
			}	
			
			if (!ac4 || !dflag)
			{
				PutHead(ac1, cas, dir, sprc, x, y); // 捧备..
			}

			if (ch->peacests ) if( ac3 ) PutCompressedImage( x,y, ch->sp3 );  // 规菩..
			if (ch->nCurrentAction==ACTION_SITDOWN) AfterDrawChair(ch);		// 011214 LTS
			
			if (ac4 && dflag )
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc]);
				PutHead(ac1, cas, dir, sprc, x, y); // 捧备..
			}

			break;
		}
	case 3: 
		{	//	漠 规菩, 个
			if (ac4 && !dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc]);
			}

			PutWeapon(ch, x, y); // CSD-031014

			if (ch->peacests && ac3)
			{
				PutCompressedImage(x, y, ch->sp3);  // 规菩..
			}

			PutBody(ch, x, y, ch->sp, ch->bodycolor);

			if (ac0)  // 癌渴..							
			{	
				PutCompressedImage(x, y, ch->sp0);
				PutCompressedImageCharRGB(x, y, &(cas+ac0+1)->sp[dir][sprc], ch->clothcolor);
				// 011030 KHS JJING
				if (0 < ac0 && ac0 <= 40)		// LTS LOCALWAR
				{
					if (ch->jjing >= 5)
					{
						PutCompressedImage(x, y, &(cas + ac0 + 200)->sp[dir][sprc]);
					}

					if (ch->jjing >= 9)
					{
						PutCompressedImage( x,y, &(cas + ac0 + 201)->sp[dir][sprc]);
					}
				}
			}		
			
			if (!ac4 || !dflag)
			{
				PutHead(ac1, cas, dir, sprc, x, y); // 捧备..
			}

			if (ch->nCurrentAction == ACTION_SITDOWN)
			{
				AfterDrawChair(ch);		// 011214 LTS
			}

			if (ac4 && dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc]);
				PutHead(ac1, cas, dir, sprc, x, y); // 捧备..
			}

			break;	
		}
	case 4: 
		{	//	规菩, 漠, 个
			if (ac4 && !dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc]);
			}

			if (ch->peacests && ac3)
			{
				PutCompressedImage(x, y, ch->sp3);  // 规菩..
			}

			PutWeapon(ch, x, y); // CSD-031014
			PutBody(ch, x, y, ch->sp, ch->bodycolor);

			if (ac0)  // 癌渴..
			{	
				PutCompressedImage(x, y, ch->sp0);
				PutCompressedImageCharRGB(x, y, &(cas + ac0 + 1)->sp[dir][sprc], ch->clothcolor);
				// 011030 KHS JJING
				if (0 < ac0 && ac0 <= 40)		// LTS LOCALWAR
				{
					if (ch->jjing >= 5)
					{
						PutCompressedImage( x,y, &(cas + ac0 + 200)->sp[dir][sprc]);
					}

					if (ch->jjing >= 9)
					{
						PutCompressedImage( x,y, &(cas + ac0 + 201)->sp[dir][sprc]);
					}
				}
			}

			if (!ac4 || !dflag)
			{
				PutHead(ac1, cas, dir, sprc, x, y); // 捧备..
			}
			
			if (ch->nCurrentAction == ACTION_SITDOWN)
			{
				AfterDrawChair(ch);		// 011214 LTS
			}
			
			if (ac4 && dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc]);
				PutHead(ac1, cas, dir, sprc, x, y); // 捧备..
			}

			break;	
		}
	case 5: 
		{	// 规菩, 个, 漠
			if (ac4 && !dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc]);
			}

			if (ch->peacests && ac3)
			{
				PutCompressedImage(x, y, ch->sp3);  // 规菩..

			}

			PutBody(ch, x, y, ch->sp, ch->bodycolor);
			
			if (ac0)  // 癌渴..
			{	
				PutCompressedImage(x, y, ch->sp0);
				PutCompressedImageCharRGB(x, y, &(cas + ac0 + 1)->sp[dir][sprc], ch->clothcolor);
				// 011030 KHS JJING
				if (0 < ac0 && ac0 <= 40)		// LTS LOCALWAR
				{
					if (ch->jjing >= 5)	
					{
						PutCompressedImage(x, y, &(cas+ac0+200)->sp[dir][sprc]);
					}

					if (ch->jjing >= 9)
					{
						PutCompressedImage(x, y, &(cas+ac0+201)->sp[dir][sprc]);
					}
				}
			}
			
			if (!ac4 || !dflag)
			{
				PutHead(ac1, cas, dir, sprc, x, y); // 捧备..
			}

			PutWeapon(ch, x, y); // CSD-031014

			if (ch->nCurrentAction == ACTION_SITDOWN)
			{
				AfterDrawChair(ch);		// 011214 LTS
			}

			if (ac4 && dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc]);
				PutHead(ac1, cas, dir, sprc, x, y); // 捧备..6
			}

			break;
		}
	}		
}	//> CSD-040331

void PutCharacter(int spriteorder, LPCHARACTER ch, CharacterSprite *cas, int x, int y, int dir, int sprc, int ac0, int ac1, int ac2, int ac3, int ac4, DWORD dwColor1, DWORD dwColor2)
{	//< CSD-030331
	int dflag;
	
	if( ch->peacests && ( ch->nCurrentAction == ACTION_BATTLE_NONE_TWOHAND	|| ch->nCurrentAction == ACTION_BATTLE_NONE ) )
	{
		dflag= (( ch->direction >= 1 && ch->direction <= 5 ) ?true:false);  // KHS ; 噶肚狼 版快 困规亲( dir == 5 )阑 焊绊 乐阑锭俊绰 盖付瘤阜俊 嘛囚具 茄促. 
	}
	else
	{
		dflag= (( ch->direction >= 2 && ch->direction <= 6 ) ?true:false);  // KHS ; 噶肚狼 版快 困规亲( dir == 5 )阑 焊绊 乐阑锭俊绰 盖付瘤阜俊 嘛囚具 茄促. 
	}
	//> CSD-031104	
	if (ch->nCurrentAction == ACTION_SITDOWN)
	{	// 011214 LTS
		BeforeDrawChair(ch);		
	}

	switch (spriteorder)
	{			
	case 0:
		{	//	个.公扁.规菩
			if (ac4 && !dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc], dwColor2);
			}

			PutBody(ch, x, y, ch->sp, dwColor1);
			
			if (ac0)  // 癌渴..
			{	
				PutCompressedImageCharRGB(x ,y, ch->sp0, dwColor2);
				PutCompressedImageCharRGB(x, y, &(cas + ac0 + 1)->sp[dir][sprc], dwColor2);
				// 011030 KHS JJING
				if (0 < ac0 && ac0 <= 40)		// LTS LOCALWAR
				{
					if (ch->jjing >= 5)
					{
						PutCompressedImageCharRGB(x, y, &(cas + ac0 + 200)->sp[dir][sprc], dwColor2);
					}

					if (ch->jjing >= 9)
					{
						PutCompressedImageCharRGB(x, y, &(cas + ac0 + 201)->sp[dir][sprc], dwColor2);
					}
				}
			}

			if (!ac4 || !dflag)
			{
				PutHead(ac1, cas, dir, sprc, x, y, dwColor2); // 捧备.
			}

			PutWeapon(ch, x, y, dwColor2); // CSD-031014

			if (ch->peacests && ac3)
			{
				PutCompressedImageCharRGB(x, y, ch->sp3, dwColor2);  // 规菩.
			}

			if (ch->nCurrentAction == ACTION_SITDOWN)
			{
				AfterDrawChair(ch);		// 011214 LTS
			}

			if (ac4 && dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc], dwColor2);
				PutHead(ac1, cas, dir, sprc, x, y, dwColor2); // 捧备.
			}

			break;
		}
	case 1: 
		{	//	个. 规菩, 漠
			if (ac4 && !dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc], dwColor2);
			}

			PutBody(ch, x, y, ch->sp, dwColor1);	// 漠..
			
			if (ac0)  // 癌渴.					
			{	
				PutCompressedImageCharRGB(x, y, ch->sp0, dwColor2);
				PutCompressedImageCharRGB(x, y, &(cas + ac0 + 1)->sp[dir][sprc], dwColor2);
				// 011030 KHS JJING
				if (0 < ac0 && ac0 <= 40)		// LTS LOCALWAR
				{
					if (ch->jjing >= 5)
					{
						PutCompressedImageCharRGB(x, y, &(cas + ac0 + 200)->sp[dir][sprc], dwColor2);
					}

					if (ch->jjing >= 9)
					{
						PutCompressedImageCharRGB(x, y, &(cas + ac0 + 201)->sp[dir][sprc], dwColor2);
					}
				}
			}

			if (!ac4 || !dflag)
			{
				PutHead(ac1, cas, dir, sprc, x, y, dwColor2); // 捧备..
			}
			
			if (ch->peacests && ac3)
			{
				PutCompressedImageCharRGB(x, y, ch->sp3, dwColor2);  // 规菩..
			}

			PutWeapon(ch, x, y, dwColor2); // CSD-031014
			
			if (ch->nCurrentAction == ACTION_SITDOWN)
			{
				AfterDrawChair(ch);		// 011214 LTS
			}

			if (ac4 && dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc], dwColor2);
				PutHead( ac1, cas, dir, sprc, x, y); // 捧备..
			}

			break;
		}
	case 2: 
		{	//	漠. 个. 规菩
			if (ac4 && !dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc], dwColor2);
			}

			PutWeapon(ch, x, y, dwColor2); // CSD-031014
			PutBody(ch, x, y, ch->sp, dwColor1);

			if (ac0)  // 癌渴..				
			{	
				PutCompressedImageCharRGB(x, y, ch->sp0, dwColor2);
				PutCompressedImageCharRGB(x, y, &(cas+ac0 + 1)->sp[dir][sprc], dwColor2);
				// 011030 KHS JJING
				if (0 < ac0 && ac0 <= 40)		// LTS LOCALWAR
				{
					if (ch->jjing >= 5)
					{
						PutCompressedImageCharRGB(x, y, &(cas + ac0 + 200)->sp[dir][sprc], dwColor2);
					}

					if (ch->jjing >= 9)
					{
						PutCompressedImageCharRGB(x, y, &(cas + ac0 + 201)->sp[dir][sprc], dwColor2);
					}
				}
			}	
			
			if (!ac4 || !dflag)
			{
				PutHead(ac1, cas, dir, sprc, x, y, dwColor2); // 捧备..
			}

			if (ch->peacests && ac3)
			{
				PutCompressedImageCharRGB(x, y, ch->sp3, dwColor2);  // 规菩..
			}

			if (ch->nCurrentAction == ACTION_SITDOWN)
			{
				AfterDrawChair(ch);		// 011214 LTS
			}

			if (ac4 && dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc], dwColor2);
				PutHead(ac1, cas, dir, sprc, x, y, dwColor2); // 捧备..
			}

			break;
		}
	case 3: 
		{	//	漠 规菩, 个
			if (ac4 && !dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc], dwColor2);
			}

			PutWeapon(ch, x, y, dwColor2); // CSD-031014

			if (ch->peacests && ac3)
			{
				PutCompressedImageCharRGB(x, y, ch->sp3, dwColor2);  // 规菩..
			}

			PutBody(ch, x, y, ch->sp, dwColor1);

			if (ac0)  // 癌渴..							
			{	
				PutCompressedImageCharRGB(x, y, ch->sp0, dwColor2);
				PutCompressedImageCharRGB(x, y, &(cas + ac0 + 1)->sp[dir][sprc], dwColor2);
				// 011030 KHS JJING
				if (0 < ac0 && ac0 <= 40)		// LTS LOCALWAR
				{
					if (ch->jjing >= 5)
					{
						PutCompressedImageCharRGB(x, y, &(cas + ac0 + 200)->sp[dir][sprc], dwColor2);
					}

					if (ch->jjing >= 9)
					{
						PutCompressedImageCharRGB(x, y, &(cas + ac0 + 201)->sp[dir][sprc], dwColor2);
					}
				}
			}		
			
			if (!ac4 || !dflag)
			{
				PutHead(ac1, cas, dir, sprc, x, y, dwColor2); // 捧备..
			}

			if (ch->nCurrentAction == ACTION_SITDOWN)
			{
				AfterDrawChair(ch);		// 011214 LTS
			}

			if (ac4 && dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc], dwColor2);
				PutHead(ac1, cas, dir, sprc, x, y, dwColor2); // 捧备..
			}

			break;	
		}
	case 4: 
		{	//	规菩, 漠, 个
			if (ac4 && !dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc], dwColor2);
			}

			if (ch->peacests && ac3)
			{
				PutCompressedImageCharRGB(x, y, ch->sp3, dwColor2);  // 规菩..
			}

			PutWeapon(ch, x, y, dwColor2); // CSD-031014
			PutBody(ch, x, y, ch->sp, dwColor1);

			if (ac0)  // 癌渴..
			{	
				PutCompressedImageCharRGB(x, y, ch->sp0, dwColor2);
				PutCompressedImageCharRGB(x, y, &(cas + ac0 + 1)->sp[dir][sprc], dwColor2);
				// 011030 KHS JJING
				if (0 < ac0 && ac0 <= 40)		// LTS LOCALWAR
				{
					if (ch->jjing >= 5)
					{
						PutCompressedImageCharRGB(x, y, &(cas + ac0 + 200)->sp[dir][sprc], dwColor2);
					}

					if (ch->jjing >= 9)
					{
						PutCompressedImageCharRGB(x, y, &(cas + ac0 + 201)->sp[dir][sprc], dwColor2);
					}
				}
			}

			if (!ac4 || !dflag)
			{
				PutHead(ac1, cas, dir, sprc, x, y, dwColor2); // 捧备..
			}
			
			if (ch->nCurrentAction == ACTION_SITDOWN)
			{
				AfterDrawChair(ch);		// 011214 LTS
			}
			
			if (ac4 && dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc], dwColor2);
				PutHead(ac1, cas, dir, sprc, x, y, dwColor2); // 捧备..
			}

			break;	
		}
	case 5: 
		{	// 规菩, 个, 漠
			if (ac4 && !dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc], dwColor2);
			}

			if (ch->peacests && ac3)
			{
				PutCompressedImageCharRGB(x, y, ch->sp3, dwColor2);  // 规菩..
			}

			PutBody(ch, x, y, ch->sp, dwColor1);
			
			if (ac0)  // 癌渴..
			{	
				PutCompressedImageCharRGB(x, y, ch->sp0, dwColor2);
				PutCompressedImageCharRGB(x, y, &(cas + ac0 + 1)->sp[dir][sprc], dwColor2);
				// 011030 KHS JJING
				if (0 < ac0 && ac0 <= 40)		// LTS LOCALWAR
				{
					if (ch->jjing >= 5)	
					{
						PutCompressedImageCharRGB(x, y, &(cas+ac0+200)->sp[dir][sprc], dwColor2);
					}

					if (ch->jjing >= 9)
					{
						PutCompressedImageCharRGB(x, y, &(cas+ac0+201)->sp[dir][sprc], dwColor2);
					}
				}
			}
			
			if (!ac4 || !dflag)
			{
				PutHead(ac1, cas, dir, sprc, x, y, dwColor2); // 捧备..
			}

			PutWeapon(ch, x, y, dwColor2); // CSD-031014

			if (ch->nCurrentAction == ACTION_SITDOWN)
			{
				AfterDrawChair(ch);		// 011214 LTS
			}

			if (ac4 && dflag)
			{
				PutMantle(ch, x, y, &(cas + ac4)->sp[dir][sprc], dwColor2);
				PutHead(ac1, cas, dir, sprc, x, y, dwColor2); // 捧备..
			}

			break;
		}
	}		
}	//> CSD-030331

inline void PutCharacterConfusion(int spriteorder, LPCHARACTER ch, CharacterSprite *cas, int x, int y, int dir, int sprc, int ac0, int ac1, int ac2, int ac3, int ac4)
{	//< CSD-031029
	DWORD dwColor1 = ReturnBlendRGB(255, 0, 0);
	DWORD dwColor2 = ReturnBlendRGB(255, 100, 100);
	PutCharacter(spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4, dwColor1, dwColor2);
}	//> CSD-031029

inline void PutCharacterPoison(int spriteorder, LPCHARACTER ch, CharacterSprite *cas, int x, int y, int dir, int sprc, int ac0, int ac1, int ac2, int ac3, int ac4)
{	//< CSD-031029
	DWORD dwColor1 = ReturnBlendRGB(0, 255, 0);
	DWORD dwColor2 = ReturnBlendRGB(100, 255, 100);
	PutCharacter(spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4, dwColor1, dwColor2);
}	//> CSD-031029

inline void PutCharacterStone( int spriteorder, LPCHARACTER ch, CharacterSprite *cas, int x, int y, int dir, int sprc, int ac0, int ac1, int ac2, int ac3, int ac4  )
{	//< CSD-031029
	DWORD dwColor1 = ReturnBlendRGB( 100, 100, 100);
	DWORD dwColor2 = ReturnBlendRGB( 100, 100, 100);
	PutCharacter(spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4, dwColor1, dwColor2);
}	//> CSD-031029

inline void PutCharacterSlow(int spriteorder, LPCHARACTER ch, CharacterSprite *cas, int x, int y, int dir, int sprc, int ac0, int ac1, int ac2, int ac3, int ac4)
{	//< CSD-031029
	DWORD dwColor1 = ReturnBlendRGB(255, 255, 0);
	DWORD dwColor2 = ReturnBlendRGB(255, 255, 100);
	PutCharacter(spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4, dwColor1, dwColor2);
}	//> CSD-031029

inline void PutCharacterFrozen(int spriteorder, LPCHARACTER ch, CharacterSprite *cas, int x, int y, int dir, int sprc, int ac0, int ac1, int ac2, int ac3, int ac4)
{	//< CSD-031029
	DWORD dwColor1 = ReturnBlendRGB(0, 0, 255);
	DWORD dwColor2 = ReturnBlendRGB(100, 100, 255);
	PutCharacter(spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4, dwColor1, dwColor2);
}	//> CSD-031029

inline void PutCharacterCurse( int spriteorder, LPCHARACTER ch, CharacterSprite *cas, int x, int y, int dir, int sprc, int ac0, int ac1, int ac2, int ac3, int ac4  )
{	//< CSD-031029
	DWORD dwColor1 = ReturnBlendRGB(150, 0, 150);
	DWORD dwColor2 = ReturnBlendRGB(150, 0, 150);
	PutCharacter(spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4, dwColor1, dwColor2);
}	//> CSD-031029

inline void PutCharacterTransparency ( int spriteorder, LPCHARACTER ch, CharacterSprite *cas, int x, int y, int dir, int sprc, int ac0, int ac1, int ac2, int ac3, int ac4  )
{
	if(ch==Hero)
	{
		PutCompressedImageFX( x,y, ch->sp, 6, 1 );
		if( ac0 )
		{
			PutCompressedImageFX( x, y, ch->sp0, 6, 1);
		}
		
		if( ac4 ) PutCompressedImageFX( x,y, &(cas+ac4)->sp[ dir ][ sprc], 6,1 );	// 噶肚 
		
		if( ac1 != 1 )
			if( ac1 ) 		PutCompressedImageFX( x, y, &(cas+ac1)->sp[ dir ][ sprc], 6, 1);
			else			PutCompressedImageFX( x, y, &(cas+104)->sp[ dir ][ sprc], 6, 1);
			
			if(	ac2 ) 		PutCompressedImageFX( x, y, ch->sp2, 6, 1);
			if(	ac3 ) 		PutCompressedImageFX( x, y, ch->sp3, 6, 1);
	}
	else
	{
		PutCompressedImageVoid( x+(rand()%5-2), y+(rand()%5-2), ch->sp);
		
		if( ac0 )
		{
			PutCompressedImageVoid( x+(rand()%5-2), y+(rand()%5-2), ch->sp0);
		}
		if( ac4 ) PutCompressedImageVoid( x+(rand()%5-2),y+(rand()%5-2), &(cas+ac4)->sp[ dir ][ sprc] );	// 噶肚 
		
		if( ac1 != 1 )
			if( ac1 ) 		PutCompressedImageVoid( x+(rand()%5-2), y+(rand()%5-2), &(cas+ac1)->sp[ dir ][ sprc]);
			else			PutCompressedImageVoid( x+(rand()%5-2), y+(rand()%5-2), &(cas+104)->sp[ dir ][ sprc]);
			
			if(	ac2 ) 		PutCompressedImageVoid( x+(rand()%5-2), y+(rand()%5-2), ch->sp2);
			if(	ac3 ) 		PutCompressedImageVoid( x+(rand()%5-2), y+(rand()%5-2), ch->sp3);
	}
}

inline void PutCharacterTree( int spriteorder, LPCHARACTER ch, CharacterSprite *cas, int x, int y, int dir, int sprc, int ac0, int ac1, int ac2, int ac3, int ac4  )
{	//< CSD-031029
	DWORD dwColor = ReturnBlendRGB(0,255,0);
	PutCharacter(spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4, dwColor, dwColor);
}	//> CSD-031029	

inline void PutCharacterGreen( int spriteorder, LPCHARACTER ch, CharacterSprite *cas, int x, int y, int dir, int sprc, int ac0, int ac1, int ac2, int ac3, int ac4  )
{	//< CSD-031029
	DWORD dwColor1 = ReturnBlendRGB(125, 251, 226);
	DWORD dwColor2 = ReturnBlendRGB(192, 251, 148);
	PutCharacter(spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4, dwColor1, dwColor2);
}	//> CSD-031029	

inline void PutCharacterRed( int spriteorder, LPCHARACTER ch, CharacterSprite *cas, int x, int y, int dir, int sprc, int ac0, int ac1, int ac2, int ac3, int ac4  )
{	//< CSD-031029	
	DWORD dwColor = ReturnBlendRGB(255, 100, 100);
	PutCharacter(spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4, dwColor, dwColor);
}	//> CSD-031029


// 011018 KHS 噶肚
inline void PutCharacterGhost( int spriteorder, LPCHARACTER ch, CharacterSprite *cas, int x, int y, int dir, int sprc, int ac0, int ac1, int ac2, int ac3, int ac4 )
{	
	int trans = 10;
	
	y -= ch->ghostani;
	
	PutCompressedImageFX( x,y, ch->sp, trans, 1 );
	if( ac0 )
	{
		PutCompressedImageFX( x,y, ch->sp0,  trans, 1 );
		//PutCompressedImageCharRGB( x,y, &(cas+ac0+1)->sp[ dir ][ sprc], color );
	}
	
	if( ac4 ) PutCompressedImageFX( x,y, &(cas+ac4)->sp[ dir ][ sprc], trans, 1 );	// 噶肚 
	
	if( ac1 != 1 )
		if( ac1 ) PutCompressedImageFX( x,y, &(cas+ac1)->sp[ dir ][ sprc], trans ,1 );	  // 捧备..
		else  PutCompressedImageFX( x,y, &(cas+104)->sp[ dir ][ sprc], trans ,1 );	// 捧备..
		
		//	if( ch->peacests ) if( ac2 ) PutCompressedImageFX( x,y, ch->sp2, trans ,1 );	// 漠..
		//	if( ch->peacests ) if( ac3 ) PutCompressedImageFX( x,y, ch->sp3, trans ,1 );	// 规菩..
}		




//--------------------------- 001213 KHS
// 011018 KHS 噶肚
void h_DisplayPC( LPCHARACTER ch, int shadow )
{				
	int spriteorder;
	int x,y;	
	
	if (ch->HorseNo && ch->viewtype!=VIEWTYPE_GHOST_)						// LTS HORSERIDER TEMP
	{
		g_HorseRider.DrawHorseRider(ch);	//	h_DisPlay_HorseRider(ch,shadow);
		return;
	}
	
	int dir		= ch->direction;
	int sprc	= ch->AnimationTable[ ch->nCurrentAction].cFrame[  ch->nCurrentFrame ];
	int sprno	= ch->sprno;
	int ac0		= ch->accessory[0];
	int ac1		= ch->accessory[1];
	int ac2		= ch->accessory[2];
	int ac3		= ch->accessory[3];
	int ac4		= GetItemAttr2( ch->sprno, ch->mantle, ITEMSHOW );
	
	ch->sp		= &CharSpr[ sprno ].sp[ dir][ sprc];
	
	x = ch->x;
	y = ch->y;
	
#ifdef _DEBUG
	////////////////////  1206 test blood ///////////////////////
	if(SysInfo.notconectserver)
	{
		if( ch->nCurrentAction == ACTION_DIE && ch->dieani != 0 )
			PutDeadEffectBottom(ch);
		
		HprintBold( x - Mapx, y - Mapy, RGB( 0xff, 0xff, 0xff ), 0, "xl:%d  yl:%d", ch->sp->xl, ch->sp->yl );
	}
	else
#endif
	{
		if( (ch->blood_Set == true || ch->dieani!=0 ) && ch->nCurrentAction==ACTION_DIE )
			PutDeadEffectBottom(ch);
	}
	
	if (shadow == 0)
	{
		switch (ch->viewtype)
		{
		case VIEWTYPE_NORMAL_:
		case VIEWTYPE_CONFUSION_:
		case VIEWTYPE_POISON_:
		case VIEWTYPE_STONE_:
		case VIEWTYPE_SLOW_:
		case VIEWTYPE_FROZEN_:
		case VIEWTYPE_CURSE_:
			{
				PutCompressedImageShadow(x - Mapx - 1,y - Mapy - 3, ch->sp);
				return;
			}
		}
	}
	
	CharacterSprite *cas = CharAccessorySpr[sprno];
	ch->sp0		= &(cas+ac0)->sp[ dir][ sprc];
	ch->sp1		= &(cas+ac1)->sp[ dir][ sprc];
	ch->sp2		= &(cas+ac2)->sp[ dir][ sprc];
	ch->sp3		= &(cas+ac3)->sp[ dir][ sprc];
	ch->sp4		= &(cas+ac4)->sp[ dir][ sprc];
	
	//----------------
	int sx, sy, ex, ey;
	int tsx, tsy, tex, tey;
	int h = ch->height;
	sx = x - ch->sp->ox;		 sy = y - ch->sp->oy - h; 
	ex = sx +  ch->sp->xl;		 ey = sy + ch->sp->yl;
	
	tsx = x - ch->sp0->ox;		tsy = y - ch->sp0->oy - h; 
	tex = sx +  ch->sp0->xl;	tey = sy + ch->sp0->yl;
	if( tsx < sx ) sx = tsx;	if( tsy < sy ) sy = tsy;
	if( tex > ex ) ex = tex;	if( tey > ey ) ey = tey;
	
	tsx = x - ch->sp1->ox;		tsy = y - ch->sp1->oy - h; 
	tex = sx +  ch->sp1->xl;	tey = sy + ch->sp1->yl;
	if( tsx < sx ) sx = tsx;	if( tsy < sy ) sy = tsy;
	if( tex > ex ) ex = tex;	if( tey > ey ) ey = tey;
	
	tsx = x - ch->sp2->ox;		tsy = y - ch->sp2->oy - h; 
	tex = sx +  ch->sp2->xl;	tey = sy + ch->sp2->yl;
	if( tsx < sx ) sx = tsx;	if( tsy < sy ) sy = tsy;
	if( tex > ex ) ex = tex;	if( tey > ey ) ey = tey;
	
	tsx = x - ch->sp3->ox;		tsy = y - ch->sp3->oy - h; 
	tex = sx +  ch->sp3->xl;	tey = sy + ch->sp3->yl;
	if( tsx < sx ) sx = tsx;	if( tsy < sy ) sy = tsy;
	if( tex > ex ) ex = tex;	if( tey > ey ) ey = tey;
	//----------
	
	x -= Mapx;	
	y -= Mapy;
	y -= ch->height;
	
	switch (sprno)
	{	//< CSD-031024
	case 0:	
		{
			switch (ac2) // 咯磊..
			{	
			case 78:
			case 79: 
			case 162:
			case 172: spriteorder = SpriteOrder[sprno + 2][dir][sprc]; break;
			default:  spriteorder = SpriteOrder[sprno][dir][sprc]; break;
			}	
			
			break;
		}
	case 1:
		{
			switch (ac2) // 巢磊..
			{	
			case 80: 
			case 81: 
			case 162:
			case 172: spriteorder = SpriteOrder[sprno + 2][dir][sprc]; break;
			default:  spriteorder = SpriteOrder[sprno][dir][sprc]; break;
			}
		
			break;
		}
	}	//> CSD-031024
	
	static int sr= 120,sg = 255, sb= 205;
	static int er= 220,eg = 255, eb= 145;
	
#define SPEED__  10
	
	static int COLO__ =  10;
	
	if( ch == g_GameInfo.lpcharacter && (ch->viewtype != VIEWTYPE_GHOST_ && ch->viewtype != VIEWTYPE_TRANSPARENCY_) )
	{
		static int ro, rr = sr;
		static int go, gg = sg;
		static int bo, bb = sb;
		
		static int ao, aa = 10;
		
		if( ao )	{ aa++; if( aa >  8 )	{ ao = !ao; }}
		else		{ aa--; if( aa <  7  )	{ ao = !ao; }}
		
		COLO__ = aa;
		
		//		if( ro )	{ rr+=(( er-sr) * SPEED__ / 100); if( rr > er )	{ ro = !ro; }}
		//		else		{ rr-=(( er-sr) * SPEED__ / 100); if( rr < sr )	{ ro = !ro; }}
		
		if( ro )	{ rr+=(( er-sr) * SPEED__ / 100); if( rr > er )	{ ro = !ro; }}
		else		{ rr-=(( er-sr) * SPEED__ / 100); if( rr < sr )	{ ro = !ro; }}
		if( go )	{ gg+=(( eg-sg) * SPEED__ / 100); if( gg > eg )	{ go = !go; }}
		else		{ gg-=(( eg-sg) * SPEED__ / 100); if( gg < sg )	{ go = !go; }}
		if( bo )	{ bb+=(( eb-sb) * SPEED__ / 100); if( bb > eb )	{ bo = !bo; }}
		else		{ bb-=(( eb-sb) * SPEED__ / 100); if( bb < sb )	{ bo = !bo; }} 
		//		WORD color = RGB16( rr,gg,bb );
		//		WORD color = RGB16( 110,10,10 );
		/* 
		PutCompressedImageFX( x+1,y, ch->sp, COLO__/2, 2 );
		PutCompressedImageFX( x-1,y, ch->sp, COLO__/2, 2 );
		
		  if( ac0 )  // 癌渴..
		  {	
		  PutCompressedImageFX( x+1,y, ch->sp0, COLO__/2, 2 );
		  PutCompressedImageFX( x-1,y, ch->sp0, COLO__/2, 2 );
		  } 
		  if( ac1 )
		  {	
		  PutCompressedImageFX( x+1,y, ch->sp1, COLO__/2, 2 );
		  PutCompressedImageFX( x-1,y, ch->sp1, COLO__/2, 2 );
		  }
		  if( ac2 && ch->peacests )
		  {
		  PutCompressedImageFX( x+1,y, ch->sp2, COLO__/2, 2 );
		  PutCompressedImageFX( x-1,y, ch->sp2, COLO__/2, 2 );
		  }
		  if( ac3 && ch->peacests )
		  {
		  PutCompressedImageFX( x+1,y, ch->sp3, COLO__/2, 2 );
		  PutCompressedImageFX( x-1,y, ch->sp3, COLO__/2, 2 );
		  }
		  
		*/
	}	
	
	switch (ch->viewtype)
	{	//< CSD-040331	
	case VIEWTYPE_NORMAL_:
		{
			if( ch == g_GameInfo.lpcharacter )
			{
				if( ch == Hero ) 
				{
					PutCharacterGreen( spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4 );
				}
				else
				{
					PutCharacterRed( spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4 );
				}
			}
			else 
			{
				PutCharacter( spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4 );
			}

			break;
		}
	case VIEWTYPE_CONFUSION_:	
		{
			PutCharacterConfusion(spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4 );
			break;
		}
	case VIEWTYPE_POISON_:		
		{ 
			PutCharacterPoison(spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4 );
			break;
		}
	case VIEWTYPE_STONE_:		
		{
			PutCharacterStone(spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4 );
			break;
		}
	case VIEWTYPE_SLOW_:		
		{
			PutCharacterSlow(spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4 );
			break;
		}
	case VIEWTYPE_FROZEN_:	
		{
			PutCharacterFrozen(spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4 );
			break;
		}
	case VIEWTYPE_CURSE_:		
		{
			PutCharacterCurse(spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4 );
			break;
		}
	case VIEWTYPE_TRANSPARENCY_ :
		{
			PutCharacterTransparency ( spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4 );	
			break;
		}	
	case VIEWTYPE_CAMELEON_:
		{
			if( ch == Hero || ch->canview )
			{
				PutCharacter( spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4 );
				PutCompressedImageVoid( x+Random(6)-3,y+Random(6)-3, ch->sp );
				PutCompressedImageVoid( x+Random(6)-3,y+Random(6)-3, ch->sp );
				PutCompressedImageVoid( x+Random(6)-3,y+Random(6)-3, ch->sp );
				PutCompressedImageVoid( x+Random(6)-3,y+Random(6)-3, ch->sp );
				PutCompressedImageVoid( x+Random(6)-3,y+Random(6)-3, ch->sp );
			}
			else
			{
				PutCompressedImageVoid( x,y, ch->sp );
				if( ac0 ) PutCompressedImageVoid( x,y, ch->sp0 );
				if( ac2 ) PutCompressedImageVoid( x, y, ch->sp2 );
			}

			break;
		}
	case VIEWTYPE_TREE_:
		{
			if( ch == Hero || Hero->canview )
			{
				PutCharacterTree( spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4 );
			}

			break;
		}
	case VIEWTYPE_GHOST_:
		{
			if( !IsHidenGhost( ch ) && ( CanViewGhost(Hero) || Hero->viewtype == VIEWTYPE_GHOST_ ) ) //020320 lsw
			{
				PutCharacterGhost( spriteorder, ch, cas, x, y, dir, sprc, ac0, ac1, ac2, ac3, ac4 );
			}

			break;
		}
	}	//> CSD-040331	
	
	if( !tool_DrawFrontMap )
	{
		int i,j;
		
		ex += (TILE_SIZE + 1 ); ey += (TILE_SIZE + 1 );
		ex += ((ex-sx)/2);
		sx	/= TILE_SIZE;		sy	/= TILE_SIZE;
		ex	/= TILE_SIZE;		ey	/= TILE_SIZE;
		
		if( sx < 0 ) sx = 0;
		if( sy < 0 ) sy = 0;
		if( ex >= g_Map.file.wWidth  ) ex = g_Map.file.wWidth;
		if( ey >= g_Map.file.wHeight ) ey = g_Map.file.wHeight;
		
		
		y +=Mapy;
		
		
		for( i = sx ; i < ex ; i ++ )
			for( j = sy ; j < ey ; j ++ )
			{	
				int front = TileMap[ i][ j].front;
				if(  front )
					if(  (int )TileMap[ i][ j].frontoffset * TILE_SIZE >= y )
					{ 
						if( Sou[ front].xl == -1 )		PutTile( i*TILE_SIZE - Mapx,  j*TILE_SIZE - Mapy,  Sou[front].img, g_DestBackBuf );
						else							PutCompressedImage(  i*TILE_SIZE - Mapx,  j*TILE_SIZE - Mapy,  &Sou[ front] );
					}
			}		
			
			y -=Mapy;
	}
	
	if( ch->viewtype != VIEWTYPE_TRANSPARENCY_) // 捧疙牢埃付过篮 努腐篮 登瘤父 农腐瞪锭 滴伯霸 焊捞绰巴篮 救茄促. 
		if( ch == g_GameInfo.lpcharacter )
		{		
			if( ch->viewtype == VIEWTYPE_GHOST_ )		y -= ch->ghostani;
			
			
			
			PutCompressedImageFX( x, y, ch->sp, COLO__/2, 2 );
			
			if( ac0 )  // 癌渴..
			{	
				PutCompressedImageFX( x, y, ch->sp0, COLO__/2, 2 );
			}	
			if( ac1 != 1 )
				if( ac1 )
				{	
					PutCompressedImageFX( x, y, ch->sp1, COLO__/2, 2 );
				}	
				if( ac2 && ch->peacests )
				{	
					PutCompressedImageFX( x, y, ch->sp2, COLO__/2, 2 );
				}	
				if( ac3 && ch->peacests )
				{	
					PutCompressedImageFX( x, y, ch->sp3, COLO__/2, 2 );
				}	
		}		
		
		/////////////////////////////////////////////////////////////////////////
		
#ifdef _DEBUG
		if( CheckEquipmentFlag == FALSE ) return;
		{
			PutCompressedImage( x+ 100,y, &CharSpr[sprno].sp[ dir ][ sprc]);
			if( ac0 )  // 癌渴..
			{		
				PutCompressedImage( x+150,y - 50, &CharAccessorySpr[sprno][ 100].sp[ dir ][ sprc]);
				PutCompressedImage( x+150,y , &(cas+ ac0  )->sp[ dir ][ sprc]);//,  (15<<11) | (0<<6) | ( 0 ));
				PutCompressedImage( x+150,y + 50, &(cas+ ac0+1)->sp[ dir ][ sprc]);
			}		
			if( ac1 != 1 )
				if( ac1 )
				{
					PutCompressedImage( x+200,y, &(cas+ac1)->sp[ dir ][ sprc]);  // 捧备..
				}
				PutWeapon(ch, x + 250, y); // CSD-031014
				if( ac3 ) PutCompressedImage( x+300,y, &(cas+ac3)->sp[dir][sprc]);  // 规菩..
				if( ac4 ) PutCompressedImage( x+300,y, &(cas+ac4)->sp[dir][sprc]);  // 噶肚..
		}
#endif
}			

void h_DisplayPC2( LPCHARACTER ch )
{	
	return;	// 010220 KHS
	
	int x = ch->x - Mapx;
	int y = ch->y - Mapy - ch->height;
	if( ch->sprno > 1 ) return;
	if( ch->viewtype != VIEWTYPE_NORMAL_ ) return;
	
	// 010103 KHS 唱俊霸辑 钢府乐澜 扒拱第俊 啊妨廉 捧康登绰 何盒阑 弊府瘤 臼绰促. ( 努扼捞攫飘弥利拳 )
	if( ch != Hero ) 
	{
		if( ( ch->x - Hero->x ) * ( ch->x - Hero->x ) + ( ch->y - Hero->y ) * ( ch->y - Hero->y ) > 20000 ) return;
	}
	
	PutCompressedImageFX( x,y, ch->sp, 8, 6 );
	if( ch->sp0 )PutCompressedImageFX( x,y, ch->sp0, 8, 6 );
	if( ch->sp1 )PutCompressedImageFX( x,y, ch->sp1, 8, 6 );
	if( ch->sp2 && ch->peacests )PutCompressedImageFX( x,y, ch->sp2, 8, 6 );
	if( ch->sp3 && ch->peacests )PutCompressedImageFX( x,y, ch->sp3, 8, 6 );
	
#ifdef _DEBUG
	if( GetSysInfo( SI_GAME_MAKE_MODE ) ){Line( x-5, y+ch->height, x+5, y+ch->height, 0xffff );Line( x, y-5+ch->height, x, y+5+ch->height, 0xffff );}
#endif
	
	return;
}	


// 010925 LTS _________________________________________________________________
void DrawNGuardStone(int AliveType,LPCHARACTER ch,int x,int y)		// 010924 LTS		// 011015 LTS
{
	Spr *sp;
	const char NGuardStoneAni[6][60]={  
		{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2 },		// 乞惑矫 
		{ 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15 },                 // 气颇矫 0 ~ 06 橇饭烙 (第)
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 3, 4, 5, 6, 6, 7, 7, 8, 8, 8, 9, 9, 9,10,10,10,11,11,11,12,12,12,13,13,13,14,14,14,-1,-1,-1,-1 },                 //        7 ~ 18 橇饭烙(吝埃)
		{16,16,16,16,17,17,17,17,18,18,18,18,19,19,19,19,20,20,20,20,21,21,21,21,22,22,22,22,23,24,24,25,25,26,26,27,27,28,28,29,29,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },                 //        0 ~ 13 橇饭烙 (菊)
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30 },                //       14 ~ 19 橇饭烙 (菊)    
		{15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15 } };
		
		
		if (AliveType)
		{
			if (ch->sealstoneani<0||ch->sealstoneani>59) ch->sealstoneani=0;
			
			sp = &CharSpr[ ch->sprno ].sp[0][NGuardStoneAni[0][ch->sealstoneani]];  
			PutCompressedImageShadow( x,y-5, sp);	
			PutCompressedImage( x,y, sp );                //窜鉴洒 靛肺雷 器牢飘甫 备茄促...
			
			ch->sealstoneani++;
		}
		else
		{
			if (ch->GuardStoneAni==1)	// 磷菌促绊 菩哦阑 罐阑锭 技泼..
			{
				if (ch->SetAni2Zero==1) 
				{
					ch->sealstoneani=0;
					ch->SetAni2Zero=0;
					
					int distx=ch->x-Hero->x;		// LTS SOUND
					int disty=ch->y-Hero->y;
					
					PlayListAutoSounds(261,distx,disty,0);	// LTS SOUND
				}
				if (ch->sealstoneani<60)
				{
					if (NGuardStoneAni[1][ch->sealstoneani]>0)
					{
						sp = &CharSpr[ ch->sprno ].sp[ 0 ][NGuardStoneAni[1][ch->sealstoneani]];	//第
						PutCompressedImage( x,y, sp );
					}
					if (NGuardStoneAni[2][ch->sealstoneani]>0)
					{
						sp = &CharSpr[ ch->sprno ].sp[ 0 ][NGuardStoneAni[2][ch->sealstoneani]];	//第
						PutCompressedImage( x,y, sp );
					}
					if (NGuardStoneAni[3][ch->sealstoneani]>0)
					{
						sp = &CharSpr[ ch->sprno ].sp[ 0 ][NGuardStoneAni[3][ch->sealstoneani]];	//吝埃
						PutCompressedImageFX( x,y, sp,5,2);
					}
					if (NGuardStoneAni[4][ch->sealstoneani]>0)
					{
						sp = &CharSpr[ ch->sprno ].sp[ 0 ][NGuardStoneAni[4][ch->sealstoneani]];	//菊捞促..
						PutCompressedImageFX( x,y, sp,5,2);
					}
					ch->sealstoneani++; 
				}
				else					// 俊聪皋捞记 风橇甫 倒疽促搁
				{
					sp = &CharSpr[ ch->sprno ].sp[ 0 ][NGuardStoneAni[5][0]];	//磷菌促..
					PutCompressedImage( x,y, sp );
				}
			}
			else						// 矫具俊 绝菌带 仇捞搁... 弊成 
			{
				sp = &CharSpr[ ch->sprno ].sp[ 0 ][NGuardStoneAni[5][0]];
				PutCompressedImage( x,y, sp );
			}
		}
}

void DrawBGuardStone(int AliveType,LPCHARACTER ch,int x,int y)		// 010924 LTS	// 011015 LTS
{
	Spr *sp;
	const char NGuardStoneAni[6][87]={  
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },		// 乞惑矫 (第)
		{ 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2 },		// 乞惑矫 (菊)
		{ 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9,10,10,10,11,11,11,12,12,12,13,13,13,14,14,14,15,15,15,16,16,16,17,17,17,18,19,20,21,21,22,22,23,23,24,24,25,25,25,26,26,26,27,27,27,28,28,28,29,29,29,30,30,30,31,31,31,32,32,32,33,33,33,34,34,34,35,35,35,-1 },		// 颇鲍矫 (第)
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36 },		// 颇鲍矫(吝埃)
		{-1,-1,-1,37,37,37,38,38,38,39,39,39,40,40,40,41,41,41,42,42,42,43,43,43,44,44,44,45,45,45,46,46,46,47,47,47,48,48,48,49,49,49,50,51,52,53,53,54,54,55,55,56,56,57,57,57,58,58,58,59,59,59,60,60,60,61,61,61,62,62,62,63,63,63,64,64,64,65,65,65,66,66,66,67,67,67,68 },        // 颇鲍矫 (菊)
		{36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36 } };
		
		
		if (AliveType)
		{
			if (ch->sealstoneani>86) ch->sealstoneani=0;
			sp = &CharSpr[ ch->sprno ].sp[0][NGuardStoneAni[0][ch->sealstoneani]];  
			PutCompressedImageShadow( x,y-5, sp);	
			PutCompressedImage( x,y, sp );                //窜鉴洒 靛肺雷 器牢飘甫 备茄促...
			sp = &CharSpr[ ch->sprno ].sp[0][NGuardStoneAni[1][ch->sealstoneani]];  
			PutCompressedImage( x,y, sp );                //窜鉴洒 靛肺雷 器牢飘甫 备茄促...
			ch->sealstoneani++;
		}
		else
		{
			if (ch->GuardStoneAni==1)	// 磷菌促绊 菩哦阑 罐阑锭 技泼..
			{
				if (ch->SetAni2Zero==1) 
				{
					ch->sealstoneani=0;
					ch->SetAni2Zero=0;
					int distx=ch->x-Hero->x;		// LTS SOUND
					int disty=ch->y-Hero->y;
					
					PlayListAutoSounds(261,distx,disty,0);	// LTS SOUND
				}
				if (ch->sealstoneani<87)
				{
					if (NGuardStoneAni[2][ch->sealstoneani]>0)
					{
						sp = &CharSpr[ ch->sprno ].sp[ 0 ][NGuardStoneAni[2][ch->sealstoneani]];	//第
						PutCompressedImageShadow( x,y-5, sp);	
						PutCompressedImage( x,y, sp );
					}
					if (NGuardStoneAni[3][ch->sealstoneani]>0)
					{
						sp = &CharSpr[ ch->sprno ].sp[ 0 ][NGuardStoneAni[3][ch->sealstoneani]];	//吝埃
						PutCompressedImage( x,y, sp );
					}
					if (NGuardStoneAni[4][ch->sealstoneani]>0)
					{
						sp = &CharSpr[ ch->sprno ].sp[ 0 ][NGuardStoneAni[4][ch->sealstoneani]];	//菊捞促..
						PutCompressedImageFX( x,y, sp,5,2);
					}
					ch->sealstoneani++; 
				}
				else					// 俊聪皋捞记 风橇甫 倒疽促搁
				{
					sp = &CharSpr[ ch->sprno ].sp[ 0 ][NGuardStoneAni[5][0]];	//磷菌促..
					PutCompressedImage( x,y, sp );
				}
			}
			else						// 矫具俊 绝菌带 仇捞搁... 弊成 
			{
				sp = &CharSpr[ ch->sprno ].sp[ 0 ][NGuardStoneAni[5][0]];
				PutCompressedImage( x,y, sp );
			}
		}
}

void DrawCastleGateWay65(int AliveType,LPCHARACTER ch,int x,int y)				// 011015 LTS
{
	Spr *sp;
	const char CastleGateWayAni65[4][62]={	
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //巩 何辑瘤绰芭
		{ 5, 2, 5, 5, 2, 2, 4, 4, 4, 3, 3, 4, 4, 2, 2, 5, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9,10,10,11,11,12,12,13,13,14,14,15,15,16,16, 6, 6, 7, 7, 8, 8, 9, 9,10,10,11,11,12,12,13,13,14,14,15,15,16,16}, //磐瘤绰芭(飘罚胶)
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };
		
		if (AliveType)
		{
			if (ch->sealstoneani<0||ch->sealstoneani>61) ch->sealstoneani=0;
			
			sp = &CharSpr[ ch->sprno ].sp[0][CastleGateWayAni65[0][ch->sealstoneani]];  
			PutCompressedImage( x,y, sp );                //窜鉴洒 靛肺雷 器牢飘甫 备茄促...
			ch->sealstoneani++;
		}
		else
		{
			if (ch->GuardStoneAni==1)	// 磷菌促绊 菩哦阑 罐阑锭 技泼..
			{
				if (ch->SetAni2Zero==1) 
				{
					ch->sealstoneani=0;
					ch->SetAni2Zero=0;
					int distx=ch->x-Hero->x;		// LTS SOUND
					int disty=ch->y-Hero->y;
					
					PlayListAutoSounds(261,distx,disty,0);	// LTS SOUND
				}
				if (ch->sealstoneani<62)
				{
					sp = &CharSpr[ ch->sprno ].sp[ 0 ][CastleGateWayAni65[1][ch->sealstoneani]];	//菊捞促..
					PutCompressedImage( x,y, sp );
					sp = &CharSpr[ ch->sprno ].sp[ 0 ][CastleGateWayAni65[2][ch->sealstoneani]];	//飘罚胶
					PutCompressedImageFX( x,y, sp,5,2);
					ch->sealstoneani++; 
				}
				else					// 俊聪皋捞记 风橇甫 倒疽促搁
				{
					sp = &CharSpr[ ch->sprno ].sp[ 0 ][CastleGateWayAni65[3][0]];	//磷菌促..
					PutCompressedImage( x,y, sp );
				}
			}
			else						// 矫具俊 绝菌带 仇捞搁... 弊成 
			{
				sp = &CharSpr[ ch->sprno ].sp[ 0 ][CastleGateWayAni65[3][0]];
				PutCompressedImage( x,y, sp );
			}
		}
}

void DrawCastleGateWay66(int AliveType,LPCHARACTER ch,int x,int y)			// 011015 LTS
{
	Spr *sp;
	const char CastleGateWayAni66[4][69]={	
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},	// 己巩何辑瘤绰芭
		{ 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 3, 3, 3, 4, 4, 5, 6, 6, 7, 7, 8, 8, 9, 9,10,10,11,11,12,12,13,13,14,14,15,15,16,16, 6, 6, 7, 7, 8, 8, 9, 9,10,10,11,11,12,12,13,13,14,14,15,15,16,16, 6, 6, 7, 7, 8, 8, 9, 9,10},	// 飘坊胶
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };
		
		if (AliveType)
		{
			if (ch->sealstoneani<0||ch->sealstoneani>69) ch->sealstoneani=0;
			
			sp = &CharSpr[ ch->sprno ].sp[0][CastleGateWayAni66[0][ch->sealstoneani]];  
			PutCompressedImage( x,y, sp );                //窜鉴洒 靛肺雷 器牢飘甫 备茄促...
			ch->sealstoneani++;
		}
		else
		{
			if (ch->GuardStoneAni==1)	// 磷菌促绊 菩哦阑 罐阑锭 技泼..
			{
				if (ch->SetAni2Zero==1) 
				{
					ch->sealstoneani=0;
					ch->SetAni2Zero=0;
					int distx=ch->x-Hero->x;		// LTS SOUND
					int disty=ch->y-Hero->y;
					
					PlayListAutoSounds(260,distx,disty,0);	// LTS SOUND
				}
				if (ch->sealstoneani<69)
				{
					sp = &CharSpr[ ch->sprno ].sp[ 0 ][CastleGateWayAni66[1][ch->sealstoneani]];	//菊捞促..
					PutCompressedImage( x,y, sp );
					sp = &CharSpr[ ch->sprno ].sp[ 0 ][CastleGateWayAni66[2][ch->sealstoneani]];	//飘罚胶
					PutCompressedImageFX( x,y, sp,5,2);
					ch->sealstoneani++; 
				}
				else					// 俊聪皋捞记 风橇甫 倒疽促搁
				{
					sp = &CharSpr[ ch->sprno ].sp[ 0 ][CastleGateWayAni66[3][0]];	//磷菌促..
					PutCompressedImage( x,y, sp );
				}
			}
			else						// 矫具俊 绝菌带 仇捞搁... 弊成 
			{
				sp = &CharSpr[ ch->sprno ].sp[ 0 ][CastleGateWayAni66[3][0]];
				PutCompressedImage( x,y, sp );
			}
		}
}

void DrawCastleGateWay67(int AliveType,LPCHARACTER ch,int x,int y)			// 011015 LTS
{
	Spr *sp;
	const char CastleGateWayAni67[4][69]={	
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},	// 己巩何辑瘤绰芭
		{ 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 3, 3, 3, 4, 4, 5, 6, 6, 7, 7, 8, 8, 9, 9,10,10,11,11,12,12,13,13,14,14,15,15,16,16, 6, 6, 7, 7, 8, 8, 9, 9,10,10,11,11,12,12,13,13,14,14,15,15,16,16, 6, 6, 7, 7, 8, 8, 9, 9,10},	// 飘坊胶
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };
		
		if (AliveType)
		{
			if (ch->sealstoneani<0||ch->sealstoneani>69) ch->sealstoneani=0;
			
			sp = &CharSpr[ ch->sprno ].sp[0][CastleGateWayAni67[0][ch->sealstoneani]];  
			PutCompressedImage( x,y, sp );                //窜鉴洒 靛肺雷 器牢飘甫 备茄促...
			ch->sealstoneani++;
		}
		else
		{
			if (ch->GuardStoneAni==1)	// 磷菌促绊 菩哦阑 罐阑锭 技泼..
			{
				if (ch->SetAni2Zero==1) 
				{
					ch->sealstoneani=0;
					ch->SetAni2Zero=0;
					int distx=ch->x-Hero->x;		// LTS SOUND
					int disty=ch->y-Hero->y;
					
					PlayListAutoSounds(260,distx,disty,0);	// LTS SOUND
				}
				if (ch->sealstoneani<69)
				{
					sp = &CharSpr[ ch->sprno ].sp[ 0 ][CastleGateWayAni67[1][ch->sealstoneani]];	//菊捞促..
					PutCompressedImage( x,y, sp );
					sp = &CharSpr[ ch->sprno ].sp[ 0 ][CastleGateWayAni67[2][ch->sealstoneani]];	//飘罚胶
					PutCompressedImageFX( x,y, sp,5,2);
					ch->sealstoneani++; 
				}
				else					// 俊聪皋捞记 风橇甫 倒疽促搁
				{
					sp = &CharSpr[ ch->sprno ].sp[ 0 ][CastleGateWayAni67[3][0]];	//磷菌促..
					PutCompressedImage( x,y, sp );
				}
			}
			else						// 矫具俊 绝菌带 仇捞搁... 弊成 
			{
				sp = &CharSpr[ ch->sprno ].sp[ 0 ][CastleGateWayAni67[3][0]];
				PutCompressedImage( x,y, sp );
			}
		}
}

void DrawGuardTower(int nAction, LPCHARACTER ch, int x, int y)
{	//< CSD-040317
	int& rChange = ch->mantle;
	int& rAction = ch->jjing;

	const char aGuardTower[6][56] =
	{   //1	  2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47  48  49  50  51  52  53  54  55  56
		// 扁夯
		{ 0,  0,  0,  0,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  6,  6,  6,  6,  0,  0,  0,  0,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  6,  6,  6,  6},
		{11, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16, 17, 17, 17, 17, 11, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16, 17, 17, 17, 17}, 
		// 傍拜
		{33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33},
		{26, 26, 26, 26, 27, 27, 27, 27, 28, 28, 28, 28, 29, 29, 29, 29, 29, 30, 30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 31, 31, 26, 26, 26, 26, 27, 27, 27, 27, 28, 28, 28, 28, 29, 29, 29, 29, 29, 30, 30, 30, 30, 30, 30, 31, 31, 31}, 
		// 磷扁
		{ 0,  0,  0,  0,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  6,  6,  6,  6,  7,  7,  7,  7,  7,  8,  8,  8,  8,  8,  9,  9,  9,  9,  9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
		{21, 21, 21, 21, 18, 18, 18, 18, 20, 20, 20, 20, 19, 19, 19, 19, 20, 20, 20, 20, 18, 18, 18, 18, 21, 21, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 27, 28, 28, 28, 29, 29, 29, 30, 30, 30, 31, 31, 31, 32, 32, 32}
	};
		
	Spr* sp = NULL;
	
	switch (nAction)
	{
	case MON1_BATTLE_WALK:
	case MON1_ATTACKED:
	case MON1_BATTLE_ATTACK:
	case MON1_STAND_UP:
	case MON1_CASTING_MAGIC:
	case MON1_BATTLE_ATTACK2:
	case MON1_BATTLE_ATTACK3:
	case MON1_JUST_ACTION:
		{
			nAction = MON1_NONE;
			break;
		}
	}

	if (nAction == MON1_NONE && rAction == MON1_BATTLE_MAGIC)
	{
		nAction = MON1_BATTLE_MAGIC;
	}

	if (nAction == MON1_NONE)
	{
		rChange = 0;

		if (ch->sealstoneani < 0 || ch->sealstoneani > 55)
		{
			ch->sealstoneani = 0;
		}

		sp = &CharSpr[ch->sprno].sp[0][aGuardTower[0][ch->sealstoneani]];
		PutCompressedImage(x, y, sp);
		sp = &CharSpr[ch->sprno].sp[0][aGuardTower[1][ch->sealstoneani]];
		PutCompressedImageFX(x, y, sp, 5, 2);
		++ch->sealstoneani;
	}

	if (nAction == MON1_BATTLE_MAGIC)
	{
		sp = &CharSpr[ch->sprno].sp[0][aGuardTower[2][ch->sealstoneani]];
		PutCompressedImage(x, y, sp);
		
		if (++ch->sealstoneani == 56)
		{
			rAction = MON1_NONE;
			return;
		}
	}

	if (nAction == MON1_DIE)
	{
		switch (rChange)
		{
		case 0:
			{
				sp = &CharSpr[ch->sprno].sp[0][aGuardTower[0][ch->sealstoneani]];
				PutCompressedImage(x, y, sp);
				sp = &CharSpr[ch->sprno].sp[0][aGuardTower[1][ch->sealstoneani]];
				PutCompressedImageFX(x, y, sp, 5, 2);

				if (++ch->sealstoneani >= 56)
				{
					ch->sealstoneani = 0;
					rChange = 1;
				}

				break;
			}
		case 1:
			{	//OutputDebugString(VA("%d, %d\n", ch->sealstoneani, aGuardTower[4][ch->sealstoneani]));
				sp = &CharSpr[ch->sprno].sp[0][aGuardTower[4][ch->sealstoneani]];
				PutCompressedImage( x,y, sp );
				sp = &CharSpr[ch->sprno].sp[0][aGuardTower[5][ch->sealstoneani]];
				PutCompressedImageFX(x, y, sp, 5, 2);
				
				if (++ch->sealstoneani >= 56)
				{
					ch->sealstoneani = 28;
					rChange = 2;
				}

				break;
			}
		default:
			{
				if (ch->sealstoneani >= 28)
				{
				
					sp = &CharSpr[ch->sprno].sp[0][aGuardTower[4][55]];
					PutCompressedImage( x,y, sp );
					sp = &CharSpr[ch->sprno].sp[0][aGuardTower[5][ch->sealstoneani]];
					PutCompressedImageFX(x, y, sp, 5, 2);
				}

				if (++ch->sealstoneani >= 56)
				{
					ch->sealstoneani = 28;
				}

				break;
			}
		}
	}

	rAction = nAction;
}	//> CSD-040317

// 010925 LTS _________________________________________________________________
void DrawDragon(LPCHARACTER ch,int nX,int nY)			// LTS DRAGON MODIFY
{
	nY -= ch->height;									//臭捞蔼阑 啊瘤绊 乐绰 某腐磐狼 臭捞蔼 哗具瘤!
	switch (ch->viewtype)
	{
	case VIEWTYPE_STONE_:
		{ 
			//			PutCompressedImageShadow(nX,nY-5, ch->sp);	
			PutCompressedImageCharRGB(nX, nY, ch->sp, COLOR_STONE);
			break;
		}
	case VIEWTYPE_POISON_:
		{
			//			PutCompressedImageShadow(nX,nY-5, ch->sp);	
			PutCompressedImageCharRGB(nX, nY, ch->sp, COLOR_POISON);
			break;
		}
	case VIEWTYPE_CONFUSION_:
		{
			//			PutCompressedImageShadow(nX,nY-5, ch->sp);	
			PutCompressedImageCharRGB(nX, nY, ch->sp, COLOR_DAZE);
			break;
		}
	case VIEWTYPE_FROZEN_:
		{
			//			PutCompressedImageShadow(nX,nY-5, ch->sp);	
			PutCompressedImageCharRGB(nX, nY, ch->sp, COLOR_FROZEN);
			break;
		}
	case VIEWTYPE_SLOW_:
		{
			//			PutCompressedImageShadow(nX,nY-5, ch->sp);	
			PutCompressedImageCharRGB(nX, nY, ch->sp, COLOR_SLOW);
			break;
		}
	case VIEWTYPE_CURSE_:
		{
			//			PutCompressedImageShadow(nX,nY-5, ch->sp);	
			PutCompressedImageCharRGB(nX, nY, ch->sp, ReturnBlendRGB( 180, 255, 255));
			break;
		} 
		
	default :
		{
			if (ch->nCurrentAction==2) 
			{
				PutCompressedImageCharRGB(nX, nY, ch->sp, ReturnBlendRGB( 255, 0, 0));
				PutCompressedImageFX(nX, nY, ch->sp,rand()%8,2);
			}
			else PutCompressedImage(nX, nY, ch->sp);
			break;	
		}
	}
}

void DragonDeath(LPCHARACTER ch,int nX,int nY)			// LTS DRAGON MODIFY
{
	static char Mix=32;
	nY -= ch->height;									//臭捞蔼阑 啊瘤绊 乐绰 某腐磐狼 臭捞蔼 哗具瘤!
	if (ch->dieani==0) 
	{
		Mix=32;
	}
	PutCompressedImageFX(nX,nY,ch->sp,Mix,2);
	Mix--;
	if (Mix<0) Mix=0;
}


void DrawStateNPC(int nState, Spr* pData, int nX, int nY)
{
	switch (nState)
	{
	case VIEWTYPE_STONE_:
		{ 
			PutCompressedImageCharRGB(nX, nY, pData, COLOR_STONE);
			break;
		}
	case VIEWTYPE_POISON_:
		{
			PutCompressedImageCharRGB(nX, nY, pData, COLOR_POISON);
			break;
		}
	case VIEWTYPE_CONFUSION_:
		{
			PutCompressedImageCharRGB(nX, nY, pData, COLOR_DAZE);
			break;
		}
	case VIEWTYPE_FROZEN_:
		{
			PutCompressedImageCharRGB(nX, nY, pData, COLOR_FROZEN);
			break;
		}
	case VIEWTYPE_SLOW_:
		{
			PutCompressedImageCharRGB(nX, nY, pData, COLOR_SLOW);
			break;
		}
	case VIEWTYPE_CURSE_:
		{
			PutCompressedImageCharRGB(nX, nY, pData, ReturnBlendRGB( 180, 255, 255));
			break;
		} 
    case VIEWTYPE_SELECT_:
		{
			static int ao, aa = 10;
			if (ao)	{ aa++; if( aa >  8 )	{ ao = !ao; }}
			else		{ aa--; if( aa <  7  )	{ ao = !ao; }}
			
			PutCompressedImageCharRGB(nX, nY, pData, ReturnBlendRGB(255, 0, 0));
			PutCompressedImageFX(nX, nY, pData, aa/2, 2);
			
			break;	
		}
	default:
		{
			PutCompressedImage(nX, nY, pData);
			break;	
		}
	}
}

void h_DisplayNPC( LPCHARACTER ch, int shadow )
{
	const int nFrame = ch->AnimationTable[ch->nCurrentAction].cFrame[ch->nCurrentFrame];
	const int nSprNo = ch->sprno;
		
	switch (nSprNo)	// LTS DRAGON MODIFY
	{	//< CSD-030515
	case 50:
	case 51:
	case 52:
		{
			ch->direction = DIRECTION_DOWN;
			ch->todir = DIRECTION_LEFTDOWN;
			ch->sp = &CharSpr[nSprNo].sp[DIRECTION_DOWN][nFrame];
			break;
		}
	case 118:
	case 119:
	case 177:		// LTH-040403-KO 规氢 绊沥 => 捞巴 巩力肺 寇己荐龋籍苞 己巩捞 傍拜登促 富疽带 巴捞菌寸 -_-;
	case 178:		// LTH-040403-KO 规氢 绊沥 => 捞巴 巩力肺 寇己荐龋籍苞 己巩捞 傍拜登促 富疽带 巴捞菌寸 -_-;
		{
			ch->direction = DIRECTION_DOWN;
			ch->todir = DIRECTION_DOWN;
			ch->sp = &CharSpr[nSprNo].sp[DIRECTION_DOWN][nFrame];
			break;
		}
	}	//> CSD-030515

	if (ch->nRace == GUARDTOWER)
	{	//< CSD-040202
		ch->direction = DIRECTION_DOWN;
		ch->todir = DIRECTION_DOWN;
		ch->sp = &CharSpr[nSprNo].sp[DIRECTION_DOWN][nFrame];
	}	//> CSD-040202

	const int nDir = ch->direction;
	const int nMutant = ch->mutant;
	ch->sp0	= &CharLayerSpr[nSprNo][0][nMutant].sp[nDir][nFrame];
	ch->sp1	= &CharLayerSpr[nSprNo][1][nMutant].sp[nDir][nFrame];
	ch->sp2	= &CharLayerSpr[nSprNo][2][nMutant].sp[nDir][nFrame];
	ch->sp3	= &CharLayerSpr[nSprNo][3][nMutant].sp[nDir][nFrame];
	ch->sp4	= &CharLayerSpr[nSprNo][4][nMutant].sp[nDir][nFrame];
	// 老馆 荐龋籍狼 俊聪皋捞记
	static int ttt, ttto;
	int x = ch->x;
	int y = ch->y;
	int dieani = 0;
	Spr *sp;
	
	int i,j;
	int ex, ey, sx, sy;
	sx = x - ch->sp->ox;		 sy = y - ch->sp->oy; 
	ex = sx +  ch->sp->xl;		 ey = sy + ch->sp->yl;
	
	x -= Mapx;
	y -= Mapy;
	
	
#ifdef _DEBUG
	if(SysInfo.notconectserver)
	{					
		HprintBold( x, y - ch->sp->yl, RGB( 0xff, 0xff, 0xff ), 0, "xl:%d  yl:%d", ch->sp->xl, ch->sp->yl );
	}					
#endif
	
	
	if ((ch->nCurrentAction == MON1_DIE && ch->dieani != 0) || ch->blood_Set == true)
	{	//< CSD-040316
		PutDeadEffectBottom(ch);
		
		switch (ch->sprno)
		{
		case 63:
		case 178:
			{	// 010924 LTS
				DrawNGuardStone(0, ch, x, y);
				return;
			}
		case 64:
			{
				DrawBGuardStone(0, ch, x, y);
				return;
			}
		case 65:
		case 66:
		case 67:
		case 177:
			{	// LTH-040216-KO
				DrawCastleGateWay67(0, ch, x, y);
				return;
			}
		case 50:
		case 51:
			{	// LTS DRAGON MODIFY
				DragonDeath(ch, x,y);
				return;
			}
		}
		
		if (ch->nRace == GUARDTOWER)
		{
			DrawGuardTower(ch->nCurrentAction, ch, x, y);
			return;
		}
	}	//> CSD-040316				
		
	if( 0 < ch->dieani && ch->dieani < 32 )
	{					
		PutCompressedImageFX( x,y, ch->sp, ch->dieani, 1 );
		return; // 0810 KHS
	}	
	
	switch (ch->sprno)
	{	//< CSD-040316
	case 63:
	case 178:
		{	// LTH-040216-KO
			 DrawNGuardStone(1, ch, x, y);
			 return;
		}
	case 64:
		{
			DrawBGuardStone(1, ch, x, y);
			return;
		}
	case 65:
		{
			DrawCastleGateWay65(1, ch, x, y);
			return;
		}
	case 66:
		{
			DrawCastleGateWay66(1, ch, x, y);
			return;
		}
	case 67:
		{
			DrawCastleGateWay67(1, ch, x, y);
			return;
		}
	case 177:
		{	// LTH-040216-KO
			DrawCastleGateWay67(1, ch, x, y);
			return;
		}
	case 50:
	case 51:
	case 52:
		{	// LTS DRAGON MODIFY
			DrawDragon(ch,x,y);
			return;
		}
	}	//> CSD-040316

	if (ch->nRace == GUARDTOWER)
	{	//< CSD-040202
		DrawGuardTower(ch->nCurrentAction, ch, x, y);
		return;
	}	//> CSD-040202
	
	if( ch->sealstone ) // 搬拌籍.
	{	
		sp = &CharSpr[ ch->sprno ].sp[ 0 ][ 0];
		PutCompressedImageShadow( x,y-5, sp);	
		PutCompressedImage( x,y, sp );
		PutCompressedImageFX( x,y, ch->sp, 24, 2 );	
		ch->sealstoneani -= 5; 
		return;
	}	
	
	
	if(ch->sprno==45 )	
	{	
		static int countimsi ;
		countimsi = rand()%6;
		PutCompressedImageFX( x,y, ch->sp, 20, 2 );	// 1011 kkh 烙矫 
		goto NPC_MUTANT_;
	}	
	
	
	if(ch->sprno==6 )	//浇贰烙
	{	
		PutCompressedImageFX( x,y, ch->sp, 24, 4 );	// 浇贰烙..
		return;
	}	
	else if( shadow == 0 ) 
	{	
		PutCompressedImageShadow( x,y-5, ch->sp);	
		return;
	}	
	else
	{	
		y -= ch->height;		//臭捞蔼阑 啊瘤绊 乐绰 某腐磐狼 臭捞蔼 哗具瘤!
NPC_MUTANT_:
		const int nState = (ch == g_GameInfo.lpcharacter) ? VIEWTYPE_SELECT_:ch->viewtype;
		
		switch (nMutant)
		{ 
		case MUTANT_ENCHANT:	
			{
				if (nState != VIEWTYPE_NORMAL_)
				{
					DrawStateNPC(nState, ch->sp, x, y);
					break;
				}
				
				if (rand()%5)
				{
					PutCompressedImageCharRGB(x, y, ch->sp, ReturnBlendRGB(250, 255, 58));
				}
				else 
				{
					DrawStateNPC(nState, ch->sp, x, y);
				}
				
				PutCompressedImageFX(x - 1 + (rand()%3), y - 1 + (rand()%3), ch->sp, (rand()%3)*6, 6);
				break;
			}
		case MUTANT_MADDER:	
			{
				if (nState != VIEWTYPE_NORMAL_)
				{
					DrawStateNPC(nState, ch->sp, x, y);
					break;
				}
				
				if (rand()%5)
				{
					PutCompressedImageCharRGB(x, y, ch->sp, ReturnBlendRGB(70, 210, 75));
				}
				else 
				{
					DrawStateNPC(nState, ch->sp, x, y);
				}
				
				PutCompressedImageFX(x, y - 1, ch->sp, (rand()%3)*6 , 6);
				break;
			}
		case MUTANT_BOSS:	
			{
				if (nState != VIEWTYPE_NORMAL_)
				{
					DrawStateNPC(nState, ch->sp, x, y);
					break;
				}
				
				if (rand()%5)
				{
					PutCompressedImageCharRGB(x, y, ch->sp, ReturnBlendRGB(100, 100, 100));
				}
				else
				{
					DrawStateNPC(nState, ch->sp, x, y);
				}
				
				ch->nutantani++;
				if (ch->nutantani > 6)  ch->nutantani = 0;
				PutCompressedImageVoid(x - 1 + (rand()%3), y - (ch->nutantani*3), ch->sp);	// 浇贰烙..
				PutCompressedImageVoid(x - 1 + (rand()%3), y - (ch->nutantani), ch->sp);	// 浇贰烙..
				break;
			}
		case MUTANT_HIGH_RANK:
			{ 
				DrawStateNPC(nState, ch->sp, x, y);
				DrawStateNPC(nState, ch->sp0, x, y);
				DrawStateNPC(nState, ch->sp1, x, y);
				DrawStateNPC(nState, ch->sp2, x, y);
				PutCompressedImageFX(x, y, ch->sp3, 20, 2);
				break;
			}
		case MUTANT_SPECIAL:
			{ 
				DrawStateNPC(nState, ch->sp, x, y);
				DrawStateNPC(nState, ch->sp0, x, y);
				DrawStateNPC(nState, ch->sp1, x, y);
				DrawStateNPC(nState, ch->sp2, x, y);
				
				static int nChange;
				
				if (nChange/2 == 0)
				{
					PutCompressedImageFX(x, y, ch->sp3, 20, 2);
				}
				else
				{
					PutCompressedImageFX(x, y, ch->sp4, 20, 2);
				}
				break;
			}
		case MUTANT_COMPANY:
			{ 
				static int nChange;
				DrawStateNPC(nState, ch->sp, x, y);
				DrawStateNPC(nState, ch->sp0, x, y);
				DrawStateNPC(nState, ch->sp1, x, y);
				DrawStateNPC(nState, ch->sp2, x, y);
				(nChange/2) ? DrawStateNPC(nState, ch->sp3, x, y):DrawStateNPC(nState, ch->sp4, x, y);
				break;
			}
		default:
			{
				DrawStateNPC(nState, ch->sp, x, y);
				break;
			}
    }
  }
  
  if( ch->height > 100 ) return;
  
  if( !tool_DrawFrontMap )
  {
	  ex += (TILE_SIZE + 1 ); ey += (TILE_SIZE + 1 );
	  ex += ((ex-sx)/2); // 弊覆磊 锭巩俊.
	  sx	/= TILE_SIZE;		sy	/= TILE_SIZE;
	  ex	/= TILE_SIZE;		ey	/= TILE_SIZE;
	  
	  
	  if( sx < 0 ) sx = 0;
	  if( sy < 0 ) sy = 0;
	  if( ex >= g_Map.file.wWidth  ) ex = g_Map.file.wWidth;
	  if( ey >= g_Map.file.wHeight ) ey = g_Map.file.wHeight;
	  
	  
	  y +=Mapy;
	  for( i = sx ; i < ex ; i ++ )
		  for( j = sy ; j < ey ; j ++ )
		  {	
			  int front = TileMap[ i][ j].front;
			  if(  front )
				  if(  (int )TileMap[ i][ j].frontoffset * TILE_SIZE >= y )
				  { 
					  if( Sou[ front].xl == -1 )		PutTile( i*TILE_SIZE - Mapx,  j*TILE_SIZE - Mapy,  Sou[front].img, g_DestBackBuf );
					  else							PutCompressedImage(  i*TILE_SIZE - Mapx,  j*TILE_SIZE - Mapy,  &Sou[ front] );
				  }
		  }	
  }
}

//--------------------------- khs 0804
void h_DisplayPC_Temp( LPCHARACTER ch )
{				
	int x = 200;
	int y = 200;
	int dir = SpriteOrderDir;
	int sprc =  SpriteOrderFrame;
	
	int	*accessory = ch->accessory;
	int sprno = ch->sprno;
	
	int ac0 = ch->accessory[0];
	int ac1 = ch->accessory[1];
	int ac2 = ch->accessory[2];
	int ac3 = ch->accessory[3];
	
	PutCompressedImageShadow( x,y, &CharSpr[sprno].sp[ dir ][ sprc]);
	
	int height =  ch->AnimationTable[ ch->nCurrentAction].height[  ch->nCurrentFrame ];
	y -= height;
	
	PutCompressedImage( x,y, &CharSpr[sprno].sp[ dir ][ sprc]);
	
	if( ch->sprno > 1 ) return;
	
	switch( SpriteOrder[ sprno ][ dir][ sprc] )
	{			
	case 0 : //	个.公扁.规菩
		PutCompressedImageShadow( x,y, &CharSpr[sprno].sp[ dir ][ sprc]);
		PutCompressedImage( x,y, &CharSpr[sprno].sp[ dir ][ sprc]);
		
		if( ac0 )  // 癌渴..
		{		
			PutCompressedImage( x,y, &CharAccessorySpr[sprno][ ac0  ].sp[ dir ][ sprc]);//,  (15<<11) | (0<<6) | ( 0 ));
			PutCompressedImage( x,y, &CharAccessorySpr[sprno][ ac0+1].sp[ dir ][ sprc]);
		}		
		if( ac1 != 1 )
			if( ac1 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac1].sp[ dir ][ sprc]);  // 捧备..
			
			if( ac2 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac2].sp[ dir ][ sprc]);  // 公扁..
			if( ac3 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac3].sp[ dir ][ sprc]);  // 规菩..
			break;		
	case 1 : //	个. 规菩, 漠 
		PutCompressedImageShadow( x,y, &CharSpr[sprno].sp[ dir ][ sprc]);
		PutCompressedImage( x,y, &CharSpr[sprno].sp[ dir ][ sprc]);
		if( ac0 )  // 癌渴.
		{		
			PutCompressedImage( x,y, &CharAccessorySpr[sprno][ ac0  ].sp[ dir ][ sprc]);//,  (15<<11) | (0<<6) | ( 0 ));
			PutCompressedImage( x,y, &CharAccessorySpr[sprno][ ac0+1].sp[ dir ][ sprc]);
			
		}		
		if( ac1 != 1 )
			if( ac1 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac1].sp[ dir ][ sprc]);  // 捧备..
			
			if( ac3 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac3].sp[ dir ][ sprc]);  // 规菩..
			if( ac2 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac2].sp[ dir ][ sprc]);  // 公扁..
			break;		
	case 2 : //	漠. 个. 规菩
		if( ac2 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac2].sp[ dir ][ sprc]);  // 公扁..
		PutCompressedImageShadow( x,y, &CharSpr[sprno].sp[ dir ][ sprc]);
		PutCompressedImage( x,y, &CharSpr[sprno].sp[ dir ][ sprc]);
		if( ac0 )  // 癌渴..
		{	
			PutCompressedImage( x,y, &CharAccessorySpr[sprno][ ac0  ].sp[ dir ][ sprc]);//,  (15<<11) | (0<<6) | ( 0 ));
			PutCompressedImage( x,y, &CharAccessorySpr[sprno][ ac0+1].sp[ dir ][ sprc]);
		}		
		if( ac1 != 1 )
			if( ac1 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac1].sp[ dir ][ sprc]);  // 捧备..
			if( ac3 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac3].sp[ dir ][ sprc]);  // 规菩..
			break;
	case 3 : //	漠 规菩, 个
		if( ac2 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac2].sp[ dir ][ sprc]);  // 公扁..
		if( ac3 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac3].sp[ dir ][ sprc]);  // 规菩..
		
		PutCompressedImageShadow( x,y, &CharSpr[sprno].sp[ dir ][ sprc]);
		PutCompressedImage( x,y, &CharSpr[sprno].sp[ dir ][ sprc]);
		if( ac0 )  // 癌渴..
		{		
			PutCompressedImage( x,y, &CharAccessorySpr[sprno][ ac0  ].sp[ dir ][ sprc]);//,  (15<<11) | (0<<6) | ( 0 ));
			PutCompressedImage( x,y, &CharAccessorySpr[sprno][ ac0+1].sp[ dir ][ sprc]);
		}		
		if( ac1 != 1 )
			if( ac1 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac1].sp[ dir ][ sprc]);  // 捧备..
			break;	
	case 4 : //	规菩, 漠, 个
		if( ac3 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac3].sp[ dir ][ sprc]);  // 规菩..
		if( ac2 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac2].sp[ dir ][ sprc]);  // 公扁..
		
		PutCompressedImageShadow( x,y, &CharSpr[sprno].sp[ dir ][ sprc]);
		PutCompressedImage( x,y, &CharSpr[sprno].sp[ dir ][ sprc]);
		if( ac0 )  // 癌渴..
		{		
			PutCompressedImage( x,y, &CharAccessorySpr[sprno][ ac0  ].sp[ dir ][ sprc]);//,  (15<<11) | (0<<6) | ( 0 ));
			PutCompressedImage( x,y, &CharAccessorySpr[sprno][ ac0+1].sp[ dir ][ sprc]);
		}	
		if( ac1 != 1 )
			if( ac1 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac1].sp[ dir ][ sprc]);  // 捧备..
			break;	
	case 5 : // 规菩, 个, 漠
		
		if( ac3 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac3].sp[ dir ][ sprc]);  // 规菩..
		
		PutCompressedImageShadow( x,y, &CharSpr[sprno].sp[ dir ][ sprc]);
		PutCompressedImage( x,y, &CharSpr[sprno].sp[ dir ][ sprc]);
		if( ac0 )  // 癌渴..
		{		
			PutCompressedImage( x,y, &CharAccessorySpr[sprno][ ac0  ].sp[ dir ][ sprc]);//,  (15<<11) | (0<<6) | ( 0 ));
			PutCompressedImage( x,y, &CharAccessorySpr[sprno][ ac0+1].sp[ dir ][ sprc]);
		}
		if( ac1 != 1 )
			if( ac1 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac1].sp[ dir ][ sprc]);  // 捧备..
			if( ac2 ) PutCompressedImage( x,y, &CharAccessorySpr[sprno][ac2].sp[ dir ][ sprc]);  // 公扁..
			break;	
	}			
}				

inline int IsNation( void )
{
	int na;
	
	switch( MapInfo[ MapNumber].nation )
	{
	case N_VYSEUS	:	
	case N_ZYPERN	: 	
	case N_YILSE	:	na = MapInfo[ MapNumber].nation; break;
	default :	
		switch( Hero->name_status.nation )
		{
		case N_VYSEUS	:
		case N_ZYPERN	:
		case N_YILSE	:		na = Hero->name_status.nation;
		}break;
	}
	
	return na;
}

int IsNK( LPCHARACTER ch )
{
	switch( IsNation() )
	{
	case N_VYSEUS	:		return ch->nk3;
	case N_ZYPERN	: 		return ch->nk4;
	case N_YILSE	:		return ch->nk6;
		break;
	}
	
	return 0;
}



int IsNK( LPCHARACTER ch, int nation )
{
	switch( nation )
	{
	case N_VYSEUS	:		return ch->nk3;
	case N_ZYPERN	: 		return ch->nk4;
	case N_YILSE	:		return ch->nk6;
		break;
	}
	
	return 0;
}





int IsNK( LPCHARACTER ch, char *nation )
{	
	switch( IsNation() )
	{
	case N_VYSEUS	:		strcpy( nation, "Vysues NK:" ); return ch->nk3;
	case N_ZYPERN	: 		strcpy( nation, "Zypern NK:" ); return ch->nk4;
	case N_YILSE	:		strcpy( nation, "Yilse NK:" );  return ch->nk6;
		break;
	default :				*nation = 0;
		break;
	}
	
	return 0;
}

inline bool IsEnemyNation(int anation, int bnation)
{	//< CSD-040127
	if (anation != bnation)
	{
		if (anation == 3 && bnation == 4)
		{
			return true;
		}

		if (anation == 4 && bnation == 3)
		{
			return true;
		}
	}
	
	return false;
}	//> CSD-040127



extern int NPCCOUNT;
BOOL AddCharacter( LPCHARACTERLIST lpCharacterList, LPCHARACTER ch, LPGPCHARINFO gpch)
{
	LPCHARACTER		lpNew = NULL;
	
	MemAlloc( lpNew, sizeof( CHARACTER ) );
	
	lpNew->type				= gpch->type;
	lpNew->id				= gpch->id;
	lpNew->sprno 			= gpch->sprno;
	lpNew->mutant			= gpch->mutant;		// 0810 KHS
	lpNew->sex				= gpch->sex;
	lpNew->gox  = lpNew->x				= gpch->position.x;
	lpNew->goy  = lpNew->y				= gpch->position.y;
	
	lpNew->position.x		= gpch->position.x / TILE_SIZE;
	lpNew->position.y		= gpch->position.y / TILE_SIZE;
	
	lpNew->oldposition = lpNew->position;
	
	lpNew->animationType	= gpch->animationType;
	lpNew->nCurrentAction	= gpch->nCurrentAction;
	lpNew->basicAction 		= gpch->basicAction;
	lpNew->basicWalk 		= gpch->basicWalk;
	lpNew->todir = lpNew->direction	= gpch->direction; // CSD-030419
	
	lpNew->nCurrentFrame	=  rand()%3;
	lpNew->pathcount		=  0;
	
	lpNew->hp	= gpch->hp;
	lpNew->mp	= gpch->mp;
	lpNew->Sp	= gpch->sp;
	lpNew->hpmax= gpch->hpmax;
	lpNew->mpmax= gpch->mpmax;
	lpNew->Spmax= gpch->spmax;
	
	lpNew->spell= gpch->spell;
	lpNew->face	= gpch->face;
	
	lpNew->light			= gpch->sight = 8;
	lpNew->ChatDelay		=  0;
				
	lpNew->accessory[0] = gpch->accessory[0];
	lpNew->accessory[1] = gpch->accessory[1];
	lpNew->accessory[2] = gpch->accessory[2];
	lpNew->accessory[3] = gpch->accessory[3];
	// 011018 KHS 噶肚
	lpNew->mantle		= gpch->mantle;
	lpNew->mantlecolor	= GetItemAttr( gpch->mantle, ITEM_COLOR );
	
	lpNew->jjing		= gpch->jjing; // 011030 KHS JJING
	lpNew->JoinLocalWar	= gpch->JoinLocalWar;	// 011213 LTS
	lpNew->ChairNum		= gpch->ChairNum;	// LTS SITDOWN BUG			// LTS AI NPC AI俊 且寸 
	
//	lpNew->set_Sp(lpNew->Sp);//SP截获.在这里初始化,具体还要在别的函数里截获
	lpNew->set_mutant(lpNew->mutant);//转职截获.在这里初始化,具体还要在别的函数里截获
	lpNew->set_sight(lpNew->sight);//视野截获
	lpNew->set_movp(lpNew->movp);//移动力截获.在这里初始化,具体还要在别的函数里截获
	lpNew->set_nAttackedType(lpNew->nAttackedType);//攻击类型截获.在这里初始化,具体还要在别的函数里截获
	lpNew->set_bCanViewGhost(lpNew->bCanViewGhost);//看鬼开关截获,在这里初始化,
	lpNew->set_excute_MagicNum(lpNew->excute_MagicNum);//释放魔法类型
	lpNew->set_continue_RunMagic(lpNew->continue_RunMagic);////魔法持续时间截获,	
	lpNew->set_nActiveCombat(lpNew->nActiveCombat);//弓盗技能截获
	lpNew->set_nActiveLevel(lpNew->nActiveLevel);//弓盗技能等级截获
	lpNew->set_nCurrentAction(lpNew->nCurrentAction); //人物动作行为
	lpNew->set_nCurrentFrame(lpNew->nCurrentFrame); //人物晃动值	
	set_is_mainmenu_active(is_mainmenu_active);//窗口基址
	g_Menu_Variable.set_bCheckSalvation(g_Menu_Variable.bCheckSalvation);//无限名声
	lpNew->set_bow_Type(lpNew->bow_Type); //魔法冰箭
	/////////////////////////////***新加的***
	lpNew->set_ready_Magic(lpNew->ready_Magic);
	lpNew->set_start_Casting(lpNew->start_Casting);
	lpNew->set_end_Casting(lpNew->end_Casting);
	lpNew->set_end_ReadyMagic(lpNew->end_ReadyMagic);
	lpNew->set_end_RunMagic(lpNew->end_RunMagic);
	lpNew->set_castStartingTime(lpNew->castStartingTime); //魔法间隔时间?

#ifndef LIGHT_VERSION_
	lpNew->HorseNo		= gpch->HorseNo;		// LTS HORSERIDER
	lpNew->RiderNo		= gpch->RiderNo;		// LTS HORSERIDER
	lpNew->HorseGrade	= gpch->HorseGrade;		// LTS HORSERIDER
	lpNew->RiderGrade	= gpch->RiderGrade;		// LTS HORSERIDER
#endif
	
	lpNew->DragonLordWarTeam = gpch->DragonLordWarTeam;		// LTS DRAGONLORD
	
	lpNew->bodycolor	= ReturnBlendRGB( gpch->bodyr, gpch->bodyg, gpch->bodyb );
	lpNew->clothcolor	= ReturnBlendRGB( gpch->clothr, gpch->clothg, gpch->clothb );
	lpNew->sight		= gpch->sight;
	//< CSD-031007
	const WORD idWeaponItem = gpch->idWeaponItem; 

	if (idWeaponItem/1000 == 6)
	{	//< CSD-031101
		CItem_Weapon* pWeapon = static_cast<CItem_Weapon*>(ItemUnit(idWeaponItem));

		if (idWeaponItem != 0)
		{
			if (pWeapon != NULL)
			{
				const int nEffect = pWeapon->effect;

				if (nEffect > 0)
				{
					LoadHeroClothAccessoryDataOne(lpNew->sprno, nEffect);
				}

				switch (ch->sprno)
				{
				case 0: LoadHeroClothAccessoryDataOne(lpNew->sprno, pWeapon->shape_woman); break;
				case 1: LoadHeroClothAccessoryDataOne(lpNew->sprno, pWeapon->shape_man); break;
				}
			}
		}
	}	//> CSD-031101

	lpNew->idWeaponItem = idWeaponItem;
	//> CSD-031007
	memcpy( lpNew->name, gpch->name, 20 );
	
	lpNew->HostName[0] = 0; // 老窜 hostname篮 蝶肺 罐霸等促. 
	
	lpNew->age  		= gpch->Age;
	lpNew->viewtype		= gpch->viewtype;
	lpNew->nAttr = gpch->nAttr;
	lpNew->nRace = gpch->nRace; // CSD-030419
	CheckAndSetPartyOnMySight( lpNew, 1 );		// 010404 YGI
	
	if (gpch->bRecallNpc)
	{	//< CSD-021119
		::InsertMagic(lpNew, lpNew, 453, 8, 0, 0, lpNew->x, lpNew->y, false, 0, true);
	}	//> CSD-021119
	
	if (lpNew->sprno > 1)
	{
		const int nSprNo = lpNew->sprno;
		
		if( CharBuf[nSprNo] == NULL )
		{	
			char temp[FILENAME_MAX];
			sprintf(temp, "./char/%03d000.spr", nSprNo);
			LoadCharSpriteData(temp, &CharBuf[nSprNo], &CharBufLength[nSprNo], &CharSpr[nSprNo], LD_CHAR_SPRITE_ALL_LOAD);
			CharBufCount[nSprNo] = 1;
		}	
		
		const int nMutant = lpNew->mutant;
		
		if (nMutant >= MUTANT_HIGH_RANK && nMutant <= MUTANT_COMPANY)
		{ 
			for (int i = 0; i < MAX_CHARACTOR_PART_; i++)
			{
				if (CharLayerBuf[nSprNo][i][nMutant] == NULL)
				{
					char szBuffer[FILENAME_MAX];
					sprintf(szBuffer, "./char/%03d%01d%02d.spr", nSprNo, i, nMutant);
					LoadCharSpriteData(szBuffer, 
						&CharLayerBuf[nSprNo][i][nMutant],
						&CharLayerBufLength[nSprNo][i][nMutant],
						&CharLayerSpr[nSprNo][i][nMutant], 
						LD_CHAR_SPRITE_ALL_LOAD);
					CharLayerBufCount[nSprNo][i][nMutant] = 1;
				}
			}
		} 
	}
	else 
	{
		CharBufCount[ lpNew->sprno] ++;
	}
	
#ifdef _DEBUG	
	if( SysInfo.makeanitable_bin )
	{		
		int tempsprno = lpNew->sprno;
		char temp[ FILENAME_MAX];
		sprintf( temp, "./char/AniTime.txt" );
		FILE *fp = Fopen( temp, "wt" );
		for( int i = 0 ; i < 92 ; i ++)
		{	
			lpNew->sprno = i;
			lpNew->TotalAni = 0;
			LoadChacterAnimationData( lpNew );
			if( lpNew->TotalAni )
			{
				EatRearWhiteChar(g_infNpc[i].szName); // CSD-030419
				fprintf( fp, "<%2d> %s \n"
					"------------------------------------------------------------------------\n", i+1, g_infNpc[i].szName); // CSD-030419
				for( int j = 0 ; j < lpNew->TotalAni ; j ++)
				{
					fprintf( fp, lan->OutputMessage(7,1), j+1, AnimationName[j], lpNew->AnimationTable[j].nCount,  lpNew->AnimationTable[j].nCount / 15.0  );
				}
				fprintf( fp, "========================================================================\n\n" );
			}
		}	
		fclose(fp);
		lpNew->sprno = tempsprno;
	}		
#endif		
	
	lpNew->light		= 18;
	
	lpNew->peacests		= gpch->peacests;
	if( lpNew->peacests < 0 || lpNew->peacests > 1 ) lpNew->peacests = 0;
	
	
	
	LoadChacterAnimationData( lpNew );
	SettingBasicActionSub( lpNew );
	
	if( lpNew->hp <= 0 && lpNew->viewtype != VIEWTYPE_GHOST_ )
	{		
		if( lpNew->id >= 10000 ) 
		{
			lpNew->nCurrentAction	= MON1_DIE;
			lpNew->diedelay			= 3;
			lpNew->dieani			= 33;
			lpNew->blood_Set		= true;
	lpNew->set_nCurrentAction(lpNew->nCurrentAction); //人物动作行为

		}
		else 
		{
			lpNew->diedelay		= 3;
			lpNew->dieani		= 33;
			lpNew->blood_Set	= true;
			lpNew->nCurrentAction = ACTION_DIE;
		}
		lpNew->nCurrentFrame = lpNew->AnimationTable[ lpNew->nCurrentAction].nCount -1;
	lpNew->set_nCurrentFrame(lpNew->nCurrentFrame); //人物晃动值

	}		
	
	if( SysInfo.notconectserver )
	{	
		lpNew->hp		=   1;
		lpNew->movp		= 70;
		lpNew->set_movp(lpNew->movp);//移动力截获.DEBUG模式
		lpNew->sight	= 720;
		lpNew->set_sight(lpNew->sight);//视野,DEBUG模式
	}	
	
	if ( ch == NULL )
	{				
		lpNew->lpPrev = lpNew->lpNext = NULL;
		lpCharacterList->lpFirst	= lpCharacterList->lpLast = lpNew;
		lpCharacterList->nCount		= 1;
	}	
	else
	{	
		lpNew->lpPrev =    ch;
		lpNew->lpNext =  NULL;
		ch->lpNext	  = lpNew;
		lpCharacterList->lpLast = lpNew;
		lpCharacterList->nCount++;
	}	
	
	switch (lpNew->viewtype)
	{
	case VIEWTYPE_GHOST_:	
		{
			lpNew->ghostani = 15; 
			lpNew->ghostanito = 0;
		}break;
    case VIEWTYPE_STAND_UP_:
		{
			if (lpNew->sprno > 1)  
			{
				if (lpNew->nCurrentAction == MON1_STAND_UP)
				{
					lpNew->nCurrentAction = MON1_NONE;
				}
				else
				{
					lpNew->nCurrentAction = MON1_STAND_UP;
				}
			}
		}break;
	}
	
	switch( lpNew->sprno )
	{
	case  3	:
	case  5:
	case  6:
	case  7:
	case  8:
	case  9:
	case  10:
	case  13:
	case  14:
	case  15:
	case  16:
	case  49:
	case  50:
	case  51:
	case  54:
	case  31:
	case  35:
	case  36:
	case  37:
	case  38:
	case  39:
	case  40:
	case  41:
	case  42:
	case  43:
	case  44:
	case  45:
	case  46:
	case  47:
	case  48:	
		{
			lpNew->notcomeinside = 1;
		}break;
	default :	
		{
			lpNew->notcomeinside = 0;
		}break;
	}
	
	switch( lpNew->sprno )
	{
	case 0 :
	case 1 :
	case 18 :	
	case 19 :	
	case 20 :	
	case 21 :	
	case 22 :	
	case 23 :	
	case 24 :	
	case 56 :	
	case 57 :	
	case 71 :	
	case 72 :	
	case 73 :	
	case 74 :
	case 75 :
	case 76 :
	case 77 :
	case 78 :
	case 79 :
	case 80 :	lpNew->dummy_pk = 0;
		break;
		
		//		default :	lpNew->dummy_pk = 100;
		break;
	}
	
	
	// 010423 KHS
	// SealStone...
	switch( lpNew->sprno )
	{
	case  98 :
	case  99 : 
	case  91 : lpNew->sealstone = true;
		break;
	default :  lpNew->sealstone = false;
	}
	
	
	lpNew->social_status	= gpch->social_status;
	lpNew->name_status		= gpch->name_status;			// 1004 YGI
	lpNew->SetGuildCode(gpch->guild_code); // CSD-031001
	lpNew->fame				= gpch->fame;
	
	lpNew->nk3				= gpch->nk3;
	lpNew->nk4				= gpch->nk4;
	lpNew->nk6				= gpch->nk6;
	
	lpNew->lv				= gpch->lv;				//	0807 YGI
	lpNew->class_type		= gpch->class_type;		//	0807 YGI
	
	lpNew->homex			= gpch->homex;
	lpNew->homey			= gpch->homey;
	
	memcpy(&lpNew->NWCharacter,&gpch->NWCharacter,sizeof(DWORD));	// 010915 LTS	// LOOP UNROLL阑 荤侩窍瘤 臼绰 捞蜡..
	// 窜鉴墨乔窍绰单  老老捞 内爹秦 拎具 茄促.
	// 狐瘤搁 滚弊狼 盔牢捞 等促.
	
	/*
	//__________________________________________ // 010904 LTS
	lpNew->isCommander		= gpch->isCommander;	//惫啊傈 包访 
	lpNew->isSquadLeader	= gpch->isSquadLeader;	// LTS CHAR
	lpNew->WarNo			= gpch->WarNo;
	lpNew->WarfieldNo		= gpch->WarfieldNo;
	lpNew->SquadNo			= gpch->SquadNo;
	lpNew->YL_JoinNation	= gpch->YL_JoinNation;
	//__________________________________________ // 010904 LTS*/
	
	Hero = g_CharacterList.lpFirst;
	
	lpNew->eventdelay = 0;	// Event甫 楷加利栏肺 努腐且荐 绝促. 
	
	// 010515 KHS
	lpNew->eventnpc = gpch->eventnpc;
	
	lpNew->aStepInfo[0] = gpch->nClsStep;
	lpNew->aStepInfo[1] = gpch->nDualCls;
	
	// 021008 YGI
#ifndef LIGHT_VERSION_
	g_HorseRider.LoadHorseData(lpNew);							// LTS HORSERIDER
#endif
	
	// 辨靛俊 措茄 沥焊啊 官诧霸 乐绰瘤俊 措茄 夸没 010719 YGI
	//	if( lpNew->GetGuildCode() ) 
	//		SendReqGuildInfo1( lpNew->GetGuildCode() );
	return	TRUE;
}		

void	
DeleteCharacter( LPCHARACTERLIST lpCharacterList, LPCHARACTER ch )
{		
	CheckAndSetPartyOnMySight( ch, 0 );		// 0803 YGI
	
	int sprno = ch->sprno;
	int ac0 = ch->accessory[0];
	int ac1 = ch->accessory[1];
	int ac2 = ch->accessory[2];
	int ac3 = ch->accessory[3];
	
	if( g_GameInfo.lpcharacter == ch )			//泅犁 急琶等 措惑眉 器牢磐啊 瘤况廉具窍绰 措惑捞扼搁 
	{	
		g_GameInfo.lpvSelectedSprite = NULL;
		g_GameInfo.nSelectedSpriteType = 0;
		g_GameInfo.lpcharacter = NULL;
	}	
	
	if ( ch != NULL )
	{	
		// 傍拜窍妨绊 lpAttacked俊 函荐甫 历厘沁绰单 Delete登搁 俊矾唱摆磊...
		LPCHARACTER check = Hero;
		while( check != NULL )
		{
			if( check->lpAttacked == ch )	check->lpAttacked = NULL;
			check = check->lpNext;
		}
		
		// 010724-1 KHS
		TileMap[ ch->position.x][ ch->position.y].occupied = 0;
		
		DeleteOwnerPoint( ch );
		
		if ( ch->lpPrev != NULL )
		{
			ch->lpPrev->lpNext = ch->lpNext;
		}
		if ( ch->lpNext != NULL )
		{
			ch->lpNext->lpPrev = ch->lpPrev;
		}
		
		if ( lpCharacterList->lpLast == ch )
		{
			lpCharacterList->lpLast = ch->lpPrev;
		}
		lpCharacterList->nCount--;
		
		MemFree( ch );
		ch = NULL;
		
		if( sprno > 1 )
		{
			
		}
		else 
		{
			FreeHeroClothAccessoryDataOne( sprno, ac0 );
			FreeHeroClothAccessoryDataOne( sprno, ac1 );
			FreeHeroClothAccessoryDataOne( sprno, ac2 );
			FreeHeroClothAccessoryDataOne( sprno, ac3 );
		}
	}	
}		




LPCHARACTER FindCharacter( LPCHARACTERLIST lpCharacterList, DWORD dwID )
{		
	LPCHARACTER		ch = g_CharacterList.lpFirst;
	
	while( ch != NULL )
	{	
		if( (DWORD )ch->id == dwID )
		{
			return	ch;
		}
		
		ch = ch->lpNext;
	}		
	
	return	NULL;
}		

///////////////////////////////////////////////////////////////////////////////
//	

void 
SetCharacterPosition( LPCHARACTER ch, POINT position )
{	
	ch->oldposition = ch->position = position;
}	

LPPOINT 
GetCharacterPosition( LPCHARACTER ch )
{	
	return	&ch->position;
}	



#define PAI   3.141592


BOOL ChangeDirection( DIRECTION* direction, int sx, int sy, int dx, int dy )
//int ReturnDirection( float tx1, float ty1, float tx2, float ty2 )
{	
	double t;
	
	if( dy == sy && sx == dx ) return 0;
	
	t = atan2( (float)(dy - sy) , (float)(dx - sx) );
	if(    PAI*7/8 < t && t <=  PAI + 1   )	{*direction = DIRECTION_LEFT;		return 1; }
	if(   -PAI -1    <=t && t <= -PAI*7/8 )	{*direction = DIRECTION_LEFT;		return 1; }
	
	if(   -PAI*7/8 < t && t <= -PAI*5/8 )	{*direction = DIRECTION_LEFTUP;		return 1; }
	if(   -PAI*5/8 < t && t <= -PAI*3/8 )	{*direction = DIRECTION_UP;			return 1; }
	if(   -PAI*3/8 < t && t <= -PAI  /8 )	{*direction = DIRECTION_RIGHTUP;	return 1; }
	if(   -PAI  /8 < t && t <=  PAI  /8 )	{*direction = DIRECTION_RIGHT;		return 1; }
	if(    PAI  /8 < t && t <=  PAI*3/8 )	{*direction = DIRECTION_RIGHTDOWN; 	return 1; }
	if(    PAI*3/8 < t && t <=  PAI*5/8 )	{*direction = DIRECTION_DOWN; 		return 1; }
	if(    PAI*5/8 < t && t <=  PAI*7/8 )	{*direction = DIRECTION_LEFTDOWN;	return 1; }
	
	return 0;
}	


BOOL ChangeDirection16( DIRECTION* direction, int sx, int sy, int dx, int dy )
//int ReturnDirection( float tx1, float ty1, float tx2, float ty2 )
{	
	double t;
	
	if( dy == sy && sx == dx ) return 0;
	
	t = atan2( (float)(dy - sy) , (float)(dx - sx) );
	if(    PAI*15/16 < t && t <=  PAI+1		)		{*direction = DIRECTION_LEFT;		return 1; }
	if(   -PAI -1	 <=t && t <= -PAI*15/16 )		{*direction = DIRECTION_LEFT;		return 1; }
	
	if(   -PAI*15/16 < t && t <= -PAI*13/16 )	{*direction = DIRECTION_LEFTLEFTUP;		return 1; }   
	if(   -PAI*13/16 < t && t <= -PAI*11/16 )	{*direction = DIRECTION_LEFTUP;			return 1; }
	if(   -PAI*11/16 < t && t <= -PAI* 9/16 )   {*direction = DIRECTION_LEFTUPUP;		return 1; }
	if(   -PAI* 9/16 < t && t <= -PAI* 7/16 )	{*direction = DIRECTION_UP;				return 1; }
	if(	  -PAI* 7/16 < t && t <= -PAI* 5/16 )	{*direction = DIRECTION_RIGHTUPUP;		return 1; }
	if(   -PAI* 5/16 < t && t <= -PAI* 3/16 )	{*direction = DIRECTION_RIGHTUP;		return 1; }
	if(   -PAI* 3/16 < t && t <= -PAI* 1/16 )	{*direction = DIRECTION_RIGHTRIGHTUP;	return 1; }
	if(   -PAI* 1/16 < t && t <=  PAI* 1/16 )	{*direction = DIRECTION_RIGHT;			return 1; }
	if(    PAI* 1/16 < t && t <=  PAI* 3/16 )	{*direction = DIRECTION_RIGHTRIGHTDOWN;	return 1; }
	if(    PAI* 3/16 < t && t <=  PAI* 5/16 )	{*direction = DIRECTION_RIGHTDOWN;		return 1; }
	if(    PAI* 5/16 < t && t <=  PAI* 7/16 )	{*direction = DIRECTION_RIGHTDOWNDOWN;	return 1; }
	if(    PAI* 7/16 < t && t <=  PAI* 9/16 )	{*direction = DIRECTION_DOWN; 			return 1; }
	if(    PAI* 9/16 < t && t <=  PAI*11/16 )	{*direction = DIRECTION_LEFTDOWNDOWN; 	return 1; }
	if(    PAI*11/16 < t && t <=  PAI*13/16 )	{*direction = DIRECTION_LEFTDOWN;		return 1; }
	if(    PAI*13/16 < t && t <=  PAI*15/16 )	{*direction = DIRECTION_LEFTLEFTDOWN;	return 1; }
	
	return 0;
}	



///////////////////////////////////////////////////////////////////////////////
//		


/*	ACTION_NONE = 0,				//扁夯悼累
ACTION_BASIC_WALK,				//扁夯叭扁
ACTION_BATTLE_WALK_OWNHAND,		//傈捧叭扁
ACTION_BATTLE_NONE,				//傈捧扁夯
ACTION_ATTACK_ONESWORD1,		//茄颊傍拜
ACTION_ATTACK_ONESWORD2,		//茄颊傍拜2
ACTION_ATTACK_BONG_DOWN,		//豪, 帮豪, 傍拜( 郴妨摹扁 )
ACTION_ATTACK_BONG_JJIRKI,		//芒, 豪, 傍拜( 骂福扁 )
ACTION_ATTACK_BOW,				//劝筋扁
ACTION_ATTACK_PENSWORD2,		//奇教磊技
ACTION_ATTACK_PENSING1,			//奇教傍拜 1
ACTION_ATTACK_PENSING2,			//奇教傍拜 2
ACTION_RUN,						//顿扁
ACTION_ATTACKED,				//嘎扁
ACTION_DIE,						//磷扁
ACTION_MAGIC_CASTING,			//付过某胶泼
ACTION_MAGIC_BEFORE,			//付过矫傈
ACTION_SITDOWN,					//臼扁
ACTION_THROW,					//带瘤扁傍拜
ACTION_HORSE,					//富鸥扁
ACTION_BATTLE_WALK_TWOHAND,		//傈捧叭扁 2 ( 剧颊俊 八 )
ACTION_BATTLE_NONE_TWOHAND,		//傈捧扁夯 2( 剧颊 )
ACTION_ATTACK_BONG_ROUND		//豪, 芒, 帮豪, 傍拜( 戎滴福扁 )
*/		




void onLineDistantPosition( int x1, int y1, int x2,int y2, int *xx,int *yy, int dist)
{		
	float decline;
	
	decline = (float)atan2(y2-y1,x2-x1);
	*xx = (int)((float )dist*(float)cos(decline));
	*yy = (int)((float )dist*(float)sin(decline));
	
	return;
}		

//	第肺 拱矾辑绰巴捞唱 轿框狼 楷免俊 包茄 巴阑 免仿茄促. 	
//////////////////////// 0604 捞痹绕 荐沥 ///////////////////////////
/*----------------------------------------------------------------------------------------------------------------------------------
扁瓷 : 
牢磊 : 
搬苞 :	府畔蔼 : -1 BackDraw啊 瘤陛 阜 场车促. 
0 Backdraq啊 鞘夸绝促. ( 弊成 府畔 )
1 Backdraw 吝捞促. 
//----------------------------------------------------------------------------------------------------------------------------------*/

///////////////////// 0607 lkh 荐沥 ////////////////////////////
///////////////////// 0613 lkh 荐沥 ////////////////////////////
int BackDrawSub( LPCHARACTER ch )
{
	int xx, yy;
	float distance;
	int tx, ty; 
	
	if( ch->toflag == false ) return 0;
	if( ch->AnimationTable[ch->nCurrentAction].cFrame[ch->nCurrentFrame] < 4 ) return 0;		//4橇饭烙 捞惑 柳青等 版快父 剐府霸
	
	//	if( !IsDead( ch ) ) break;
	
	distance = (float)sqrt( pow( ch->x-ch->tox, 2 ) + pow( ch->y-ch->toy, 2 ) );
	distance = distance*2.f/3.f;
	
	if( distance < 8 )			//肯傈洒 促 剐赴 版快
	{
		if( !IsDead(ch) && (ch->pathcount > 2 || ch->moveFlag == TRUE) ) // 捞悼吝捞菌栏搁 
		{
			//			ch->pathcount = 0;
			//			ch->moveFlag = 0;
		}
		
		if(ch->tox || ch->toy)
		{
			ch->x = ch->tox;		//弥辆 剐赴 谅钎肺 悸泼
			ch->y = ch->toy;
			ch->tox = 0;
			ch->toy = 0;
		}
		
		Moveoldx = 0;
		Moveoldy = 0;
		Movetime = 0;
		
		ch->toflag = false;
		
		return -1;
	}
	
	onLineDistantPosition( ch->x, ch->y, ch->tox, ch->toy, &xx, &yy, (int)distance );
	
	tx = (ch->x + xx)/TILE_SIZE;
	ty = (ch->y + yy)/TILE_SIZE;
	
	//------------------------0606 --lkh -------------------
	if( FreeTile( ch, tx, ty, tx, ty ) )			//角力 哎荐 乐绰 瘤痢牢啊? *** 困摹蔼阑 罐酒 贸府窍绰巴栏肺 官槽促.
	{
		ch->x += xx;
		ch->y += yy;
		return  1;
	}
	else 
	{
		ch->tox = 0;
		ch->toy = 0;
		ch->toflag = false;
		
		/////////////////// 0724 yjs 眠啊(悼扁嘎眠扁) ////////////////////
		SendModifyPosition(ch);
	}
	
	return 0;
}	



void AnimationTypeTestCheck( LPCHARACTER ch )
{	
	int xx, yy;	
	int x =  ch->x;
	int y =  ch->y;
	int tx, ty;	
	int dist;
	
	dist = ch->AnimationTable[ ch->nCurrentAction].dx[ ch->nCurrentFrame];
	if( dist == 0 ) return ;
	
	tx = x;
	ty = y;
	
	switch( ch->direction)
	{
	case 0 :  ty = y +100;					break;
	case 1 :  ty = y +100;	tx = x -100;	break;
	case 2 :               	tx = x -100;	break;
	case 3 :  ty = y -100;	tx = x -100;	break;
	case 4 :  ty = y -100;	             	break;
	case 5 :  ty = y -100;	tx = x +100;	break;
	case 6 :            	tx = x +100;	break;
	case 7 :  ty = y +100;	tx = x +100;	break;
	}
	
	ch->gox = tx;
	ch->goy = ty;
	
	onLineDistantPosition( x, y, tx, ty, &xx, &yy, dist );
	//	ChangeDirection( &ch->direction, x,y, tx, ty );
	
	x += xx;	
	y += yy;	
	
	ch->x = x;
	ch->y = y;
	
	
	
}	
void AnimationTypeTestFrameCount( LPCHARACTER ch )
{				
	if( ch->ChatDelay > 0 ) ch->ChatDelay--;
	
	ch->nCurrentFrame++;
	ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	
	if ( ch->nCurrentFrame >= ch->AnimationTable[ ch->nCurrentAction ].nCount )
	{
		ch->nCurrentAction = ch->basicAction;
		ch->nCurrentFrame =0;
		ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
		ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	
		EndOfAnimationFlag = 3;
	}		
	
	//////////////////////////// SoundUp lkh 眠啊(力累葛靛俊辑 家府 抛胶飘 困秦) //////////////////////
	int wave = ReturnWaveAnimation( ch );
	if( wave )
	{
		if (ch==Hero) PlayListAutoSounds(wave, 0, 0, 0);		//3D sound肺 钎泅	// LTS SOUND
		else
		{
			int distx=ch->x-Hero->x;
			int disty=ch->y-Hero->y;
			PlayListAutoSounds(wave, distx, disty, 0);
		}
	}
}	



void AnimationTypeWomanCheck( LPCHARACTER ch )
{			
	int dist,halfdist;
	int wave;
	int pathcount = ch->pathcount; 
	int target_X=0;		//1226
	int target_Y=0;		//1226
	
	BackDrawSub( ch );
	
	switch ( ch->nCurrentAction )
	{					
	case ACTION_BATTLE_NONE_TWOHAND	://傈捧扁夯 2( 剧颊 )
	case ACTION_BATTLE_NONE			://傈捧扁夯
	case ACTION_NONE				://扁夯悼累
		{						
			_asm nop;
			break;			
		}					
		
	case ACTION_BATTLE_WALK_TWOHAND	://傈捧叭扁 2 ( 剧颊俊 八 )
	case ACTION_BATTLE_WALK_OWNHAND	://傈捧叭扁	
	case ACTION_BASIC_WALK			://扁夯叭扁		
	case ACTION_RUN					://顿扁			
		{
			//			CharacterWorkingWater( ch);
			
			int todir	= (int)ch->todir;
			int dir		= (int)ch->direction;
			
			if( todir - dir > 0)
			{
				if( abs(todir - dir) <= 3 ){dir ++; dir %= 8; }
				else { dir += 7; dir %= 8; }
				ch->direction = (DIRECTION)dir;
				break;
			}
			else if( todir - dir < 0 )
			{
				if( abs(dir - todir) <= 4 ){ dir += 7; dir %= 8; }
				else {dir ++; dir %= 8; }
				ch->direction = (DIRECTION)dir;
				break;
			}
			
			if ( pathcount > 2 || ch->moveFlag == TRUE )
			{
				ReturnCharMoveDistance( ch, &dist );
				halfdist = ((dist+1)>>1);
				
				
				{
					int xx, yy;	
					int x =  ch->x;
					int y =  ch->y;
					int tx, ty;	
					
					if( dist == 0 ) break;
					
					if(  pathcount > 2 ) 
					{
						tx = (ch->path[ pathcount - 2 ][ 0 ] << 5) + 16;
						ty = (ch->path[ pathcount - 2 ][ 1 ] << 5) + 16;
					}
					else		
					{
						tx = ch->gox;
						ty = ch->goy;
					}		
					
					onLineDistantPosition( x, y, tx, ty, &xx, &yy, dist ); 
					ChangeDirection( &ch->todir, x,y, tx, ty ); 
					
					x += xx;
					y += yy;
					
					ch->x = x;
					ch->y = y;
					
					ch->oldposition = ch->position;
					ch->position.x = x / TILE_SIZE;
					ch->position.y = y / TILE_SIZE;
					
					// 笼俊 甸绢艾绰瘤 咯何甫 Check茄促. 
					CheckEnterHouse( ch );
					
					if( BoxAndDotCrash( tx-halfdist, ty-halfdist, dist+1,dist+1, x, y ))
					{		
						if( tx == ch->gox && ty == ch->goy )
						{
							ch->x = tx;
							ch->y = ty;
						}
						
						if ( pathcount <= 2 && ch->moveFlag == TRUE )
						{		
							ch->moveFlag = FALSE;
							
							UnDisplayDestPoing(ch);
						}		 
						else	
						{		
							ch->pathcount--;
						}		
						
						//---------------------------------------------------------------------------------------
						//---------------------------------------------------------------------------------------
					}		
				}			
			}		
			else	
			{		
				ch->nCurrentAction = ch->basicAction;
				ch->nCurrentFrame = 0;
				ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
				ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	
				
				// 捞悼阑 肯丰沁阑锭 努扼捞攫飘狼 困摹甫 辑滚肺 舅妨林绰 贸府 //
				SendModifyPosition( ch );
			}		
			
			break;	
		}			
		
		
	case ACTION_ATTACKED			://嘎扁
		{				
			if (ch->nCurrentAction==17) ch->nCurrentAction=0;		// LTS SITDOWN
			break;
		}		
	case ACTION_ATTACK_BOW: // 劝筋扁
	case ACTION_THROW: //带瘤扁傍拜
		{
			if (ch->condition&CON_STONE)
			{ //< CSD-021021
				break;
			} //> CSD-021021
			
			if (ch == Hero && ch->nCurrentFrame == 0)
			{
				ch->shoot_PointX = Mox;
				ch->shoot_PointY = Moy;
			}
			
			const int attackf = ch->AnimationTable[ch->nCurrentAction].attackf[ch->nCurrentFrame];
			//< CSD-020814
			if (attackf && ch->end_Casting == 0)
			{		
				ch->changeActionFlag = 1;
				
				LPCHARACTER	pTarget = (LPCHARACTER)(ch->lpAttacked);
				
				if (pTarget != NULL)
				{
					if (!ch->bShoot)
					{
						GetCombat()->Effect(ch, pTarget);
					}
					else
					{
						GetStrike()->Effect(ch, pTarget);
					}
					
					ch->shoot_PointX = 0;
					ch->shoot_PointY = 0;
				}
				else// 030311 kyo
				{//鸥百捞 绝促搁 悼累阑 肛冕促. 
					ch->nCurrentAction = ACTION_NONE; 
					ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为

				}
			}	
			//> CSD-020814
			break;
		}
	case ACTION_DIE					://磷扁
		{		
			ch->changeActionFlag = 1;
			
			// 磷阑 锭 家府啊 抄促. 
			if( ch->nCurrentFrame == 1 )
			{
				if( ch->sprno == 0  )
				{
					if (ch==Hero) PlayListAutoSounds( 814, 0, 0, 0 );	// LTS SOUND
					else
					{
						int distx=ch->x-Hero->x;
						int disty=ch->y-Hero->y;
						PlayListAutoSounds( 814, distx, disty, 0 );
					}
				}
				else
				{
					if (ch==Hero) PlayListAutoSounds( 834, 0, 0, 0 );	// LTS SOUND
					else
					{
						int distx=ch->x-Hero->x;
						int disty=ch->y-Hero->y;
						PlayListAutoSounds( 834, distx, disty, 0 );
					}
				}
			}		
			
			
			break;
		}		
		
		// 0720 lkh 荐沥 
	case ACTION_HORSE				://富鸥扁
		{		
			ch->changeActionFlag = 1;
			break;
		}		
		
	case ACTION_ATTACK_ONESWORD1	://茄颊傍拜
	case ACTION_ATTACK_ONESWORD2	://茄颊傍拜2
	case ACTION_ATTACK_BONG_DOWN	://豪, 帮豪, 傍拜( 郴妨摹扁 )
	case ACTION_ATTACK_BONG_JJIRKI	://芒, 豪, 傍拜( 骂福扁 )			
	case ACTION_ATTACK_PENSWORD2	://奇教磊技
	case ACTION_ATTACK_PENSING1		://奇教傍拜 1
	case ACTION_ATTACK_PENSING2		://奇教傍拜 2
	case ACTION_MAGIC_CASTING		://付过某胶泼
	case ACTION_MAGIC_BEFORE		://付过矫傈
	case ACTION_SITDOWN				://臼扁
	case ACTION_ATTACK_BONG_ROUND	://豪, 芒, 帮豪, 傍拜( 戎滴福扁 )
		{	
			if (ch->condition&CON_STONE)
			{ //< CSD-021021
				break;
			} //> CSD-021021
			
			ch->changeActionFlag = 1;
			
			if( SysInfo.notconectserver )
			{
				LPCHARACTER tchar = (LPCHARACTER)ch->lpAttacked;
				if( tchar != NULL ) 
				{
					tchar->NumberAnimationValue = rand()%50 + 30;
					
					tchar->attackedresult		= 1;// Hit and Dead....
					tchar->attackedresult		= 2;// Hit and Not Dead
				}
			}
			
			GetAttacked( ch );
			//CalcDirection( ch );
			break;
		}		
	case ACTION_VICTORY :		// LTS ACTION		// 荤款靛 棺 扁鸥 瓤苞甫 困茄 风凭捞 甸绢哎镑
		break;
	case ACTION_ANGER :
		break;
		
		
  } // switch..
  
  ////////////////////////////// 0620 lkh 眠啊 //////////////////////////////////
  ////////////////////////////// 0620_ lkh 眠啊 //////////////////////////////////
  
  
  TileMap[ ch->oldposition.x][ch->oldposition.y].occupied = 0;
  if (ch->hp>0) TileMap[ ch->position.x][ch->position.y].occupied = 1;	// 010915 LTS
  
  
  
  wave = ReturnWaveAnimation( ch );
  
  
  if(wave>0)
  {
	  if( TileMap[ ch->x/TILE_SIZE][ch->y/TILE_SIZE].attr_river )
	  {
		  if(	ch->nCurrentAction == ACTION_BATTLE_WALK_TWOHAND	||
			  ch->nCurrentAction == ACTION_BATTLE_WALK_OWNHAND	||
			  ch->nCurrentAction == ACTION_BASIC_WALK				)
			  switch( ch->sprno )
		  {
			case 0 :case 1 :	wave = (rand()%2)?1144:1145;
				break;
		  }
		  else if( ch->nCurrentAction == ACTION_RUN	)
			  switch( ch->sprno )
		  {
			case 0 :case 1 :	wave = 1146;
				break;
		  }
	  }
	  
	  if (!(ch->condition == CON_DEATH || IsDead(ch)) && g_mgrBattle.IsSound(ch))	
	  { //< CSD-020707
		  if (ch == Hero)
		  {
			  PlayListAutoSounds( wave, 0, 0, 0 );
		  }
		  else
		  {
			  int distance_X = ch->x-Hero->x;		// LTS SOUND
			  int distance_Y = ch->y-Hero->y;		// LTS SOUND
			  PlayListAutoSounds( wave, distance_X, distance_Y, 0 );	//3D sound肺 钎泅	// LTS SOUND
		  }	
	  }	//> CSD-020707
  }			
}			



void AnimationTypeWomanFrameCount( LPCHARACTER ch )
{			
	if( ch->ChatDelay > 0 ) ch->ChatDelay--;
	
	
	if( ch->canview )		ch->canview --;
	if( ch->canviewitem )	ch->canviewitem --;
	
	if( CanViewBomb(ch) )  //020320 lsw
	{
		if( !(rand()%10))InsertMagic( Hero, Hero, 213, 0, Hero->x - TILE_SIZE * 5 +(rand()%(TILE_SIZE*10)),  Hero->x - TILE_SIZE * 5  +(rand()%(TILE_SIZE*10)), 0,0 );
	}
	if( CanViewTrap(ch) )  //020320 lsw
	{
		if( !(rand()%10))	InsertMagic( Hero, Hero, 213, 0,Hero->x - TILE_SIZE * 5 +(rand()%(TILE_SIZE*10)),  Hero->x - TILE_SIZE * 5  +(rand()%(TILE_SIZE*10)), 0,0  );
	}
	
	if( ch->attackanimationON ) 
	{
		//< CSD-020704
		if (ch->condition&CON_FROZEN)
		{
			static int nTemp;
			ch->attackanimationON -= (++nTemp%2) ? 0:1;
		}
		else
		{
			ch->attackanimationON--;
		}
		//> CSD-020704
		ch->pluse_value++;
	}
	
	
	if( ch->smiledelay > 0 )
	{
		ch->smiledelay--;
	}
	
	
	if( ch->feeldelay > 0 )
	{
		ch->feeldelay --;
		if( ch->feeldelay == 0 )
		{
			switch( ch->feeltype )
			{
			case FT_BAD_ : AddCurrentStatusMessage( 255,0,0, lan->OutputMessage(5,1) ); break;//010215lsw 磷绢辑 扁盒 唱晦聪促.
			}
		}
	}
	
	if( ch->viewtype == VIEWTYPE_GHOST_ )
	{
		if( ch->ghostanito ){	ch->ghostani ++; if( ch->ghostani > 30 ) ch->ghostanito = !ch->ghostanito; }
		else {	ch->ghostani--; if( ch->ghostani < 15 ) ch->ghostanito = !ch->ghostanito; }
	}
	
	if( ch->eventdelay ) ch->eventdelay--;
	
	if( ch->dieani	> 0 )
	{
		ch->diedelay --;
		if( ch->diedelay <=0 )
		{ 
			ch->diedelay = 3;
			ch->dieani--;
			if(ch->dieani==32)		//server俊辑 磷篮 仇阑 瘤况滚府扼绊 且锭 荤扼瘤霸 窍扁 困秦 
				ch->dieani=33;
		}
	}
	// 酒贰 惑怕俊辑绰 橇饭烙 沥瘤
	if (ch->condition&CON_STONE)
	{
		
	}
	else 
	{
		ch->nCurrentFrame++;
		ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	

	}
	
	switch ( ch->nCurrentAction )
	{			
    case ACTION_BATTLE_NONE_TWOHAND	://傈捧扁夯 2( 剧颊 )
    case ACTION_BATTLE_NONE			://傈捧扁夯
	case ACTION_NONE				://扁夯悼累
		{		
			ch->changeActionFlag = 0;
			if (ch->HorseNo)				// LTS HORSERIDER
			{
				g_HorseRider.CheckMoveAnimation(ch);
			}
			else
			{
				if ( ch->nCurrentFrame >= ch->AnimationTable[ ch->nCurrentAction ].nCount )
				{
					if( !IsDead(ch) && (ch->pathcount > 2 || ch->moveFlag == TRUE) )
					{
						CharDoAni_Moving(ch, ch->movetype );
					}
					else
					{
						ch->nCurrentFrame =0;
						ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值		

					}
				}
			}
			break;
		}		
		
	case ACTION_DIE					://磷扁
		{
			ch->changeActionFlag = 1;
			
			if (ch->HorseNo)					// LTS HORSERIDER
			{
				g_HorseRider.CheckDeathAnimation(ch);
			}
			else
			{
				if ( ch->nCurrentFrame >= ch->AnimationTable[ ch->nCurrentAction].nCount )
				{	
					ch->nCurrentFrame = ch->AnimationTable[ ch->nCurrentAction].nCount-1;
					ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	

					
#ifdef _DEBUG
					if( SysInfo.notconectserver )
					{
						if( ch->dieani == 0 ) // 磷菌栏搁..
						{
							ch->diedelay	=  3;	// 荤扼瘤绰悼累阑 玫玫洒...
							ch->dieani		= 80;	// 32焊促 累栏搁  荤扼柳促.
							ch->dieanitype  =  0;	// 
						}
					}
					else	//辑滚客 楷搬等 版快
#endif
					{
						if( ch->blood_Set == false && ch->dieani == 0 )
						{
							ch->diedelay	= 3;
							ch->dieani		= 80;
							ch->blood_Set	= true;
						}
					}
				}
			}
			break;
		}
		
	case ACTION_MAGIC_BEFORE		://付过矫傈			
	case ACTION_RUN					://顿扁
	case ACTION_BATTLE_WALK_OWNHAND	://傈捧叭扁
	case ACTION_BATTLE_WALK_TWOHAND	://傈捧叭扁 2 ( 剧颊俊 八 )
	case ACTION_BASIC_WALK			://扁夯叭扁
		{		
			//		PlaySoundWalkingWater( ch, ch->nCurrentAction );
			ch->changeActionFlag = 0;
			if (ch->HorseNo)										// LTS HORSERIDER
			{
				g_HorseRider.CheckAnimation(ch);
			}
			else
			{
				if ( ch->nCurrentFrame >= ch->AnimationTable[ ch->nCurrentAction].nCount )
				{	
					ch->nCurrentFrame = 0;
					ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	

				}
			}	
			break;
		}		
		
	case ACTION_ATTACK_ONESWORD1	://茄颊傍拜
	case ACTION_ATTACK_ONESWORD2	://茄颊傍拜2
	case ACTION_ATTACK_BONG_DOWN	://豪, 帮豪, 傍拜( 郴妨摹扁 )
	case ACTION_ATTACK_BONG_JJIRKI	://芒, 豪, 傍拜( 骂福扁 )
	case ACTION_ATTACK_BOW			://劝筋扁	
	case ACTION_ATTACK_PENSWORD2	://奇教磊技
	case ACTION_ATTACK_PENSING1		://奇教傍拜 1
	case ACTION_ATTACK_PENSING2		://奇教傍拜 2
	case ACTION_ATTACKED			://嘎扁
		{	
			if( ch->toflag == true )
			{
				if ( ch->nCurrentFrame >= 3 ) ch->nCurrentFrame = 3;
			}
		}	
	case ACTION_THROW				://带瘤扁傍拜
	case ACTION_HORSE				://富鸥扁
	case ACTION_ATTACK_BONG_ROUND	://豪, 芒, 帮豪, 傍拜( 戎滴福扁 )
		{		
			if (ch->HorseNo)				// LTS HORSERIDER
			{
				g_HorseRider.CheckAttackAnimation(ch);
			}
			else
			{
				if ( ch->nCurrentFrame >= ch->AnimationTable[ ch->nCurrentAction ].nCount )
				{	
					// 捞悼吝捞菌栏搁...
					if( !IsDead(ch) && (ch->pathcount > 2 || ch->moveFlag == TRUE) )
					{
						CharDoAni_Moving(ch, ch->movetype );
					}
					else
					{
						ch->changeActionFlag	= 0;
						ch->nCurrentAction		= ch->basicAction;
						ch->nCurrentFrame		= 0;
						ch->offset.x = 0; ch->offset.y = 0;
						ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
						ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	
						
						if (ch->nActiveCombat == SHARK_MISSILE)
						{
							ch->attackanimationON = 0;
						}
					}
				}
			}
			break;
		}		
	case ACTION_MAGIC_CASTING		://付过某胶泼
		{
			if (ch->HorseNo)								// LTS HORSERIDER
			{
				g_HorseRider.CheckDeathAnimation(ch);
			}
			else
			{
				if ( ch->nCurrentFrame >= ch->AnimationTable[ ch->nCurrentAction ].nCount )
				{
					ch->nCurrentFrame = ch->AnimationTable[ ch->nCurrentAction ].nCount-1;
					ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	

				}
			}
			break;
		}
	case ACTION_SITDOWN				://臼扁				// LTS SITDOWN	// 旧篮 橇饭烙 靛肺雷 何盒
		{
			if ( ch->nCurrentFrame >= ch->AnimationTable[ ch->nCurrentAction ].nCount )
			{
				ch->nCurrentFrame = ch->AnimationTable[ ch->nCurrentAction ].nCount-1;
				ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	

			}
			break;
		}
	case ACTION_VICTORY				:	// LTS ACTION		// 扁坏窍扁
	case ACTION_ANGER					:	// LTS ACTION		// 拳郴扁
		{
			if (ch->nCurrentFrame>=ch->AnimationTable[ ch->nCurrentAction ].nCount)	//悼累捞 辆丰登菌促搁..
			{
				if (ch->nCurrentAction==ACTION_ANGER) g_ActionAnger=0;
				ch->nCurrentAction=ch->basicAction;
				ch->nCurrentFrame=0;
				ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
				ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	

			}
			break;
		}
		
  } // switch..
  
  if( ch->willdie )
  {
	  ch->willdie--;
	  if( ch->willdie <= 1 ) 
	  {
		  ch->willdie = 0;
		  ch->hp = 0;
		  ch->nCurrentAction	= ACTION_DIE;
		  ch->nCurrentFrame	= 0;
		  ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为	
		  ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值		

	  }
  }
}			

/*	MON1_NONE = 0,		// 扁夯悼累
MON1_BATTLE_NONE,	// 傈捧扁夯
MON1_ATTACKED,		// 嘎扁
MON1_DIE,			// 磷扁....
MON1_BATTLE_WALK_,	// 扁夯叭扁
MON1_BATTLE_ATTACK, // 茄颊傍拜
*/		

// 010724 KHS						
void AnimationType_Mon1_Check( LPCHARACTER ch )
{				
	int dist, halfdist;
	int wave;	
	int pathcount = ch->pathcount;
	
	if( ch->just_ani == true )
	{
		goto JUST_ANI_NEXT__;
	}
				
	switch ( ch->nCurrentAction )
	{			
	case MON1_NONE			://扁夯悼累
		{			
			break;	
		}			
	case MON1_BATTLE_WALK	://傈捧叭扁 2 ( 剧颊俊 八 )
		{			
			if ( pathcount > 2 || ch->moveFlag == TRUE )
			{		
				ReturnCharMoveDistance( ch, &dist );
				halfdist = ((dist+1)>>1);
				
				//if( ch->pathcount > 1 )
				{				
					int xx, yy;	
					int x =  ch->x;
					int y =  ch->y;
					int tx, ty;		
					
					
					if( dist == 0 ) break;
					
					if(  pathcount > 2 ) 
					{			
						tx = (ch->path[ pathcount - 2 ][ 0 ] << 5) + 16;
						ty = (ch->path[ pathcount - 2 ][ 1 ] << 5) + 16;
					}			
					else			
					{			
						tx = ch->gox;
						ty = ch->goy;
					}			
					
					onLineDistantPosition( x, y, tx, ty, &xx, &yy, dist );
					ChangeDirection( &ch->todir, x,y, tx, ty );
					
					x += xx;	
					y += yy;	
					
					// 面悼窍搁 力磊府俊 辑乐绰促. 
					int oldx, oldy;
					oldx = ch->x;
					oldy = ch->y;
					ch->x = x;	
					ch->y = y;	
					ch->oldposition = ch->position;
					ch->position.x = x / TILE_SIZE;
					ch->position.y = y / TILE_SIZE;
					
					
					if( BoxAndDotCrash( tx-halfdist, ty-halfdist, dist+1,dist+1, x, y ))
					{			
						
						ch->x = tx;
						ch->y = ty;
						
						if ( pathcount <= 2 && ch->moveFlag == TRUE )
						{		
							ch->moveFlag = FALSE;
						}		
						else	
						{		
							ch->pathcount--;
						}		
					}		
				}			
			}		
			else	
			{	
				LPCHARACTER t;
				t = ReturnCharacterPoint( ch->movetargetid );
				if( t==NULL )	return;
				if( t != NULL )
				{
					DIRECTION dir;
					if( ChangeDirection( &dir, ch->x, ch->y, t->x, t->y ) )
					{
						ch->todir = dir;
					}
				}
				
			}		
			
			break;	
		}			
	case MON1_ATTACKED			://嘎扁	
		{
			BackDrawSub( ch );
			break;
		}
	case MON1_DIE:           break;
	case MON1_BATTLE_MAGIC:  break;
	case MON1_BATTLE_ATTACK: // 茄颊傍拜
		{		
			ch->changeActionFlag = 1;
			
			// 0810 KHS-----------
			LPCHARACTER dch = ReturnCharacterPoint( ch->targetid );
			DIRECTION  d;
			if( dch )
			{	
				if( ChangeDirection( &d, ch->x, ch->y, dch->x, dch->y ) )
				{
					ch->direction = ch->todir = d;
				}
			}	
			//--------------------
			
			GetAttacked( ch );
			break;
		}		  
	case MON1_CASTING_MAGIC :		// 08付过矫傈			// LTS DRAGON
		{
			break;
		}
	case MON1_BATTLE_ATTACK2 :	// 09傍拜 2
		{
			break;
		}
	case MON1_BATTLE_ATTACK3 :	// 10傍拜 3
		{
			break;
		}
	case MON1_JUST_ACTION :		// 11烙狼狼 悼累 	
		{
			break;
		}
		
  } // switch..
  
JUST_ANI_NEXT__:
  
  
  TileMap[ ch->oldposition.x][ch->oldposition.y].occupied = 0;
  if (ch->hp>0) TileMap[ ch->position.x][ch->position.y].occupied = 1;		// 011105 LTS
  
  
  wave = ReturnWaveAnimation( ch );
  
  //int distance_X = abs(ch->x-Hero->x)/(TILE_SIZE/4);			// LTS SOUND
  //int distance_Y = abs(ch->y-Hero->y)/(TILE_SIZE/4);			// LTS SOUND
  
  int distance_X = ch->x-Hero->x;			// LTS SOUND
  int distance_Y = ch->y-Hero->y;			// LTS SOUND
  
  //if( ch->x < Hero->x)	distance_X = -distance_X;				// LTS SOUND
  //if( ch->y > Hero->y)	distance_Y = -distance_Y;				// LTS SOUND
  
  if( wave > 0 ) 
	  if( TileMap[ ch->x/TILE_SIZE][ch->y/TILE_SIZE].attr_river )
		  if(	ch->nCurrentAction == MON1_BATTLE_WALK )
			  switch( ch->sprno )
		  {
	case 45 : case 6 : wave = 1151;
		break;
		  }
		  
		  if (g_mgrBattle.IsSound(ch))
		  { //< CSD-020707
			  PlayListAutoSounds(wave,distance_X, distance_Y, 0);		//3D sound肺 钎泅	// LTS SOUND
		  } //> CSD-020707
		  
		  // Animation俊 瓤苞免仿 Data啊 乐促搁 ...
		  int effect = ReturnEffectAnimation( ch );
		  
		  if( effect > 0 && effect < MAX_EFFECT_COUNT )
		  {		
			  InsertMagic( ch, ch, effect, 4, 0,0,0,0,0 );
		  }		
}


void AnimationType_Mon1_FrameCount( LPCHARACTER ch )
{	
	int			redirection = 0;
	
	if( ch->ChatDelay > 0 )	ch->ChatDelay --;
	
	if( ch->dieani	> 0 )
	{
		ch->diedelay --;
		if( ch->diedelay <=0 )
		{ 
			ch->diedelay = 3;
			ch->dieani--;
			if(ch->dieani==32)		//server俊辑 磷篮 仇阑 瘤况滚府扼绊 且锭 荤扼瘤霸 窍扁 困秦 
				ch->dieani=33;
		}
	}
	// 酒贰 惑怕俊辑绰 橇饭烙 沥瘤
	if (ch->condition&CON_STONE)
	{	
		
	}	
	else 
	{	
		ch->nCurrentFrame++;
		ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值		
	}	
	
	if( ch->just_ani == true )
	{
		ch->changeActionFlag = 0;
		ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	
		if ( ch->nCurrentFrame >= ch->AnimationTable[ ch->nCurrentAction ].nCount )
		{
			ch->just_ani		= false;
			ch->nCurrentFrame	= 0;
			ch->nCurrentAction	= MON1_NONE;
			ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
			ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	
		}
		return;
	}
	
	
	switch ( ch->nCurrentAction )
	{ 
	case MON1_NONE			:	//扁夯悼累
		{		
			ch->changeActionFlag = 0;
			if ( ch->nCurrentFrame >= ch->AnimationTable[ ch->nCurrentAction ].nCount )
			{
				ch->nCurrentFrame =0;
				ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	
				
			}	
			
			break;
		}	
	case MON1_BATTLE_WALK	:	//傈捧叭扁 2 ( 剧颊俊 八 )
		{		
			ch->changeActionFlag = 0;
			
			if (ch->nCurrentFrame >= ch->AnimationTable[ch->nCurrentAction].nCount)
			{	//< CSD-030417	
				if (++ch->insertPattern == 5)
				{
					ch->insertPattern=0;
					
					if (rand()%2)
					{
						redirection = ((int)(ch->direction) + 7)%8;
					}
					else 
					{
						redirection=((int)(ch->direction) + 2)%8;
					}
					
					CharDoAni(ch, (DIRECTION)redirection, MON1_BATTLE_WALK);
				}
				
				ch->nCurrentFrame = 0;
			}	//> CSD-030417
			
			break;
		}			
	case MON1_DIE			:	// 磷扁....
		{		
			ch->changeActionFlag = 0;
			if ( ch->nCurrentFrame >= ch->AnimationTable[ ch->nCurrentAction].nCount )
			{	
				ch->nCurrentFrame = ch->AnimationTable[ ch->nCurrentAction].nCount-1;
				ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	

				if( SysInfo.notconectserver )
				{
					if( ch->dieani == 0 ) // 磷菌栏搁..
					{
						ch->diedelay	=  3;	// 荤扼瘤绰悼累阑 玫玫洒...
						ch->dieani		= 80;	// 32焊促 累栏搁  荤扼柳促.
						ch->dieanitype  =  0;	// 
					}
				}
				else 
				{
					if( ch->blood_Set == false )
					{
						ch->diedelay	= 3;
						ch->dieani		= 80;
						ch->blood_Set	= true;
					}
				}
			}	
			break;
		}
	case MON1_ATTACKED		:	//嘎扁	
		if( ch->toflag == true )
		{
			ch->nCurrentFrame = ch->AnimationTable[ ch->nCurrentAction ].nCount -1;
			break;
		}
	case MON1_CASTING_MAGIC :	// 08付过矫傈		// LTS DRAGON
	case MON1_BATTLE_ATTACK2 :	// 09傍拜 2			// LTS DRAGON
	case MON1_BATTLE_ATTACK3 :	// 10傍拜 3			// LTS DRAGON
	case MON1_JUST_ACTION :		// 11烙狼狼 悼累 	// LTS DRAGON
	case MON1_BATTLE_MAGIC:
	case MON1_BATTLE_ATTACK :	// 茄颊傍拜
		{	
			if ( ch->nCurrentFrame >= ch->AnimationTable[ ch->nCurrentAction ].nCount )
			{
				ch->changeActionFlag	= 0;
				ch->nCurrentAction		= ch->basicAction;
				ch->nCurrentFrame		= 0;
				ch->offset.x			= 0; 
				ch->offset.y			= 0;
				ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
				ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	
				
				break;
			}
			break;
		}		
		
		// 1224 KHS
	default : 
		{
			if ( ch->nCurrentFrame >= ch->AnimationTable[ ch->nCurrentAction ].nCount )
			{
				ch->changeActionFlag	= 0;
				ch->nCurrentAction		= ch->basicAction;
				ch->nCurrentFrame		= 0;
				ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
				ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	

			}
		}
	} // switch..
	
	if( ch->willdie )
	{
		ch->willdie--;
		if( ch->willdie <= 1 ) 
		{
			ch->willdie = 0;
			ch->hp = 0;
			ch->nCurrentAction = MON1_DIE;
			ch->nCurrentFrame = 0;

			if (ch->nRace == GUARDTOWER)
			{	//< CSD-040202
				ch->sealstoneani = 0;
			}	//> CSD-040202
		}
	}
}			

int CharDoAni_Moving( LPCHARACTER ch, int movetype )
{
	switch( ch->animationType )										
	{														
	case ANIMATION_TYPE_MON1 :	if ( ch->nCurrentAction != ch->basicWalk )
								{						
									ch->nCurrentAction = ch->basicWalk;
									ch->nCurrentFrame = 0;
									ch->set_nCurrentAction(ch->nCurrentAction); //??膠??鱗??槨
									ch->set_nCurrentFrame(ch->nCurrentFrame); //??膠????令

								}								
		break;												
		
	case ANIMATION_TYPE_WOMAN :							
	case ANIMATION_TYPE_MAN :   switch( movetype )
								{				
	case 0 :	if ( ch->nCurrentAction != ch->basicWalk )
				{	
					ch->nCurrentAction = ch->basicWalk;
					ch->nCurrentFrame = 0;
					ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
					ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	

				}	
		break;		
	case 1 :	if ( ch->nCurrentAction != ACTION_RUN )
				{	
					if( ch->nCurrentAction == ch->basicWalk )
					{
						ch->pathcount --;
					}
					ch->nCurrentAction = ACTION_RUN;
					ch->nCurrentFrame = 0;
					ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
					ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	

				}	
		break;		
								}				
	}											
	
	return 0;
}


extern void SendCMD_HERO_ACTION_CHANGED(LPCHARACTER ch);	// 011214 LTS

int CharDoAni( LPCHARACTER ch, DIRECTION dir, int anino )
{
	switch (ch->animationType)
	{			
	case ANIMATION_TYPE_MAN:
	case ANIMATION_TYPE_WOMAN:		
		{
			if (anino == ACTION_DIE)  goto NEXT_;	//  15 磷扁
			break;	
		}
	case ANIMATION_TYPE_MON1: 	
		{
			if (anino == MON1_DIE)  goto NEXT_;	//  15 磷扁
			break;	
		}
	}
	
	if (IsDead(ch))  return FALSE;
	if (ch->changeActionFlag == 1)	// Animation阑 肛苗乐绢具 窍绰 悼累嫡捞扼档 ( 抗甫 甸搁 矫傈悼累. )
		// 鸥拜阑 罐栏搁 ACTION_ATTACKED悼累阑 秦具 茄促. 
	{
		switch (ch->animationType)
		{
		case ANIMATION_TYPE_MAN:
		case ANIMATION_TYPE_WOMAN:		
			{
				if (ch->nCurrentAction == ACTION_ATTACKED)  ch->changeActionFlag = 0;
				break;	
			}
		case ANIMATION_TYPE_MON1: 	
			{
				if (ch->nCurrentAction == MON1_ATTACKED)  ch->changeActionFlag = 0;
				break;	
			}
		default:  return FALSE; 
		}
	}
	
NEXT_:
	if (ch == Hero)	
	{
		switch( ch->nCurrentAction )
		{
		case ACTION_BATTLE_WALK_TWOHAND	://傈捧叭扁 2 ( 剧颊俊 八 )
		case ACTION_BATTLE_WALK_OWNHAND	://傈捧叭扁	
		case ACTION_BASIC_WALK			://扁夯叭扁		
		case ACTION_RUN					://顿扁			
			{
				if (!IsDead(ch) && (ch->pathcount > 2 || ch->moveFlag == TRUE))
				{
				}
				else 
				{
					SendModifyPosition( Hero );
				}
				
				break;
			}
		}
	}
	
	if (ch == Hero && ch->nCurrentAction == ACTION_SITDOWN) 
	{
		SendCMD_HERO_ACTION_CHANGED(ch);	// 011214 LTS
	}
	
	ch->direction		= dir;
	ch->nCurrentAction	= anino;
	ch->nCurrentFrame	= 0;
	ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
	ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	

	return TRUE;
}

// 011030 KHS JJING
void LoadAttackRangeTable( int sprno )
{	
	FILE *fp;
	char temp[ FILENAME_MAX];
	int  no, min, max;
	int  basic, walk, att1, att2, att3, standtype, wavewoman, waveman;
	int  i;
	
	sprintf( temp, "./data/%03dAttackRangeTable.txt", sprno );
	
	fp = Fopen( temp, "rt" );
	if( fp )
	{		
		fgets( temp, FILENAME_MAX, fp );
		while( 1 )
		{	
			if( fgets( temp, FILENAME_MAX, fp ) == NULL ) break;
			//sscanf(temp, "%d %d %d %d %d %d %d %d %d %d %d %s\n", &no, &min, &max, &basic, &walk, &att1, &att2, &att3, &standtype, &wavewoman, &waveman);
			sscanf(temp, "%d %d %d %d %d %d %d %d %d %d %d\n", &no, &min, &max, &basic, &walk, &att1, &att2, &att3, &standtype, &wavewoman, &waveman);
			AttackRangeTable[sprno][no][0] = min;
			AttackRangeTable[sprno][no][1] = max; 
			if( basic) basic --; AttackRangeTable[sprno][no][2] = basic;
			if( walk ) walk  --; AttackRangeTable[sprno][no][3] = walk;
			if( att1 ) att1  --; AttackRangeTable[sprno][no][4] = att1;
			if( att2 ) att2  --; AttackRangeTable[sprno][no][5] = att2;
			if( att3 ) att3  --; AttackRangeTable[sprno][no][6] = att3;
			AttackRangeTable[sprno][no][7] = standtype;
			AttackRangeTable[sprno][no][8] = wavewoman;
			AttackRangeTable[sprno][no][9] = waveman;
		}	
		fclose(fp);
		
		sprintf( temp, "./data/%03dAttackRangeTable.bin", sprno );
		fp = Fopen( temp, "wb" );
		if( fp )
		{	
			char *tt = (char *)AttackRangeTable[sprno];
			char crc = 0;
			for( i = 0 ;  i < sizeof( int ) * CHARACTER_ATTACKRANGE_MAX_*10 ;  i ++)
			{
				*tt += i;
				crc += *tt;
				tt ++;
			}
			
			fwrite( &crc, 1,1, fp );
			fwrite( AttackRangeTable[sprno], sizeof( int ), CHARACTER_ATTACKRANGE_MAX_*10, fp);
			fwrite( &crc, 1,1, fp );
			fclose(fp);
			
			tt = (char *)AttackRangeTable[sprno];
			for( i = 0 ;  i < sizeof( int ) * CHARACTER_ATTACKRANGE_MAX_*10 ;  i ++)
			{
				*tt -= i;
				tt ++;
			}
		}
	}	
	else
	{	
		char *tt = (char *)AttackRangeTable[sprno];
		char crc = 0, crc1, crc2;
		sprintf( temp, "./data/%03dAttackRangeTable.bin", sprno );
		fp = Fopen( temp, "rb" );
		fread( &crc1, 1,1, fp );
		fread( AttackRangeTable[sprno],  sizeof( int ), CHARACTER_ATTACKRANGE_MAX_*10, fp);
		fread( &crc2, 1,1, fp );
		fclose(fp);
		
		for( i = 0 ;  i < sizeof( int ) * CHARACTER_ATTACKRANGE_MAX_*10 ;  i ++)
		{
			crc += *tt;
			*tt -=i;
			tt ++;
		}
		
		if( crc  != crc1 ) if( CheckSumError == 0 ) CheckSumError = CHECKSUM_ERROR_ATTACKRANGE;
		if( crc  != crc2 ) if( CheckSumError == 0 ) CheckSumError = CHECKSUM_ERROR_ATTACKRANGE;
	}
}	



int IsTooNearCharacter( int x, int y, LPCHARACTER ch )
{	
	return 1;
	
	
	LPCHARACTER lptemp = ch->lpNext;
	
	// 绊磨巴1010....
	
	int range = (AttackRangeTable[ch->sprno][ ch->accessory[ 2]][0] + 10) / 2;
	int ex, ey;
	
	while( lptemp != NULL )
	{	
		ex = lptemp->x;
		ey = lptemp->y;
		if( (ex-x)*(ex-x) + (ey-y)*(ey-y) < range*range ) return 1;
		lptemp = lptemp->lpNext;
	}	
	
	return 0;
}		



//--------------------------------
//			
//			
//			
//			
//	叼弃飘肺  64 鸥老 1俺 窜困肺 积阿 

//////////////////////// 0529 lkh 荐沥 ////////////////////////////
int IsAttackRange( 	int *minrange, int *maxrange, LPCHARACTER ch, int ex, int ey )
{			
	//	傍拜 裹困俊 康氢阑 固摹绰 荤亲俊 措茄 扒.
	int minret, maxret;
	int curr;	
	int nNo = GetCharAccessory(ch); // CSD-030716

	if ((nNo-121)>=0&&(nNo-121)<6)	// LTS HORSERIDER // Flag	// 芒狼 荤沥芭府肺 技泼
	{
		nNo=75;
	}
	
	//	公扁磊眉狼 荤沥芭府.
	int minr = int(AttackRangeTable[ch->sprno][nNo][0]>>1);
	int maxr = AttackRangeTable[ch->sprno][nNo][1] + int(AttackRangeTable[ch->sprno][nNo][1]/3);	
	int x = ch->x;
	int y = ch->y;
	//	int ex = ((LPCHARACTER)(ch->lpAttacked))->x;
	//	int ey = ((LPCHARACTER)(ch->lpAttacked))->y;
	
	*minrange = minr;
	*maxrange = maxr;
	
	curr = (ex-x)*(ex-x)+(ey-y)*(ey-y);
	
	if( curr > maxr*maxr ) return ATTACKRANGE_OUT_MAX_;
	if( curr < minr*minr ) return ATTACKRANGE_IN_MIN_;
	
	return ATTACKRANGE_IN_AREA_;
	
	//	minret = BoxAndDotCrash( eminx, eminy, minr+3, minr+3, x, y );
	//	maxret = BoxAndDotCrash( emaxx, emaxy, maxr+3, maxr+3, x, y );
	
	//	*range= sqrt( (ex-x)*(ex-x) + (ey-y)*(ey-y));
	
	if( !maxret ) return ATTACKRANGE_OUT_MAX_;
	if( minret ) return ATTACKRANGE_IN_MIN_;
	
	return ATTACKRANGE_IN_AREA_;
}



/////////////////////////////////////// 0625 lkh 荐沥 //////////////////////////////////////////
int ReturnXYNearPosition( int *x, int *y, int *dx, int *dy, int range, LPCHARACTER hero, int destx, int desty )
{		
	DIRECTION dir;	
	int mx, my, lx, ly;
	int tox, toy;
	
	int tdir;
	int i;
	
	////////////////// 0524 lkh 荐沥 //////////////////
	int dirtable[10] = { 0, 36, 72, 108, 144, 180, 216, 252, 288, 324 };		//阿档
	
	//	ChangeDirection( &dir, destx, desty, hero->x, hero->y );
	
	//	ReturnCircleResult(int s_X, int s_Y, int e_X, int e_Y, float &result_X, float &result_Y, int angle)
	
	onLineDistantPosition(  destx, desty, hero->x, hero->y, &tox, &toy, range );
	lx = destx + tox;
	ly = desty + toy;
	
	mx = lx / TILE_SIZE;
	my = ly / TILE_SIZE;
	
	if( FreeTile( hero, mx, my, mx, my ))
	{
		*x = lx, *y = ly;
		*dx = mx, *dy = my;
		return 1;
	}
	
	//	return 0;
	
	int tx = lx, ty = ly;
	float fx, fy;
	for( i = 0 ; i < 10 ;  i ++)
	{
		ReturnCircleResult( destx, desty, tx, ty, fx, fy, dirtable[i] );
		
		lx = (int )fx;
		ly = (int )fy;
		
		mx = lx / TILE_SIZE;
		my = ly / TILE_SIZE;
		
		
		if( FreeTile( hero, mx, my, mx, my ))
		{
			*x = lx, *y = ly;
			*dx = mx, *dy = my;
			return 1;
		}
	}
	
	return 0;
	
	for( i = 0 ; i < 8 ; i ++)
	{	
		lx = mx = destx;
		ly = my = desty;
		tdir = ( dirtable[i] + dir ) % 8;
		switch( tdir )
		{
		case	DIRECTION_UP:						my += 1000;	break;
		case	DIRECTION_RIGHTUP:		mx -= 1000;	my += 1000;	break; 
		case	DIRECTION_LEFTUP:		mx += 1000;	my += 1000;	break;
		case	DIRECTION_RIGHT:		mx -= 1000;				break;
		case	DIRECTION_LEFT:			mx += 1000;				break;
		case	DIRECTION_RIGHTDOWN:	mx -= 1000;	my -= 1000;	break;
		case	DIRECTION_LEFTDOWN:		mx += 1000;	my -= 1000;	break;
		case	DIRECTION_DOWN:						my -= 1000;	break;
		}
		
		onLineDistantPosition( lx, ly, mx, my,  &tox, &toy, range );
		
		lx += tox;		
		ly += toy;		
		
		*dx = lx / TILE_SIZE;
		*dy = ly / TILE_SIZE;
		
		int minrange, maxrange;
		int minormax;
		
		if ( !TileMap[ *dx ][ *dy ].attr_dont && FreeTile( hero, *dx, *dy, *dx, *dy ))
		{				
			minormax = IsAttackRange( &minrange, &maxrange, Hero, destx, desty );
			
			*x = lx;
			*y = ly;
			return 1;	
		}				
	}						
	
	return 0;
}		


int ReturnXYNearNPC( int *x, int *y, int *dx, int *dy, int range, LPCHARACTER hero, LPCHARACTER mon )
{				
	DIRECTION dir;	
	int mx, my, lx, ly;
	int tox, toy;
	int tdir;	
	int dirtable[ 8] = { 0, 1, -1, 2, -2, 3,-3, 4 };
				
	ChangeDirection( &dir, hero->x, hero->y, mon->x, mon->y );
				
	for( int i = 0 ; i < 8 ; i ++)
	{					
		lx = mx = mon->x;
		ly = my = mon->y;
		
		tdir = ( dirtable[i] + dir ) % 8;
		
		switch( AttackRangeTable[hero->sprno][hero->accessory[2]][7] )
		{		
		case 0 : switch( tdir )	
				 {				
		case	DIRECTION_UP:						my += 1000;	break;
		case	DIRECTION_RIGHTUP:		mx -= 1000;	my += 700;	break; 
		case	DIRECTION_LEFTUP:		mx += 1000;	my += 700;	break;
		case	DIRECTION_RIGHT:		mx -= 1000;				break;
		case	DIRECTION_LEFT:			mx += 1000;				break;
		case	DIRECTION_RIGHTDOWN:	mx -= 1000;	my -= 700;	break;
		case	DIRECTION_LEFTDOWN:		mx += 1000;	my -= 700;	break;
		case	DIRECTION_DOWN:						my -= 1000;	break;
				 }				
			break;
		default :switch( tdir )	
				 {				
		case	DIRECTION_UP:						my += 1000;	break;
		case	DIRECTION_RIGHTUP:		mx -= 1000;	my += 1000;	break; 
		case	DIRECTION_LEFTUP:		mx += 1000;	my += 1000;	break;
		case	DIRECTION_RIGHT:		mx -= 1000;				break;
		case	DIRECTION_LEFT:			mx += 1000;				break;
		case	DIRECTION_RIGHTDOWN:	mx -= 1000;	my -= 1000;	break;
		case	DIRECTION_LEFTDOWN:		mx += 1000;	my -= 1000;	break;
		case	DIRECTION_DOWN:						my -= 1000;	break;
				 }		
			break;
		}		
		
		onLineDistantPosition( lx, ly, mx, my,  &tox, &toy, range );
		
		lx += tox;		
		ly += toy;		
		
		*dx = lx / TILE_SIZE;
		*dy = ly / TILE_SIZE;
		
		if ( !TileMap[ *dx ][ *dy ].attr_dont && FreeTile( hero, *dx, *dy, *dx, *dy ))
		{				
			*x = lx;			
			*y = ly;		
			return 1;	
		}				
	}						
	
	return 0;		
}		


int ReturnAccesablePosition( LPCHARACTER ch, int sx, int sy, int ex, int ey, int range, int *dx, int *dy )
{	
	int range0 = (int)sqrt( (ex-sx)*(ex-sx) + (ey-sy)*(ey-sy) );
	
	if( range0 == 0 ) return 0;
	
	*dx = sx + ( range * ( ex - sx )/ range0 ) ;
	*dy = sy + ( range * ( ey - sy )/ range0 ) ;
	int mx = *dx / TILE_SIZE;
	int my = *dy / TILE_SIZE;
	
	if( mx < 0 ) return 0;
	if( my < 0 ) return 0;
	if( mx >= g_Map.file.wWidth  ) return 0;
	if( my >= g_Map.file.wHeight ) return 0;
	
	if ( !TileMap[ mx ][ my ].attr_dont && FreeTile( ch, mx, my, mx, my ))
	{				
		return 1;	
	}				
	
	return 0;
}




//   傍拜阑 茄促绰巴篮 傍拜裹困俊 乐促绰 巴捞骨肺 
//   捞窃荐甫 龋免且鞘夸 绝促.	
//   盔贰 傍拜 悼累捞 甸绢啊扁 傈俊 傍拜且荐 乐绰瘤甫 魄窜窍瘤父 傍拜阑 沁促秦档 
//   弊镑俊 惑措啊 绝阑荐 乐栏骨肺  傍拜 Frame俊辑 促矫 魄窜窍扁 困茄 窃荐 ... 
//	澜.. 弊烦 傍拜啊瓷 MinRange, MaxRange蔼阑 罐酒 贸府秦具 摆焙....
int InAttackArea( LPCHARACTER ch, LPCHARACTER tchar )
{		
	DIRECTION dir;
	int hx = ch->x, hy = ch->y, 
		ex = tchar->x, ey = tchar->y;
	int minr = ch->attackMinRange;
	int maxr = ch->attackMaxRange;
	
	
	return TRUE;
	
	
	
	ChangeDirection( &dir, hx, hy, ex, ey );
	if( dir == ch->direction )
	{		
		if( ( ey - hy ) * ( ey - hy ) + ( ex - hx ) * ( ex - hx ) > minr * minr )
			if( ( ey - hy ) * ( ey - hy ) + ( ex - hx ) * ( ex - hx ) < maxr * maxr )
			{	
				return TRUE;
			}	
	}		
	
	return FALSE;
}	


//	悼累吝 attackf[]啊 1牢啊甫 魄窜窍绊 1捞搁 傍拜蜡瓤贸府甫 茄促. 
//		
void GetAttacked(LPCHARACTER ch)
{	//< CSD-031014	
	switch (ch->sprno)
	{
	case 50:
	case 51:
		{
			if (ch->nCurrentFrame == 5) // 宏饭胶甫 徽绢扼...
			{
				InsertMagic(ch, ch, 102, 0, 0, 0, -370, 60, 0, 0, 0);
			}

			break;
		}
	default:
		{
			if (ch->AnimationTable[ch->nCurrentAction].effect[ch->nCurrentFrame])
			{
				InsertMagic(ch, ch, ch->AnimationTable[ch->nCurrentAction].effect[ch->nCurrentFrame], 4, 0, 0, 0, 0);
			}

			break;
		}
	}

	if (ch->AnimationTable[ch->nCurrentAction].attackf[ch->nCurrentFrame] == 0)
	{
		return;
	}

	switch (ch->nAttackedType)
	{	
	case SPRITETYPE_CHARACTER: 
	case SPRITETYPE_MONSTER: 
	case SPRITETYPE_NPC:
		{
			LPCHARACTER tchar = (LPCHARACTER)ch->lpAttacked;

			if (tchar == NULL)
			{ 
				//JustMsg( "tchar == NULL");
				break;
			}
			
			if (!IsDead(tchar))		//磷绢乐瘤 臼篮 版快
			{	
				if( !tchar->sealstone && tchar->sprno != 100 && tchar->sprno != 101 && 
					tchar->sprno!=63 && tchar->sprno!=64 && tchar->sprno!=65 && tchar->sprno!=66 &&			// LTS DRAGON BUG
					tchar->sprno!=67 && tchar->sprno!=50 && tchar->sprno!=51 && tchar->sprno!=52) // 荐龋籍1,2 己巩1,2,3  // 嘎酒档 规氢捞 官差搁 救等促.
					//_____________________________________________________________________________________ // 010904 LTS
				{	//< CSD-040313
					ChangeDirection( &tchar->direction, tchar->x, tchar->y, ch->x, ch->y );
					tchar->todir = tchar->direction;
				}	//> CSD-040313
				
				int ani = AttackedAction( tchar );//ACTION_ATTACKED;
				if( ani != tchar->nCurrentAction ) CharDoAni( tchar, tchar->direction, ani );
				
				if (rand()%2==1 && !(tchar->sprno >= 92 || tchar->sprno == 16 || tchar->sprno == 9 || tchar->sprno ==37 ||
					tchar->sprno == 38 || tchar->sprno == 44 || tchar->sprno == 45 || tchar->sprno == 46 ||	tchar->sprno == 48 || tchar->sprno == 52))			//歹固牢 版快
				{ 
					RandomAttackEffect(BLOOD_EFFECT, ch, tchar);
				}
				else 
				{
					RandomAttackEffect(METAL_EFFECT, ch, tchar);
				}
				
				GetCombat()->Effect(ch, tchar);
			}
			
			break;
		}
	case SPRITETYPE_ITEM:
		{
			LPITEMGROUND i = (LPITEMGROUND)ch->lpAttacked;
			
			if (i == NULL)
			{
				break;
			}
			
			if (i->attr[IATTR_ATTR]&IA2_BOX)
			{
				if (!(i->attr[IATTR_ATTR]&IA2_OPENED)) // 凯妨 乐绰霸 酒聪搁 
				{
					//  辑滚俊辑 捞何盒阑 罐绰促. 
					//i->Anitype = ITEMANIMATION_OPENNING;
					//
					if (ch->boxitembreaktype == 0)
					{
						InsertMagic(Hero, Hero, 208,0, i->x, i->y, 0,0);
					}
					else if (ch == Hero)
					{
						if (Hero->sprno == 0) //咯磊捞搁.
						{
							switch( Hero->accessory[2] )
							{
							case 76:
							case 77:
							case 78:
							case 79: // 劝捞唱 厚八牢版快肺 捞公扁绰 Magic贸府俊辑 Send窍霸等促. 
								{
									break;
								}
							default: 
								{
									Send_CMD_ITEM_BOX_BREAK_RESULT(i->server_id);
									break;
								}
							}
						}
						else
						{
							switch (Hero->accessory[2])
							{
							case 78:
							case 79:
							case 80:
							case 81: // 劝捞唱 厚八牢版快肺 捞公扁绰 Magic贸府俊辑 Send窍霸等促. 
								{
									break;
								}
							default: 
								{
									Send_CMD_ITEM_BOX_BREAK_RESULT(i->server_id);
									break;
								}
							}
						}
					}
				}
			}
			break;
		}
	}	
}	//> CSD-031014

int CanAttackAnimation( LPCHARACTER ch )
{
	LPCHARACTER dch = (LPCHARACTER )( ch->lpAttacked );
	int dist;
	
	return 1;
	
	
	if( dch == NULL ) return 0;
	
	switch( ch->sprno )
	{
	case 38 : dist = 80;	break;
	default : dist = 85;	break;
	}
	
	if( (dch->x-ch->x)*(dch->x-ch->x) + (dch->y-ch->y)*(dch->y-ch->y) > dist * dist )
	{
		return 0;
	}
	
	else return 1;
	
	//	ch->animationType, MON1_BATTLE_ATTACK )
}	


//	傍拜 悼累 锅龋甫 府畔茄促. 
//	唱吝俊 某腐磐喊( 巢/赤 ) 备盒, 厘馒茄 公扁俊 蝶扼 傍拜 悼累锅龋甫  府畔茄促.  

//  0531 lkh 荐沥(拳混.厚八 惯荤矫 眉农风凭 眠啊)
int AttackAction( LPCHARACTER ch )
{			
	if( IsDead( ch ) ) return ch->nCurrentAction;
	
	if( ch->sex == 0 )
	{		
	}		
	
	switch( ch->animationType )
	{		
	case ANIMATION_TYPE_MON1 :	//	return MON1_BATTLE_ATTACK;
		
		if( CanAttackAnimation( ch ) )	
		{
			return MON1_BATTLE_ATTACK;
		}
		else 
		{	
			ch->lpAttacked = NULL;
			return ch->nCurrentAction;
		}	
		break;
		
	case ANIMATION_TYPE_WOMAN :
	case ANIMATION_TYPE_MAN :
		break;
	}		
	
	int ret = g_mgrBattle.Action(ch);
	
	if( ret== ACTION_ATTACK_BOW || ret == ACTION_THROW ) // 
	{		
		LPCHARACTER		target_Char;
		LPITEMGROUND	i;
		
		switch( ch->nAttackedType )
		{
		case SPRITETYPE_ITEM :			i = (LPITEMGROUND )(ch->lpAttacked);
			/////////////////// 0609 lkh 牢磊 荐沥 /////////////////
			if( !TileLineCheck( ch->x, ch->y, i->x, i->y, 1 ))		//惯荤傈 固府 版肺惑 厘局拱 咯何 犬牢->阜腮 版快
			{
				ret = ch->basicAction;
			}
			break;
			
		case SPRITETYPE_NPC :
		case SPRITETYPE_MONSTER :
		case SPRITETYPE_CHARACTER:		target_Char=(LPCHARACTER)(ch->lpAttacked);
			if( !TileLineCheck( ch->x, ch->y, target_Char->x, target_Char->y) )			//惯荤傈 固府 版肺惑 厘局拱 咯何 犬牢
			{
				ret = ch->nCurrentAction;
				ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为

			}
			break;
			// 0221 lkh 眠啊
		case SPRITETYPE_ON_THE_CEILING_CHAR:
			target_Char=(LPCHARACTER)(ch->lpAttacked);
			ret = ch->nCurrentAction;
			ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
			break;
			
		}
		
		if( ch == Hero )	
		{					
			if( ret == ACTION_ATTACK_BOW )
			{		
				const int iItemKind = GetItemAttr(EquipItemAttr[WT_SHIELD].item_no, ITEM_KIND);
				switch(iItemKind)
				{
				case IK_TWO_ARROW:
				case IK_DUAL_ARROW://030102 lsw
					{
						ret = ACTION_ATTACK_BOW;
					}break;
				default:
					{
						AddCurrentStatusMessage(255, 20, 20, lan->OutputMessage(0,31));//010215lsw 拳混绝嚼聪促.
						ret	= ch->nCurrentAction = ch->basicAction;
					ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为

					}break;
				}
			}				
			else if( ret == ACTION_THROW ) 	// 厚八老 版快
			{
				ret = ACTION_THROW;
			}
		}
		
	}
	
	return ret;
}


int AttackActionItem( LPCHARACTER ch )
{			
	int t;
	int ret;
	
	if( IsDead( ch ) ) return ch->nCurrentAction;
	
	if( ch->sex == 0 )
	{		
	}		
	
	switch( ch->animationType )
	{		
	case ANIMATION_TYPE_MON1 :	return MON1_BATTLE_ATTACK;
		break;
		
	case ANIMATION_TYPE_WOMAN :
	case ANIMATION_TYPE_MAN :
		break;
	}
	
	t = ch->accessory[2];
	
	if( AttackRangeTable[ch->sprno][ t][5] )
	{	
		if( Random( 2 ) ) ret  = AttackRangeTable[ch->sprno][ t][5];
		else ret  = AttackRangeTable[ch->sprno][ t][4];
	}	
	else 
	{	
		ret = AttackRangeTable[ch->sprno][ t][4];
	}
	
	/////////////////////////// 0530 lkh 眠啊 /////////////////////////////////////
	if( ret == ACTION_ATTACK_BOW || ret == ACTION_THROW )
	{
		LPCHARACTER		target_Char=(LPCHARACTER)(ch->lpAttacked);
		
		if( target_Char != NULL )
		{	
			if( ret == ACTION_ATTACK_BOW )
			{
				
				const int iItemKind = GetItemAttr(EquipItemAttr[WT_SHIELD].item_no, ITEM_KIND);
				switch(iItemKind)
				{
				case IK_TWO_ARROW:
				case IK_DUAL_ARROW://030102 lsw
					{
						if( !TileLineCheck( ch->x, ch->y, target_Char->x, target_Char->y) )				//惯荤傈 固府 版肺惑 厘局拱 咯何 犬牢(绝绰 版快 惯荤)
						{
							ret = ch->nCurrentAction;
							EquipItemAttr[WT_SHIELD].attr[IATTR_MUCH]--;
					ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为

						}
					}break;
				default:
					{
						if( ch == Hero )
							AddCurrentStatusMessage(255, 20, 20, lan->OutputMessage(0,31));//010215 lsw
						ret	= ch->nCurrentAction = ch->basicAction;
					ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为

					}break;
				}
			}				
			else	// 厚八老 版快
			{
				EquipItemAttr[WT_WEAPON].attr[IATTR_MUCH]--;
			}
		}
	}
	return ret;
}			

// 020818 YGI
void ChangeMovePointByDontBox( LPCHARACTER ch )
{
	int x = ch->x/TILE_SIZE;
	int y = ch->y/TILE_SIZE;
	if( TileMap[x][y].attr_dont )
	{
		for( int i=-1; i<=1; i++)
			for( int j=-1; j<=1; j++ )
			{
				if( x+i <= 0 || x+i >= g_Map.file.wWidth || y+i <= 0 || y+i >= g_Map.file.wHeight ) continue;
				if( !TileMap[x+i][y+j].attr_dont )
				{
					ch->x = (x+i)*TILE_SIZE;
					ch->y = (y+j)*TILE_SIZE;
					return;
				}
			}
	}
}


//  傍拜寸茄第狼 青悼 .
int AttackedAction( LPCHARACTER ch )
{				
	if( IsDead( ch ) ) return ch ->nCurrentAction;
	
	
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		if( ch ==  Hero )
		{
			AddNumberAni( NUMBER_ANIMATION_TYPE_DOWN_, ch->y+1, ch->x + 50, ch->y - 44, ch->hp - ch->NumberAnimationValue );
			AddNumberAni( NUMBER_ANIMATION_TYPE_DOWN_, ch->y+1, ch->x + 100, ch->y - 44, ch->hpmax );
		}
	}
	
	switch( ch->attackedresult )
	{
		// Damage and Death..
		////////////////// 0109 捞痹绕 眠啊/荐沥 ///////////////////////////	
	case 1 :	
		switch( ch->animationType )
		{
		case ANIMATION_TYPE_MAN   :
		case ANIMATION_TYPE_WOMAN :	
			return ACTION_DIE;
		case ANIMATION_TYPE_MON1  :	
			return MON1_DIE;
		}
		break;	
		
		// Failed..
		case 0 :	ch->NumberAnimationValue = 0;
			break;
			// Damage and Not Death..
		case 2 : 	//if( ch->NumberAnimationValue <= 0 )	break; / //###
			
			if( ch->pathcount > 2 || ch->moveFlag == TRUE ) // 捞悼吝捞菌栏搁 
			{
				ch->pathcount = 0;
				ch->moveFlag = 0;
				Moveoldx = 0;
				Moveoldy = 0;
				// 020818 YGI --------------
				if( ch == Hero )
				{
					ChangeMovePointByDontBox( ch );
					
					if( !ch->x && !ch->y )
					{
						ch->gox = ch->x;
						ch->goy = ch->y;
						SendMoveData( ch );
					}
				}
				// ----------------------------
			}
			
			switch( ch->animationType )
			{
			case ANIMATION_TYPE_MAN   :
			case ANIMATION_TYPE_WOMAN :	return ACTION_ATTACKED;
			case ANIMATION_TYPE_MON1  :	return MON1_ATTACKED;
			}
			break;	
	}			
	
	return ch->nCurrentAction; // basicAction;
					ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为

}				



int DieAction( LPCHARACTER ch )
{				
	int ani;
	
	if( ch->sex == 0 )
	{			
	}		
	
	switch( ch->animationType )
	{		
	case ANIMATION_TYPE_MON1 :	if( ch->nCurrentAction != MON1_DIE ) ch->nCurrentFrame = 0;
								ani = MON1_DIE;
								break;
								
	case ANIMATION_TYPE_WOMAN :
	case ANIMATION_TYPE_MAN :	if( ch->nCurrentAction != ACTION_DIE ) ch->nCurrentFrame = 0;
								ani = ACTION_DIE;
	}
	
	return ani;
	
}				

// 公扁啊 官顿搁 捞窃荐档 龋免秦林绢具 茄促. 
// 弊贰具 Animation捞 官差聪鳖..
void SettingBasicActionSub( LPCHARACTER ch )
{	//< CSD-030723
	if (ch->sprno > 1)
	{
		return;
	}

	int t;		
	int oldbasic, oldwalk;	
	int sprno = ch->sprno;
	
	switch (ch->peacests)
	{	
	case 0: // 乞拳惑怕...	
		{
			oldbasic = ch->basicAction;
			oldwalk  = ch->basicWalk;
			
			ch->basicAction = ACTION_NONE;
			ch->basicWalk	= ACTION_BASIC_WALK; 
			if( ch->nCurrentAction == oldbasic )	
			{ ch->nCurrentAction = ch->basicAction; ch->nCurrentFrame = 0; ch->set_nCurrentFrame(ch->nCurrentFrame); ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
 }//人物晃动值	
			if( ch->nCurrentAction == oldwalk )		
			{ ch->nCurrentAction = ch->basicWalk;	ch->nCurrentFrame = 0; ch->set_nCurrentFrame(ch->nCurrentFrame); ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
 }//人物晃动值	 
			break;
		}
	case 1:
		{
			oldbasic = ch->basicAction;
			oldwalk  = ch->basicWalk;
			t = GetCharAccessory(ch);

			if ((t-121)>=0&&(t-121)<6)	// LTS HORSERIDER // Flag	// 芒狼 荤沥芭府肺 技泼
			{
				t=75;
			}
			
			ch->basicAction = AttackRangeTable[sprno][t][ 2];
			ch->basicWalk	= AttackRangeTable[sprno][t][ 3];
			
			if (ch->nCurrentAction == oldbasic)
			{ ch->nCurrentAction = ch->basicAction; ch->nCurrentFrame = 0; ch->set_nCurrentFrame(ch->nCurrentFrame); ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
} //人物晃动值	}
			if( ch->nCurrentAction == oldwalk )		
			{ ch->nCurrentAction = ch->basicWalk;	ch->nCurrentFrame = 0; ch->set_nCurrentFrame(ch->nCurrentFrame); ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
} //人物晃动值	}
			break;				
		}
	}	
}	//> CSD-030723
// main_interface俊辑 peace/battle滚瓢阑 穿福搁 龋免等促. 
void SettingBasicAction( int peacests )
{	
	//acer7
	Hero->peacests = peacests?1:0;
	SettingBasicActionSub( Hero );
	
	SendPeaceBattle( Hero );
}	

void SendModifyPosition( LPCHARACTER ch )
{
	t_packet p;
	
	if( ch != Hero ) return;
	
	p.h.header.type = CMD_MODIFY_POSITION;
	{
		p.u.client_modify_position.x = ch->x;
		p.u.client_modify_position.y = ch->y;
	}
	p.h.header.size = sizeof( t_client_modify_position );
	
	QueuePacket( &p, 1 );
}

void RecvModifyPosition( t_server_modify_position *p )
{
	//	if( SysInfo.gamemakemode ) AddCurrentStatusMessage( 255,150,100, "Modify Position" );
	
	LPCHARACTER ch = FindCharacter( &g_CharacterList, p->server_id );
	if( ch == NULL ) return;
	
	if( ch->gox == p->gox && ch->goy == p->goy )
	{
		switch(ch->animationType)
		{
		case ANIMATION_TYPE_MON1 :
			if (ch->nCurrentAction!=ch->basicAction)
				ch->nCurrentAction=ch->basicAction; ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
		}
	}
	else
	{
		switch( ch->animationType )	
		{							
		case ANIMATION_TYPE_MON1 :	ch->gox = p->gox;
			ch->goy = p->goy;
			ch->moveFlag = TRUE;
			
			
			
			if ( ch->nCurrentAction != ch->basicWalk )
			{
				ch->nCurrentAction = ch->basicWalk;
				ch->nCurrentFrame = 0;
				ch->set_nCurrentAction(ch->nCurrentAction); //人物动作行为
				ch->set_nCurrentFrame(ch->nCurrentFrame); //人物晃动值	}

			}
			break;					
		}										
	}
}

// 010905 KHS
void RecvModifyPositionAbsolute( t_server_modify_position_absolute *p )
{
	LPCHARACTER ch = FindCharacter( &g_CharacterList, p->id );
	if( ch == NULL ) return;
	ch->x = p->x;
	ch->y = p->y;
	
	TileMap[ ch->position.x][ch->position.y].occupied = 0;
	ch->position.x = ch->x / TILE_SIZE;
	ch->position.y = ch->y / TILE_SIZE;
	if (ch->hp>0) TileMap[ ch->position.x][ch->position.y].occupied = 1;	// 010915 LTS
}


void RecvAcceptJoin( t_server_accept_join  *p )
{
	Hero->x = 	p->x;
	Hero->y =	p->y;
	
	Hero->position.x = 	p->x / TILE_SIZE;
	Hero->position.y =	p->y / TILE_SIZE;
}
//----------------------------------------------------------
void RecvMantleCandle( t_mantle_candle *tp )
{
	LPCHARACTER ch = ReturnCharacterPoint( tp->id );
	if( ch == NULL ) return;
	
	ch->mantle = tp->mantleno;
	
	ch->candle = tp->candle;
}

void RecvCandleOn( t_candle_on *tp )
{
	LPCHARACTER ch = ReturnCharacterPoint( tp->id );
	if( ch == NULL ) return;
	
	ch->candle = 1;
}

void RecvCandleOff( t_candle_off *tp )
{
	LPCHARACTER ch = ReturnCharacterPoint( tp->id );
	if( ch == NULL ) return;
	
	ch->candle = 0;
}

char* GetNpcName(int nSprNo)
{	//< CSD-030419
	if (strcmp(g_infNpc[nSprNo].szName, "") == 0)
	{
		return g_infNpc[nSprNo%100].szName;
	}
	
	return g_infNpc[nSprNo].szName;
}	//> CSD-030419

int GetNpcSprNo(CHARACTER* pNpc)
{	//< CSD-030419
	const int nSprNo = pNpc->sprno;
	
	if (strcmp(g_infNpc[nSprNo].szName, "") == 0)
	{
		return nSprNo%100;
	}
	
	return nSprNo;
}	//> CSD-030419

bool IsExistNpcSprNo(int nSprNo)
{	//< CSD-030419
	if (strcmp(g_infNpc[nSprNo].szName, "") == 0)
	{
		return false;
	}
	
	return true;
}	//> CSD-030419

int GetCharAccessory(CHARACTER* pTarget)
{	//< CSD-031013
	return pTarget->accessory[AT_WEAPON];
}	//> CSD-031013