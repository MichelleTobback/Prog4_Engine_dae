#include "PlayerController.h"
#include "Input/Input.h"
#include <memory>
#include "Scene/Scene.h"

dae::PlayerController::PlayerController(GameObject* pOwner, BurgerTimeMovementController* pCharactarerController, int controllerIndex)
	: Component{ pOwner }
	, m_pCharacterController{ pCharactarerController }
	, m_ControllerIndex{ controllerIndex }
{
	//m_pTank->GetHealth().GetOnDeath() += std::bind(&PlayerController::OnDeath, this);

	//Movement
	{
		glm::vec2 dir{ -1.f, 0.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&BurgerTimeMovementController::Move, m_pCharacterController) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::A;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 1.f, 0.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&BurgerTimeMovementController::Move, m_pCharacterController) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::D;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 0.f, 1.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&BurgerTimeMovementController::Move, m_pCharacterController) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::S;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 0.f, -1.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&BurgerTimeMovementController::Move, m_pCharacterController) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::W;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
}

void dae::PlayerController::Awake()
{
	
}

void dae::PlayerController::LateUpdate()
{
	
}

void dae::PlayerController::OnDeath()
{
	
}
