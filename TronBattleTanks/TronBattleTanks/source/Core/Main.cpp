#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Core/Minigin.h"
#include "Managers/SceneManager.h"

void load()
{
	dae::SceneManager::GetInstance().CreateScene("Demo");
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}