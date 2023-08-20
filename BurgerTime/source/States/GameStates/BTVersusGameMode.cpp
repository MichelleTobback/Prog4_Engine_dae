#include "BTVersusGameMode.h"
#include "Prefabs.h"
#include "Components/ObjectSpawner.h"
#include "BurgerTIme.h"

dae::BTVersusGameMode::BTVersusGameMode(size_t lvlIdx)
    : BTGameMode(lvlIdx)
{
}

void dae::BTVersusGameMode::OnEnter(Scene& scene)
{
    const uint32_t spawnId{ static_cast<uint32_t>(dae::BurgerTime::SpawnID::Player) };
    dae::ObjectSpawner::Register(spawnId, std::bind(&BTVersusGameMode::CreatePlayer, this, std::placeholders::_1));

    BTGameMode::OnEnter(scene);
}

void dae::BTVersusGameMode::OnExit(Scene& scene)
{
    BTGameMode::OnExit(scene);
}

void dae::BTVersusGameMode::StartRound()
{
    RespawnAllActiveCharacters();
}

dae::GameObject* dae::BTVersusGameMode::CreateHUD(Scene&)
{
    return nullptr;
}

dae::GameObject* dae::BTVersusGameMode::CreatePlayer(dae::Scene* pScene)
{
    auto pPlayers{ pScene->GetGameObjectWithTag("Player") };
    if (!pPlayers.size())
        return Prefabs::CreatePeterPepper(pScene, static_cast<int>(pPlayers.size()) - 1, !pPlayers.size());

    return Prefabs::CreateControlledEnemy(Prefabs::CreateMrHotDog(pScene));
}
