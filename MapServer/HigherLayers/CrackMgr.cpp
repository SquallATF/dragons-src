// CrackMgr.cpp: implementation of the CCrackMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
#include "CrackMgr.h"

#include "LogManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*041229_KCH_Taiwan(Acer) CrackingMgr -> Client �������ܵǴ� ����. [ex)�Ʒ������� �����ڰ� ���ɶ�, ������ ��98pc���� Accelater�۵����� �߸� �ν��Ͽ� ���ܵ�]
 �׷��� ������ �ð����� �ÿ��ֵ��� ����.(�׽�Ʈ�� ���� Ʃ���� �ʿ�������, ���� 30�ʸ� 60�ʷ� �ÿ��ֵ��� ����)
 ���� Client������ CHECK_ACCEL_RECOMMAND_TIME �ð����� ���� �����ؾ� �Ѵ�.
const	int DEFAULT_CHECK_ACCEL_TIME	= 35000;//�ݵ�� CHECK_ACCEL_RECOMMAND_TIME���� 5�� �̻� Ŀ�� �Ѵ� ������ ms
const	int CHECK_ACCEL_RECOMMAND_TIME	= 30000;//30 ��
const	int CHECK_ACCEL_PING_TIME		= 1800; */

const	int DEFAULT_CHECK_ACCEL_TIME	=	100000; //�ݵ�� CHECK_ACCEL_RECOMMAND_TIME ������ �ð��� ������ ���ؼ� �������.(Accel�ð��� �ƴѰ����� �����ϰڴ�.) ������ ms
												   //050408_KCH ^40�� ���� �����Ƿ�, ���߰����� �����Ұ����� ����.
const	int CHECK_ACCEL_RECOMMAND_TIME	=	 60000; //050328_KCH �̽ð����� ��ٸ��ٰ�, ������ �޼����� �뺸�Ѵ�.(�����ؾ� �ҽð� 60��)
const	int CHECK_ACCEL_PING_TIME		=	  1800;

//<050421_KCH Accelerate üũ ��ƾ ����(ȿ���̹�,AMDPC���� ���ܹ���)
CCrackMgr CrackMgr;
//>050421_KCH

CCrackMgr::CCrackMgr()
{

}

CCrackMgr::~CCrackMgr()
{

}

void CCrackMgr::AutoCheckAccelator(CHARLIST *ch)
{
	const int cn = ch->GetServerID();
	const DWORD dwRealTime = timeGetTime();
	const int iMyState = connections[cn].state;

	if(iMyState==CONNECT_JOIN  || iMyState==CONNECT_JOIN_START)//���� ���� ��츸 üũ �Ѵ�
	{
		if( dwRealTime >=  ch->dwCheckAccelTime)//�ð� üũ
		{
			if( ch->dwSendedAccelTime )//�Ʊ� ���´µ� ���� ������ ���� ������ ������
			{
				return;
			}
			t_packet	packet;
			ch->dwCheckAccelTime = dwRealTime + DEFAULT_CHECK_ACCEL_TIME;
			ch->dwSendedAccelTime = dwRealTime;
			packet.h.header.type = CMD_CHECK_ACCEL;
			packet.h.header.size = sizeof(t_server_check_accel);
			packet.u.server_check_accel.dwCheckAccelNowtime	= ch->dwSendedAccelTime;//������ �ð�.
			::QueuePacket(connections, cn, &packet, 1);
		}
	}
	else//�������� �ƴϸ�
	{
		ch->dwSendedAccelTime = 0;//������ ��ȿȭ ��Ų��
	}	
	return;
}

void CCrackMgr::ComformAccelator( const int cn, const t_packet *packet )
{
	CHARLIST *ch = ::CheckServerId(cn);
	if(!ch) {return;}

	char msg[300] = {0,};
	const DWORD dwRealtime = timeGetTime();	
	const DWORD dwRecvSendedServerTime	=	packet->u.server_check_accel.dwCheckAccelNowtime;
	const DWORD dwSendedAccelTime		=	ch->dwSendedAccelTime;
	const int iMyState = connections[cn].state;
	//������ ������ (���� 90����- ��Ÿ��)���� (���ݱ���) �̴�
	
	if(iMyState==CONNECT_JOIN  || iMyState==CONNECT_JOIN_START)//���� ���� ��츸 üũ �Ѵ�
	{
		if	((	dwRealtime-dwRecvSendedServerTime < CHECK_ACCEL_RECOMMAND_TIME)//-CHECK_ACCEL_PING_TIME )//CHECK_ACCEL_RECOMMAND_TIME ���� ���� �۴�//CHECK_ACCEL_PING_TIME ���ִٰ� ģ��
			||(	dwSendedAccelTime != dwRecvSendedServerTime))//�Ʊ� ���� ���� �ٸ� ���� ���´�
		{
			sprintf( msg,"Accel Time Error 90 Never.. \nSended = %d , Recved = %d , Value = %d",dwSendedAccelTime, dwRecvSendedServerTime, dwRealtime-dwRecvSendedServerTime);

			if(g_accelator_user_closeconnection )	
			{	
				g_pLogManager->SaveLogAccelater(ch, msg,cn);	//050328_KCH �α� �߰�.
				SendHackingUser( cn, HACKING_ACCELATOR_, msg );
				//<0540413_KCH ��а� �α׸� Ȯ���ϰ�, Ŭ���̾�Ʈ ������ ���� �ʴ´�. �ֳ��ϸ�, ����, ����1500�� ��Ŷ�� �и��� ������, ������ �������Ŀ�, �߰��Ұ�
//				closeconnection( connections, cn, -208 );
				//>0540413_KCH
			}
		}
		ch->dwSendedAccelTime = 0;//�޾����ϱ� ���´� �ð��� Ŭ���� ���ش�// ��ġ ���� ����. �������� ����. �ּ� ó�� ���� ����
	}
	else
	{
		ch->dwSendedAccelTime = 0;
	}
}

void CCrackMgr::SendHackingUser( const int cn, const int type, const char *cause )
{
	t_packet p;
	CHARLIST * ch =	::CheckServerId(cn);
	if(!ch){return;}
	if(ch->SprType == SPRITETYPE_NPC ) {return;}

	p.h.header.type = CMD_HACKING_USER;
	p.h.header.size = sizeof( t_hacking_user );
	t_hacking_user *tp = &p.u.hacking_user;
	tp->type = type;//	short	type;
	::memcpy( tp->id, connections[cn].id ,20);//	char	id[ 20];
	::memcpy( tp->name, connections[cn].name ,20);//	char	name[20];
	::memcpy( tp->ip, connections[cn].ip_address,20 );//	char	ip[20];
	::memcpy( tp->cause, cause ,100);//	char	cause[100];

	QueuePacket( connections, DB_DEMON, &p, 1 );
}

void CCrackMgr::RecvAccelType(int cn, t_packet *p)
{	//< CSD-030808
	CHARLIST* pTarget = ::CheckServerId(cn);

	if (pTarget == NULL)
	{
		return;
	}

	t_packet t;
	t.h.header.type = CMD_SEND_ACCEL_TYPE;
	t.h.header.size = 0;
	QueuePacket(connections,cn,&t,1);
	
	g_pLogManager->SaveLogAccelater(pTarget, p->u.Accel_Type.strMsg,cn);
}	//> CSD-030808
