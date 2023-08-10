#pragma once
#include "Component/Component.h"
#include "Component/Physics/RigidBody2DComponent.h"

namespace dae
{
	class BTTileGridComponent;
	class BTGameMode;
	class SnapToGridComponent final : public Component
	{
	public:
		SnapToGridComponent(GameObject* pOwner, RigidBody2DComponent* pRigidBody);

		virtual void Awake() override;
		virtual void Update() override;

	private:
		RigidBody2DComponent* m_pRigidBody{ nullptr };
		BTTileGridComponent* m_pGrid{ nullptr };
	};
}