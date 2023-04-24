#include "InputCommandTestComponent.h"

#include "Input.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Time.h"

dae::ControllerCommandTestComponent::ControllerCommandTestComponent(GameObject* pGameObject, float speed, int controllerId)
	: Component(pGameObject), m_MovementSpeed{speed}, m_ControllerId{controllerId}
{
	m_pTransform = GetOwner()->GetComponent<TransformComponent>();

	Input::InputActionCommand actionCommand{ ActionCommand::Create(this, &ControllerCommandTestComponent::MoveDown) };
	actionCommand.IsController = (controllerId >= 0);
	actionCommand.Keyboard.Key = Keyboard::KeyCode::S;
	actionCommand.Keyboard.State = Keyboard::KeyState::Down;
	actionCommand.Controller.ButtonState = Controller::ControllerButtonState::Pressed;
	actionCommand.Controller.Button = Controller::ControllerButton::DPadDown;
	actionCommand.Controller.ControllerID = controllerId;

	Input::GetInstance().AddActionCommand(actionCommand);
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