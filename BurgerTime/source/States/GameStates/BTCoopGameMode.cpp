#include "BTCoopGameMode.h"
#include "Components/ObjectSpawner.h"
#include "BurgerTIme.h"
#include "Prefabs.h"
#include "Components/PlayerController.h"
#include "GameManager.h"
#include "Managers/ServiceLocator.h"
#include "Input/Input.h"

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

dae::GameObject* dae::BTCoopGameMode::CreateHUD(Scene& scene)
{ 
    auto pPlayers{ scene.GetGameObjectWithTag("Player") };
    if (pPlayers.size() != 2)
        return nullptr;

    GameObject* pObject{ scene.Instantiate(0u, nullptr) };

    const float tileSize{ 8.f };
    TextureComponent* pTexture{ pObject->AddComponent<TextureComponent>() };
    pTexture->SetTexture("Textures/BurgerTimeCharacters.png");
    SpriteAtlasComponent* pSpriteAtlas{ pObject->AddComponent<SpriteAtlasComponent>(pTexture) };

    Window& window{ ServiceLocator::GetWindow() };

    Prefabs::CreatePlayerLifesHUD(pSpriteAtlas, pPlayers[0]->GetComponent<HealthComponent>())->GetTransform().SetLocalPosition(glm::vec3{-tileSize, 200.f, 0.f});
    Prefabs::CreatePlayerLifesHUD(pSpriteAtlas, pPlayers[1]->GetComponent<HealthComponent>())->GetTransform().SetLocalPosition(glm::vec3{ window.GetWidth() * 0.5f - tileSize * 3.f, 200.f, 0.f});

    GameObject* score{ Prefabs::CreateScoreHUD(pObject, GetScore(), "1up", glm::vec4{ 1.f, 0.06f, 0.06f, 1.f }) };
    score->GetTransform().SetLocalPosition(glm::vec3{ 0.f, -24.f, 0.f });

    GameManager& gameManager{ GameManager::GetInstance() };
    GameObject* hiscore{ Prefabs::CreateScoreHUD(pObject, gameManager.GetHiScore(), "hi-score", glm::vec4{1.f, 0.06f, 0.06f, 1.f}) };
    hiscore->GetTransform().SetLocalPosition(glm::vec3{ 50.f, -24.f, 0.f });

    PlayerController* pController0{ pPlayers[0]->GetComponent<PlayerController>()};
    PlayerController* pController1{ pPlayers[1]->GetComponent<PlayerController>()};
    GameObject* peppersleft{ Prefabs::CreateScoreHUD(pObject, pController1->GetPeppers(), "p1", glm::vec4{0.06f, 1.f, 0.06f, 1.f}) };
    peppersleft->GetTransform().SetLocalPosition(glm::vec3{ 150.f, -24.f, 0.f });

    peppersleft = Prefabs::CreateScoreHUD(pObject, pController0->GetPeppers(), "p2", glm::vec4{0.06f, 1.f, 0.06f, 1.f});
    peppersleft->GetTransform().SetLocalPosition(glm::vec3{ 182.f, -24.f, 0.f });

    return pObject;
}

dae::GameObject* dae::BTCoopGameMode::CreatePlayer(dae::Scene* pScene)
{
    auto pPlayers{ pScene->GetGameObjectWithTag("Player") };
    bool isSecondController{ Input::GetInstance().GetController(1)->IsConnected() };
    return Prefabs::CreatePeterPepper(pScene, static_cast<int>(pPlayers.size()) - 1 + isSecondController, !pPlayers.size());
}
