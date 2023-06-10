#pragma once
#include "Component.h"

#include <glm/glm.hpp>

//TODO - Make wrapper class so these includes can be moved to a cpp file
#pragma warning(push)
#pragma warning(disable: 4201)
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#pragma warning(pop)

namespace dae
{
	class TransformComponent final : public Component
	{
	public:
		enum class TransformFlag
		{
			None = 0,

			Position = 1 << 0,
			Rotation = 1 << 1,
			Scale = 1 << 2,

			Forward = 1 << 3
		};

		TransformComponent(GameObject* pOwner);
		virtual ~TransformComponent() override = default;

		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
		const glm::quat& GetWorldRotation();
		const glm::quat& GetLocalRotationQuat() const { return m_LocalRotQ; }
		float GetWorldRotationAngle();
		float GetLocalRotation() const { return m_LocalRotation; }


		void SetLocalPosition(const glm::vec3& position);
		void SetLocalRotation(float rotation);

		void Translate(const glm::vec3& translation);
		void Rotate(float rotation);

		const glm::vec3& GetForward();

	protected:
		void SetChildrenDirty(TransformFlag flag);

	private:
		void RecalculateWorldPosition();
		void RecalculateWorldRotation();
		void SetDirty(TransformFlag flag, bool isDirty);
		bool IsDirty(TransformFlag flag) const;

		void RecalculateForward();

		glm::vec3 m_LocalPosition{};
		glm::vec3 m_WorldPosition{};
		float m_LocalRotation{};
		glm::quat m_LocalRotQ;
		glm::quat m_WorldRotQ;

		glm::vec3 m_Forward{1.f, 0.f, 0.f};

		int m_DirtyFlags{ 1 | 2 | 3 };

		friend class GameObject;
	};
}
