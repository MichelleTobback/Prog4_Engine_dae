#pragma once
#include "Component.h"

#include <glm/glm.hpp>

namespace dae
{
	class TransformComponent final : public Component
	{
	public:
		TransformComponent(GameObject* pOwner);
		virtual ~TransformComponent() override = default;

		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition() { return m_LocalPosition; }

		void SetLocalPosition(const glm::vec3 position);

	private:
		void RecalculateWorldPosition();
		void SetPositionDirty();

		glm::vec3 m_LocalPosition{};
		glm::vec3 m_WorldPosition{};

		bool m_IsPositionDirty{ false };

		friend class GameObject;
	};
}
