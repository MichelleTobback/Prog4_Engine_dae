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
#include "Editor/Editor.h"

#include "Component/CameraComponent.h"

#include "Scene/GameObject.h"

#include "Components/BurgerPlate.h"
#include "Components/NodeComponent.h"
#include "Components/ObjectSpawner.h"

#include "States/GameModes/BTSinglePlayerGameMode.h"

void load()
{
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

	auto& gameState{ dae::GameState::GetInstance() };
	gameState.SetGameMode(std::make_shared<dae::BTSinglePlayerGameMode>());

	//auto& scene{ sceneManager.CreateScene("Level1") };
	auto& scene{ sceneManager.LoadScene("Scenes/TestPath.scene")};
	//auto& scene{ sceneManager.LoadScene("Scenes/BurgerTimeLevel1.scene") }; //gridtest

	//dae::Prefabs::CreateTestLevel(&scene);
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::TopBun)->GetTransform().Translate(glm::vec3{24.f, 40.f, 0.f});
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

	

	dae::CameraComponent* pCamera{ scene.Instantiate(0u, nullptr)->AddComponent<dae::CameraComponent>(0.5f) }; 
	pCamera->GetTransform().Translate(glm::vec3{ 24.f, 64.f, 0.f });
	pCamera->SetCurrent();

	//dae::Prefabs::CreatePeterPepper(&scene)->GetTransform().Translate(glm::vec3{8.f, 16.f, 0.f});
	dae::Editor editor{};
	editor.Init();
}

int main(int, char* []) 
{
	dae::Minigin engine("Content/");
	engine.Run(load);
	return 0;
}