#pragma once
#include "Component/Component.h"

namespace dae
{
	class RigidBody2DComponent;
	class Bullet : public Component
	{
	public:
		Bullet(GameObject* pOwner);
		~Bullet() = default;

		virtual void Awake() override;

	private:
		RigidBody2DComponent* m_pRigidBody{nullptr};
	};
}