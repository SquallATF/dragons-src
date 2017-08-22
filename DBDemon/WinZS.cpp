/******************************************************************************
 작성자   : 장 석규
 Compile  : bcc -ms
 화일 설명: 이 화일은 스크립트 내용을 문법적으로 해석하고 이것을 실행기가
			쓸수 있는 내용의 이진 화일로 생성한다.
			* 모든 함수명은 1바이트로 변환된다. 그러므로 최대 명령어의 갯
			  수는 내부, 외부 명령어 합쳐서 255개이다.
			* 모든 변수는 실제값과 0x8000을 OR해서 생성시킨다.
			* 모든 수치값은 실제값을 따른다. ( 0 ~ 32767 )
			* 모든 GOTO 명령의 점프 주소는 2바이트이다.
			* switch문에서 case문은 최대 50개이다.


date of Modification : 1997년 10월 30일
who modified : Kim Hongseok
contents of modification : porting of windows
******************************************************************************/
#include "StdAfx.h"
//#include <Windows.h>
//#include <dos.h>
//#include <stdio.h>
//#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include <stdarg.h>

#define __WINZS_H__
#include "WinZs.h"

#include "Scrp_int.h"


#ifndef __TYPEDEF__
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
#endif


#define MAX_SCR_ARRAY     10000


// 각 명령어에 해당하는 문자열의 값을 정의한다.
// 다음의 정의문은 컴파일된 이진화일에 실제로 쓰여지는 값이다.
/******************************************************************************
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
******************************************************************************/
// 다음의 정의문은 컴파일할때만 요구되는 값이다.
enum {
	else_ = 0xd0,
	case_ = 0xd1,
	endswitch_ = 0xd2,
	default_ = 0xd3,
	switch_ = 0xd4,
	break_ = 0xd5,
	include_ = 0xd6,
};


#define LABEL_LENGTH 20        // 라벨의 최대 길이를 나타냄

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
	char s[50 + 1];
	int n;
} COMMSET;


typedef struct {        // if_else 문의 내용을 저장하기 위한 구조체
	int off;            // if나 else문이 위치한 곳의 옵셋
	BYTE d;             // 깊이를 나타냄
	BYTE t;             // 0 = if, 1 = else
} IF_ELSE;


typedef struct {        // 사용자가 정의한 함수명을 저장할 구조체
	char s[50 + 1];       // 함수명을 저장할 변수
	char format[31];  // 함수에 해당하는 인수의 format을 저장할 변수
	int n;             // 함수명이 있는곳의 위치
} USER_FUNC;


typedef struct {        // switch 문의 내용을 저장할 구조체
	int total;          // 프로그램 처음에 case의 갯수를 조사한 내용
	int count;
	int def;
	int numoff[50];
} SWITCHCASE;


// 각각에 해당하는 명령어와 그 명령어에 해당하는 숫자를 가지는 구조체
static COMMSET comm[255] = {
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


static COMMSET variable[MAX_SCR_ARRAY];             // 사용자가 정의한 변수의 내용을 저장
static COMMSET label1[MAX_SCR_ARRAY];               // 원래 라벨의 내용을 저장하는 변수
static COMMSET label2[MAX_SCR_ARRAY];               // goto문 다음에 쓰인 라벨을 저장
static IF_ELSE ifelse[MAX_SCR_ARRAY];               // if, else문의 내용을 저장
static COMMSET userfunc1[MAX_SCR_ARRAY];            // 원래 함수명의 내용을 저장
static COMMSET userfunc2[MAX_SCR_ARRAY];            // call문 다음에 쓰인 함수를 저장
static USER_FUNC myfunction[MAX_SCR_ARRAY];           // 유저가 정의한 함수를 저장
static SWITCHCASE swit[50];               // 하나의 switch문에서 각각의
									 // case문을저장

static char TOKEN[MAX_SCR_ARRAY];                    // 불리해낸 토큰을 저장할 버퍼
static char TOKENTYPE;                      // 얻어진 토큰의 타입
static int  TOK;                            // 토큰을 변환한 후의 값을 저장

static int ifsp;                            // if, else의 스택값
static int switsp;                          // switch의 변위값
static int label1_n, label2_n;              // 원래의 라벨값과 쓰여진 라벨값을 저장
static int user1_n, user2_n;                // 원래의 함수값과 쓰여진 함수값을 저장
static int funclabelsp;						// GetLabel () 함수에서 사용

static int LINE = 0;                        // 소스화일의 라인값을 카운트

BYTE *wzsprog,                          // 소스 프로그램을 읽어들일 버퍼
*oldwzsprog;                       // 처음에는 wzsprog와 같은 값을 가지며
								// 프로그램 종료시 free함수에서 사용
static BYTE *outprog,                       // 출력 버퍼를 가리킴
*oldoutprog;                    // 처음에는 oldoutprog와 같은 값을 가지며
						 // 프로그램 종료시 free함수에서 사용
static int SOURCEFILESIZE;                  // 소스화일의 크기를 나타냄
static char TARGETFILENAME[FILENAME_MAX];             // 출력할 화일의 이름을 나타냄
static FILE *outfile;                       // 출력할 화일을 가리키는 화일 포인터
static int INCLUDEFILESIZE;                 // 인클루드될 화일의 크기를 가리킴

static int ErrorCount;


static char Wz_scriptfilename[FILENAME_MAX];


// Scrp_int.cpp
extern int func_off[100];

//////////////////////////////////////////////////////////////////////////
// 실질적인 함수들의 정의
//////////////////////////////////////////////////////////////////////////


enum {
	ZS_FALSE,
	ZS_TRUE,
};

#define ZSAlloc(x)      ZS_Alloc( (x), __FILE__, __LINE__ )

void ZSError(char *s, ...)
{
	va_list arg;
	char temp[FILENAME_MAX];

	va_start(arg, s);
	vsprintf(temp, s, arg);
	va_end(arg);

	MessageBox(0, s, "ZSerror", MB_OK);

	exit(1);
}


char *ZS_Alloc(int size, const char *file, int line)
{
	char *memorytemp;
	memorytemp = (char *)GlobalAlloc(GMEM_FIXED, size);
	//memorytemp = (char *)malloc( size );

	if (!memorytemp)
		ZSError("%s(%d) : \n %d alloc Err\n", file, line, size);

	return(memorytemp);
}


void ZSfree(void *t)
{
	GlobalFree(t);
	//	free( t );
}



FILE *ZS_Fopen(const char *file, const char *mode)
{
	FILE *tempfile;

	tempfile = fopen(file, mode);
	if (!tempfile)
		ZSError("%s FILE PROCESS ERROR.\n", file);

	return(tempfile);
}

//------------------------------------------------------------------------





/******************************************************************************
 함수명 : Incode
 인  자 : s = 암호화할 문자열
 리턴값 : 없음
 설  명 : s문자열을 받아서 내부에 정의된 수치와 연산을 수행해서 암호화함
 ******************************************************************************/
void WinZsIncode(char *s)
{
	//    static BYTE t[ 20 ] = {
	//        101, 255, 168, 210, 184, 47, 176,  54,  58, 63,
	//         78,   1,  34,  69, 200,  88,  77, 111, 48, 121
	//    };
	BYTE t = 0xff;
	int i = 0;

	while (*s) {
		(*s++) ^= 0xff;
		if (++i > 19) i = 0;
	}
}


/******************************************************************************
 함수명 : SearchFunction
 인  자 : s = 찾고자 하는 함수의 문자열
 리턴값 : 찾지못하면 -1, 찾으면 번호를 리턴
 설  명 : s문자열에 해당하는 유저가 정의한 함수명을 function배열에서 찾는다.
 ******************************************************************************/
int SearchFunction(char *s)
{
	int i;

	for (i = 0; myfunction[i].s[0] != 0; i++)
		if (!strcmp(myfunction[i].s, s)) return(i);

	return(-1);
}


/******************************************************************************
 함수명 : SearchCommand
 인  자 : s = 찾고자하는 명령어의 문자열
 리턴값 : 찾지못하면 -1, 찾으면 번호를 리턴
 설  명 : s문자열에 해당하는 명령어를 찾음
 ******************************************************************************/
int SearchCommand(char *s)
{
	int i;

	for (i = 0; comm[i].s[0] != 0; i++)
		if (!strcmp(comm[i].s, s)) return(i);

	return(-1);
}


/******************************************************************************
 함수명 : SearchVariable
 인  자 : s = 찾고자 하는 변수의 문자열
 리턴값 : 찾지못하면 -1, 찾으면 번호를 리턴
 설  명 : s문자열에 해당하는 변수명을 찾음
 ******************************************************************************/
int SearchVariable(char *s)
{
	int i;

	for (i = 0; variable[i].s[0] != 0; i++)
		if (!strcmp(variable[i].s, s)) return(i);

	return(-1);
}


/******************************************************************************
 함수명 : SearchLabel
 인  자 : s = 찾고자 하는 라벨의 문자열
 리턴값 : 찾지못하면 -1, 찾으면 번호를 리턴
 설  명 : s문자열에 해당하는 라벨을 찾음
 ******************************************************************************/
int SearchLabel(char *s)
{
	int i;

	for (i = 0; label1[i].s[0] != 0; i++)
		if (!strcmp(label1[i].s, s))
			return(i);

	return(-1);
}



/******************************************************************************
 함수명 : SearchUserFunc
 인  자 : s = 찾고자 하는 유저가 정의한 함수의 문자열
 리턴값 : 찾지못하면 -1, 찾으면 번호를 리턴
 설  명 : 소스에서 정의한 s문자열에 해당하는 함수명을 찾음
		  함수명은 반드시 '_'로 시작되어야 함
******************************************************************************/
int SearchUserFunc(char *s)
{
	int i;

	for (i = 0; userfunc1[i].s[0] != 0; i++)
		if (!strcmp(userfunc1[i].s, s)) return(i);

	return(-1);
}




/******************************************************************************
 함수명 : Serror
 인  자 : x = 출력하고자 하는 에러 메시지의 번호
 리턴값 : 없음
 설  명 : x에 해당하는 에러 메시지를 출력하고 프로그램을 종료한다.
 ******************************************************************************/
void Serror(int x)
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

	//ZSfree( oldwzsprog );
	//ZSfree( oldoutprog );

	ErrorCount++;

	remove(TARGETFILENAME);


	char nn[FILENAME_MAX];
	strcpy(nn, Wz_scriptfilename);
	for (int i = strlen(nn) - 1; i > 0; i--)	   if (*(nn + i) == '\\') break;

	//sprintf( temp, "%s: %d line: '%s'\n\n ' %s '를 열까요?",s[x], LINE, TOKEN,  strupr( nn+i+1) );
	//if( YesOrNo( temp, "Script Compile Error") == IDYES )	NotePad( Wz_scriptfilename );



#ifdef MAKE_EXE_FILE	
	if (x == 0) printf("\n%s\n", s[0]);
	else {
		printf("\n\aError:\n%5d LINE: '%s' %s\n", LINE, TOKEN, s[x]);
	}
	printf("Anyway, Process?( y/N ) ");
	t = _getch();
	if (t == 'y' || t == 'Y') return;
	else exit(1);
#endif
}



/******************************************************************************
 함수명 : Isdeli
 인  자 : i = 검사할 문자
 리턴값 : 구분문자이면 1, 아니면 0
 설  명 : i문자가 구분문자인지를 검사한다.
 ******************************************************************************/
int Isdeli(char i)
{
	if (strchr(" ';,+-*/%#=<>(){}[]:!@\t&|", i))
		return(1);

	return(0);
}



/******************************************************************************
 함수명 : Isspace
 인  자 : i = 검사할 문자
 리턴값 : 공백, 탭, 리턴, 라인 피드문자이면 1, 아니면 0
 설  명 : i문자가 공백, 탭, 리턴, 라인 피드문자인지를 검사한다.
 ******************************************************************************/
int Isspace(char i)
{
	switch (i) {
	case 0x0a:        // 라인 피드 문자이면 LINE변수를 하나 증가한다.
		LINE++;
	case 0x0d:
	case '\t':
	case ' ':
		return(1);
	default:
		return(0);
	}
}



/******************************************************************************
 함수명 : PutToken
 인  자 : 없음
 리턴값 : 없음
 설  명 : TOKEN배열의 내용을 wzsprog에 다시쓴다.
 ******************************************************************************/
void PutToken(void)
{
	int i;

	i = strlen(TOKEN);

	while (i) {
		*--wzsprog = TOKEN[i - 1];
		i--;
	}
	if (TOKENTYPE == LABEL) {
		*--wzsprog = ':';
	}
}



/******************************************************************************
 함수명 : GetToken
 인  자 : 없음
 리턴값 : 토큰 타입을 리턴한다.
 설  명 : wzsprog버퍼를 분석해서 토큰을 얻는다.
 ******************************************************************************/
int GetToken(void)
{
	register char *temp;

	// 토큰 버퍼를 초기화 한다.
	temp = TOKEN;
	TOKENTYPE = 0;
	TOK = 0;

start:
	// 화일의 끝에 다다랐을때
	if (*wzsprog == 0) return(TOKENTYPE = END);

	// 공백, 탭문자, 리턴, 라인피드 문자를 없앤다.
	while (Isspace(*wzsprog)) ++wzsprog;
	if (*wzsprog == 0) goto start;        // 화일의 끝이면

	// 라벨명이면 TOKEN변수에 이름을 저장하고 리턴
	if (*wzsprog == ':') {
		if (Isdeli(*++wzsprog)) { Serror(7); return -1; } // 라벨명은 영문자와 숫자로 구성 
		while (!Isdeli(*wzsprog) && *wzsprog != '\r' && *wzsprog != 0)
			*temp++ = *wzsprog++;
		*temp = 0;
		return(TOKENTYPE = LABEL);
	}

	// 주석이면 라인의 끝까지를 스킵하고 다시 처음으로 돌아간다.
	if (*wzsprog == ';') {
		++wzsprog;
		while (*wzsprog != '\n' && *wzsprog++ != 0);
		goto start;
	}

	// 사용자 정의 함수이면
	if (*wzsprog == '@') {
		++wzsprog;
		while (!Isdeli(*wzsprog) && *wzsprog != '\r' && *wzsprog != 0)
			*temp++ = *wzsprog++;
		*temp = 0;
		TOK = SearchUserFunc(TOKEN);
		return(TOKENTYPE = USERFUNC);
	}

	// 구분문자이면
	if (Isdeli(*wzsprog)) {
		*temp++ = *wzsprog++;
		*temp = 0;
		return(TOKENTYPE = DELIMITER);
	}

	// 문자열이면
	if (*wzsprog == '"') {
		++wzsprog;
		while (*wzsprog != '"' && *wzsprog != '\r' && *wzsprog != 0)
			*temp++ = *wzsprog++;
		// 마지막 문자가 라인피드나 화일의 끝이면 에러
		if (*wzsprog == '\r' || *wzsprog == 0) { Serror(1); return -1; }
		*temp = 0;
		wzsprog++;        // '"'문자를 스킵
		return(TOKENTYPE = STRING);
	}

	// 숫자이면
	if (isdigit(*wzsprog)) {
		while (isdigit(*wzsprog) && !Isdeli(*wzsprog) &&
			*wzsprog != '\r' && *wzsprog != 0)
			*temp++ = *wzsprog++;
		*temp = 0;
		return(TOKENTYPE = NUMERIC);
	}

	// 명령어나 라벨, 변수를 체크
	if (isalpha(*wzsprog)) {
		while (!Isdeli(*wzsprog) && *wzsprog != '\r' && *wzsprog != 0)
			*temp++ = *wzsprog++;
		*temp = 0;

		// 만약 얻어진 문자열이 명령어라면
		TOK = SearchCommand(TOKEN);
		if (TOK != -1) return(TOKENTYPE = COMMAND);

		// 만약 얻어진 문자열이 변수라면
		TOK = SearchVariable(TOKEN);
		if (TOK != -1) return(TOKENTYPE = VARIABLE);

		// 만약 얻어진 문자열이 라벨명이라면
		TOK = SearchLabel(TOKEN);
		if (TOK != -1) return(TOKENTYPE = LABEL);

		// 만약 얻어진 문자열이 사용자 정의 함수명이라면
		TOK = SearchFunction(TOKEN);
		if (TOK != -1) return(TOKENTYPE = FUNCTION);

		// 그외는 문자열로 간주
		return(TOKENTYPE = STRING);
	}

	{ Serror(1); return -1; }

	return(TOKENTYPE);
}



/******************************************************************************
 함수명 : WriteInt
 인  자 : x = outprog에 출력하고자 하는 값
 리턴값 : 없음
 설  명 : x에 해당하는 값을 outprog버퍼에 기록한다.
 ******************************************************************************/
void WriteInt(int x)
{
	*((int *)outprog) = x;
	outprog += sizeof(int);
}



/******************************************************************************
 함수명 : WriteBYTE
 인  자 : x = outprog버퍼에 출력하고자 하는 값
 리턴값 : 없음
 설  명 : x에 해당하는 값을 outprog버퍼에 기록한다.
 ******************************************************************************/
void WriteBYTE(char x)
{
	*outprog++ = x;
}



/******************************************************************************
 함수명 : First
 인  자 : 없음
 리턴값 : 없음
 설  명 : '}' 기호를 발견하면 ifelse 스택을 팝하고 데이타를 처리한다.
 ******************************************************************************/
int First(void)
{
	--ifsp;

	if (ifsp < 0) { Serror(3);  return -1; }       // ifelse 胶琶捞 坷滚敲肺快 灯阑版快
	if (ifelse[ifsp].d != ifsp) { Serror(3); return -1; }  // 胶琶狼 表捞啊 嘎瘤臼阑 版快

	*((int *)(oldoutprog + ifelse[ifsp].off)) =
		outprog - oldoutprog;

	return 1;
}



/******************************************************************************
 함수명 : Inc
 인  자 : 없음
 리턴값 : 없음
 설  명 : 사칙연산, %, |, &, ~ 연산을 수행한다.
 ******************************************************************************/
int Inc(void)
{
	int jj[3][2];               // 첫번째 첨자는 변수인지 숫자인지를 나타냄
	int i;                          // 변수이면 0, 숫자이면 1
	BYTE j;

	jj[0][0] = 0;                   // 첫번째 인자를 얻음, 첫번째 인자는 무조건 변수
	jj[0][1] = variable[TOK].n;

	if (GetToken() == -1) return -1;

	if (strchr("+-/*%|&", *TOKEN))      // 증감연산자등일 경우
	{
		j = *TOKEN;

		if (GetToken() == -1) return -1;
		if (*TOKEN != '=') { Serror(1);  return -1; }

		switch (j)
		{
		case '+': j = add2_;  break;
		case '-': j = sub2_;  break;
		case '*': j = mul2_;  break;
		case '/': j = div2_;  break;
		case '%': j = rest2_; break;
		case '|': j = or2_;   break;
		case '&': j = and2_;  break;
		}
		WriteBYTE(j);

		WriteBYTE((char)variable_);
		WriteInt(jj[0][1]);

		if (GetToken() == -1) return -1;

		switch (TOKENTYPE)
		{
		case VARIABLE:
			WriteBYTE((char)variable_);
			WriteInt(variable[TOK].n);
			break;

		case NUMERIC:
			WriteBYTE((char)value_);
			WriteInt(atoi(TOKEN));
			break;

		default:
		{Serror(1);  return -1; }
		}
	}
	else
	{
		if (*TOKEN != '=') { Serror(1);  return -1; }

		if (GetToken() == -1) return -1;

		switch (TOKENTYPE)             // 두번째 인자가 변수인지 숫자인지를 검사
		{
		case VARIABLE:              // 변수일 경우 처리
			jj[1][0] = 0;
			jj[1][1] = variable[TOK].n;
			break;

		case NUMERIC:               // 숫자일 경우 처리
			jj[1][0] = 1;
			jj[1][1] = atoi(TOKEN);
			break;

		default:                    // 그외의 TYPE이면 에러
		{ Serror(1); return -1; }
		}

		if (GetToken() == -1) return -1;
		if (!strchr("+-/*%|&", *TOKEN))          // 만약 대입문이라면
		{
			PutToken();
			WriteBYTE((char)equal_);

			for (i = 0; i < 2; i++)
			{
				if (jj[i][0] == 0)                  // 변수라면
					WriteBYTE((char)variable_);
				else                                 // 숫자라면
					WriteBYTE((char)value_);

				WriteInt(jj[i][1]);
			}
		}
		else                                         // 대입문이 아니라면
		{
			switch (*TOKEN)
			{
			case '+': j = add_;  break;
			case '-': j = sub_;  break;
			case '*': j = mul_;  break;
			case '/': j = div_;  break;
			case '%': j = rest_; break;
			case '|': j = or_;   break;
			case '&': j = and_;  break;
			default: { Serror(1); return -1; }
			}
			WriteBYTE(j);

			if (GetToken() == -1) return -1;               // 세번째 인자를 얻음

			switch (TOKENTYPE)
			{
			case VARIABLE:
				jj[2][0] = 0;
				jj[2][1] = variable[TOK].n;
				break;

			case NUMERIC:
				jj[2][0] = 1;
				jj[2][1] = atoi(TOKEN);
				break;

			default: { Serror(1); return -1; }
			}

			for (i = 0; i < 3; i++)
			{
				if (jj[i][0] == 0)        // 변수이면
					WriteBYTE((char)variable_);
				else                       // 숫자이면
					WriteBYTE((char)value_);

				WriteInt(jj[i][1]);
			}
		}
	} // end if

	return 1;
}


/******************************************************************************
 함수명 : GetLabel
 인  자 : 없음
 리턴값 : 없음
 설  명 : 라벨명이 있는곳의 위치를 조사하여 label1변수에 셋팅한다.
 ******************************************************************************/
void GetLabel(void)
{
	//    static int i;

	label1[funclabelsp++].n = (int)(outprog - oldoutprog);
}



/******************************************************************************
 함수명 : GetCommand
 인  자 : 없음
 리턴값 : 없음
 설  명 : 기본적인 명령을 처리한다.
 ******************************************************************************/
int GetCommand(void)
{
	int i, j;

	switch (comm[TOK].n)
	{
		// if문을 처리한다.
	case if_:
		WriteBYTE((char)if_);

		if (GetToken() == -1) return -1;
		if (*TOKEN != '(') { Serror(1); return -1; }

		if (GetToken() == -1) return -1;
		// 만약 얻은 토큰이 변수가 아니라면
		if (TOKENTYPE != VARIABLE) { Serror(10); return -1; }

		WriteBYTE((char)variable_);                             // 변수의 값을 저장
		WriteInt(variable[TOK].n);

		if (GetToken() == -1) return -1;
		if (!strchr("<>!=", *TOKEN)) { Serror(1); return -1; }

		// 비교 기호를 찾아서 적당한 처리를 한다.
		// < - 0, <= - 1, > - 2, >= - 3, != - 4, == - 5
		switch (*TOKEN)
		{
		case '<':
			if (GetToken() == -1) return -1;
			if (*TOKEN != '=')          // '<='연산자가 아니라면
			{
				PutToken();
				WriteBYTE(0);
			}
			else
			{
				WriteBYTE(1);
			}
			break;

		case '>':
			if (GetToken() == -1) return -1;
			if (*TOKEN != '=')          // '>='연산자가 아니라면
			{
				PutToken();
				WriteBYTE(2);
			}
			else {
				WriteBYTE(3);
			}
			break;

		case '!':
			if (GetToken() == -1) return -1;
			if (*TOKEN != '=') { Serror(1); return -1; }
			WriteBYTE(4);
			break;

		case '=':
			if (GetToken() == -1) return -1;
			if (*TOKEN != '=') { Serror(1); return -1; }
			WriteBYTE(5);
			break;
		}

		// 비교할 대상의 우변값을 처리한다. 우변값은 변수이거나 수치이다.
		if (GetToken() == -1) return -1;
		switch (TOKENTYPE)
		{
		case NUMERIC:
			WriteBYTE((char)value_);
			WriteInt(atoi(TOKEN));
			break;

		case VARIABLE:
			WriteBYTE((char)variable_);
			WriteInt(variable[TOK].n);
			break;

		default: { Serror(1); return -1; }
		}

		if (GetToken() == -1) return -1;
		if (*TOKEN != ')') { Serror(1); return -1; }

		if (GetToken() == -1) return -1;
		if (*TOKEN != '{') { Serror(1); return -1; }

		// 조건이 거짓일경우 점프할 주소를 가상으로 만들어 0으로 셋팅
		WriteBYTE((char)goto_);
		WriteInt(0);

		ifelse[ifsp].off = outprog - oldoutprog - sizeof(int);
		ifelse[ifsp].d = ifsp;
		ifelse[ifsp].t = 0;
		++ifsp;
		break;

		// else문을 처리한다.
	case else_:
		// goto문에 해당하는 값을 저장
		WriteBYTE((char)goto_);
		WriteInt(0);

		// if에서 거짓일 경우 점프할 주소를 현재의 주소로 셋팅한다.
		*((int *)(oldoutprog + ifelse[ifsp].off)) += (sizeof(int) + 1);

		if (GetToken() == -1) return -1;
		if (*TOKEN != '{') { Serror(1); return -1; }
		if (ifelse[ifsp].d != ifsp) { Serror(1); return -1; }

		ifelse[ifsp].off = outprog - oldoutprog - sizeof(int);
		ifelse[ifsp].d = ifsp;
		ifelse[ifsp].t = 1;
		++ifsp;
		break;

		// goto문을 처리
	case goto_:
		WriteBYTE((char)goto_);
		if (GetToken() == -1) return -1;
		if (TOKENTYPE != LABEL) { Serror(9); return -1; }
		// 라벨명을 label2에 저장. 이 값은 나중에 label1과 비교할때 쓰임
		label2[label2_n].n = (int)(outprog - oldoutprog);
		strcpy(label2[label2_n++].s, TOKEN);
		WriteInt(0);        // 점프할 주소를 임시로 만듬
		break;

		// call문을 처리
	case call_:
		WriteBYTE((char)call_);

		if (GetToken() == -1) return -1;
		if (*TOKEN != '(') { Serror(1); return -1; }

		if (GetToken() == -1) return -1;
		if (TOKENTYPE != USERFUNC) { Serror(15); return -1; }
		strcpy(userfunc2[user2_n].s, TOKEN);
		// call문으로 점프할 가상공간을 확보하고 0을 셋팅
		userfunc2[user2_n++].n = outprog - oldoutprog;
		WriteInt(0);

		if (GetToken() == -1) return -1;
		if (*TOKEN != ')') Serror(1);
		break;

		// end문을 처리
	case end_:
		WriteBYTE((char)end_);
		if (ifsp > 0) { Serror(3); return -1; }
		break;

		// switch문을 처리
	case switch_:
		++switsp;

		if (GetToken() == -1) return -1;
		if (*TOKEN != '(') { Serror(1); return -1; }

		if (GetToken() == -1) return -1;
		if (TOKENTYPE != VARIABLE) { Serror(22); return -1; }
		j = variable[TOK].n;

		if (GetToken() == -1) return -1;
		if (*TOKEN != ')') { Serror(1); return -1; }

		// 프로그램 처음에 조사한 case문의 갯수만큼 공간을 확보
		for (i = 0; i < swit[switsp].total; i++)
		{
			WriteBYTE((char)ifequal_);

			WriteBYTE((char)variable_);
			WriteInt(j);

			// 비교할 대상을 위한 공간을 확보
			swit[switsp].numoff[i] = outprog - oldoutprog;
			WriteBYTE(0);
			WriteInt(0);

			// 라벨명에 현재 스위치의 값을 저장해서 다음에 비교할때
			// 사용한다.
			sprintf(label2[label2_n].s, "case%02d_%02d", switsp, i);
			label2[label2_n++].n = outprog - oldoutprog;
			WriteInt(0);
		}

		// 디폴트문이 있으면 라벨명에 문자열을 추가하여 default문을 실행
		WriteBYTE((char)goto_);
		sprintf(label2[label2_n].s, "defa%02d", switsp);
		label2[label2_n++].n = outprog - oldoutprog;
		WriteInt(0);
		break;

		// case문을 처리
	case case_:
		// 라벨명에 case문에 해당하는 문자열을 추가하여 나중에 처리한다.
		sprintf(label1[label1_n].s, "case%02d_%02d", switsp,
			swit[switsp].count);
		label1[label1_n++].n = outprog - oldoutprog;

		if (GetToken() == -1) return -1;
		switch (TOKENTYPE)
		{
		case NUMERIC:
			*((char *)(oldoutprog +
				swit[switsp].numoff[swit[switsp].count])) = (char)value_;
			j = atoi(TOKEN);
			break;

		case VARIABLE:
			*((char *)(oldoutprog +
				swit[switsp].numoff[swit[switsp].count])) = (char)variable_;
			j = variable[TOK].n;
			break;

		default: { Serror(1); return -1; }
		}

		*((int *)(oldoutprog + 1 +
			swit[switsp].numoff[swit[switsp].count++])) = j;
		break;

		// default문을 처리
	case default_:
		// 라벨명에 default문의 문자열을 입력하여 나중에 처리
		sprintf(label1[label1_n].s, "defa%02d", switsp);
		label1[label1_n++].n = outprog - oldoutprog;
		break;

		// endswitch문을 처리
	case endswitch_:
		// switch문의 끝남을 의미한다.
		if (swit[switsp].def != 1) {
			sprintf(label1[label1_n].s, "defa%02d", switsp);
			label1[label1_n++].n = outprog - oldoutprog;
		}
		sprintf(label1[label1_n].s, "defabreak%02d", switsp);
		label1[label1_n++].n = outprog - oldoutprog;

		swit[switsp].count = 0;
		break;

		// break문을 처리
	case break_:
		WriteBYTE((char)goto_);
		sprintf(label2[label2_n].s, "defabreak%02d", switsp);
		label2[label2_n++].n = outprog - oldoutprog;
		WriteInt(0);
		break;
	}

	return 1;
}




/******************************************************************************
* 함수명 : GetFunction
* 인  자 : 없음
* 리턴값 : 없음
* 설  명 : zs.dat에서 정의한 함수명을 분석하여 알맞은 작업을 한다.
*          i는 int를 의미하며, s는 문자열을 의미한다.
******************************************************************************/
int GetFunction(void)
{
	int i, slen, len, *p;
	int tok = TOK;

	WriteBYTE((char)myfunction[tok].n);          // 함수의 번호를 셋팅

	len = strlen(myfunction[tok].format);  // 매개변수의 길이를 얻음

	if (GetToken() == -1) return -1;
	if (*TOKEN != '(') { Serror(1);  return -1; }

	for (i = 0; i < len; i++) {
		switch (myfunction[tok].format[i]) {
		case 'i':                      // int형을 처리
			if (GetToken() == -1) return -1;
			switch (TOKENTYPE)
			{
			case NUMERIC:
				WriteBYTE((char)value_);
				WriteInt(atoi(TOKEN));
				break;

			case VARIABLE:
				WriteBYTE((char)variable_);
				//WriteInt( atoi( TOKEN ) );
				WriteInt(variable[TOK].n);
				break;

			default:
			{ Serror(6); return -1; }
			}
			break;

		case 's':                      // 문자열을 처리
			if (GetToken() == -1) return -1;
			if (TOKENTYPE != STRING) { Serror(6); return -1; }
			slen = strlen(TOKEN);
			WinZsIncode((char *)TOKEN);        // 문자열을 암호화함
			//strcpy( outprog, (char *)TOKEN );
			WriteBYTE((char)value_);
			p = (int*)outprog;
			WriteInt(slen);                       // 문자열의 길이를기록
			memcpy(outprog, (char*)TOKEN, slen);    // 문자열을 저장
			outprog += slen;
		REPEA:
			if (GetToken() == -1) return -1;
			if (TOKENTYPE == STRING)
			{
				slen = strlen(TOKEN);
				WinZsIncode((char *)TOKEN);        // 문자열을 암호화함
				(*p) += slen;
				memcpy(outprog, (char*)TOKEN, slen);
				outprog += slen;
				goto REPEA;
			}
			else
			{
				PutToken();
			}

			break;

		default: { Serror(6); return -1; }
		}

		// 매개 변수사이의 ','를 처리한다.
		if (len > 1 && i != (len - 1)) {
			if (GetToken() == -1) return -1;
			if (*TOKEN != ',') { Serror(4); return -1; }
		}
	}
	if (GetToken() == -1) return -1;
	if (*TOKEN != ')') { Serror(1);  return -1; }

	return 1;
}



/******************************************************************************
 함수명 : GetUserFunc
 인  자 : 없음
 리턴값 : 없음
 설  명 : 현재의 출력버퍼의 위치를 유저 함수 버퍼에 기록한다.
 ******************************************************************************/
void GetUserFunc(void)
{
	userfunc1[user1_n++].n = outprog - oldoutprog;
}



/******************************************************************************
* 함수명 : Delimeter
* 인  자 : 없음
* 리턴값 : 없음
* 설  명 : 구분자를 받았을 경우 처리한다.
*          '#'은 인클루드문을 의미한다. 인클루드 문이라면 현재 출력버퍼인
*          outprog에 인클루드 화일의 내용을 읽어서 쓴다.
******************************************************************************/
int Delimeter(void)
{
	FILE *fp;
	char temp[1024];
	int i;

	if (*TOKEN != '#') { Serror(1); return -1; }

	if (GetToken() == -1) return -1;
	if (TOKENTYPE != COMMAND || comm[TOK].n != include_) Serror(1);

	if (GetToken() == -1) return -1;
	if (TOKENTYPE != STRING) { Serror(1); return -1; }

	fp = fopen(TOKEN, "rb");
	if (fp == NULL) { Serror(23); return -1; }

	while (1) {
		i = fread(temp, 1, 1024, fp);
		fwrite(temp, 1, i, outfile);
		INCLUDEFILESIZE += i;
		SOURCEFILESIZE += i;
		if (i != 1024) break;

	}
	fclose(fp);

	return 1;
}


/******************************************************************************
 함수명 : Run
 인  자 : 없음
 리턴값 : 없음
 설  명 : 각 명령에 맞는 함수를 호출한다.
 ******************************************************************************/
int  Run(void)
{
	while (1) {
		if (GetToken() == -1) return -1;

		if (TOKENTYPE == END) break;        // 화일의 끝이라면

		else if (*TOKEN == '}') {                // if,else문의 종결자라면
			if (First() == -1) return -1;
			continue;
		}

		else if (TOKENTYPE == NUMERIC)       // 문장의 처음에는 숫자가 올수없다.
		{
			Serror(1); return -1;
		}

		else if (TOKENTYPE == DELIMITER)     // 구분자 명령을 처리
		{
			if (Delimeter() == -1) return -1;;
			continue;
		}

		else if (TOKENTYPE == VARIABLE) {
			if (Inc() == -1) return -1;
			continue;
		}

		else if (TOKENTYPE == LABEL) {
			GetLabel();
			continue;
		}

		else if (TOKENTYPE == COMMAND) {
			if (GetCommand() == -1) return -1;
			continue;
		}

		else if (TOKENTYPE == FUNCTION) {
			if (GetFunction() == -1) return -1;
			continue;
		}

		else if (TOKENTYPE == USERFUNC) {
			GetUserFunc();
			continue;
		}

		{ Serror(1); return -1; }
	}
	if (ifsp > 0) { Serror(3); return -1; }

	return 1;
}



/******************************************************************************
* 함수명 : LoadData
* 인  자 : 없음
* 리턴값 : 없음
* 설  명 : zs.dat를 오픈해서 알맞은 해석을 하고 변수의 이름과 대표값, 함수의
*          이름과 대표값을 셋팅함
******************************************************************************/
int  LoadData(char *szdat)
{
	FILE *fp;
	char temp[FILENAME_MAX];
	DWORD size;
	int i = 0, j;
	BYTE *p;

	sprintf(temp, "./script/%s", szdat);

	fp = fopen(temp, "rb");
	if (fp == NULL) { return 0; }

	size = _filelength(_fileno(fp));

	wzsprog = p = (BYTE *)ZSAlloc(size + 1);
	fread(wzsprog, (size_t)size, 1, fp);
	fclose(fp);

	if (*(wzsprog + size - 1) == 0x1a)
		*(wzsprog + size - 1) = 0;
	else
		*(wzsprog + size) = 0;


	//printf( "\nZS.DAT file compile" );

	while (1) {
		if (GetToken() == -1) return -1;
		if (TOKENTYPE == END) break;

		// 변수를 표시하는 문자열을 찾는다.
		if (strcmp(TOKEN, "variable") != 0) { Serror(1); return -1; }


		// 변수에 해당하는 대표명과 그 값을 variable배열에 셋팅한다.
		while (1) {
			if (GetToken() == -1) return -1;
			if (!strcmp(TOKEN, "end")) break;        // 끝이면 다음으로

			if (TOKENTYPE != STRING) Serror(1);
			j = SearchVariable(TOKEN);                // 같은 변수가 있는지 검사
			if (j != -1) Serror(14);
			strcpy(variable[i].s, TOKEN);

			if (GetToken() == -1) return -1;
			if (TOKENTYPE != NUMERIC) Serror(1);
			j = atoi(TOKEN);
			if (j > 30000) { Serror(18); return -1; }
			variable[i++].n = j;
		}

		i = 0;

		if (GetToken() == -1) return -1;
		if (strcmp(TOKEN, "function") != 0) { Serror(1); return -1; }

		// 함수에 해당하는 대표명과 그 값을 function배열에 셋팅한다.
		while (1) {
			if (GetToken() == -1) return -1;
			if (!strcmp("end", TOKEN)) break;

			if (TOKENTYPE != STRING) { Serror(1); return -1; }
			j = SearchFunction(TOKEN);
			if (j != -1) Serror(14);                 // 같은 함수가 있는지 검사
			strcpy(myfunction[i].s, TOKEN);

			GetToken();
			if (TOKENTYPE != STRING) { Serror(1); return -1; }
			if (!strcmp(TOKEN, "null"))              // 매개변수가 없으면
				myfunction[i].format[0] = 0;
			else
				strcpy(myfunction[i].format, TOKEN);    // 매개변수가 있으면

			if (GetToken() == -1) return -1;
			if (TOKENTYPE != NUMERIC) { Serror(1); return -1; }
			j = atoi(TOKEN);
			if (j > 0xcf) { Serror(18); return -1; }
			myfunction[i++].n = j;
		}

		if (i == 0) { Serror(1); return -1; }
	}
	LINE = 0;
	ZSfree(p);

	return 1;
	//printf( " --------> completed.\n\n" );
}

/******************************************************************************
 함수명 : InitProgram
 인  자 : s = 컴파일하고자 하는 화일의 이름
 리턴값 : 없음
 설  명 : 소스를 메모리로 읽어들인후, 출력할 화일의 이름을 생성시킨다.
 ******************************************************************************/
int  InitProgram(char *s)
{
	FILE *fp;
	char *p;

	fp = fopen(s, "rb");
	if (fp == NULL) {
		exit(1);
	}

	SOURCEFILESIZE = _filelength(_fileno(fp));

	fread(wzsprog, SOURCEFILESIZE, 1, fp);
	fclose(fp);

	if (*(wzsprog + (int)SOURCEFILESIZE - 1) == 0x1a)
		*(wzsprog + (int)SOURCEFILESIZE - 1) = 0;
	else
		*(wzsprog + (int)SOURCEFILESIZE) = 0;

	strcpy(TARGETFILENAME, s);
	p = strchr(TARGETFILENAME, '.');
	strcpy(p, ".ZSC");

	//if( GameMakeFlag == ON )
	//{
	//	outfile = Fopen( TARGETFILENAME, "wb" );
	//	if( fp == NULL ) { 
	//        exit( 1 );
	//	}
	//}

	return TRUE;
}



/******************************************************************************
 함수명 : Path1
 인  자 : 없음
 리턴값 : 없음
 설  명 : 소스를 한번 읽으면서 문법적 에러를 체크한다.
 ******************************************************************************/
int Path1(void)
{
	BYTE *p = wzsprog;
	int j;
	int func = 0;
	int funcdepth = 0;
	int if_else_count = 0;
	int switch_case = 0;
	int switch_count = 0;

	while (1) {
		if (GetToken() == -1) return -1;
		if (TOKENTYPE == END) break;

		// if, else의 갯수를 검사해서 에러를 체크한다.
		if (*TOKEN == '{') ++if_else_count;
		if (*TOKEN == '}') --if_else_count;

		// 라벨명일 경우에는 라벨을 찾아보고, 있으면 라벨 중복 에러를 출력하고
		// 없으면 등록한다.
		if (TOKENTYPE == LABEL)
		{
			if (strlen(TOKEN) > LABEL_LENGTH) { Serror(8); return -1; }
			j = SearchLabel(TOKEN);
			if (j != -1) Serror(14);
			strcpy(label1[label1_n++].s, TOKEN);
		}

		// 유저가 정의한 함수이면 함수를 찾아보고, 있으면 함수 중복 에러를
		// 출력하고 없으며 등록을 한다.
		if (TOKENTYPE == USERFUNC)
		{
			if (TOK != -1) { Serror(15); return -1; }
			strcpy(userfunc1[funcdepth++].s, TOKEN);
			if (func++ != 0) { Serror(13); return -1; }
		}

		if (TOKENTYPE == COMMAND) {
			switch (comm[TOK].n) {
				// switch문의 case와 default문의 갯수를 확인하고 에러를 검사
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

				// call명령문 다음에도 라벨명이 오기때문에 이것을 피한다.
			case call_:
				if (GetToken() == -1) return -1;
				if (GetToken() == -1) return -1;
				if (GetToken() == -1) return -1; break;

				// end문을 처리하며 if-else, switch-endswitch, 그리고 함수가
				// 정상적인가를 판단한다.
			case end_:
				if (func-- != 1) { Serror(13); return -1; }
				if (if_else_count != 0) { Serror(12); return -1; }
				if (switch_case != 0) { Serror(21); return -1; }
				break;

			case goto_:
				if (GetToken() == -1) return -1;
				break;
			}
		}
	}
	if (func > 0) { Serror(13); return -1; }

	ifsp = 0;
	wzsprog = p;
	LINE = 0;

	return TRUE;
}


/******************************************************************************
* 함수명 : EndProgram
* 인  자 : 없음
* 리턴값 : 없음
* 설  명 : 프로그램의 맨 마지막에 실행되며, 라벨명의 위치를 기록,
*          함수명의 위치를 기록, 함수의 갯수를 출력화일의 처음 부분에 기록한다.
		  ******************************************************************************/
int EndProgram(void)
{
	int i, j;
	int size = 0;

	// 원래의 라벨명과 유저가 사용한 라벨명을 서로 비교해서 그값을 출력한다.
	for (i = 0; i < label2_n; i++) {
		j = SearchLabel(label2[i].s);
		if (j == -1)
		{
			strcpy(TOKEN, label2[i].s);
			{ Serror(9); return -1; }
		}

		*((int *)(oldoutprog + label2[i].n)) = label1[j].n;
	}


	// 원래의 함수명과 유저가 사용한 함수명을 서로 비교해서 그값을 출력한다.
	for (i = 0; i < user2_n; i++) {
		j = SearchUserFunc(userfunc2[i].s);
		if (j == -1) {
			strcpy(TOKEN, userfunc2[i].s);
			Serror(15);
		}

		*((int *)(oldoutprog + userfunc2[i].n)) = userfunc1[j].n;
	}

	for (i = 0; userfunc1[i].s[0]; i++) func_off[i] = userfunc1[i].n;
	TotalFunction = i;



	//if( GameMakeFlag == ON )
	//{
	//	// 모든 함수의 갯수를 화일의 처음에 기록을 하고 함수의 옵셋값을 기록한다.
	//	temp = ftell( outfile );
	//	fseek( outfile, 8, SEEK_CUR );        // 헤더를 출력하기위한 여백

	//	for( i = 0; userfunc1[i].s[0]; i++ )
	//	{
	//		fwrite( &userfunc1[i].n, 1, sizeof( int ), outfile );
	//		size += sizeof( int );
	//	}

	//	// 컴파일된 결과를 화일로 출력한다.
	//	size += (outprog - oldoutprog - ( i * sizeof( int )) );
	//	fwrite( oldoutprog, outprog - oldoutprog, 1, outfile );

	//	fseek( outfile, temp, SEEK_SET );
	//	fwrite( &size, 1, sizeof( int ), outfile );        // 화일의 크기를 기록
	//	fwrite( &i, 1, sizeof( int ), outfile );           // 함수의 갯수를 기록
	//	fclose( outfile );
	//}

#ifdef MAKE_EXE_FILE
	printf("\nTotal compile line     : %10d lines\n", --LINE);
	printf("\nSource file size       : %10d BYTEs\n", SOURCEFILESIZE);
	printf("\nTarget file size       : %10d BYTEs\n\n", size + INCLUDEFILESIZE + i * sizeof(int));
#endif

	return TRUE;
}

static char *Filename;
static int Totalcount;

/*===========================================================================
   스프라이트로 만들 모든 화일을 검색한다.
===========================================================================*/
int SearchFile(char *fn)
{
	char temp[FILENAME_MAX], *p, (*f)[FILENAME_MAX];
	//struct find_t ff;
	struct	_finddata_t ff;
	int handle, c;

	strcpy(temp, fn);
	p = strchr(temp, '.');
	if (p)
		strcpy(p, ".ZS");
	else
		strcat(temp, ".ZS");

	f = (char(*)[FILENAME_MAX])Filename;

	handle = _findfirst(temp, &ff);
	if (handle == -1) return 0;
	c = 0;
	while (!c)
	{
		strcpy((char *)f++, ff.name);
		Totalcount++;

		c = _findnext(handle, &ff);
	}

	return Totalcount;
}


#ifdef MAKE_EXE_FILE	
void main(int argc, char *argv[])
{
	int i;
	char temp[FILENAME_MAX], (*f)[FILENAME_MAX];

	printf("\n   ZS Script Compiler For Win95  Version 2.0\n");
	printf("----------------------------------------------------------------\n");

	wzsprog = oldwzsprog = (BYTE *)ZSAlloc(100000 + 1);
	outprog = oldoutprog = (BYTE *)ZSAlloc(50000);
	Filename = ZSAlloc(FILENAME_MAX * 2000);

	if (argc == 1)	strcpy(temp, "*.*");
	else			strcpy(temp, argv[1]);

	if (SearchFile(temp)) LoadData();

	f = (char(*)[FILENAME_MAX])Filename;
	for (i = 0; i < Totalcount; i++, f++)
	{
		wzsprog = oldwzsprog;
		outprog = oldoutprog;
		memset(wzsprog, 0, SOURCEFILESIZE + 1);

		memset(label1, 0, sizeof(COMMSET)	*	MAX_SCR_ARRAY);            // 원래 라벨의 내용을 저장하는 변수
		memset(label2, 0, sizeof(COMMSET)	*	MAX_SCR_ARRAY);            // goto문 다음에 쓰인 라벨을 저장
		memset(ifelse, 0, sizeof(IF_ELSE)	*	MAX_SCR_ARRAY);            // if, else문의 내용을 저장
		memset(userfunc1, 0, sizeof(COMMSET)	*	MAX_SCR_ARRAY);            // 원래 함수명의 내용을 저장
		memset(userfunc2, 0, sizeof(COMMSET)	*	MAX_SCR_ARRAY);            // call문 다음에 쓰인 함수를 저장
		memset(swit, 0, sizeof(SWITCHCASE) * 50);            // 하나의 switch문에서 각각의 case문을저장

		printf(" %3d/%3d     : %13s File Compiling...........\n", i + 1, Totalcount, (char*)f);


		ifsp = 0;                            // if, else의 스택값
		switsp = 0;                          // switch의 변위값
		label1_n = 0, label2_n = 0;              // 원래의 라벨값과 쓰여진 라벨값을 저장
		user1_n = 0, user2_n = 0;                // 원래의 함수값과 


		InitProgram((char*)f);
		Path1();
		Run();
		EndProgram();
	}

	printf("\n    %d ZS Files Compiled ..\n", Totalcount);
	printf("    %d Error occured...\n", ErrorCount);
	printf("-------------------------------------------Copyright(c),1995 F.E.W \n");
	printf("-------------------------------------------------- Coded by Phyneix  \n");

	ZSfree(oldwzsprog);
	ZSfree(oldoutprog);
	ZSfree(Filename);

	_getch();
}

#else


void InitTextScript(void)
{
	oldwzsprog = (BYTE *)ZSAlloc(100000 + 1);
	oldoutprog = (BYTE *)ZSAlloc(50000);
}


void EndTextScript(void)
{
	if (oldwzsprog != NULL) { ZSfree(oldwzsprog); oldwzsprog = NULL; }
	if (oldoutprog != NULL) { ZSfree(oldoutprog); oldoutprog = NULL; }
}

void NotePad(char *name)
{
	//"NotePad.exe"
	//ShellExecute(NULL, NULL, SysInfo.notepad, name, NULL, SW_SHOWNORMAL);
}


BYTE *LoadTextScript(char *name)
{
	LINE = 0;

	TOK = 0; // ^^^^ 0308
	memset(variable, 0, sizeof(COMMSET) * 2000);	//MAX_SCR_ARRAY );             // 사용자가 정의한 변수의 내용을 저장
	memset(myfunction, 0, sizeof(USER_FUNC) * 1000); //MAX_SCR_ARRAY );           // 유저가 정의한 함수를 저장

	LoadData("Zs.Dat");

	wzsprog = oldwzsprog;
	outprog = oldoutprog;

	strcpy(Wz_scriptfilename, name);

	memset(wzsprog, 0, 100000 + 1);
	memset((char*)label1, 0, sizeof(COMMSET) * 100); // MAX_SCR_ARRAY );            // 원래 라벨의 내용을 저장하는 변수
	memset((char*)label2, 0, sizeof(COMMSET) * 100); // MAX_SCR_ARRAY );            // goto문 다음에 쓰인 라벨을 저장
	memset((char*)ifelse, 0, sizeof(IF_ELSE) * 500); // MAX_SCR_ARRAY );            // if, else문의 내용을 저장
	memset((char*)userfunc1, 0, sizeof(COMMSET) * 1000);//MAX_SCR_ARRAY );            // 원래 함수명의 내용을 저장
	memset((char*)userfunc2, 0, sizeof(COMMSET) * 1000); //MAX_SCR_ARRAY );            // call문 다음에 쓰인 함수를 저장
	memset((char*)swit, 0, sizeof(SWITCHCASE) * 50);						// 하나의 switch문에서 각각의 case문을저장

	ifsp = 0;                   // if, else의 스택값
	switsp = 0;                   // switch의 변위값
	label1_n = 0, label2_n = 0;   // 원래의 라벨값과 쓰여진 라벨값을 저장
	user1_n = 0, user2_n = 0;   // 원래의 함수값과 
	funclabelsp = 0;

	if (InitProgram(name) == -1) goto ERROR_;
	if (Path1() == -1) goto ERROR_;
	if (Run() == -1) goto ERROR_;
	if (EndProgram() == -1) goto ERROR_;

	CLoadScript = SCRIPT_NEW;
	return oldoutprog;
ERROR_:

	char nn[FILENAME_MAX];
	strcpy(nn, Wz_scriptfilename);

	for (int i = strlen(nn) - 1; i > 0; i--)	   if (*(nn + i) == '\\') break;

	//sprintf( temp, "'%s'를 다시 로드 합니다.", strupr(nn+ i +1)  );
	//if( YesOrNo( temp, "확인") == IDYES ) goto AGAIN_;

	return NULL;
}


#endif  // !MAKE_EXE_FILE







