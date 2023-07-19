#pragma once
#include "Component/Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(GameObject* pOwner, float aspectRatio);
		CameraComponent(GameObject* pOwner, const glm::vec2& size);
		virtual ~CameraComponent() = default;

		CameraComponent(const CameraComponent& other) = delete;
		CameraComponent(CameraComponent&& other) = delete;
		CameraComponent& operator=(const CameraComponent& other) = delete;
		CameraComponent& operator=(CameraComponent&& other) = delete;

		inline void SetSize(const glm::vec2& size) { m_Size = size; }
		inline const glm::vec2& GetSize() const { return m_Size; }

		void SetCurrent();

	private:
		friend class Renderer;
		glm::vec2 m_Size;
		bool m_Current{ false };
	};
}