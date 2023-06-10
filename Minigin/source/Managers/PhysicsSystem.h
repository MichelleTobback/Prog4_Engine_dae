#pragma once
#include <vector>
#include <functional>

namespace dae
{
	class RigidBody2DComponent;
	class PhysicsScene final
	{
	public:
		PhysicsScene() = default;

		void ForEachRigidBody(const std::function<void(RigidBody2DComponent*)>& fn);
		//return false to break
		void ForEachRigidBodyWithBreak(const std::function<bool(RigidBody2DComponent*)>& fn);
		void ForEachStaticRigidBody(const std::function<void(RigidBody2DComponent*)>& fn);
		void ForEachDynamicRigidBody(const std::function<void(RigidBody2DComponent*)>& fn);

	private:
		void AddRigidBody(RigidBody2DComponent* pRigidBody);
		void RemoveRigidBody(RigidBody2DComponent* pRigidBody);

		friend class RigidBody2DComponent;
		std::vector<RigidBody2DComponent*> m_pRigidBodies{};
	};
}