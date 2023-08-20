#pragma once
#include <memory>

namespace dae
{
	class Controller final
	{
	public:
		enum class ControllerButton
		{
			DPadUp			= 0x0001,
			DPadDown		= 0x0002,
			DPadLeft		= 0x0004,
			DPadRight		= 0x0008,
			Start			= 0x0010,
			Back			= 0x0020,
			LeftThumbStick	= 0x0040,
			RightThumbStick	= 0x0080,
			LeftShoulder	= 0x0100,
			RightShoulder	= 0x0200,
			ButtonA			= 0x1000,
			ButtonB			= 0x2000,
			ButtonX			= 0x4000,
			ButtonY			= 0x8000
		};

		enum class ControllerButtonState
		{
			Down, Released, Pressed
		};

		explicit Controller(int controllerIndex);
		~Controller();

		void Update();

		bool IsButtonDown(ControllerButton button) const;
		bool IsButtonUp(ControllerButton button) const;
		bool IsButtonPressed(ControllerButton button) const;

		bool IsConnected() const;

	private:
		class ControllerImpl;
		std::unique_ptr<ControllerImpl> m_pImpl{};

	};
}