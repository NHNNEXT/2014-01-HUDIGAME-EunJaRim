﻿#include "stdafx.h"
#include "GuardArea.h"
#include "PacketType.h"
#include "Corps.h"
#include "GameRoom.h"
#include "MacroSet.h"
#include "Attack.h"

GuardArea::GuardArea()
{
}


GuardArea::~GuardArea()
{
}

void GuardArea::SetTargetCorps( Corps* targetCrops )
{
	m_TargerCrops = targetCrops;
	m_GuardModeOn = true;
}

void GuardArea::OnBegin()
{
	Log( "GuardArea OnBegin \n" );
	m_ActionStatus = ACTION_TICK;

	m_OwnerCrops->ReCalculatePosition();
	m_TargerCrops->ReCalculatePosition();

	if ( !m_GuardModeOn )
	{
		m_OwnerCrops->DoNextAction( this, 0 );
		return;
	}


	const PositionInfo& targetPositionInfo = m_TargerCrops->GetPositionInfo();

	D3DXVECTOR2 destination;
	destination.x = targetPositionInfo.m_EyePoint.x;
	destination.y = targetPositionInfo.m_EyePoint.z;
	float length = m_OwnerCrops->GetTargetLength( destination );

	// 공격명령이 바로 앞에서 지시될때와 이동해야할 때를 구분 
	if ( length < m_OwnerCrops->GetAttackRange() )
	{
		m_OwnerCrops->DoNextAction( this, m_OwnerCrops->GetAttackDelay() );
	}
	else
	{
		m_OwnerCrops->DoNextAction( this, 0 );
	}

}

void GuardArea::OnTick()
{
	// 둘중 하나라도 죽으면 어텍 취소
	if ( m_OwnerCrops->IsDead() || m_TargerCrops->IsDead() )
	{
		Log( "Attack Failed \n" );
		m_ActionStatus = ACTION_END;
		m_OwnerCrops->DoNextAction( this, 0 );
		return;
	}

	m_OwnerCrops->MoveStop();
	m_TargerCrops->ReCalculatePosition();
	const PositionInfo& myCorpsPositionInfo = m_OwnerCrops->GetPositionInfo();
	const PositionInfo& targetPositionInfo = m_TargerCrops->GetPositionInfo();

	float nowX = myCorpsPositionInfo.m_EyePoint.x;
	float nowZ = myCorpsPositionInfo.m_EyePoint.z;
	float targetX = targetPositionInfo.m_EyePoint.x;
	float targetZ = targetPositionInfo.m_EyePoint.z;


	D3DXVECTOR2 destination;
	destination.x = targetPositionInfo.m_EyePoint.x;
	destination.y = targetPositionInfo.m_EyePoint.z;
	float length = m_OwnerCrops->GetTargetLength( destination );

	D3DXVECTOR2 vector;
	vector.x = targetX - nowX;
	vector.y = targetZ - nowZ;


	if ( m_GuardModeOn && length < m_OwnerCrops->GetAttackRange( ) )
	{
		m_TargerCrops->MoveStop();
		

		// 공격 하세요
		// attack result packet 보내기
		m_TargerCrops->AddDamage( m_OwnerCrops->GetAttackPower() );

		AttackCorpsResult outPacket;
		outPacket.m_AttackerCorpsID = m_OwnerCrops->GetCorpsID();
		outPacket.m_TargetCorpsID = m_TargerCrops->GetCorpsID();
		outPacket.m_AttackerNowX = myCorpsPositionInfo.m_EyePoint.x;
		outPacket.m_AttackerNowZ = myCorpsPositionInfo.m_EyePoint.z;

		outPacket.m_AttackerLookX = targetPositionInfo.m_EyePoint.x;
		outPacket.m_AttackerLookZ = targetPositionInfo.m_EyePoint.z;

		outPacket.m_TargetNowX = targetPositionInfo.m_EyePoint.x;
		outPacket.m_TargetNowZ = targetPositionInfo.m_EyePoint.z;

		outPacket.m_TargetLookX = myCorpsPositionInfo.m_LookAtPoint.x;
		outPacket.m_TargetLookZ = myCorpsPositionInfo.m_LookAtPoint.z;

		outPacket.m_TargetUnitNum = m_TargerCrops->GetUnitNum();


		m_ClientManager->BroadcastPacket( &outPacket );

		Log( "[GuardArea] length:%f  range:%f damage:%f \n", length, m_OwnerCrops->GetAttackRange(), m_OwnerCrops->GetAttackPower() );
		Log( "[GuardArea] Attacker:[%f][%f] Defencer:[%f][%f] \n", m_OwnerCrops->GetPositionInfo().m_EyePoint.x, m_OwnerCrops->GetPositionInfo().m_EyePoint.z, m_TargerCrops->GetPositionInfo().m_EyePoint.x, m_TargerCrops->GetPositionInfo().m_EyePoint.z );
		Log( "GuardArea OnTick Attack Success \n" );


		if ( m_TargerCrops->IsDead() )
		{
			Log( "Guarder Killed Target! \n" );
			ReturnMyBase();
			return;
		}
		else if ( m_OwnerCrops->IsDead() )
		{
			m_ActionStatus = ACTION_END;
			m_OwnerCrops->DoNextAction( this, 0 );
		}
		else
		{
			Log( "Ready Re Attack!! \n" );
			m_ActionStatus = ACTION_TICK;
			m_OwnerCrops->DoNextAction( this, m_OwnerCrops->GetAttackDelay() );
		}
	}
	else
	{
		// 하드 코딩 쫓아가는것을 포기하는 거리
		if ( length > 25 )
		{
			Log( "Too far to chase \n" );
			ReturnMyBase();
			return;
		}

		// 마저 쫓아 가세요
		D3DXVECTOR2 vector;
		vector.x = targetX - nowX;
		vector.y = targetZ - nowZ;

		float halfRange = m_OwnerCrops->GetAttackRange() / 2;
		if ( vector.x > 0 )
		{
			targetX = targetX - halfRange;
		}
		else
		{
			targetX = targetX + halfRange;
		}
		if ( vector.y > 0 )
		{
			targetZ = targetZ - halfRange;
		}
		else
		{
			targetZ = targetZ + halfRange;
		}

		D3DXVECTOR2 destination;
		destination.x = targetX;
		destination.y = targetZ;
		ULONGLONG movingTime = m_OwnerCrops->MoveStart2( destination, 2 );


		Log( "GuardArea OnTick Chase \n" );
		m_ActionStatus = ACTION_TICK;
		m_OwnerCrops->DoNextAction( this, movingTime );



	}
}

void GuardArea::OnEnd()
{
	Log( "GuardArea OnEnd \n" );
	m_OwnerCrops->MoveStop();
	Action::OnEnd();
}

void GuardArea::ReturnMyBase()
{
	Log( "Return to my original Position! \n" );

	const PositionInfo& targetPositionInfo = m_TargerCrops->GetPositionInfo();


	UnitType unitType = m_OwnerCrops->GetUnitType();
	PositionInfo originalPosition;

	if ( UnitType::UNIT_GUARD == unitType )
	{
		int guardIndex = m_ClientManager->GetGuardIndexByID( m_OwnerCrops->GetCorpsID() );
		originalPosition = m_ClientManager->GetGuardPositionInfo( guardIndex );
	}
	else if ( UnitType::UNIT_KING == unitType )
	{
		int kingIndex = m_ClientManager->GetKingIndexByID( m_OwnerCrops->GetCorpsID() );
		originalPosition = m_ClientManager->GetKingPositionInfo( kingIndex );
	}
	m_GuardModeOn = false;


	D3DXVECTOR2 destination;
	destination.x = originalPosition.m_EyePoint.x + 0.01f;
	destination.y = originalPosition.m_EyePoint.z + 0.01f;
	ULONGLONG movingTime = m_OwnerCrops->MoveStart2( destination );

	Log( "GuardArea OnTick Return to my original Position \n" );
	m_ActionStatus = ACTION_END;
	m_OwnerCrops->DoNextAction( this, movingTime );
}
