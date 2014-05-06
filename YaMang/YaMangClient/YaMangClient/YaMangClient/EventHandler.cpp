﻿#include "stdafx.h"
#include "PacketType.h"
#include "NetworkManager.h"
#include "GameManager.h"
#include "Corps.h"
#include "SceneManager.h"
#include "ScenePlay.h"
#include "TextManager.h"
#include "SoundManager.h"

HandlerFunc HandlerTable[PKT_MAX];

static void DefaultHandler( PacketHeader& pktBase )
{
	assert( false );
	GameManager::GetInstance()->Stop();
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
	static void Handler_##PKT_TYPE( PacketHeader& pktBase ); \
	static RegisterHandler _register_##PKT_TYPE( PKT_TYPE, Handler_##PKT_TYPE ); \
	static void Handler_##PKT_TYPE( PacketHeader& pktBase )

REGISTER_HANDLER( PKT_SC_LOGIN )
{
	LoginResult recvData = static_cast<LoginResult&>( pktBase );
	NetworkManager::GetInstance()->HandleLoginResult( recvData );	
}

void NetworkManager::HandleLoginResult( LoginResult& inPacket )
{
	if ( m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size ) )
	{
		// 패킷처리
		if ( inPacket.m_PlayerId == -1 )
		{
			/// 여기 걸리면 로그인 실패다.
			GameManager::GetInstance()->Stop();
		}

		printf_s( "My playerID[%d] \n", inPacket.m_PlayerId );
		NetworkManager::GetInstance()->m_MyPlayerId = inPacket.m_PlayerId;
	}
	else
	{
		assert( false );
	}
}

REGISTER_HANDLER( PKT_SC_GAMEOVER )
{
	GameOverResult recvData = static_cast<GameOverResult&>( pktBase );
	NetworkManager::GetInstance()->HandleGameOverResult( recvData );
}

void NetworkManager::HandleGameOverResult( GameOverResult& inPacket )
{
	if ( m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size ) )
	{
		/// 여기 걸리면 서버로부터 잘못된 아이디가 넘어온것
		assert( inPacket.m_PlayerId != -1 );

		if ( m_MyPlayerId != inPacket.m_PlayerId )
		{
			return;
		}

		if ( inPacket.m_IsWon )
		{
			Log( "I'm WIN!! \n" );
			TextManager::GetInstance( )->RegistText( TEXT_GAME_RESULT, L"I'm WIN!!", 200, 200 );
		}
		else
		{
			Log( "I'm Lose... \n" );
			TextManager::GetInstance( )->RegistText( TEXT_GAME_RESULT, L"I'm Lose... ", 200, 200 );
		}
	}
	else
	{
		assert( false );
	}
}

REGISTER_HANDLER( PKT_SC_CHAT )
{
	ChatBroadcastResult recvData = static_cast<ChatBroadcastResult&>( pktBase );
	NetworkManager::GetInstance()->HandleChatResult( recvData );
}

void NetworkManager::HandleChatResult( ChatBroadcastResult& inPacket )
{
	if ( m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size ) )
	{
		/// 여기 걸리면 로그인 안된놈이 보낸거다
		assert( inPacket.m_PlayerId != -1 );

		char buff[MAX_CHAT_LEN] = { 0, };
		sprintf_s( buff, "CHAT from Player[%s]: %s \n", inPacket.m_Name, inPacket.m_Chat );

		Log( "%s \n", buff );
	}
	else
	{
		assert( false );
	}
}


REGISTER_HANDLER( PKT_SC_GENERATE_CORPS )
{
		GenerateCorpsResult recvData = static_cast<GenerateCorpsResult&>( pktBase );
		NetworkManager::GetInstance( )->HandleGenerateCorpsResult( recvData );
}

void NetworkManager::HandleGenerateCorpsResult( GenerateCorpsResult& inPacket )
{
	if ( m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size ) )
	{
		/// 여기 걸리면 로그인 안된놈이 보낸거다
		assert( inPacket.m_PlayerId != -1 );

		UnitType unitType = inPacket.m_UnitType;

		float nowX = inPacket.m_NowX;
		float nowZ = inPacket.m_NowZ;
		float lookX = inPacket.m_LookX;
		float lookZ = inPacket.m_LookZ;

		PositionInfo position;
		position.m_EyePoint = { nowX, 0.0f, nowZ };
		position.m_LookAtPoint = { lookX, 0.0f, lookZ };

		int corpsID = inPacket.m_CorpsID;
		int playerID = inPacket.m_PlayerId;

		Corps* corps = new Corps( corpsID, playerID, position );
		corps->Create( 10, unitType );
		corps->SetVisible( true );

		Scene* scene =  SceneManager::GetInstance()->GetNowScene();
		if ( typeid( ScenePlay ) == typeid( *scene ) )
		{
			ScenePlay* scenePlay = static_cast<ScenePlay*>( SceneManager::GetInstance()->GetNowScene() );
			scenePlay->AddCorps( corpsID, corps );
			Log( "GenerateCorps! Type:%d CorpID:%d \n", unitType, corpsID );

			SoundManager::GetInstance( )->PlaySound( SOUND_CORPS_GENERATE );
		}
		else
		{
			// 플레이중이 아닌데 플레이용 패킷을 받음 
			assert( false );
		}
	}
	else
	{
		assert( false );
	}
}


REGISTER_HANDLER( PKT_SC_CORPS_CHANGE_FORMATION )
{
	ChangeCorpsFormationResult recvData = static_cast<ChangeCorpsFormationResult&>( pktBase );
	NetworkManager::GetInstance()->HandleChangeCorpsFormationResult( recvData );
}

void NetworkManager::HandleChangeCorpsFormationResult( ChangeCorpsFormationResult& inPacket )
{
	if ( m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size ) )
	{

		int corpsID = inPacket.m_CorpsID;
		FormationType formationType = inPacket.m_FormationType;

		Scene* scene = SceneManager::GetInstance()->GetNowScene();
		if ( typeid( ScenePlay ) == typeid( *scene ) )
		{
			ScenePlay* scenePlay = static_cast<ScenePlay*>( SceneManager::GetInstance()->GetNowScene() );
			scenePlay->ChangeCorpsFormation( corpsID, formationType );
			Log( "ChangeCorpsFormation! CorpID:%d Type:%d  \n", corpsID, formationType );
		}
		else
		{
			// 플레이중이 아닌데 플레이용 패킷을 받음 
			assert( false );
		}
	}
	else
	{
		assert( false );
	}
}



REGISTER_HANDLER( PKT_SC_MOVE_CORPS )
{
	MoveCorpsResult recvData = static_cast<MoveCorpsResult&>( pktBase );
	NetworkManager::GetInstance( )->HandleMoveCorpsResult( recvData );
}

void NetworkManager::HandleMoveCorpsResult( MoveCorpsResult& inPacket )
{
	if ( m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size ) )
	{
		int corpsID = inPacket.m_CorpsID;
		float speed = inPacket.m_Speed;

		float targetX = inPacket.m_TargetX;
		float targetZ = inPacket.m_TargetZ;
		float lookX = inPacket.m_LookX;
		float lookZ = inPacket.m_LookZ;
		
		D3DXVECTOR3 targetPosition = { targetX, 0.0f, targetZ };
		D3DXVECTOR3 lookAtVector = { lookX, 0.0f, lookZ };

		Scene* scene = SceneManager::GetInstance()->GetNowScene();
		if ( typeid( ScenePlay ) == typeid( *scene ) )
		{
			ScenePlay* scenePlay = static_cast<ScenePlay*>( SceneManager::GetInstance()->GetNowScene() );
			scenePlay->MoveCorpsStart( corpsID, targetPosition, lookAtVector, speed );
			Log( "CorpsMoving! CorpID:%d - Move To X:%f Z:%f Speed:%f \n", corpsID, lookAtVector.x, lookAtVector.z, speed );
			SoundManager::GetInstance()->PlaySound( SOUND_CORPS_MOVE );
		}
		else
		{
			// 플레이중이 아닌데 플레이용 패킷을 받음 
			assert( false );
		}
	}
	else
	{
		assert( false );
	}
}


REGISTER_HANDLER( PKT_SC_STOP_CORPS )
{
	StopCorpsResult recvData = static_cast<StopCorpsResult&>( pktBase );
	NetworkManager::GetInstance()->HandleStopCorpsResult( recvData );
}

void NetworkManager::HandleStopCorpsResult( StopCorpsResult& inPacket )
{
	if ( m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size ) )
	{

		int corpsID = inPacket.m_CorpsID;

		Scene* scene = SceneManager::GetInstance()->GetNowScene();
		if ( typeid( ScenePlay ) == typeid( *scene ) )
		{
			ScenePlay* scenePlay = static_cast<ScenePlay*>( SceneManager::GetInstance()->GetNowScene() );
			scenePlay->MoveCorpsStop( corpsID );
			Log( "CorpsStop! CorpID:%d \n", corpsID );
		}
		else
		{
			// 플레이중이 아닌데 플레이용 패킷을 받음 
			assert( false );
		}
	}
	else
	{
		assert( false );
	}
}



REGISTER_HANDLER( PKT_SC_CORPS_ATTACK )
{
	AttackCorpsResult recvData = static_cast<AttackCorpsResult&>( pktBase );
	NetworkManager::GetInstance()->HandleAttackCorpsResult( recvData );
}

void NetworkManager::HandleAttackCorpsResult( AttackCorpsResult& inPacket )
{
	if ( m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size ) )
	{

		int attackingCorpsID = inPacket.m_AttackingCorpsID;
		int targetCorpsID = inPacket.m_TargetCorpsID;
		int unitNum = inPacket.m_TargetUnitNum;

		Scene* scene = SceneManager::GetInstance()->GetNowScene();
		if ( typeid( ScenePlay ) == typeid( *scene ) )
		{
			ScenePlay* scenePlay = static_cast<ScenePlay*>( SceneManager::GetInstance()->GetNowScene() );
			scenePlay->MoveCorpsStop( attackingCorpsID );
			scenePlay->MoveCorpsStop( targetCorpsID );
			scenePlay->SetCorpsHP( targetCorpsID, unitNum );

			UnitType unitType = scenePlay->GetUnitTypeByID( attackingCorpsID );
			switch ( unitType )
			{
				case UnitType::UNIT_ARROW:
					SoundManager::GetInstance()->PlaySound( SOUND_CORPS_ATTACK_ARROW );
					break;
				case UnitType::UNIT_GUARD:
					SoundManager::GetInstance()->PlaySound( SOUND_CORPS_ATTACK_GUARD );
					break;
				case UnitType::UNIT_KNIGHT:
					SoundManager::GetInstance()->PlaySound( SOUND_CORPS_ATTACK_KNIGHT );
					break;
				case UnitType::UNIT_PIKE:
					SoundManager::GetInstance()->PlaySound( SOUND_CORPS_ATTACK_PIKE );
					break;
				case UnitType::UNIT_SWORD:
					SoundManager::GetInstance()->PlaySound( SOUND_CORPS_ATTACK_SWORD );
					break;
				default:
					break;
			}

			Log( "CorpsAttack! [%d]->[%d] \n", attackingCorpsID, targetCorpsID );
		}
		else
		{
			// 플레이중이 아닌데 플레이용 패킷을 받음 
			assert( false );
		}
	}
	else
	{
		assert( false );
	}
}


REGISTER_HANDLER( PKT_SC_REFRESH_UI )
{
	RefreshUIResult recvData = static_cast<RefreshUIResult&>( pktBase );
	NetworkManager::GetInstance( )->HandleRefreshUIResult( recvData );
}

void NetworkManager::HandleRefreshUIResult( RefreshUIResult& inPacket )
{
	if ( m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size ) )
	{

		
		Scene* scene = SceneManager::GetInstance()->GetNowScene();
		if ( typeid( ScenePlay ) == typeid( *scene ) )
		{
			wchar_t wsFood[100] = { 0, };
			wsprintf( wsFood, L"FOOD : %d", inPacket.m_Food );
			wchar_t wsCorpsNum[100] = { 0, };
			wsprintf( wsCorpsNum, L"CorpsNum : %d", inPacket.m_CorpsNum );
			wchar_t wsBaseNum[100] = { 0, };
			wsprintf( wsBaseNum, L"BaseNum : %d", inPacket.m_BaseNum );

			TextManager::GetInstance( )->RegistText( TEXT_FOOD, wsFood, 20, 300 );
			TextManager::GetInstance( )->RegistText( TEXT_CORPS_NUM, wsCorpsNum, 20, 350 );
			TextManager::GetInstance( )->RegistText( TEXT_BASE_NUM, wsBaseNum, 20, 400 );
			Log( "Refresh UI! \n" );
		}
		else
		{
			// 플레이중이 아닌데 플레이용 패킷을 받음 
			assert( false );
		}
	}
	else
	{
		assert( false );
	}
}