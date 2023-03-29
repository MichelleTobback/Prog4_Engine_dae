//#include <SDL.h>
#include "Input.h"
#include <backends/imgui_impl_sdl2.h>

bool dae::Input::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		// etc...
		// 
		//ImGui Input
		ImGui_ImplSDL2_ProcessEvent(&e);
	}



	return true;
}

dae::Input::Input()
{
	const int controllers{ 4 };
	for (int i{}; i < controllers; ++i)
	{
		m_Controllers.push_back(std::make_unique<dae::Controller>(i));
	}
}

void dae::Input::HandleInput()
{
	UpdateControllers();
	HandleControllersCommands();
	HandleKeyboardCommands();
}

void dae::Input::AddControllerActionCommand(const ControllerActionCommand& actionCommand)
{
	m_ControllerCommands.push_back(actionCommand);
}

void dae::Input::AddKeyboardActionCommand(const KeyboardActionCommand& actionCommand)
{
	m_KeyboardCommands.push_back(actionCommand);
}

void dae::Input::HandleKeyboardCommands()
{
	auto keys{ SDL_GetKeyboardState(NULL) };
	for (auto& keyboardCommand : m_KeyboardCommands)
	{
		int buttonCode{ static_cast<int>(keyboardCommand.Button) };
		if (keys[buttonCode])
		{
			keyboardCommand.action.Execute();
		}
	}
}

void dae::Input::UpdateControllers()
{
	for (auto& pController : m_Controllers)
		pController->Update();
}

void dae::Input::HandleControllersCommands()
{
	for (auto& controllerCommand : m_ControllerCommands)
	{
		switch (controllerCommand.ButtonState)
		{
		case dae::Controller::ControllerButtonState::Down:
		{
			if (m_Controllers[controllerCommand.ControllerID]->IsButtonDown(controllerCommand.Button))
			{
				controllerCommand.action.Execute();
			}
		}
		break;

		case dae::Controller::ControllerButtonState::Released:
		{
			if (m_Controllers[controllerCommand.ControllerID]->IsButtonUp(controllerCommand.Button))
			{
				controllerCommand.action.Execute();
			}
		}
		break;

		case dae::Controller::ControllerButtonState::Pressed:
		{
			if (m_Controllers[controllerCommand.ControllerID]->IsButtonPressed(controllerCommand.Button))
			{
				controllerCommand.action.Execute();
			}
		}
		break;
		}
	}
}
