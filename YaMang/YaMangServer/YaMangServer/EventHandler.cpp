#include "stdafx.h"
#include "ClientSession.h"
#include "PacketType.h"
#include "DatabaseJobContext.h"
#include "DatabaseJobManager.h"

//////////////////////////////////////////////////////////////////////////
#include "RoomManager.h"
#include "ClientManager.h"
// �׽�Ʈ�� ���

extern RoomManager* g_RoomManager;
//////////////////////////////////////////////////////////////////////////

HandlerFunc HandlerTable[PKT_MAX];

static void DefaultHandler( ClientSession* session, PacketHeader& pktBase )
{
	assert( false );
	session->Disconnect();
}

struct InitializeHandlers
{
	InitializeHandlers()
	{
		for ( int i = 0; i < PKT_MAX; ++i )
		{
			HandlerTable[i] = DefaultHandler;
		}

	}
} _init_handlers_;

struct RegisterHandler
{
	RegisterHandler( int pktType, HandlerFunc handler )
	{
		HandlerTable[pktType] = handler;
	}
};

#define REGISTER_HANDLER(PKT_TYPE)	\
	static void Handler_##PKT_TYPE( ClientSession* session, PacketHeader& pktBase ); \
	static RegisterHandler _register_##PKT_TYPE( PKT_TYPE, Handler_##PKT_TYPE ); \
	static void Handler_##PKT_TYPE( ClientSession* session, PacketHeader& pktBase )

REGISTER_HANDLER( PKT_CS_LOGIN )
{
	LoginRequest inPacket = static_cast<LoginRequest&>( pktBase );
	session->HandleLoginRequest( inPacket );
}

void ClientSession::HandleLoginRequest( LoginRequest& inPacket )
{
	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	/// �α����� DB �۾��� ���ľ� �ϱ� ������ DB �۾� ��û�Ѵ�.
	LoadPlayerDataContext* newDbJob = new LoadPlayerDataContext( m_Socket, inPacket.m_PlayerId );
	g_DatabaseJobManager->PushDatabaseJobRequest( newDbJob );
}

REGISTER_HANDLER( PKT_CS_CHAT )
{
	ChatBroadcastRequest inPacket = static_cast<ChatBroadcastRequest&>( pktBase );
	session->HandleChatRequest( inPacket );
}

//////////////////////////////////////////////////////////////////////////
// �ӽ÷� �߰� �� �κ�
//////////////////////////////////////////////////////////////////////////
std::map<int, std::array<int, 4>> g_EventTimeTable;

struct EventData
{
	int				targetId = 0;
	ClientSession*	session = nullptr;
};

void CALLBACK StopEvent( LPVOID lpArg, DWORD dwTimerLowValue, DWORD dwTimerHighValue )
{
	for ( auto& iter : g_EventTimeTable )
	{
		auto key = iter.first;
		auto& value = iter.second;

		for ( int i = 0; i < 4; ++i )
		{
			if ( value[i] > 0 )
			{
				++value[i];
			}
		}

		ChatBroadcastResult inPacket;
		inPacket.m_PlayerId = key;
		strcpy_s( inPacket.m_Name, "MSG" );

		if ( value[0] > 3 )
		{
			value[0] = 0;
			strcpy_s( inPacket.m_Chat, "StopUp" );
			auto it = g_RoomManager->m_Lobby;
			if ( it )
			{
				printf_s( "%d�� Ŭ���̾�Ʈ Stop! \n", key );
				it->DirectPacket( key, &inPacket );
			}
		}
		if ( value[1] > 3 )
		{
			value[1] = 0;
			strcpy_s( inPacket.m_Chat, "StopDown" );
			auto it = g_RoomManager->m_Lobby;
			if ( it )
			{
				printf_s( "%d�� Ŭ���̾�Ʈ Stop! \n", key );
				it->DirectPacket( key, &inPacket );
			}
		}
		if ( value[2] > 3 )
		{
			value[2] = 0;
			strcpy_s( inPacket.m_Chat, "StopLeft" );
			auto it = g_RoomManager->m_Lobby;
			if ( it )
			{
				printf_s( "%d�� Ŭ���̾�Ʈ Stop! \n", key );
				it->DirectPacket( key, &inPacket );
			}
		}
		if ( value[3] > 3 )
		{
			value[3] = 0;
			strcpy_s( inPacket.m_Chat, "StopRight" );
			auto it = g_RoomManager->m_Lobby;
			if ( it )
			{
				printf_s( "%d�� Ŭ���̾�Ʈ Stop! \n", key );
				it->DirectPacket( key, &inPacket );
			}
		}
	}
}

void ClientSession::HandleChatRequest( ChatBroadcastRequest& inPacket )
{
	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	ChatBroadcastResult outPacket;
	outPacket.m_PlayerId = inPacket.m_PlayerId;
	strcpy_s( outPacket.m_Name, m_PlayerName );
	// strcpy_s( outPacket.m_Chat, inPacket.m_Chat );

	//////////////////////////////////////////////////////////////////////////
	// �ӽ÷� ���� �� ��
	//////////////////////////////////////////////////////////////////////////
	std::string packetMessage;
	packetMessage.append( inPacket.m_Chat );

	if ( g_EventTimeTable.find( inPacket.m_PlayerId ) == g_EventTimeTable.end() )
	{
		ZeroMemory( &(g_EventTimeTable[inPacket.m_PlayerId]), sizeof( std::array<int, 4> ) );
	}

	if ( packetMessage == "IsMoveUpOK")
	{
		if ( g_EventTimeTable[inPacket.m_PlayerId][0] > 0 )
			return;
		g_EventTimeTable[inPacket.m_PlayerId][0] += 1;
		strcpy_s( outPacket.m_Chat, "MoveUp" );
		printf_s( "%d�� Ŭ���̾�Ʈ Move! \n", inPacket.m_PlayerId );
		SendRequest( &outPacket );
		return;
	}
	else if ( packetMessage == "IsMoveDownOK")
	{
		if ( g_EventTimeTable[inPacket.m_PlayerId][1] > 0 )
			return;
		g_EventTimeTable[inPacket.m_PlayerId][1] += 1;
		strcpy_s( outPacket.m_Chat, "MoveDown" );
		printf_s( "%d�� Ŭ���̾�Ʈ Move! \n", inPacket.m_PlayerId );
		SendRequest( &outPacket );
		return;
	}
	else if ( packetMessage == "IsMoveLeftOK" )
	{
		if ( g_EventTimeTable[inPacket.m_PlayerId][2] > 0 )
			return;
		g_EventTimeTable[inPacket.m_PlayerId][2] += 1;
		strcpy_s( outPacket.m_Chat, "MoveLeft" );
		printf_s( "%d�� Ŭ���̾�Ʈ Move! \n", inPacket.m_PlayerId );
		SendRequest( &outPacket );
		return;
	}
	else if ( packetMessage == "IsMoveRightOK")
	{
		if ( g_EventTimeTable[inPacket.m_PlayerId][3] > 0 )
			return;
		g_EventTimeTable[inPacket.m_PlayerId][3] += 1;
		strcpy_s( outPacket.m_Chat, "MoveRight" );
		printf_s( "%d�� Ŭ���̾�Ʈ Move! \n", inPacket.m_PlayerId );
		SendRequest( &outPacket );
		return;
	}
	else
	{
		strcpy_s( outPacket.m_Chat, inPacket.m_Chat );
	}
	//////////////////////////////////////////////////////////////////////////
	// �� ������ �ӽ÷� ������ ����

	/// ä���� �ٷ� ��� �ϸ� ��
	if ( !Broadcast( &outPacket ) )
	{
		Disconnect();
	}
}

REGISTER_HANDLER( PKT_CS_GAMEOVER )
{
	GameOverRequest inPacket = static_cast<GameOverRequest&>( pktBase );
	session->HandleGameOverRequest( inPacket );
}

void ClientSession::HandleGameOverRequest( GameOverRequest& inPacket )
{

	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	std::string packetMessage;
	packetMessage.append( inPacket.m_Chat );

	//////////////////////////////////////////////////////////////////////////
	// �׽�Ʈ������ �ӽ÷� �ٿ���
	//////////////////////////////////////////////////////////////////////////
	try
	{
		int pid = stoi( packetMessage.substr( 1, 4 ) );
		// ���� ��Ȳ�� �߻� �� �� ����
		//
		// 1) 4���� �̸��� ��� ��!
		// 2) ���ڰ� �ƴ� ��� ��!

		GameOverResult outPacket;
		outPacket.m_PlayerId = pid;

		if ( packetMessage.at( 0 ) == 'W' )
		{
			outPacket.m_IsWon = true;
		}
		else
		{
			outPacket.m_IsWon = false;
		}

		printf_s( "[GameOverMessage][%d]%s \n", inPacket.m_PlayerId, inPacket.m_Chat );

		/// ä���� �ٷ� ��� �ϸ� ��
		if ( !Broadcast( &outPacket ) )
		{
			Disconnect();
		}
	}
	catch (...)
	{
		return;
	}
}



REGISTER_HANDLER( PKT_CS_ROOM_CREATE )
{
	RoomCreateRequest inPacket = static_cast<RoomCreateRequest&>( pktBase );
	session->HandleRoomCreateRequest( inPacket );
}

void ClientSession::HandleRoomCreateRequest( RoomCreateRequest& inPacket )
{

	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	try
	{
		// ������ �� ����� ��û�ϴ� ����� �׹����� �־��ִ°� ������?
		int pid = inPacket.m_PlayerId;

		int roomNumber = g_RoomManager->AddRoom();

		RoomCreateResult outPacket;

		outPacket.m_RoomNumber = roomNumber;

		if ( !DirectSend( &outPacket ) )
		{
			Disconnect();
		}

	}
	catch ( ... )
	{
		return;
	}
}




REGISTER_HANDLER( PKT_CS_ROOM_CHANGE )
{
	RoomChangeRequest inPacket = static_cast<RoomChangeRequest&>( pktBase );
	session->HandleRoomChangeRequest( inPacket );
}

void ClientSession::HandleRoomChangeRequest( RoomChangeRequest& inPacket )
{

	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	try
	{
		int pid = inPacket.m_PlayerId;

		int roomFrom = inPacket.m_RoomFrom;
		int roomTo = inPacket.m_RoomTo;

		if ( !g_RoomManager->ChangeRoom( roomFrom, roomTo, pid ) )
		{
			Disconnect();
		}

		RoomChangeResult outPacket;
		outPacket.m_RoomNumber = roomTo;

		if ( !DirectSend( &outPacket ) )
		{
			Disconnect();
		}

		g_RoomManager->PrintClientList(); // �׽�Ʈ ����Ʈ
	}
	catch ( ... )
	{
		return;
	}
}