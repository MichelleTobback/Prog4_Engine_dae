#include "BTSinglePlayerGameMode.h"
#include "Prefabs.h"
#include "Managers/SceneManager.h"

void dae::BTSinglePlayerGameMode::OnEnter()
{
    OnSceneLoaded();
}

dae::State::StatePtr dae::BTSinglePlayerGameMode::OnUpdate()
{
    return this;
}

void dae::BTSinglePlayerGameMode::OnExit()
{

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
