#include "EnemyControlledState.h"
#include "Scene/GameObject.h"
#include "Component/InputHandlerComponent.h"
#include "Components/EnemyComponent.h"
#include "Scene/Scene.h"
#include "BurgerTime.h"
#include "Input/Input.h"

dae::EnemyControlledState::EnemyControlledState(EnemyComponent* pEnemy)
    : EnemyState(pEnemy)
{
    m_pInput = GetEnemy()->GetScene()->Instantiate(GetEnemy()->GetOwner())->AddComponent<InputHandlerComponent>();
	bool isSecondController{ Input::GetInstance().GetController(1)->IsConnected() };
	auto& commandBinding{ m_pInput->GetHandler().AddAxisBinding(static_cast<uint32_t>(BurgerTime::InputID::Move))};
	dae::InputCommand::ICDevices binding{};
	BitFlag::Set(binding.flags, InputCommand::ICFlag::ControllerButton, true);
	binding.controller.Button = dae::Controller::ControllerButton::DPadLeft;
	binding.controller.ButtonState = dae::Controller::ControllerButtonState::Pressed;
	binding.controller.ControllerID = isSecondController;
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

	m_pInput->GetHandler().BindAxisCommand(
		static_cast<uint32_t>(BurgerTime::InputID::Move), 
		InputCommand::AxisCommand::Create(GetCharacter().pController, &CharacterController2D::Move));
}

void dae::EnemyControlledState::OnEnter()
{
    m_pInput->GetOwner()->SetActive(true);
}

dae::State::StatePtr dae::EnemyControlledState::OnUpdate()
{
    if (GetEnemy()->GetCharacter()->Get().pHealth->GetValue() == 0)
        return GetEnemy()->GetStates().pDieState.get();
    if (GetEnemy()->GetOverlappedBurger() && GetEnemy()->GetOverlappedBurger()->GetVelocity().y > 0.1f)
        return GetEnemy()->GetStates().pFallState.get();
    if (GetEnemy()->GetOverlappedPlayer())
        return GetEnemy()->GetStates().pAttackState.get();

    return this;
}

void dae::EnemyControlledState::OnExit()
{
    m_pInput->GetOwner()->SetActive(false);
}
