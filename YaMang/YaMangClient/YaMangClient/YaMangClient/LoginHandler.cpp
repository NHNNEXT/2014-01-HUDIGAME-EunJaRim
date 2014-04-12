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
	if ( NetworkManager::GetInstance( )->m_RecvBuffer.Read( (char*)&recvData, recvData.m_Size ) )
	{
		// ��Ŷó��
		if ( recvData.m_PlayerId == -1 )
		{
			/// ���� �ɸ��� �α��� ���д�.
			ExitProcess( -1 );
		}

		printf_s( "My playerID[%d] \n", recvData.m_PlayerId );
		NetworkManager::GetInstance()->m_MyPlayerId = recvData.m_PlayerId;

		// 				/// ä�� ��� ��Ŷ ������ Ÿ�̸� ������.. 
		// 				SetTimer( hWnd, 337, 3000, NULL );
	}
	else
	{
		assert( false );
	}
}
