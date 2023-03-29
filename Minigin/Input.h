#pragma once
#include "Singleton.h"

#include "Controller.h"
#include "Command.h"

#include <map>
#include <SDL.h>

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
			ActionCommand action;
		};

		struct KeyboardActionCommand
		{
			SDL_Scancode Button{};
			ActionCommand action;
		};

		Input();

		bool ProcessInput();

		void HandleInput();
		void AddControllerActionCommand(const ControllerActionCommand& actionCommand);
		void AddKeyboardActionCommand(const KeyboardActionCommand& actionCommand);

	private:
		std::vector<KeyboardActionCommand> m_KeyboardCommands{};
		std::vector<ControllerActionCommand> m_ControllerCommands{};
		std::vector<std::unique_ptr<Controller>> m_Controllers;

		void HandleKeyboardCommands();
		void HandleControllersCommands();
		void UpdateControllers();
	};
}
