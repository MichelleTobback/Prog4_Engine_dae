#pragma once
#include "Component/Component.h"
#include "Component/Physics/RigidBody2DComponent.h"

namespace dae
{
	class SpriteAtlasComponent;
	class SpriteAnimatorComponent;
	class PepperComponent final : public Component
	{
	public:
		PepperComponent(GameObject* pOwner, SpriteAnimatorComponent* pAnimator, SpriteAtlasComponent* pSpriteAtlas, RigidBody2DComponent* pRigidBody);

		virtual void Awake() override;
		virtual void Update() override;

	private:
		void OnBeginOverlap(const CollisionHit& hit);
		void OnAnimEnd();

		SpriteAnimatorComponent* m_pAnimator;
	};
}