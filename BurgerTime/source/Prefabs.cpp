#include "Prefabs.h"
#include "Component/Physics/CharacterController2D.h"
#include "Components/PlayerController.h"
#include "Components/BurgerTimeMovementController.h"
#include "Components/BurgerPlate.h"
#include "Components/EnemyComponent.h"
#include "States/SpriteAnim/CharacterAnimStates.h"
#include "Components/HealthComponent.h"
#include "Components/CharacterAnimationController.h"
#include "Components/SnapToGridComponent.h"
#include "Components/PepperComponent.h"
#include "Component/CameraComponent.h"

#include "BurgerTime.h"
#include "Component/SpriteAnimatorComponent.h"
#include "Component/SpriteAtlasComponent.h"
#include "Component/RenderComponent.h"
#include "State/StateMachine.h"

#include "GameManager.h"
#include "States/GameStates/BTGameMode.h"
#include "States/Enemy/EnemyControlledState.h"
#include "Managers/ResourceManager.h"
#include "Component/TextComponent.h"
#include "Component/UI/ButtonComponent.h"
#include "Component/UI/TextInputComponent.h"

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
            if (pSpriteObject)
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

    GameManager& gameManager{ GameManager::GetInstance() };
    BTGameMode* pGameMode{ dynamic_cast<BTGameMode*>(&gameManager.GetState()) };

    GameObject* score{ CreateScoreHUD(pObject, pGameMode->GetScore(), "1up", glm::vec4{ 1.f, 0.06f, 0.06f, 1.f }) };
    score->GetTransform().SetLocalPosition(glm::vec3{0.f, -24.f, 0.f});

    GameObject* hiscore{ CreateScoreHUD(pObject, gameManager.GetHiScore(), "hi-score", glm::vec4{1.f, 0.06f, 0.06f, 1.f})};
    hiscore->GetTransform().SetLocalPosition(glm::vec3{ 50.f, -24.f, 0.f });

    GameObject* peppersleft{ CreateScoreHUD(pObject, pController->GetPeppers(), "pepper", glm::vec4{0.06f, 1.f, 0.06f, 1.f})};
    peppersleft->GetTransform().SetLocalPosition(glm::vec3{ 150.f, -24.f, 0.f });

    return pObject;
}

dae::GameObject* dae::Prefabs::CreateTextObject(Scene* pScene, const std::string& text, std::shared_ptr<Font> pFont, const glm::vec4& color)
{
    GameObject* pObject{ pScene->Instantiate() };
    pObject->AddComponent<dae::TextureRenderComponent>(pObject->AddComponent<dae::TextureComponent>());
    pObject->AddComponent<TextComponent>(text, pFont, color);
    return pObject;
}

dae::GameObject* dae::Prefabs::CreateMenuObject(Scene* pScene)
{
    auto fontBig{ dae::ResourceManager::GetInstance().LoadFont("Fonts/super-burger-time.ttf", 16) };
    auto fontMedium{ dae::ResourceManager::GetInstance().LoadFont("Fonts/super-burger-time.ttf", 8) };
    const float tileSize{ 16.f };

    dae::CameraComponent* pCamera{ pScene->Instantiate(0u, nullptr)->AddComponent<dae::CameraComponent>(0.5f) };
    pCamera->GetTransform().Translate(glm::vec3{ 24.f, 64.f, 0.f });
    pCamera->SetCurrent();

    GameObject* pRoot{ pScene->Instantiate() };
    pRoot->AddTag("root");
    TextureComponent* pUiTexture{ pRoot->AddComponent<TextureComponent>() };
    pUiTexture->SetTexture("Textures/BTUIButton.png");
    SpriteAtlasComponent* pAtlas{ pRoot->AddComponent<SpriteAtlasComponent>(pUiTexture) };
    float width{ 120.f }, height{ 17.f };
    uint32_t buttonDefaultSprite{ pAtlas->AddSprite(0.f, 0.f, width, height) };
    uint32_t buttonHoveredSprite{ pAtlas->AddSprite(0.f, height, width, height) };
    uint32_t buttonSelectedSprite{ pAtlas->AddSprite(0.f, height * 2.f, width, height) };

    {
        GameObject* pCanvas{ pScene->Instantiate(pRoot) };
        pCanvas->AddTag("main");
        auto pTitleObject{ Prefabs::CreateTextObject(pScene, "Burger Time", fontBig, { 0.03f, 0.01f, 1.f, 1.f }) };
        pTitleObject->AttachToGameObject(pCanvas);
        pTitleObject->GetTransform().SetLocalPosition({ tileSize, 0.f, 0.f });

        //SinglePlayer
        {
            GameObject* pButtonObject{ pScene->Instantiate(pCanvas, glm::vec3{ tileSize * 3.f, tileSize * 4.f, 0.f }) };
            ButtonComponent* pButton{ pButtonObject->AddComponent<ButtonComponent>(glm::vec2{tileSize * 6.5f, tileSize}) };
            pButton->EnableToggle(false);
            GameObject* pButtonText{ Prefabs::CreateTextObject(pScene, "Single Player", fontMedium, { 182.f / 255.f, 182.f / 255.f, 170.f / 255.f, 1.f }) };
            pButtonText->AttachToGameObject(pButtonObject);
            pButtonText->GetTransform().Translate({ tileSize * 0.4f, tileSize * 0.3f, 0.f });
            UIButtonLayout layout{};
            //layout.color = glm::vec4{ 0.f, 0.5f, 0.7f, 1.f };
            layout.pSprite = pAtlas->GetSprite(buttonDefaultSprite);
            pButton->SetLayout(layout, UIButtonFlag::DefaultState);
            //layout.color = glm::vec4{ 0.f, 1.f, 0.f, 1.f };
            layout.pSprite = pAtlas->GetSprite(buttonSelectedSprite);
            pButton->SetLayout(layout, UIButtonFlag::Selected);
            //layout.color = glm::vec4{ 0.f, 0.5f, 0.7f, 1.f };
            layout.pSprite = pAtlas->GetSprite(buttonHoveredSprite);
            pButton->SetLayout(layout, UIButtonFlag::Hovered);
            pButtonObject->AddTag("SPButton");
        }
        //Co-op
        {
            GameObject* pButtonObject{ pScene->Instantiate(pCanvas, glm::vec3{ tileSize * 3.f, tileSize * 4.f + height + 4.f, 0.f }) };
            ButtonComponent* pButton{ pButtonObject->AddComponent<ButtonComponent>(glm::vec2{tileSize * 6.5f, tileSize}) };
            pButton->EnableToggle(false);
            GameObject* pButtonText{ Prefabs::CreateTextObject(pScene, "co-op", fontMedium, { 182.f / 255.f, 182.f / 255.f, 170.f / 255.f, 1.f }) };
            pButtonText->AttachToGameObject(pButtonObject);
            pButtonText->GetTransform().Translate({ tileSize * 0.4f, tileSize * 0.3f, 0.f });
            UIButtonLayout layout{};
            //layout.color = glm::vec4{ 0.f, 0.5f, 0.7f, 1.f };
            layout.pSprite = pAtlas->GetSprite(buttonDefaultSprite);
            pButton->SetLayout(layout, UIButtonFlag::DefaultState);
            //layout.color = glm::vec4{ 0.f, 1.f, 0.f, 1.f };
            layout.pSprite = pAtlas->GetSprite(buttonSelectedSprite);
            pButton->SetLayout(layout, UIButtonFlag::Selected);
            //layout.color = glm::vec4{ 0.f, 0.5f, 0.7f, 1.f };
            layout.pSprite = pAtlas->GetSprite(buttonHoveredSprite);
            pButton->SetLayout(layout, UIButtonFlag::Hovered);
            pButtonObject->AddTag("COButton");
        }
        //Versus
        {
            GameObject* pButtonObject{ pScene->Instantiate(pCanvas, glm::vec3{ tileSize * 3.f, tileSize * 4.f + height * 2.f + 8.f, 0.f }) };
            ButtonComponent* pButton{ pButtonObject->AddComponent<ButtonComponent>(glm::vec2{tileSize * 6.5f, tileSize}) };
            pButton->EnableToggle(false);
            GameObject* pButtonText{ Prefabs::CreateTextObject(pScene, "versus", fontMedium, { 182.f / 255.f, 182.f / 255.f, 170.f / 255.f, 1.f }) };
            pButtonText->AttachToGameObject(pButtonObject);
            pButtonText->GetTransform().Translate({ tileSize * 0.4f, tileSize * 0.3f, 0.f });
            UIButtonLayout layout{};
            //layout.color = glm::vec4{ 0.f, 0.5f, 0.7f, 1.f };
            layout.pSprite = pAtlas->GetSprite(buttonDefaultSprite);
            pButton->SetLayout(layout, UIButtonFlag::DefaultState);
            //layout.color = glm::vec4{ 0.f, 1.f, 0.f, 1.f };
            layout.pSprite = pAtlas->GetSprite(buttonSelectedSprite);
            pButton->SetLayout(layout, UIButtonFlag::Selected);
            //layout.color = glm::vec4{ 0.f, 0.5f, 0.7f, 1.f };
            layout.pSprite = pAtlas->GetSprite(buttonHoveredSprite);
            pButton->SetLayout(layout, UIButtonFlag::Hovered);
            pButtonObject->AddTag("VSButton");
        }
    }

    //hiscores
    {
        GameObject* pCanvas{ pScene->Instantiate(pRoot) };
        pCanvas->AddTag("hs");
        auto pTitleObject{ Prefabs::CreateTextObject(pScene, "Hi-Scores", fontBig, { 0.03f, 0.01f, 1.f, 1.f }) };
        pTitleObject->AttachToGameObject(pCanvas);
        pTitleObject->GetTransform().SetLocalPosition({ tileSize + 25.f, 0.f, 0.f });

        //MenuButton
        {
            GameObject* pButtonObject{ pScene->Instantiate(pCanvas, glm::vec3{ tileSize * 3.f, tileSize * 10.f + height * 2.f + 8.f, 0.f }) };
            ButtonComponent* pButton{ pButtonObject->AddComponent<ButtonComponent>(glm::vec2{tileSize * 6.5f, tileSize}) };
            pButton->EnableToggle(false);
            GameObject* pButtonText{ Prefabs::CreateTextObject(pScene, "Menu", fontMedium, { 182.f / 255.f, 182.f / 255.f, 170.f / 255.f, 1.f }) };
            pButtonText->AttachToGameObject(pButtonObject);
            pButtonText->GetTransform().Translate({ tileSize * 0.4f, tileSize * 0.3f, 0.f });
            UIButtonLayout layout{};
            //layout.color = glm::vec4{ 0.f, 0.5f, 0.7f, 1.f };
            layout.pSprite = pAtlas->GetSprite(buttonDefaultSprite);
            pButton->SetLayout(layout, UIButtonFlag::DefaultState);
            //layout.color = glm::vec4{ 0.f, 1.f, 0.f, 1.f };
            layout.pSprite = pAtlas->GetSprite(buttonSelectedSprite);
            pButton->SetLayout(layout, UIButtonFlag::Selected);
            //layout.color = glm::vec4{ 0.f, 0.5f, 0.7f, 1.f };
            layout.pSprite = pAtlas->GetSprite(buttonHoveredSprite);
            pButton->SetLayout(layout, UIButtonFlag::Hovered);
            pButtonObject->AddTag("MButton");
        }

        glm::vec2 pos{ (tileSize + 1) * 0.4f + 50.f, 0.f };
        const int numScores{ 10 };
        for (int i{}; i < numScores; ++i)
        {
            pos.y = (tileSize + 0.5f) * (i + 2);
            GameObject* pNameText{ Prefabs::CreateTextObject(pScene, "Name", fontMedium, { 182.f / 255.f, 182.f / 255.f, 170.f / 255.f, 1.f }) };
            pNameText->AttachToGameObject(pCanvas);
            pNameText->GetTransform().Translate({ pos.x, pos.y, 0.f });
            pNameText->AddTag("hsname");

            GameObject* pScoreText{ Prefabs::CreateTextObject(pScene, "0", fontMedium, { 182.f / 255.f, 182.f / 255.f, 170.f / 255.f, 1.f }) };
            pScoreText->AttachToGameObject(pCanvas);
            pScoreText->GetTransform().Translate({ 100.f + pos.x, pos.y, 0.f });
            pScoreText->AddTag("hsscore");
        }

        pCanvas->SetActive(false);
    }

    //Enter name
    {
        GameObject* pCanvas{ pScene->Instantiate(pRoot) };
        pCanvas->AddTag("en");
        auto pTitleObject{ Prefabs::CreateTextObject(pScene, "Name", fontBig, { 0.03f, 0.01f, 1.f, 1.f }) };
        pTitleObject->AttachToGameObject(pCanvas);
        pTitleObject->GetTransform().SetLocalPosition({ tileSize + 50.f, 0.f, 0.f });

        InputHandlerComponent* pInputHandler{ pCanvas->AddComponent<InputHandlerComponent>() };

        TextComponent* pText{ CreateTextObject(pScene, "name", fontMedium, {182.f / 255.f, 182.f / 255.f, 170.f / 255.f, 1.f})->GetComponent<TextComponent>()};
        pText->GetOwner()->AttachToGameObject(pCanvas);
        pText->GetTransform().SetLocalPosition({ tileSize * 3.f, tileSize * 8.f, 0.f });
        pText->GetOwner()->AddComponent<TextInputComponent>(pText, pInputHandler);

        //Next Button
        {
            GameObject* pButtonObject{ pScene->Instantiate(pCanvas, glm::vec3{ tileSize * 3.f, tileSize * 10.f + height * 2.f + 8.f, 0.f }) };
            ButtonComponent* pButton{ pButtonObject->AddComponent<ButtonComponent>(glm::vec2{tileSize * 6.5f, tileSize}) };
            pButton->EnableToggle(false);
            GameObject* pButtonText{ Prefabs::CreateTextObject(pScene, "Next", fontMedium, { 182.f / 255.f, 182.f / 255.f, 170.f / 255.f, 1.f }) };
            pButtonText->AttachToGameObject(pButtonObject);
            pButtonText->GetTransform().Translate({ tileSize * 0.4f, tileSize * 0.3f, 0.f });
            UIButtonLayout layout{};
            //layout.color = glm::vec4{ 0.f, 0.5f, 0.7f, 1.f };
            layout.pSprite = pAtlas->GetSprite(buttonDefaultSprite);
            pButton->SetLayout(layout, UIButtonFlag::DefaultState);
            //layout.color = glm::vec4{ 0.f, 1.f, 0.f, 1.f };
            layout.pSprite = pAtlas->GetSprite(buttonSelectedSprite);
            pButton->SetLayout(layout, UIButtonFlag::Selected);
            //layout.color = glm::vec4{ 0.f, 0.5f, 0.7f, 1.f };
            layout.pSprite = pAtlas->GetSprite(buttonHoveredSprite);
            pButton->SetLayout(layout, UIButtonFlag::Hovered);
            pButtonObject->AddTag("NButton");
        }

        pCanvas->SetActive(false);
    }
    return pRoot;
}

dae::GameObject* dae::Prefabs::CreatePepper(Scene* pScene)
{
    GameObject* pObject{ pScene->Instantiate(0u, nullptr) };

    TextureComponent* pTexture{ pObject->AddComponent<TextureComponent>() };
    pTexture->SetTexture("Textures/BurgerTimeCharacters.png");
    SpriteAtlasComponent* pSpriteAtlas{ pObject->AddComponent<SpriteAtlasComponent>(pTexture) };
    SpriteRenderComponent* pRenderer{ pObject->AddComponent<SpriteRenderComponent>() };
    pRenderer->SetLayer(3);
    SpriteAnimatorComponent* pAnimator{ pObject->AddComponent<SpriteAnimatorComponent>(pRenderer) };

    RigidBody2DComponent* pRigidBody{ pObject->AddComponent<RigidBody2DComponent>() };
    const glm::vec2 size{ 16.f, 16.f };
    BoxCollider2DComponent* pCollider{ pObject->AddComponent<BoxCollider2DComponent>() };
    pCollider->SetShape(pObject->AddComponent<QuadComponent>(size));
    pCollider->SetCollisionLayer(BurgerTime::PEPPER_COLLISION_LAYER);
    pCollider->SetCollisionIgnoreLayer(BurgerTime::PLAYER_COLLISION_LAYER, true);
    pCollider->SetCollisionIgnoreLayer(BurgerTime::LEVEL_COLLISION_LAYER, true);
    pCollider->SetCollisionIgnoreLayer(BurgerTime::PLATE_COLLISION_LAYER, true);
    pCollider->SetCollisionIgnoreLayer(BurgerTime::INGREDIENT_COLLISION_LAYER, true);
    pCollider->SetTrigger(true);
    pRigidBody->AddCollider(pCollider);

    pObject->AddComponent<PepperComponent>(pAnimator, pSpriteAtlas, pRigidBody);

    return pObject;
}

dae::GameObject* dae::Prefabs::CreateEnemy(GameObject* pObject)
{
    const float tileSize{ 8.f };

    const uint32_t reward{ 100 };
    CharacterInfoComponent* pCharacter{ pObject->GetComponent<CharacterInfoComponent>() };
    EnemyComponent* pEnemyComponent{ pObject->AddComponent<EnemyComponent>(pCharacter, reward) };
    pObject->AddComponent<StateMachine>(pEnemyComponent->GetStates().pGoToPlayerState.get());

    RigidBody2DComponent* pRigidBody{ pObject->GetComponent<RigidBody2DComponent>() };
    auto& pColliders{ pRigidBody->GetColliders() };
    for (auto& pCollider : pColliders)
    {
        pCollider->SetCollisionLayer(BurgerTime::ENEMY_COLLISION_LAYER);
        pCollider->SetCollisionIgnoreLayer(BurgerTime::ENEMY_COLLISION_LAYER, true);
        pCollider->SetCollisionIgnoreLayer(BurgerTime::PLAYER_COLLISION_LAYER, true);
    }

    BoxCollider2DComponent* pTrigger{ pObject->GetScene()->Instantiate(pObject)->AddComponent<BoxCollider2DComponent>()};
    pTrigger->GetOwner()->GetTransform().Translate(glm::vec3{ 0.f, -tileSize , 0.f });
    pTrigger->SetShape(pTrigger->GetOwner()->AddComponent<QuadComponent>(glm::vec2{ tileSize, tileSize * 2.5f }));
    pTrigger->SetCollisionLayer(BurgerTime::ENEMY_COLLISION_LAYER);
    pTrigger->SetCollisionIgnoreLayer(BurgerTime::ENEMY_COLLISION_LAYER, true);
    pTrigger->SetCollisionIgnoreLayer(BurgerTime::LEVEL_COLLISION_LAYER, true);
    pTrigger->SetCollisionIgnoreLayer(BurgerTime::PLATE_COLLISION_LAYER, true);
    pTrigger->SetTrigger(true);
    pRigidBody->AddCollider(pTrigger);

    return pObject;
}

dae::GameObject* dae::Prefabs::CreateControlledEnemy(GameObject* pObject)
{
    GameObject* pEnemy{ CreateEnemy(pObject) };
    EnemyComponent* pEnemyComponent{ pEnemy->GetComponent<EnemyComponent>() };
    auto pState{ std::make_unique<EnemyControlledState>(pEnemyComponent) };
    pEnemy->GetComponent<StateMachine>()->SetState(pState.get());
    pEnemyComponent->GetStates().pGoToPlayerState = std::move(pState);
    return pEnemy;
}

dae::GameObject* dae::Prefabs::CreateMrHotDog(Scene* pScene)
{
    GameObject* pObject{ pScene->Instantiate() };
    const float tileSize{ 8.f };

    CharacterController2D::CharacterController2DDesc controllerDesc{};
    controllerDesc.movementSpeed = 16.f;
    CharacterController2D* pCharacterController{ pObject->AddComponent<CharacterController2D>(controllerDesc) };
    pCharacterController->SetRigidBody(pObject->AddComponent<RigidBody2DComponent>());
    BoxCollider2DComponent* pCollider{ pObject->AddComponent<BoxCollider2DComponent>() };
    pCollider->SetShape(pObject->AddComponent<QuadComponent>(glm::vec2{ tileSize, tileSize }));
    pCollider->SetTrigger(false);
    pCharacterController->SetCollider(pCollider);

    //sprites
    TextureComponent* pTexture{ pObject->AddComponent<TextureComponent>() };
    pTexture->SetTexture("Textures/BurgerTimeCharacters.png");
    SpriteAtlasComponent* pSpriteAtlas{ pObject->AddComponent<SpriteAtlasComponent>(pTexture) };
    GameObject* pSpriteObject{ pObject->GetScene()->Instantiate(pObject, glm::vec3{0.f, -tileSize, 0.f}) };
    SpriteRenderComponent* pRenderer{ pSpriteObject->AddComponent<SpriteRenderComponent>() };
    pRenderer->SetLayer(3);

    SpriteAnimatorComponent* pAnimator{ pObject->AddComponent<SpriteAnimatorComponent>(pRenderer) };
    CharacterAnimationController* pAnimController{ pObject->AddComponent<CharacterAnimationController>(pAnimator) };

    pAnimController->AddState<WalkDownAnimState>(
        dae::CharacterAnimationController::CharacterAnim::WalkDown,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 0, 2, 2, 1 }));

    pAnimController->AddState<WalkLeftAnimState>(
        dae::CharacterAnimationController::CharacterAnim::WalkLeft,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 2, 2, 2, 1 }));

    pAnimController->AddState<WalkRightAnimState>(
        dae::CharacterAnimationController::CharacterAnim::WalkRight,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 2, 2, 2, 1 }));

    pAnimController->AddState<WalkUpAnimState>(
        dae::CharacterAnimationController::CharacterAnim::WalkUp,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 4, 2, 2, 1 }));

    pAnimController->AddState<DieAnimState>(
        dae::CharacterAnimationController::CharacterAnim::Die,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 0, 3, 4, 1 }));
    pAnimController->GetClip(dae::CharacterAnimationController::CharacterAnim::Die).speed *= 0.87f;

    pAnimController->AddState<StunnedAnimState>(
        dae::CharacterAnimationController::CharacterAnim::Stunned,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 4, 3, 2, 1 }));

    HealthComponent* pHealth{ pObject->AddComponent<HealthComponent>(1) };
    pObject->AddComponent<SnapToGridComponent>(pCharacterController->GetRigidBody());

    pObject->AddComponent<CharacterInfoComponent>(pHealth, pCharacterController, pAnimController);
    return pObject;
}

dae::GameObject* dae::Prefabs::CreateMrPickle(Scene* pScene)
{
    GameObject* pObject{ pScene->Instantiate() };
    const float tileSize{ 8.f };

    CharacterController2D::CharacterController2DDesc controllerDesc{};
    controllerDesc.movementSpeed = 16.f;
    CharacterController2D* pCharacterController{ pObject->AddComponent<CharacterController2D>(controllerDesc) };
    pCharacterController->SetRigidBody(pObject->AddComponent<RigidBody2DComponent>());
    BoxCollider2DComponent* pCollider{ pObject->AddComponent<BoxCollider2DComponent>() };
    pCollider->SetShape(pObject->AddComponent<QuadComponent>(glm::vec2{ tileSize, tileSize }));
    pCollider->SetTrigger(false);
    pCharacterController->SetCollider(pCollider);

    //sprites
    TextureComponent* pTexture{ pObject->AddComponent<TextureComponent>() };
    pTexture->SetTexture("Textures/BurgerTimeCharacters.png");
    SpriteAtlasComponent* pSpriteAtlas{ pObject->AddComponent<SpriteAtlasComponent>(pTexture) };
    GameObject* pSpriteObject{ pObject->GetScene()->Instantiate(pObject, glm::vec3{0.f, -tileSize, 0.f}) };
    SpriteRenderComponent* pRenderer{ pSpriteObject->AddComponent<SpriteRenderComponent>() };
    pRenderer->SetLayer(3);

    SpriteAnimatorComponent* pAnimator{ pObject->AddComponent<SpriteAnimatorComponent>(pRenderer) };
    CharacterAnimationController* pAnimController{ pObject->AddComponent<CharacterAnimationController>(pAnimator) };

    pAnimController->AddState<WalkDownAnimState>(
        dae::CharacterAnimationController::CharacterAnim::WalkDown,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 0, 4, 2, 1 }));

    pAnimController->AddState<WalkLeftAnimState>(
        dae::CharacterAnimationController::CharacterAnim::WalkLeft,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 2, 4, 2, 1 }));

    pAnimController->AddState<WalkRightAnimState>(
        dae::CharacterAnimationController::CharacterAnim::WalkRight,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 2, 4, 2, 1 }));

    pAnimController->AddState<WalkUpAnimState>(
        dae::CharacterAnimationController::CharacterAnim::WalkUp,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 4, 4, 2, 1 }));

    pAnimController->AddState<DieAnimState>(
        dae::CharacterAnimationController::CharacterAnim::Die,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 0, 5, 4, 1 }));
    pAnimController->GetClip(dae::CharacterAnimationController::CharacterAnim::Die).speed *= 0.87f;

    pAnimController->AddState<StunnedAnimState>(
        dae::CharacterAnimationController::CharacterAnim::Stunned,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 4, 5, 2, 1 }));

    HealthComponent* pHealth{ pObject->AddComponent<HealthComponent>(1) };
    pObject->AddComponent<SnapToGridComponent>(pCharacterController->GetRigidBody());

    pObject->AddComponent<CharacterInfoComponent>(pHealth, pCharacterController, pAnimController);
    return pObject;
}

dae::GameObject* dae::Prefabs::CreateMrEgg(Scene* pScene)
{
    GameObject* pObject{ pScene->Instantiate() };
    const float tileSize{ 8.f };

    CharacterController2D::CharacterController2DDesc controllerDesc{};
    controllerDesc.movementSpeed = 16.f;
    CharacterController2D* pCharacterController{ pObject->AddComponent<CharacterController2D>(controllerDesc) };
    pCharacterController->SetRigidBody(pObject->AddComponent<RigidBody2DComponent>());
    BoxCollider2DComponent* pCollider{ pObject->AddComponent<BoxCollider2DComponent>() };
    pCollider->SetShape(pObject->AddComponent<QuadComponent>(glm::vec2{ tileSize, tileSize }));
    pCollider->SetTrigger(false);
    pCharacterController->SetCollider(pCollider);

    //sprites
    TextureComponent* pTexture{ pObject->AddComponent<TextureComponent>() };
    pTexture->SetTexture("Textures/BurgerTimeCharacters.png");
    SpriteAtlasComponent* pSpriteAtlas{ pObject->AddComponent<SpriteAtlasComponent>(pTexture) };
    GameObject* pSpriteObject{ pObject->GetScene()->Instantiate(pObject, glm::vec3{0.f, -tileSize, 0.f}) };
    SpriteRenderComponent* pRenderer{ pSpriteObject->AddComponent<SpriteRenderComponent>() };
    pRenderer->SetLayer(3);

    SpriteAnimatorComponent* pAnimator{ pObject->AddComponent<SpriteAnimatorComponent>(pRenderer) };
    CharacterAnimationController* pAnimController{ pObject->AddComponent<CharacterAnimationController>(pAnimator) };

    pAnimController->AddState<WalkDownAnimState>(
        dae::CharacterAnimationController::CharacterAnim::WalkDown,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 0, 6, 2, 1 }));

    pAnimController->AddState<WalkLeftAnimState>(
        dae::CharacterAnimationController::CharacterAnim::WalkLeft,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 2, 6, 2, 1 }));

    pAnimController->AddState<WalkRightAnimState>(
        dae::CharacterAnimationController::CharacterAnim::WalkRight,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 2, 6, 2, 1 }));

    pAnimController->AddState<WalkUpAnimState>(
        dae::CharacterAnimationController::CharacterAnim::WalkUp,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 4, 6, 2, 1 }));

    pAnimController->AddState<DieAnimState>(
        dae::CharacterAnimationController::CharacterAnim::Die,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 0, 7, 4, 1 }));
    pAnimController->GetClip(dae::CharacterAnimationController::CharacterAnim::Die).speed *= 0.87f;

    pAnimController->AddState<StunnedAnimState>(
        dae::CharacterAnimationController::CharacterAnim::Stunned,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 4, 7, 2, 1 }));

    HealthComponent* pHealth{ pObject->AddComponent<HealthComponent>(1) };
    pObject->AddComponent<SnapToGridComponent>(pCharacterController->GetRigidBody());

    pObject->AddComponent<CharacterInfoComponent>(pHealth, pCharacterController, pAnimController);
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

dae::GameObject* dae::Prefabs::CreatePeterPepper(Scene* pScene, int index, bool useKey)
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

    pObject->AddComponent<PlayerController>(pCharacterController, index, useKey);

    //sprites
    const float spriteSize{ tileSize * 2.f };
    TextureComponent* pTexture{ pObject->AddComponent<TextureComponent>() };
    pTexture->SetTexture("Textures/BurgerTimeCharacters.png");
    SpriteAtlasComponent* pSpriteAtlas{ pObject->AddComponent<SpriteAtlasComponent>(pTexture) };
    uint32_t spritedID{ pSpriteAtlas->AddSprite(0.f, 0.f, spriteSize, spriteSize) };
    GameObject* pSpriteObject{ pObject->GetScene()->Instantiate(pObject, glm::vec3{0.f, -tileSize, 0.f}) };
    SpriteRenderComponent* pRenderer{pSpriteObject->AddComponent<SpriteRenderComponent>(pSpriteAtlas->GetSprite(spritedID))};
    pRenderer->SetLayer(3);

    HealthComponent* pHealth{ pObject->AddComponent<HealthComponent>(3) };
    SpriteAnimatorComponent* pAnimator{ pObject->AddComponent<SpriteAnimatorComponent>(pRenderer) };
    CharacterAnimationController* pAnimController{ pObject->AddComponent<CharacterAnimationController>(pAnimator) };

    pAnimController->AddState<WalkDownAnimState>(
        dae::CharacterAnimationController::CharacterAnim::WalkDown,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 0, 0, 3, 1 }));

    pAnimController->AddState<WalkLeftAnimState>(
        dae::CharacterAnimationController::CharacterAnim::WalkLeft,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 3, 0, 3, 1 }));

    pAnimController->AddState<WalkRightAnimState>(
        dae::CharacterAnimationController::CharacterAnim::WalkRight,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 3, 0, 3, 1 }, true));

    pAnimController->AddState<WalkUpAnimState>(
        dae::CharacterAnimationController::CharacterAnim::WalkUp,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 6, 0, 3, 1 }));

    pAnimController->AddState<DieAnimState>(
        dae::CharacterAnimationController::CharacterAnim::Die,
        dae::BurgerTime::CreateAnimClip(pSpriteAtlas, pAnimator, { 3, 1, 6, 1 }));
    pAnimController->GetClip(dae::CharacterAnimationController::CharacterAnim::Die).speed *= 0.87f;

    pObject->AddComponent<CharacterInfoComponent>(pHealth, pCharacterController, pAnimController);
    pObject->AddComponent<SnapToGridComponent>(pCharacterController->GetRigidBody());

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
