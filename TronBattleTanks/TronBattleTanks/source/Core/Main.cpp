#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Core/Minigin.h"
#include "Managers/SceneManager.h"
#include "Prefabs.h"
#include "Editor/Editor.h"
#include "Components/TileComponent.h"
#include <iostream>

void load()
{
	auto& sceneManager{ dae::SceneManager::GetInstance() };
	sceneManager.GetOnDeserializedComponentDelegate() += &dae::TileComponent::Deserialize;

	//sceneManager.CreateScene("Demo");
	sceneManager.LoadScene("Scenes/TileTest.scene");
	
	//dae::Prefab::CreateTestLevel(&scene);

	//dae::Prefab::CreatePlayer(&scene, 3.f);
	
	//dae::Editor editor{};
	//editor.Init();
}

int main(int, char* []) {
	dae::Minigin engine("Content/");
	engine.Run(load);
	return 0;
}