#include "GameState.h"


dae::GameState::GameState()
	: m_pEntryGameMode{std::make_shared<GameMode>()}
{
	m_pGameModeFSM = std::make_unique<StateMachine>(nullptr, m_pEntryGameMode);
}

void dae::GameState::Start()
{
	m_pGameModeFSM->Awake();
	m_Running = true;
}

void dae::GameState::Update()
{
	if (m_Running)
	{
		m_pGameModeFSM->Update();
	}
}

void dae::GameState::Shutdown()
{
	m_pGameModeFSM->Sleep();
	m_Running = false;
}

void dae::GameState::SetGameMode(std::shared_ptr<GameMode>&& pGameState)
{
	if (!pGameState)
		return;

	if (m_Running)
	{
		m_pGameModeFSM->Sleep();
		m_pEntryGameMode = std::move(pGameState);
		m_pGameModeFSM = std::make_unique<StateMachine>(nullptr, m_pEntryGameMode);
		m_pGameModeFSM->Awake();
	}
	else
	{
		m_pEntryGameMode = std::move(pGameState);
		m_pGameModeFSM = std::make_unique<StateMachine>(nullptr, m_pEntryGameMode);
	}
}

dae::GameMode& dae::GameState::GetGameMode() const
{
	return *dynamic_cast<GameMode*>(&m_pGameModeFSM->GetCurrent());
}
