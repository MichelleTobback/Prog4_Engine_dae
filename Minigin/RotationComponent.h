#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class TransformComponent;
	class RotateComponent : public Component
	{
	public:
		RotateComponent(GameObject* pGameObject, float rotationSpeed);
		virtual ~RotateComponent() {}

		RotateComponent(const RotateComponent& other) = delete;
		RotateComponent(RotateComponent&& other) = delete;
		RotateComponent& operator=(const RotateComponent& other) = delete;
		RotateComponent& operator=(RotateComponent&& other) = delete;

		virtual void Update() override;

	private:
		float m_RotationSpeed;
		TransformComponent* m_pTransformComponent{ nullptr };
	};

}