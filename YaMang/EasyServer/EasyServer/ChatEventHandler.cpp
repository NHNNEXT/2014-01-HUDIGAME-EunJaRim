#include "stdafx.h"
#include "ChatEventHandler.h"


ChatEventHandler::ChatEventHandler()
{
}


ChatEventHandler::~ChatEventHandler()
{
}

void ChatEventHandler::HandleEvent( ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket )
{
	ChatBroadcastRequest inPacket;
	buffer->Read( (char*)&inPacket, header->mSize );
	printf_s( "CHAT RECV:%s \n", inPacket.mChat );

	ChatBroadcastResult outPacket;
	outPacket.mPlayerId = inPacket.mPlayerId;
	strcpy_s( outPacket.mName, client->GetPlayerName() );
	strcpy_s( outPacket.mChat, inPacket.mChat );

	/// ä���� �ٷ� ��� �ϸ� ��
	if ( !client->Broadcast( &outPacket ) )
		return;
}
