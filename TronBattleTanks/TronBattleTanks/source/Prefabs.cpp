#include "Prefabs.h"
#include "Scene/GameObject.h"

//player
#include "Components/HealthComponent.h"
#include "Components/PlayerController.h"
#include "Component/Physics/RigidBody2DComponent.h"
#include "Components/TankComponent.h"

//rendering
#include "Component/TextureComponent.h"
#include "Component/RenderComponent.h"
#include "Component/TextComponent.h"
#include "Components/GridLevelComponent.h"

//hud
#include "Components/PlayerHUD.h"

#include "Managers/ResourceManager.h"
#include "Component/TransformComponent.h"

#include "State/StateMachine.h"
#include "State/Enemy/EnemyStates.h"

#include <iostream>

dae::GameObject* dae::Prefab::CreateTestLevel(Scene* pScene)
{
	auto pRoot{pScene->Instantiate()};
	auto pSpriteAtlas{ dae::Prefab::CreateSpriteAtlasObject(pScene, "Sprites/TronBattleTanksLevel.png") };
	pSpriteAtlas->GetOwner()->AttachToGameObject(pRoot);

	GridLevelDesc desc{};

	desc.Cols = 30;
	desc.Rows = 27;
	desc.pSpriteAtlas = pSpriteAtlas;

	pRoot->AddComponent<GridLevelComponent>(desc);

	return pRoot;
}

dae::GameObject* dae::Prefab::CreatePlayer(Scene* pScene, float health, int controllerIndex)
{
	auto go{ pScene->Instantiate() };

	auto pController{ go->AddComponent<PlayerController>(controllerIndex) };
	pController->GetTank().GetHealth().SetMax(health, true);

	//debug
	pController->GetTank().GetRigidBody().GetOnBeginOverlap() += [](const CollisionHit&)
	{
		std::cout << "begin overlap player\n";
	};
	pController->GetTank().GetRigidBody().GetOnEndOverlap() += [](const CollisionHit&)
	{
		std::cout << "end overlap player\n";
	};

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

dae::GameObject* dae::Prefab::CreateEnemyTank(Scene* pScene)
{
	auto go{ pScene->Instantiate() };

	TankComponent::TankDesc desc{};
	desc.health = 3.f;
	auto pTank{ go->AddComponent<TankComponent>(desc) };
	go->AddComponent<StateMachine>(std::move(std::make_shared<EnemyIdleState>(pTank)));
	return go;
}

dae::TextComponent* dae::Prefab::CreateTextObject(Scene* pScene, const std::string& txt, const glm::vec4& color)
{
	auto go{ pScene->Instantiate() };

	go->AddComponent<dae::TextureComponent>();
	go->AddComponent<dae::TextureRenderComponent>();

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);
	return go->AddComponent<dae::TextComponent>(txt, font, color);
}
