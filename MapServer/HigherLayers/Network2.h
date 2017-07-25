﻿#pragma once

extern int WaitMoveMap ;

#define MAX_BOX_ITEM			15
#define MAX_GOD					4

#define MAX_MSG					200
#define MAX_GUILD_HOUSE			200
////////////////////////////////////////////////////////////////////////////////////
#define CMD_RESEND_ITEM			10001		// 인벤토리 확인을 위한 서버에서 새로 전송
#define CMD_RESEND_CHARLIST		10003       //
#define CMD_MAGIC_STRIKE		10004		// 매직에 당했을 때,
#define CMD_OTHER_CH_INV		10005		// 다른 사람의 아이템 목록을 볼때
#define CMD_DIVIDE_ITEM			10006		// 아이템 나누기 인터페이스에 의한 서버 콜
#define CMD_CREATE_ITEM			10007		// 아이템 생성
#define CMD_BOX_OPEN			10008		// 박스 오픈

#define CMD_RESEND_ITEM_EQUIP	10009		
#define CMD_RESEND_ITEM_QUICK	10010

#define CMD_DELETE_ITEM			10011		// 아이템 부서짐
#define CMD_DECREASE_ITEM		10012		// 아이템 dur 감소

#define CMD_LVUP_POINT_EACH		10013		// 각각의 레벨업 수치 전송

#define REQ_LEVELUP_POINT		10014		// 레벨업 포인트를 요청한다. 임시
#define CMD_LVDOWN_POINT_EACH	10015		// 각각의 레벨업 수치 전송
#define CMD_LEVELDOWN_OK		10016		// 레벨다운 오케이

#define CMD_LEARN_ITEM			10017		// 아이템 배우기 
// #define CMD_LEARN_ITEM_OK		10018		// 아이템 배우기 성공
#define CMD_ITEMREPAIR_REQ		10019		// 아이템 고치기
#define CMD_ITEMREPAIR_OK		10020		// 아이템 고치기 성공
#define CMD_RESULT				10021		// 공용으로 쓰일 서버계산 결과 (예 -1:돈이 부족)
#define CMD_LEARN_SKILL_OK		10022		
#define CMD_SKILL_EXP			10023
#define CMD_OPEN_STATUS_REQ		10024		// 스테이터스 창이 열릴때

#define CMD_REQ_CHAR_INFO_PLUS	10025		// 캐릭터 추가 정보 요청

#define CMD_SEND_CHAR_DISEASE	10026		
#define CMD_PARTY				10028
#define CMD_RELATION			10029
#define CMD_EMPLOYMENT			10030
#define CMD_ITEM_INDEX			10031	
#define CMD_DEFRAYAL			10032
#define CMD_SEND_BANK_INV		10033

#define CMD_DO_PARTY			10034
#define CMD_DELETE_PARTY		10035

#define CMD_REQ_PARTY_MEMBER	10036

#define CMD_REQ_LEARN_MAGIC		10037

#define CMD_CLIENT_CHAR_NAME	10038

#define CMD_LEARN_MAGIC_OK		10039
#define CMD_PREVENTING			10040		// 질병 예방	// 돈이 부족할 수 있기에 결과를 보내준다.
#define CMD_CURE_DISEASE		10041		// 질병 치료	// 100%이기에 결과를 보내주지 않는다.

#define CMD_REQ_ITEM_VALUE				10042
#define CMD_REQ_ITEM_VALUE_SELL			10043
#define CMD_REQ_ITEM_VALUE_REPAIR		10044

#define CMD_SERVER_PARTY				10045
#define CMD_CLIENT_INN					10046
//#define CMD_TAC_SKILL_EACH_EXP			10047

#define CMD_SERVER_WAKE_UP				10048

#define CMD_REQ_JOIN_GAME				10049	// 게임에 들어왔다는 것을 알린다.
#define CMD_SERVER_EXIT_GAME			10050	// 게임에서 나갔다는 것을 알린다.
#define CMD_REQ_MY_PARTY				10051

#define CMD_PARTY_JOIN_OK				10052
#define CMD_PARTY_JOIN_RELEASE			10053

#define CMD_SERVER_EACH_ITEM			10054
#define CMD_OPEN_ABILITY				10055
#define CMD_SUBTRACT_MONEY				10056
#define CMD_ADD_MONEY					10057

#define CMD_LEARN_ALL					10058
#define CMD_LEARN_ALL_NOT				10059


#define CMD_EXCHANGE_ITEM_READY			10060		// 거래하기 위해 내가 물건을 넘겼다
#define CMD_EXCHANGE_ITEM_REQ			10061		// 누군가 당신에게 물건 거래를 요청했다.
#define CMD_EXCHANGE_ITEM				10062
#define CMD_EXCHANGE_ITEM_RESULT		10063		// 거래 신청에 대한 결과 전송
#define CMD_EXCHANGE_OK_SIGN			10064
#define CMD_EXCHANGE_CANCEL				10065
#define	CMD_EXCHANGE_ITEM_EACH			10066
#define CMD_EXCHANGE_ITEM_DEL			10067

#define	CMD_CHECK_BETA_TEST				10068
#define	CMD_CHECK_BETA_TEST_OK			10069
#define	CMD_CHECK_BETA_TEST_NOT			10070

#define CMD_IATTR_DURATION				10071
#define CMD_IATTR_MUCH					10072
#define CMD_IATTR_LIMIT					10073
#define CMD_IATTR_ATTR					10074
#define CMD_IATTR_ID					10075
#define CMD_IATTR_RESERVED0				10076
#define CMD_IATTR_RESERVED1				10077

#define CMD_REQ_AC						10078
#define CMD_OTHER_GHOST					10079
#define CMD_REQ_DMG						10080
#define CMD_SERVER_KILLMON				10081

#define	CMD_PARTY_ACCESS				10082
#define CMD_REQ_PARTY_TOGETHER			10083		// 서로 동맹을 맺고 있는지 알아보기 위해 로그인 서버로 디비를 요청한다.

#define CMD_PARTY_NO_NAME				10084
#define CMD_RELATION_NO_NAME			10085
#define CMD_EMPLOYMENT_NO_NAME			10086

#define CMD_REQ_MAGERY					10087

#define CMD_THROW_DICE					10088		// 주사위 던지기...
#define CMD_CREATE_ABILITY				10089		// 기본수치 랜덤 생성

#define	CMD_PUBLIC_NOTICE_BBS_REQ		10090
#define	CMD_RANK_BBS_REQ				10091
#define	CMD_FORUM_BBS_REQ				10092
#define	CMD_EVENT_BBS_REQ				10093	
#define	CMD_MYDIARY_BBS_REQ				10094
#define	CMD_BBS_BBS_REQ					10095
#define	CMD_ETC_BBS_REQ					10096

#define CMD_GET_SERVER_TEXT				10097
#define CMD_LOGIN_GET_BOX_ITEM			10098
#define CMD_PUT_BOX_ITEM				10099		// 로그인 서버로 전송

#define CMD_PREACH_NAME					10100		// 전도자 이름

#define CMD_GOD_REGIST_OK				10101		// 신전에 등록한다.
#define CMD_GOD_REGIST_CANCEL			10102		// 신전등록되어 있는 것을 취소시킨다.

#define CMD_LOGIN_PREACH_NAME			10103		// 로그인 서버로 보내 쿼리문을 실행한다.
#define CMD_LOGIN_PREACH_NAME_OK		10104
#define CMD_LOGIN_PREACH_NAME_CANCEL	10105			
#define CMD_PREACH_NAME_OK				10106		// 전도 한 사람 신앙수치를 올려 주자

#define CMD_NOW_GOD_MEETING_START		10107		//	이젠 예배 시간이다
#define CMD_NOW_GOD_MEETING_END			10108		//	이젠 예배 시간끝이다. 

#define CMD_TOOL_PASSWORD				10109		// 툴을 사용하기 위한 패스워드

#define CMD_CHECK_NEW_CHAR				10120		// 캐릭터 초기화
#define CMD_CHECK_NEW_CHAR_OK			10121
#define CMD_CHECK_NEW_CHAR_NOT			10122

#define CMD_LOGIN_EVANGELIST_CREATE		10123
#define CMD_LOGIN_EVANGELIST_DELETE		10124

#define CMD_GET_GOD_TABLE				10125
#define CMD_MEETING_TEXT				10126

#define	CMD_CHAR_UPDATE_DATA_BY_KEIN	10127
#define	CMD_GET_CHARINFO_BY_KEIN		10128		// 처음 신앙수치등의 캐릭터 정보를 게임서버로 보내줄때

#define CMD_SERVER_KILLPC				10129		// 0410 KHS추가...

#define CMD_CAN_MEMORIZE				10130		// 메모라이즈 할수 있는가...
#define CMD_CAN_MEMORIZE_OK				10131		// 메모라이즈 가능하다
#define CMD_CAN_MEMORIZE_NOT			10132		// 메모라이즈 할 시간이 아니다.

#define CMD_SEND_EAR_MESSAGE			10133		// 귓속말 전송
#define CMD_SEND_EAR_MESSAGE_OK			10134		
#define CMD_SEND_EAR_MESSAGE_NOT		10135		
#define CMD_SEND_EAR_MESSAGE_OTHER		10136		// 귓속말을 전송할때 다른 맵에 있는지 알아 본다.
#define CMD_SEND_EAR_MESSAGE_RESULT		10137		// 받은 귓속말에 대한 결과
#define	CMD_LOGIN2GAME_EAR_MESSAGE_RESULT	10138	// 로그인 서버가 속해 있는 게임서버로 보내준다.
#define CMD_GAME2CLIENT_EAR_RESULT			10139	// 보낸 귓속말에 대한 회신
#define CMD_GAME2LOGIN_CHECK_NAME			10140

#define CMD_GAME2LOGIN_MOVING_ITEM			10141	// 아이템 이동을 캐릭터 별로 저장한다.

#define CMD_CHANGE_MAP_KEIN					10149	// 다른 맵으로 이동

#define CMD_ACCESS_JOIN_KEIN				10150	// connections에 아이디와 이름만 등록한다.
#define CMD_LOGOUT_KEIN						10151	// 로그아웃을 보내준다. ( 맵전환시 ) 업데이트는 하지 않는다.

#define CMD_ACCEPT_JOIN_KEIN				10152

#define CMD_CHANGE_MAP_DATA1				10153	// 다른 맵으로 이동 데이타 전송1
#define CMD_CHANGE_MAP_DATA2				10154	// 다른 맵으로 이동 데이타 전송2
#define CMD_CHANGE_MAP_DATA3				10155	// 다른 맵으로 이동 데이타 전송3
#define CMD_CHANGE_MAP_DATA4				10156	// 다른 맵으로 이동 데이타 전송4
#define CMD_CHANGE_MAP_DATA5				10157	// 다른 맵으로 이동 데이타 전송5
#define CMD_CHANGE_MAP_DATA6				10158	// 다른 맵으로 이동 데이타 전송6
#define CMD_CHANGE_MAP_DATA7				10159	// 다른 맵으로 이동 데이타 전송7
#define CMD_CHANGE_MAP_DATA8				10160	// 다른 맵으로 이동 데이타 전송8
#define CMD_CHANGE_MAP_DATA9				10161	// 다른 맵으로 이동 데이타 전송9

#define CMD_RECALL_BOX_ITEM					10162	// 로그인 서버로 박스 아이템을 다시 요청한다.
#define CMD_IF_DOUBLE_BOX_THEN_DELETE		10163	// 똑같은 박스가 있을 경우 지운다.

#define CMD_CHANGE_MAP_CONFIRM				10164	// 맵간 이동시 시간지연으로 에러가 발생하면 서버로 확인을 요청한다.
#define CMD_CHANGE_MAP_FAIL					10165	// 맵이동에 문제가 생겼다.
#define CMD_NOW_CHANGEMAP					10166	// 맵이동 중이다...

#define	CMD_REQ_SKILL_ALL					10167	// 스킬 경험치 요청

#define CMD_REQ_FAITH_UP					10168	// 다른 맵에 접속해 있는지 확인해서 신앙심을 올려준다.
#define CMD_DIVINE_UP						10169	// divine을 올려준다. 
#define CMD_DIVINE_DOWN						10170	// divine이 떨어짐을 클라이언트에 알려준다.
#define CMD_OPEN_SUPPER						10171	// 신전에서 제공받는 음식을 요청한다.

#define CMD_I_GET_ITEM_OF_SUPPER			10172	// 신전에서 음식을 제공 받는다.

#define CMD_NOTE_MEETING					10173	// 미팅 한시간 전인것을 알린다.
#define	CMD_NOT_EXIST_GAMESERVER			10174	// 게임서버가 하나도 존재하지 않는다면...

#define CMD_REQ_CHAR_INFO					10175	// 캐릭터의 정보를 전송받는다.
#define CMD_OH_MY_PARTY						10176	// 내가 들어 왔당

#define CMD_REQ_PARTY_INFO					10177	// 현재 맺고 있는 캐릭터 상태 정보를 받아 온다.

#define CMD_LOGIN_TO_GAME_PUBLIC_BBS		10178	// 로그인서버에서 클라이언트로 공지사항을 보낼때 
#define CMD_CALL_PUBLIC_BBS					10179	// 공지사항

#define CMD_SEND_MAIL_OK					10180	// 메일 보내기 성공
#define CMD_SEND_MAIL_FAIL					10181	// 메일 보내기 실패

#define CMD_START_GAME_MAIL					10182	// 처음 들어왔을때 메일 확인
#define CMD_SEND_LEARN_ITEM_INDEX			10183	// 처음 배운 아이템 목록..

#define CMD_CALL_PUBLIC_BBS_ONLY_ONE		10184	// 한명에게 보내주는 공지사항..
#define CMD_HUNTER_REGIST					10185
#define CMD_GET_HUNTER_LIST					10186
#define CMD_SEND_HUNTER_LIST				10187

#define	CMD_KILL_HUNTER_LIST				10188	// 현상범을 잡았다.
#define	CMD_KILL_HUNTER_LIST_OK				10189	// 메시지 출력, 축하합니다. 현상금 얼마를 받았습니다...
#define	CMD_KILL_HUNTER_LIST_CANCEL			10190	// 결과		// 늦었습니다. 이미 누군가 먼저 현상금을 받았습니다.
#define CMD_HUNTER_REGIST_RESULT			10191	// 누구누구가 당신께서 현상범 게시판에 등록한 누구누구를 죽이는데 성공했습니다.
#define CMD_KILLED_BY_HUNTER				10192

#define CMD_KILL_WHO						10193	// 내가 누굴 죽였다.
#define CMD_DANGER_OF_HUNTER				10194	// 누군가 나를 노린다.

#define CMD_REQ_QUEST_INDEX					10195
#define CMD_REP_QUEST_INDEX					10196

#define CMD_PARTY_NOT_BY_LEVEL				10197
#define CMD_DO_PARTY_OK						10198

#define	CMD_REQ_MAN_TO_MAN					10199	// 1:1 결투 신청
#define	CMD_REQ_MAN_TO_MAN_RESULT			10200	// 결투 신청에 따른 결과( 성공적으로 신청했다, 아님 저쪽이 거절 상태이다.. 레벨이 안된다.. 등등의.. )
#define CMD_REQ_MAN_TO_MAN_CONSENT			10201	// 결투 승낙
#define CMD_REQ_MAN_TO_MAN_REFUSAL			10202	// 결투 거절
#define	CMD_DELETE_BOX_ITEM					10203
///////////////////////////////////////////////////////////////////////////////////


#define	CMD_MAN_TO_MAN_ITEM_EACH			10204	// 1:1 대결 할려고 할때 걸게 되는 아이템
#define CMD_FIGHT_MAP_MESSAGE				10205	// 전투 맵에서 여러 메시지를 출력할때...
#define CMD_FIGHT_MAP_ADD_OK				10206	// 참여했다
#define	CMD_FIGHT_MAP_ADD_WHO				10207	// 누군가 참여했다.
#define	CMD_FIGHT_MAP_DELETE_WHO			10208	// 누군가 팀에서 빠졌다.
#define	CMD_FIGHT_MAP_MEMBER_INFO			10209	// 처음으로 배틀존에 등록하면 그동안 등록한 사람의 정보를 보내준다.
#define CMD_FIGHT_MAP_START					10210	// 운영자가 배틀존 스타트를 알린다.
#define	CMD_FIGHT_MAP_END					10211	// 운영자가 배틀존이 끝났음을 서버에 알리다
#define	CMD_FIGHT_MAP_ADD_FULL				10212	// 인원이 꽉 찼다



#define CMD_RESET_JOB						10213	// JOB를 새로 선택
#define	CMD_DELETE_ITEM_BY_DURATION			10214	// 아이템이 사용에 의해 부서질 경우
#define CMD_SELECT_NATION					10215	// 나라 선택 

#define CMD_SCRIPT_CHANGE					10216

#define CMD_CHANGE_DURATION					10217
#define	CMD_SEND_EAR_MESSAGE_RESULT_ETC		10218	// 귓속말에 대한 몇몇 결과값
#define CMD_EXIST_NAME						10219	// 존재하는 이름이냐?

#define CMD_ISSAME_MY_NATION				10220	// 나와 나라가 같은가?
#define CMD_EXIST_NAME_AND_NATION			10221

#define	CMD_VOTE_GAME2LOGIN					10223	// 투표관련된 게임 서버와 로그인 서버간의 패킷..
#define CMD_VOTE_OK							10224	// 선거를 했다.
#define	CMD_VOTE_NOT						10225	// 선거할수 없다.

#define CMD_REGIST_CANDIDATE				10226	// 후보 등록
#define CMD_REGIST_CANDIDATE_OK				10233	// 후보 등록 성공
#define CMD_REGIST_CANDIDATE_NOT			10234	// 후보 등록 실패

#define CMD_IS_THIS_MAN_CANDIDATE			10227	// 후보자 명단에 있는 사람이냐?
#define CMD_THIS_MAN_IS_CANDIDATE_OK		10228	// 명단에 있다.
#define CMD_THIS_MAN_IS_CANDIDATE_NOT		10229	// 명단에 없다.

#define CMD_CHECK_VOTE						10230	// 투표할수 있는지 없는지를 체크
#define CMD_CHECK_VOTE_NOT					10231	// 투표할수 있는지의 결과
#define CMD_CHECK_VOTE_OK					10232	// 투표할수 있다.

#define CMD_SET_VALUE_OF_VOTE				10235	// 선거 시스템에서 사용할 데이타 가져오기
#define CMD_RESET_VOTE_VALUE				10236
#define CMD_RETRY							10237	// 다시 시도 해달라는 메시지를 뿌리게 한다.

#define CMD_REQ_VOTE_SCORE					10238	// 클라이언트에서 디비데몬으로
#define CMD_REP_VOTE_SCORE					10239	// 디비 데몬에서 클라이언트로 

#define CMD_GET_SKILL_INFO					10240	// 마우스 가져다 댔을때 뜨는 스킬 레벨 요청

#define CMD_SEND_ALL_MSG					10241	// 국가 공지
#define CMD_RECV_ALL_MSG					10242	

#define CMD_YOU_ARE_KING_LOG2LOG			10243	// 넌 왕이다. ( 데몬에서 5000번으로 보낸다. )
#define CMD_YOU_ARE_KING_LOG2CLIENT			10244	// 넌 왕이다. ( 5000에서 클라이언트로 보낸다. )

#define CMD_YOU_ARE_NO_KING_LOG2LOG			10245	// 넌 왕에서 해임. ( 데몬에서 5000번으로 보낸다. )
#define CMD_YOU_ARE_NO_KING_LOG2CLIENT		10246	// 넌 왕에서 해임. ( 5000에서 클라이언트로 보낸다. )

#define CMD_GUILD_EXIT_COMMAND				10247
#define CMD_GUILD_EXIT_YOU					10248	// 넌 탈퇴당했다.
#define CMD_GUILD_EXIT_OK					10249	// 탈퇴시키는데 성공했다. 
#define CMD_GUILD_EXIT_WHO					10250	// 탈퇴당한 사람 주변 사람들에게 뿌려준다.

#define CMD_GUILD_JOIN_SERVER				10251	// 길드 가입 시켜라 ( db_demon 까지 보낸다. )
#define CMD_GUILD_JOIN_SERVER_OK			10252	// 길드 가입 가능하다.
#define CMD_GUILD_JOIN_SERVER_NOT			10253	// 길드 가입 불가능하다.

#define CMD_GUILD_JOIN_CONFIRM				10254	// 당사자에게 확인을 물어 본다.
#define CMD_GUILD_JOIN_CONFIRM_OK			10255	// 허락했다.
#define CMD_GUILD_JOIN_CONFIRM_NOT			10256	// 거부했다.
#define CMD_SCAN_TARGET						10257	// 길마가 사용하는 디텍트

#define CMD_GUILD_ALL_MSG					10258	// 길드 전체 메시지
#define CMD_GUILD_ALL_MSG_RECV				10259	// 길드 전체 메시지 // 서버에서
#define CMD_GUILD_EACH_LEVEL				10260	// 길드내 등급을 바꾸는 명령
#define CMD_GUILD_JOIN_CONFIRM_GUILMA		10261	// 길드 가입 확인을 보냈다는 사실을 길마에게도 보내준다.

#define CMD_GM_REGIST						10262	// GM 등록을 신청
#define CMD_GM_REGIST2						10263	// GM에 등록할 자격은 안되지만 비슷한 사람
#define CMD_BBS_ALL							10264	// 디폴트 추카 공지 사항
#define CMD_COME_OUT_GM_FROM_LOGIN			10265	// GM 등록에 성공했다.

#define CMD_CHANGE_VIEW_JOB					10266	// change_job
#define CMD_REQ_STOP_WAR_TO_LOGIN			10267	// 휴전 요청
#define CMD_REQ_STOP_WAR_TO_CLIENT			10268	// 휴전 요청을 클라이언트로

#define CMD_REP_STOP_WAR_REFUSAL_TO_LOGIN	10269	// 휴전 거절
#define CMD_REP_STOP_WAR_REFUSAL_TO_CLIENT	10270	

#define CMD_REP_STOP_WAR_ACCEPT_TO_LOGIN	10271	// 휴전 승낙
#define CMD_REP_STOP_WAR_ACCEPT_TO_CLIENT	10272

// Added by chan78 at 2000/11/25 :: New commands.
#define CMD_MAP_CHANGE_REQUEST				10273
#define CMD_MAP_CHANGE_ACCEPTED				10275
#define CMD_MAP_CHANGE_DENIED				10276

// Added by chan78 at 2000/11/28
// 001212 YGI
#define CMD_SCRIPT_MESSAGE_BOX				10273	// 스크립트로 메시지 박스를 띄운다.
#define CMD_SEND_SOUND						10274	// 클라이언트에 사운드...

#define	CMD_SALVATION_MONEY					10275	// 기부한다.
#define CMD_REQ_SALVATION_MONEY				10276	// 지금까지 기부한거..
#define CMD_DB2GAME_SALVATION				10277	// 게임서버로 보내준다.

#define CMD_GM_REGIST_GAMEMAKEMODE			10278
#define CMD_MSG_ALL_SALVATION_TOTAL_MONEY	10279	// 기부금 총액과, 나라 이름을 가르쳐 준다.

// 010210 YGI
#define CMD_CHANGE_MONEY					10280	// 환전
#define CMD_CHANGE_MONEY_OK					10281	// 돈을 바꿨다.


#define CMD_EAT_CHOCOLATE					10282	// 초콜렛을 먹었다.
#define CMD_EAT_CHOCOLATE_OK				10283	// 몇개나 먹었냐..

#define CMD_PAY_RUMOR						10284
#define CMD_PAY_RUMOR_OK					10285
#define CMD_PAY_RUMOR_NOT					10286

#define CMD_TEAMBATTLE_INFO					10287
#define CMD_OPENSCHOOL_DATA					10288	// 개학 이벤트를 위한 정보 저장
#define CMD_OPENSCHOOL_DATA_OK				10289

#define CMD_GUILD_EXIT_COMMAND_GLOBAL		10290
#define CMD_GUILD_EXIT_COMMAND_GLOBAL_NOT	10291	// 같은 길드원이 아니다.
#define CMD_GUILD_EXIT_COMMAND_GLOBAL_OK	10292

#define CMD_GUILD_EXIT_COMMAND_GLOBAL_LET	10293	// 모든 맵서버에게 보내 그 놈을 탈퇴시켜라 명령한다.

#define CMD_EAT_CANDY						10294
#define CMD_EAT_CANDY_OK					10295

#define CMD_REQ_LOAN						10296		// 대출을 요구한다.
#define CMD_REQ_LOAN_INFO					10297		// 대출관련 정보를 요구한다.
#define CMD_REQ_REPAYMENT					10298		// 상환한다.

#define CMD_YOU_FIGHT_LEADER				10299		// 리더
#define CMD_FIGHT_MAP_ADD_WAIT				10300		// 파이터 맵의 대기자 등록
#define CMD_FIGHT_MAP_DELETE_WAIT			10301		// 파이터 맵의 대기자 등록
#define CMD_FIGHT_MEMBER_TO_WAIT_MEMBER		10302		// 다시 대기자로 돌아가기
#define	CMD_WHO_FIGHT_LEADER				10303		// 누군가 파이트 맵 리더가 되었다.

#define CMD_FIGHT_MAP_YOU_WIN				10304		// 이겼다.
#define CMD_FIGHT_MAP_YOU_DEFEAT			10305		// 졌다.


#define	CMD_FIGHT_MAP_CHAR_INFO				10306		// 캐릭 결투 정보를 가져온다.

#define CMD_MERCHANT_BBS_ALL				10307		// 상인메뉴를 통해 온 전체공지
#define CMD_MERCHANT_BBS					10308		
#define	CMD_MERCHANT_BBS_MAP_TO_MAP			10309

#define	CMD_FIGHT_MAP_YOU_DISCONNECT		10310		// 비겼다.
#define CMD_FIGHT_MEMBER_TO_EXIT			10311		// 팀에서 빠진다.

#define CMD_FIGHT_MAP_REMAIN_TIME			10312		// 남은 시간
#define CMD_FIGHT_MAP_ADD_FAIL_FIGHT_START	10313		// 이미 시작해서 더이상 팀등록을 못한다.

#define	CMD_PARTY_ALL_FREE					10314		// 파티 모두 해제

#define CMD_MERCHANT_ITEM_BBS_ALL			10315		// 상인메뉴 아이템 공지, 전체
#define CMD_MERCHANT_ITEM_BBS				10316		// 상인메뉴 아이템 공지, 현재 맵
#define CMD_MERCHANT_ITEM_BBS_MAP_TO_MAP	10317

#define	CMD_ALIVE_FAIL_RESULT				10318		// 주문이 틀려서 못 살아는 경우
#define CMD_UPDATE_LADDER_SCORE				10319		// 래더 점수를 서버에 저장한다.

#define CMD_REQ_RANK_LADDER					10320
#define CMD_REP_RANK_LADDER					10321

#define CMD_DIRECT_CLIENT					10322

#define CMD_REP_RANK_LADDER_FINISH			10323
#define CMD_DIRECT_CLIENT_TO_DB				10324		// 클라이언트에서 바로 디비로 보내는 패킷
#define CMD_REQ_SEARCH_RANK_LADDER			10325

#define CMD_REQ_SEARCH_RANK_LADDER_FAIL		10326
#define CMD_SEND_EFFECT						10327

#define CMD_REQ_VISA						10328
#define CMD_BUY_VISA						10329
#define CMD_BUY_VISA_OK						10330
#define CMD_BUY_VISA_FAIL					10331

#define CMD_NATION_INFO_MAP_TO_MAP			10332		// 이 타입을 쓰려면 첫 바이트는 타입을 넣어줘야 한다.
#define CMD_SAVE_NATION_INFO				10333		// 국가 관련, 저장해야할 내용들


#define CMD_EMOTION							10334		// 감정				// 010613 YGI
#define CMD_EXIT_GUILD_SELF					10335		// 스스로길드탈퇴

#define CMD_REGIST_GUILD_IMSI				10337
#define CMD_REGIST_GUILD_IMSI_OK			10338
#define CMD_REGIST_GUILD_IMSI_OK_ALL		10339		// 모든 맵서버에 임시 길드 등록된 사실을 알려준다.

#define CMD_GUILD_INFO1_REQ					10340
#define CMD_GUILD_INFO_NAME					10341
#define CMD_GUILD_INFO_DEGREE				10342
#define CMD_GUILD_INFO_IMAGE				10343
#define CMD_GUILD_INFO_DELETED				10344

#define CMD_REQ_GUILD_EXPLAIN				10345		// 길드의 설명을 알아본다.
#define CMD_REGIST_GUILD_MARK				10346
#define CMD_GUILD_NUMBER_INFO_DB_TO_MAP		10347

#define CMD_REGIST_GUILD					10349

#define CMD_GUILD_DEGREE_INFO				10350
#define CMD_REQ_GUILD_IMAGE					10351

#define CMD_REQ_GUILD_INTRODUCTION			10352
#define CMD_REQ_GUILD_INTRODUCTION_PLUS		10353
#define CMD_SAVE_GUILD_MEMBER_LIST			10354

#define CMD_PUT_MENU_STRING					10364


#define CMD_CHECK_MY_GUILD					10369
#define CMD_REFRESH_GUILD_CODE				10373
#define CMD_REGIST_GUILD_OK					10374

#define CMD_GUILD_CHAT						10375
#define CMD_GUILD_CHAT_MAP					10376


#define CMD_REQ_RESET_ABILITY				10377

#define CMD_DELETE_GUILD					10379
#define CMD_DELETE_GUILD_OK					10380
#define CMD_GUILD_DEGREE_INFO2			10381
// << 011019 ygi
#define CMD_GET_GHOST_QUEST_INFO		10382
#define CMD_SHOW_ME_DUAL_CHAR			10383
#define CMD_CHANGE_COLOR				10384
#define CMD_SALVATION_RESULT_ADD_FAME	10385
#define CMD_GET_NATION_MONEY			10386		// 국고 정보 가져오기
#define CMD_REP_NATION_MONEY			10387

#define CMD_REFRESH_SALVATION_NAME			10389

#define CMD_GAMBLE_ITEM					10390
#define CMD_GAMBLE_ITEM_RESULT			10391
#define CMD_GAMBLE_START				10392
#define CMD_SEND_LOGOUT_CHAR			10393
#define CMD_SEND_DIRECT_CLIENT_TO_MAP	10394		// 다른 맵의 유저에게 직접 메시지를 보낼때
#define CMD_REPORTER_MODE				10395

#define CMD_DELETE_TODAY_GAMBLE_ITEM	10404
#define CMD_SET_TODAY_GAMBLE_ITEM		10405
#define CMD_GET_TODAY_GAMBLE_ITEM		10406
#define CMD_TODAY_ITEM_GET_WHO			10408
#define CMD_CHANGE_PARTY_SELECT_EXP		10410

#define CMD_GOTO_POTAL					10411

#define	CMD_ADD_EVENT_ITEM				10412
#define CMD_SET_MAP_TYPE				10413
#define CMD_OX_QUESTION					10414

#define CMD_OX_SELECT					10415
#define CMD_OX_ANSER					10416

#define CMD_OX_SET_TIME					10417
#define CMD_CHECK_SKILL					10419

#define CMD_GET_GUILD_ITEM				10421		// 길드 아이템 전체 보여주기		// 040623 YGI
#define CMD_PUT_GUILD_ITEM				10422		// 길드 아이템 저장
#define CMD_GET_GUILD_ITEM_CLIENT		10423		// 실제 아이템 가져오기
#define CMD_GET_GUILD_ITEM_MAP			10424		// 실제 아이템 가져오기

#define CMD_ADD_GUILD_BOX_MONEY				10427

#define CMD_CHANGE_GUILD_MASTER				10429
#define CMD_CHANGE_GUILD_MASTER_ALL_MAP		10430

#define CMD_CHECK_CHANGE_GUILD_INFO			10433

#define CMD_GET_NATION_ITEM					10434
#define CMD_BUY_NATION_ITEM					10435
#define CMD_BUY_NATION_ITEM_OK				10436

#define CMD_CHECK_COLOSSUS_MAP				10437
#define CMD_CHECK_COLOSSUS_MAP_REPLY		10438

#define CMD_INVITE_COLOSSUS					10439
#define CMD_INVITE_COLOSSUS_REPLY			10440

#define CMD_JOIN_COLOSSUS_MAP				10441
#define CMD_ATTACK_COLOSSUS_STONE			10442
#define CMD_ATTACK_COLOSSUS_STONE_RESULT	10443

#define CMD_REVIVAL_COLOSSUS				10444
#define CMD_CALL_MAIN_MENU					10445

#define CMD_GUILD_HOUSE_INFO				10446
#define CMD_CHANGE_GUILDHOUSE_INFO			10447
#define CMD_BUY_GUILD_HOUSE					10448
#define CMD_CHECK_GUILD_HOUSE				10449

#define CMD_RELOAD_GUILD_HOUSE				10450
#define CMD_PK_ON_OFF						10451
#define CMD_GET_PK_ON_OFF					10452
#define CMD_FAIL_ATTACK_BY_PK_MODE			10453

#define CMD_SAVE_ITEM_LOG				10454
#define CMD_DIRECT_MAP_MOVE				10455		// gn_tool 기능

//acer4

#define CMD_SEARCH_OTHER_CHAR			10456
#define CMD_SEARCH_OTHER_CHAR_MAP		10457

//acer7
#define CMD_USE_GM_TOOL					10458
#define CMD_SEND_EVENT_NO				10459
#define CMD_SETTING_MAP_TILE_EVENT			10460
#define CMD_DELETE_MAP_TILE_EVENT			10461

#define CMD_GET_SCENARIO_INFO				10462
#define CMD_SEND_DUNGEON_HISTORY			10463
#define CMD_GET_SCENARIO_INFO_MAP			10464
#define CMD_SCENARIO_COMMAND				10465		// 시나리오 관리 클래스가 여러 맵에게 명령을 내린다.
#define CMD_SCENARIO_MESSAGE				10466		// 클라이언트로 메시지를 보내준다.

#define CMD_SAVE_AMOUNT_EVENT_NPC_ITEM		10467

#define CMD_SAVE_EVENT_MAPMOVE_NOW			10469




#define KEIN_FUNCTIONS_START				10600


#define CMD_CHECK_GOTO_SCENARIO_DUNGEON		10600		// 시나리오 던전맵으로 갈 수 있는가?
#define CMD_CHECK_GOTO_SCENARIO_DUNGEON_RESULT	10601	// 시나리오 던전맵으로 갈 수 있는지 결과
#define CMD_SCENARIO_MESSAGE_MAP				10602		// 모든 서버로 보내서 모든 서버에서 모든 클라이언트로 보내주게 한다.
#define CMD_TRAP_INFO							10603
#define CMD_TRAP_INFO_MAP						10604
#define CMD_SCENARIO_TRAP_KEY					10605
#define CMD_DRAGONROAD_VOTE						10606
#define CMD_SCENARIO_TIME_INFO	  			    10607 // CSD-030306
#define CMD_CHANGE_SCENARIO_TIME				10608
#define CMD_MOVEMAP_FOR_LIGHT_VERSION			10609		// 021008 YGI
#define CMD_CHECK_EVENT_SCRIPT_ITEM				10610		// 한 계정당 하나의 아이템의 경우 처리
#define CMD_EVENT_MOVE_MAP						10611
#define CMD_EVENT_MOVE_MAP_CHECK_MAP			10612		// 해당 맵으로 확인들어간다.
#define CMD_EVENT_MOVE_MAP_OK					10613		// 좋다 이동해 와라

#define CMD_CHECK_DOUBLE_NAME					10614		// 다른 맵에 같은 이른을 쓰는 놈이 있는가?
#define CMD_CHECK_DOUBLE_NAME_OK				10615		// 우린 맵에 같은 놈이 있으니깐 그쪽 맵에서 끊어라..
#define CMD_CHECK_DOUBLE_NAME_DB_TO_MAP			10616
#define CMD_CHECK_DOUBLE_NAME_WHERE_FIRST		10617		// 디비에서 데이타 받기 시작할 때
#define CMD_CHECK_DOUBLE_NAME_WHERE_OK			10618		// 디비에서 데이타 받기 시작할 때
#define CMD_CHECK_RECV_MAIL						10619 // CSD-030227

// 031009 CI YGI
#define CMD_WRITE_LIMIT_NUMBER					10620		// 디비에 리미트 넘버 생성에 사용하는 고유 번호를 저장한다.
// 040105 YGI 보물
#define CMD_CHECK_TREASURE_XY					10621
#define CMD_GET_TREASURE_MAP_DETAIL				10622
#define CMD_GET_TREASURE_MAP_DETAIL_DB			10623

// 040601 YGI
#define CMD_GET_ITEM_MALL						10624		// 내가 산 아이템 리스트 가져오기 ( 맵에서 디비로 요청 )
#define CMD_SEND_ITEM_MALL_ITEM_LIST			10625		// 산 아이템 리스트 보내주기	( 디비에서 맵으로 답변 )
#define CMD_ITEM_MALL_RESULT					10626		// 맵에서 디비로 아이템 몰 아이템 지급에 대한 결과를 보내준다.

// 040602-3 YGI
#define CMD_ITEM_DROP_MESSAGE					10627
#define CMD_ITEM_REPAIR_BY_ITEM					10628		// 040621 YGI

//<050224_KCH 마일리지몰 작업
#define CMD_DELETE_ITEM_MALL_ITEM_PER_1MIN	10630	//마일리지에서 아이템 삭제시 따로 메세지를 보여주어야 한다.

#define CMD_GET_ITEM_MALL_AB_FIRST			10631	//Account Box에 처음으로 요청하는 메세지(총아이템갯수,인덱스를 리턴,CMD_GET_GUILD_ITEM_FIRST참고)
#define CMD_GET_ITEM_MALL_AB				10632	//Acoount Box에 담을 아이템을 요청(한페이지화면에 15개의 아이템)
#define CMD_GET_ITEM_MALL_CLIENT			10633	//Account Box에서 보여진 아이템 클릭시 실제 아이템 가져오기
#define CMD_DELETE_ITEM_MALL_ITEM_CLIENT	10634	//마일리지 Client에서 아이템 사용시 시간지난아이템이면, 삭제요청
#define CMD_DELETE_ITEM_MALL_ITEM_PER_1MIN_CLIENT	10635	//Client 아템 삭제명령
//>050224_KCH 마일리지몰 작업
#define CMD_TEST							14999	// 테스트에 사용

//----------------------------- 1.04p Patch -------------------------------

#define CMD_BAD_USER_IMPRISON				10469		// LTH-040719-KO 불량 유저를 감옥에 투옥한다.
#define CMD_PRISON_SYS_PROC_RESULT			10470		// LTH-040727-KO 감옥 시스템 처리에 따른 결과.
#define CMD_BAD_USER_RELEASE				10471		// LTH-040805-KO 불량 유저를 감옥에서 빼낸다.
#define CMD_UPDATE_IMPRISON_TERM			10472		// LTH-040810-KO 감옥에 수감되어 있는 시간 갱신.
#define CMD_REQUEST_MOVE_TO_PRISON			10473		// LTH-040813-KO 썅!! 같은 맵에 불량 유저가 없으면 다른 맵으로 전달해서 감옥으로 옮긴다.
#define CMD_REQUEST_MOVE_TO_VILLAGE			10474		// LTH-040813-KO 우띠!! 다른맵의 불량유저가 마을로 돌아가도록...

/**
 * @struct	t_BadUserInfo.
 * @brief	불량 유저 정보.
 */
//< LTH-040719-KO.
struct t_BadUserInfo
{
	char	szName[20];				// 불량 유저 이름.
};
//> LTH-040719-KO.

/**
 * @struct	t_PrisonSysProcResult.
 * @brief	감옥 시스템 처리에 따른 결과 정보.
 */
//< LTH-040727-KO.
struct t_PrisonSysProcResult
{
	int		nReason;				// 결과 상수값.
};
//> LTH-040727-KO.

/**
 * @struct	t_RequestMoveToPrison.
 * @brief	다른 맵에 있는 불량 유저를 감옥으로 이동 시키기 위한 패킷.
 */
//< LTH-040813-KO.
struct t_RequestMoveToPrison
{
	char	szUserId[20];			// 불량 유저 ID.
	int		nNation;				// 불량 유저 국가.
};
//> LTH-040813-KO.
//----------------------------- 1.04p Patch -------------------------------

//////////////////////////////////////////////
// 아이템 몰 // 040601 YGI
struct k_item_mall_result_to_db
{
	int itemmall_index;
	int itemMallItemKind_index;		// itemMallItemKind	테이블에서의 인덱스 값
	int itemmall_limit;			// 040602 YGI
	char result_type;
};

struct k_send_item_mall_item_kind
{
//	short int server_id;		//050408_KCH LocalMap외부로 나가는 cn(server_id)는 신뢰불가.
	int itemmall_index; 
	int itemMallItemKind_index;	//050224_KCH 마일리지몰 작업	itemMallItemKind 테이블에서의 인덱스 값
	char name[20];
	ItemAttr item;				//t_EventItem item;
	char result_type;
};

//050408_KCH
struct k_ItemMall
{
	int itemmall_index;				// 아이템몰에서의 인덱스 값
	int itemMallItemKind_index;		// itemMallItemKind	테이블에서의 인덱스 값
};

struct k_get_item_mall
{
//	short int	server_id;			//050408_KCH LocalMap외부로 나가는 cn(server_id)는 신뢰불가.
	DWORD		dwItemmall_Index;	//050224_KCH 마일리지몰 작업
	char		login_id[20];
	char		name[20];
};

//050224_KCH
struct k_get_item_mall_first
{
	int max;
	DWORD item_index[200];
};

struct k_get_item_mall_item
{
	char count;
	DWORD	index[15];
	ItemAttr item[15];
};

struct k_Delete_Item_Mall			//유저최초 로그인시에 기간제한 아이템 삭제요청하기위한 패킷
{
	int		itemmall_index;			// 아이템몰에서의 인덱스 값
	int		itemMallItemKind_index;				// itemMallItemKind	테이블에서의 인덱스 값
	int		item_limit;				// 아이템의 Unique Limit값
	char	recv_name[20];
	int		ItemNo;					//유저가 소유한 Inv,Bank,equip등에서 비교할 아이템 번호.
	time_t	EndUseTime;				//아이템 사용기간으로 이시간이 지나면 아이템을 사용할수 없다. 백업 테이블로 이동시켜버리자.
};	//320bit = 40Byte

#define Size_Delete_Item_Mall_ARRAY  30
struct k_Delete_Item_Mall_ARRAY			//유저최초 로그인시에 기간제한 아이템 삭제요청하기위한 패킷
{
	k_Delete_Item_Mall	_DelItemMall_ARRAY[Size_Delete_Item_Mall_ARRAY];		//9600bit (320bit * 30)
	int					nCnt_DelItemMall;										//  32bit
};																				//9632bit 이므로 MAX_PACKET_SIZE 10240을 넘지 않는다.

struct k_Info_Item_Mall		//구입한 아이템몰아이템(Item_to_Game)에서 유저에게 보여줄 정보
{
	int					nIdx_ItemMall;
	char				szRecv_LoginId[20];
	TIMESTAMP_STRUCT	RecvTime;
	char				szSend_Name[20];
	char				szMessage[200];
};


struct k_get_item_to_game_TBL
{
	int itemmall_index;	// 아이템몰에서의 인덱스 값
	int itemMallItemKind_index;		// itemMallItemKind	테이블에서의 인덱스 값
	int item_limit;
//	short int server_id;
	char recv_loginid[20];
	char recv_name[20];
	CTime EndUseTime;	//아이템 사용기간으로 이시간이 지나면 아이템을 사용할수 없다. 백업 테이블로 이동시켜버리자.
};

//<050224_KCH 마일리지몰 작업	(기간제 아이템처리를 위해서 수정)
enum _eRecvType_ItemMallItem
{
/*Status*/
/*지급대기*/		Wait_ItemMallItem						=0,	//홈페이지에서 구입하고 아템 받을수 있는상태.
/*지급중*/			HOLD_ON_SUPPLY_INV_ItemMallItem			=1,	//캐릭에게 지급하는 중간단계
/*지급완료*/		SUPPLY_INV_ItemMallItem					=2,	//캐릭에게 지급완료.

/*기간아템지급중*/	HOLD_ON_Progressive_ItemMallItem		=3,	//지급완료된 아이템중 기간제아템 시간을 세팅하는 중간 단계.
/*기간아템사용시작*/Progressive_TIMESTAMP_ItemMallItem		=4,	//기간제 아이템일 경우 지급받은후, 사용시작되고 있는 중.
				//	HOLD_ON_Expire_TIMESTAME_ItemMallItem	=5,	//기간제 아이템을 처리하는 상태
/*기간아템사용끝*/	Expire_TIMESTAMP_ItemMallItem			=6	//기간제 아이템일 사용끝난 아이템, 이녀석들을 일정시기마다, 백업테이블로 옮겨버리자.
};

enum _eRetType
{
	FailedCreateTBL = -1,
	FailedEditTBL	= -2,
	FailedMoveTBL	= -3,
	FailedDeleteTBL	= -4,
	SucessedTBL		= 1
};
//>050224_KCH

///////////////////////////////////////

// 040105 YGI 보물
struct k_get_treasure_map_detail_to_map
{
	POS pos;
	short int cn;
	short int map_number;
	short int x;
	short int y;
};

struct k_get_treasure_map_detail_to_db
{
	POS pos;			// 아이템 위치
	short int cn;		// 아이템을 가지고 있는 놈
};

struct k_check_treasure_xy
{
	short x;
	short y;
};
//////////////
struct k_save_limit_number	// 031009 CI YGI
{
	WORD port;
	DWORD limit_number;
};


struct k_check_recv_mail
{	//< CSD-030227
	char count;
	char name[40][20];
};	//> CSD-030227

struct k_save_event_move_map
{
	int event_no;
	short int max;
	short int now;
};

struct k_event_move_map_ok
{
	short int server_id;
	char map_name[20];
	int map_x;
	int map_y;
};

struct k_event_move_map_check_map		// 해당 맵으로 보내서 체크한다.
{
	short int server_id;		// 대상자
	short int server_port;		// 대상자가 있는 맵의 포트번호
	short int event_no;
};



struct k_event_script_item
{
	short int event_no;
	short int server_id;
	short int npc_index;
	short int bag_num;
	char id[20];
	char name[20];			// 해당 캐릭터 이름 --> 혹시 그 사이에 로그 아웃 했을 경우 대비
	char	script_no;		// 해당 이벤트 스크립트 이벤트 클래스 포인터를 얻어오기 위해
	char	result;			// 성공 실패에 관한
};

struct k_event_object_sound
{
	char active;
	char object_toi[30];
	char object_b[30];
	int sound_no;
};

struct k_event_object
{
	char active;
	char object_toi[30];
	char object_b[30];
};

struct k_event_sound
{
	char active;
	int  sound_no;
};

struct k_new_server_change_map
{
	t_server_change_map data1;
	union
	{
		k_event_object_sound	data2;
		k_event_sound			data3;
	}u;
};

struct k_check_double_when_first
{
	short int server_id;
	WORD	port;
	char name[20];
};
struct k_login_check_double_name
{
	char name[4][20];
};

// 021008 YGI
struct k_map_move
{
	short int x;
	short int y;
	char	mapname[20];
};

struct k_client_scenario_time_info
{	//< CSD-030306
	int		nPort;		// 리퀘스트한 포트번호가 들어 간다// 유저가 그맵에 있을때는. 그맵 포트 겠지.ㅡㅡ;
	int		nServer_Id; // 클라이이언트의 서버 아이디가 들어간다. 클라이언트에서는 당연히 않쓰이지.ㅡㅡ;
	DWORD idDungeon;
};	//> CSD-030306

struct k_server_scenario_time_info
{	//< CSD-030306
	DWORD idStartUp;
	DWORD idDungeon;
	int	  nApplyDay;
	int   nStartHour;
	int   nStartMinute;
	int   nEndHour;
	int   nEndMinute;
};	//> CSD-030306

struct k_scenario_time_change
{	//< CSD-030306
	DWORD idStartUp;
	DWORD idDungeon;
	int	  nApplyDay;  
	int   nStartHour;
	int   nStartMinute;
	int   nEndHour;
	int   nEndMinute;
};	//> CSD-030306

struct k_scenario_trap_key
{
	char sum;
	char qube[7];
};

struct k_scenario_detail_info
{
	char type;
	int time;			
	char total_trap;
	char active_trap;
};

struct k_scenario_message
{
	char	type;
	WORD	port;
};

struct k_check_goto_scenario_dungeon_map
{
	short int cn;
	short int map_port;
	char name[20];
	int x;
	int y;
	char dungeon[20];
};

const int MAX_START_UP_INFO = 30;//030321 lsw

struct k_get_scenario_info_basic
{
	char day;
	char SHour;
	char SMin;
	char type;
	char layer;
	
	char EHour;
	char EMin;
	char dungeon_map[20];
};

struct k_get_scenario_info
{
	char count;
	k_get_scenario_info_basic data[MAX_START_UP_INFO];
};

struct k_send_dungeon_history
{
	char type;
	char layer;
	char name[20];
	char map[20];
};

struct k_setting_map_tile
{
	char index;			// 추가 변경 속성의 인덱스
	char type;			// 타일 속성
	char number;		// 0은 속성을 없앨때 사용, 그외는 event 타일의 번호를 지정
	short int x;
	short int y;		// 지정할 중심 타일 번호
	char range;			// 지정할 범위	// x+y 의 타일 값을 지정 
};

struct k_char_info
{
	DWORD name_status;
	BYTE btLevel; // CSD-030806
	short int x;
	short int map_number;
	short int nk;
	char curr_tactic_kind;
	char curr_tactic_lv;
	short int y;
	short int guild_code;	// thai2 YGI
};

struct k_char_info_map
{
	short int port;
	short int cn;
	char name[20];
};


struct k_direct_map_move
{
	char map_number;
	int x, y;
	char name[20];
};

struct k_item_log
{
	ItemAttr item;
	char lv;
	char type;
	WORD port;
	char name1[20];
	char name2[20];
	
};



struct k_check_guild_house
{
	char count;
	char pHouseId[MAX_GUILD_HOUSE];
};

struct k_change_guild_house_info_db2map
{
	char house_id;
	short int guild_code;
	short int year;
	char month;
	char day;
};

struct k_change_guild_house_info
{
	char id;
	char day_type;		// 1: 신규 신청, 2 : 연장
	short int guild_code;
};

struct k_guild_house_info_unit
{
	char id;				// 번호
	char lv;				// 등급
	short int guild_code;			// 소유길드
	char time;				// 남은 기간 // 날자
	short int red;
	short int blue;
	char map_name[20];		// 위치
};

struct k_guild_house_info
{
	char count;
	k_guild_house_info_unit	data[30];
};

struct k_attack_colossus_stone_result
{
	short int server_id;
	short int item_id;
	DWORD attr;
};

struct k_invite_colossus_map
{
	WORD port;		// 초대를 원하는 맵
	char name[20];		// 방장
};

struct k_direct_map_req		// 다른 맵으로 정보 요구
{
	WORD port;
	short int server_id;
};

struct k_colossus_info_map_reply // 클라이언트로 보내줌
{
	WORD	port;			// 그 맵의 포트 번호
	short int ch_count;		// 현재 인원
	char name[20];			// 방장
};

struct k_colossus_info_map		// 요청한 맵으로 대답
{
	WORD	port;			// 그 맵의 포트 번호
	short int ch_count;		// 현재 인원
	short int server_id;	// 요청한 사람
	char name[20];			// 방장
};

struct k_nation_item
{
	short int item_no;
	char give_type;			// 기부 타입 // 돈인지, fame 인지
	int give_much;			// 그 양
};

struct k_check_guild_change
{
	char type;
	char ret;
};

struct k_change_guild_degree
{
	short int server_id;
	short int guild_code ;
	char degree;
};

struct k_req_guild_item
{
	short int guild_code;
	DWORD index;
};

struct k_get_guild_item_db
{
	short int server_id;
	ItemAttr item;
};

struct k_get_guild_item_map		// 실제 아이템 가져오기
{
	short int server_id;
	short int guild_code;
	DWORD index;	
};

struct k_put_guild_item
{
	ItemAttr item;
	short int guild_code;
	char	name[20];
};

struct k_save_event_item
{
	short int item_no;
	char count;
	char name[20];
};

struct k_set_map_type
{
	short int map_num;
	char type;
};
struct k_potal_map_unit
{
	char map_number;
	int map_money;
};
#define MAX_POTAL_MAP	40
struct k_potal_map
{
	char count;
	k_potal_map_unit map[MAX_POTAL_MAP];
};

struct k_today_gamble_item
{
	ItemAttr item;
	char nation;
	char name[20];
};

struct k_direct_map2user		// 다른 맵의 유저에게 직접 메시지를 보낼때
{
	short int type;
	short int server_id;
	char data[MAX_PACKET_SIZE-4];
};

struct k_focuse_logout_map
{
	char name[20];
	short int map_port;
	short int cn;
};

struct k_put_menu_string
{
	char type;
	short int str_num;
	char szName[256];	//050224_KCH 마일리지몰 작업 ItemName나 캐릭터 명을 같이 보내주자.
};

struct k_gamble_item_result
{
	char result_type;
	short int item_no;
	char name[20];
};

struct k_gamble_item
{
	char count;
	POS	pos[20];
};

struct k_nation_money
{
	char nation_type;
	DWORD nation_money[3];
};

struct k_change_color_map2client
{
	short int server_id;
	char type;
	BYTE r, g, b;
};
struct k_change_color_db
{
	char type;
	BYTE r, g, b;
	char name[20];
};
struct k_change_color
{
	DWORD type1 : 4;
	DWORD type2	: 4;
	DWORD r		: 8;
	DWORD g		: 8;
	DWORD b		: 8;
};
// >> 011019 ygi

struct k_gamble_data
{
	char house_type;
	char lv;
	short int user_count;
};

struct k_reset_ability
{ 
	short int		Str  ;	
	short int		Con  ;	
	short int		Dex  ;	
	short int		Wis  ;	
	short int		Int  ;	
	short int		MoveP;	  
	short int		Char ;	 
	short int		Endu ;	 
	short int		Moral;	  
	short int		Luck ;	 
	short int		wsps ;	 
	
	short int		reserved_point;
	
};

struct k_guild_chat_map
{
	short int guild_code;
	char name[20];
	char msg[1024];
};

struct k_guild_chat
{
	char name[20];
	char msg[1024];
};

struct k_delete_guild_mail
{
	short int server_id;
	DWORD	mail_id;
	short int guild_code;	
	char	degree;
};

struct k_guild_mail_id
{
	DWORD guild_mail_id;
	char name[20];
};


struct k_save_guild_member_list
{
	short int guild_code;
	short int old_guild_code;
	char	degree;
	char name[20];
};

struct k_guild_instroduction_basic
{
	short int guild_code;
	char guild_name[20];
};

struct k_guild_instroduction
{
	short int count;
	k_guild_instroduction_basic guild[101];
};

struct k_guild_degree_info
{
	short int guild_code;
	char info_num;
	char guild_degree[5][20];
};

#define IMAGE_UNIT_SIZE			1000
struct k_regist_guild_mark
{
	short int guild_code;
	char	total;
	DWORD	size;
	char	xl;
	char	yl;
	char	sub_count;
	char	img[IMAGE_UNIT_SIZE];
};

struct k_guild_explain
{
	short int guild_code;
	char explain[1024];
};

struct k_guild_info_name
{
	short int guild_code;
	char make;
	char first;
	char guild_name[20];
};

struct k_guild_info_degree
{
	short int guild_code;
	char info_num;
	char guild_degree[5][20];
};

struct k_req_guild_info1
{
	short int guild_code;
	char make;
	char mark_num;
	char info_num;
	char first;
};


struct k_send_regist_imsi_guild_info
{
	short int server_id;		// 임시 길드를 등록한 사람
	short int guild_code;
	char make;
	char guild_name[20];
};

struct k_set_guild_info
{
	short int guild_code;
	char  active;
	char make;
	char first;
	char info_num;
	char mark_num;
	char nation;
	
	char guild_name[20];
	char guild_degree[5][20];
};

struct k_set_guild_number_info
{
	short int guild_code;
	char type;
	char number;
};

struct k_regist_guild_imsi_ok
{
	short int server_id;
	short int guild_code;
};

struct k_regist_guild_imsi_db
{
	short int server_id;
	char	nation;
	char guild_name[20];
	char guild_ma[20];
	char guild_explain[1024];
};

struct k_regist_guild_imsi
{
	char guild_name[20];
	char guild_explain[1024];
};

struct k_save_nation_info
{
	char	nation;
	double	nation_money;
	int		king_support;
	
	int		friend_va;
	int		friend_zy;
	int		friend_ill;
	
	int		nation_item[20];
};

struct k_nation_info_money		// 국고 변화량 
{
	char	type;
	char	nation;
	double	money;
};

struct k_visa_buy
{
	short int	item_no;
	char		nation;
};

struct k_visa_item
{
	short int item_no[4];
	DWORD	item_price[4];
};
// 040105 YGI 보물
struct k_send_effect
{
	short int effect_num;
	short int target_id;
	short int x;
	short int y;
};

struct k_send_db_direct_map
{
	short int	server_id;
	char		data[MAX_PACKET_SIZE-2];
};

struct k_send_db_direct_client
{
	short int	type;
	char		data[MAX_PACKET_SIZE-2];
};

struct k_direct_client
{
	short int type;
	short int server_id;
	char data[MAX_PACKET_SIZE-4];
};

struct k_req_rank_ladder_to_db
{
	int page;
	short int server_id;
};

struct k_get_ladder_score_rank_unit
{
	int		rank;		// 순위
	DWORD	score;
	char	nation;
	char	cls;
	BYTE	btLevel; // CSD-030801
	char	name[20];
};

struct k_db_get_ladder_score_rank
{
	//short int server_id;
	char	is_end;		// 여기가 끝인지 아닌지...
	k_get_ladder_score_rank_unit	unit[10];
};

struct k_update_ladder_score
{
	DWORD	score;
	
	char	naiton;
	char	cls;
	BYTE	btLevel; // CSD-030806
	
	char	name[20];
};

struct k_recv_merchant_item_bbs		// 서버에서 클라이언트로 보내는 정보
{
	DWORD		dwMoney;			// 아이템 가격
	char		map_number;			// 맵 번호
	short int	pos_x;				// 위치
	short int	pos_y;				// 위치
	char		szItemName[30];		// 아이템 이름
	char		szCharName[20];		// 캐릭터 이름
};

struct k_merchant_item_bbs			// 서버로 보내는 정보
{
	DWORD		dwMoney;
	char		szItemName[30];
};

struct k_name_message
{
	char name[20];
	char message[200];
};

struct k_character
{	//< CSD-030506	
	int     nArenaGame;
	char	name[20];
};	//> CSD-030506

struct k_fight_map_char_info
{
	DWORD	win_defeat;
	DWORD	ladderscore;
	DWORD	total_id;
	char	level;
	char	Class;
	char	dual_cls;
	char	cls_step;
};

struct k_loan_info
{
	DWORD last_loan;
	DWORD max_loan;
};

struct k_name2
{
	char name1[20];
	char name2[20];
};

struct k_eat_candy
{
	DWORD total_id;			// 자기 자신의 total_id
	DWORD candy_limit;		// 캔디의 리미트 번호
	char  name[20];			// 자기 이름
};

struct k_guild_exit_global
{
	short int server_id;
	short int guild_code;
	char name[20];	
};

struct k_openschool_data_result
{
	char number;
	short int npc_id;
};

struct k_openschool_data_ok
{
	short int npc_id;
	short int server_id;
	char number;
	short int item_no;	
};

struct k_openschool_data
{
	short int npc_id;
	short int item_no;
	short int server_id;
	char nation;
	char name[20];
};

struct k_check_chocolate
{
	int count;
	char name[20];
};

struct k_login_check_chocolate
{
	DWORD server_id;
	int count;
	char name[20];
};

struct k_eat_chocolate_ok
{
	DWORD	server_id;
	int		chocolate_count;
};
struct k_eat_chocolate
{
	DWORD server_id;
	DWORD limit;
	char name[20];
};


struct k_change_money
{
	DWORD money;
};

struct k_send_db2map_each_item
{
	short int server_id;
	short int item_no;
};

struct k_total_salvation_all_msg
{
	char nation;
	DWORD	money;
	double total_money;
	char name[20];
};
typedef struct tag_db_salvation_money		// 기부금을 저장하라..
{
	short int server_id;
	char	nation;
	DWORD	money;
	char	name[20];
}k_db_salvation_money;

typedef struct tag_db2game_salvation_money		// 기부금값을 보내준다.
{
	short int server_id;
	DWORD	money;
}k_db2game_salvation_money;

typedef struct tag_req_salvation_money		// 기부한금액을 가져온다.
{
	short int server_id;
	char	name[20];
}k_req_salvation_money;


typedef struct tag_stop_war_all_msg		// 두 나라가 휴전이라는 사실을 모두에게 알린다.
{
	char	send_nation;
	char	target_nation;
}k_stop_war_all_msg;

typedef struct tag_rep_stop_war_login
{
	short int server_id;
	short int game_id;
}k_rep_stop_war_login;

typedef struct tag_send_client_req_stop_war
{
	char send_nation;		// 휴전을 신청한 나라
	short int server_id;	// 휴전을 요청한 놈 아이디
	short int game_id;		// 휴전을 요청한 놈이 있는 포트번호
}k_send_client_req_stop_war;

typedef struct tag_send_game_server_req_stop_war
{
	char target_nation;		// 휴전을 결정할 나라
	char send_nation;		// 휴전을 신청한 나라
	short int server_id;	// 휴전을 요청한 놈 아이디
	short int game_id;		// 휴전을 요청한 놈이 있는 포트번호
}k_send_game_server_req_stop_war;

typedef struct tag_send_login_req_stop_war
{
	char target_nation;			// 어느나라로 보낼요청 할꺼냐?
	char send_nation;			// 어느 나라가 보내냐?
	short int server_id;		// 어떤 넘이 보내냐?
}k_send_login_req_stop_war;

typedef struct tag_send_gm_regist_ok
{
	char gm_index;				// 어떤 직업의 gm인지
	short int gm_rank;			// 그직업의 몇번째 gm인지
	short int total_gm_rank;	// 토탈 gm의 몇번째 인지
	char	name[20];				
}k_send_gm_regist_ok;

typedef struct tag_bbs_ALL
{
	char type;		// 보낼 공지의 종류 파악
	char name[20];
}k_bbs_all;

typedef struct tag_gm_regist_db
{
	char gm_list;
	char name[20];
}k_gm_regist_db;

typedef struct tag_gm_regist
{
	char gm_list_index;
}k_gm_regist;

typedef struct tag_guild_each_level
{
	short int	target_id;
	char		each_level;
}k_guild_each_level;

typedef struct tag_guild_msg
{
	short int guild_code;
	char	  msg[MAX_MSG];
}k_guild_msg;

typedef struct tag_x_y
{
	char x;
	char y;
}k_x_y;


typedef struct tag_scan_target
{
	char	count;
	k_x_y	xy[250];
}k_scan_target;

typedef struct tag_guild_join_ok
{
	short int guild_code;			// 어느 길드인지
	short int target_id;			// 길드 가입이 된 놈
}k_guild_join_ok;

typedef struct tag_guild_join_result
{
	short int target_id;
	short int server_id;
}k_guild_join_result;

typedef struct tag_guild_join_server
{
	short int target_id;		// 길드를 바꿀놈
	short int server_id;		// 명령을 내린놈 
	short int guild_code;
	char id[20];
}k_guild_join_server;

typedef struct tag_send_login_nation_msg
{
	char	nation;
	char	msg[200];
}k_send_login_nation_msg;

typedef struct tag_get_skill_info
{
	short int target_id;
	char	job;
	char	call_lv;
}k_get_skill_info;

typedef struct tag_req_vote_score
{
	short int	server_id;
	short int	vote_num;
	char		nation;
}k_req_vote_score;

typedef struct tag_score_and_name
{
	int score;
	char name[20];
}k_score_and_name;

typedef struct tag_rep_vote_score
{
	short int server_id;
	char	max;
	k_score_and_name	name_score[3];
}k_rep_vote_score;

typedef struct tag_client_rep_vote_score
{
	int score[3];
	char	max;
	k_score_and_name	name_score[3];
}k_client_rep_vote_score;

typedef struct tag_set_value_of_vote
{
	char	vote_num[6];
	char	is_voting[6];	
}k_set_value_of_vote;

typedef struct tag_check_vote
{
	short int	server_id;
	short int	vote_num;
	char		nation;
	char		login_id[20]; 
}k_check_vote;

typedef struct tag_is_this_man_candidate
{
	short int server_id;
	short int vote_num;
	char name[20];
}k_is_this_man_candidate;

typedef struct tag_candidate
{
	short int	number;		// 선거 번호
	
	DWORD		lv : 7;
	DWORD		class_type : 3;
	DWORD		gender : 1;
	DWORD		nation : 4;
	
	DWORD		guild_code : 9;
	DWORD		face : 7;
	
	char		name[20];
}k_candidate;

typedef struct tag_regist_candidate
{
	short int	server_id;
	k_candidate	candidate;
}k_regist_candidate;

typedef struct tag_check_vote_ok
{
	short int server_id;
	char candidate_count;
	k_candidate candidate[3];
}k_check_vote_ok;		// 후보 정보

typedef struct tag_check_vote_client_ok
{
	char candidate_count;
	k_candidate candidate[3];
}k_check_vote_client_ok;

typedef struct tag_vote_game2login
{
	short int	server_id;
	short int	vote;
	char		support;
	char		nation;
	char	login_id[20];
}k_vote_game2login;

typedef struct tag_exist_name_nation	// 메일 보내기에 가능한 이름인지 확인
{
	char nation;
	short int server_id;
	char name[20];
}k_exist_name_and_nation;

typedef struct tag_exist_name		// 메일 보내기에 가능한 이름인지 확인
{
	short int server_id;
	char name[20];
}k_exist_name;

typedef struct tag_change_dur
{
	POS			pos;
	DWORD		dur;
}k_change_dur;

typedef struct tag_script_change
{
	short int	script_num;
	int			value;
}k_script_change;

typedef struct tag_reset_job
{
	char	job;
	char	cls;
	char	name[20];
}k_reset_job;

typedef struct tag_team_add_message
{
	DWORD		total_id;
	char		team_num;
	short int	id;
	char		name[20];
}k_team_add_message;

typedef struct tag_team_battle_msg2
{
	char ret;
	char attack_ch[20];	
	char team1;
	char death_ch[20];
	char team2;
}k_team_battle_msg2;

typedef struct tag_team_battle_msg3
{
	char ret;
	char attack_ch[20];	
	char team1;
	char stone_number;
}k_team_battle_msg3;


typedef struct tag_quest_unit
{
	short int index;
	short int ret;
}k_quest_unit;

typedef struct tag_rep_quest_index
{
	char			max;
	k_quest_unit	quest[14];
}k_rep_quest_index;

typedef struct tag_req_man_to_man_result
{
	short int	id;
	char		ret;
}k_req_man_to_man_result;

typedef struct tag_req_man_to_man
{
	//	char		name[20];		// 신청한 캐릭터 이름
	short int	id;				// 신청한 캐릭터의 아이디
}k_req_man_to_man;

typedef struct tag_do_party_ok
{
	char name[20];
	short int type;
}k_do_party_ok;

typedef struct t_danger_of_hunter
{
	short int	target_id;
	char		type;		// 1: 시야안으로 들어왔다.  0: 사라졌다.
}k_danger_of_hunter;

typedef struct t_kill_who
{
	char name[20];
}k_kill_who;

//////////////////////////////////////////////////////////////
// 현상범을 잡고 나서...
typedef struct t_kill_hunter_ok		// CMD_KILL_HUNTER_LIST_OK
{
	DWORD	money;
}k_kill_hunter_ok;

typedef struct t_kill_who_whom
{
	char hunter_name[20];		// 죽인사람
	char target_name[20];		// 죽은 사람
}k_kill_who_whom;

typedef struct t_kill_who_whom_login
{
	char hunter_name[20];		// 죽인사람
	char target_name[20];		// 죽은 사람
	char regist_name[20];		// 의뢰한 사람
}k_kill_who_whom_login;

typedef struct t_send_kill_hunter_list
{
	//	char target_name[20];
	//	char regist_name[20];
	DWORD	money;
	int server_id;
}k_send_kill_hunter_list;

typedef struct t_kill_hunter_list_login
{
	short int server_id;
	int	list_id;
	char hunter_name[20];		// 사냥꾼의 이름	
}k_kill_hunter_list_login;

///////////////////////////////////////////////////////////////////
// 현상범 리스트 보내주기 위한 구조체
typedef struct t_get_hunter_list
{
	int list_id;
}k_get_hunter_list;


typedef struct t_hunter_list		// 게임에서 클라이언트로
{
	int	 index;
	char target_name[20];
	char regist_name[20];
	DWORD	money;
	char gender;
	short int face;
	char text[1000];
}k_hunter_list;

typedef struct t_login_game_hunter_list		// 디비데몬에서 게임으로 
{
	short int		server_id;
	k_hunter_list	list;
}k_login_game_hunter_list;

typedef struct t_game_login_hunter_list		// 게임에서 로그인으로 요청
{
	short int	server_id;
	int			list_id;
}k_game_login_hunter_list;
////////////////////////////////////////////////////////////////////

typedef struct t_hunter_regist
{
	char name[20];
	DWORD money;
	char text[1000];
}k_hunter_regist;

typedef struct t_hunter_regist2
{
	char regist_name[20];
	char target_name[20];
	DWORD money;
	char text[1000];
}k_hunter_regist2;

typedef struct t_server_learn_item
{
	short int item_count;
	short int item[MAX_LEARN_ITEM];
}k_server_learn_item;

typedef struct t_server_id
{
	short int server_id;
}k_server_id;

typedef struct t_start_game_mail
{
	short int server_id;
	short int guild_code;
	char name[20];
}k_start_game_mail;

typedef struct t_req_party_info
{
	char party_name[20];
}k_req_party_info;

typedef struct t_rep_party_info
{
	char party_name[20];
	int			x;
	int			y;
	short int	hp;
}k_rep_party_info;

typedef struct t_oh_my_party
{
	char	my_name[20];		// 새로 들어온놈
	char	party_name[20];		// 같이 파티 맺은 사람
}k_oh_my_party;

typedef struct t_server_req_char_info
{
	short int	id;
	//	char		lv;
	int			Hp;
	int			HpMax;
}k_server_req_char_info;

typedef struct tag_note_meeting
{
	char believe_god;
	char meeting_type;
}k_note_meeting;

typedef struct tag_i_get_item_of_supper
{
	short int item_no;
}k_i_get_item_of_supper;

typedef struct tag_server_to_client_open_supper
{
	char much;		// 얻을 수 있는 빵의 갯수
}k_server_to_client_open_supper;

typedef struct tag_req_faith_up
{
	char name[20];				// 신앙심 올려줄 사람
	short int up_point;			// 신앙심 올려줄 양
}k_req_faith_up;

typedef struct tag_game2login_recall_box_item
{
	DWORD box_id;
	short int server_id;		// 자기 자신건 안지우기 위해
}k_game2login_recall_box_item;

typedef struct tag_char_update_data
{
	char		believe_god;
	short int	faith;
	short int	god_cast_level;
	char		name[20];
}k_char_update_data;

///////////////////////////////////////////////////////////
typedef struct tag_change_map9
{
	short int						game_port;
	char							name[20];
	
	short int						Gender;
	short int						Face;
	short int						Class;
	short int						Job;
	short int						Spell;
	short int						SprNo;
	unsigned char					Peacests;
	short int						BodyR;
	short int						BodyG;
	short int						BodyB;
	short int						ClothR;
	short int						ClothG;
	short int						ClothB;
	
	DWORD							total_id;
	
	short int						nk3;
	short int						nk4;
	short int						nk6;
	
	//NW_Character					NWCharacter;			010927 LTS
	//< CSD-011106
	int nTransPort;
	int nTransX;
	int nTransY;
	//> CSD-011106
	char							JoinLocalWar;			// LTS LOCALWAR
	char							GainedFame;	
	char							CurrentPhase;
	Event_Join						EventJoin;
	
	char							HorseNo;					// LTS HORSERIDER
	char							RiderNo;					// LTS HORSERIDER
	char							HorseGrade;					// LTS HORSERIDER
	char							RiderGrade;					// LTS HORSERIDER
	bool							bIsTradeOn;					//030127 lsw
	short							nSkillSadCount;				//030127 lsw
	short							nSkillBlessLeaf;			//030127 lsw
	WORD							nBillingType;
	int								nUserAge;					// 030929 kyo		
}k_change_map9;

typedef struct tag_change_map8
{
	short int						game_port;
	k_char_update_data				ch_data;
}k_change_map8;

typedef struct tag_change_map7
{
	short int						game_port;
	t_server_update_bankitem_data	ch_data;
}k_change_map7;

typedef struct tag_change_map6
{
	short int						game_port;
	t_server_update_item_data		ch_data;
}k_change_map6;

typedef struct tag_change_map5
{
	short int						game_port;
	t_server_update_inv_data		ch_data;
}k_change_map5;

typedef struct tag_change_map4
{
	short int						game_port;
	t_server_update_binary_data1	ch_data;
}k_change_map4;

typedef struct tag_change_map3
{
	short int						game_port;
	t_server_update_script_data		ch_data;
}k_change_map3;

typedef struct tag_change_map2
{
	short int						game_port;
	t_server_update_binary_data0	ch_data;
}k_change_map2;
typedef struct tag_change_map1
{
	short int			game_port;
	t_update_char_db	ch_data;
}k_change_map1;

// Added by chan78 at 2000/11/25 :: change_map_protocol (MAP to MAP)
typedef struct change_map_protocol
{
	// Agent Informations
	DWORD FromServerCn;
	WORD wAgentServerID;
	DWORD dwIDForAgent;
	
	char id[ID_LENGTH];
	char password[PW_LENGTH];
	char name[NM_LENGTH];
	char mapname[NM_LENGTH];
	char ip[NM_LENGTH];
	
	// Character Informations
	k_change_map1 mapdata1;
	k_change_map2 mapdata2;
	k_change_map3 mapdata3;
	k_change_map4 mapdata4;
	k_change_map5 mapdata5;
	k_change_map6 mapdata6;
	k_change_map7 mapdata7;
	k_change_map8 mapdata8;
	k_change_map9 mapdata9;
} t_change_map_protocol;

typedef struct map_change_result
{
	DWORD FromServerCn;
	DWORD TargetServerCn;
	WORD wAgentServerID;
	DWORD dwIDForAgent;
} t_map_change_result;

struct k_new_map_change_result
{
	t_map_change_result data1;
	union
	{
		k_event_object_sound	data2;
		k_event_sound			data3;
	}u;
};
///////////////////////////////////////////////////////////////////////////////

typedef struct tag_send_game_port
{
	short int game_port;
}k_send_game_port;

typedef struct tag_who_send_mail
{
	char name[20];
}k_who_send_mail;

typedef struct tag_game_to_client_rep_recv_mail_body
{
	DWORD mail_id;
	char body[1000];
}k_game_to_client_rep_recv_mail_body;

typedef struct tag_login_to_game_rep_recv_mail_body
{
	short int server_id;
	DWORD mail_id;
	char body[1000];
}k_login_to_game_rep_recv_mail_body;

typedef struct tag_game_to_login_req_recv_mail_body
{
	short int server_id;
	DWORD mail_id;
}k_game_to_login_req_recv_mail_body;

typedef struct tag_req_recv_mail_body
{
	DWORD mail_id;
}k_req_recv_mail_body;

struct k_login_to_game_guild_mail_title
{
	short int	server_id;
	DWORD		mail_id;
	char		degree;
	short int	mail_year;
	char		mail_month;
	char		mail_day;
	char		mail_name[20];
	char		mail_title[60];
};

typedef struct tag_login_to_game_mail_title
{
	short int	server_id;
	DWORD		mail_id;
	short int	mail_year;
	char		mail_month;
	char		mail_day;
	char		mail_name[20];
	char		mail_title[60];
}k_login_to_game_mail_title;

typedef struct tag_req_mail_body
{
	DWORD mail_id;
}k_req_mail_body;

struct k_guild_mail
{
	DWORD		mail_id;
	char		degree;
	short int	year;
	char		month;
	char		day;
	char		send_name[20];
	char		mail_title[60];
	char		mail_body[1000];
};


typedef struct tag_mail
{
	DWORD		mail_id;
	short int	year;
	char		month;
	char		day;
	char		send_name[20];
	char		mail_title[60];
	char		mail_body[1000];
	
}MAIL;

struct k_guild_mail_unit
{
	short int guild_code;
	char	degree;
};

typedef struct tag_game_to_login_send_mail
{
	char send_name[20];
	union
	{
		char recv_name[20];
		k_guild_mail_unit guild_info;		
	}uni;
	char title[60];
	char body[1000];
}k_game_to_login_send_mail;

typedef struct tag_send_mail
{
	char name[20];
	char title[60];
	char body[1000];
}k_send_mail;

typedef struct tag_moving_item
{
	char		name[20];
	char		source_type;
	char		dest_type;
	ItemAttr	item;
	DWORD		money;
}k_moving_item;

typedef struct tag_game2client_ear_result
{
	char ret;
}k_game2client_ear_result;

typedef struct tag_login2game_ear_message_result
{
	short int send_server_id;
	char	ret;									// ret  1 : ok, 2:죽음, 3:거절함
}k_login2game_ear_message_result;

typedef struct tag_send_ear_message_result
{
	short int send_game_server;
	short int send_server_id;
	char	ret;									// ret  1 : ok, 2:죽음, 3:거절함
}k_send_ear_message_result;

typedef struct tag_loginserver_to_gameserver_ear_message
{
	short int server_id;
	char name[20];
}k_loginserver_to_gameserver_ear_message;

typedef struct tag_send_ear_message_ok_to_login
{
	short int gameserver_port;
	short int server_id;
	char name[20];
	char ret;				// 결과
}k_send_ear_message_ok_to_login;

typedef struct tag_login_to_gameserver_ear_message
{
	short int game_server;		// 보낸사람이 속한 게임 서버
	short int server_id;		// 보낸사람의 아이디 
	char nation;				// 나라를 비교하기 위해
	char recv_name[20];				// 누구한테 보내는 가
	char send_name[20];				// 누가 보낸건가...
	char message[200];			// 어떤 메시지 인가...
}k_login_to_gameserver_ear_message;

typedef struct tag_login_send_ear_message
{
	short int server_id;
	char nation;			// 나라를 비교하기 위해
	char recv_name[20];		// 받는 사람이름
	char send_name[20];		// 보내는 사람 이름
	char message[200];
}k_login_send_ear_message;

typedef struct tag_send_ear_message_ok
{
	char name[20];
}k_send_ear_message_ok;

typedef struct tag_send_ear_message
{
	char name[20];
	short int send_game_server;		// 보낸 사람이 속한 게임 서버 포트번호
	short int send_server_id;		// 보낸 사람의 아이디
	char message[200];
}k_send_ear_message;

typedef struct server_to_client_char_info_plus		// 추가 캐릭터 정보를 클라이언트로 보내준다.
{
	char		believe_god;
	short int	faith;
}k_server_to_client_char_info_plus;


typedef struct server_killpc
{
	short   id;
	DWORD	killpc;
}k_server_killpc;


typedef struct tag_req_chr_info_etc
{
	short int	server_id;
	char		name[20];
}k_req_chr_info_etc;

typedef struct tag_get_char_info	// loginserver -> gameserver
{
	char		name[20];		// 030923 HK YGI
	short int	server_id;
	char		believe_god;
	short int	faith;
	short int	god_cast_level;
}k_get_char_info;

typedef struct tag_server_text_default
{
	char text[512];
	unsigned short int len;			// 총 길이
}k_server_text_default;

typedef struct tag_server_text_only_one
{
	char text[512];
	unsigned short int len;			// 총 길이
	short int server_id;
}k_server_text_only_one;

typedef struct tag_get_god_table
{
	char				god_idex;
	TIMESTAMP_STRUCT	day;
	TIMESTAMP_STRUCT 	month;
	TIMESTAMP_STRUCT 	year;
}k_get_god_table;

typedef struct tag_check_new_char
{
	char name[20];
}k_check_new_char;

typedef struct tool_password
{
	char password[11];
}k_tool_password;

typedef struct login_sql_evangelist_create
{
	char my_name[20];
	char eva_name[20];
}k_login_sql_evangelist_create;

typedef struct login_sql_evangelist_delete
{
	char my_name[20];
}k_login_sql_evangelist_delete;

typedef struct server_client_preach_name		// 결과
{
	char	result;
	char	name[20];
	BYTE	believe_god;
}k_server_client_preach_name;

typedef struct loginserver_result
{
	BYTE type;
	BYTE result;
	short int server_id;
}k_loginserver_result;

typedef struct loginserver_preach_name
{
	short int	server_id;
	char		name[20];
	BYTE		believe_god;
}k_loginserver_preach_name;

typedef struct loginserver_preach_name_result
{
	short int		server_id;
	char			result;
	char			name[20];
	BYTE			believe_god;
}k_loginserver_preach_name_result;

typedef struct client_god_regist_ok
{
	char type;
}k_client_god_regist_ok;

typedef struct server_preach_name
{
	char	name[20];
	BYTE	believe_god;
}k_server_preach_name;

typedef struct login_put_box_item
{
	//	short int server_id;		// 클라이언트로 보내줄 필요가 없기 때문에
	DWORD		box_id;
	char		para;				//위치
	ItemAttr	item;
}k_login_put_box_item;

typedef struct get_server_text
{
	char text[512];		
	char ct;						// 클레스의 배열
	unsigned short int len;			// 총길이
}k_get_server_text;

typedef struct req_bbs_title
{
	char count;
}k_req_bbs_title;

typedef struct transfer_item		//Only Use Exchange Item
{
	K_ITEM item;
}k_transfer_item;

typedef struct client_throw_dice
{
	char type;
}k_client_throw_dice;

typedef struct start_create_ability
{
	unsigned int str	: 2;
	unsigned int dex	: 2;
	unsigned int con	: 2;
	unsigned int wis	: 2;
	
	unsigned int inte	: 2;
	unsigned int cha	: 2;
	unsigned int movp	: 2;
	unsigned int endu	: 2;
	
	unsigned int mor	: 2;
	unsigned int wsps	: 2;
	unsigned int luck	: 2;
}k_start_create_ability;

typedef struct start_throw_dice		// 주사위 돌릴 경우 0~10 까지의 더하기 수치
{
	char ability1;
	char ability2;
	/*
	DWORD str	: 2;
	DWORD dex	: 2;
	DWORD con	: 2;
	DWORD wis	: 2;
	
	  DWORD inte	: 2;
	  DWORD cha	: 2;
	  DWORD movp	: 2;
	  DWORD endu	: 2;
	  
		DWORD mor   : 2;
		DWORD wsps	: 2;
		DWORD luck  : 2;
	*/
}k_start_throw_dice;
//< CSD-011107
typedef struct server_tactics
{
	unsigned __int8 nStep;
	DWORD	magery;
}k_server_tactics;
//> CSD-011107
typedef struct cmd_party_no_name
{
	short int server_id;
	char name[31];
}k_cmd_party_no_name;

typedef struct rep_party_together
{
	short int server_id;
	char party_name[31];
}k_rep_party_together;

typedef struct server_req_party_together
{
	char		party_name[31];
	char		my_name[31];
	short int	server_id;
}k_server_req_party_together;

typedef struct server_req_party_db
{
	short int	server_id;
	char		name[31];
	char		ct;
}k_server_req_party_db;

typedef struct server_server_party_info
{
	short int	Server_id;		// 이때 사용하는 서버 아이디는 요청한 클라이언트의 아이디 이다...
	char		ct;				// 파티 번호
	
	short int	Face;
	short int	Level;
	short int	Str;
	char 		Gender;
	char		Class;
}k_server_server_party_info;

typedef struct server_killmon
{
	DWORD	killmon;
}k_server_killmon;

typedef struct server_ac
{
	short int ac;
}k_server_ac;

typedef struct server_item_attr
{
	POS		pos;
	DWORD	attr;
}k_server_item_attr;

typedef struct beta_check_id
{
	char			id[ID_LENGTH];
}k_beta_check_id;

typedef struct tag_exchange_item_del
{
	POS			pos;
	short int	you_id;
}k_exchange_item_del;

typedef struct tag_try_item_trade
{
	char result;
}k_try_item_trade;

typedef struct tag_server_money
{
	DWORD money;
}k_server_money;

typedef struct tag_server_resist
{
	short int naBasicResist[MAX_RESIST_TYPE];
	short int naExtentionResist[MAX_RESIST_TYPE];
}k_server_resist;

/*
typedef struct tac_skill_each_exp
{
unsigned int tac_exp;
}t_tac_skill_each_exp;
*/
typedef struct tag_client_inn
{
	char type;			// 쉬기->3, 잠자기->3( 낮, 밤으로 나뉨 )
}k_client_inn;

typedef struct tag_party_info
{
	short int	type;
	char		index;
	char		m_Name[20];
	char		m_Gender;
	short int	m_ImageNo;
	short int	m_Str;
	BYTE		m_Lv; // CSD-030806	 
	char		m_Class;
	char		m_Join;
}k_party_info;


typedef struct tag_server_item_money
{
	int item_no;
	int money;
}k_server_item_money;

typedef struct tag_client_item_no
{
	int item_no;
}k_client_item_no;

typedef struct tag_client_disease
{
	short int disease_num;
}k_client_disease;

typedef struct tag_learn_magic_ok
{
	short int magic_num;
}k_learn_magic_ok;

typedef struct tag_client_char_name
{
	char	name[20];
}k_client_char_name;

typedef struct tag_client_learn_magic
{
	short int num;
}k_client_learn_magic;

typedef struct tag_client_delete_party
{
	short int	type;
	char		ct;
}k_client_delete_party;

typedef struct tag_client_do_party
{
	short int	type;
	char		ct;
	char		name[31];
}k_client_do_party;

typedef struct tag_server_bank_item
{
	char	first;
	char	second;
	ItemAttr inv[6];
}k_server_bank_item;

typedef struct tag_server_item_index
{
	short int item[1000];
	short int itemmax;
}k_server_item_index;

typedef struct tag_server_relation
{
	char		name[3][31];
	char		Gender[3];
	char		Level[3];
	short int	Face[3];
	char		Str[3];
	char		Class[3];
}k_server_relation;

typedef struct tag_server_party		// employment와같이 쓴다.
{
	char		name[6][31];
	char		Gender[6];
	char		Level[6];
	short int	Face[6];
	int			Str[6];
	char		Class[6];
}k_server_party;

typedef struct tag_server_disease
{
	char disease[6];
}k_server_disease;

typedef struct client_char_info_plus
{
	BYTE	type;
}k_client_char_info_plus;

typedef struct server_status_open
{
	DWORD	killmon;
	short int	ac;
	short int	mov;
	short int	fame;
}k_server_status_open;

//011014 lsw >
typedef struct server_skill_exp
{
	BYTE		type;
	t_skillexp   exp;
}k_server_skill_exp;
//011014 lsw <

typedef struct server_learn_skill_ok
{
	short int skill_no;
}k_server_learn_skill_ok;

typedef struct server_result
{
	BYTE	type;
	BYTE	result;
}k_server_result;

typedef struct server_item_repair_ok
{
	short int	item_pos;		// 고칠 위치 ( << 5 | <<3  | ... 사용 )
	short int	money;
	DWORD		dur;
}k_server_item_repair_ok;


typedef struct tag_item_no
{
	short int item_no;
}k_item_no;

typedef struct tag_client_learn_item
{
	short int item_no;
	short int join_no;
}k_client_learn_item;

typedef struct client_lvup_point_each
{
	unsigned char type;
}k_client_lvup_point_each;

typedef struct client_other_ch_inv
{
	short int other_ch;					// 엿보려는 사람의 아이디
}kein_client_other_ch_inv;

typedef struct server_other_ch_inv
{
	short int target_id;
	char item_count;
//	short int item[15];						// 엿보는 사람의 아이템 목록
	ItemAttr	item[15];
}kein_server_other_ch_inv;

typedef struct client_item_attr_pos		// 아이템 속성및 위치
{
	ItemAttr		item_attr;
	POS				item_pos;
	int				much;
}kein_item_attr_pos;

typedef struct client_bank_deposit
{
	DWORD			money;		// 총 예금한 금액
} k_client_bank_deposit;

typedef struct client_bank_repayment		
{
	DWORD			loan_money;
	DWORD			last_loan;
	DWORD			last_loan_time;
} k_client_bank_repayment;


typedef struct client_create_item
{
	int type;
	int num;
	DWORD dur;
	int   gennum;
} k_client_create_item;


typedef struct req_login_server_box_open
{
	short int	server_id;
	DWORD		box_id;
}k_req_login_server_box_open;

typedef struct login_server_box_open
{
	short int	server_id;		// 클라이언트 아이디
	ItemAttr	box_item[MAX_BOX_ITEM];
	DWORD		box_id;
}k_login_server_box_open;

typedef struct server_box_open
{
	ItemAttr	box_item[MAX_BOX_ITEM];
}k_server_box_open;

typedef struct server_delete_item
{
	POS				item_pos;
}k_server_delete_item;

typedef struct server_decrease_item
{
	POS				item_pos;
	DWORD			dur;
}k_server_decrease_item;

typedef struct tag_exchange_item_start
{
	K_ITEM		item;
	short int	you_id;
	int			iExchangeType;//021030 lsw
}k_exchange_item_start;


typedef union kein_imsi
{
	char					data[MAX_PACKET_SIZE];
	
	k_server_result					server_result;
	
	kein_server_other_ch_inv		server_other_ch_inv;
	kein_client_other_ch_inv		client_other_ch_inv;
	kein_item_attr_pos				item_attr_pos;
	
	k_client_bank_deposit			client_bank_deposit;
	k_client_bank_repayment			client_bank_repayment;
	k_client_create_item			client_create_item;
	K_ITEM							box_item;
	k_server_box_open				server_box_open;
	k_server_delete_item			server_delete_item;
	k_server_decrease_item			server_decrease_item;
	
	k_client_lvup_point_each		client_lvup_point_each;
	k_client_learn_item				client_learn_item;
	k_item_no						item_no;
	
	k_server_item_repair_ok			server_item_repair_ok;	
	k_server_learn_skill_ok			server_learn_skill_ok;
	k_server_skill_exp				server_skill_exp;
	
	k_server_status_open			server_status_open;
	k_client_char_info_plus			client_char_info_plus;
	k_server_party					server_party;
	k_server_relation				server_relation;
	k_server_item_index				server_item_index;
	k_server_disease				server_disease;
	
	k_server_bank_item				server_bank_item;	
	k_client_do_party				client_do_party;
	
	k_client_delete_party			client_delete_party;
	
	k_client_learn_magic			client_learn_magic;
	
	k_client_char_name				client_char_name;
	k_learn_magic_ok				learn_magic_ok;
	
	k_client_disease				client_disease;
	
	k_client_item_no				client_item_no;
	k_server_item_money				server_item_money;
	
	k_party_info					party_info;
	k_client_inn					client_inn;
	//	t_tac_skill_each_exp			tac_skill_each_exp;
	
	k_server_killpc					server_killpc;
	K_ITEM							server_item;
	k_server_resist					server_resist;
	
	k_server_money					server_money;
	
	K_ITEM							item;
	
	k_exchange_item_start			exchange_item_start;
	k_try_item_trade				try_item_trade;
	
	k_exchange_item_del				exchange_item_del;
	
	k_beta_check_id					beta_check_id;
	k_server_item_attr				server_item_attr;
	
	k_server_ac						server_ac;
	k_server_killmon				server_killmon;
	k_server_server_party_info		server_server_party_info;
	
	k_server_req_party_db			server_req_party_db;
	
	k_server_req_party_together		server_req_party_together;
	k_rep_party_together			rep_party_together;
	k_cmd_party_no_name				cmd_party_no_name;
	
	k_server_tactics							server_tactics;
	
	k_start_create_ability						start_create_ability;
	k_start_throw_dice							start_throw_dice;
	k_client_throw_dice							client_throw_dice;
	k_transfer_item								transfer_item;
	
	k_req_bbs_title								req_bbs_title;
	
	k_get_server_text							get_server_text;
	k_login_server_box_open						login_server_box_open;
	
	k_req_login_server_box_open					req_login_server_box_open;
	k_login_put_box_item						login_put_box_item;
	
	k_server_preach_name						server_preach_name;
	k_client_god_regist_ok						client_god_regist_ok;
	
	k_loginserver_preach_name					loginserver_preach_name;
	k_loginserver_result						loginserver_result;
	
	k_server_client_preach_name					server_client_preach_name;
	k_tool_password								tool_password;
	
	k_check_new_char							check_new_char;
	
	k_loginserver_preach_name_result			loginserver_preach_name_result;
	
	k_login_sql_evangelist_create				login_sql_evangelist_create;
	k_login_sql_evangelist_delete				login_sql_evangelist_delete;
	
	k_get_god_table								get_god_table;
	
	k_server_text_default						server_text_default;
	
	k_char_update_data							char_update_data;
	k_get_char_info								get_char_info;
	k_req_chr_info_etc							req_chr_info_etc;
	
	k_server_to_client_char_info_plus			server_to_client_char_info_plus;
	k_send_ear_message							send_ear_message;
	k_send_ear_message_ok						send_ear_message_ok;
	k_login_send_ear_message					login_send_ear_message;
	
	k_login_to_gameserver_ear_message			login_to_gameserver_ear_message;
	k_send_ear_message_ok_to_login				send_ear_message_ok_to_login;
	k_loginserver_to_gameserver_ear_message		loginserver_to_gameserver_ear_message;
	k_send_ear_message_result					send_ear_message_result;
	
	k_login2game_ear_message_result				login2game_ear_message_result;
	k_game2client_ear_result					game2client_ear_result;
	
	k_moving_item								moving_item;
	k_send_mail									send_mail;
	k_game_to_login_send_mail					game_to_login_send_mail;
	k_req_mail_body								req_mail_body;
	
	k_login_to_game_mail_title					login_to_game_mail_title;
	MAIL										mail;
	
	k_req_recv_mail_body						req_recv_mail_body;
	k_game_to_login_req_recv_mail_body			game_to_login_req_recv_mail_body;
	k_login_to_game_rep_recv_mail_body			login_to_game_rep_recv_mail_body;
	k_game_to_client_rep_recv_mail_body			game_to_client_rep_recv_mail_body;
	
	k_who_send_mail								who_send_mail;
	
	k_send_game_port							send_game_port;
	k_change_map1								change_map1;
	k_change_map2								change_map2;
	k_change_map3								change_map3;
	k_change_map4								change_map4;
	k_change_map5								change_map5;
	k_change_map6								change_map6;
	k_change_map7								change_map7;
	k_change_map8								change_map8;
	k_change_map9								change_map9;
	
	k_game2login_recall_box_item				game2login_recall_box_item;
	
	k_req_faith_up								req_faith_up;
	k_server_to_client_open_supper				server_to_client_open_supper;
	k_i_get_item_of_supper						i_get_item_of_supper;
	
	k_note_meeting								note_meeting;
	
	k_server_req_char_info						server_req_char_info;
	k_oh_my_party								oh_my_party;
	
	k_req_party_info							req_party_info;
	k_rep_party_info							rep_party_info;
	
	k_start_game_mail							start_game_mail;
	k_server_id									server_id;
	
	k_server_learn_item							server_learn_item;
	k_hunter_regist								hunter_regist;
	k_hunter_regist2							hunter_regist2;
	k_hunter_list								hunter_list;
	
	k_server_text_only_one						server_text_only_one;
	k_login_game_hunter_list					login_game_hunter_list;
	k_game_login_hunter_list					game_login_hunter_list;
	
	k_get_hunter_list							get_hunter_list;
	
	
	k_kill_hunter_list_login					kill_hunter_list_login;
	k_send_kill_hunter_list						send_kill_hunter_list;
	k_kill_who									kill_who;
	k_kill_hunter_ok							kill_hunter_ok;
	k_kill_who_whom								kill_who_whom;
	
	k_kill_who_whom_login						kill_who_whom_login;
	k_danger_of_hunter							danger_of_hunter;
	
	char										default_char;
	short int									default_short_int;
	int											default_int;
	DWORD										default_dword;
	ItemAttr									default_item;
	char										default_name[20];
	char										default_msg[MAX_MSG];
	POS											default_pos;
	
	k_do_party_ok								do_party_ok;
	
	k_req_man_to_man							req_man_to_man;
	k_req_man_to_man_result						req_man_to_man_result;
	k_rep_quest_index							rep_quest_index;
	
	k_team_add_message							team_add_message;
	k_reset_job									reset_job;
	
	k_script_change								script_change;
	k_change_dur								change_dur;
	k_exist_name								exist_name;
	
	k_exist_name_and_nation						exist_name_and_nation;
	k_vote_game2login							vote_game2login;
	
	k_regist_candidate							regist_candidate;
	k_is_this_man_candidate						is_this_man_candidate;
	k_check_vote								check_vote;
	k_check_vote_ok								check_vote_ok;
	k_check_vote_client_ok						check_vote_client_ok;
	
	k_set_value_of_vote							set_value_of_vote;
	
	k_req_vote_score							req_vote_score;
	k_rep_vote_score							rep_vote_score;
	k_client_rep_vote_score						client_rep_vote_score;
	
	k_get_skill_info							get_skill_info;
	k_send_login_nation_msg						send_login_nation_msg;
	
	k_guild_join_server							guild_join_server;
	k_guild_join_result							guild_join_result;
	
	k_guild_join_ok								guild_join_ok;
	k_scan_target								scan_target;
	
	k_guild_msg									guild_msg;
	k_guild_each_level							guild_each_level;
	
	k_gm_regist									gm_regist;
	k_bbs_all									bbs_all;
	k_gm_regist_db								gm_regist_db;
	k_send_gm_regist_ok							send_gm_regist_ok;
	
	k_send_login_req_stop_war					send_login_req_stop_war;
	k_send_client_req_stop_war					send_client_req_stop_war;
	
	k_rep_stop_war_login						rep_stop_war_login;
	k_stop_war_all_msg							stop_war_all_msg;
	
	k_send_game_server_req_stop_war				send_game_server_req_stop_war;
	
	k_db_salvation_money						db_salvation_money;
	k_db2game_salvation_money					db2game_salvation_money;
	k_req_salvation_money						req_salvation_money;
	
	k_total_salvation_all_msg					total_salvation_all_msg;
	
	k_send_db2map_each_item						send_db2map_each_item;
	k_change_money								change_money;
	
	k_eat_chocolate								eat_chocolate;
	k_eat_chocolate_ok							eat_chocolate_ok;
	k_login_check_chocolate						login_check_chocolate;
	k_check_chocolate							check_chocolate;
	
	k_openschool_data							openschool_data;
	k_openschool_data_ok						openschool_data_ok;
	
	k_openschool_data_result					openschool_data_result;
	
	k_guild_exit_global							guild_exit_global;
	k_eat_candy									eat_candy;
	
	k_name2										name2;
	k_loan_info									loan_info;
	k_fight_map_char_info						fight_map_char_info;
	k_character									character;
	k_name_message								name_message;
	
	k_merchant_item_bbs							merchant_item_bbs;
	k_recv_merchant_item_bbs					recv_merchant_item_bbs;
	k_update_ladder_score						update_ladder_score;
	k_req_rank_ladder_to_db						req_rank_ladder_to_db;
	
	k_db_get_ladder_score_rank					db_get_ladder_score_rank;
	k_direct_client								direct_client;
	k_send_db_direct_map						send_db_direct_map;
	k_send_db_direct_client						send_db_direct_client;
	k_send_effect								send_effect;
	k_visa_item									visa_item;
	k_visa_buy									visa_buy;
	k_nation_info_money							nation_info_money;
	
	char										nation_type;
	
	k_save_nation_info							save_nation_info;
	k_regist_guild_imsi							regist_guild_imsi;
	k_regist_guild_imsi_db						regist_guild_imsi_db;
	k_regist_guild_imsi_ok						regist_guild_imsi_ok;
	k_set_guild_info							set_guild_info;
	k_send_regist_imsi_guild_info				send_regist_imsi_guild_info;
	
	k_req_guild_info1							req_guild_info1;
	k_guild_info_name							guild_info_name;
	k_guild_info_degree							guild_info_degree;
	
	k_set_guild_number_info						set_guild_number_info;
	k_guild_degree_info							guild_degree_info;
	
	k_guild_instroduction						guild_instroduction;
	k_save_guild_member_list					save_guild_member_list;
	k_guild_mail_id								guild_mail_id;
	k_guild_mail								guild_mail;
	
	k_login_to_game_guild_mail_title			login_to_game_guild_mail_title;
	k_delete_guild_mail							delete_guild_mail;
	k_guild_instroduction_basic					guild_instroduction_basic;
	
	k_guild_chat								guild_chat;
	k_guild_chat_map							guild_chat_map;
	
	k_reset_ability								reset_ability;
	k_gamble_data								gamble_data;
	k_change_color								change_color;
	k_change_color_db							change_color_db;
	k_change_color_map2client					change_color_map2client;
	k_nation_money								nation_money;
	k_gamble_item								gamble_item;
	k_gamble_item_result						gamble_item_result;
	k_put_menu_string							put_menu_string;
	k_focuse_logout_map							focuse_logout_map;
	k_direct_map2user							direct_map2user;
	k_today_gamble_item							today_gamble_item;
	
	k_potal_map									potal_map;
	k_set_map_type								set_map_type;
	k_save_event_item							save_event_item;
	
	k_put_guild_item							put_guild_item;
	k_get_guild_item_map						get_guild_item_map;
	k_get_guild_item_db							get_guild_item_db;
	k_change_guild_degree						change_guild_degree;
	
	k_check_guild_change						check_guild_change;
	k_nation_item								nation_item;
	k_colossus_info_map							colossus_info_map;
	k_direct_map_req							direct_map_req;
	k_colossus_info_map_reply					colossus_info_map_reply;
	k_invite_colossus_map						invite_colossus_map;
	k_attack_colossus_stone_result				attack_colossus_stone_result;
	k_team_battle_msg2							team_battle_msg2;
	k_team_battle_msg3							team_battle_msg3;
	k_change_guild_house_info					change_guild_house_info;
	k_change_guild_house_info_db2map			change_guild_house_info_db2map;
	k_check_guild_house							check_guild_house;
	k_item_log									item_log;
	k_direct_map_move							direct_map_move;
	
	// acer4
	k_char_info									char_info;
	k_char_info_map								char_info_map;
	k_setting_map_tile							setting_map_tile;
	k_send_dungeon_history						send_dungeon_history;
	k_get_scenario_info							get_scenario_info;
	k_check_goto_scenario_dungeon_map			check_goto_scenario_dungeon_map;
	k_scenario_message							scenario_message;
	k_scenario_detail_info						scenario_detail_info;
	k_scenario_trap_key							scenario_trap_key;
	//< CSD-030306
	k_client_scenario_time_info					client_scenario_time_info; 
	k_server_scenario_time_info                 server_scenario_time_info;
	k_scenario_time_change                      scenario_time_change;
	//> CSD-030306
	k_map_move									map_move;
	k_new_map_change_result						new_map_change_result;		// 맵서버 끼리
	k_new_server_change_map						new_server_change_map;		// 맵서버에서 클라이언트로
	k_event_move_map_check_map					event_move_map_check_map;
	k_event_move_map_ok							event_move_map_ok;
	k_save_event_move_map						save_event_move_map;
	
	k_login_check_double_name					login_check_double_name;
	k_check_double_when_first					check_double_when_first;
	k_check_recv_mail							check_recv_mail; // CSD-030227
	k_save_limit_number							save_limit_number;		// 031009 CI YGI
// 040105 YGI 보물
	k_get_treasure_map_detail_to_db				get_treasure_map_detail_to_db;
	k_get_treasure_map_detail_to_map			get_treasure_map_detail_to_map;

	// 040601 YGI
	k_get_item_mall								get_item_mall;
	k_send_item_mall_item_kind					send_item_mall_item_kind;
	k_item_mall_result_to_db					item_mall_result_to_db;
	
	//<050224_KCH 마일리지몰 작업
	k_get_item_mall_first						get_item_mall_first;
	k_get_item_mall_item						get_item_mall_item;
	k_req_guild_item							req_item_mall_item;

	k_Delete_Item_Mall_ARRAY					delete_item_mall_array;
	k_Info_Item_Mall							Info_Item_Mall;
	//>050224_KCH 마일리지몰 작업

	//< LTH-040719-KO 1.04p Patch
	t_BadUserInfo								tagBadUserInfo;						// LTH-040719-KO 불량 유저 정보
	t_PrisonSysProcResult						tagPrisonSysProcResult;				// LTH-040727-KO 감옥 시스템 처리 결과.
	t_RequestMoveToPrison						tagRequestMoveToPrison;				// LTH-040813-KO 다른맵의 불량유저를 감옥으로 옮겨달라고 요청.
	//> LTH-040719-KO
} t_kein;


