#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Core/Minigin.h"
#include "State/GameState/GameState.h"
#include "Prefabs.h"
#include <iostream>

#include "Scene/Scene.h"
#include "Managers/SceneManager.h"
#include "Editor/Editor.h"

void load()
{
	auto& sceneManager{ dae::SceneManager::GetInstance() };
	//auto& scene{ sceneManager.CreateScene("Level1") };
	auto& scene{ sceneManager.LoadScene("Scenes/BurgerTimeLevel1.scene")};
	//dae::Prefabs::CreateTestLevel(&scene);
	dae::Prefabs::CreatePeterPepper(&scene);
	dae::Editor editor{};
	editor.Init();
}

int main(int, char* []) 
{
	dae::Minigin engine("Content/");
	engine.Run(load);
	return 0;
}