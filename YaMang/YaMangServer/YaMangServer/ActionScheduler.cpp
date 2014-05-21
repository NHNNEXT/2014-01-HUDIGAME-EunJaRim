﻿#include "stdafx.h"
#include "Action.h"
#include "ActionScheduler.h"
#include "GameRoom.h"
#include "Corps.h"
#include "MacroSet.h"


ActionScheduler::ActionScheduler( GameRoom* clientManager ):
m_ClientManager( clientManager )
{
	m_BeginTime = GetTickCount64( );
}


ActionScheduler::~ActionScheduler()
{
	m_BeginTime = GetTickCount64( );
	m_CurrentTime = GetTickCount64( );
}

ULONGLONG ActionScheduler::GetCurrentTick( ) const
{
	return GetTickCount64( ) - m_BeginTime;
}

void ActionScheduler::AddActionToScheduler( Action* addedAction, ULONGLONG remainTime )
{
	ULONGLONG workingTime = remainTime + GetTickCount64();
	addedAction->SetTime( workingTime );
	m_ActionQueue.push( addedAction );
}

void ActionScheduler::DoScheduledAction()
{
	m_CurrentTime = GetTickCount64();

	while ( !m_ActionQueue.empty() )
	{
		Action* headAction = m_ActionQueue.top();

		if ( m_CurrentTime < (headAction->GetTime()) )
		{
			break;
		}

		// 일단 위의 검증이 끝났으므로, 우선순위 큐에서 뽑는다.
		m_ActionQueue.pop();

		// @author 신동찬
		// Action을 뜯어서 상태를 확인
		// 상태가 영 좋지 않은 곳에 맞았다면 제거
		if ( headAction->Gozarani( )  )
		{
			delete headAction;
			continue;
		}
		if ( ACTION_END == headAction->GetActionStatus( ) )
		{
			headAction->DoAction();
			delete headAction;
			continue;
		}
		
		// 상태가 좋으면 owner corps id 탐색 후 action 지정

		Corps* corp = headAction->GetOwnerCorps( );
		
		if ( nullptr != corp ) ///# 널체크 수준은 그냥 if ( corp ) 이렇게.. 
		{
			Action* holdingAction = corp->GetHoldingAction();

			// 처음 액션을 받는 콥스가 아닐 경우
			if ( nullptr != holdingAction && headAction != holdingAction)
			{
				holdingAction->LowKick(); ///# lowkick은 OnEnd에서 자동적으로 불리도록 하면 되는데.. 이렇게 강제로 불러주는 이유가 있다면 구조적으로 좋지 못함.
			}
			corp->SetHoldingAction( headAction );

			headAction->DoAction();
		}
		else
		{
			// 콥스 없이 움직이는 액션들 generateCorps등등...
			headAction->DoAction( );
		}

	}
}

