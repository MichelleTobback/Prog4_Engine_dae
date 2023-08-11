#pragma once
#include "Core/Singleton.h"
#include "Input/Controller.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "InputCommand.h"
#include <vector>

namespace dae
{
	class Input final : public Singleton<Input>
	{
	public:
		Input();

		bool ProcessInput();

		Mouse* GetMouse() const;
		Keyboard* GetKeyboard() const;
		Controller* GetController(int index);
		InputCommand::ICHandler& GetCommandHandler();

	private:
		std::vector<std::unique_ptr<Controller>> m_Controllers;
		std::unique_ptr<Keyboard> m_pKeyboard{ nullptr };
		std::unique_ptr<Mouse> m_pMouse{ nullptr };
		std::unique_ptr<InputCommand::ICHandler> m_pCommandHandler;

		void HandleInput();
		void UpdateControllers();
	};
}
