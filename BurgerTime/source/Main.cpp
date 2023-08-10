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

#include "BurgerTime.h"

#include "Scene/Scene.h"
#include "Managers/SceneManager.h"

#include "Scene/GameObject.h"

#include "Components/BurgerPlate.h"
#include "Components/NodeComponent.h"
#include "Components/ObjectSpawner.h"

#include "States/GameModes/BTSinglePlayerGameMode.h"
#include "States/GameModes/MainMenuGameMode.h"
#include "Editor/BTEditorGameMode.h"

void load()
{
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

	auto& gameState{ dae::GameState::GetInstance() };
	//gameState.SetGameMode(std::make_shared<dae::BTSinglePlayerGameMode>());
	gameState.SetGameMode(std::make_shared<dae::MainMenuGameMode>());
	//gameState.SetGameMode(std::make_shared<dae::BTEditorGameMode>());

	//auto& scene{ sceneManager.CreateScene("MainMenu") };
	//auto& scene{ sceneManager.LoadScene("Scenes/TestPath.scene")};
	//auto& scene{ sceneManager.LoadScene("Scenes/BurgerTimeLevel1.scene") };

	//dae::Prefabs::CreateTestLevel(&scene);
	//dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::TopBun)->GetTransform().Translate(glm::vec3{24.f, 40.f, 0.f});
	//dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::Lettuce)->GetTransform().Translate(glm::vec3{ 24.f, 72.f, 0.f });
	//dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::Patty)->GetTransform().Translate(glm::vec3{ 24.f, 120.f, 0.f });
	//dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::BottomBun)->GetTransform().Translate(glm::vec3{ 24.f, 152.f, 0.f });
	//
	//dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::TopBun)->GetTransform().Translate(glm::vec3{ 64.f, 8.f, 0.f });
	//dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::Lettuce)->GetTransform().Translate(glm::vec3{ 64.f, 88.f, 0.f });
	//dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::Patty)->GetTransform().Translate(glm::vec3{ 64.f, 120.f, 0.f });
	//dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::BottomBun)->GetTransform().Translate(glm::vec3{ 64.f, 152.f, 0.f });
	//
	//dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::TopBun)->GetTransform().Translate(glm::vec3{ 112.f, 8.f, 0.f });
	//dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::Lettuce)->GetTransform().Translate(glm::vec3{ 112.f, 40.f, 0.f });
	//dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::Patty)->GetTransform().Translate(glm::vec3{ 112.f, 88.f, 0.f });
	//dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::BottomBun)->GetTransform().Translate(glm::vec3{ 112.f, 152.f, 0.f });
	//
	//dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::TopBun)->GetTransform().Translate(glm::vec3{ 160.f, 8.f, 0.f });
	//dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::Lettuce)->GetTransform().Translate(glm::vec3{ 160.f, 40.f, 0.f });
	//dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::Patty)->GetTransform().Translate(glm::vec3{ 160.f, 72.f, 0.f });
	//dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::BottomBun)->GetTransform().Translate(glm::vec3{ 160.f, 104.f, 0.f });
}

int main(int, char* []) 
{
	dae::Minigin engine("Content/");
	engine.Run(load);
	return 0;
}