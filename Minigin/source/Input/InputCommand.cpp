#include "InputCommand.h"
#include "Input.h"

dae::InputCommand::ICActionBinding& dae::InputCommand::ICHandler::AddActionBinding(uint32_t id)
{
    m_ActionCommands[id] = std::make_unique<ICActionBinding>();
    return *m_ActionCommands[id];
}

dae::InputCommand::ICScalarBinding& dae::InputCommand::ICHandler::AddValueBinding(uint32_t id)
{
    m_ValueCommands[id] = std::make_unique<ICScalarBinding>();
    return *m_ValueCommands[id];
}

dae::InputCommand::ICAxisBinding& dae::InputCommand::ICHandler::AddAxisBinding(uint32_t id)
{
    m_AxisCommands[id] = std::make_unique<ICAxisBinding>();
    return *m_AxisCommands[id];
}

std::pair<uint32_t, dae::InputCommand::ICActionBinding*> dae::InputCommand::ICHandler::AddActionBinding()
{
	uint32_t id = FindAvailableID(m_ActionCommands);
	return std::make_pair(id, &AddActionBinding(id));
}

std::pair<uint32_t, dae::InputCommand::ICScalarBinding*> dae::InputCommand::ICHandler::AddValueBinding()
{
	uint32_t id{ FindAvailableID(m_ValueCommands) };
	return std::make_pair(id, &AddValueBinding(id));
}

std::pair <uint32_t, dae::InputCommand::ICAxisBinding*> dae::InputCommand::ICHandler::AddAxisBinding()
{
	uint32_t id{ FindAvailableID(m_AxisCommands) };
	return std::make_pair(id, &AddAxisBinding(id));
}

void dae::InputCommand::ICHandler::BindActionCommand(uint32_t id, const std::shared_ptr<ActionCommand>& actionCommand)
{
    auto it{ m_ActionCommands.find(id) };
    assert(it != m_ActionCommands.end() && "No action exists with id");
    it->second->pActions.push_back(actionCommand);
}

void dae::InputCommand::ICHandler::BindValueCommand(uint32_t id, const std::shared_ptr<ScalarCommand>& valueBinding)
{
    auto it{ m_ValueCommands.find(id) };
    assert(it != m_ValueCommands.end() && "No action exists with id");
    it->second->pActions.push_back(valueBinding);
}

void dae::InputCommand::ICHandler::BindAxisCommand(uint32_t id, const std::shared_ptr<AxisCommand>& axisBinding)
{
    auto it{ m_AxisCommands.find(id) };
    assert(it != m_AxisCommands.end() && "No action exists with id");
    it->second->pActions.push_back(axisBinding);
}

void dae::InputCommand::ICHandler::UnbindActionCommand(uint32_t id, const std::shared_ptr<ActionCommand>& actionCommand)
{
    auto& commands{ m_ActionCommands[id]->pActions };
    commands.erase(std::remove(commands.begin(), commands.end(), actionCommand), commands.end());
}

void dae::InputCommand::ICHandler::UnbindValueCommand(uint32_t id, const std::shared_ptr<ScalarCommand>& valueBinding)
{
    auto& commands{ m_ValueCommands[id]->pActions };
    commands.erase(std::remove(commands.begin(), commands.end(), valueBinding), commands.end());
}

void dae::InputCommand::ICHandler::UnbindAxisCommand(uint32_t id, const std::shared_ptr<AxisCommand>& axisBinding)
{
    auto& commands{ m_AxisCommands[id]->pActions };
    commands.erase(std::remove(commands.begin(), commands.end(), axisBinding), commands.end());
}

void dae::InputCommand::ICHandler::Handle()
{
	Input& input{ Input::GetInstance() };
	Mouse* pMouse{ input.GetMouse() };
	Keyboard* pKeyboard{ input.GetKeyboard() };

	{
		for (auto& [id, action] : m_ActionCommands)
		{
			bool triggered{ false };
			if (BitFlag::IsSet(action->deviceBinding.flags, ICFlag::ControllerButton))
			{
				Controller* pController{ input.GetController(action->deviceBinding.controller.ControllerID) };
				triggered = HandleControllerActionCommand(action->deviceBinding.controller, pController);
			}
			if (!triggered && BitFlag::IsSet(action->deviceBinding.flags, ICFlag::Keyboard))
			{
				triggered = HandleKeyboardActionCommand(action->deviceBinding.keyboard, pKeyboard);
			}
			if (!triggered && BitFlag::IsSet(action->deviceBinding.flags, ICFlag::MouseButton))
			{
				triggered = HandleMouseActionCommand(action->deviceBinding.mouse, pMouse);
			}
			if (triggered)
			{
				for (auto& command : action->pActions)
				{
					command->Execute();
				}
			}
		}
	}
	{
		for (auto& [id, action] : m_ValueCommands)
		{
			for (auto& binding : action->deviceBindings)
			{
				bool triggered{ false };
				if (BitFlag::IsSet(binding.first.flags, ICFlag::ControllerButton))
				{
					Controller* pController{ input.GetController(binding.first.controller.ControllerID) };
					triggered = HandleControllerValueCommand(binding.first.controller, pController);
				}
				if (!triggered && BitFlag::IsSet(binding.first.flags, ICFlag::Keyboard))
				{
					triggered = HandleKeyboardValueCommand(binding.first.keyboard, pKeyboard);
				}
				if (triggered)
				{
					for (auto& command : action->pActions)
					{
						command->SetValue(binding.second);
						command->Execute();
					}
				}
			}
		}
	}
	{
		for (auto& [id, action] : m_AxisCommands)
		{
			for (auto& binding : action->deviceBindings)
			{
				bool triggered{ false };
				if (BitFlag::IsSet(binding.first.flags, ICFlag::ControllerThumbStick) ||
					BitFlag::IsSet(binding.first.flags, ICFlag::ControllerButton))
				{
					Controller* pController{ input.GetController(binding.first.controller.ControllerID) };
					triggered = HandleControllerAxisCommand(binding.first.controller, pController);
				}
				if (!triggered && BitFlag::IsSet(binding.first.flags, ICFlag::Keyboard))
				{
					triggered = HandleKeyboardAxisCommand(binding.first.keyboard, pKeyboard);
				}
				if (BitFlag::IsSet(binding.first.flags, ICFlag::MouseMoved))
				{
					if (pMouse->MovedThisFrame())
					{
						for (auto& command : action->pActions)
						{
							command->SetValue(pMouse->GetMouseDeltaPos());
							command->Execute();
						}
					}
				}
				if (triggered)
				{
					for (auto& command : action->pActions)
					{
						command->SetValue(binding.second);
						command->Execute();
					}
				}
			}
		}
	}
}

bool dae::InputCommand::ICHandler::HandleControllerActionCommand(ICController& device, Controller* pController)
{
	switch (device.ButtonState)
	{
	case dae::Controller::ControllerButtonState::Down:
	{
		return (pController->IsButtonDown(device.Button));
	}
	break;

	case dae::Controller::ControllerButtonState::Released:
	{
		return (pController->IsButtonUp(device.Button));
	}
	break;

	case dae::Controller::ControllerButtonState::Pressed:
	{
		return (pController->IsButtonPressed(device.Button));
	}
	break;
	}
	return false;
}

bool dae::InputCommand::ICHandler::HandleKeyboardActionCommand(ICKeyboard& device, Keyboard* pKeyboard)
{
	switch (device.State)
	{
	case dae::Keyboard::KeyState::Down:
	{
		return (pKeyboard->IsKeyDown(device.Key));
	}
	break;

	case dae::Keyboard::KeyState::Released:
	{
		return (pKeyboard->IsKeyUp(device.Key));
	}
	break;

	case dae::Keyboard::KeyState::Pressed:
	{
		return (pKeyboard->IsKeyPressed(device.Key));
	}
	break;
	}
	return false;
}

bool dae::InputCommand::ICHandler::HandleMouseActionCommand(ICMouse& device, Mouse* pMouse)
{
	switch (device.state)
	{
	case dae::Mouse::MouseButtonState::Down:
	{
		return (pMouse->IsButtonDown(device.button));
	}
	break;

	case dae::Mouse::MouseButtonState::Pressed:
	{
		return (pMouse->IsButtonPressed(device.button));
	}
	break;

	case dae::Mouse::MouseButtonState::Released:
	{
		return (pMouse->IsButtonReleased(device.button));
	}
	break;
	}
	return false;
}

bool dae::InputCommand::ICHandler::HandleControllerValueCommand(ICController& device, Controller* pController)
{
	switch (device.ButtonState)
	{
	case dae::Controller::ControllerButtonState::Down:
	{
		return (pController->IsButtonDown(device.Button));
	}
	break;

	case dae::Controller::ControllerButtonState::Released:
	{
		return (pController->IsButtonUp(device.Button));
	}
	break;

	case dae::Controller::ControllerButtonState::Pressed:
	{
		return (pController->IsButtonPressed(device.Button));
	}
	break;
	}
	return false;
}

bool dae::InputCommand::ICHandler::HandleKeyboardValueCommand(ICKeyboard& device, Keyboard* pKeyboard)
{
	switch (device.State)
	{
	case dae::Keyboard::KeyState::Down:
	{
		return (pKeyboard->IsKeyDown(device.Key));
	}
	break;

	case dae::Keyboard::KeyState::Released:
	{
		return (pKeyboard->IsKeyUp(device.Key));
	}
	break;

	case dae::Keyboard::KeyState::Pressed:
	{
		return (pKeyboard->IsKeyPressed(device.Key));
	}
	break;
	}
	return false;
}

bool dae::InputCommand::ICHandler::HandleControllerAxisCommand(ICController& device, Controller* pController)
{
	switch (device.ButtonState)
	{
	case dae::Controller::ControllerButtonState::Down:
	{
		return (pController->IsButtonDown(device.Button));
	}
	break;

	case dae::Controller::ControllerButtonState::Released:
	{
		return (pController->IsButtonUp(device.Button));
	}
	break;

	case dae::Controller::ControllerButtonState::Pressed:
	{
		return (pController->IsButtonPressed(device.Button));
	}
	break;
	}
	return false;
}

bool dae::InputCommand::ICHandler::HandleKeyboardAxisCommand(ICKeyboard& device, Keyboard* pKeyboard)
{
	switch (device.State)
	{
	case dae::Keyboard::KeyState::Down:
	{
		return (pKeyboard->IsKeyDown(device.Key));
	}
	break;

	case dae::Keyboard::KeyState::Released:
	{
		return (pKeyboard->IsKeyUp(device.Key));
	}
	break;

	case dae::Keyboard::KeyState::Pressed:
	{
		return (pKeyboard->IsKeyPressed(device.Key));
	}
	break;
	}
	return false;
}
