#include "MainMenuGameMode.h"
#include "Managers/SceneManager.h"
#include "Scene/Scene.h"
#include "Component/CameraComponent.h"
#include "Prefabs.h"
#include "Managers/ResourceManager.h"
#include "Component/UI/ButtonComponent.h"
#include "Editor/Editor.h"
#include "States/GameStates/BTSinglePlayerGameMode.h"
#include "States/GameStates/BTCoopGameMode.h"
#include "States/GameStates/BTVersusGameMode.h"
#include "States/GameStates/EnterNameMenuState.h"
#include "GameManager.h"

dae::MainMenuGameMode::MainMenuGameMode()
    : GameState(1)
{
   
}

void dae::MainMenuGameMode::OnEnter(Scene& scene)
{
    scene.GetGameObjectWithTag("main")[0]->SetActive(true);
    ButtonComponent* pButton{scene.GetGameObjectWithTag("SPButton")[0]->GetComponent<ButtonComponent>()};
    pButton->GetOnPressedDelegate() += std::bind(&MainMenuGameMode::OnSinglePlayerModeSelected, this);

    pButton = scene.GetGameObjectWithTag("COButton")[0]->GetComponent<ButtonComponent>();
    pButton->GetOnPressedDelegate() += std::bind(&MainMenuGameMode::OnCoOpModeSelected, this);

    pButton = scene.GetGameObjectWithTag("VSButton")[0]->GetComponent<ButtonComponent>();
    pButton->GetOnPressedDelegate() += std::bind(&MainMenuGameMode::OnVersusModeSelected, this);
}

void dae::MainMenuGameMode::OnExit(Scene& scene)
{
    scene.GetGameObjectWithTag("main")[0]->SetActive(false);
    ButtonComponent* pButton{ scene.GetGameObjectWithTag("SPButton")[0]->GetComponent<ButtonComponent>() };
    pButton->GetOnPressedDelegate().Clear();

    pButton = scene.GetGameObjectWithTag("COButton")[0]->GetComponent<ButtonComponent>();
    pButton->GetOnPressedDelegate().Clear();

    pButton = scene.GetGameObjectWithTag("VSButton")[0]->GetComponent<ButtonComponent>();
    pButton->GetOnPressedDelegate().Clear();
}

void dae::MainMenuGameMode::OnSinglePlayerModeSelected()
{
    auto& gameManager{ dae::GameManager::GetInstance() };
    gameManager.PushState(std::make_shared<dae::EnterNameMenuState>(std::make_shared<dae::BTSinglePlayerGameMode>(2)));
}

void dae::MainMenuGameMode::OnVersusModeSelected()
{
    auto& gameManager{ dae::GameManager::GetInstance() };
    gameManager.PushState(std::make_shared<dae::EnterNameMenuState>(std::make_shared<dae::BTVersusGameMode>(2)));
}

void dae::MainMenuGameMode::OnCoOpModeSelected()
{
    auto& gameManager{ dae::GameManager::GetInstance() };
    gameManager.PushState(std::make_shared<dae::EnterNameMenuState>(std::make_shared<dae::BTCoopGameMode>(2)));
}

void dae::MainMenuGameMode::CreateScene(Scene* pScene)
{
    auto fontBig{ dae::ResourceManager::GetInstance().LoadFont("Fonts/super-burger-time.ttf", 16) };
    auto fontMedium{ dae::ResourceManager::GetInstance().LoadFont("Fonts/super-burger-time.ttf", 8) };
    const float tileSize{ 16.f };

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
            pButton->GetOnReleasedDelegate() += std::bind(&MainMenuGameMode::OnSinglePlayerModeSelected, this);
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
            pButton->GetOnReleasedDelegate() += std::bind(&MainMenuGameMode::OnCoOpModeSelected, this);
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
            pButton->GetOnReleasedDelegate() += std::bind(&MainMenuGameMode::OnVersusModeSelected, this);
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
            pButton->GetOnReleasedDelegate() += std::bind(&MainMenuGameMode::OnVersusModeSelected, this);
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
        for (int i{}; i < 10; ++i)
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

        //Next Button
        {
            GameObject* pButtonObject{ pScene->Instantiate(pCanvas, glm::vec3{ tileSize * 3.f, tileSize * 10.f + height * 2.f + 8.f, 0.f }) };
            ButtonComponent* pButton{ pButtonObject->AddComponent<ButtonComponent>(glm::vec2{tileSize * 6.5f, tileSize}) };
            pButton->EnableToggle(false);
            pButton->GetOnReleasedDelegate() += std::bind(&MainMenuGameMode::OnVersusModeSelected, this);
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
}
