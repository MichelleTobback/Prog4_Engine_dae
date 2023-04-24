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

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	dae::Prefab::CreatePlayer(&scene, 3.f);
}

int main(int, char* []) {
	dae::Minigin engine("Content/");
	engine.Run(load);
	return 0;
}