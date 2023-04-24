#include "Prefabs.h"
#include "GameObject.h"

//player
#include "HealthComponent.h"
#include "PlayerController.h"
#include  "PlayerScore.h"

//rendering
#include "TextureComponent.h"
#include "RenderComponent.h"
#include "TextObject.h"

//hud
#include "PlayerHUD.h"

#include "ResourceManager.h"

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
