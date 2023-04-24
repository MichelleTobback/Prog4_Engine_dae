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
#include "RotationComponent.h"
#include "ImGuiPlotComponent.h"
#include "InputCommandTestComponent.h"
#include "Prefabs.h"
#include "HealthComponent.h"
#include "PlayerController.h"
#include "PlayerScore.h"
#include "SteamAchievementUtils.h"
#include <iostream>

void AddTextureObject(const std::string& filename, const glm::vec2& pos, dae::Scene& scene);
void AddTextObject(const std::string& text, const std::string& filename, unsigned int size, const glm::vec2& pos, dae::Scene& scene);
void AddFPSCounterObject(dae::Scene& scene);
void AddWeek02Circles(dae::Scene& scene);
void AddInputCommandExercise(dae::Scene& scene);

void AddObserverExercise(dae::Scene& scene);

void AddImGuiPlots(dae::Scene& scene);

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	AddTextureObject("background.tga", {0.f, 0.f}, scene);

	AddTextureObject("logo.tga", { 216.f, 180.f }, scene);

	AddTextObject("Programming 4 Assignment", "Lingua.otf", 36, { 80.f, 20.f }, scene);

	AddFPSCounterObject(scene);

	AddObserverExercise(scene);
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
	scene.Add(go);

	dae::TextureComponent& tc{ *go->AddComponent<dae::TextureComponent>() };
	tc.SetTexture(filename);
	go->AddComponent<dae::TextureRenderComponent>();

	auto& transform{ *go->GetComponent<dae::TransformComponent>() };
	transform.SetLocalPosition({ pos.x, pos.y, 0.f });
}

void AddTextObject(const std::string& text, const std::string& filename, unsigned int size, const glm::vec2& pos, dae::Scene& scene)
{
	auto go{ std::make_shared<dae::GameObject>() };
	scene.Add(go);

	go->AddComponent<dae::TextureComponent>();
	go->AddComponent<dae::TextureRenderComponent>();

	auto font = dae::ResourceManager::GetInstance().LoadFont(filename, size);
	go->AddComponent<dae::TextComponent>(text, font);

	auto& transform{ *go->GetComponent<dae::TransformComponent>() };
	transform.SetLocalPosition({ pos.x, pos.y, 0.f });
}

void AddFPSCounterObject(dae::Scene& scene)
{
	auto go{ std::make_shared<dae::GameObject>() };
	scene.Add(go);

	go->AddComponent<dae::TextureComponent>();
	go->AddComponent<dae::TextureRenderComponent>();

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);
	go->AddComponent<dae::TextComponent>("FPS", font);
	go->AddComponent<dae::FPSComponent>();
}

void AddWeek02Circles(dae::Scene& scene)
{
	//parent 
	auto root{ std::make_shared<dae::GameObject>() };
	scene.Add(root);

	root->GetComponent<dae::TransformComponent>()->SetLocalPosition({ 256.f, 270.f, 0.f });

	//first
	auto go{ std::make_shared<dae::GameObject>() };
	scene.Add(go);
	go->AttachToGameObject(root.get());

	auto& tc{ *go->AddComponent<dae::TextureComponent>() };
	tc.SetTexture("Textures/Dot1.png");
	go->AddComponent<dae::TextureRenderComponent>();
	go->AddComponent<dae::RotateComponent>(3.1415f * 0.001f);

	go->GetComponent<dae::TransformComponent>()->SetLocalPosition({ 10.f, 10.f, 0.f });

	//second
	auto go2{ std::make_shared<dae::GameObject>() };
	scene.Add(go2);
	go2->AttachToGameObject(go.get());

	auto& tc2{ *go2->AddComponent<dae::TextureComponent>() };
	tc2.SetTexture("Textures/Dot2.png");
	go2->AddComponent<dae::TextureRenderComponent>();
	go2->AddComponent<dae::RotateComponent>(-3.1415f * 0.001f);

	go2->GetComponent<dae::TransformComponent>()->SetLocalPosition({ 20.f, 20.f, 0.f });
}

void AddImGuiPlots(dae::Scene& scene)
{
	auto plot1{ std::make_shared<dae::GameObject>() };
	scene.Add(plot1);
	plot1->AddComponent<dae::ImGuiPlotComponent>("Excercise 1");
}

void AddInputCommandExercise(dae::Scene& scene)
{
	float speed{ 0.1f };

	//keyboard
	{
		auto go{ std::make_shared<dae::GameObject>() };
		scene.Add(go);

		auto& tc{ *go->AddComponent<dae::TextureComponent>() };
		tc.SetTexture("Textures/Dot1.png");
		go->AddComponent<dae::TextureRenderComponent>();

		go->GetComponent<dae::TransformComponent>()->SetLocalPosition({ 10.f, 10.f, 0.f });

		go->AddComponent<dae::ControllerCommandTestComponent>(speed * 2.f, -1);
	}

	//controller
	{
		auto go2{ std::make_shared<dae::GameObject>() };
		scene.Add(go2);

		auto& tc2{ *go2->AddComponent<dae::TextureComponent>() };
		tc2.SetTexture("Textures/Dot2.png");
		go2->AddComponent<dae::TextureRenderComponent>();

		go2->GetComponent<dae::TransformComponent>()->SetLocalPosition({ 10.f, 10.f, 0.f });

		go2->AddComponent<dae::ControllerCommandTestComponent>(speed, 0);
	}
}

void AddObserverExercise(dae::Scene& scene)
{
	dae::SteamAchievementUtils::CreateSpaceWarAchievements();

	std::cout << "Controlls:\n";
	std::cout << "Player 1 : Key Space to attack\n";
	std::cout << "Player 2 : Controller A button to attack\n";
	std::cout << "Key R to reset lifes\n";

	auto player1{ dae::Prefab::CreatePlayer(&scene, 10.f, -1) };
	auto player2{ dae::Prefab::CreatePlayer(&scene, 10.f, 0) };
	auto controller1{ player1->GetComponent<dae::PlayerController>() };
	auto controller2{ player2->GetComponent<dae::PlayerController>() };
	controller1->SetOpponent(controller2);
	controller2->SetOpponent(controller1);
	dae::Prefab::CreatePlayerHUD(&scene, player1, player2);
}