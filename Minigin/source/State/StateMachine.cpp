#include "StateMachine.h"

dae::StateMachine::StateMachine(GameObject* pOwner, State::StatePtr pEntry)
	: Component(pOwner)
	, m_pCurrent{pEntry}
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
	State::StatePtr pNext{ m_pCurrent->OnUpdate() };
	if (pNext != m_pCurrent && pNext)
	{
		m_pCurrent->OnExit();
		m_pCurrent = std::move(pNext);
		m_pCurrent->OnEnter();
	}
}

dae::State& dae::StateMachine::GetCurrent() const
{
	return *m_pCurrent;
}
