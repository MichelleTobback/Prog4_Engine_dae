#include "TextInputComponent.h"

dae::TextInputComponent::TextInputComponent(GameObject* pOwner, TextComponent* pText, InputHandlerComponent* pInput)
	: Component(pOwner), m_pTextComponent{pText}
{
	InputCommand::ICHandler& input{ pInput->GetHandler() };
	CreateAddCharInputBindings(input);

	auto& commandBinding{ input.AddActionBinding(1) };
	dae::InputCommand::ICDevices& binding{ commandBinding.deviceBinding };
	BitFlag::Set(binding.flags, InputCommand::ICFlag::Keyboard, true);
	binding.keyboard.Key = Keyboard::KeyCode::Comma;
	binding.keyboard.State = Keyboard::KeyState::Released;
	input.BindActionCommand(1, ActionCommand::Create(this, &TextInputComponent::RemoveChar));
}

void dae::TextInputComponent::AddChar(float value)
{
	std::string text{ m_pTextComponent->GetText() };
	char character{ static_cast<char>(value) };
	text.push_back(character);
	m_pTextComponent->SetText(text);
}

void dae::TextInputComponent::RemoveChar()
{
	std::string text{ m_pTextComponent->GetText() };
	if (text.empty())
		return;
	text.pop_back();
	m_pTextComponent->SetText(text);
}

void dae::TextInputComponent::CreateAddCharInputBindings(InputCommand::ICHandler& input)
{
	auto& commandBinding{ input.AddValueBinding(0) };
	dae::InputCommand::ICDevices binding{};
	BitFlag::Set(binding.flags, InputCommand::ICFlag::Keyboard, true);
	binding.keyboard.State = Keyboard::KeyState::Released;

	binding.keyboard.Key = Keyboard::KeyCode::Q;
	commandBinding.Add(static_cast<float>('q'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::W;
	commandBinding.Add(static_cast<float>('w'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::E;
	commandBinding.Add(static_cast<float>('e'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::R;
	commandBinding.Add(static_cast<float>('r'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::T;
	commandBinding.Add(static_cast<float>('t'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::Y;
	commandBinding.Add(static_cast<float>('y'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::U;
	commandBinding.Add(static_cast<float>('u'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::I;
	commandBinding.Add(static_cast<float>('i'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::O;
	commandBinding.Add(static_cast<float>('o'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::P;
	commandBinding.Add(static_cast<float>('p'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::A;
	commandBinding.Add(static_cast<float>('a'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::S;
	commandBinding.Add(static_cast<float>('s'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::D;
	commandBinding.Add(static_cast<float>('d'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::F;
	commandBinding.Add(static_cast<float>('f'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::G;
	commandBinding.Add(static_cast<float>('g'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::H;
	commandBinding.Add(static_cast<float>('h'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::J;
	commandBinding.Add(static_cast<float>('j'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::K;
	commandBinding.Add(static_cast<float>('k'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::L;
	commandBinding.Add(static_cast<float>('l'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::Z;
	commandBinding.Add(static_cast<float>('z'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::X;
	commandBinding.Add(static_cast<float>('x'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::C;
	commandBinding.Add(static_cast<float>('c'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::V;
	commandBinding.Add(static_cast<float>('v'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::B;
	commandBinding.Add(static_cast<float>('b'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::N;
	commandBinding.Add(static_cast<float>('n'), binding);

	binding.keyboard.Key = Keyboard::KeyCode::M;
	commandBinding.Add(static_cast<float>('m'), binding);

	input.BindValueCommand(0, InputCommand::ScalarCommand::Create(this, &TextInputComponent::AddChar));
}
