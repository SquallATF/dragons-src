#pragma once

#define MSG_SHUT_DOWN				100	//ShutDown
#define MSG_RELOAD_GAMESERVER_DATA  101 //Reload Gameserver data
#define MSG_CHANGE_WEATHER			102
#define MSG_ECHO					103

#define MSG_RM_KICKOFF_USER			111	// 030224 kyo 유저 강제로그아웃
#define MSG_RM_KICKOFF_USER_ALL		112 // 030716 kyo 유저 몽땅 강제 로그아웃(위험해서 안씀)

#pragma pack (push,1)
struct PACKET_SHUT_DOWN
{
	BYTE bPtcl;
	BYTE bHeader;
};


struct PACKET_RELOAD_GAMESERVER_DATA
{
	BYTE bPtcl;
	BYTE bHeader;
};

struct PACKET_CHANGE_WEATHER
{
	BYTE bPtcl;
	BYTE bHeader;
	BYTE bWeather;
	BYTE bStopWeather;
	DWORD dwAmount;
};



#pragma pack(pop)