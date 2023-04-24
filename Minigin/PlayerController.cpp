#include "PlayerController.h"
#include "HealthComponent.h"
#include "Input.h"
#include "GameObject.h"
#include "PlayerScore.h"

dae::PlayerController::PlayerController(GameObject* pOwner, int controllerIndex)
	: Component{pOwner}
	, m_ControllerIndex{controllerIndex}
{
	//attack
	{
		Input::InputActionCommand actionCommand{ ActionCommand::Create(this, &PlayerController::Attack) };
		actionCommand.IsController = (controllerIndex >= 0);
		actionCommand.Keyboard.Key = Keyboard::KeyCode::Space;
		actionCommand.Keyboard.State = Keyboard::KeyState::Pressed;
		actionCommand.Controller.ButtonState = Controller::ControllerButtonState::Down;
		actionCommand.Controller.Button = Controller::ControllerButton::ButtonA;
		actionCommand.Controller.ControllerID = controllerIndex;
		Input::GetInstance().AddActionCommand(actionCommand);
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
	float damage{ 2.f };
	if (m_pOpponent)
	{
		m_pOpponent->DealDamage(damage);
	}
}

void dae::PlayerController::DealDamage(float damage)
{
	m_pHealth->DealDamage(damage);
}

void dae::PlayerController::SetOpponent(PlayerController* pOpponent)
{
	m_pOpponent = pOpponent;
	auto onOpponentDeath{ pOpponent->GetOwner()->GetComponent<HealthComponent>()->GetOnDeath() };
	onOpponentDeath->AddObserver(GetOwner()->GetComponent<PlayerScore>()->GetIncreaseScore());
}

void dae::PlayerController::Reset()
{
	m_pHealth->Heal(m_pHealth->GetMaxValue());
}
