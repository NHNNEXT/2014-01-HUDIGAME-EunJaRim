#include "stdafx.h"
#include "ActionScheduler.h"
#include "Action.h"



ActionScheduler::ActionScheduler()
{
	
}


ActionScheduler::~ActionScheduler()
{
	m_BeginTime = Clock::now();
	m_CurrentTime = /*ActionScheduler::*/GetCurrentTick();
}

int64_t ActionScheduler::GetCurrentTick()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>( Clock::now() - m_BeginTime ).count();
}

void ActionScheduler::AddActionToScheduler( Action* addedAction, int64_t remainTime )
{
	int64_t workingTime = remainTime + m_CurrentTime;
	addedAction->SetTime( workingTime );
	m_ActionQueue.push( addedAction );
}

void ActionScheduler::DoScheduledAction()
{
	m_CurrentTime = GetCurrentTick();

	while ( !m_ActionQueue.empty() )
	{
		Action* headAction = m_ActionQueue.top();

		if ( m_CurrentTime < (headAction->GetTime()) )
		{
			break;
		}

		//Action�� �� ���¸� Ȯ��
		//���°� �� ���� ���� ���� �¾Ҵٸ� ����

		//���°� ������ owner corps id Ž�� �� action ����
	}
}

