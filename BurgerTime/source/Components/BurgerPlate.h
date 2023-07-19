#pragma once
#include "Component/Component.h"
#include "Component/Physics/RigidBody2DComponent.h"
#include "Scene/Serializable.h"

namespace dae
{
	constexpr CollisionLayer PLATE_COLLISION_LAYER = CollisionLayer::Five;

	class BurgerPlate final : public Component
	{
	public:
		BurgerPlate(GameObject* pOwner);
		BurgerPlate(GameObject* pOwner, RigidBody2DComponent* pRigidBody);

		void SetRigidBody(RigidBody2DComponent* pRigidBody);

	private:
		void OnOverlap(const CollisionHit& hit);
	};

	class PlateComponentSerializer final : public Serializable
	{
	public:
		PlateComponentSerializer();
		virtual ~PlateComponentSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;
	};
}
