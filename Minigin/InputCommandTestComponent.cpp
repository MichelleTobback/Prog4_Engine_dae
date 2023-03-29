#include "InputCommandTestComponent.h"

#include "Input.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Time.h"

dae::ControllerCommandTestComponent::ControllerCommandTestComponent(GameObject* pGameObject, float speed, unsigned int controllerId)
	: Component(pGameObject), m_MovementSpeed{speed}, m_ControllerId{controllerId}
{
	m_pTransform = GetOwner()->GetComponent<TransformComponent>();

	//Left
	Input::ControllerActionCommand actionCommandLeft
	{
		m_ControllerId,
		Controller::ControllerButton::DPadLeft,
		Controller::ControllerButtonState::Pressed,
		ActionCommand{std::bind(&dae::ControllerCommandTestComponent::MoveLeft, this)}
	};
	
	Input::GetInstance().AddControllerActionCommand(actionCommandLeft);

	//Right
	Input::ControllerActionCommand actionCommandRight
	{
		m_ControllerId,
		Controller::ControllerButton::DPadRight,
		Controller::ControllerButtonState::Pressed,
		ActionCommand{std::bind(&dae::ControllerCommandTestComponent::MoveRight, this)}
	};

	Input::GetInstance().AddControllerActionCommand(actionCommandRight);

	//Up
	Input::ControllerActionCommand actionCommandUp
	{
		m_ControllerId,
		Controller::ControllerButton::DPadUp,
		Controller::ControllerButtonState::Pressed,
		ActionCommand{std::bind(&dae::ControllerCommandTestComponent::MoveUp, this)}
	};

	Input::GetInstance().AddControllerActionCommand(actionCommandUp);

	//Down
	Input::ControllerActionCommand actionCommandDown
	{
		m_ControllerId,
		Controller::ControllerButton::DPadDown,
		Controller::ControllerButtonState::Pressed,
		ActionCommand{std::bind(&dae::ControllerCommandTestComponent::MoveDown, this)}
	};

	Input::GetInstance().AddControllerActionCommand(actionCommandDown);
}

void dae::ControllerCommandTestComponent::MoveUp()
{
	Move(0.f, -1.f);
}

void dae::ControllerCommandTestComponent::MoveDown()
{
	Move(0.f, 1.f);
}

void dae::ControllerCommandTestComponent::MoveRight()
{
	Move(1.f, 0.f);
}

void dae::ControllerCommandTestComponent::MoveLeft()
{
	Move(-1.f, 0.f);
}

void dae::ControllerCommandTestComponent::Move(float x, float y)
{
	float speed{ m_MovementSpeed * Time::GetInstance().GetDeltaTime() };
	glm::vec3 translation{ x * speed, y * speed, 0.f };
	m_pTransform->Translate(translation);
}

dae::KeyboardCommandTestComponent::KeyboardCommandTestComponent(GameObject* pGameObject, float speed)
	: Component(pGameObject), m_MovementSpeed{ speed }
{
	m_pTransform = GetOwner()->GetComponent<TransformComponent>();

	//Left
	Input::KeyboardActionCommand actionCommandLeft
	{
		SDL_SCANCODE_A,
		ActionCommand{std::bind(&dae::KeyboardCommandTestComponent::MoveLeft, this)}
	};

	Input::GetInstance().AddKeyboardActionCommand(actionCommandLeft);

	//Right
	Input::KeyboardActionCommand actionCommandRight
	{
		SDL_SCANCODE_D,
		ActionCommand{std::bind(&dae::KeyboardCommandTestComponent::MoveRight, this)}
	};

	Input::GetInstance().AddKeyboardActionCommand(actionCommandRight);

	//Up
	Input::KeyboardActionCommand actionCommandUp
	{
		SDL_SCANCODE_W,
		ActionCommand{std::bind(&dae::KeyboardCommandTestComponent::MoveUp, this)}
	};

	Input::GetInstance().AddKeyboardActionCommand(actionCommandUp);

	//Down
	Input::KeyboardActionCommand actionCommandDown
	{
		SDL_SCANCODE_S,
		ActionCommand{std::bind(&dae::KeyboardCommandTestComponent::MoveDown, this)}
	};

	Input::GetInstance().AddKeyboardActionCommand(actionCommandDown);
}

void dae::KeyboardCommandTestComponent::MoveUp()
{
	Move(0.f, -1.f);
}

void dae::KeyboardCommandTestComponent::MoveDown()
{
	Move(0.f, 1.f);
}

void dae::KeyboardCommandTestComponent::MoveRight()
{
	Move(1.f, 0.f);
}

void dae::KeyboardCommandTestComponent::MoveLeft()
{
	Move(-1.f, 0.f);
}

void dae::KeyboardCommandTestComponent::Move(float x, float y)
{
	float speed{ m_MovementSpeed * Time::GetInstance().GetDeltaTime() };
	glm::vec3 translation{ x * speed, y * speed, 0.f };
	m_pTransform->Translate(translation);
}
