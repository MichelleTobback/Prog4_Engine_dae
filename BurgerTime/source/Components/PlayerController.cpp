#include "PlayerController.h"
#include "Input/Input.h"
#include <memory>
#include "Scene/Scene.h"

#include "States/GameStates/BTGameMode.h"
#include "GameManager.h"

#include "Components/CharacterAnimationController.h"
#include "HealthComponent.h"

#include "Component/InputHandlerComponent.h"
#include "BurgerTime.h"
#include "Prefabs.h"

dae::PlayerController::PlayerController(GameObject* pOwner, CharacterController2D* pCharactarerController, int controllerIndex, bool useKey)
	: Component{ pOwner }
	, m_pCharacterController{ pCharactarerController }
	, m_ControllerIndex{ controllerIndex }
{
	auto& input{ GetOwner()->AddComponent<InputHandlerComponent>()->GetHandler() };
	//Move
	{
		auto& commandBinding{ input.AddAxisBinding(static_cast<uint32_t>(BurgerTime::InputID::Move)) };
		dae::InputCommand::ICDevices binding{};
		BitFlag::Set(binding.flags, InputCommand::ICFlag::Keyboard, useKey);
		binding.keyboard.Key = Keyboard::KeyCode::A;
		binding.keyboard.State = Keyboard::KeyState::Down;
		BitFlag::Set(binding.flags, InputCommand::ICFlag::ControllerButton, controllerIndex >= 0);
		binding.controller.Button = dae::Controller::ControllerButton::DPadLeft;
		binding.controller.ButtonState = dae::Controller::ControllerButtonState::Pressed;
		binding.controller.ControllerID = m_ControllerIndex;
		commandBinding.Add({ -1.f, 0.f }, binding);

		binding.keyboard.Key = Keyboard::KeyCode::D;
		binding.controller.Button = dae::Controller::ControllerButton::DPadRight;
		commandBinding.Add({ 1.f, 0.f }, binding);

		binding.keyboard.Key = Keyboard::KeyCode::W;
		binding.controller.Button = dae::Controller::ControllerButton::DPadUp;
		commandBinding.Add({ 0.f, -1.f }, binding);

		binding.keyboard.Key = Keyboard::KeyCode::S;
		binding.controller.Button = dae::Controller::ControllerButton::DPadDown;
		commandBinding.Add({ 0.f, 1.f }, binding);

		input.BindAxisCommand(static_cast<uint32_t>(BurgerTime::InputID::Move), InputCommand::AxisCommand::Create(m_pCharacterController, &CharacterController2D::Move));
	}
	//Throw
	{
		auto& commandBinding{ input.AddActionBinding(static_cast<uint32_t>(BurgerTime::InputID::Throw)) };
		dae::InputCommand::ICDevices& binding{ commandBinding.deviceBinding };
		BitFlag::Set(binding.flags, InputCommand::ICFlag::Keyboard, useKey);
		binding.keyboard.Key = Keyboard::KeyCode::P;
		binding.keyboard.State = Keyboard::KeyState::Released;
		BitFlag::Set(binding.flags, InputCommand::ICFlag::ControllerButton, controllerIndex >= 0);
		binding.controller.Button = dae::Controller::ControllerButton::ButtonA;
		binding.controller.ButtonState = dae::Controller::ControllerButtonState::Down;
		binding.controller.ControllerID = m_ControllerIndex;
		input.BindActionCommand(static_cast<uint32_t>(BurgerTime::InputID::Throw), ActionCommand::Create(this, &PlayerController::ThrowPepper));
	}	

	m_pDamageSound = std::make_unique<AudioClip>("Sounds/04_Lose_Life.mp3");
}

void dae::PlayerController::Awake()
{
	Reset();

	HealthComponent* pHealth{ GetOwner()->GetComponent<HealthComponent>() };
	pHealth->GetOnHealthChanged() += std::bind(&PlayerController::OnHit, this, std::placeholders::_1);

	CharacterAnimationController* pAnimController{ GetOwner()->GetComponent<CharacterAnimationController>() };
	pAnimController->GetClip(CharacterAnimationController::CharacterAnim::Die).GetAnimEvent(4) += std::bind(&PlayerController::OnDeath, this);
}

void dae::PlayerController::LateUpdate()
{
}

void dae::PlayerController::Reset()
{
	m_pCurrentGameMode = dynamic_cast<BTGameMode*>(&GameManager::GetInstance().GetState());
	if (m_pCurrentGameMode)
		m_Peppers = m_pCurrentGameMode->GetPlayerMaxPeppers();
}

void dae::PlayerController::ThrowPepper()
{
	if (m_Peppers() > 0)
	{
		--m_Peppers;

		RigidBody2DComponent* pPepper{ Prefabs::CreatePepper(GetScene())->GetComponent<RigidBody2DComponent>() };
		const glm::vec2& direction{ m_pCharacterController->GetDirection() };
		const float throwForce{16.f};
		const glm::vec3 force{ direction.x * throwForce, direction.y * throwForce, 0.f };
		const glm::vec3 spawnPos{ GetTransform().GetWorldPosition() + force };
		pPepper->GetTransform().SetLocalPosition(spawnPos);
		pPepper->SetVelociy(force);
	}
}

void dae::PlayerController::OnHit(uint32_t health)
{
	if (m_pCurrentGameMode)
	{
		if (health < m_pCurrentGameMode->GetPlayerMaxLifes() && health > 0)
			m_pDamageSound->Play();
	}
}

void dae::PlayerController::OnDeath()
{
	HealthComponent* pHealth{ GetOwner()->GetComponent<HealthComponent>() };
	if (pHealth->GetValue() > 0)
		m_pCurrentGameMode->OnPlayerLostLife();
	else
		m_pCurrentGameMode->OnPlayerDeath();
}
