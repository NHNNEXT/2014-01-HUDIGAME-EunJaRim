#include "stdafx.h"
#include "TestEventHandler.h"


TestEventHandler::TestEventHandler()
{
}


TestEventHandler::~TestEventHandler()
{
}

void TestEventHandler::HandleEvent( ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket )
{
	ChatBroadcastRequest inPacket;
	buffer->Read( (char*)&inPacket, header->mSize );
	printf_s( "TEST RECV:%s \n", inPacket.mChat );

	ChatBroadcastResult outPacket;
	outPacket.mPlayerId = inPacket.mPlayerId;
	strcpy_s( outPacket.mName, client->GetPlayerName() );
	strcpy_s( outPacket.mChat, inPacket.mChat );

	/// ä���� �ٷ� ��� �ϸ� ��
	if ( !client->Broadcast( &outPacket ) )
		return;
}
