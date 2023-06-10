#include "PhysicsSystem.h"
#include "Component/Physics/RigidBody2DComponent.h"

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

void dae::PhysicsScene::AddRigidBody(RigidBody2DComponent* pRigidBody)
{
	m_pRigidBodies.push_back(pRigidBody);
}

void dae::PhysicsScene::RemoveRigidBody(RigidBody2DComponent* pRigidBody)
{
	m_pRigidBodies.erase(std::remove(m_pRigidBodies.begin(), m_pRigidBodies.end(), pRigidBody), m_pRigidBodies.end());
}
