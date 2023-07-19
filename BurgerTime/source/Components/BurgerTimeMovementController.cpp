#include "BurgerTimeMovementController.h"
#include "Scene/GameObject.h"
#include "Prefabs.h" // LADDER_COLLISION_LAYER

dae::BurgerTimeMovementController::BurgerTimeMovementController(GameObject* pOwner, CharacterController2D* pCharactarerController, CollisionLayer layer)
    : Component(pOwner)
    , m_pCharacterController{ pCharactarerController }
{
	const float tileSize{ 16.f };

	m_pLadderCollider = GetScene()->Instantiate(GetOwner(), glm::vec3{ tileSize * 0.5f - 2.f, -2.f, 0.f })->AddComponent<BoxCollider2DComponent>();
	m_pLadderCollider->SetShape(m_pLadderCollider->GetOwner()->AddComponent<QuadComponent>(glm::vec2{ 4.f, tileSize * 0.3f }));
	m_pLadderCollider->SetCollisionLayer(layer);
	pCharactarerController->GetRigidBody()->AddCollider(m_pLadderCollider);

	m_pCharacterController->GetRigidBody()->GetOnBeginOverlap() += std::bind(&BurgerTimeMovementController::OnOverlap, this, std::placeholders::_1);
	m_pCharacterController->GetRigidBody()->GetOnEndOverlap() += std::bind(&BurgerTimeMovementController::OnEndOverlap, this, std::placeholders::_1);
}

dae::CharacterController2D& dae::BurgerTimeMovementController::GetCharacterController() const
{
    return *m_pCharacterController;
}

void dae::BurgerTimeMovementController::Move(const glm::vec2& dir)
{
	const float width{ m_pCharacterController->GetCollider()->GetShape()->GetSize().x };

	GeometryUtils::Ray ray{};
	ray.direction = glm::vec3{ 0.f, 1.f, 0.f };
	ray.length = 4.f;
	ray.origin = GetTransform().GetWorldPosition() + glm::vec3{ width * 0.5f + dir.x, width, 0.f };

	CollisionHit hit{};
	const float epsilon{ 0.001f };
	bool moveVertical{ std::abs(dir.y) > epsilon };

	if (moveVertical)
	{
		glm::vec3 colliderPos{ m_pLadderCollider->GetTransform().GetLocalPosition() };
		colliderPos.y = width + dir.y * 2.f;
		m_pLadderCollider->GetTransform().SetLocalPosition(colliderPos);
	}

	CollisionLayer layer{ m_pCharacterController->GetCollider()->GetCollisionLayer() };
	if ((!moveVertical && GetScene()->GetPhysics().Raycast(ray, hit, layer)) ||
		(moveVertical && m_CanMoveVertical))
	{
		m_pCharacterController->Move(dir);
	}
}

void dae::BurgerTimeMovementController::OnOverlap(const CollisionHit& hit)
{
	if (hit.pOtherCollider->GetCollisionLayer() == LADDER_COLLISION_LAYER)
	{
		m_CanMoveVertical = true;
	}
}

void dae::BurgerTimeMovementController::OnEndOverlap(const CollisionHit& hit)
{
	if (hit.pOtherCollider->GetCollisionLayer() == LADDER_COLLISION_LAYER)
	{
		m_CanMoveVertical = false;
	}
}
