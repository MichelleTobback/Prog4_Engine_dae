#include "PlayerController.h"
#include "HealthComponent.h"
#include "Input/Input.h"
#include "Scene/GameObject.h"
#include "PlayerScore.h"
#include "Core/Time.h"

dae::PlayerController::PlayerController(GameObject* pOwner, int controllerIndex)
	: Component{pOwner}
	, m_ControllerIndex{controllerIndex}
{
	m_pTransform = &GetOwner()->GetTransform();
	//attack
	//{
	//	Input::InputActionCommand actionCommand{ ActionCommand::Create(this, &PlayerController::Attack) };
	//	actionCommand.IsController = (controllerIndex >= 0);
	//	actionCommand.Keyboard.Key = Keyboard::KeyCode::Space;
	//	actionCommand.Keyboard.State = Keyboard::KeyState::Pressed;
	//	actionCommand.Controller.ButtonState = Controller::ControllerButtonState::Down;
	//	actionCommand.Controller.Button = Controller::ControllerButton::ButtonA;
	//	actionCommand.Controller.ControllerID = controllerIndex;
	//	Input::GetInstance().AddActionCommand(actionCommand);
	//}

	//Movement
	{
		glm::vec2 dir{-1.f, 0.f};
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&PlayerController::Move, this) };
		actionCommand.value = dir;
		actionCommand.IsController = false;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::A;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 1.f, 0.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&PlayerController::Move, this) };
		actionCommand.value = dir;
		actionCommand.IsController = false;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::D;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 0.f, 1.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&PlayerController::Move, this) };
		actionCommand.value = dir;
		actionCommand.IsController = false;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::S;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 0.f, -1.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&PlayerController::Move, this) };
		actionCommand.value = dir;
		actionCommand.IsController = false;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::W;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}

	//Reset
	{
		Input::InputActionCommand actionCommand{ ActionCommand::Create(this, &PlayerController::Reset) };
		actionCommand.IsController = false;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::R;
		actionCommand.Keyboard.State = Keyboard::KeyState::Pressed;
		Input::GetInstance().AddActionCommand(actionCommand);
	}

	m_pHealth = GetOwner()->GetComponent<HealthComponent>();
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
	float speed{ m_MovementSpeed * Time::GetInstance().GetDeltaTime() };
	m_pTransform->Translate({ dir.x * speed, dir.y * speed, 0.f });
}
