#include "PlayerController.h"
#include "Input/Input.h"
#include <memory>
#include "Scene/Scene.h"

#include "State/GameState/GameState.h"
#include "States/GameModes/BTGameMode.h"

#include "Components/CharacterAnimationController.h"
#include "HealthComponent.h"

#include "Prefabs.h"

dae::PlayerController::PlayerController(GameObject* pOwner, CharacterController2D* pCharactarerController, int controllerIndex)
	: Component{ pOwner }
	, m_pCharacterController{ pCharactarerController }
	, m_ControllerIndex{ controllerIndex }
{
	//Movement
	{
		glm::vec2 dir{ -1.f, 0.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&CharacterController2D::Move, m_pCharacterController) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::A;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 1.f, 0.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&CharacterController2D::Move, m_pCharacterController) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::D;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 0.f, 1.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&CharacterController2D::Move, m_pCharacterController) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::S;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 0.f, -1.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&CharacterController2D::Move, m_pCharacterController) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::W;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}

	{
		Input::InputActionCommand actionCommand{ ActionCommand::Create(this, &PlayerController::ThrowPepper) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.Keyboard.Key = Keyboard::KeyCode::P;
		actionCommand.Keyboard.State = Keyboard::KeyState::Pressed;
		Input::GetInstance().AddActionCommand(actionCommand);
	}

	m_pDamageSound = std::make_unique<AudioClip>("Sounds/04_Lose_Life.mp3");
}

void dae::PlayerController::Awake()
{
	Reset();

	HealthComponent* pHealth{ GetOwner()->GetComponent<HealthComponent>() };
	pHealth->GetOnHealthChanged() += std::bind(&PlayerController::OnHit, this, std::placeholders::_1);

	CharacterAnimationController* pAnimController{ GetOwner()->GetComponent<CharacterAnimationController>() };
	//pAnimController->GetClip(CharacterAnimationController::CharacterAnim::Die).GetOnClipEndDelegate() += std::bind(&PlayerController::OnDeath, this);
	pAnimController->GetClip(CharacterAnimationController::CharacterAnim::Die).GetAnimEvent(4) += std::bind(&PlayerController::OnDeath, this);
}

void dae::PlayerController::LateUpdate()
{
	
}

void dae::PlayerController::Reset()
{
	m_pCurrentGameMode = dynamic_cast<BTGameMode*>(&GameState::GetInstance().GetGameMode());
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
