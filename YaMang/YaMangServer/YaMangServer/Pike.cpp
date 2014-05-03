﻿#include "stdafx.h"
#include "Pike.h"

Pike::Pike( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler ):Corps( playerID, corpsID, position, actionScheduler )
{
	// xml로 뺄것
	m_HP = 100.0f;
	m_MoveSpeed = 3.0f;
	m_AttackRange = 3.0f;
	m_AttackPower = 3.0f;
	m_Defense = 3.0f;
}

Pike::~Pike()
{
}
