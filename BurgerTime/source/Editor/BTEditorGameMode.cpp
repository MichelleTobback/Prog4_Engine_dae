#include "BTEditorGameMode.h"

#include "Managers/SceneManager.h"
#include "Scene/Scene.h"
#include "Component/CameraComponent.h"
#include "Editor.h"

dae::BTEditorGameMode::BTEditorGameMode(size_t sceneIndex)
    : GameState(sceneIndex)
{
    
}

void dae::BTEditorGameMode::OnEnter(Scene& scene)
{
    Editor editor{};
    editor.Init();

    dae::CameraComponent* pCamera{ scene.Instantiate(0u, nullptr)->AddComponent<dae::CameraComponent>(0.5f) };
    pCamera->GetTransform().Translate(glm::vec3{ 24.f, 64.f, 0.f });
    pCamera->SetCurrent();
}

void dae::BTEditorGameMode::OnExit(Scene&)
{
}
