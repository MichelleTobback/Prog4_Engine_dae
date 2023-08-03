#include "BurgerPlate.h"
#include "Scene/GameObject.h"
#include "BurgerIngredient.h"
#include "BurgerTime.h"

dae::BurgerPlate::BurgerPlate(GameObject* pOwner)
	: Component(pOwner)
{
}

dae::BurgerPlate::BurgerPlate(GameObject* pOwner, RigidBody2DComponent* pRigidBody)
	: Component(pOwner)
{
	SetRigidBody(pRigidBody);
}

void dae::BurgerPlate::Awake()
{
	m_pCollider = GetOwner()->GetComponent<BoxCollider2DComponent>();
}

void dae::BurgerPlate::SetRigidBody(RigidBody2DComponent* pRigidBody)
{
	pRigidBody->GetOnBeginOverlap() += std::bind(&BurgerPlate::OnOverlap, this, std::placeholders::_1);
}

void dae::BurgerPlate::OnOverlap(const CollisionHit& hit)
{
	if (hit.pCollider == m_pCollider &&
		hit.pOtherCollider->GetCollisionLayer() == BurgerTime::INGREDIENT_COLLISION_LAYER)
	{
		BurgerIngredient* pIngredient{ BurgerIngredient::GetFromCollider(hit.pOtherCollider) };
		pIngredient->PlateEnter();
	}
}
//===============================//
//		Serializable			 //
//===============================//

dae::PlateComponentSerializer::PlateComponentSerializer()
	: Serializable{ Serializable::Create<BurgerPlate>() }
{
}

void dae::PlateComponentSerializer::Serialize(BinaryWriter& out, Component* pComponent) const
{
	BurgerPlate* pPlate{ pComponent->As<BurgerPlate>() };
	uint64_t rigidUuid{ pPlate->GetOwner()->GetParent()->GetUUID() };
	out.Write(rigidUuid);
}

dae::Component* dae::PlateComponentSerializer::Deserialize(DeserializeParams& params)
{
	if (params.pGameObject)
	{
		uint64_t rigidUuid{};
		params.in.Read(rigidUuid);

		BurgerPlate* pInstance{ params.pGameObject->AddComponent<BurgerPlate>() };

		auto waitForRigidBody{ std::make_unique<OnGameObjectDeserialized>(UUID(rigidUuid), params.pGameObject->GetScene(),
			std::vector<std::string>{Component::GetName<RigidBody2DComponent>()},
			[pInstance](GameObject* pObj)->bool
			{
				if (pObj->HasComponent<RigidBody2DComponent>())
				{
					pInstance->SetRigidBody(pObj->GetComponent<RigidBody2DComponent>());
					return true;
				}
				return false;
			}) };

		params.outOnDeserialized.push(std::move(waitForRigidBody));

		return pInstance;
	}
	return nullptr;
}
