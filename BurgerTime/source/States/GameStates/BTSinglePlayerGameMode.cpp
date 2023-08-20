#include "BTSinglePlayerGameMode.h"
#include "Prefabs.h"
#include "BurgerTIme.h"
#include "Components/ObjectSpawner.h"

dae::BTSinglePlayerGameMode::BTSinglePlayerGameMode(size_t lvlIdx)
    : BTGameMode(lvlIdx)
{
}

void dae::BTSinglePlayerGameMode::OnEnter(Scene& scene)
{
    const uint32_t spawnId{ static_cast<uint32_t>(dae::BurgerTime::SpawnID::Player) };
    dae::ObjectSpawner::Register(spawnId, std::bind(&BTSinglePlayerGameMode::CreatePlayer, this, std::placeholders::_1));

    BTGameMode::OnEnter(scene);
}

void dae::BTSinglePlayerGameMode::OnExit(Scene& scene)
{
    BTGameMode::OnExit(scene);
}

void dae::BTSinglePlayerGameMode::StartRound()
{
    RespawnAllActiveCharacters();
}

dae::GameObject* dae::BTSinglePlayerGameMode::CreateHUD(Scene& scene)
{
    auto pPlayers{ scene.GetGameObjectWithTag("Player")};
    GameObject* pPlayer{ nullptr };
    if (pPlayers.size() > 0)
    {
        pPlayer = pPlayers[0];
    }
    return pPlayer ? Prefabs::CreateSinglePlayerHUD(GetLevelRoot(), pPlayer) : nullptr;
}

dae::GameObject* dae::BTSinglePlayerGameMode::CreatePlayer(dae::Scene* pScene)
{
    auto pPlayers{ pScene->GetGameObjectWithTag("Player") };
    if (pPlayers.size() == 0)
        return Prefabs::CreatePeterPepper(pScene);

    return nullptr;
}
