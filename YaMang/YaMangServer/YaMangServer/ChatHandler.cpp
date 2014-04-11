#include "stdafx.h"
#include "ChatHandler.h"


ChatHandler::ChatHandler()
{
}


ChatHandler::~ChatHandler()
{
}

void ChatHandler::HandleEvent( ClientSession* session, PacketHeader& pktBase )
{
	ChatBroadcastRequest inPacket = static_cast<ChatBroadcastRequest&>( pktBase );

	session->mRecvBuffer.Read( (char*)&inPacket, inPacket.mSize );

	ChatBroadcastResult outPacket;
	outPacket.mPlayerId = inPacket.mPlayerId;
	strcpy_s( outPacket.mName, session->mPlayerName );
	strcpy_s( outPacket.mChat, inPacket.mChat );

	printf_s( "[%s]%s \n", session->mPlayerName, inPacket.mChat );

	/// ä���� �ٷ� ��� �ϸ� ��
	if ( !session->Broadcast( &outPacket ) )
	{
		session->Disconnect();
	}
}
