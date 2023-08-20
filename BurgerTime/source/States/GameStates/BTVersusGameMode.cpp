#include "BTVersusGameMode.h"
#include "Prefabs.h"
#include "Components/ObjectSpawner.h"
#include "BurgerTIme.h"
#include "GameManager.h"
#include "Components/PlayerController.h"
#include "Input/Input.h"

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

dae::GameObject* dae::BTVersusGameMode::CreateHUD(Scene& scene)
{
    auto pPlayers{ scene.GetGameObjectWithTag("Player") };
    GameObject* pPlayer{ nullptr };
    if (pPlayers.size() > 0)
    {
        pPlayer = pPlayers[0];
    }
    if (!pPlayer)
        return nullptr;

    GameObject* pObject{ scene.Instantiate(0u, nullptr) };

    const float tileSize{ 8.f };
    TextureComponent* pTexture{ pObject->AddComponent<TextureComponent>() };
    pTexture->SetTexture("Textures/BurgerTimeCharacters.png");
    SpriteAtlasComponent* pSpriteAtlas{ pObject->AddComponent<SpriteAtlasComponent>(pTexture) };

    Prefabs::CreatePlayerLifesHUD(pSpriteAtlas, pPlayer->GetComponent<HealthComponent>())->GetTransform().SetLocalPosition(glm::vec3{ -tileSize, 200.f, 0.f });

    GameObject* score{ Prefabs::CreateScoreHUD(pObject, GetScore(), "1up", glm::vec4{ 1.f, 0.06f, 0.06f, 1.f }) };
    score->GetTransform().SetLocalPosition(glm::vec3{ 0.f, -24.f, 0.f });

    GameManager& gameManager{ GameManager::GetInstance() };
    GameObject* hiscore{ Prefabs::CreateScoreHUD(pObject, gameManager.GetHiScore(), "hi-score", glm::vec4{1.f, 0.06f, 0.06f, 1.f}) };
    hiscore->GetTransform().SetLocalPosition(glm::vec3{ 50.f, -24.f, 0.f });

    PlayerController* pController{ pPlayer->GetComponent<PlayerController>() };
    GameObject* peppersleft{ Prefabs::CreateScoreHUD(pObject, pController->GetPeppers(), "pepper", glm::vec4{0.06f, 1.f, 0.06f, 1.f}) };
    peppersleft->GetTransform().SetLocalPosition(glm::vec3{ 150.f, -24.f, 0.f });

    return pObject;
}

dae::GameObject* dae::BTVersusGameMode::CreatePlayer(dae::Scene* pScene)
{
    auto pPlayers{ pScene->GetGameObjectWithTag("Player") };
    bool isSecondController{ Input::GetInstance().GetController(1)->IsConnected() };

    if (!pPlayers.size())
        return Prefabs::CreatePeterPepper(pScene, static_cast<int>(pPlayers.size()) - 1 + isSecondController, !pPlayers.size());

    return Prefabs::CreateControlledEnemy(Prefabs::CreateMrHotDog(pScene));
}
