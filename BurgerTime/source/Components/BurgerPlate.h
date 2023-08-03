#pragma once
#include "Component/Component.h"
#include "Component/Physics/RigidBody2DComponent.h"
#include "Scene/Serializable.h"

namespace dae
{
	class BurgerPlate final : public Component
	{
	public:
		BurgerPlate(GameObject* pOwner);
		BurgerPlate(GameObject* pOwner, RigidBody2DComponent* pRigidBody);

		virtual void Awake();

		void SetRigidBody(RigidBody2DComponent* pRigidBody);

	private:
		void OnOverlap(const CollisionHit& hit);

		ColliderComponent* m_pCollider{ nullptr };
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
