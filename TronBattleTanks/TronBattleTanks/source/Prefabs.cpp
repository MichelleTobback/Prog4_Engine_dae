#include "Prefabs.h"
#include "Scene/GameObject.h"

//player
#include "Components/HealthComponent.h"
#include "Components/PlayerController.h"

//rendering
#include "Component/TextureComponent.h"
#include "Component/RenderComponent.h"
#include "Component/TextComponent.h"
#include "Components/GridLevelComponent.h"

//hud
#include "Components/PlayerHUD.h"

#include "Managers/ResourceManager.h"
#include "Component/TransformComponent.h"

dae::GameObject* dae::Prefab::CreateTestLevel(Scene* pScene)
{
	auto pRoot{pScene->Instantiate()};
	auto pSpriteAtlas{ dae::Prefab::CreateSpriteAtlasObject(pScene, "Sprites/TronBattleTanksLevel.png") };
	pSpriteAtlas->GetOwner()->AttachToGameObject(pRoot);

	//GameObject* pRenderers{ pScene->Instantiate(pSpriteAtlas->GetOwner()) };

	//{
	//	auto idx{ pSpriteAtlas->AddSprite({ 0.f, 0.f, 64.f, 64.f }) };
	//	auto pRenderer{ pScene->Instantiate(pRenderers)->AddComponent<SpriteRenderComponent>(pSpriteAtlas->GetSprite(idx)) };
	//	pRenderer->GetOwner()->GetTransform().SetLocalPosition({ 000.f, 0.f, 0.f });
	//}
	//
	//{
	//	auto idx{ pSpriteAtlas->AddSprite({ 0.f, 000.f, 64.f, 64.f }) };
	//	auto pRenderer{ pScene->Instantiate(pRenderers)->AddComponent<SpriteRenderComponent>(pSpriteAtlas->GetSprite(idx)) };
	//	pRenderer->GetOwner()->GetTransform().SetLocalPosition({ 200.f, 400.f, 000.f });
	//}

	GridLevelDesc desc{};

	desc.Cols = 30;
	desc.Rows = 27;
	desc.pSpriteAtlas = pSpriteAtlas;

	//desc.Tiles =
	//{
	//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//	0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
	//	0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
	//	0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
	//	0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
	//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//};
	//
	//for (int y{}; y < desc.Cols; ++y)
	//{
	//	for (int x{}; x < desc.Rows; ++x)
	//	{
	//		desc.TileLayouts[]
	//	}
	//}

	pRoot->AddComponent<GridLevelComponent>(desc);

	return pRoot;
}

dae::GameObject* dae::Prefab::CreatePlayer(Scene* pScene, float health, int controllerIndex)
{
	auto go{ pScene->Instantiate() };

	go->AddComponent<HealthComponent>(health, health);
	go->AddComponent<PlayerController>(controllerIndex);

	return go;
}

dae::GameObject* dae::Prefab::CreatePlayerHUD(Scene* pScene, GameObject* pPlayer0, GameObject* pPlayer2)
{
	auto go{ pScene->Instantiate() };

	go->AddComponent<PlayerHUD>(pPlayer0, pPlayer2);

	return go;
}

dae::GameObject* dae::Prefab::CreateTextureRendererObject(Scene* pScene, const std::string& path)
{
	auto go{ pScene->Instantiate() };

	go->AddComponent<TextureComponent>()->SetTexture(path);
	go->AddComponent<TextureRenderComponent>();

	return go;
}

dae::GameObject* dae::Prefab::CreateSpriteObject(Scene* pScene, const std::string& path, const glm::vec4& src)
{
	auto go{ pScene->Instantiate() };

	auto pTextureComponent{ go->AddComponent<TextureComponent>() };
	pTextureComponent->SetTexture(path);
	auto pSpriteComponent{ go->AddComponent<SpriteComponent>(pTextureComponent, src) };
	go->AddComponent<SpriteRenderComponent>(pSpriteComponent);

	return go;
}

dae::SpriteAtlasComponent* dae::Prefab::CreateSpriteAtlasObject(Scene* pScene, const std::string& path)
{
	auto go{ pScene->Instantiate() };

	auto pTextureComponent{ go->AddComponent<TextureComponent>() };
	pTextureComponent->SetTexture(path);
	auto pSpriteComponent{ go->AddComponent<SpriteAtlasComponent>(pTextureComponent) };

	return pSpriteComponent;
}

dae::TextComponent* dae::Prefab::CreateTextObject(Scene* pScene, const std::string& txt, const glm::vec4& color)
{
	auto go{ pScene->Instantiate() };

	go->AddComponent<dae::TextureComponent>();
	go->AddComponent<dae::TextureRenderComponent>();

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);
	return go->AddComponent<dae::TextComponent>(txt, font, color);
}
