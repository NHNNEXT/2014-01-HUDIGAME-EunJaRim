﻿#pragma once
#include "Action.h"
class GuardArea:
	public Action
{
public:
	GuardArea();
	virtual ~GuardArea( );

	void		SetTargetCorps( Corps* targetCrops );
	Corps*		GetTargetCorps() const { return m_TargetCrops; }

private:
	virtual void OnBegin();
	virtual void OnTick();
	virtual void OnEnd();

	void		ReturnMyBase();

private:
	Corps*		m_TargetCrops = nullptr;
	bool		m_GuardModeOn = false;
};

