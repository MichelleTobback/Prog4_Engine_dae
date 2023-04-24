#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

#include "Controller.h"

//================================================================
//				ControllerImpl
//================================================================

class dae::Controller::ControllerImpl final
{
public:

	ControllerImpl(int controllerIndex)
		: m_ControllerIndex{ controllerIndex }
	{
		ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	}

	void Update()
	{
		CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		XInputGetState(m_ControllerIndex, &m_CurrentState);

		auto buttonChanges{ m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons };
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
	}

	bool IsButtonDownThisFrame(unsigned int button) const
	{
		return m_ButtonsPressedThisFrame & button;
	}

	bool IsButtonUpThisFrame(unsigned int button) const
	{
		return m_ButtonsReleasedThisFrame & button;
	}

	bool IsButtonPressed(unsigned int button) const
	{
		return m_CurrentState.Gamepad.wButtons & button;
	}

private:
	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};

	WORD m_ButtonsPressedThisFrame{};
	WORD m_ButtonsReleasedThisFrame{};

	int m_ControllerIndex{};
};

//================================================================
//				Controller
//================================================================

dae::Controller::Controller(int controllerIndex)
{
	m_pImpl = std::make_unique<ControllerImpl>(controllerIndex);
}

dae::Controller::~Controller()
{
}

void dae::Controller::Update()
{
	m_pImpl->Update();
}

bool dae::Controller::IsButtonDown(ControllerButton button) const
{
	return m_pImpl->IsButtonDownThisFrame(static_cast<unsigned int>(button));
}

bool dae::Controller::IsButtonUp(ControllerButton button) const
{
	return m_pImpl->IsButtonUpThisFrame(static_cast<unsigned int>(button));
}

bool dae::Controller::IsButtonPressed(ControllerButton button) const
{
	return m_pImpl->IsButtonPressed(static_cast<unsigned int>(button));
}

