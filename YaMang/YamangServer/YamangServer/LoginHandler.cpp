#include "stdafx.h"
#include "LoginHandler.h"
#include "DatabaseJobContext.h"
#include "DatabaseJobManager.h"


LoginHandler::LoginHandler()
{
}


LoginHandler::~LoginHandler()
{
}

void LoginHandler::HandleEvent( ClientSession* session, PacketHeader& pktBase )
{
	LoginRequest inPacket = static_cast<LoginRequest&>( pktBase );
	session->mRecvBuffer.Read( (char*)&inPacket, inPacket.mSize );

	/// �α����� DB �۾��� ���ľ� �ϱ� ������ DB �۾� ��û�Ѵ�.
	LoadPlayerDataContext* newDbJob = new LoadPlayerDataContext( session->mSocket, inPacket.mPlayerId );
	GDatabaseJobManager->PushDatabaseJobRequest( newDbJob );
}


