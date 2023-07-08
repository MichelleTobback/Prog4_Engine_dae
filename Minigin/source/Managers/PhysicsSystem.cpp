#include "PhysicsSystem.h"

#include <algorithm>

void dae::PhysicsScene::ForEachRigidBody(const std::function<void(RigidBody2DComponent*)>& fn)
{
	std::for_each(m_pRigidBodies.begin(), m_pRigidBodies.end(), fn);
}

void dae::PhysicsScene::ForEachRigidBodyWithBreak(const std::function<bool(RigidBody2DComponent*)>& fn)
{
	for (auto pRigidBody : m_pRigidBodies)
	{
		if (!fn(pRigidBody))
			return;
	}
}

void dae::PhysicsScene::ForEachStaticRigidBody(const std::function<void(RigidBody2DComponent*)>& fn)
{
	std::for_each(m_pRigidBodies.begin(), m_pRigidBodies.end(), [fn](RigidBody2DComponent* pRigidbody)
		{
			if (pRigidbody->IsStatic())
				fn(pRigidbody);
		});
}

void dae::PhysicsScene::ForEachDynamicRigidBody(const std::function<void(RigidBody2DComponent*)>& fn)
{
	std::for_each(m_pRigidBodies.begin(), m_pRigidBodies.end(), [fn](RigidBody2DComponent* pRigidbody)
		{
			if (pRigidbody->IsDynamic())
				fn(pRigidbody);
		});
}

bool dae::PhysicsScene::Raycast(const GeometryUtils::Ray& ray, CollisionHit& outHit, CollisionLayer collisionIngore)
{
	CollisionHit result{};
	outHit.depth = ray.length + 1.f;
	ForEachRigidBody([&](RigidBody2DComponent* pRigidBody)
		{
			const auto& colliders{ pRigidBody->GetColliders() };
			for (const auto& pCollider : colliders)
			{
				if (pCollider->DoRaycast(ray, result, collisionIngore))
				{
					if (result.depth < outHit.depth)
						outHit = result;
				}
			}
		});
	return outHit.succes;
}

void dae::PhysicsScene::AddRigidBody(RigidBody2DComponent* pRigidBody)
{
	m_pRigidBodies.push_back(pRigidBody);
}

void dae::PhysicsScene::RemoveRigidBody(RigidBody2DComponent* pRigidBody)
{
	if (m_pRigidBodies.size() == 0)
		return;
	m_pRigidBodies.erase(std::remove(m_pRigidBodies.begin(), m_pRigidBodies.end(), pRigidBody), m_pRigidBodies.end());
}
