#include "stdafx.h"
#include "RoomManager.h"
#include "ClientSession.h"
#include "ClientManager.h"


RoomManager* g_RoomManager = nullptr;

RoomManager::RoomManager()
{
	ClientManager* waitingRoom = new ClientManager();
	//m_RoomList.push_back( RoomList::value_type( 0, waitingRoom ) ); // 0�� ������ ����
	m_RoomList.insert( RoomList::value_type( 0, waitingRoom ) ); // 0�� ������ ����
}


RoomManager::~RoomManager()
{
	for ( auto& it : m_RoomList )
	{
		ClientManager* room = it.second;
		delete room;
	}
}

ClientSession* RoomManager::CreateClient( SOCKET sock )
{
	// ��ȸ�� �� �ʿ� ������ ���߿� map���� vector�� �ٲܿ���
	for ( auto& it : m_RoomList )
	{
		// ���濡 ����
		int roomNumber = it.first;
		if ( 0 == roomNumber )
		{
			ClientManager* room = it.second;
			return room->CreateClient( sock );
		}
	}

	CRASH_ASSERT( false );
	return nullptr;
}

void RoomManager::FlushClientSend()
{
	for ( auto& it : m_RoomList )
	{
		ClientManager* room = it.second;
		room->FlushClientSend();
	}
}

void RoomManager::OnPeriodWork()
{
// 	for ( auto& it = m_RoomList.begin( ); it != m_RoomList.end( ); ++it )
// 	{
// 		ClientManager* room = it.second;
// 		room->OnPeriodWork();
// 	}
	for ( auto& it : m_RoomList )
	{
		ClientManager* room = it.second;
		room->OnPeriodWork();
	}
}
