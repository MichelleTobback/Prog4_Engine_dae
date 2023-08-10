#include "MainMenuGameMode.h"
#include "Managers/SceneManager.h"
#include "Scene/Scene.h"
#include "Component/CameraComponent.h"
#include "Prefabs.h"
#include "Managers/ResourceManager.h"
#include "Component/UI/ButtonComponent.h"
#include "Editor/Editor.h"
#include "States/GameModes/BTSinglePlayerGameMode.h"

dae::MainMenuGameMode::MainMenuGameMode()
    : GameMode()
{
    m_SceneIndex = SceneManager::GetInstance().LoadScene("Scenes/MainMenu.scene");
}

void dae::MainMenuGameMode::OnEnter()
{
   SceneManager::GetInstance().OpenSceneByIndex(m_SceneIndex, 
        std::bind(&MainMenuGameMode::OnSceneLoaded, this, std::placeholders::_1));
}

dae::State::StatePtr dae::MainMenuGameMode::OnUpdate()
{
    return this;
}

void dae::MainMenuGameMode::OnExit()
{
    SceneManager::GetInstance().GetCurrent()->Sleep();
}

void dae::MainMenuGameMode::OnSceneLoaded(Scene& scene)
{
    auto fontBig{ dae::ResourceManager::GetInstance().LoadFont("Fonts/super-burger-time.ttf", 16) };
    auto fontMedium{ dae::ResourceManager::GetInstance().LoadFont("Fonts/super-burger-time.ttf", 8) };
    const float tileSize{ 16.f };

    GameObject* pCanvas{ scene.Instantiate() };
    TextureComponent* pUiTexture{ pCanvas->AddComponent<TextureComponent>() };
    pUiTexture->SetTexture("Textures/BTUIButton.png");
    SpriteAtlasComponent* pAtlas{ pCanvas->AddComponent<SpriteAtlasComponent>(pUiTexture) };
    float width{120.f}, height{17.f};
    uint32_t buttonDefaultSprite{ pAtlas->AddSprite(0.f, 0.f, width, height) };
    uint32_t buttonHoveredSprite{ pAtlas->AddSprite(0.f, height, width, height) };
    uint32_t buttonSelectedSprite{ pAtlas->AddSprite(0.f, height * 2.f, width, height) };

    auto pTitleObject{ Prefabs::CreateTextObject(&scene, "Burger Time", fontBig, { 0.03f, 0.01f, 1.f, 1.f }) };
    pTitleObject->AttachToGameObject(pCanvas);
    pTitleObject->GetTransform().SetLocalPosition({ tileSize, 0.f, 0.f });

    //SinglePlayer
    {
        GameObject* pButtonObject{ scene.Instantiate(pCanvas, glm::vec3{ tileSize * 3.f, tileSize * 4.f, 0.f }) };
        ButtonComponent* pButton{ pButtonObject->AddComponent<ButtonComponent>(glm::vec2{tileSize * 6.5f, tileSize}) };
        pButton->EnableToggle(false);
        pButton->GetOnPressedDelegate() += std::bind(&MainMenuGameMode::OnSinglePlayerModeSelected, this);
        GameObject* pButtonText{ Prefabs::CreateTextObject(&scene, "Single Player", fontMedium, { 182.f / 255.f, 182.f / 255.f, 170.f / 255.f, 1.f }) };
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
        GameObject* pButtonObject{ scene.Instantiate(pCanvas, glm::vec3{ tileSize * 3.f, tileSize * 4.f + height + 4.f, 0.f }) };
        ButtonComponent* pButton{ pButtonObject->AddComponent<ButtonComponent>(glm::vec2{tileSize * 6.5f, tileSize}) };
        pButton->EnableToggle(false);
        pButton->GetOnPressedDelegate() += std::bind(&MainMenuGameMode::OnCoOpModeSelected, this);
        GameObject* pButtonText{ Prefabs::CreateTextObject(&scene, "co-op", fontMedium, { 182.f / 255.f, 182.f / 255.f, 170.f / 255.f, 1.f }) };
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
        GameObject* pButtonObject{ scene.Instantiate(pCanvas, glm::vec3{ tileSize * 3.f, tileSize * 4.f + height * 2.f + 8.f, 0.f }) };
        ButtonComponent* pButton{ pButtonObject->AddComponent<ButtonComponent>(glm::vec2{tileSize * 6.5f, tileSize}) };
        pButton->EnableToggle(false);
        pButton->GetOnPressedDelegate() += std::bind(&MainMenuGameMode::OnVersusModeSelected, this);
        GameObject* pButtonText{ Prefabs::CreateTextObject(&scene, "versus", fontMedium, { 182.f / 255.f, 182.f / 255.f, 170.f / 255.f, 1.f }) };
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

    dae::CameraComponent* pCamera{ scene.Instantiate(0u, nullptr)->AddComponent<dae::CameraComponent>(0.5f) };
    pCamera->GetTransform().Translate(glm::vec3{ 24.f, 64.f, 0.f });
    pCamera->SetCurrent();

    //dae::Editor editor{};
    //editor.Init();
}

void dae::MainMenuGameMode::OnSinglePlayerModeSelected()
{
    auto& gameState{ dae::GameState::GetInstance() };
    gameState.SetGameMode(std::make_shared<dae::BTSinglePlayerGameMode>());
}

void dae::MainMenuGameMode::OnVersusModeSelected()
{
}

void dae::MainMenuGameMode::OnCoOpModeSelected()
{
}
