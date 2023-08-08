#include "StateMachine.h"

//dae::StateMachine::StateMachine(GameObject* pOwner, State::StatePtr pEntry)
//	: Component(pOwner)
//	, m_pCurrent{pEntry}
//{
//	
//}

dae::StateMachine::StateMachine(GameObject* pOwner, State::StatePtr pState)
	: Component(pOwner)
	, m_pCurrent{ pState }
{
}

void dae::StateMachine::Awake()
{
	m_pCurrent->OnEnter();
}

void dae::StateMachine::Sleep()
{
	m_pCurrent->OnExit();
}

void dae::StateMachine::Update()
{
	if (m_pCurrent)
	{
		State::StatePtr pNext{ m_pCurrent->OnUpdate() };
		if (pNext && pNext != m_pCurrent)
		{
			m_pCurrent->OnExit();
			m_pCurrent = std::move(pNext);
			m_pCurrent->OnEnter();
		}
	}
}

dae::State& dae::StateMachine::GetCurrent() const
{
	return *m_pCurrent;
}

void dae::StateMachine::SetState(State::StatePtr pState)
{
	if (pState && pState != m_pCurrent)
	{
		if (m_pCurrent)
			m_pCurrent->OnExit();
		m_pCurrent = std::move(pState);
		m_pCurrent->OnEnter();
	}

}
