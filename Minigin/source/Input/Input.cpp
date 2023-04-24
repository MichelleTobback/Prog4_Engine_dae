//#include <SDL.h>
#include "Input.h"

bool dae::Input::ProcessInput()
{
	return m_pKeyboard->ProcessInput();
}

dae::Input::Input()
{
	const int controllers{ 4 };
	for (int i{}; i < controllers; ++i)
	{
		m_Controllers.push_back(std::make_unique<dae::Controller>(i));
	}
	m_pKeyboard = std::make_unique<dae::Keyboard>();
}

void dae::Input::HandleInput()
{
	UpdateKeyboard();
	UpdateControllers();
	HandleCommands();
}

void dae::Input::AddActionCommand(const InputActionCommand& actionCommand)
{
	m_ActionCommands.push_back(actionCommand);
}

void dae::Input::AddValueCommand(const InputValueCommand<float>& valueCommand)
{
	m_ValueCommands.push_back(valueCommand);
}

void dae::Input::AddAxisCommand(const InputValueCommand<glm::vec2>& axisCommand)
{
	m_AxisCommands.push_back(axisCommand);
}

void dae::Input::UpdateControllers()
{
	for (auto& pController : m_Controllers)
		pController->Update();
}

void dae::Input::HandleCommands()
{
	for (auto& action : m_ActionCommands)
	{
		if (action.IsController)
		{
			HandleControllerActionCommand(action);
		}
		else
		{
			HandleKeyboardActionCommand(action);
		}
	}
	for (auto& action : m_ValueCommands)
	{
		if (action.IsController)
		{
			HandleControllerValueCommand(action);
		}
		else
		{
			HandleKeyboardValueCommand(action);
		}
	}
	for (auto& action : m_AxisCommands)
	{
		if (action.IsController)
		{
			HandleControllerAxisCommand(action);
		}
		else
		{
			HandleKeyboardAxisCommand(action);
		}
	}
}

void dae::Input::HandleControllerActionCommand(InputActionCommand& action)
{
	switch (action.Controller.ButtonState)
	{
	case dae::Controller::ControllerButtonState::Down:
	{
		if (m_Controllers[action.Controller.ControllerID]->IsButtonDown(action.Controller.Button))
		{
			action.Action.Execute();
		}
	}
	break;

	case dae::Controller::ControllerButtonState::Released:
	{
		if (m_Controllers[action.Controller.ControllerID]->IsButtonUp(action.Controller.Button))
		{
			action.Action.Execute();
		}
	}
	break;

	case dae::Controller::ControllerButtonState::Pressed:
	{
		if (m_Controllers[action.Controller.ControllerID]->IsButtonPressed(action.Controller.Button))
		{
			action.Action.Execute();
		}
	}
	break;
	}
}

void dae::Input::HandleKeyboardActionCommand(InputActionCommand& action)
{
	switch (action.Keyboard.State)
	{
	case dae::Keyboard::KeyState::Down:
	{
		if (m_pKeyboard->IsKeyDown(action.Keyboard.Key))
		{
			action.Action.Execute();
		}
	}
	break;

	case dae::Keyboard::KeyState::Released:
	{
		if (m_pKeyboard->IsKeyUp(action.Keyboard.Key))
		{
			action.Action.Execute();
		}
	}
	break;

	case dae::Keyboard::KeyState::Pressed:
	{
		if (m_pKeyboard->IsKeyPressed(action.Keyboard.Key))
		{
			action.Action.Execute();
		}
	}
	break;
	}
}

void dae::Input::HandleControllerValueCommand(InputValueCommand<float>&)
{
}

void dae::Input::HandleKeyboardValueCommand(InputValueCommand<float>& action)
{
	switch (action.Keyboard.State)
	{
	case dae::Keyboard::KeyState::Down:
	{
		if (m_pKeyboard->IsKeyDown(action.Keyboard.Key))
		{
			action.Action.SetValue(action.value);
			action.Action.Execute();
		}
	}
	break;

	case dae::Keyboard::KeyState::Released:
	{
		if (m_pKeyboard->IsKeyUp(action.Keyboard.Key))
		{
			action.Action.SetValue(action.value);
			action.Action.Execute();
		}
	}
	break;

	case dae::Keyboard::KeyState::Pressed:
	{
		if (m_pKeyboard->IsKeyPressed(action.Keyboard.Key))
		{
			action.Action.SetValue(action.value);
			action.Action.Execute();
		}
	}
	break;
	}
}

void dae::Input::HandleControllerAxisCommand(InputValueCommand<glm::vec2>&)
{
}

void dae::Input::HandleKeyboardAxisCommand(InputValueCommand<glm::vec2>& action)
{
	switch (action.Keyboard.State)
	{
	case dae::Keyboard::KeyState::Down:
	{
		if (m_pKeyboard->IsKeyDown(action.Keyboard.Key))
		{
			action.Action.SetValue(action.value);
			action.Action.Execute();
		}
	}
	break;

	case dae::Keyboard::KeyState::Released:
	{
		if (m_pKeyboard->IsKeyUp(action.Keyboard.Key))
		{
			action.Action.SetValue(action.value);
			action.Action.Execute();
		}
	}
	break;

	case dae::Keyboard::KeyState::Pressed:
	{
		if (m_pKeyboard->IsKeyPressed(action.Keyboard.Key))
		{
			action.Action.SetValue(action.value);
			action.Action.Execute();
		}
	}
	break;
	}
}

void dae::Input::UpdateKeyboard()
{
	m_pKeyboard->Update();
}
