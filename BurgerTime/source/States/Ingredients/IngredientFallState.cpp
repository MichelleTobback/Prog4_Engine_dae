#include "IngredientFallState.h"
#include "Components/BurgerIngredient.h"
#include "Scene/GameObject.h"
#include "BurgerTime.h"

void dae::IngredientFallState::OnEnter()
{
    ColliderComponent* pCollider{ GetIngredient().GetRigidBody().GetCollider(0) };
    pCollider->SetTrigger(true);
	pCollider->SetCollisionIgnoreLayer(BurgerTime::ENEMY_COLLISION_LAYER, false);

	GetIngredient().AddReward();

	const float fallSpeed{ 60.f };
	GetIngredient().GetRigidBody().SetVelociy(glm::vec3{ 0.f, fallSpeed, 0.f });

	m_MinHeight = GetIngredient().GetTransform().GetWorldPosition().y + 16.f;
}

dae::State::StatePtr dae::IngredientFallState::OnUpdate()
{
	if (GetIngredient().IsOnPlate())
		return GetIngredient().GetStates().pInPlate.get();
	if (GetIngredient().GetRigidBody().GetVelocity().y <= 1.f)
		return GetIngredient().GetStates().pIdle.get();
    return this;
}

void dae::IngredientFallState::OnExit()
{
	ColliderComponent* pCollider{ GetIngredient().GetRigidBody().GetCollider(0) };
	pCollider->SetTrigger(false);
	pCollider->SetCollisionIgnoreLayer(BurgerTime::ENEMY_COLLISION_LAYER, true);
}

void dae::IngredientFallState::OnBeginOverlap(const CollisionHit& hit)
{
	if (hit.pOtherCollider->GetCollisionLayer() == BurgerTime::LEVEL_COLLISION_LAYER
		&& hit.pCollider == GetIngredient().GetRigidBody().GetCollider(0)
		&& m_MinHeight < GetIngredient().GetTransform().GetWorldPosition().y)
	{
		hit.pCollider->SetTrigger(true);
	}
}

void dae::IngredientFallState::OnEndOverlap(const CollisionHit& hit)
{
	if (hit.pOtherCollider->GetCollisionLayer() == BurgerTime::LEVEL_COLLISION_LAYER
		&& hit.pCollider == GetIngredient().GetRigidBody().GetCollider(0) 
		&& m_MinHeight < GetIngredient().GetTransform().GetWorldPosition().y)
	{
		hit.pCollider->SetTrigger(false);
	}
}
