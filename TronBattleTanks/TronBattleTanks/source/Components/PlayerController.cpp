#include "PlayerController.h"
#include "HealthComponent.h"
#include "Input/Input.h"
#include "Scene/GameObject.h"
#include "PlayerScore.h"
#include "Components/MovementComponent.h"
#include "Core/Time.h"
#include <memory>
#include "Scene/Scene.h"
//rendering
#include "Component/TextureComponent.h"
#include "Component/RenderComponent.h"
#include "TankCanon.h"

dae::PlayerController::PlayerController(GameObject* pOwner, int controllerIndex)
	: Component{pOwner}
	, m_ControllerIndex{controllerIndex}
{
	auto pScene{ GetOwner()->GetScene() };
	m_pMovement = GetOwner()->AddComponent<MovementComponent>();

	//Body
	auto pTankBody{ pScene->Instantiate(GetOwner())};
	//sprite
	pTankBody->AddComponent<TextureComponent>()->SetTexture("Sprites/TankBody.png");
	pTankBody->AddComponent<TextureRenderComponent>();

	//Canon
	m_pTankCanon = pScene->Instantiate(GetOwner());
	auto pCanon{ m_pTankCanon->AddComponent<TankCanon>() };

	//attack
	{
		Input::InputActionCommand actionCommand{ ActionCommand::Create(pCanon, &TankCanon::Shoot) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, (controllerIndex == 0));
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::ControllerButton, (controllerIndex >= 0));
		actionCommand.Controller.ButtonState = Controller::ControllerButtonState::Down;
		actionCommand.Controller.Button = Controller::ControllerButton::ButtonA;
		actionCommand.Controller.ControllerID = controllerIndex;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::Space;
		actionCommand.Keyboard.State = Keyboard::KeyState::Pressed;
		Input::GetInstance().AddActionCommand(actionCommand);
	}

	//Movement
	{
		glm::vec2 dir{-1.f, 0.f};
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&PlayerController::Move, this) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::A;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 1.f, 0.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&PlayerController::Move, this) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::D;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 0.f, 1.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&PlayerController::Move, this) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::S;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 0.f, -1.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&PlayerController::Move, this) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::W;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}

	//rotation
	{
		float dir{ 1.f };
		Input::InputValueCommand<float> actionCommand{ ValueCommand<float>::Create(&PlayerController::Rotate, this) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::Right;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddValueCommand(actionCommand);
	}
	{
		float dir{ -1.f };
		Input::InputValueCommand actionCommand{ ValueCommand<float>::Create(&PlayerController::Rotate, this) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::Left;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddValueCommand(actionCommand);
	}

	//Reset
	{
		Input::InputActionCommand actionCommand{ ActionCommand::Create(this, &PlayerController::Reset) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.Keyboard.Key = Keyboard::KeyCode::R;
		actionCommand.Keyboard.State = Keyboard::KeyState::Pressed;
		Input::GetInstance().AddActionCommand(actionCommand);
	}

	m_pHealth = GetOwner()->GetComponent<HealthComponent>();
}

void dae::PlayerController::Update()
{
	
}

void dae::PlayerController::Attack()
{
	
}

void dae::PlayerController::DealDamage(float damage)
{
	m_pHealth->DealDamage(damage);
}

void dae::PlayerController::Reset()
{
	m_pHealth->Heal(m_pHealth->GetMaxValue());
}

void dae::PlayerController::Move(const glm::vec2& dir)
{
	//float epsilon{ 0.01f };
	//if (std::abs(dir.x) > epsilon)
	//{
	//	if (std::abs(m_pMovement->GetVelocity().y) < epsilon && Math::AreSignsEqual(dir.x, m_pMovement->GetVelocity().x))
	//	{
	//		m_pMovement->Move({ dir.x, 0.f });
	//	}
	//}
	//else if (std::abs(dir.y) > epsilon)
	//{
	//	if (std::abs(m_pMovement->GetVelocity().x) < epsilon && Math::AreSignsEqual(dir.y, m_pMovement->GetVelocity().y))
	//	{
	//		m_pMovement->Move({ 0.f, dir.y });
	//	}
	//}

	m_pMovement->Move({ dir.x, dir.y });
}

void dae::PlayerController::Rotate(float dir)
{
	float dt{ Time::GetInstance().GetDeltaTime() };
	float speed{ m_RotationSpeed * dt * dir };
	m_pTankCanon->GetTransform().Rotate(speed);
}
