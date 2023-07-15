#include "Editor.h"
#include "Managers/SceneManager.h"
#include "SceneGraphPanel.h"
#include "BurgerTimeLevelPanel.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"

dae::Editor::Editor()
{
	
}

void dae::Editor::Init()
{
	Scene* pScene{ SceneManager::GetInstance().GetCurrent() };
	GameObject* pEditorRoot{ pScene->Instantiate(dae::UUID(0u)) };
	SceneGraphPanel* pSceneGraphPanel{ pEditorRoot->AddComponent<SceneGraphPanel>() };
	pEditorRoot->AddComponent<BurgerTimeLevelPanel>(pSceneGraphPanel);
}
