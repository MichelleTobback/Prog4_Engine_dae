#include "BTCoopGameMode.h"
#include "Components/ObjectSpawner.h"
#include "BurgerTIme.h"
#include "Prefabs.h"

dae::BTCoopGameMode::BTCoopGameMode(size_t lvlIdx)
    : BTGameMode(lvlIdx)
{
}

void dae::BTCoopGameMode::OnEnter(Scene& scene)
{
    const uint32_t spawnId{ static_cast<uint32_t>(dae::BurgerTime::SpawnID::Player) };
    dae::ObjectSpawner::Register(spawnId, std::bind(&BTCoopGameMode::CreatePlayer, this, std::placeholders::_1));

    BTGameMode::OnEnter(scene);
}

void dae::BTCoopGameMode::OnExit(Scene& scene)
{
    BTGameMode::OnExit(scene);
}

void dae::BTCoopGameMode::StartRound()
{
    RespawnAllActiveCharacters();
}

dae::GameObject* dae::BTCoopGameMode::CreateHUD(Scene&)
{ 
    return nullptr;
}

dae::GameObject* dae::BTCoopGameMode::CreatePlayer(dae::Scene* pScene)
{
    auto pPlayers{ pScene->GetGameObjectWithTag("Player") };
    return Prefabs::CreatePeterPepper(pScene, static_cast<int>(pPlayers.size()) - 1, !pPlayers.size());
}
