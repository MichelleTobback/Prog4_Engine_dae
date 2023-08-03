#pragma once
#include <memory>
#include <glm/glm.hpp>

namespace dae
{
	class Mouse
	{
	public:
		enum class MouseButton
		{
			None, Left, Right, Middle
		};
		enum class MouseButtonState
		{
			None, Down, Released, Pressed
		};

		enum class MouseFlags
		{
			None = 0, 
			LeftButtonDown = 1, RightButtonDown = 2, MiddleButtonDown = 4,
			MovedThisFrame = 8
		};

		Mouse();
		~Mouse();

		virtual void Update();

		virtual bool IsButtonDown(MouseButton button) const;
		virtual bool IsButtonUp(MouseButton button) const;
		virtual bool IsButtonPressed(MouseButton button) const;
		virtual bool IsButtonReleased(MouseButton button) const;
		virtual bool MovedThisFrame() const;

		virtual glm::vec2 GetMousePos() const;
		virtual glm::vec2 GetMouseDeltaPos() const;

		static MouseFlags GetButtonDownFlag(MouseButton button);

	private:
		class MouseImpl;
		std::unique_ptr<MouseImpl> m_pImpl{};
	};
}