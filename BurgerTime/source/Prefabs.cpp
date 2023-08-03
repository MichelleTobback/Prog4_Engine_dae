#include "Prefabs.h"
#include "Component/Physics/CharacterController2D.h"
#include "Components/PlayerController.h"
#include "Components/BurgerTimeMovementController.h"
#include "Components/BurgerPlate.h"
#include "Components/EnemyComponent.h"
#include "States/Enemy/EnemyGoToPlayerState.h"
#include "States/Enemy/EnemyClimbLadderState.h"
#include "Components/HealthComponent.h"

#include "BurgerTime.h"
#include "Component/SpriteAnimatorComponent.h"
#include "Component/SpriteAtlasComponent.h"
#include "Component/RenderComponent.h"
#include "State/StateMachine.h"

#include "State/GameState/GameState.h"
#include "States/GameModes/BTGameMode.h"
#include "Managers/ResourceManager.h"
#include "Component/TextComponent.h"

dae::GameObject* dae::Prefabs::CreateScoreHUD(GameObject* pHudObject, ObservableType<uint32_t>& value, const std::string& text, const glm::vec4& color)
{
    //text
    GameObject* pObject{ pHudObject->GetScene()->Instantiate(pHudObject) };
    pObject->AddComponent<dae::TextureRenderComponent>(pObject->AddComponent<dae::TextureComponent>());
    auto font{ dae::ResourceManager::GetInstance().LoadFont("Fonts/super-burger-time.ttf", 8) };
    pObject->AddComponent<TextComponent>(text, font, color);

    //actual score text
    GameObject* pScoreObject{ pObject->GetScene()->Instantiate(pObject) };
    pScoreObject->GetTransform().Translate(glm::vec3{ 0.f, 8.f, 0.f });
    pScoreObject->AddComponent<dae::TextureRenderComponent>(pScoreObject->AddComponent<dae::TextureComponent>());
    TextComponent* pScoreText{ pScoreObject->AddComponent<TextComponent>(std::to_string(value()), font, glm::vec4{0.89f, 0.89f, 0.89f, 1.f})};

    value.GetOnValueChangedDelegate() += [pScoreText](uint32_t score)
    {
        pScoreText->SetText(std::to_string(score));
    };

    return pObject;
}

dae::GameObject* dae::Prefabs::CreatePlayerLifesHUD(SpriteAtlasComponent* pSpriteAtlas, HealthComponent* pPlayerHealth)
{
    GameObject* pObject{ pSpriteAtlas->GetOwner()->GetScene()->Instantiate(pSpriteAtlas->GetOwner())};
    const float tileSize{ 8.f };
    const uint32_t maxLifes{ pPlayerHealth->GetMaxValue() };
    glm::vec3 lifePos{};
    uint32_t spritedLifeID{ pSpriteAtlas->AddSprite(tileSize * 25.f, 0.f, tileSize, tileSize) };
    for (uint32_t i{}; i < maxLifes; ++i)
    {
        GameObject* pSpriteObject{ pObject->GetScene()->Instantiate(pObject, lifePos) };
        pSpriteObject->AddComponent<SpriteRenderComponent>(pSpriteAtlas->GetSprite(spritedLifeID));

        pPlayerHealth->GetOnHealthChanged() += [pSpriteObject, i](uint32_t health)
        {
            pSpriteObject->SetActive(health > i);
        };

        lifePos.y -= tileSize;
    }
    return pObject;
}

dae::GameObject* dae::Prefabs::CreateSinglePlayerHUD(GameObject* pLevelObject, GameObject* pPlayer)
{
    GameObject* pObject{ pLevelObject->GetScene()->Instantiate(0u, pLevelObject) };

    const float tileSize{ 8.f };
    TextureComponent* pTexture{ pObject->AddComponent<TextureComponent>() };
    pTexture->SetTexture("Textures/BurgerTimeCharacters.png");
    SpriteAtlasComponent* pSpriteAtlas{ pObject->AddComponent<SpriteAtlasComponent>(pTexture) };

    PlayerController* pController{ pPlayer->GetComponent<PlayerController>() };

    CreatePlayerLifesHUD(pSpriteAtlas, pPlayer->GetComponent<HealthComponent>())->GetTransform().SetLocalPosition(glm::vec3{ -tileSize, 200.f, 0.f });

    BTGameMode* pGameMode{ dynamic_cast<BTGameMode*>(&GameState::GetInstance().GetGameMode()) };

    GameObject* score{ CreateScoreHUD(pObject, pGameMode->GetScore(), "1up", glm::vec4{ 1.f, 0.06f, 0.06f, 1.f }) };
    score->GetTransform().SetLocalPosition(glm::vec3{0.f, -24.f, 0.f});

    GameObject* hiscore{ CreateScoreHUD(pObject, pGameMode->GetScore(), "hi-score", glm::vec4{ 1.f, 0.06f, 0.06f, 1.f }) };
    hiscore->GetTransform().SetLocalPosition(glm::vec3{ 50.f, -24.f, 0.f });

    GameObject* peppersleft{ CreateScoreHUD(pObject, pController->GetPeppers(), "pepper", glm::vec4{0.06f, 1.f, 0.06f, 1.f})};
    peppersleft->GetTransform().SetLocalPosition(glm::vec3{ 150.f, -24.f, 0.f });

    return pObject;
}

dae::GameObject* dae::Prefabs::CreateEnemy(GameObject* pObject)
{
    const float tileSize{ 8.f };

    CharacterInfoComponent* pInfo{ pObject->GetComponent<CharacterInfoComponent>() };
    pInfo->Get().pStates.push_back(std::make_unique<EnemyGoToPlayerState>(pInfo));
    pInfo->Get().pStates.push_back(std::make_unique<EnemyClimbLadderState>(pInfo));
    pObject->AddComponent<StateMachine>(pInfo->Get().pStates[0].get());
    const uint32_t reward{ 100 };
    pObject->AddComponent<EnemyComponent>(reward);

    RigidBody2DComponent* pRigidBody{ pObject->GetComponent<RigidBody2DComponent>() };
    auto& pColliders{ pRigidBody->GetColliders() };
    for (auto& pCollider : pColliders)
    {
        pCollider->SetCollisionLayer(BurgerTime::ENEMY_COLLISION_LAYER);
        pCollider->SetCollisionIgnoreLayer(BurgerTime::ENEMY_COLLISION_LAYER, true);
    }

    BoxCollider2DComponent* pTrigger{ pObject->GetScene()->Instantiate(pObject)->AddComponent<BoxCollider2DComponent>() };
    pTrigger->GetOwner()->GetTransform().Translate(glm::vec3{ 0.f, -tileSize , 0.f });
    pTrigger->SetShape(pTrigger->GetOwner()->AddComponent<QuadComponent>(glm::vec2{ tileSize * 2.f, tileSize * 2.f }));
    pTrigger->SetCollisionLayer(BurgerTime::ENEMY_COLLISION_LAYER);
    pTrigger->SetCollisionIgnoreLayer(BurgerTime::ENEMY_COLLISION_LAYER, true);
    pTrigger->SetCollisionIgnoreLayer(BurgerTime::LEVEL_COLLISION_LAYER, true);
    pTrigger->SetCollisionIgnoreLayer(BurgerTime::PLATE_COLLISION_LAYER, true);
    pTrigger->SetTrigger(true);
    pRigidBody->AddCollider(pTrigger);

    return pObject;
}

dae::GameObject* dae::Prefabs::CreateMrHotDog(Scene* pScene)
{
    GameObject* pObject{ pScene->Instantiate() };
    const float tileSize{ 8.f };

    CharacterController2D::CharacterController2DDesc controllerDesc{};
    controllerDesc.movementSpeed = 25.f;
    CharacterController2D* pCharacterController{ pObject->AddComponent<CharacterController2D>(controllerDesc) };
    pCharacterController->SetRigidBody(pObject->AddComponent<RigidBody2DComponent>());
    BoxCollider2DComponent* pCollider{ pObject->AddComponent<BoxCollider2DComponent>() };
    pCollider->SetShape(pObject->AddComponent<QuadComponent>(glm::vec2{ tileSize, tileSize }));
    pCollider->SetTrigger(false);
    pCharacterController->SetCollider(pCollider);

    //sprites
    const float spriteSize{ tileSize * 2.f };
    TextureComponent* pTexture{ pObject->AddComponent<TextureComponent>() };
    pTexture->SetTexture("Textures/BurgerTimeCharacters.png");
    SpriteAtlasComponent* pSpriteAtlas{ pObject->AddComponent<SpriteAtlasComponent>(pTexture) };
    GameObject* pSpriteObject{ pObject->GetScene()->Instantiate(pObject, glm::vec3{0.f, -tileSize, 0.f}) };
    SpriteRenderComponent* pRenderer{ pSpriteObject->AddComponent<SpriteRenderComponent>() };
    pRenderer->SetLayer(3);
    std::vector<SpriteAnimFrame> walkFrames{};
    const int cols{ 2 };
    for (int y{}; y < cols; ++y)
    {
        uint32_t spritedID{ pSpriteAtlas->AddSprite(y * spriteSize, 2.f * spriteSize, spriteSize, spriteSize) };
        walkFrames.push_back(SpriteAnimFrame{ pSpriteAtlas->GetSprite(spritedID) });
    }

    SpriteAnimatorComponent* pAnimator{ pObject->AddComponent<SpriteAnimatorComponent>() };
    pAnimator->SetRendererComponent(pRenderer);
    pAnimator->AddAnimClip(walkFrames, 1.f);
    pAnimator->PlayClip(0);

    pObject->AddComponent<HealthComponent>(1);

    pObject->AddComponent<CharacterInfoComponent>();
    return pObject;
}

dae::GameObject* dae::Prefabs::CreateBurgerPlate(Scene* pScene, RigidBody2DComponent* pRigidBody)
{
    GameObject* pObject{nullptr};
    if (!pRigidBody)
    {
        pObject = pScene->Instantiate();
        pRigidBody = pObject->AddComponent<RigidBody2DComponent>(CollisionMode::Static);
    }
    else
        pObject = pScene->Instantiate(pRigidBody->GetOwner());
    const glm::vec2 size{38.f, 38.f};
    BoxCollider2DComponent* pCollider{ pObject->AddComponent<BoxCollider2DComponent>() };
    pCollider->SetShape(pObject->AddComponent<QuadComponent>(size));
    pCollider->SetCollisionLayer(BurgerTime::PLATE_COLLISION_LAYER);
    pCollider->SetCollisionIgnoreLayer(BurgerTime::PLAYER_COLLISION_LAYER, true);
    pCollider->SetTrigger(true);
    pRigidBody->AddCollider(pCollider);
    pObject->AddComponent<BurgerPlate>(pRigidBody);
    return pObject;
}

dae::GameObject* dae::Prefabs::CreateBurgerIngredient(Scene* pScene, BurgerIngredient::IngredientType type)
{
    GameObject* pObject{ pScene->Instantiate() };
    RigidBody2DComponent* pRigidBody{ pObject->AddComponent<RigidBody2DComponent>() };
    TextureComponent* pTextureComponent{ pObject->AddComponent<TextureComponent>() };
    pTextureComponent->SetTexture("Textures/BurgerTimeCharacters.png");
    SpriteAtlasComponent* pSpriteAtlas{ pObject->AddComponent<SpriteAtlasComponent>(pTextureComponent)};

    const uint32_t reward{ 50 };
    pObject->AddComponent<BurgerIngredient>(type, pSpriteAtlas, pRigidBody, reward);

    return pObject;
}

dae::GameObject* dae::Prefabs::CreatePeterPepper(Scene* pScene)
{
    GameObject* pObject{ pScene->Instantiate() };
    const float tileSize{ 8.f };

    CharacterController2D::CharacterController2DDesc controllerDesc{};
    controllerDesc.movementSpeed = 25.f;
    CharacterController2D* pCharacterController{ pObject->AddComponent<CharacterController2D>(controllerDesc) };
    pCharacterController->SetRigidBody(pObject->AddComponent<RigidBody2DComponent>());
    BoxCollider2DComponent* pCollider{ pObject->AddComponent<BoxCollider2DComponent>() };
    pCollider->SetShape(pObject->AddComponent<QuadComponent>(glm::vec2{ tileSize, tileSize }));
    pCollider->SetCollisionLayer(BurgerTime::PLAYER_COLLISION_LAYER);
    pCollider->SetTrigger(false);
    pCharacterController->SetCollider(pCollider);

    pObject->AddComponent<PlayerController>(pCharacterController);

    //sprites
    const float spriteSize{ tileSize * 2.f };
    TextureComponent* pTexture{ pObject->AddComponent<TextureComponent>() };
    pTexture->SetTexture("Textures/BurgerTimeCharacters.png");
    SpriteAtlasComponent* pSpriteAtlas{ pObject->AddComponent<SpriteAtlasComponent>(pTexture) };
    uint32_t spritedID{ pSpriteAtlas->AddSprite(0.f, 0.f, spriteSize, spriteSize) };
    GameObject* pSpriteObject{ pObject->GetScene()->Instantiate(pObject, glm::vec3{0.f, -tileSize, 0.f}) };
    SpriteRenderComponent* pRenderer{pSpriteObject->AddComponent<SpriteRenderComponent>(pSpriteAtlas->GetSprite(spritedID))};
    pRenderer->SetLayer(3);
    std::vector<SpriteAnimFrame> walkFrames{};
    const int cols{ 3 };
    for (int y{}; y < cols; ++y)
    {
        spritedID = pSpriteAtlas->AddSprite(y * spriteSize, 0.f, spriteSize, spriteSize);
        walkFrames.push_back(SpriteAnimFrame{ pSpriteAtlas->GetSprite(spritedID) });
        //for (int x{}; x < rows; ++x)
        //{
        //
        //}
    }

    SpriteAnimatorComponent* pAnimator{ pObject->AddComponent<SpriteAnimatorComponent>() };
    pAnimator->SetRendererComponent(pRenderer);
    pAnimator->AddAnimClip(walkFrames, 1.f);
    pAnimator->PlayClip(0);

    pObject->AddComponent<HealthComponent>(3);
    pObject->AddComponent<CharacterInfoComponent>();

    pObject->AddTag("Player");

    return pObject;
}

dae::GameObject* dae::Prefabs::CreateTestLevel(Scene* pScene)
{
    GameObject* pObject{ pScene->Instantiate(nullptr, glm::vec3{8.f, 0.f, 0.f})};

    TextureComponent* pTexture{ pObject->AddComponent<TextureComponent>() };
    pTexture->SetTexture("Textures/BurgerTimeStages.png");
    SpriteComponent* pSprite{ pObject->AddComponent<SpriteComponent>(pTexture, 0.f, 0.f, 208.f, 200.f) };
    pObject->AddComponent<SpriteRenderComponent>(pSprite);

    return pObject;
}
