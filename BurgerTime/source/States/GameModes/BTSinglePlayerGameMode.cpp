#include "BTSinglePlayerGameMode.h"
#include "Prefabs.h"
#include "Managers/SceneManager.h"

dae::BTSinglePlayerGameMode::BTSinglePlayerGameMode(size_t level)
    : BTGameMode(level)
{
}

void dae::BTSinglePlayerGameMode::OnEnter()
{
    BTGameMode::OnEnter();
}

dae::State::StatePtr dae::BTSinglePlayerGameMode::OnUpdate()
{
    return this;
}

void dae::BTSinglePlayerGameMode::OnExit()
{
    BTGameMode::OnExit();
}

void dae::BTSinglePlayerGameMode::StartRound()
{
    RespawnAllActiveCharacters();
}

dae::GameObject* dae::BTSinglePlayerGameMode::CreateHUD()
{
    Scene* pScene{ SceneManager::GetInstance().GetCurrent() };
    auto pPlayers{ pScene->GetGameObjectWithTag("Player")};
    GameObject* pPlayer{ nullptr };
    if (pPlayers.size() > 0)
    {
        pPlayer = pPlayers[0];
    }
    return pPlayer ? Prefabs::CreateSinglePlayerHUD(GetLevelRoot(), pPlayer) : nullptr;
}
