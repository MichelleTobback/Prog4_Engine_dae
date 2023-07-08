#include "ColliderComponent.h"
#include "Core/GeometryUtils.h"
#include "Scene/GameObject.h"
#include "Component/Physics/RigidBody2DComponent.h"

#include "Core/BitFlag.h"

#include <glm/glm.hpp>

#ifdef _DEBUG
	#include "Component/RenderComponent.h"
	#include "Scene/Scene.h"
#endif

void dae::ColliderComponent::SetCollisionLayer(CollisionLayer layer)
{
	m_CollisionLayer = layer;
}

void dae::ColliderComponent::SetCollisionIgnoreLayer(CollisionLayer layer, bool set)
{
	BitFlag::Set(m_CollisionIgnoreLayer, layer, set);
}

dae::CollisionLayer dae::ColliderComponent::GetCollisionLayer() const
{
	return m_CollisionLayer;
}

dae::CollisionLayer dae::ColliderComponent::GetCollisionIgnoreLayer() const
{
	return m_CollisionIgnoreLayer;
}

dae::RigidBody2DComponent* dae::ColliderComponent::GetRigidBody() const
{
	return m_pRigidBody;
}

void dae::ColliderComponent::SetTrigger(bool set)
{
	BitFlag::Set(m_Flags, CollisionFlags::IsTrigger, set);
}

bool dae::ColliderComponent::IsTrigger() const
{
	return BitFlag::IsSet(GetFlags(), CollisionFlags::IsTrigger);
}

bool dae::ColliderComponent::IsOverlapping() const
{
	return BitFlag::IsSet(m_Flags, CollisionFlags::Overlapped);
}

void dae::ColliderComponent::SetRigidBody(RigidBody2DComponent* pRigidBody)
{
	m_pRigidBody = pRigidBody;
}

bool dae::BoxCollider2DComponent::HandleCollision(ColliderComponent* pOther, const glm::vec3& fixedVel, CollisionHit& result)
{
	if (!m_pShape)
		return false;

	auto pos{ GetOwner()->GetTransform().GetWorldPosition() + fixedVel };
	glm::vec4 quad{ pos.x, pos.y, m_pShape->GetSize().x, m_pShape->GetSize().y };
	std::vector<glm::vec2> quadVerts(4);
	m_pShape->GetVerticesWorldPosition(quadVerts);

	switch (pOther->GetType())
	{
	case ColliderType::Box:
	{
		auto pBox{ dynamic_cast<BoxCollider2DComponent*>(pOther) };
		if (!pBox || !pBox->GetShape())
			return false;

		std::vector<glm::vec2> otherQuadVerts(4);
		pBox->GetShape()->GetVerticesWorldPosition(otherQuadVerts);

		GeometryUtils::IntersectionResult collisionResult{};
		result.succes = GeometryUtils::SATPolygonIntersection(quadVerts, otherQuadVerts, collisionResult);
		result.normal = { collisionResult.intersectionNormal, 0.f };
		result.depth = collisionResult.depth;
		result.pointOfImpact = pos - result.normal * result.depth;
		result.pCollider = this;
		result.pOther = pOther->GetOwner();
		result.pOtherCollider = pOther;
	}
		break;

	case ColliderType::Circle:
		break;
	}

	return result.succes;
}

bool dae::BoxCollider2DComponent::DoRaycast(const GeometryUtils::Ray& ray, CollisionHit& result, CollisionLayer ignoreLayers)
{
	if (!m_pShape || BitFlag::IsSet(ignoreLayers, GetCollisionLayer()))
		return false;

	std::vector<glm::vec2> quadVerts(m_pShape->GetVertexCount());
	m_pShape->GetVerticesWorldPosition(quadVerts);

	GeometryUtils::IntersectionResult collisionResult{};
	result.succes = GeometryUtils::RayPolygonIntersection(ray, quadVerts, collisionResult);
	if (result.succes && GetRigidBody())
	{
		result.normal = { collisionResult.intersectionNormal, 0.f };
		result.depth = collisionResult.depth;
		result.pointOfImpact = { collisionResult.intersectionPoint, 1.f };
		result.pCollider = nullptr;
		result.pOther = GetRigidBody()->GetOwner();
		result.pOtherCollider = this;
	}
	else
		result.succes = false;

	return result.succes;
}

void dae::BoxCollider2DComponent::SetShape(QuadComponent* pQuad)
{
	m_pShape = pQuad;

#ifdef _DEBUG
	if (!m_pDebugRenderer)
		m_pDebugRenderer = GetOwner()->GetScene()->Instantiate(0u, GetOwner())->AddComponent<QuadRendererComponent>();
	m_pDebugRenderer->SetQuad(pQuad);
#endif
}

bool dae::CircleColliderComponent::HandleCollision(ColliderComponent* pOther, const glm::vec3& fixedVel, CollisionHit& result)
{
	if (!m_pShape)
		return false;

	const glm::vec2& pos{ GetTransform().GetWorldPosition() + fixedVel };

	switch (pOther->GetType())
	{
	case ColliderType::Box:
	{
		auto pBox{ dynamic_cast<BoxCollider2DComponent*>(pOther) };
		if (!pBox || !pBox->GetShape())
			return false;

		std::vector<glm::vec2> otherQuadVerts(pBox->GetShape()->GetVertexCount());
		pBox->GetShape()->GetVerticesWorldPosition(otherQuadVerts);

		GeometryUtils::IntersectionResult collisionResult{};
		result.succes = GeometryUtils::SATCirclePolygonIntersection(pos, m_pShape->GetRadius(), otherQuadVerts, collisionResult);
		if (result.succes)
		{
			result.normal = { collisionResult.intersectionNormal, 0.f };
			result.depth = collisionResult.depth;
			result.pointOfImpact = glm::vec3{ pos, 1.f } - result.normal * m_pShape->GetRadius();
			result.pCollider = this;
			result.pOther = pOther->GetOwner();
			result.pOtherCollider = pOther;
		}
	}
	break;

	case ColliderType::Circle:
		break;
	}

	return false;
}

bool dae::CircleColliderComponent::DoRaycast(const GeometryUtils::Ray& , CollisionHit& , CollisionLayer )
{
	return false;
}

void dae::CircleColliderComponent::SetShape(CircleComponent* pCircle)
{
	m_pShape = pCircle;
}
