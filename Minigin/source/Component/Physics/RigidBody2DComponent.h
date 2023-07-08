#pragma once
#include "Component/Component.h"
#include "ColliderComponent.h"
#include "Core/Delegate.h"

namespace dae
{
	using OnOverlapDelegate = Delegate<void(const CollisionHit& hit)>;

	enum class CollisionMode
	{
		Static, Dynamic //,Kinematic
	};

	class GameObject;
	class RigidBody2DComponent final : public Component
	{
	public:
		RigidBody2DComponent(GameObject* pOwner, CollisionMode mode = CollisionMode::Dynamic);
		virtual ~RigidBody2DComponent() override = default;

		RigidBody2DComponent(const RigidBody2DComponent& other) = delete;
		RigidBody2DComponent(RigidBody2DComponent&& other) = delete;
		RigidBody2DComponent& operator=(const RigidBody2DComponent& other) = delete;
		RigidBody2DComponent& operator=(RigidBody2DComponent&& other) = delete;

		virtual void Awake() override;
		virtual void Sleep() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;

		void SetDynamic();
		void SetStatic();
		bool IsStatic() const;
		bool IsDynamic() const;

		uint32_t AddCollider(ColliderComponent* pCollider);
		ColliderComponent* GetCollider(uint32_t id);
		inline const std::vector<ColliderComponent*>& GetColliders() const { return m_pColliders; }

		void ApplyForce(const glm::vec3& force);
		void Move(const glm::vec3& translation);

		void SetVelociy(const glm::vec3& velocity);
		const glm::vec3& GetVelocity() const;
		inline float GetRestitution() const { return m_Restitution; }
		inline void SetRestitution(float value) { m_Restitution = value; }
		inline float GetMass() const { return m_Mass; }
		inline void SetMass(float value) { m_Mass = value; m_MassDirty = true; }
		inline CollisionMode GetCollisionMode() const { return m_CollisionMode; }
		float GetMassInverse();

		OnOverlapDelegate& GetOnBeginOverlap();
		OnOverlapDelegate& GetOnEndOverlap();

	private:
		glm::vec3 m_CurrentPos{};
		bool m_PosDirty{};
		glm::vec3 m_Velocity{};
		glm::vec3 m_Force{};
		float m_Restitution{ 0.5f };
		float m_Mass{ 1.f };
		float m_MassInv{};
		bool m_MassDirty{ true };
		CollisionMode m_CollisionMode;

		std::vector<ColliderComponent*> m_pColliders{};

		std::unique_ptr<OnOverlapDelegate> m_pOnBeginOverlap;
		std::unique_ptr<OnOverlapDelegate> m_pOnEndOverlap;
	};
}