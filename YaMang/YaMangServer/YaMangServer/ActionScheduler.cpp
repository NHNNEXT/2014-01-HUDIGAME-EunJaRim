#include "stdafx.h"
#include "Action.h"
#include "ActionScheduler.h"
#include "ClientManager.h"
#include "Corps.h"


ActionScheduler::ActionScheduler( ClientManager* clientManager ):
m_ClientManager( clientManager )
{
	m_BeginTime = Clock::now();
}


ActionScheduler::~ActionScheduler()
{
	m_BeginTime = Clock::now();
	m_CurrentTime = GetCurrentTick();
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

		//@author �ŵ���
		//Action�� �� ���¸� Ȯ��
		//���°� �� ���� ���� ���� �¾Ҵٸ� ����
		if ( headAction->Gozarani() )
		{
			delete headAction;
			continue;
		}
		//���°� ������ owner corps id Ž�� �� action ����

		int ownerCorpsID = headAction->GetOwnerCorpsID();
		Corps* corp = m_ClientManager->GetCorpsByCorpsID( ownerCorpsID );
		
		if ( nullptr != corp )
		{
			Action* holdingAction = corp->GetHoldingAction();

			// ó�� �׼��� �޴� �߽��ϰ��
			if ( nullptr != holdingAction )
			{
				holdingAction->LowKick();
			}
			corp->SetHoldingAction( headAction );
		}

	}
}

