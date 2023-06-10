#include "Editor.h"
#include "Managers/SceneManager.h"
#include "SceneGraphPanel.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "TileGridEditor.h"

dae::Editor::Editor()
{
	
}

void dae::Editor::Init()
{
	Scene* pScene{ SceneManager::GetInstance().GetCurrent() };
	GameObject* pEditorRoot{ pScene->Instantiate(dae::UUID(0u)) };
	auto pSceneGraph{ pEditorRoot->AddComponent<SceneGraphPanel>() };
	auto pTileEditor{ pEditorRoot->AddComponent<TileGridEditor>() };

	pTileEditor->GetOnGameObjectSelected() += [pSceneGraph](GameObject* pGameObject)
	{
		pSceneGraph->SelectGameObject(pGameObject);
	};
}
