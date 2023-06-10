#include "StateMachine.h"

dae::StateMachine::StateMachine(GameObject* pOwner, State::StatePtr&& pEntry)
	: Component(pOwner)
	, m_pCurrent{std::move(pEntry)}
{
	
}

void dae::StateMachine::Awake()
{
	m_pCurrent->OnEnter();
}

void dae::StateMachine::OnDestroy()
{
	m_pCurrent->OnExit();
}

void dae::StateMachine::Update()
{
	State::StatePtr pNext{ std::move(m_pCurrent->OnUpdate()) };
	if (pNext != m_pCurrent && pNext)
	{
		m_pCurrent->OnExit();
		m_pCurrent = std::move(pNext);
		m_pCurrent->OnEnter();
	}
}
