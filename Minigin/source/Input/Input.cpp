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
	m_pMouse = std::make_unique<dae::Mouse>();
}

void dae::Input::HandleInput()
{
	m_pMouse->Update();
	m_pKeyboard->Update();
	UpdateControllers();
	HandleCommands();
}

void dae::Input::AddActionCommand(const InputActionCommand& actionCommand)
{
	m_ActionCommands.push_back(actionCommand);
}

void dae::Input::AddValueCommand(const InputScalarCommand& valueCommand)
{
	m_ValueCommands.push_back(valueCommand);
}

void dae::Input::AddAxisCommand(const InputAxisCommand& axisCommand)
{
	m_AxisCommands.push_back(axisCommand);
}

dae::Mouse* dae::Input::GetMouse() const
{
	return m_pMouse.get();
}

dae::Keyboard* dae::Input::GetKeyboard() const
{
	return m_pKeyboard.get();
}

dae::Controller* dae::Input::GetController(int index)
{
	assert(size_t(index) < m_Controllers.size() && "no valid controller index!");
	return m_Controllers[index].get();
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
		if (BitFlag::IsSet(action.flags, InputCommandFlag::ControllerButton))
		{
			HandleControllerActionCommand(action);
		}
		else if (BitFlag::IsSet(action.flags, InputCommandFlag::Keyboard))
		{
			HandleKeyboardActionCommand(action);
		}
		else if (BitFlag::IsSet(action.flags, InputCommandFlag::MouseButton))
		{
			HandleMouseActionCommand(action);
		}
	}
	for (auto& action : m_ValueCommands)
	{
		if (BitFlag::IsSet(action.flags, InputCommandFlag::ControllerButton))
		{
			HandleControllerValueCommand(action);
		}
		else if (BitFlag::IsSet(action.flags, InputCommandFlag::Keyboard))
		{
			HandleKeyboardValueCommand(action);
		}
	}
	for (auto& action : m_AxisCommands)
	{
		if (BitFlag::IsSet(action.flags, InputCommandFlag::ControllerThumbStick))
		{
			HandleControllerAxisCommand(action);
		}
		else if (BitFlag::IsSet(action.flags, InputCommandFlag::Keyboard))
		{
			HandleKeyboardAxisCommand(action);
		}
		else if (BitFlag::IsSet(action.flags, InputCommandFlag::MouseMoved))
		{
			if (m_pMouse->MovedThisFrame())
			{
				action.Action.SetValue(m_pMouse->GetMouseDeltaPos());
				action.Action.Execute();
			}
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

void dae::Input::HandleMouseActionCommand(InputActionCommand& action)
{
	switch (action.Mouse.state)
	{
	case dae::Mouse::MouseButtonState::Down:
	{
		if (m_pMouse->IsButtonDown(action.Mouse.button))
		{
			action.Action.Execute();
		}
	}
	break;

	case dae::Mouse::MouseButtonState::Pressed:
	{
		if (m_pMouse->IsButtonPressed(action.Mouse.button))
		{
			action.Action.Execute();
		}
	}
	break;

	case dae::Mouse::MouseButtonState::Released:
	{
		if (m_pMouse->IsButtonReleased(action.Mouse.button))
		{
			action.Action.Execute();
		}
	}
	break;
	}
}

void dae::Input::HandleControllerValueCommand(InputScalarCommand&)
{
}

void dae::Input::HandleKeyboardValueCommand(InputScalarCommand& action)
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

void dae::Input::HandleControllerAxisCommand(InputAxisCommand&)
{
}

void dae::Input::HandleKeyboardAxisCommand(InputAxisCommand& action)
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
