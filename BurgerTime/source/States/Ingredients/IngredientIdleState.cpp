#include "IngredientIdleState.h"
#include "Components/BurgerIngredient.h"
#include "Scene/GameObject.h"
#include "BurgerTime.h"

void dae::IngredientIdleState::OnEnter()
{
	m_OverlappedYPos = GetIngredient().GetTransform().GetWorldPosition().y + 16.f;

	const std::vector<ColliderComponent*>& pColliders{ GetIngredient().GetRigidBody().GetColliders() };
	pColliders[0]->SetTrigger(false);

	m_AllFlags = 0;
	m_WalkedOnTilesFlags = 0;
	for (size_t i{ 1 }; i < pColliders.size(); ++i)
	{
		TransformComponent& transform{ pColliders[i]->GetTransform() };
		const glm::vec3 pos{ transform.GetLocalPosition() };
		transform.SetLocalPosition({ pos.x, 0.f, pos.z });

		m_AllFlags |= (1 << i);
	}
}

dae::State::StatePtr dae::IngredientIdleState::OnUpdate()
{
	if (m_WalkedOnTilesFlags == m_AllFlags )
		return GetIngredient().GetStates().pFall.get();

    return this;
}

void dae::IngredientIdleState::OnExit()
{
	m_WalkedOnTilesFlags = m_AllFlags;
	const std::vector<ColliderComponent*>& pColliders{ GetIngredient().GetRigidBody().GetColliders() };
	for (size_t i{ 1 }; i < pColliders.size(); ++i)
	{
		TransformComponent& transform{ pColliders[i]->GetTransform() };
		const glm::vec3 pos{ transform.GetLocalPosition() };
		transform.SetLocalPosition({ pos.x, 2.f, pos.z });
		pColliders[i]->SetCollisionIgnoreLayer(BurgerTime::PLAYER_COLLISION_LAYER, false);
	}
}

void dae::IngredientIdleState::OnBeginOverlap(const CollisionHit& hit)
{
	int i{};
	const auto& colliders{ GetIngredient().GetRigidBody().GetColliders()};
	const int numColliders{ static_cast<int>(colliders.size()) };
	for (int index{ 1 }; index <= numColliders; ++index)
	{
		const bool isValidCollider{ index < numColliders };
		if (isValidCollider && hit.pCollider == colliders[index])
		{
			i = index;
			break;
		}
		else if (!isValidCollider)
			return;
	}

	int tileFlag{ 1 << i };
	const CollisionLayer collidingLayer{ hit.pOtherCollider->GetCollisionLayer() };
	TransformComponent& transform{ hit.pCollider->GetTransform() };
	const glm::vec3 pos{ transform.GetLocalPosition() };

	if (collidingLayer == BurgerTime::PLAYER_COLLISION_LAYER)
	{
		hit.pCollider->SetCollisionIgnoreLayer(BurgerTime::PLAYER_COLLISION_LAYER, true);
		transform.SetLocalPosition(glm::vec3{ pos.x, 2.f, pos.z });
		m_WalkedOnTilesFlags |= tileFlag;
	}
	else if (collidingLayer == BurgerTime::INGREDIENT_COLLISION_LAYER
		&& hit.pOtherCollider->GetRigidBody()->GetTransform().GetWorldPosition().y 
		> GetIngredient().GetTransform().GetWorldPosition().y)
	{
		m_WalkedOnTilesFlags = m_AllFlags;
	}
}

void dae::IngredientIdleState::OnEndOverlap(const CollisionHit&)
{
	
}
