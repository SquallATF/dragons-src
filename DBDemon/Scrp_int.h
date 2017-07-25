#ifndef __SCRIPT_INT_H__
#define __SCRIPT_INT_H__

const int SCRIPT_OLD = 0;
const int SCRIPT_NEW = 1;

enum 
{
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


#ifndef __SCRIPT_C__

    
	extern int  NScriptNo;    //SCRIPT_NO;
    extern int  CLoadScript;  //LOAD_SCRIPT;
    
	extern int  TotalFunction;

#endif

void Incode( char *s );

/* 
 窃荐疙 : GetByte
 牢  磊 : 绝澜
 府畔蔼 : 1官捞飘狼 蔼
 汲  疙 : prog俊 扁废等 郴侩俊辑 1官捞飘甫 府畔窍绊 prog器牢磐甫 1刘啊
 */
BYTE GetByte( void );


/* 
 窃荐疙 : GetInt
 牢  磊 : 绝澜
 府畔蔼 : 4官捞飘狼 蔼
 汲  疙 : prog俊 扁废等 郴侩俊辑 4官捞飘甫 府畔窍绊 prog器牢磐甫 4刘啊
 */
int GetInt( void );


/* 
 窃荐疙 : GetString
 牢  磊 :
 府畔蔼 : 巩磊凯阑 历厘窍绊 乐绰 硅凯狼 矫累 器牢磐
 汲  疙 : 胶农赋飘 单捞鸥俊辑 巩磊凯阑 佬绢甸牢促.
 */
void GetString( char *buf );


/* 
 窃荐疙 : Script
 牢  磊 : 绝澜
 府畔蔼 : 绝澜
 汲  疙 : 胶农赋飘 林 角青扁
 */
void Script( int num );


/* 
 窃荐疙 : LoadScript
 牢  磊 : name - 佬绢甸捞绊磊 窍绰 胶农赋飘 单捞鸥 拳老疙捞 酒聪扼 老访锅龋.
 府畔蔼 :
 汲  疙 : 沥惑利栏肺 佬绢甸捞搁 1阑 府畔茄促.
 */
int LoadScript( char *scriptname );
int LoadScriptText( char *scrname );




#endif


