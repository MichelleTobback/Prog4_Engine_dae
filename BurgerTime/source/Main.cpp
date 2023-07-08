#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Core/Minigin.h"
#include "State/GameState/GameState.h"
#include <iostream>

#include "Scene/Scene.h"
#include "Managers/SceneManager.h"

void load()
{
	auto& sceneManager{ dae::SceneManager::GetInstance() };
	sceneManager.CreateScene("Level1");
	//dae::Editor editor{};
	//editor.Init();
}

int main(int, char* []) 
{
	dae::Minigin engine("Content/");
	engine.Run(load);
	return 0;
}