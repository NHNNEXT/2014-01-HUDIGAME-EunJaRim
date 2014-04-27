﻿#pragma once

#include <WinSock2.h>
#include "EnumSet.h"

class Corps;
class ClientSession;
class ActionScheduler;
class Action;
struct PositionInfo;
struct PacketHeader;
struct DatabaseJobContext;
struct Tile
{
	unsigned char R; // 맵 데이터의 R값 아직 어떤 속성값을 넣을지 정해지지 않음 맵툴쪽과 합의 필요
	unsigned char G; 
	unsigned char B; // 아마도 height값이 올 예정
};

class ClientManager
{
public:
	ClientManager( int roomNumber );
	~ClientManager();

	void				GameStart();


	ClientSession*		CreateClient( SOCKET sock );

	bool				DeleteClient( ClientSession* client );
	void				InputClient( ClientSession* client );

	int					GetClientSize() { return static_cast<int>(m_ClientList.size()); }
	int					GetRoomNumber() { return m_RoomNumber; }

	void				BroadcastPacket( ClientSession* from, PacketHeader* pkt );
	bool				DirectPacket( int pid, PacketHeader* pkt );

	void				OnPeriodWork();


	bool				ReadMapFile( const char* filename );




	int					GenerateCorps( int playerID, UnitType type, PositionInfo position );
	void				AddActionToScheduler( Action* addedAction, int64_t remainTime );


	/// DB에 플레이어 정보를 생성하거나 삭제하는 함수
	void				DBCreatePlayer( int pid, double x, double y, double z, const char* name, const char* comment );
	void				DBDeletePlayer( int pid );

	void				FlushClientSend();

	void				PrintClientList( ); // 테스트용 함수

	Corps*				GetCorpsByCorpsID( int corpsID );


private:
	void				CreatePlayerDone( DatabaseJobContext* dbJob );
	void				DeletePlayerDone( DatabaseJobContext* dbJob );

private:
	void				CollectGarbageSessions();
	void				ClientPeriodWork();
	void				DispatchDatabaseJobResults();


private:
	int					m_RoomNumber;

	typedef std::map<SOCKET, ClientSession*> ClientList;
	ClientList			m_ClientList;

	typedef std::hash_map<int, Corps*> CorpsList;
	CorpsList			m_CorpsList;
	int					m_CorpsIDCount = 0;

	DWORD				m_LastGCTick;
	DWORD				m_LastClientWorkTick;


	std::vector<std::vector<Tile>>	m_BattleMap;

	bool				m_IsGameStart = false;
	ActionScheduler*	m_ActionScheduler = nullptr;
};