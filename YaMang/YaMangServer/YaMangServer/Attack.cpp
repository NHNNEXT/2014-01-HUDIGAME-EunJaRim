﻿#include "stdafx.h"
#include "Attack.h"
#include "PacketType.h"
#include "Corps.h"
#include "GameRoom.h"
#include "MacroSet.h"
Attack::Attack()
{
}


Attack::~Attack()
{
}

void Attack::OnBegin()
{
	Log( "Attack OnBegin \n" );
	m_OwnerCrops->ReCalculatePosition();
	m_TargerCrops->ReCalculatePosition();


	const PositionInfo& targetPositionInfo = m_TargerCrops->GetPositionInfo();

	D3DXVECTOR2 destination;
	destination.x = targetPositionInfo.m_EyePoint.x;
	destination.y = targetPositionInfo.m_EyePoint.z;
	float length = m_OwnerCrops->GetTargetLength( destination );

	m_ActionStatus = ACTION_TICK;
	// 공격명령이 바로 앞에서 지시될때와 이동해야할 때를 구분 
	if ( length < m_OwnerCrops->GetAttackRange() )
	{
		m_OwnerCrops->DoNextAction( this, m_OwnerCrops->GetAttackDelay( ) );
	}
	else
	{
		m_OwnerCrops->DoNextAction( this, 0 );
	}
	
}

void Attack::OnTick()
{

	// 둘중 하나라도 죽으면 어텍 취소
	if ( m_OwnerCrops->IsDead( ) || m_TargerCrops->IsDead( ) )
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
	

	D3DXVECTOR2 destination;
	destination.x = targetPositionInfo.m_EyePoint.x;
	destination.y = targetPositionInfo.m_EyePoint.z;
	float length = m_OwnerCrops->GetTargetLength( destination );

	if ( length < m_OwnerCrops->GetAttackRange() )
	{
		
		m_OwnerCrops->AttackCorps( m_TargerCrops );

		Log( "Attack OnTick Attack Success \n" );


		if ( m_OwnerCrops->IsDead() || m_TargerCrops->IsDead() )
		{
			Log( "Dead! \n" );
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
		// 마저 쫓아 가세요

		float nowX = myCorpsPositionInfo.m_EyePoint.x;
		float nowZ = myCorpsPositionInfo.m_EyePoint.z;
		float targetX = targetPositionInfo.m_EyePoint.x;
		float targetZ = targetPositionInfo.m_EyePoint.z;
		
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
		ULONGLONG movingTime = m_OwnerCrops->MoveStart( destination, 2 );


		Log( "Attack OnTick Chase \n" );
		m_ActionStatus = ACTION_TICK;
		m_OwnerCrops->DoNextAction( this, movingTime );
	}
}

void Attack::OnEnd()
{
	Log( "Attack OnEnd \n" );
	m_OwnerCrops->MoveStop( );
	Action::OnEnd();
}
