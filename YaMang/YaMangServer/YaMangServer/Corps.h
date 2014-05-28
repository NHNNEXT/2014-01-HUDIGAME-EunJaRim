﻿#pragma once
#include "SharedDefine.h"
#include "EnumSet.h"

class Action;
class GameRoom;
class Corps
{
public:
	Corps( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* gameRoom );
	virtual ~Corps();

	int						GetCorpsID( ) const { return m_CorpsID; }
	int						GetPlayerID() const { return m_PlayerID; }
	UnitType				GetUnitType() const { return m_UnitType; }
	int						GetUnitNum() const { return m_UnitNum; }
	const PositionInfo&		GetPositionInfo( ) const { return m_Position; }
	void					SetPositionInfo( PositionInfo position ) { m_Position = position; };

	void					SetHoldingAction( Action* holdAction ) { m_HoldAction = holdAction; }
	Action*					GetHoldingAction( ) const { return m_HoldAction; }

	void					ChangeFormation( FormationType formation );
	FormationType			GetFormationType() const { return m_Formation; }

	void					AddDamage( float damage );

	float					GetHP() const { return m_HP; }
	void					SetHP( float HP );
	float					GetSpeed( ) const;
	float					GetAttackRange( ) const;
	float					GetAttackPower( ) const;
	float					GetDefense( ) const;
	ULONGLONG				GetAttackDelay() const;
	bool					IsDead() const { return m_IsDead; }

	void					DoNextAction( Action* addedAction, ULONGLONG remainTime );

	float					GetTargetLength( D3DXVECTOR2 destination );
	ULONGLONG				MoveStart( D3DXVECTOR2 destination, int divideMove = 1 );
	void					MoveStop();
	void					AttackCorps( Corps* targetCrops );

	void					ReCalculatePosition();
	void					ReCalculateStatus();

protected:
	void					CalculateHP();


private:
	Corps();
	Corps( const Corps& );
	Corps& operator=( const Corps& );

protected:

	int							m_PlayerID = -1;
	int							m_CorpsID = -1;
	UnitType					m_UnitType = UnitType::UNIT_NONE;
	bool						m_IsDead = false;
	int							m_UnitNum = -1;

	PositionInfo				m_Position;
	FormationType				m_Formation = FormationType::FORMATION_RUSH;

	Action*						m_HoldAction = nullptr;

	float						m_HP = 0.0f;
	float						m_MoveSpeed = 0.0f;
	float						m_MoveSpeedFormationBonus = 0.0f;
	float						m_MoveSpeedMapBonus = 0.0f;

	float						m_AttackRange = 0.0f;
	float						m_AttackRangeFormationBonus = 0.0f;
	float						m_AttackRangeMapBonus = 0.0f;

	float						m_AttackPower = 0.0f;
	float						m_AttackPowerFormationBonus = 0.0f;
	float						m_AttackPowerMapBonus = 0.0f;

	float						m_Defense = 0.0f;
	float						m_DefenseFormationBonus = 0.0f;
	float						m_DefenseMapBonus = 0.0f;

	ULONGLONG					m_AttackDelay = 0;
	ULONGLONG					m_AttackDelayFormationBonus = 0;
	ULONGLONG					m_AttackDelayMapBonus = 0;

	bool						m_IsMoving = false;
	ULONGLONG					m_MovingStartedTime = 0;
	ULONGLONG					m_MovingDuringTime = 0;
	PositionInfo				m_MovingRoute;


	std::vector<D3DXVECTOR2>	m_MovingAllRoute;

private:
	GameRoom*			m_GameRoom = nullptr;
};

