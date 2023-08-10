#include "BTEditorGameMode.h"

#include "Managers/SceneManager.h"
#include "Scene/Scene.h"
#include "Component/CameraComponent.h"
#include "Editor.h"

dae::BTEditorGameMode::BTEditorGameMode()
{
    SceneManager::GetInstance().GetOnSceneLoaded() += [](Scene* pScene, size_t)
    {
        Editor editor{};
        editor.Init();

        dae::CameraComponent* pCamera{ pScene->Instantiate(0u, nullptr)->AddComponent<dae::CameraComponent>(0.5f) };
        pCamera->GetTransform().Translate(glm::vec3{ 24.f, 64.f, 0.f });
        pCamera->SetCurrent();
    };
    m_SceneIndex = SceneManager::GetInstance().LoadScene("Scenes/BurgerTimeLevel1NoBurger.scene");
    //SceneManager::GetInstance().CreateScene("BurgerTimeLevel3", true);
}

void dae::BTEditorGameMode::OnEnter()
{
    SceneManager::GetInstance().OpenSceneByIndex(1);
}

dae::State::StatePtr dae::BTEditorGameMode::OnUpdate()
{
    return this;
}

void dae::BTEditorGameMode::OnExit()
{
}
