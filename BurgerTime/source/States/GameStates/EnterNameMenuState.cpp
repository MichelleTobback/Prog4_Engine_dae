#include "EnterNameMenuState.h"
#include "Component/UI/ButtonComponent.h"
#include "Component/TextComponent.h"
#include "Component/UI/TextInputComponent.h"
#include "GameManager.h"

dae::EnterNameMenuState::EnterNameMenuState(const std::shared_ptr<BTGameMode>& pGameMode)
	: GameState(1), m_pSelectedGameMode{pGameMode}
{
}

void dae::EnterNameMenuState::OnEnter(Scene& scene)
{
	scene.GetGameObjectWithTag("en")[0]->SetActive(true);
	ButtonComponent* pButton{ scene.GetGameObjectWithTag("NButton")[0]->GetComponent<ButtonComponent>() };
	pButton->GetOnReleasedDelegate() += std::bind(&EnterNameMenuState::StartGame, this);
}

void dae::EnterNameMenuState::OnExit(Scene& scene)
{
	GameObject* pCanvas{ scene.GetGameObjectWithTag("en")[0] };
	pCanvas->SetActive(false);
	ButtonComponent* pButton{ scene.GetGameObjectWithTag("NButton")[0]->GetComponent<ButtonComponent>() };
	pButton->GetOnReleasedDelegate().Clear();

	for (auto pChild : pCanvas->GetChildren())
	{
		if (pChild->HasComponent<TextInputComponent>())
		{
			GameManager& gameManager{ GameManager::GetInstance() };
			const std::string& name{ pChild->GetComponent<TextComponent>()->GetText() };
			gameManager.SetPlayerName(name);
			break;
		}
	}
}

void dae::EnterNameMenuState::StartGame()
{
	GameManager& gameManager{ GameManager::GetInstance() };
	gameManager.PushState(std::move(m_pSelectedGameMode));
}
