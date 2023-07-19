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

#include "Scene/Scene.h"
#include "Managers/SceneManager.h"
#include "Editor/Editor.h"

#include "Component/CameraComponent.h"

#include "Scene/GameObject.h"

#include "Components/BurgerPlate.h"

void load()
{
	auto& sceneManager{ dae::SceneManager::GetInstance() };
	sceneManager.GetComponentFactory()->RegisterComponent<dae::PlateComponentSerializer>();

	//auto& scene{ sceneManager.CreateScene("Level1") };
	auto& scene{ sceneManager.LoadScene("Scenes/BurgerTimeLevel2.scene")};

	//dae::Prefabs::CreateTestLevel(&scene);
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::TopBun)->GetTransform().Translate(glm::vec3{16.f, 40.f, 0.f});
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::Lettuce)->GetTransform().Translate(glm::vec3{ 16.f, 72.f, 0.f });
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::Patty)->GetTransform().Translate(glm::vec3{ 16.f, 120.f, 0.f });
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::BottomBun)->GetTransform().Translate(glm::vec3{ 16.f, 152.f, 0.f });
	
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::TopBun)->GetTransform().Translate(glm::vec3{ 64.f, 8.f, 0.f });
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::Lettuce)->GetTransform().Translate(glm::vec3{ 64.f, 88.f, 0.f });
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::Patty)->GetTransform().Translate(glm::vec3{ 64.f, 120.f, 0.f });
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::BottomBun)->GetTransform().Translate(glm::vec3{ 64.f, 152.f, 0.f });
	
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::TopBun)->GetTransform().Translate(glm::vec3{ 112.f, 8.f, 0.f });
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::Lettuce)->GetTransform().Translate(glm::vec3{ 112.f, 40.f, 0.f });
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::Patty)->GetTransform().Translate(glm::vec3{ 112.f, 88.f, 0.f });
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::BottomBun)->GetTransform().Translate(glm::vec3{ 112.f, 152.f, 0.f });
	
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::TopBun)->GetTransform().Translate(glm::vec3{ 160.f, 8.f, 0.f });
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::Lettuce)->GetTransform().Translate(glm::vec3{ 160.f, 40.f, 0.f });
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::Patty)->GetTransform().Translate(glm::vec3{ 160.f, 72.f, 0.f });
	dae::Prefabs::CreateBurgerIngredient(&scene, dae::BurgerIngredient::IngredientType::BottomBun)->GetTransform().Translate(glm::vec3{ 160.f, 104.f, 0.f });

	dae::CameraComponent* pCamera{ scene.Instantiate()->AddComponent<dae::CameraComponent>(0.5f) }; 
	pCamera->GetTransform().Translate(glm::vec3{ 32.f, 64.f, 0.f });
	pCamera->SetCurrent();

	dae::Prefabs::CreatePeterPepper(&scene);
	dae::Editor editor{};
	editor.Init();
}

int main(int, char* []) 
{
	dae::Minigin engine("Content/");
	engine.Run(load);
	return 0;
}