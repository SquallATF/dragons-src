/*****************************************************************************\
* Copyright (c), Future Entertainment World / Seoul, Republic of Korea        *
* All Rights Reserved.                                                        *
*                                                                             *
* This document contains proprietary and confidential information.  No        *
* parts of this document or the computer program it embodies may be in        *
* any way copied, duplicated, reproduced, translated into a different         *
* programming language, or distributed to any person, company, or             *
* corporation without the prior written consent of Future Entertainment World *

	.무한긍적의 신념과 투지로, 하면된다.
	.창의력과 집중력으로 창조성을 발휘, 않되면 되게 하라.
	.평범한 모습/ 평상의 마음이 가장 아름답다.

		
\*****************************************************************************/


#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <Math.h>
#include "Resource.h"


#include "Dragon.h"

#define __HONG_SUB_H__
#include "Hong_Sub.h"


#include "Hong_Sprite.h"
#include "Hangul.h"

#include "Tool.h"

extern CHARACTERLIST g_CharacterList;

int YesOrNo( char *s, char *title )
{	
    char temp[ FILENAME_MAX];
    va_list arg;
    va_start( arg, s );
    vsprintf( temp, s, arg );
    va_end( arg );
	
	HWND hwnd = GetActiveWindow();
	return MessageBox( hwnd, temp, title, MB_YESNO | MB_ICONQUESTION );
}	

/*────────────────────────────────────────
함수명 : void  ( char *mssg,... )
설  명 : Error Message를 출력하고 OK를 누르면 종료한다.
인  자 :
리  턴 : 없슴.
────────────────────────────────────────*/
void Error ( char *mssg, ... )
{
	char caTemp[ FILENAME_MAX];
	va_list va;
	HWND hwnd;

	va_start( va, mssg );
	vsprintf( caTemp, mssg, va );
	va_end( va );

	hwnd = GetActiveWindow();

	MessageBox( hwnd, caTemp, "DragonRaja On-Line", MB_OK );

	_fcloseall();
	PostQuitMessage(0);

	return;
}

void MemFree_Sub( char **buf, char *file, int line  )
{
	if( *buf == NULL ) {return;}

    IMemUseCount--;

	SAFE_DELETE_ARRAY(*buf);
}

void __Alloc( char **s, int size, const char *file, int line )
{	
	SAFE_DELETE_ARRAY(*s);
    *s = new char[size];
	ITotalUseMemory += size;
    if( !(*s) )
	{
        Error( lan->OutputMessage(6,62), file, line, size );//lsw
	}
	::memset( *s, 0, size );
    IMemUseCount++;
}	

/*────────────────────────────────────────
함수명 : FILE *FopenSub_( char *name, char *mode )
설  명 :
인  자 :
리  턴 :
────────────────────────────────────────*/
char checkbeta=2;

extern void DeleteBetaFile( char *dir );
void SaveBetaFileName( char *file_name )
{
	static int first = 1;
	FILE *fp;
	if( first )
	{
		first = 0;
		fp = fopen( "./SaveBetaFileName.txt", "wt" );
	}
	else fp = fopen( "./SaveBetaFileName.txt", "at+" );

	if( !fp ) return;
	fprintf( fp, "%s\n", file_name );
	fclose( fp );
}

// thai2 YGI
#ifdef _DEBUG
class KeinFile
{	//< CSD-030324
public :
	FILE *m_fp;
public :	
	KeinFile( const char *filename, const char *mode )
	{
		m_fp = fopen( filename, mode );
	}
	~KeinFile()
	{
		if( m_fp ) fclose( m_fp );
	}
};	//> CSD-030324

//KeinFile temp_fp("UsedFileName.txt", "wt") ;
#endif


FILE *FopenSub_( char *name, char *filemode, char *wherefile, int line )
{ 
	
//	 thai2 YGI
//#ifdef _DEBUG
//	if( temp_fp.m_fp )
//	{
//		fprintf( temp_fp.m_fp, "%s\n", name );
//	}
//#endif
	

	FILE *fp;
	if(checkbeta == 2 )//한번만 호출되는 부분.. 어째되던 값이 0또는 1로 
	{
		fp = fopen("./beta.ini","r");
		if(fp!=NULL)
		{
			fread( &checkbeta, 4,4,fp );
			fclose( fp );

			if (checkbeta=='1')
			{	//< CSD-031030
				fp = fopen( "./data_b5/beta.txt", "r" );
				if( !fp )
				{
					MessageBox( GetActiveWindow(), "프리 베타 서버로 접속하려면 홈페이지에서 꼭 패치파일을 다운 받아야 합니다. \n http://www.dr-online.co.kr", "Information", MB_OK | MB_ICONINFORMATION );
					exit(0);
				}
				else fclose( fp );

				DeleteBetaFile( "./delete_dir.txt" );
			}	//< CSD-031030
		}
		else
		{
			checkbeta='0';
		}
	}

	if(checkbeta=='1')	//베타다.. 공짜 좋하는놈들.. 
	{
		char save_name[_MAX_PATH];
		char result_str[_MAX_PATH];
		strcpy( save_name, name );
		char *work_name = save_name;
		char *temp;
		if( *GetCurrentWorkingDirectory() )
		{			
			temp = strstr( save_name, GetCurrentWorkingDirectory() );
			if( temp ) 
			{
				work_name = temp + strlen( GetCurrentWorkingDirectory() )+1;
			}
		}
		char *conv_slash = work_name;
		while( conv_slash = strchr( conv_slash, '\\' ) )
		{
			*conv_slash = '/';
		}		

		temp = strstr( work_name, "./" );
		if( temp ) work_name += 2;

		char *token = strstr( work_name, "/" );
		if( token )		// / 가 하나도 없는 경우를위해서.. 
		{	//< CSD-031030
			*token=0;
			token++;
			sprintf( result_str, "%s_b5/%s", work_name, token );
			fp = fopen( result_str, filemode );
			if( fp ) return fp;
		#ifdef _DEBUG
			else
			{
				SaveBetaFileName( result_str );
			}
		#endif
		}	//> CSD-031030
	}

	return fopen( name, filemode );
}	
/*────────────────────────────────────────
함수명 : void MakeFullPathName( char *d, char *path, char *filename )
설  명 : 받은 인자로 full filename을 만든다.
인  자 : char *d : 생성될 화일이름 저장  Pointer ( size는 FILENAME_MAX== 80 ( stdio.h) ).
         char *path : "./data/"와 같은 형태의 String.
         char *filename: "han.fnt"와 같은 형태의 String.
리  턴 : 없슴.
────────────────────────────────────────*/
void MakeFullPathName( char *d, char *path, char *filename )
{
   memset( d, 0, FILENAME_MAX );
   strcpy( d, path );
   strcat( d, filename );

   if( strlen( d ) >= FILENAME_MAX ) Error( "too long [%s %d]\n", __FILE__, __LINE__ );
}




/*────────────────────────────────────────
함수명 : void MakeFullPathNameCD( char *d, char drive, char *path, char *filename )
설  명 : Full File Name을 만든다.
인  자 : char *d     : 생성될 화일이름 저장  Pointer ( size는 FILENAME_MAX== 80 ( stdio.h) ).
         char drive  : 드리이브 명.( 'D'또는 'd' )
         char *path  : ":/data/"와 같은 형태의 String.
         char *filename: "han.fnt"와 같은 형태의 String.
리  턴 :
────────────────────────────────────────*/
void MakeFullPathNameCD( char *d, char drive, char *path, char *filename )
{
	memset( d, 0, FILENAME_MAX );
	if( SysInfo.cddata == 1 )
	{
		*d= drive;
		strcat( d, path );
		strcat( d, filename );
	    if( strlen( d ) >= FILENAME_MAX ) Error( "too long [%s %d]\n", __FILE__, __LINE__ );
	}
	else
	{
		strcpy( d, "./CD1" );
		strcat( d, path+1 );
		strcat( d, filename );
	}
}


/*────────────────────────────────────────
함수명 : void MakeFullPathNameWithNum( char *d, char *path, char *filename,... )
설  명 : 가변인자를 받아 Full File Name을 만든다.
인  자 : char *d     : 생성될 화일이름 저장  Pointer ( size는 FILENAME_MAX== 80 ( stdio.h) ).
         char drive  : 드리이브 명.( 'D'또는 'd' )
         char *path  : ":/data/"와 같은 형태의 String.
         char *filename: "han%03d.fnt"와 같은 형태의 String.
리  턴 :
────────────────────────────────────────*/
void MakeFullPathNameWithNum( char *d, char *path, char *filename,... )
{
   char Str[ FILENAME_MAX];
   va_list val;

   va_start( val, filename);
   vsprintf(Str, filename, val);
   va_end(val);

   MakeFullPathName( d, path, Str );
}




/*────────────────────────────────────────
함수명 : void MakeFullPathNameWithNumCD( char *d, char drive, char *path, char *filename,... )
설  명 : 가변인자를 받아 Full File Name을 만든다.
인  자 : char *d     : 생성될 화일이름 저장  Pointer ( size는 FILENAME_MAX== 80 ( stdio.h) ).
         char drive  : 드리이브 명.( 'D'또는 'd' )
         char *path  : ":/data/"와 같은 형태의 String.
         char *filename: "han%03d.fnt"와 같은 형태의 String.
리  턴 :
────────────────────────────────────────*/
void MakeFullPathNameWithNumCD( char *d, char drive, char *path, char *filename,... )
{
   char Str[ FILENAME_MAX];
   va_list val;

   va_start( val, filename);
   vsprintf(Str, filename, val);
   va_end(val);

   MakeFullPathNameCD( d, drive,path, Str );
}

/*────────────────────────────────────────
함수명 : int Distance( int sx, int sy, int dx, int dy )
설  명 : 두점사이의 거리의 제곱을 리턴한다.
인  자 :
리  턴 :
────────────────────────────────────────*/
int Distance( int sx, int sy, int dx, int dy )
{
   int ddx = abs( dx - sx );
   int ddy = abs( dy - sy );

   return ddx * ddx + ddy * ddy;
}

void JustMsg( const char *s, ... )
{	
    char temp[ FILENAME_MAX];
    va_list arg;
    va_start( arg, s );
    vsprintf( temp, s, arg );
    va_end( arg );
	HWND hwnd = GetActiveWindow();
	
	if(g_DirectDrawInfo.lpDirectDraw )
		g_DirectDrawInfo.lpDirectDraw->FlipToGDISurface();
	
	MessageBox( hwnd, temp, "Information", MB_OK | MB_ICONINFORMATION );
}	
	
int BoxAndDotCrash( int dx, int dy, int dxl, int dyl, int x, int y )
{
	// BoxOneColor( dx + IMapX, dy+ IMapY, dxl, dyl, 255);
	if( dx <= x && x <= dx + dxl  ) if( dy <= y && y <= dy+dyl ) return 1;
	return 0;
}	
int BoxAndBoxCrash( int ax, int ay, int axl, int ayl,  int bx, int by, int bxl, int byl )
{	
	if( ax + axl >= bx && bx + bxl >= ax &&	ay + ayl >= by && by + byl >= ay ) return 1;
	return 0;
}	

/////////////////////////////// 이규훈 1025 수정 /////////////////////////////////////////////	
int BoxAndDotCrash( int dx, int dy, int dxl, int dyl, int x, int y, int dz, int dzl, int z)
{
	// BoxOneColor( dx + IMapX, dy+ IMapY, dxl, dyl, 255);
	if( dx <= x && x <= dx + dxl  ) if( dy <= y && y <= dy+dyl )
		if(dz<=z && z<=dz+dzl)	return 1;		//높이값 체크		
		else					return 0;
	return 0;
}	
	
	
int BoxAndBoxCrash( int ax, int ay, int axl, int ayl,  int bx, int by, int bxl, int byl, int az, int azl, int bz, int bzl )
{	
	
	if( ax + axl >= bx && bx + bxl >= ax &&	ay + ayl >= by && by + byl >= ay ) 
		if(az+azl >= bz && bz+bzl>= az)		return 1;		//높이값 체크
		else								return 0;
	return 0;
}	
/////////////////////////////////////////////////////////////////////////////////////////////	
// ------------------------------------------------------------------
int ViewCheckRoutine( const int t )
{	
	return 0;
#ifdef _DEBUG

	static int TimeCheckCount;
	static int TickCount, OldTickCount, TickFlag;
	static int currentfps;
	
	if( t == 0 ) TickFlag = 0;
	if( TickFlag == 0 )	{	OldTickCount = TickCount = ::timeGetTime();		TickFlag = 1; 	}
	else
	{
		OldTickCount= TickCount;
		TickCount	= ::timeGetTime();
	}
	char temp[ FILENAME_MAX];
	sprintf( temp, "%d : %d \n", t, TickCount - OldTickCount );
	OutputDebugString( temp );
	return OldTickCount - TickCount;

#else

	return 0;

#endif
}					

int Determinant( int ax, int ay, int bx, int by, int X, int Y)
{
	int ret = ( ax * ( by - Y ) - ay * ( bx - X ) + bx * Y - (by * X ));
	return ret;
}


int IsInBox( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x, int y )
{
		if( Determinant( x1,y1,x2,y2,x,y ) > 0 )
		if( Determinant( x2,y2,x3,y3,x,y ) > 0 )
		if( Determinant( x3,y3,x4,y4,x,y ) > 0 )
		if( Determinant( x4,y4,x1,y1,x,y ) > 0 )
			return 1;
	return 0;
}

static inline void swap( int  *a, int *b )
{
	int t;

	t = *a;
	*a = *b;
	*b = t;	
}			
			
			
int CheckIntersect(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y, int p4x, int p4y)
{			
    long m;	
    float x;

	if( p1x == p2x ) p1x++;
	if( p1y == p2y ) p1y++;
	if( p3x == p4x ) p3x++;
	if( p3y == p4y ) p3y++;

    if (p1x > p2x)
	{		
	swap(&p1x, &p2x);
	swap(&p1y, &p2y);
	}		
    if (p3x > p4x)
	{		
	swap(&p3x, &p4x);
	swap(&p3y, &p4y);
	}		
    m = (p4x - p3x)*(p2y - p1y) - (p4y - p3y)*(p2x - p1x);
			
    x = ((float)(p3y - p1y)*(p2x - p1x)*(p4x - p3x)
	 + (float)p1x*(p2y - p1y)*(p4x - p3x)
	 - (float)p3x*(p4y - p3y)*(p2x - p1x)) / (float)m;
    if (p1x <= x && x <= p2x && p3x <= x && x <= p4x) return 1;
    else return -1;
}			

void SaveAliasHeader( int xl, int yl, FILE *fp )
{
	unsigned char tt[10]={ 0x02, 0x80, 0x01, 0xe0, 0x00, 0x00, 0x01,  0xdf, 0x00, 0x18};
	fwrite( tt, 10, 1, fp );
}	
	
	
void CaptureScreen ( void )
{	
	char temp[FILENAME_MAX];
	unsigned char r,g,b;
	FILE *fp;
	int c;

	fp =Fopen( "capturescreen.tmp", "rt" );
	if( fp )
	{
		fscanf( fp, "%d", &c );
		fclose(fp);
	}
	else
	{
		c = 0;
	}

	sprintf( temp,"save%04d.img", c);
	fp = Fopen( temp, "wb" );
	if( fp )
	{
		SaveAliasHeader( 640, 480, fp);
		for( int i = 0 ; i < SCREEN_HEIGHT ; i ++)
		{
			for( int j = 0 ; j < SCREEN_WIDTH * 2 ; j +=2)
			{
				if( _PixelInfo.BitMaskR != 0x7C00) // 555가 아니면.
				{
					r = (0xf800 & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))>> 8;
					g = (0x07c0 & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))>> 3;
					b = (0x001f & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))<< 3;
				}
				else 
				{
					r = (0x7c00 & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))>> 7;
					g = (0x03e0 & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))>> 2;
					b = (0x001f & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))<< 3;
				}
				fputc( 1, fp);
				fwrite( &b, 1, 1, fp);
				fwrite( &g, 1, 1, fp);
				fwrite( &r, 1, 1, fp);
			}
			//fwrite( CTempvRamBuf + i * DxSize , ScreenXSize * ColorByte, 1, fp);
		}
		fclose(fp);
	}	
		
	c++;
	fp =Fopen( "capturescreen.tmp", "wt" );
	if( fp )
	{	
		fprintf( fp, "%d", c );
		fclose(fp);
	}	
}		
		
		
//31536000	= 1 year.
//2592000		= 1 month.
//86400		= 1 day.
//3600		= 1 hour.
//60			= 1 min.
		

void Log( char *msg, char *name, ... )
{
#ifdef _DEBUG
	
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{

		char temp[ FILENAME_MAX];
		va_list va;

		va_start( va, name );
		vsprintf( temp, name, va );
		va_end( va );
		FILE *fp;

		fp = Fopen( msg,  "at+" );
		if( fp )
		{
			fprintf(fp, "%s\n", temp  );
			fclose(fp);
		}			
	}				
#endif				
}					
					
					
///////////////////// 0525 lkh 수정 ///////////////////
void	ReturnCircleResult(int s_X, int s_Y, int e_X, int e_Y, float &result_X, float &result_Y, int angle)
{					
	//int		result_X, result_Y;
	//수정좌표 셋팅(시작점을 원점으로 놓고)
	double radian = (double )angle * 3.141592 / 180.;
					
	e_X = e_X - s_X;
	e_Y = e_Y - s_Y;
					
	result_X = (float)( cos(radian)*e_X-sin(radian)*e_Y);
	result_Y = (float)(-sin(radian)*e_X+cos(radian)*e_Y);
					
	result_X += s_X;
	result_Y += s_Y;
					
	//sprintf(temp, "%3.1f-%3.1f", result_X, result_Y);
	//MessageBox(NULL, temp, 0, MB_OK);
}					

int ReturnFileNumber( char *sou )
{
	char temp[FILENAME_MAX];
	WIN32_FIND_DATA		FindFileData;
	HANDLE				hFile;
	int nCount = 0;

	
	sprintf( temp, "%s*.*", sou );
	
	if ( ( hFile = FindFirstFile( temp, &FindFileData ) ) != INVALID_HANDLE_VALUE )
	{
		if ( FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
		{
			nCount++;
		}
	}
	while( FindNextFile( hFile, &FindFileData ) )
	{
		if ( FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
		{
			nCount++;
		}
	}
	FindClose( hFile );

	return nCount;
}

	
void Dir2DirCopy( char *sou, char *dest )
{	
	WIN32_FIND_DATA		FindFileData;
	HANDLE				hFile;
	int nCount = 0;

	char tempsou[ FILENAME_MAX];
	char tempdest[ FILENAME_MAX];
	char temp[FILENAME_MAX];
	
	sprintf( temp, "%s*.*", sou );
	
	if ( ( hFile = FindFirstFile( temp, &FindFileData ) ) != INVALID_HANDLE_VALUE )
	{
		if ( FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
		{

			sprintf( tempsou, "%s%s", sou, FindFileData.cFileName );
			sprintf( tempdest, "%s%s", dest, FindFileData.cFileName );

			SetFileAttributes( tempdest, FILE_ATTRIBUTE_NORMAL );
			CopyFile( tempsou, tempdest, FALSE );
			SetFileAttributes( tempsou, FILE_ATTRIBUTE_NORMAL );
			DeleteFile( tempsou);

			nCount++;
		}
	}
	while( FindNextFile( hFile, &FindFileData ) )
	{
		if ( FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
		{
			sprintf( tempsou, "%s%s", sou, FindFileData.cFileName );
			sprintf( tempdest, "%s%s", dest, FindFileData.cFileName );
			SetFileAttributes( tempdest, FILE_ATTRIBUTE_NORMAL );
			CopyFile( tempsou, tempdest, FALSE );
			SetFileAttributes( tempsou, FILE_ATTRIBUTE_NORMAL );
			DeleteFile( tempsou);

			nCount++;
		}
	}
	FindClose( hFile );
}

extern bool CheckTempChar( char ch, char *white );
void DeleteDir( char *dir )
{	
	WIN32_FIND_DATA		FindFileData;
	HANDLE				hFile;

	char tempsou[ FILENAME_MAX];
	char temp[FILENAME_MAX];
	
	sprintf( temp, "%s/*.*", dir );
	
	if ( ( hFile = FindFirstFile( temp, &FindFileData ) ) != INVALID_HANDLE_VALUE )
	{
		do 
		{
			if( CheckTempChar( FindFileData.cFileName[0], " .") ) continue;
			sprintf( tempsou, "%s/%s", dir, FindFileData.cFileName );

			if ( FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
			{				
				SetFileAttributes( tempsou, FILE_ATTRIBUTE_NORMAL );
				DeleteFile( tempsou);
			}
			else
			{
				DeleteDir( tempsou );
			}
		}while( FindNextFile( hFile, &FindFileData ) );
		FindClose( hFile );
	}
	int ret = RemoveDirectory( dir );
}

/*--------------------------------------------------------------------------------------------------------------------

기 능 : 사용되는 Language에 따라 
결 과 : 한글은 1, 중국어 2, 일어 3 을 리턴한다. 
		그외에는 -1를 리턴한다. 
		영어도 -1를 리턴한다. 


	LANGID langid = GetSystemDefaultLangID();

		0x0000 Language Neutral 
		0x0400 Process Default Language 
		0x0436 Afrikaans 
		0x041c Albanian 
		0x0401 Arabic (Saudi Arabia) 
		0x0801 Arabic (Iraq) 
		0x0c01 Arabic (Egypt) 
		0x1001 Arabic (Libya) 
		0x1401 Arabic (Algeria) 
		0x1801 Arabic (Morocco) 
		0x1c01 Arabic (Tunisia) 
		0x2001 Arabic (Oman) 
		0x2401 Arabic (Yemen) 
		0x2801 Arabic (Syria) 
		0x2c01 Arabic (Jordan) 
		0x3001 Arabic (Lebanon) 
		0x3401 Arabic (Kuwait) 
		0x3801 Arabic (U.A.E.) 
		0x3c01 Arabic (Bahrain) 
		0x4001 Arabic (Qatar) 
		0x042b Windows 2000: Armenian. This is Unicode only. 
		0x044d Windows 2000: Assamese. This is Unicode only. 
		0x042c Azeri (Latin) 
		0x082c Azeri (Cyrillic) 
		0x042d Basque 
		0x0423 Belarussian 
		0x0445 Windows 2000: Bengali. This is Unicode only. 
		0x0402 Bulgarian 
		0x0455 Burmese 
		0x0403 Catalan 
		0x0404 Chinese (Taiwan) 
		0x0804 Chinese (PRC) 
		0x0c04 Chinese (Hong Kong SAR, PRC) 
		0x1004 Chinese (Singapore) 
		0x1404 Chinese (Macau SAR) 
		0x041a Croatian 
		0x0405 Czech 
		0x0406 Danish 
		0x0413 Dutch (Netherlands) 
		0x0813 Dutch (Belgium) 
		0x0409 English (United States) 
		0x0809 English (United Kingdom) 
		0x0c09 English (Australian) 
		0x1009 English (Canadian) 
		0x1409 English (New Zealand) 
		0x1809 English (Ireland) 
		0x1c09 English (South Africa) 
		0x2009 English (Jamaica) 
		0x2409 English (Caribbean) 
		0x2809 English (Belize) 
		0x2c09 English (Trinidad) 
		0x3009 English (Zimbabwe) 
		0x3409 English (Philippines) 
		0x0425 Estonian 
		0x0438 Faeroese 
		0x0429 Farsi 
		0x040b Finnish 
		0x040c French (Standard) 
		0x080c French (Belgian) 
		0x0c0c French (Canadian) 
		0x100c French (Switzerland) 
		0x140c French (Luxembourg) 
		0x180c French (Monaco) 
		0x0437 Windows 2000: Georgian. This is Unicode only. 
		0x0407 German (Standard) 
		0x0807 German (Switzerland) 
		0x0c07 German (Austria) 
		0x1007 German (Luxembourg) 
		0x1407 German (Liechtenstein) 
		0x0408 Greek 
		0x0447 Windows 2000: Gujarati. This is Unicode only. 
		0x040d Hebrew 
		0x0439 Windows 2000: Hindi. This is Unicode only. 
		0x040e Hungarian 
		0x040f Icelandic 
		0x0421 Indonesian 
		0x0410 Italian (Standard) 
		0x0810 Italian (Switzerland) 
		0x0411 Japanese 
		0x044b Windows 2000: Kannada. This is Unicode only. 
		0x0860 Kashmiri (India) 
		0x043f Kazakh 
		0x0457 Windows 2000: Konkani. This is Unicode only. 
		0x0412 Korean 
		0x0812 Korean (Johab) 
		0x0426 Latvian 
		0x0427 Lithuanian 
		0x0827 Lithuanian (Classic) 
		0x042f Macedonian 
		0x043e Malay (Malaysian) 
		0x083e Malay (Brunei Darussalam) 
		0x044c Windows 2000: Malayalam. This is Unicode only. 
		0x0458 Manipuri 
		0x044e Windows 2000: Marathi. This is Unicode only. 
		0x0861 Windows 2000: Nepali (India). This is Unicode only. 
		0x0414 Norwegian (Bokmal) 
		0x0814 Norwegian (Nynorsk) 
		0x0448 Windows 2000: Oriya. This is Unicode only. 
		0x0415 Polish 
		0x0416 Portuguese (Brazil) 
		0x0816 Portuguese (Standard) 
		0x0446 Windows 2000: Punjabi. This is Unicode only. 
		0x0418 Romanian 
		0x0419 Russian 
		0x044f Windows 2000: Sanskrit. This is Unicode only. 
		0x0c1a Serbian (Cyrillic) 
		0x081a Serbian (Latin) 
		0x0459 Sindhi 
		0x041b Slovak 
		0x0424 Slovenian 
		0x040a Spanish (Traditional Sort) 
		0x080a Spanish (Mexican) 
		0x0c0a Spanish (Modern Sort) 
		0x100a Spanish (Guatemala) 
		0x140a Spanish (Costa Rica) 
		0x180a Spanish (Panama) 
		0x1c0a Spanish (Dominican Republic) 
		0x200a Spanish (Venezuela) 
		0x240a Spanish (Colombia) 
		0x280a Spanish (Peru) 
		0x2c0a Spanish (Argentina) 
		0x300a Spanish (Ecuador) 
		0x340a Spanish (Chile) 
		0x380a Spanish (Uruguay) 
		0x3c0a Spanish (Paraguay) 
		0x400a Spanish (Bolivia) 
		0x440a Spanish (El Salvador) 
		0x480a Spanish (Honduras) 
		0x4c0a Spanish (Nicaragua) 
		0x500a Spanish (Puerto Rico) 
		0x0430 Sutu 
		0x0441 Swahili (Kenya) 
		0x041d Swedish 
		0x081d Swedish (Finland) 
		0x0449 Windows 2000: Tamil. This is Unicode only. 
		0x0444 Tatar (Tatarstan) 
		0x044a Windows 2000: Telugu. This is Unicode only. 
		0x041e Thai 
		0x041f Turkish 
		0x0422 Ukrainian 
		0x0420 Urdu (Pakistan) 
		0x0820 Urdu (India) 
		0x0443 Uzbek (Latin) 
		0x0843 Uzbek (Cyrillic) 
		0x042a Vietnamese 
--------------------------------------------------------------------------------------------------------------------*/
int ReturnLanguageID( void )
{
	LANGID langid = GetSystemDefaultLangID();

	switch( langid )
	{
	case 0x0412 ://Korean 
	case 0x0812 ://Korean (Johab) 
		return 1;

	case 0x0404 ://Chinese (Taiwan)	
	case 0x0804 ://Chinese (PRC)	
	case 0x0c04 ://Chinese (Hong Kong SAR, PRC) 
	case 0x1004 ://Chinese (Singapore) 
	case 0x1404 ://Chinese (Macau SAR) 
		return 2;

	case 0x0411 ://Japanese 
		return 3;

	}

	return -1;
}



// 010314 KHS  Blocking the Accelator 

#define WV_NT_		2
#define WV_9X_		1
#define WV_ETC_		0

int GetWindowVersion( void )
{
	DWORD dwVersion = GetVersion();
	
	// Get the Windows version.
	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));
	
	// Get the build number.
	
	if (dwVersion < 0x80000000)              // Windows NT/2000, Whistler
		return WV_NT_;
	else if (dwWindowsMajorVersion < 4)      // Win32s
		return WV_ETC_;
	else                                     // Windows 95/98/Me
		return WV_9X_;
}	
	

#pragma optimize( "agpswy", on )


#define FREE_INT_NO 5	
void Ring0()
{	//在Windows9x下进入ring0后进行的操作
	__asm
	{
		cli
		mov al, 34h
		out 43h, al	//写入8253控制寄存器，设置写0号定时器
		mov ax, bx
		out 40h, al	//写定时值低位
		mov al, ah
		out 40h, al	//写定时值高位
		sti
		iretd;
	}
}



void SetClock9x(int freq)
{
	union Function_Pointer
	{
		void(*pointer)();
		char bytes[sizeof(void *)];
	}OldIntAddress, NewIntAddress;

	int IDTAddress;		//IDT表基地址
	int IDTItemAddress;	//要修改的中断门所在地址
	char *Pointer;		//要修改的中断门所在地址，指针形式

	__asm
	{
		push eax
		sidt[esp - 2]
		pop eax
		mov IDTAddress, eax	//得到IDT表基地址
	}

	IDTItemAddress = FREE_INT_NO * 8 + IDTAddress;
	Pointer = (char *)IDTItemAddress;
	NewIntAddress.pointer = Ring0;

	OldIntAddress.bytes[0] = Pointer[0];
	OldIntAddress.bytes[1] = Pointer[1];
	OldIntAddress.bytes[2] = Pointer[6];
	OldIntAddress.bytes[3] = Pointer[7];	//保存旧的中断门

	Pointer[0] = NewIntAddress.bytes[0];
	Pointer[1] = NewIntAddress.bytes[1];
	Pointer[6] = NewIntAddress.bytes[2];
	Pointer[7] = NewIntAddress.bytes[3];	//设置新的中断门

	__asm
	{
		mov ebx, freq
		int FREE_INT_NO			//产生中断，进入ring0
	}

	Pointer[0] = OldIntAddress.bytes[0];
	Pointer[1] = OldIntAddress.bytes[1];
	Pointer[6] = OldIntAddress.bytes[2];
	Pointer[7] = OldIntAddress.bytes[3];	//恢复旧的中断门
}

void CurrentGameDate( DWORD t, int *y, int *mo, int *d, int *h, int *mi, int *sec )
{
	DWORD rest;

	*y		= t / 31536000;		// 0
	rest	= t % 31536000;

	*mo		= rest / 2592000;   // +1
	rest	= rest % 2592000;

	*d		= rest / 86400;		// +1
	rest	= rest % 86400;

	*h		= rest / 3600;		//  0 
	rest	= rest % 3600;

	*mi		= rest / 60;		//  0
	*sec	= rest % 60;
}
