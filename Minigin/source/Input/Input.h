#pragma once
#include "Core/Singleton.h"

#include "Input/Controller.h"
#include "Core/Command.h"
#include "Keyboard.h"

#include <map>

namespace dae
{
	class Input final : public Singleton<Input>
	{
	public:
		struct ControllerActionCommand
		{
			unsigned int ControllerID{};
			Controller::ControllerButton Button{};
			Controller::ControllerButtonState ButtonState{};
		};

		struct KeyboardActionCommand
		{
			Keyboard::KeyCode Key{};
			Keyboard::KeyState State{};
		};

		struct InputActionCommand
		{
			InputActionCommand(const ActionCommand& actionCommand)
				: Action{actionCommand}{}
			ControllerActionCommand Controller{};
			KeyboardActionCommand Keyboard{};
			ActionCommand Action;
			bool IsController{ false };
		};

		Input();

		bool ProcessInput();

		void HandleInput();
		void AddActionCommand(const InputActionCommand& actionCommand);

	private:
		std::vector<InputActionCommand> m_ActionCommands{};
		std::vector<std::unique_ptr<Controller>> m_Controllers;
		std::unique_ptr<Keyboard> m_pKeyboard{ nullptr };

		void HandleCommands();
		void HandleControllerActionCommand(InputActionCommand& action);
		void HandleKeyboardActionCommand(InputActionCommand& action);

		void UpdateControllers();
		void UpdateKeyboard();
	};
}
