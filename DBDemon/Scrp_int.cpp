
#define __SCRIPT_C__

#include "StdAfx.h"
#include "Main.h"


#include "scrp_int.h"
#include "scrp_exe.h"
#include "Winzs.h"


int  CLoadScript;  //LOAD_SCRIPT;
int  TotalFunction;                           // 총 함수의 갯수를 저장
static char *prog, *oldprog;                    // 스크립트 데이타를 저장할 버퍼
static int CALLSTACK[ 10 ];                     // call문 실행시 주소를 저장할 버퍼
static int CALLSP;                              // call문을 표시하는 스택 포인터변수
int func_count;
int func_off[ 100 ];


int  NScriptNo;



/*耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?
 함수명 : Incode
 인  자 : s = 암호화할 문자열
 리턴값 : 없음
 설  명 : s문자열을 받아서 내부에 정의된 수치와 연산을 수행해서 암호화함
耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?*/
void Incode( char *s )
{
    int i = 0;
    while( *s )
    {
        *s++ ^= 0xff;	if( ++i > 19 ) i = 0;
    }
}


/* 耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?
 함수명 : GetByte
 인  자 : 없음
 리턴값 : 1바이트의 값
 설  명 : prog에 기록된 내용에서 1바이트를 리턴하고 prog포인터를 1증가
耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?*/
BYTE GetByte( void )
{
    return( *prog++ );
}


/*耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?
 함수명 : GetInt
 인  자 : 없음
 리턴값 : 4바이트의 값
 설  명 : prog에 기록된 내용에서 4바이트를 리턴하고 prog포인터를 4증가
耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?*/
int GetInt( void )
{
    int i, j;
	
    i = GetByte();
    j = *((int *)prog);

    prog += sizeof( int );

    if( i == variable_ )
        j = var[EventPC][j];

    return( j );
}



static void ChangeName( char *buf, int size, char *oldname, char *newname )
{
	for( int i = 0 ; i < size - (int)strlen( oldname ) ; i ++)
	{
		if( strncmp( buf + i, oldname, strlen( oldname )) == NULL )
		{
			for( int j = 0 ; j < (int)strlen( oldname ) ; j ++)
			{
				*( buf+i + j ) = newname[j];
			}
		}
	}
}


/* 耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?
 함수명 : GetString
 인  자 :
 리턴값 : 문자열을 저장하고 있는 배열의 시작 포인터
 설  명 : 스크립트 데이타에서 문자열을 읽어들인다.
耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?*/
//수정7
void GetString( char *buf )
{
    int  size;

    size = GetInt();
    memcpy( buf, prog, size );
    prog += size;
    buf[ size ] = 0;
    Incode( buf );
}


static char GetByte1( void )
{
    return( *prog++ );
}


static int GetInt1( void )
{
    int i;

    i = *((int *)prog);
    prog += sizeof( int );

    return( i );
}


/* 耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?
 함수명 : If
 인  자 : 없음
 리턴값 : 없음
 설  명 : if문에 해당하는 내용을 실행한다.
耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?*/
void If( void )
{
    int i1, i2;
    BYTE com;
    int check = 0;
	int fuck=0;

    GetByte1();                      // 변수 연산자를 읽음
    i1 = GetInt1();                  // Lvalue의 값을 얻는다
    i1 = var[EventPC][ i1 ];                // var배열에서 해당하는 값을 얻는다

    com = GetByte1();                // 연산기호를 얻는다

	fuck = GetByte1() & 0x000000ff;


    if( fuck == variable_ )    // Rvalue의 값이 변수라면
    {
        i2 = GetInt1();
        i2 = var[EventPC][ i2 ];
    }
    else                            // Rvalue의 값이 수치라면
    {
        i2 = GetInt1();
    }

    switch( com )
    {
        case 0:
            if( i1 < i2 ) check = 1;    break;
        case 1:
            if( i1 <= i2 ) check = 1;   break;
        case 2:
            if( i1 > i2 ) check = 1;    break;
        case 3:
            if( i1 >= i2 ) check = 1;   break;
        case 4:
            if( i1 != i2 ) check = 1;   break;
        case 5:
            if( i1 == i2 ) check = 1;   break;
    }

    if( check == 1 )        // 조건이 참이라면
        prog += ( sizeof( int ) + 1 );          // 다음의 goto문을 스킵
}




/* 耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?
 함수명 : Cal
 인  자 : 없음
 리턴값 : 없음
 설  명 : 사칙연산, %, 간단한 비트연산등을 수행한다
耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?*/
void Cal( int com )
{
    int i1, i2, i3, result;
    int *p2, *p3;
    BYTE op;

    GetByte1();              // 변수 연산자를 얻음
    i1 = GetInt1();          // Lvalue를 얻음

    op = GetByte1() & 0x000000ff;
    i2 = GetInt1();
    if( op == variable_ )   // 변수라면
    {
        p2 = &(var[EventPC][ i2 ]);
    }
    else                    // 수치라면
    {
        p2 = &i2;
    }


    op = GetByte1() & 0x000000ff;
    i3 = GetInt1();
    if( op == variable_ )   // 변수라면
    {
        p3 = &var[EventPC][ i3 ];
    }
    else                    // 수치라면
    {
        p3 = &i3;
    }

    switch( com )                        // 각 연산자에 따라 실행
    {
        case add_:
            result = (*p2) + (*p3); break;
        case sub_:
            result = (*p2) - (*p3); break;
        case mul_:
            result = (*p2) * (*p3); break;
        case div_:
            result = (*p2) / (*p3); break;
        case rest_:
            result = (*p2) % (*p3); break;
        case and_:
            result = (*p2) & (*p3); break;
        case or_:
            result = (*p2) | (*p3); break;
    }
    var[EventPC][ i1 ] = result;                      // 실행한 결과를 배열에 저장
}



void Cal2( int com )
{
    int i1, i2;
    int *p2;
    BYTE op;

    GetByte1();
    i1 = GetInt1();                          // Lvalue 값을 얻음

    op = GetByte1() & 0x000000ff;
    i2 = GetInt1();                          // Rvalue 값을 얻음
    if( op == variable_ )   // 변수라면
    {
        p2 = &var[EventPC][ i2 ];
    }
    else                    // 수치라면
    {
        p2 = &i2;
    }

    switch( com )
    {
        case add2_: var[EventPC][ i1 ] += (*p2); break;
        case sub2_: var[EventPC][ i1 ] -= (*p2); break;
        case mul2_: var[EventPC][ i1 ] *= (*p2); break;
        case div2_: var[EventPC][ i1 ] /= (*p2); break;
        case rest2_: var[EventPC][ i1 ] %= (*p2); break;
        case or2_ : var[EventPC][ i1 ] |= (*p2); break;
        case and2_: var[EventPC][ i1 ] &= (*p2); break;
    }
}



/* 耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?
 함수명 : Equal
 인  자 : 없음
 리턴값 : 없음
 설  명 : 변수의 대입문을 처리
耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?*/
void Equal( void )
{
    int i1, i2;
    BYTE op;

    GetByte1();
    i1 = GetInt1();

    op = GetByte1();
    i2 = GetInt1();
    if( op == variable_ )
        i2 = var[EventPC][ i2 ];

    var[EventPC][ i1 ] = i2;
}



/* 耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?
 함수명 : IfEqual
 인  자 : 없음
 리턴값 : 없음
 설  명 : 스위치문에 해당하는 내용을 처리
耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?*/
void IfEqual( void )
{
    int i1, i2;
    char op;

    GetByte1();
    i1 = GetInt1();                         // Lvalue 값을 읽음

    op = GetByte1();                        // Rvalue 값을 읽음
    i2 = GetInt1();

    if( op == value_ )
    {
        if( var[EventPC][i1] == i2 )                   // 만약 조건이 참이라면 분기
            prog = oldprog + *((int *)prog);
        else                                  // 거짓이면 다음문을 실행
            prog += sizeof( int );
    }
    else
    {
        if( var[EventPC][i1] == var[EventPC][i2] )                   // 만약 조건이 참이라면 분기
            prog = oldprog + *((int *)prog);
        else                                  // 거짓이면 다음문을 실행
            prog += sizeof( int );
    }
}



/* 耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?
 함수명 : Script
 인  자 : 없음
 리턴값 : 없음
 설  명 : 스크립트 주 실행기
耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?*/
void Script( int num )
{
    BYTE comm;
	int Fuck;



	if( CLoadScript == SCRIPT_OLD && num == 0 )	goto RET_; // 실행한것을 또할필요는 없지요...

    if( num >= TotalFunction )  goto RET_;

    NScriptNo = num;
	CLoadScript = SCRIPT_OLD;

	

    prog = oldprog + func_off[ num ];
    while( 1 )
    {
        comm = GetByte1();

        switch( comm )
        {
            case goto_:
				Fuck = *(int *)prog;
				//prog += 4;
                prog = oldprog + Fuck; 
				break;

            case if_:
                If(); break;

            case call_:
                CALLSTACK[ CALLSP++ ] = prog - oldprog + sizeof( int );
                prog = oldprog + *((int *)prog);
                break;

            case add_:
            case sub_:
            case mul_:
            case div_:
            case rest_:
            case and_:
            case or_:
                Cal( comm ); break;

            case add2_:
            case sub2_:
            case mul2_:
            case div2_:
            case rest2_:
            case and2_:
            case or2_:
                Cal2( comm ); break;

            case equal_:
                Equal(); break;

            case end_:
                if( --CALLSP < 0 )
                {
                    CALLSP = 0;
                    return;
                }
                prog = oldprog + CALLSTACK[ CALLSP ];
                break;

            case ifequal_:
                IfEqual(); break;

            default:        // 사용자 정의 함수를 실행

                (*SFunction[ comm ])();


                if( CLoadScript == SCRIPT_NEW )
                {
                  CALLSP = 0;
                  goto RET_;
                }
        }
    }

RET_:
	return;
}


/*
// 스크립트 텍스트 파일을 읽어들인다.
int LoadScriptText( char *scrname )
{
	char temp[FILENAME_MAX];
	char cwd[ FILENAME_MAX];

	getcwd( cwd, FILENAME_MAX );

	_chdir( GameStartDirectory );

	strcpy( CurScriptName, scrname );

	MakeFullPathName( temp, SCRIPT_PATH__, scrname);	// Scriptfile은 CD가 아닌 하드에서 읽는것을 기본으로 한다. 


	if( GameMakeFlag == ON )	
	{
		prog = oldprog = (char *)LoadTextScript( temp );
	}
	else 
	{
		strcat( temp, "c");
		LoadScript( temp );
	}

	CLoadScript  =  _NEW_;

	_chdir( cwd );

	return 0;
}
*/



/* 耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?
 함수명 : LoadScript
 인  자 :
 리턴값 :
 설  명 : 정상적으로 읽어들이면 1을 리턴한다.
耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶耶?*/

int LoadScriptCompiled( char *scrname )
{
    FILE *fp;
    int size, t, i;
   
    fp = fopen( scrname, "rb" );
	if( fp == NULL ) 	{		exit(0);	}

	if( oldprog != NULL )free( oldprog );

    fread( &size, 1, sizeof( int ), fp );           // 총 파일의 크기를 읽음
    fread( &TotalFunction, 1, sizeof( int ), fp );              // 함수의 갯수를 읽음

    for( i = 0; i < TotalFunction; i++ )
    {
        fread( &t, 1, sizeof( int ), fp );          // 함수의 옵셋을 읽음
        func_off[ i ] = t;
    }

	oldprog = (char *)malloc( size );
    fread( oldprog, size, 1, fp );
	prog = oldprog;
    fclose( fp );

    return( 1 );
}

int LoadScript( char *mapname )
{
 
    char scriptname[ FILENAME_MAX];

	sprintf( scriptname, "./script/%s.zs", mapname );
	
	if( 1 )
	{
		prog = oldprog = (char *)LoadTextScript( scriptname );
	}
	else 
	{
		strcat( scriptname, "c" );
		LoadScriptCompiled( scriptname );
	}
 
	CLoadScript  =  SCRIPT_NEW;

    return( 1 );
}

