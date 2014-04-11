#include "stdafx.h"
#include "LoginHandler.h"
#include "NetworkManager.h"


LoginHandler::LoginHandler()
{
}


LoginHandler::~LoginHandler()
{
}

void LoginHandler::HandleEvent( PacketHeader& pktBase )
{
	LoginResult recvData = static_cast<LoginResult&>( pktBase );
	if ( NetworkManager::GetInstance( )->m_RecvBuffer.Read( (char*)&recvData, recvData.mSize ) )
	{
		// ��Ŷó��
		if ( recvData.mPlayerId == -1 )
		{
			/// ���� �ɸ��� �α��� ���д�.
			ExitProcess( -1 );
		}

		// 				printf_s( "player[%d] \n", recvData.mPlayerId );
		// 				g_MyClientId = recvData.mPlayerId;
		// 				g_LoginComplete = true;
		// 
		// 				/// ä�� ��� ��Ŷ ������ Ÿ�̸� ������.. 
		// 				SetTimer( hWnd, 337, 3000, NULL );
	}
	else
	{
		assert( false );
	}
}
