#pragma once
#include "Core/Singleton.h"

#include "Input/Controller.h"
#include "Core/Command.h"
#include "Keyboard.h"

#include <glm/glm.hpp>
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

		struct ControllerValueCommand
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

		template <typename T>
		struct InputValueCommand
		{
			InputValueCommand(const ValueCommand<T>& actionCommand)
				: Action{ actionCommand } {}
			ControllerValueCommand Controller{};
			KeyboardActionCommand Keyboard{};
			ValueCommand<T> Action;
			bool IsController{ false };
			T value{}; //only used for triggered input (button, key, ...)
		};

		Input();

		bool ProcessInput();

		void HandleInput();
		void AddActionCommand(const InputActionCommand& actionCommand);
		void AddValueCommand(const InputValueCommand<float>& valueCommand);
		void AddAxisCommand(const InputValueCommand<glm::vec2>& axisCommand);

	private:
		std::vector<InputActionCommand> m_ActionCommands{};
		std::vector<InputValueCommand<float>> m_ValueCommands{};
		std::vector<InputValueCommand<glm::vec2>> m_AxisCommands{};

		std::vector<std::unique_ptr<Controller>> m_Controllers;
		std::unique_ptr<Keyboard> m_pKeyboard{ nullptr };

		void HandleCommands();
		void HandleControllerActionCommand(InputActionCommand& action);
		void HandleKeyboardActionCommand(InputActionCommand& action);
		void HandleControllerValueCommand(InputValueCommand<float>& action);
		void HandleKeyboardValueCommand(InputValueCommand<float>& action);
		void HandleControllerAxisCommand(InputValueCommand<glm::vec2>& action);
		void HandleKeyboardAxisCommand(InputValueCommand<glm::vec2>& action);

		void UpdateControllers();
		void UpdateKeyboard();
	};
}
