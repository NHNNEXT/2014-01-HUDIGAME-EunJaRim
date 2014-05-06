﻿#include "stdafx.h"
#include "Sword.h"


Sword::Sword( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler ):Corps( playerID, corpsID, position, actionScheduler )
{
	// xml로 뺄것
	m_HP = 100.0f;
	m_MoveSpeed = 3.0f;
	m_AttackRange = 3.0f;
	m_AttackPower = 8.0f;
	m_Defense = 5.0f;
	m_AttackDelay = 1000;
}

Sword::~Sword()
{
}