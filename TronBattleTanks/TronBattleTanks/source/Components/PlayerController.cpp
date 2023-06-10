#include "PlayerController.h"
#include "Input/Input.h"
#include "Scene/GameObject.h"
#include "PlayerScore.h"
#include "Core/Time.h"
#include <memory>
#include "Scene/Scene.h"

#include "Components/TankComponent.h"

dae::PlayerController::PlayerController(GameObject* pOwner, int controllerIndex)
	: Component{pOwner}
	, m_ControllerIndex{controllerIndex}
{
	TankComponent::TankDesc tankDesc{};
	tankDesc.health = 1.f;
	m_pTank = GetOwner()->AddComponent<TankComponent>(tankDesc);

	//attack
	{
		Input::InputActionCommand actionCommand{ ActionCommand::Create(&m_pTank->GetCanon(), &TankCanon::Shoot)};
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, (controllerIndex < 0));
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
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&TankComponent::Move, m_pTank) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::A;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 1.f, 0.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&TankComponent::Move, m_pTank) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::D;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 0.f, 1.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&TankComponent::Move, m_pTank) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::S;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 0.f, -1.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&TankComponent::Move, m_pTank) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::W;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}

	//rotation
	{
		float dir{ 1.f };
		Input::InputValueCommand<float> actionCommand{ ValueCommand<float>::Create(&TankComponent::RotateCanon, m_pTank) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::Right;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddValueCommand(actionCommand);
	}
	{
		float dir{ -1.f };
		Input::InputValueCommand actionCommand{ ValueCommand<float>::Create(&TankComponent::RotateCanon, m_pTank) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::Left;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddValueCommand(actionCommand);
	}
}
