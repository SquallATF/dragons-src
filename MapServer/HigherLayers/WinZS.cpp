/* 
 累己老   : 1995斥 3岿 27老
 累己磊   : 厘 籍痹
 Compile  : bcc -ms
 拳老 汲疙: 捞 拳老篮 胶农赋飘 郴侩阑 巩过利栏肺 秦籍窍绊 捞巴阑 角青扁啊
            镜荐 乐绰 郴侩狼 捞柳 拳老肺 积己茄促.
            * 葛电 窃荐疙篮 1官捞飘肺 函券等促. 弊矾骨肺 弥措 疙飞绢狼 肮
              荐绰 郴何, 寇何 疙飞绢 钦媚辑 255俺捞促.
            * 葛电 函荐绰 角力蔼苞 0x8000阑 OR秦辑 积己矫挪促.
			* 葛电 荐摹蔼篮 角力蔼阑 蝶弗促. ( 0 ~ 32767 )
            * 葛电 GOTO 疙飞狼 痢橇 林家绰 2官捞飘捞促.
            * switch巩俊辑 case巩篮 弥措 50俺捞促.


date of Modification : 1997斥 10岿 30老
who modified : Kim Hongseok
contents of modification : porting of windows
 */
#include "..\stdafx.h"
#include "DefaultHeader.h"



#define __WINZS_H__
#include "WinZs.h"

#include "Scrp_int.h"
#include "RajaSystem.h"




#ifndef __TYPEDEF__
    typedef unsigned char  BYTE;
    typedef unsigned short WORD;
    typedef unsigned long  DWORD;
#endif


#define MAX_SCR_ARRAY     10000


// 阿 疙飞绢俊 秦寸窍绰 巩磊凯狼 蔼阑 沥狼茄促.
// 促澜狼 沥狼巩篮 哪颇老等 捞柳拳老俊 角力肺 静咯瘤绰 蔼捞促.
/*
enum {
    goto_     =  0xe0,
    if_       =  0xe1,
    call_     =  0xe2,
    add_      =  0xe3,
    sub_      =  0xe4,
    mul_      =  0xe5,
    div_      =  0xe6,
    rest_     =  0xe7,
    equal_    =  0xe8,
    end_      =  0xe9,
    ifequal_  =  0xea,
    or_       =  0xeb,
    and_      =  0xec,
    variable_ =  0xed,
    value_    =  0xee,
    add2_     =  0xef,
    sub2_     =  0xf0,
    mul2_     =  0xf1,
    div2_     =  0xf2,
    rest2_    =  0xf3,
    or2_      =  0xf4,
    and2_     =  0xf5,
};
*/
// 促澜狼 沥狼巩篮 哪颇老且锭父 夸备登绰 蔼捞促.
enum {
    else_     =  0xd0,
    case_     =  0xd1,
    endswitch_=  0xd2,
    default_  =  0xd3,
    switch_   =  0xd4,
    break_    =  0xd5,
    include_  =  0xd6,
};


#define LABEL_LENGTH 20        // 扼骇狼 弥措 辨捞甫 唱鸥晨

enum {
    END = 0,
    DELIMITER,
    VARIABLE,
    NUMERIC,
    FUNCTION,
    STRING,
    LABEL,
    COMMAND,
    USERFUNC,
};


typedef struct {
    char s[ 50 + 1 ];
    int n;
} COMMSET;


typedef struct {        // if_else 巩狼 郴侩阑 历厘窍扁 困茄 备炼眉
    int off;            // if唱 else巩捞 困摹茄 镑狼 可悸
    BYTE d;             // 表捞甫 唱鸥晨
    BYTE t;             // 0 = if, 1 = else
} IF_ELSE;


typedef struct {        // 荤侩磊啊 沥狼茄 窃荐疙阑 历厘且 备炼眉
    char s[ 50 + 1 ];       // 窃荐疙阑 历厘且 函荐
    char format[ 31 ];  // 窃荐俊 秦寸窍绰 牢荐狼 format阑 历厘且 函荐
    int n;             // 窃荐疙捞 乐绰镑狼 困摹
} USER_FUNC;


typedef struct {        // switch 巩狼 郴侩阑 历厘且 备炼眉
    int total;          // 橇肺弊伐 贸澜俊 case狼 肮荐甫 炼荤茄 郴侩
    int count;
    int def;
    int numoff[ 50 ];
} SWITCHCASE;


// 阿阿俊 秦寸窍绰 疙飞绢客 弊 疙飞绢俊 秦寸窍绰 箭磊甫 啊瘤绰 备炼眉
static COMMSET comm[ 255 ] = {
    { "goto",           goto_ },
    { "if",             if_ },
    { "call",           call_ },
    { "end",            end_ },
    { "else",           else_ },
    { "switch",         switch_ },
    { "case",           case_ },
    { "endswitch",      endswitch_ },
    { "default",        default_ },
    { "break",          break_ },
    { "include",        include_ },
};


static COMMSET variable[ MAX_SCR_ARRAY ];             // 荤侩磊啊 沥狼茄 函荐狼 郴侩阑 历厘
static COMMSET label1[ MAX_SCR_ARRAY ];               // 盔贰 扼骇狼 郴侩阑 历厘窍绰 函荐
static COMMSET label2[ MAX_SCR_ARRAY ];               // goto巩 促澜俊 静牢 扼骇阑 历厘
static IF_ELSE ifelse[ MAX_SCR_ARRAY ];               // if, else巩狼 郴侩阑 历厘
static COMMSET userfunc1[ MAX_SCR_ARRAY ];            // 盔贰 窃荐疙狼 郴侩阑 历厘
static COMMSET userfunc2[ MAX_SCR_ARRAY ];            // call巩 促澜俊 静牢 窃荐甫 历厘
static USER_FUNC myfunction[ MAX_SCR_ARRAY ];           // 蜡历啊 沥狼茄 窃荐甫 历厘
static SWITCHCASE swit[ 50 ];               // 窍唱狼 switch巩俊辑 阿阿狼
                                     // case巩阑历厘

static char TOKEN[ MAX_SCR_ARRAY ];                    // 阂府秦辰 配奴阑 历厘且 滚欺
static char TOKENTYPE;                      // 掘绢柳 配奴狼 鸥涝
static int  TOK;                            // 配奴阑 函券茄 饶狼 蔼阑 历厘

static int ifsp;                            // if, else狼 胶琶蔼
static int switsp;                          // switch狼 函困蔼
static int label1_n, label2_n;              // 盔贰狼 扼骇蔼苞 静咯柳 扼骇蔼阑 历厘
static int user1_n, user2_n;                // 盔贰狼 窃荐蔼苞 静咯柳 窃荐蔼阑 历厘
static int funclabelsp;						// GetLabel () 窃荐俊辑 荤侩

static int LINE = 0;                        // 家胶拳老狼 扼牢蔼阑 墨款飘

BYTE *wzsprog,                          // 家胶 橇肺弊伐阑 佬绢甸老 滚欺
	 *oldwzsprog;                       // 贸澜俊绰 wzsprog客 鞍篮 蔼阑 啊瘤哥
									 // 橇肺弊伐 辆丰矫 free窃荐俊辑 荤侩
static BYTE *outprog,                       // 免仿 滚欺甫 啊府糯
			*oldoutprog;                    // 贸澜俊绰 oldoutprog客 鞍篮 蔼阑 啊瘤哥
                                     // 橇肺弊伐 辆丰矫 free窃荐俊辑 荤侩
static int SOURCEFILESIZE;                  // 家胶拳老狼 农扁甫 唱鸥晨
static char TARGETFILENAME[ FILENAME_MAX];             // 免仿且 拳老狼 捞抚阑 唱鸥晨
static FILE *outfile;                       // 免仿且 拳老阑 啊府虐绰 拳老 器牢磐
static int INCLUDEFILESIZE;                 // 牢努风靛瞪 拳老狼 农扁甫 啊府糯

static int ErrorCount;


static char Wz_scriptfilename[FILENAME_MAX];


// Scrp_int.cpp
extern int func_off[100];

//////////////////////////////////////////////////////////////////////////
// 角龙利牢 窃荐甸狼 沥狼
//////////////////////////////////////////////////////////////////////////




//------------------------------------------------------------------------

enum {
    ZS_FALSE,
    ZS_TRUE,
};

#define ZSAlloc(x)      ZS_Alloc( (x), __FILE__, __LINE__ )

void ZSError( char *s, ... )
{
    va_list arg;
	char temp[ FILENAME_MAX];

    va_start( arg, s );
    vsprintf( temp, s, arg );
    va_end( arg );

	MessageBox( 0, s, "ZSerror", MB_OK );

    exit( 1 );
}








char *ZS_Alloc( int size, const char *file, int line )
{
    char *memorytemp;
	memorytemp = (char *)GlobalAlloc( GMEM_FIXED, size );
    //memorytemp = (char *)malloc( size );

    if( !memorytemp )
        ZSError( "%s(%d) : \n %d alloc Err\n", file, line, size );

    return( memorytemp );
}


void ZSfree( void *t )
{
	GlobalFree( t );
//	free( t );
}



FILE *ZS_Fopen( const char *file, const char *mode )
{
    FILE *tempfile;

    tempfile = fopen( file, mode );
    if( !tempfile )
        ZSError( "%s FILE PROCESS ERROR.\n", file );

    return( tempfile );
}

//------------------------------------------------------------------------





/* 
 窃荐疙 : Incode
 牢  磊 : s = 鞠龋拳且 巩磊凯
 府畔蔼 : 绝澜
 汲  疙 : s巩磊凯阑 罐酒辑 郴何俊 沥狼等 荐摹客 楷魂阑 荐青秦辑 鞠龋拳窃
 */
void WinZsIncode( char *s )
{
//    static BYTE t[ 20 ] = {
//        101, 255, 168, 210, 184, 47, 176,  54,  58, 63,
//         78,   1,  34,  69, 200,  88,  77, 111, 48, 121
//    };
    BYTE t = 0xff;
    int i = 0;

    while( *s ) {
        (*s++) ^= 0xff;
        if( ++i > 19 ) i = 0;
    }
}


/* 
 窃荐疙 : SearchFunction
 牢  磊 : s = 茫绊磊 窍绰 窃荐狼 巩磊凯
 府畔蔼 : 茫瘤给窍搁 -1, 茫栏搁 锅龋甫 府畔
 汲  疙 : s巩磊凯俊 秦寸窍绰 蜡历啊 沥狼茄 窃荐疙阑 function硅凯俊辑 茫绰促.
 */
int SearchFunction( char *s )
{
    int i;

	for( i = 0; myfunction[i].s[0] != 0; i++ )
        if( !strcmp(myfunction[i].s, s ) ) return( i );

    return( -1 );
}


/* 
 窃荐疙 : SearchCommand
 牢  磊 : s = 茫绊磊窍绰 疙飞绢狼 巩磊凯
 府畔蔼 : 茫瘤给窍搁 -1, 茫栏搁 锅龋甫 府畔
 汲  疙 : s巩磊凯俊 秦寸窍绰 疙飞绢甫 茫澜
 */
int SearchCommand( char *s )
{
	int i;

    for( i = 0; comm[i].s[0] != 0; i++ )
        if( !strcmp( comm[i].s, s ) ) return( i );

    return( -1 );
}


/* 
 窃荐疙 : SearchVariable
 牢  磊 : s = 茫绊磊 窍绰 函荐狼 巩磊凯
 府畔蔼 : 茫瘤给窍搁 -1, 茫栏搁 锅龋甫 府畔
 汲  疙 : s巩磊凯俊 秦寸窍绰 函荐疙阑 茫澜
 */
int SearchVariable( char *s )
{
    int i;

    for( i = 0; variable[i].s[0] != 0; i++ )
		if( !strcmp( variable[i].s, s ) ) return( i );

    return( -1 );
}


/* 
 窃荐疙 : SearchLabel
 牢  磊 : s = 茫绊磊 窍绰 扼骇狼 巩磊凯
 府畔蔼 : 茫瘤给窍搁 -1, 茫栏搁 锅龋甫 府畔
 汲  疙 : s巩磊凯俊 秦寸窍绰 扼骇阑 茫澜
 */
int SearchLabel( char *s )
{
    int i;

    for( i = 0; label1[i].s[0] != 0; i++ )
        if( !strcmp( label1[i].s, s ) ) 
			return( i );

    return( -1 );
}



/* 
 窃荐疙 : SearchUserFunc
 牢  磊 : s = 茫绊磊 窍绰 蜡历啊 沥狼茄 窃荐狼 巩磊凯
 府畔蔼 : 茫瘤给窍搁 -1, 茫栏搁 锅龋甫 府畔
 汲  疙 : 家胶俊辑 沥狼茄 s巩磊凯俊 秦寸窍绰 窃荐疙阑 茫澜
          窃荐疙篮 馆靛矫 '_'肺 矫累登绢具 窃
 */
int SearchUserFunc( char *s )
{
    int i;

    for( i = 0; userfunc1[i].s[0] != 0; i++ )
        if( !strcmp( userfunc1[i].s, s ) ) return( i );

    return( -1 );
}




/* 
 窃荐疙 : Serror
 牢  磊 : x = 免仿窍绊磊 窍绰 俊矾 皋矫瘤狼 锅龋
 府畔蔼 : 绝澜
 汲  疙 : x俊 秦寸窍绰 俊矾 皋矫瘤甫 免仿窍绊 橇肺弊伐阑 辆丰茄促.
 */
void Serror( int x )
{
    char *s[] = {
        "Compile completed.",             // 0
        "Systax error",                   // 1
        "Unbalanced ()",                  // 2
        "Unbalanced {}",                  // 3
        "Too few patameter",              // 4
        "Mismatch patameter",             // 5
        "Invalid patameter",              // 6
        "Invalid Label",                  // 7
		"Too long label name",            // 8
        "Label name is not found",        // 9
        "Lvalue required",                // 10
        "Stack overflow",                 // 11
        "Mismatch if - else",             // 12
        "Mismatch function - end",        // 13
        "Name already exist",             // 14
        "Function name not found",        // 15
        "Symbol not found",               // 16
        "Data file (zs.dat) open error",  // 17
        "Numeric is too big",             // 18
        "file create error",              // 19
        "Not found variable",             // 20
        "Mismatch Switch - endswitch",    // 21
        "Variable not found",             // 22
        "File not found",                 // 23
    };

//    ZSfree( oldwzsprog );
//    ZSfree( oldoutprog );

	ErrorCount++;

	remove( TARGETFILENAME );


	char nn[ FILENAME_MAX];
	strcpy( nn, Wz_scriptfilename );
	for( int i = strlen( nn ) - 1  ; i > 0  ; i -- )	   if( *(nn + i ) == '\\' ) break;
	
//	sprintf( temp, "%s: %d line: '%s'\n\n ' %s '甫 凯鳖夸?",s[x], LINE, TOKEN,  strupr( nn+i+1) );
//	if( YesOrNo( temp, "Script Compile Error") == IDYES )	NotePad( Wz_scriptfilename );



#ifdef MAKE_EXE_FILE	
    if( x == 0 ) printf( "\n%s\n", s[0] );
    else {
        printf( "\n\aError:\n%5d LINE: '%s' %s\n", LINE, TOKEN, s[x] );
    }
	printf( "Anyway, Process?( y/N ) ");
	t = _getch();
	if( t == 'y' || t== 'Y' ) return;
	else exit(1);
#endif
 }



/* 
 窃荐疙 : Isdeli
 牢  磊 : i = 八荤且 巩磊
 府畔蔼 : 备盒巩磊捞搁 1, 酒聪搁 0
 汲  疙 : i巩磊啊 备盒巩磊牢瘤甫 八荤茄促.
 */
int Isdeli( char i )
{
    if( strchr( " ';,+-*/%#=<>(){}[]:!@\t&|", i ) )
        return( 1 );

    return( 0 );
}



/* 
 窃荐疙 : Isspace
 牢  磊 : i = 八荤且 巩磊
 府畔蔼 : 傍归, 徘, 府畔, 扼牢 乔靛巩磊捞搁 1, 酒聪搁 0
 汲  疙 : i巩磊啊 傍归, 徘, 府畔, 扼牢 乔靛巩磊牢瘤甫 八荤茄促.
 */
int Isspace( char i )
{
    switch( i ) {
        case 0x0a:        // 扼牢 乔靛 巩磊捞搁 LINE函荐甫 窍唱 刘啊茄促.
            LINE++;
        case 0x0d:
		case '\t':
        case ' ':
            return( 1 );
        default:
            return( 0 );
    }
}



/* 
 窃荐疙 : PutToken
 牢  磊 : 绝澜
 府畔蔼 : 绝澜
 汲  疙 : TOKEN硅凯狼 郴侩阑 wzsprog俊 促矫敬促.
 */
void PutToken( void )
{
    int i;

	i = strlen( TOKEN );

    while( i ) {
        *--wzsprog = TOKEN[ i - 1 ];
        i--;
    }
    if( TOKENTYPE == LABEL ) {
        *--wzsprog = ':';
    }
}



/* 
 窃荐疙 : GetToken
 牢  磊 : 绝澜
 府畔蔼 : 配奴 鸥涝阑 府畔茄促.
 汲  疙 : wzsprog滚欺甫 盒籍秦辑 配奴阑 掘绰促.
 */
int GetToken( void )
{
    register char *temp;

    // 配奴 滚欺甫 檬扁拳 茄促.
    temp = TOKEN;
    TOKENTYPE = 0;
	TOK = 0;

start:
    // 拳老狼 场俊 促促耳阑锭
    if( *wzsprog == 0 ) return( TOKENTYPE = END );

    // 傍归, 徘巩磊, 府畔, 扼牢乔靛 巩磊甫 绝矩促.
    while( Isspace( *wzsprog ) ) ++wzsprog;
    if( *wzsprog == 0 ) goto start;        // 拳老狼 场捞搁

    // 扼骇疙捞搁 TOKEN函荐俊 捞抚阑 历厘窍绊 府畔
    if( *wzsprog == ':' ) {
        if( Isdeli( *++wzsprog ) ) { Serror( 7 ); return -1; } // 扼骇疙篮 康巩磊客 箭磊肺 备己 
        while( !Isdeli(*wzsprog) && *wzsprog != '\r' && *wzsprog != 0 )
			*temp++ = *wzsprog++;
        *temp = 0;
        return( TOKENTYPE = LABEL );
    }

    // 林籍捞搁 扼牢狼 场鳖瘤甫 胶诺窍绊 促矫 贸澜栏肺 倒酒埃促.
    if( *wzsprog == ';' ) {
        ++wzsprog;
        while( *wzsprog != '\n' && *wzsprog++ != 0 );
        goto start;
    }

    // 荤侩磊 沥狼 窃荐捞搁
    if( *wzsprog == '@' ) {
        ++wzsprog;
        while( !Isdeli(*wzsprog) && *wzsprog != '\r' && *wzsprog != 0 )
            *temp++ = *wzsprog++;
        *temp = 0;
        TOK = SearchUserFunc( TOKEN );
        return( TOKENTYPE = USERFUNC );
	}

    // 备盒巩磊捞搁
    if( Isdeli( *wzsprog ) ) {
        *temp++ = *wzsprog++;
        *temp = 0;
        return( TOKENTYPE = DELIMITER );
    }

    // 巩磊凯捞搁
    if( *wzsprog == '"' ) {
        ++wzsprog;
        while( *wzsprog != '"' && *wzsprog != '\r' && *wzsprog != 0 )
            *temp++ = *wzsprog++;
        // 付瘤阜 巩磊啊 扼牢乔靛唱 拳老狼 场捞搁 俊矾
        if( *wzsprog == '\r' || *wzsprog == 0 ) { Serror( 1 ); return -1; }
        *temp = 0;
        wzsprog++;        // '"'巩磊甫 胶诺
        return( TOKENTYPE = STRING );
    }

    // 箭磊捞搁
    if( isdigit(*wzsprog) ) {
        while( isdigit(*wzsprog) && !Isdeli(*wzsprog) &&
               *wzsprog != '\r' && *wzsprog != 0 )
            *temp++ = *wzsprog++;
        *temp = 0;
        return( TOKENTYPE = NUMERIC );
    }

    // 疙飞绢唱 扼骇, 函荐甫 眉农
    if( isalpha(*wzsprog) ) {
        while( !Isdeli(*wzsprog) && *wzsprog != '\r' && *wzsprog != 0 )
            *temp++ = *wzsprog++;
        *temp = 0;

        // 父距 掘绢柳 巩磊凯捞 疙飞绢扼搁
        TOK = SearchCommand( TOKEN );
        if( TOK != -1 ) return( TOKENTYPE = COMMAND );

		// 父距 掘绢柳 巩磊凯捞 函荐扼搁
        TOK = SearchVariable( TOKEN );
        if( TOK != -1 ) return( TOKENTYPE = VARIABLE );

        // 父距 掘绢柳 巩磊凯捞 扼骇疙捞扼搁
        TOK = SearchLabel( TOKEN );
        if( TOK != -1 ) return( TOKENTYPE = LABEL );

        // 父距 掘绢柳 巩磊凯捞 荤侩磊 沥狼 窃荐疙捞扼搁
        TOK = SearchFunction( TOKEN );
        if( TOK != -1 ) return( TOKENTYPE = FUNCTION );

        // 弊寇绰 巩磊凯肺 埃林
        return( TOKENTYPE = STRING );
    }

	{ Serror( 1 ); return -1; }

     return( TOKENTYPE );
}



/* 
 窃荐疙 : WriteInt
 牢  磊 : x = outprog俊 免仿窍绊磊 窍绰 蔼
 府畔蔼 : 绝澜
 汲  疙 : x俊 秦寸窍绰 蔼阑 outprog滚欺俊 扁废茄促.
 */
void WriteInt( int x )
{
	static int count=0;
	count++;
    *((int *)outprog) = x;
    outprog += sizeof( int );
}



/* 
 窃荐疙 : WriteBYTE
 牢  磊 : x = outprog滚欺俊 免仿窍绊磊 窍绰 蔼
 府畔蔼 : 绝澜
 汲  疙 : x俊 秦寸窍绰 蔼阑 outprog滚欺俊 扁废茄促.
 */
void WriteBYTE( char x )
{
    *outprog++ = x;
}



/* 
 窃荐疙 : First
 牢  磊 : 绝澜
 府畔蔼 : 绝澜
 汲  疙 : '}' 扁龋甫 惯斑窍搁 ifelse 胶琶阑 扑窍绊 单捞鸥甫 贸府茄促.
 */
int First( void )
{
    --ifsp;

    if( ifsp < 0 ){ Serror( 3 );  return -1; }       // ifelse 胶琶捞 坷滚敲肺快 灯阑版快
    if( ifelse[ ifsp ].d != ifsp ) { Serror( 3 ); return -1; }  // 胶琶狼 表捞啊 嘎瘤臼阑 版快

    *((int *)(oldoutprog + ifelse[ifsp].off )) =
        outprog - oldoutprog;

	return 1;
}



/* 
 窃荐疙 : Inc
 牢  磊 : 绝澜
 府畔蔼 : 绝澜
 汲  疙 : 荤蘑楷魂, %, |, &, ~ 楷魂阑 荐青茄促.
 */
int Inc( void )
{
    int jj[ 3 ][ 2 ];               // 霉锅掳 梅磊绰 函荐牢瘤 箭磊牢瘤甫 唱鸥晨
    int i;                          // 函荐捞搁 0, 箭磊捞搁 1
    BYTE j;

    jj[0][0] = 0;                   // 霉锅掳 牢磊甫 掘澜, 霉锅掳 牢磊绰 公炼扒 函荐
    jj[0][1] = variable[TOK].n;

    if( GetToken() == -1 ) return -1;

    if( strchr( "+-/*%|&", *TOKEN ) )      // 刘皑楷魂磊殿老 版快
    {
        j = *TOKEN;

        if( GetToken() == -1 ) return -1;
        if( *TOKEN != '=' ){ Serror( 1 );  return -1; }

        switch( j )
        {
            case '+': j = add2_;  break;
            case '-': j = sub2_;  break;
            case '*': j = mul2_;  break;
            case '/': j = div2_;  break;
            case '%': j = rest2_; break;
            case '|': j = or2_;   break;
            case '&': j = and2_;  break;
        }
        WriteBYTE( j );

        WriteBYTE( (char)variable_ );
        WriteInt( jj[0][1] );

        if( GetToken() == -1 ) return -1;

        switch( TOKENTYPE )
        {
            case VARIABLE:
                WriteBYTE( (char)variable_ );
                WriteInt( variable[TOK].n );
                break;

            case NUMERIC:
                WriteBYTE( (char)value_ );
                WriteInt( atoi( TOKEN ) );
                break;

            default:
                {Serror( 1 );  return -1; }
        }
    }
    else
    {
        if( *TOKEN != '=' ) { Serror( 1 );  return -1; }

        if( GetToken() == -1 ) return -1;

        switch( TOKENTYPE )             // 滴锅掳 牢磊啊 函荐牢瘤 箭磊牢瘤甫 八荤
        {
            case VARIABLE:              // 函荐老 版快 贸府
                jj[1][0] = 0;
                jj[1][1] = variable[TOK].n;
                break;

            case NUMERIC:               // 箭磊老 版快 贸府
                jj[1][0] = 1;
                jj[1][1] = atoi( TOKEN );
                break;

            default:                    // 弊寇狼 TYPE捞搁 俊矾
                { Serror( 1 ); return -1; }
        }

        if( GetToken() == -1 ) return -1;
        if( !strchr( "+-/*%|&", *TOKEN ) )          // 父距 措涝巩捞扼搁
        {
            PutToken ();
            WriteBYTE( (char)equal_ );

            for( i = 0; i < 2; i++ )
            {
                if( jj[i][0] == 0 )                  // 函荐扼搁
                    WriteBYTE( (char)variable_ );
                else                                 // 箭磊扼搁
                    WriteBYTE( (char)value_ );

                WriteInt( jj[i][1] );
            }
        }
        else                                         // 措涝巩捞 酒聪扼搁
        {
            switch( *TOKEN )
            {
                case '+': j = add_;  break;
                case '-': j = sub_;  break;
                case '*': j = mul_;  break;
                case '/': j = div_;  break;
                case '%': j = rest_; break;
                case '|': j = or_;   break;
                case '&': j = and_;  break;
                default : { Serror( 1 ); return -1; }
            }
            WriteBYTE( j );

            if( GetToken() == -1 ) return -1;               // 技锅掳 牢磊甫 掘澜

            switch( TOKENTYPE )
            {
                case VARIABLE:
                    jj[2][0] = 0;
                    jj[2][1] = variable[TOK].n;
                    break;

                case NUMERIC:
                    jj[2][0] = 1;
                    jj[2][1] = atoi( TOKEN );
                    break;

                default: { Serror( 1 ); return -1; }
            }

            for( i = 0; i < 3; i++ )
            {
                if( jj[i][0] == 0 )        // 函荐捞搁
                    WriteBYTE( (char)variable_ );
                else                       // 箭磊捞搁
                    WriteBYTE( (char)value_ );

                WriteInt( jj[i][1] );
            }
        }
    } // end if

	return 1;
}




/* 
 窃荐疙 : GetLabel
 牢  磊 : 绝澜
 府畔蔼 : 绝澜
 汲  疙 : 扼骇疙捞 乐绰镑狼 困摹甫 炼荤窍咯 label1函荐俊 悸泼茄促.
 */
void GetLabel( void )
{
//    static int i;

    label1[funclabelsp++].n = (int)(outprog - oldoutprog);
}



/* 
 窃荐疙 : GetCommand
 牢  磊 : 绝澜
 府畔蔼 : 绝澜
 汲  疙 : 扁夯利牢 疙飞阑 贸府茄促.
 */
int GetCommand( void )
{
    int i, j;

    switch( comm[TOK].n )
    {
        // if巩阑 贸府茄促.
        case if_:
            WriteBYTE( (char)if_ );

            if( GetToken() == -1 ) return -1;
            if( *TOKEN != '(' ) { Serror( 1 ); return -1; }

            if( GetToken() == -1 ) return -1;
            // 父距 掘篮 配奴捞 函荐啊 酒聪扼搁
            if( TOKENTYPE != VARIABLE ) { Serror( 10 ); return -1; }

            WriteBYTE( (char)variable_ );                             // 函荐狼 蔼阑 历厘
            WriteInt( variable[TOK].n );

            if( GetToken() == -1 ) return -1;
            if( !strchr( "<>!=", *TOKEN ) ) { Serror( 1 ); return -1; } 

            // 厚背 扁龋甫 茫酒辑 利寸茄 贸府甫 茄促.
            // < - 0, <= - 1, > - 2, >= - 3, != - 4, == - 5
            switch( *TOKEN )
            {
                case '<':
                    if( GetToken() == -1 ) return -1;
                    if( *TOKEN != '=' )          // '<='楷魂磊啊 酒聪扼搁
                    {
                        PutToken();
                        WriteBYTE( 0 );
                    }
                    else
                    {
                        WriteBYTE( 1 );
                    }
                    break;

                case '>':
                    if( GetToken() == -1 ) return -1;
                    if( *TOKEN != '=' )          // '>='楷魂磊啊 酒聪扼搁
                    {
                        PutToken();
                        WriteBYTE( 2 );
                    }
                    else {
                        WriteBYTE( 3 );
                    }
                    break;

                case '!':
                    if( GetToken() == -1 ) return -1;
                    if( *TOKEN != '=' ) { Serror( 1 ); return -1; }
                    WriteBYTE( 4 );
                    break;

                case '=':
                    if( GetToken() == -1 ) return -1;
                    if( *TOKEN != '=' ) { Serror( 1 ); return -1; }
                    WriteBYTE( 5 );
                    break;
            }

            // 厚背且 措惑狼 快函蔼阑 贸府茄促. 快函蔼篮 函荐捞芭唱 荐摹捞促.
            if( GetToken() == -1 ) return -1;
            switch( TOKENTYPE )
            {
                case NUMERIC:
                    WriteBYTE( (char)value_ );
                    WriteInt( atoi(TOKEN) );
                    break;

                case VARIABLE:
                    WriteBYTE( (char)variable_ );
                    WriteInt( variable[TOK].n );
                    break;

                default: { Serror( 1 ); return -1; }
            }

            if( GetToken() == -1 ) return -1;
            if( *TOKEN != ')' ) { Serror( 1 ); return -1; }

            if( GetToken() == -1 ) return -1;
            if( *TOKEN != '{' ) { Serror( 1 ); return -1; }

            // 炼扒捞 芭窿老版快 痢橇且 林家甫 啊惑栏肺 父甸绢 0栏肺 悸泼
            WriteBYTE( (char)goto_ );
            WriteInt( 0 );

            ifelse[ifsp].off = outprog - oldoutprog - sizeof( int );
            ifelse[ifsp].d = ifsp;
            ifelse[ifsp].t = 0;
            ++ifsp;
            break;

        // else巩阑 贸府茄促.
        case else_:
            // goto巩俊 秦寸窍绰 蔼阑 历厘
            WriteBYTE( (char)goto_ );
            WriteInt( 0 );

            // if俊辑 芭窿老 版快 痢橇且 林家甫 泅犁狼 林家肺 悸泼茄促.
            *((int *)( oldoutprog + ifelse[ifsp].off)) += (sizeof( int ) + 1);

            if( GetToken() == -1 ) return -1;
            if( *TOKEN != '{' ) { Serror( 1 ); return -1; }
            if( ifelse[ifsp].d != ifsp ) { Serror( 1 ); return -1; }

            ifelse[ifsp].off = outprog - oldoutprog - sizeof( int );
            ifelse[ifsp].d = ifsp;
            ifelse[ifsp].t = 1;
            ++ifsp;
            break;

        // goto巩阑 贸府
        case goto_:
            WriteBYTE( (char)goto_ );
            if( GetToken() == -1 ) return -1;
            if( TOKENTYPE != LABEL ) { Serror( 9 ); return -1; }
			// 扼骇疙阑 label2俊 历厘. 捞 蔼篮 唱吝俊 label1苞 厚背且锭 静烙
			label2[label2_n].n = (int)(outprog - oldoutprog);
			strcpy( label2[label2_n++].s, TOKEN );
			WriteInt( 0 );        // 痢橇且 林家甫 烙矫肺 父惦
		break;

        // call巩阑 贸府
        case call_:
            WriteBYTE( (char)call_ );

            if( GetToken() == -1 ) return -1;
            if( *TOKEN != '(' ) { Serror( 1 ); return -1; }

            if( GetToken() == -1 ) return -1;
            if( TOKENTYPE != USERFUNC ) { Serror( 15 ); return -1; }
            strcpy( userfunc2[user2_n].s, TOKEN );
            // call巩栏肺 痢橇且 啊惑傍埃阑 犬焊窍绊 0阑 悸泼
            userfunc2[user2_n++].n = outprog - oldoutprog;
            WriteInt( 0 );

            if( GetToken() == -1 ) return -1;
            if( *TOKEN != ')' ) Serror( 1 );
            break;

        // end巩阑 贸府
        case end_:
            WriteBYTE( (char)end_ );
            if( ifsp > 0 ) { Serror( 3 ); return -1; }
            break;

        // switch巩阑 贸府
        case switch_:
            ++switsp;

            if( GetToken() == -1 ) return -1;
            if( *TOKEN != '(' ) { Serror( 1 ); return -1; }

            if( GetToken() == -1 ) return -1;
            if( TOKENTYPE != VARIABLE ) { Serror( 22 ); return -1; }
            j = variable[TOK].n;

            if( GetToken() == -1 ) return -1;
            if( *TOKEN != ')' ) { Serror( 1 ); return -1; }

            // 橇肺弊伐 贸澜俊 炼荤茄 case巩狼 肮荐父怒 傍埃阑 犬焊
            for( i = 0; i < swit[switsp].total; i++ )
            {
                WriteBYTE( (char)ifequal_ );

                WriteBYTE( (char)variable_ );
                WriteInt( j );

                // 厚背且 措惑阑 困茄 傍埃阑 犬焊
                swit[switsp].numoff[i] = outprog - oldoutprog;
                WriteBYTE( 0 );
                WriteInt( 0 );

                // 扼骇疙俊 泅犁 胶困摹狼 蔼阑 历厘秦辑 促澜俊 厚背且锭
                // 荤侩茄促.
                sprintf( label2[label2_n].s, "case%02d_%02d", switsp, i );
                label2[label2_n++].n = outprog - oldoutprog;
                WriteInt( 0 );
            }

            // 叼弃飘巩捞 乐栏搁 扼骇疙俊 巩磊凯阑 眠啊窍咯 default巩阑 角青
            WriteBYTE( (char)goto_ );
            sprintf( label2[label2_n].s, "defa%02d", switsp );
            label2[label2_n++].n = outprog - oldoutprog;
            WriteInt( 0 );
            break;

        // case巩阑 贸府
        case case_:
            // 扼骇疙俊 case巩俊 秦寸窍绰 巩磊凯阑 眠啊窍咯 唱吝俊 贸府茄促.
            sprintf( label1[label1_n].s, "case%02d_%02d", switsp,
                swit[switsp].count );
            label1[label1_n++].n = outprog - oldoutprog;

            if( GetToken() == -1 ) return -1;
            switch( TOKENTYPE )
            {
                case NUMERIC:
                    *((char *)(oldoutprog +
                        swit[switsp].numoff[swit[switsp].count])) = (char)value_;
                    j = atoi( TOKEN );
                    break;

                case VARIABLE:
                    *((char *)(oldoutprog +
                        swit[switsp].numoff[swit[switsp].count])) = (char)variable_;
                    j = variable[TOK].n;
                    break;

                default: { Serror( 1 ); return -1; }
            }

            *((int *)(oldoutprog + 1 +
                swit[switsp].numoff[swit[switsp].count++])) = j;
            break;

        // default巩阑 贸府
        case default_:
            // 扼骇疙俊 default巩狼 巩磊凯阑 涝仿窍咯 唱吝俊 贸府
            sprintf( label1[label1_n].s, "defa%02d", switsp );
            label1[label1_n++].n = outprog - oldoutprog;
            break;

        // endswitch巩阑 贸府
		case endswitch_:
            // switch巩狼 场巢阑 狼固茄促.
            if( swit[switsp].def != 1 ) {
                sprintf( label1[label1_n].s, "defa%02d", switsp );
                label1[label1_n++].n = outprog - oldoutprog;
            }
            sprintf( label1[label1_n].s, "defabreak%02d", switsp );
            label1[label1_n++].n = outprog - oldoutprog;

            swit[switsp].count = 0;
            break;

        // break巩阑 贸府
        case break_:
            WriteBYTE( (char)goto_ );
            sprintf( label2[label2_n].s, "defabreak%02d", switsp );
            label2[label2_n++].n = outprog - oldoutprog;
            WriteInt( 0 );
            break;
    }

	return 1;
}




/* 
 窃荐疙 : GetFunction
 牢  磊 : 绝澜
 府畔蔼 : 绝澜
 汲  疙 : zs.dat俊辑 沥狼茄 窃荐疙阑 盒籍窍咯 舅嘎篮 累诀阑 茄促.
          i绰 int甫 狼固窍哥, s绰 巩磊凯阑 狼固茄促.
 */
int GetFunction( void )
{
    int i, slen, len, *p;
    int tok = TOK;

    WriteBYTE( (char)myfunction[tok].n );          // 窃荐狼 锅龋甫 悸泼

    len = strlen( myfunction[tok].format );  // 概俺函荐狼 辨捞甫 掘澜

    if( GetToken() == -1 ) return -1;
    if( *TOKEN != '(' ){ Serror( 1 );  return -1; }

    for( i = 0; i < len; i++ ) {
        switch( myfunction[tok].format[i] ) {
            case 'i':                      // int屈阑 贸府
                if( GetToken() == -1 ) return -1;
                switch( TOKENTYPE )
                {
                    case NUMERIC:
                        WriteBYTE( (char)value_ );
                        WriteInt( atoi( TOKEN ) );
                        break;

                    case VARIABLE:
                        WriteBYTE( (char)variable_ );
//                        WriteInt( atoi( TOKEN ) );
                        WriteInt( variable[TOK].n );
                        break;

                    default:
                        { Serror( 6 ); return -1; }
                }
                break;

            case 's':                      // 巩磊凯阑 贸府
                if( GetToken() == -1 ) return -1;
                if( TOKENTYPE != STRING ) {Serror( 6 ); return -1; }
                slen = strlen( TOKEN );
                WinZsIncode( (char *)TOKEN );        // 巩磊凯阑 鞠龋拳窃
//                strcpy( outprog, (char *)TOKEN );
                WriteBYTE( (char)value_ );
                p = (int*)outprog;
                WriteInt( slen );                       // 巩磊凯狼 辨捞甫扁废
                memcpy( outprog, (char*)TOKEN, slen );    // 巩磊凯阑 历厘
                outprog += slen;
REPEA:
                if( GetToken() == -1 ) return -1;
                if( TOKENTYPE == STRING )
                {
                    slen = strlen( TOKEN );
                    WinZsIncode( (char *)TOKEN );        // 巩磊凯阑 鞠龋拳窃
                    (*p) += slen;
                    memcpy( outprog, (char*)TOKEN, slen );
                    outprog += slen;
                    goto REPEA;
                }
                else
                {
                    PutToken();
                }

                break;

            default: { Serror( 6 ); return -1; }
        }

        // 概俺 函荐荤捞狼 ','甫 贸府茄促.
        if( len > 1 && i != (len-1) ) {
            if( GetToken() == -1 ) return -1;
            if( *TOKEN != ',' ) { Serror( 4 ); return -1; }
        }
    }
    if( GetToken() == -1 ) return -1;
    if( *TOKEN != ')' ) {Serror( 1 );  return -1; }

	return 1;
}



/* 
 窃荐疙 : GetUserFunc
 牢  磊 : 绝澜
 府畔蔼 : 绝澜
 汲  疙 : 泅犁狼 免仿滚欺狼 困摹甫 蜡历 窃荐 滚欺俊 扁废茄促.
 */
void GetUserFunc( void )
{
	userfunc1[user1_n++].n = outprog - oldoutprog;
}



/* 
 窃荐疙 : Delimeter
 牢  磊 : 绝澜
 府畔蔼 : 绝澜
 汲  疙 : 备盒磊甫 罐疽阑 版快 贸府茄促.
          '#'篮 牢努风靛巩阑 狼固茄促. 牢努风靛 巩捞扼搁 泅犁 免仿滚欺牢
          outprog俊 牢努风靛 拳老狼 郴侩阑 佬绢辑 敬促.
 */
int Delimeter( void )
{
    FILE *fp;
    char temp[1024];
    int i;

    if( *TOKEN != '#' ) { Serror( 1 ); return -1; }

    if( GetToken() == -1 ) return -1;
	if( TOKENTYPE != COMMAND || comm[TOK].n != include_ ) Serror( 1 );

    if( GetToken() == -1 ) return -1;
    if( TOKENTYPE != STRING ) { Serror( 1 ); return -1; }

    fp = fopen( TOKEN, "rb" );
    if( fp == NULL ) { Serror( 23 ); return -1; }

    while( 1 ) {
        i = fread( temp, 1, 1024, fp );
        fwrite( temp, 1, i, outfile );
        INCLUDEFILESIZE += i;
        SOURCEFILESIZE += i;
        if( i != 1024 ) break;

    }
    fclose( fp );

	return 1;
}




/* 
 窃荐疙 : Run
 牢  磊 : 绝澜
 府畔蔼 : 绝澜
 汲  疙 : 阿 疙飞俊 嘎绰 窃荐甫 龋免茄促.
 */
int  Run( void )
{
    while( 1 ) {
        if( GetToken() == -1 ) return -1;

        if( TOKENTYPE == END ) break;        // 拳老狼 场捞扼搁

        else if( *TOKEN == '}' ) {                // if,else巩狼 辆搬磊扼搁
            if( First() == -1 ) return -1;
            continue;
        }

        else if( TOKENTYPE == NUMERIC )       // 巩厘狼 贸澜俊绰 箭磊啊 棵荐绝促.
		{ Serror( 1 ); return -1; }

        else if( TOKENTYPE == DELIMITER )     // 备盒磊 疙飞阑 贸府
        {
            if( Delimeter() == -1 ) return -1;;
            continue;
        }

        else if( TOKENTYPE == VARIABLE ) {
            if( Inc() == -1 ) return -1;
            continue;
        }

        else if( TOKENTYPE == LABEL ) {
            GetLabel();
            continue;
        }

        else if( TOKENTYPE == COMMAND ) {
            if( GetCommand() == -1 ) return -1;
            continue;
        }

        else if( TOKENTYPE == FUNCTION ) {
            if( GetFunction() == -1 ) return -1;
            continue;
        }

        else if( TOKENTYPE == USERFUNC ) {
            GetUserFunc();
            continue;
        }

        { Serror( 1 ); return -1; }
    }
    if( ifsp > 0 ) { Serror( 3 ); return -1; }

	return 1;
}



/* 
 窃荐疙 : LoadData
 牢  磊 : 绝澜
 府畔蔼 : 绝澜
 汲  疙 : zs.dat甫 坷锹秦辑 舅嘎篮 秦籍阑 窍绊 函荐狼 捞抚苞 措钎蔼, 窃荐狼
          捞抚苞 措钎蔼阑 悸泼窃
 */
int  LoadData( char *szdat )
{
    FILE *fp;
    char temp[ FILENAME_MAX];
    DWORD size;
    int i = 0, j;
    BYTE *p;

	sprintf( temp, "%s/script/%s", GameServerDataPath, szdat );

    fp = fopen( temp, "rb" );
    if( fp == NULL ) { return 0; }

    size = filelength( fileno(fp) );

    wzsprog = p = (BYTE *)ZSAlloc( size + 1 );
	fread( wzsprog, (size_t)size, 1, fp );
    fclose( fp );

    if( *(wzsprog + size -1)  == 0x1a )
        *(wzsprog+size-1) = 0;
    else
        *(wzsprog+size) = 0;


    //printf( "\nZS.DAT file compile" );

	while( 1 ) {
		if( GetToken() == -1 ) return -1;
		if( TOKENTYPE == END ) break;

        // 函荐甫 钎矫窍绰 巩磊凯阑 茫绰促.
		if( strcmp( TOKEN, "variable" ) != 0 ) { Serror( 1 ); return -1; }


		// 函荐俊 秦寸窍绰 措钎疙苞 弊 蔼阑 variable硅凯俊 悸泼茄促.
		while( 1 ) {
			if( GetToken() == -1 ) return -1;
			if( !strcmp( TOKEN, "end" ) ) break;        // 场捞搁 促澜栏肺

			if( TOKENTYPE != STRING ) Serror( 1 );
			j = SearchVariable( TOKEN );                // 鞍篮 函荐啊 乐绰瘤 八荤
			if( j != -1 ) Serror( 14 );
			strcpy( variable[i].s, TOKEN );

			if( GetToken() == -1 ) return -1;
			if( TOKENTYPE != NUMERIC ) Serror( 1 );
			j = atoi( TOKEN );
			if( j > 30000 ) { Serror( 18 ); return -1; }
			variable[i++].n = j;
		}

		i = 0;

		if( GetToken() == -1 ) return -1;
		if( strcmp( TOKEN, "function" ) != 0 ) { Serror( 1 ); return -1; }

		// 窃荐俊 秦寸窍绰 措钎疙苞 弊 蔼阑 function硅凯俊 悸泼茄促.
		while( 1 ) {
			if( GetToken() == -1 ) return -1;
			if( !strcmp( "end", TOKEN )) break;

			if( TOKENTYPE != STRING ) { Serror( 1 ); return -1; }
			j = SearchFunction( TOKEN );
			if( j != -1 ) Serror( 14 );                 // 鞍篮 窃荐啊 乐绰瘤 八荤
			strcpy( myfunction[i].s, TOKEN );

			GetToken ();
			if( TOKENTYPE != STRING ) { Serror( 1 ); return -1; }
			if( !strcmp( TOKEN, "null" ) )              // 概俺函荐啊 绝栏搁
				myfunction[i].format[0] = 0;
			else
				strcpy( myfunction[i].format, TOKEN );    // 概俺函荐啊 乐栏搁

			if( GetToken() == -1 ) return -1;
			if( TOKENTYPE != NUMERIC ) { Serror( 1 ); return -1; }
			j = atoi( TOKEN );
			if( j > 0xcf ) { Serror( 18 ); return -1; }
			myfunction[i++].n = j;
		}

		if( i == 0 ) { Serror( 1 ); return -1; }
	}
	LINE = 0;
    ZSfree( p );

	return 1; 
    //printf( " --------> completed.\n\n" );
}




/* 
 窃荐疙 : InitProgram
 牢  磊 : s = 哪颇老窍绊磊 窍绰 拳老狼 捞抚
 府畔蔼 : 绝澜
 汲  疙 : 家胶甫 皋葛府肺 佬绢甸牢饶, 免仿且 拳老狼 捞抚阑 积己矫挪促.
 */
int  InitProgram( char *s )
{
    FILE *fp;
    char *p;

    fp = fopen( s, "rb" );
    if( fp == NULL ) 
	{	
		char mapname[ FILENAME_MAX];
		sprintf( mapname, "Script File Not Found : '%s'", s );
		MessageBox(0, mapname, "ERROR", MB_OK);
        exit( 1 );
    }	
	
    SOURCEFILESIZE = filelength( fileno(fp) );


    fread( wzsprog, SOURCEFILESIZE, 1, fp );
    fclose( fp );

    if( *(wzsprog + (int)SOURCEFILESIZE -1) == 0x1a )
        *(wzsprog + (int)SOURCEFILESIZE-1) = 0;
    else
        *(wzsprog + (int)SOURCEFILESIZE) = 0;

    strcpy( TARGETFILENAME, s );
    p = strchr( TARGETFILENAME, '.' );
    strcpy( p, ".ZSC" );

/*	if( GameMakeFlag == ON )
	{
		outfile = Fopen( TARGETFILENAME, "wb" );
		if( fp == NULL ) { 
	        exit( 1 );
		}
	}
*/
	return TRUE;
}



/* 
 窃荐疙 : Path1
 牢  磊 : 绝澜
 府畔蔼 : 绝澜
 汲  疙 : 家胶甫 茄锅 佬栏搁辑 巩过利 俊矾甫 眉农茄促.
 */
int Path1( void )
{
    BYTE *p = wzsprog;
    int j;
    int func = 0;
    int funcdepth = 0;
    int if_else_count = 0;
    int switch_case = 0;
    int switch_count = 0;

    while( 1 ) {
        if( GetToken() == -1 ) return -1;
        if( TOKENTYPE == END ) break;

        // if, else狼 肮荐甫 八荤秦辑 俊矾甫 眉农茄促.
        if( *TOKEN == '{' ) ++if_else_count;
        if( *TOKEN == '}' ) --if_else_count;

        // 扼骇疙老 版快俊绰 扼骇阑 茫酒焊绊, 乐栏搁 扼骇 吝汗 俊矾甫 免仿窍绊
        // 绝栏搁 殿废茄促.
        if( TOKENTYPE == LABEL )
        {
            if( strlen( TOKEN ) > LABEL_LENGTH ) { Serror( 8 ); return -1; }
            j = SearchLabel( TOKEN );
            if( j != -1 ) Serror( 14 );
            strcpy( label1[label1_n++].s, TOKEN );
        }

        // 蜡历啊 沥狼茄 窃荐捞搁 窃荐甫 茫酒焊绊, 乐栏搁 窃荐 吝汗 俊矾甫
        // 免仿窍绊 绝栏哥 殿废阑 茄促.
        if( TOKENTYPE == USERFUNC )
        {
            if( TOK != -1 ) { Serror( 15 ); return -1; }
            strcpy( userfunc1[funcdepth++].s, TOKEN );
            if( func++ != 0 ) { Serror( 13 ); return -1; }
        }

        if( TOKENTYPE == COMMAND ) {
            switch( comm[TOK].n ) {
                // switch巩狼 case客 default巩狼 肮荐甫 犬牢窍绊 俊矾甫 八荤
                case switch_:
                    ++switch_case;
                    ++switch_count;
                    break;

                case endswitch_:
                    --switch_case; break;

                case default_:
                    swit[switch_count].def = 1; break;

                case case_:
                    ++swit[switch_count].total; break;

                // call疙飞巩 促澜俊档 扼骇疙捞 坷扁锭巩俊 捞巴阑 乔茄促.
                case call_:
                    if( GetToken() == -1 ) return -1;
                    if( GetToken() == -1 ) return -1;
                    if( GetToken() == -1 ) return -1; break;

                // end巩阑 贸府窍哥 if-else, switch-endswitch, 弊府绊 窃荐啊
                // 沥惑利牢啊甫 魄窜茄促.
                case end_:
                    if( func-- != 1 )  { Serror( 13 ); return -1; }
                    if( if_else_count != 0 ) { Serror( 12 ); return -1; }
                    if( switch_case != 0 ) { Serror( 21 ); return -1; }
                    break;

                case goto_:
                    if( GetToken() == -1 ) return -1;
                    break;
            }
        }
    }
    if( func > 0 ) { Serror( 13 ); return -1; }

    ifsp = 0;
    wzsprog = p;
    LINE = 0;

	return TRUE; 
}




/* 
 窃荐疙 : EndProgram
 牢  磊 : 绝澜
 府畔蔼 : 绝澜
 汲  疙 : 橇肺弊伐狼 盖 付瘤阜俊 角青登哥, 扼骇疙狼 困摹甫 扁废,
          窃荐疙狼 困摹甫 扁废, 窃荐狼 肮荐甫 免仿拳老狼 贸澜 何盒俊 扁废茄促.
*/
int EndProgram( void )
{
    int i, j;
    int size = 0;

    // 盔贰狼 扼骇疙苞 蜡历啊 荤侩茄 扼骇疙阑 辑肺 厚背秦辑 弊蔼阑 免仿茄促.
    for( i = 0; i < label2_n; i++ ) {
        j = SearchLabel( label2[i].s );
        if( j == -1 ) 
		{
            strcpy( TOKEN, label2[i].s );
            { Serror( 9 ); return -1; }
        }

        *((int *)(oldoutprog+label2[i].n)) = label1[j].n;
    }


    // 盔贰狼 窃荐疙苞 蜡历啊 荤侩茄 窃荐疙阑 辑肺 厚背秦辑 弊蔼阑 免仿茄促.
    for( i = 0; i < user2_n; i++ ) {
        j = SearchUserFunc( userfunc2[i].s );
        if( j == -1 ) {
            strcpy( TOKEN, userfunc2[i].s );
            Serror( 15 );
        }

        *((int *)(oldoutprog+userfunc2[i].n)) = userfunc1[j].n;
    }

	for( i = 0; userfunc1[i].s[0]; i++ ) func_off[i] = userfunc1[i].n;
	TotalFunction = i;


/*
	if( GameMakeFlag == ON )
	{
		// 葛电 窃荐狼 肮荐甫 拳老狼 贸澜俊 扁废阑 窍绊 窃荐狼 可悸蔼阑 扁废茄促.
		temp = ftell( outfile );
		fseek( outfile, 8, SEEK_CUR );        // 庆歹甫 免仿窍扁困茄 咯归

		for( i = 0; userfunc1[i].s[0]; i++ )
		{
			fwrite( &userfunc1[i].n, 1, sizeof( int ), outfile );
			size += sizeof( int );
		}

		// 哪颇老等 搬苞甫 拳老肺 免仿茄促.
		size += (outprog - oldoutprog - ( i * sizeof( int )) );
		fwrite( oldoutprog, outprog - oldoutprog, 1, outfile );

		fseek( outfile, temp, SEEK_SET );
		fwrite( &size, 1, sizeof( int ), outfile );        // 拳老狼 农扁甫 扁废
		fwrite( &i, 1, sizeof( int ), outfile );           // 窃荐狼 肮荐甫 扁废
		fclose( outfile );
	}
*/
#ifdef MAKE_EXE_FILE
    printf( "\nTotal compile line     : %10d lines\n", --LINE );
    printf( "\nSource file size       : %10d BYTEs\n", SOURCEFILESIZE );
    printf( "\nTarget file size       : %10d BYTEs\n\n",        size + INCLUDEFILESIZE + i * sizeof( int ) );
#endif

	return TRUE;
}



static char *Filename;
static int Totalcount;


/*===========================================================================
   胶橇扼捞飘肺 父甸 葛电 拳老阑 八祸茄促.
===========================================================================*/
int SearchFile( char *fn )
{	
    char temp[FILENAME_MAX], *p, (*f)[FILENAME_MAX];
    //struct find_t ff;
	struct	_finddata_t ff;
	int handle, c;
	
    strcpy( temp, fn );
    p = strchr( temp, '.' );
    if( p )
        strcpy( p, ".ZS" );
    else
        strcat( temp, ".ZS" );
	
    f = (char (*)[FILENAME_MAX])Filename;
	
	handle =  _findfirst( temp, &ff);
	if( handle == -1 ) return 0 ;
	c = 0;
    while( !c )
    {	
        strcpy( (char *)f++, ff.name );
        Totalcount++;
		
        c = _findnext( handle, &ff );
    }	
	
	return Totalcount;
}	
	
	
#ifdef MAKE_EXE_FILE	
	void main( int argc, char *argv[] )
	{	
		int i;
		char temp[FILENAME_MAX], (*f)[FILENAME_MAX];
		
		printf( "\n   ZS Script Compiler For Win95  Version 2.0\n" );
		printf( "----------------------------------------------------------------\n");
		
		wzsprog = oldwzsprog = (BYTE *)ZSAlloc( 800000 + 1 );
		outprog = oldoutprog = (BYTE *)ZSAlloc( 400000 );
		Filename = ZSAlloc( FILENAME_MAX * 2000 );
		
		if( argc == 1 )	strcpy( temp, "*.*" );
		else			strcpy( temp, argv[1] );
		
		if( SearchFile( temp ) ) LoadData();
		
		f = (char (*)[FILENAME_MAX])Filename;
		for( i = 0; i < Totalcount; i++, f++ )
		{
			wzsprog = oldwzsprog;
			outprog = oldoutprog;
			memset( wzsprog, 0, SOURCEFILESIZE + 1 );

			memset( label1,		0,	sizeof( COMMSET )	*	MAX_SCR_ARRAY );            // 盔贰 扼骇狼 郴侩阑 历厘窍绰 函荐
			memset( label2,		0,	sizeof( COMMSET )	*	MAX_SCR_ARRAY );            // goto巩 促澜俊 静牢 扼骇阑 历厘
			memset( ifelse,		0,	sizeof( IF_ELSE )	*	MAX_SCR_ARRAY );            // if, else巩狼 郴侩阑 历厘
			memset( userfunc1,	0,	sizeof( COMMSET )	*	MAX_SCR_ARRAY );            // 盔贰 窃荐疙狼 郴侩阑 历厘
			memset( userfunc2,	0,	sizeof( COMMSET )	*	MAX_SCR_ARRAY );            // call巩 促澜俊 静牢 窃荐甫 历厘
			memset( swit,		0,	sizeof( SWITCHCASE )*	 50 );            // 窍唱狼 switch巩俊辑 阿阿狼 case巩阑历厘

			printf( " %3d/%3d     : %13s File Compiling...........\n", i+1, Totalcount, (char*)f );
			

			ifsp = 0;                            // if, else狼 胶琶蔼
			switsp = 0;                          // switch狼 函困蔼
			label1_n=0, label2_n = 0;              // 盔贰狼 扼骇蔼苞 静咯柳 扼骇蔼阑 历厘
			user1_n = 0, user2_n = 0;                // 盔贰狼 窃荐蔼苞 
			
			
			InitProgram( (char*)f );
			Path1();
			Run();
			EndProgram();
		}

		printf( "\n    %d ZS Files Compiled ..\n", Totalcount );
		printf(   "    %d Error occured...\n", ErrorCount );
		printf( "-------------------------------------------Copyright(c),1995 F.E.W \n");
		printf( "-------------------------------------------------- Coded by Phyneix  \n");
		
		ZSfree( oldwzsprog );
		ZSfree( oldoutprog );
		ZSfree( Filename );
		
		_getch();
	}

#else
	
	
void InitTextScript( void )
{	
	oldwzsprog = (BYTE *)ZSAlloc( 800000 + 1 );
	oldoutprog = (BYTE *)ZSAlloc( 400000 );
}	
	

	
	
void EndTextScript( void )
{	
	if( oldwzsprog != NULL ) { ZSfree( oldwzsprog ); oldwzsprog = NULL; }
	if( oldoutprog != NULL ) { ZSfree( oldoutprog ); oldoutprog = NULL; }
}	
	
	
	
void NotePad( char *name )
{	
	//"NotePad.exe"
//	ShellExecute(NULL, NULL, SysInfo.notepad, name, NULL, SW_SHOWNORMAL);
}	
	
	
BYTE *LoadTextScript( char *name )
{	

	LINE = 0;
	
	TOK = 0; // ^^^^ 0308
	memset( variable, 0, sizeof( COMMSET ) *  2000 );	//MAX_SCR_ARRAY );             // 荤侩磊啊 沥狼茄 函荐狼 郴侩阑 历厘
	memset( myfunction, 0, sizeof( USER_FUNC ) * 1000 ); //MAX_SCR_ARRAY );           // 蜡历啊 沥狼茄 窃荐甫 历厘
	
	LoadData( "Zs.Dat" );	

	wzsprog = oldwzsprog;
	outprog = oldoutprog;

	strcpy( Wz_scriptfilename, name );

	memset( wzsprog, 0, 100000 + 1 );
	memset( (char*)label1,		0,	sizeof( COMMSET )	*	100 ); // MAX_SCR_ARRAY );            // 盔贰 扼骇狼 郴侩阑 历厘窍绰 函荐
	memset( (char*)label2,		0,	sizeof( COMMSET )	*	100 ); // MAX_SCR_ARRAY );            // goto巩 促澜俊 静牢 扼骇阑 历厘
	memset( (char*)ifelse,		0,	sizeof( IF_ELSE )	*	500 ); // MAX_SCR_ARRAY );            // if, else巩狼 郴侩阑 历厘
	memset( (char*)userfunc1,	0,	sizeof( COMMSET )	*	1000 );//MAX_SCR_ARRAY );            // 盔贰 窃荐疙狼 郴侩阑 历厘
	memset( (char*)userfunc2,	0,	sizeof( COMMSET )	*	1000 ); //MAX_SCR_ARRAY );            // call巩 促澜俊 静牢 窃荐甫 历厘
	memset( (char*)swit,		0,	sizeof( SWITCHCASE )*	 50 );						// 窍唱狼 switch巩俊辑 阿阿狼 case巩阑历厘

	ifsp		= 0;                   // if, else狼 胶琶蔼
	switsp		= 0;                   // switch狼 函困蔼
	label1_n	= 0, label2_n	= 0;   // 盔贰狼 扼骇蔼苞 静咯柳 扼骇蔼阑 历厘
    user1_n		= 0, user2_n	= 0;   // 盔贰狼 窃荐蔼苞 
	funclabelsp = 0;

	if( InitProgram( name ) == -1 ) goto ERROR_;
	if( Path1()				== -1 ) goto ERROR_;
	if( Run()				== -1 ) goto ERROR_;
	if( EndProgram()		== -1 ) goto ERROR_;
	
  CLoadScript = SCRIPT_NEW;
	return oldoutprog;
ERROR_:

	char nn[ FILENAME_MAX];
	strcpy( nn, Wz_scriptfilename );

	for( int i = strlen( nn ) - 1  ; i > 0  ; i -- )	   if( *(nn + i ) == '\\' ) break;

//	sprintf( temp, "'%s'甫 促矫 肺靛 钦聪促.", strupr(nn+ i +1)  );
//	if( YesOrNo( temp, "犬牢") == IDYES ) goto AGAIN_;

	return NULL;
}


#endif







