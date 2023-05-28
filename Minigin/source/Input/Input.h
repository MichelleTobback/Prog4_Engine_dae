#pragma once
#include "Core/Singleton.h"

#include "Input/Controller.h"
#include "Core/Command.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Core/BitFlag.h"

#include <glm/glm.hpp>
#include <map>

namespace dae
{
	class Input final : public Singleton<Input>
	{
	public:
		enum class InputCommandFlag
		{
			None = 0, 
			Keyboard = 1, ControllerButton = 2, ControllerThumbStick = 4, MouseButton = 8, MouseMoved = 16
		};

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

		struct MouseActionCommand
		{
			Mouse::MouseButton button{};
			Mouse::MouseButtonState state{};
		};

		struct InputActionCommand
		{
			InputActionCommand(const ActionCommand& actionCommand)
				: Action{actionCommand}{}
			ControllerActionCommand Controller{};
			KeyboardActionCommand Keyboard{};
			MouseActionCommand Mouse{};
			ActionCommand Action;
			InputCommandFlag flags{ InputCommandFlag::None };
		};

		template <typename T>
		struct InputValueCommand
		{
			InputValueCommand(const ValueCommand<T>& actionCommand)
				: Action{ actionCommand } {}
			ControllerValueCommand Controller{};
			KeyboardActionCommand Keyboard{};
			ValueCommand<T> Action;
			InputCommandFlag flags{ InputCommandFlag::None };
			T value{}; //only used for triggered input (button, key, ...)
		};
		using InputScalarCommand = InputValueCommand<float>;
		using InputAxisCommand = InputValueCommand<glm::vec2>;

		Input();

		bool ProcessInput();

		void HandleInput();
		void AddActionCommand(const InputActionCommand& actionCommand);
		void AddValueCommand(const InputScalarCommand& valueCommand);
		void AddAxisCommand(const InputAxisCommand& axisCommand);

		Mouse* GetMouse() const;
		Keyboard* GetKeyboard() const;
		Controller* GetController(int index);

	private:
		std::vector<InputActionCommand> m_ActionCommands{};
		std::vector<InputScalarCommand> m_ValueCommands{};
		std::vector<InputAxisCommand> m_AxisCommands{};

		std::vector<std::unique_ptr<Controller>> m_Controllers;
		std::unique_ptr<Keyboard> m_pKeyboard{ nullptr };
		std::unique_ptr<Mouse> m_pMouse{ nullptr };

		void HandleCommands();
		void HandleControllerActionCommand(InputActionCommand& action);
		void HandleKeyboardActionCommand(InputActionCommand& action);
		void HandleMouseActionCommand(InputActionCommand& action);
		void HandleControllerValueCommand(InputScalarCommand& action);
		void HandleKeyboardValueCommand(InputScalarCommand& action);
		void HandleControllerAxisCommand(InputAxisCommand& action);
		void HandleKeyboardAxisCommand(InputAxisCommand& action);

		void UpdateControllers();
	};
}
