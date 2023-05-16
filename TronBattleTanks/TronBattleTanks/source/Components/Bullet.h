#pragma once
#include "Component/Component.h"

namespace dae
{
	class MovementComponent;
	class Bullet : public Component
	{
	public:
		Bullet(GameObject* pOwner);
		~Bullet() = default;

		virtual void Update() override;

	private:
		MovementComponent* m_pMovement{nullptr};
	};
}