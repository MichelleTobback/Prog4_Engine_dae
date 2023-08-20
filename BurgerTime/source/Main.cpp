#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Core/Minigin.h"
#include "GameManager.h"
#include "Prefabs.h"
#include <iostream>

#include "BurgerTime.h"

#include "Scene/Scene.h"
#include "Managers/SceneManager.h"

#include "Scene/GameObject.h"

#include "Components/BurgerPlate.h"
#include "Components/NodeComponent.h"
#include "Components/ObjectSpawner.h"

#include "States/GameStates/BTSinglePlayerGameMode.h"
#include "States/GameStates/MainMenuGameMode.h"
#include "Editor/BTEditorGameMode.h"

void load()
{
	dae::BurgerTime::PrintControls();
	dae::BurgerTime::CreateInputBindings();
	dae::BurgerTime::CreateDebugInput();

	auto& sceneManager{ dae::SceneManager::GetInstance() };
	sceneManager.GetComponentFactory()->RegisterComponent<dae::PlateComponentSerializer>();
	sceneManager.GetComponentFactory()->RegisterComponent<dae::NodeComponentSerializer>();
	sceneManager.GetComponentFactory()->RegisterComponent<dae::ObjectSpawnerComponentSerializer>();

	dae::ObjectSpawner::Register(static_cast<uint32_t>(dae::BurgerTime::SpawnID::Player), [](dae::Scene* pScene)->dae::GameObject*
		{
			return dae::Prefabs::CreatePeterPepper(pScene);
		});
	dae::ObjectSpawner::Register(static_cast<uint32_t>(dae::BurgerTime::SpawnID::MrHotDog), [](dae::Scene* pScene)->dae::GameObject*
		{
			return dae::Prefabs::CreateEnemy(dae::Prefabs::CreateMrHotDog(pScene));
		});
	dae::ObjectSpawner::Register(static_cast<uint32_t>(dae::BurgerTime::SpawnID::MrPickle), [](dae::Scene* pScene)->dae::GameObject*
		{
			return dae::Prefabs::CreateEnemy(dae::Prefabs::CreateMrPickle(pScene));
		});
	dae::ObjectSpawner::Register(static_cast<uint32_t>(dae::BurgerTime::SpawnID::MrEgg), [](dae::Scene* pScene)->dae::GameObject*
		{
			return dae::Prefabs::CreateEnemy(dae::Prefabs::CreateMrEgg(pScene));
		});
	dae::ObjectSpawner::Register(static_cast<uint32_t>(dae::BurgerTime::SpawnID::TopBun), [](dae::Scene* pScene)->dae::GameObject*
		{
			return dae::Prefabs::CreateBurgerIngredient(pScene, dae::BurgerIngredient::IngredientType::TopBun);
		});
	dae::ObjectSpawner::Register(static_cast<uint32_t>(dae::BurgerTime::SpawnID::BottomBun), [](dae::Scene* pScene)->dae::GameObject*
		{
			return dae::Prefabs::CreateBurgerIngredient(pScene, dae::BurgerIngredient::IngredientType::BottomBun);
		});
	dae::ObjectSpawner::Register(static_cast<uint32_t>(dae::BurgerTime::SpawnID::Lettuce), [](dae::Scene* pScene)->dae::GameObject*
		{
			return dae::Prefabs::CreateBurgerIngredient(pScene, dae::BurgerIngredient::IngredientType::Lettuce);
		});
	dae::ObjectSpawner::Register(static_cast<uint32_t>(dae::BurgerTime::SpawnID::Tomato), [](dae::Scene* pScene)->dae::GameObject*
		{
			return dae::Prefabs::CreateBurgerIngredient(pScene, dae::BurgerIngredient::IngredientType::Tomato);
		});
	dae::ObjectSpawner::Register(static_cast<uint32_t>(dae::BurgerTime::SpawnID::Patty), [](dae::Scene* pScene)->dae::GameObject*
		{
			return dae::Prefabs::CreateBurgerIngredient(pScene, dae::BurgerIngredient::IngredientType::Patty);
		});
	dae::ObjectSpawner::Register(static_cast<uint32_t>(dae::BurgerTime::SpawnID::Cheese), [](dae::Scene* pScene)->dae::GameObject*
		{
			return dae::Prefabs::CreateBurgerIngredient(pScene, dae::BurgerIngredient::IngredientType::Cheese);
		});

	dae::BurgerTime::LoadScenes();

	auto& gameManager{ dae::GameManager::GetInstance() };
	//gameManager.PushState(std::make_shared<dae::BTSinglePlayerGameMode>(2));
	gameManager.PushState(std::make_shared<dae::MainMenuGameMode>());
	//gameManager.PushState(std::make_shared<dae::BTEditorGameMode>(4));
}

int main(int, char* []) 
{
	dae::Minigin engine("Content/");
	engine.Run(load);
	return 0;
}