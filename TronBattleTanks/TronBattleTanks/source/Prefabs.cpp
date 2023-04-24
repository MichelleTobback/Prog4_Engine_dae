#include "Prefabs.h"
#include "Scene/GameObject.h"

//player
#include "Components/HealthComponent.h"
#include "Components/PlayerController.h"
#include  "Components/PlayerScore.h"

//rendering
#include "Component/TextureComponent.h"
#include "Component/RenderComponent.h"
#include "Component/TextComponent.h"

//hud
#include "Components/PlayerHUD.h"

#include "Managers/ResourceManager.h"

dae::GameObject* dae::Prefab::CreatePlayer(Scene* pScene, float health, int controllerIndex)
{
	auto go{ std::make_shared<dae::GameObject>() };
	pScene->Add(go);

	go->AddComponent<HealthComponent>(health, health);
	go->AddComponent<PlayerController>(controllerIndex);
	go->AddComponent<PlayerScore>();

	return go.get();
}

dae::GameObject* dae::Prefab::CreatePlayerHUD(Scene* pScene, GameObject* pPlayer1, GameObject* pPlayer2)
{
	auto go{ std::make_shared<dae::GameObject>() };
	pScene->Add(go);

	go->AddComponent<PlayerHUD>(pPlayer1, pPlayer2);

	return go.get();
}

dae::TextComponent* dae::Prefab::CreateTextObject(Scene* pScene, const std::string& txt, const glm::vec4& color)
{
	auto go{ std::make_shared<dae::GameObject>() };
	pScene->Add(go);

	go->AddComponent<dae::TextureComponent>();
	go->AddComponent<dae::TextureRenderComponent>();

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);
	return go->AddComponent<dae::TextComponent>(txt, font, color);
}
