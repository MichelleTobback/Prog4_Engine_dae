//#include <SDL.h>
#include "Input.h"

bool dae::Input::ProcessInput()
{
	bool succes{ m_pKeyboard->ProcessInput() };
	HandleInput();
	return succes;
}

dae::Input::Input()
	: m_pCommandHandler{std::make_unique<InputCommand::ICHandler>()}
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
	m_pCommandHandler->Handle();
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

dae::InputCommand::ICHandler& dae::Input::GetCommandHandler()
{
	return *m_pCommandHandler;
}

void dae::Input::UpdateControllers()
{
	for (auto& pController : m_Controllers)
		pController->Update();
}
