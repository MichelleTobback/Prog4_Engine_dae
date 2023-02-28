#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "FPSComponent.h"

void AddTextureObject(const std::string& filename, const glm::vec2& pos, dae::Scene& scene);
void AddTextObject(const std::string& text, const std::string& filename, unsigned int size, const glm::vec2& pos, dae::Scene& scene);
void AddFPSCounterObject(dae::Scene& scene);

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	AddTextureObject("background.tga", {0.f, 0.f}, scene);

	AddTextureObject("logo.tga", { 216.f, 180.f }, scene);

	AddTextObject("Programming 4 Assignment", "Lingua.otf", 36, { 80.f, 20.f }, scene);

	AddFPSCounterObject(scene);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}

//test helper
void AddTextureObject(const std::string& filename, const glm::vec2& pos, dae::Scene& scene)
{
	auto go{ std::make_shared<dae::GameObject>() };

	dae::TextureComponent& tc{ go->AddComponent<dae::TextureComponent>() };
	tc.SetTexture(filename);
	go->AddComponent<dae::TextureRenderComponent>();
	scene.Add(go);

	auto& transform{ go->GetComponent<dae::TransformComponent>() };
	transform.SetPosition(pos.x, pos.y, 0.f);
}

void AddTextObject(const std::string& text, const std::string& filename, unsigned int size, const glm::vec2& pos, dae::Scene& scene)
{
	auto go{ std::make_shared<dae::GameObject>() };

	go->AddComponent<dae::TextureComponent>();
	go->AddComponent<dae::TextureRenderComponent>();

	auto font = dae::ResourceManager::GetInstance().LoadFont(filename, size);
	go->AddComponent<dae::TextComponent>(text, font);

	scene.Add(go);

	auto& transform{ go->GetComponent<dae::TransformComponent>() };
	transform.SetPosition(pos.x, pos.y, 0.f);
}

void AddFPSCounterObject(dae::Scene& scene)
{
	auto go{ std::make_shared<dae::GameObject>() };

	go->AddComponent<dae::TextureComponent>();
	go->AddComponent<dae::TextureRenderComponent>();

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);
	go->AddComponent<dae::TextComponent>("FPS", font);
	go->AddComponent<dae::FPSComponent>();

	scene.Add(go);
}