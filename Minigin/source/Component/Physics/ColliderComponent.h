#pragma once
#include "Component/Component.h"
#include "Component/QuadComponent.h"
#include "Component/CircleComponent.h"
#include "Core/GeometryUtils.h"

namespace dae
{
	enum class CollisionLayer
	{
		None = 0, One = 1, Two = 2, Three = 4, Four = 8, Five = 16
	};

	enum class CollisionFlags
	{
		None = 0, IsTrigger = 1, Overlapped = 2
	};

	enum class ColliderType
	{
		Box = 0, Circle = 1
	};

	class ColliderComponent;
	struct CollisionHit
	{
		glm::vec3 pointOfImpact{};
		glm::vec3 normal{};
		ColliderComponent* pCollider{nullptr};
		ColliderComponent* pOtherCollider{nullptr};
		GameObject* pOther{ nullptr };
		float depth{};
		bool succes{};
	};

	class QuadRendererComponent;
	class RigidBody2DComponent;
	class ColliderComponent : public Component
	{
	public:
		ColliderComponent(GameObject* pOwner, ColliderType type) 
			: Component(pOwner), m_Type{ type }, m_Flags{ CollisionFlags::None } {}
		virtual ~ColliderComponent() override = default;

		ColliderComponent(const ColliderComponent& other) = delete;
		ColliderComponent(ColliderComponent&& other) = delete;
		ColliderComponent& operator=(const ColliderComponent& other) = delete;
		ColliderComponent& operator=(ColliderComponent&& other) = delete;

		virtual bool HandleCollision(ColliderComponent* pOther, const glm::vec3& fixedVel, CollisionHit& result) = 0;
		virtual bool DoRaycast(const GeometryUtils::Ray& ray, CollisionHit& result, CollisionLayer ignoreLayers = CollisionLayer::None) = 0;

		inline ColliderType GetType() const { return m_Type; }

		void SetCollisionLayer(CollisionLayer layer);
		void SetCollisionIgnoreLayer(CollisionLayer layer, bool set);
		CollisionLayer GetCollisionLayer() const;
		CollisionLayer GetCollisionIgnoreLayer() const;

		RigidBody2DComponent* GetRigidBody() const;
		void SetTrigger(bool set);
		bool IsTrigger() const;
		bool IsOverlapping() const;

		CollisionFlags GetFlags() const { return m_Flags; }

	protected:
		CollisionFlags m_Flags;
		void SetRigidBody(RigidBody2DComponent* pRigidBody);

	private:
		friend class RigidBody2DComponent;
		ColliderType m_Type;
		CollisionLayer m_CollisionLayer{ CollisionLayer::One };
		CollisionLayer m_CollisionIgnoreLayer{ CollisionLayer::None };

		RigidBody2DComponent* m_pRigidBody{ nullptr };
	};

	class BoxCollider2DComponent final : public ColliderComponent
	{
	public:
		BoxCollider2DComponent(GameObject* pOwner)
			: ColliderComponent(pOwner, ColliderType::Box) {}
		virtual ~BoxCollider2DComponent() override = default;

		BoxCollider2DComponent(const BoxCollider2DComponent& other) = delete;
		BoxCollider2DComponent(BoxCollider2DComponent&& other) = delete;
		BoxCollider2DComponent& operator=(const BoxCollider2DComponent& other) = delete;
		BoxCollider2DComponent& operator=(BoxCollider2DComponent&& other) = delete;

		virtual bool HandleCollision(ColliderComponent* pOther, const glm::vec3& fixedVel, CollisionHit& result) override;
		virtual bool DoRaycast(const GeometryUtils::Ray& ray, CollisionHit& result, CollisionLayer ignoreLayers = CollisionLayer::None) override;

		void SetShape(QuadComponent* pQuad);
		QuadComponent* GetShape() const { return m_pShape; }

	private:
		QuadComponent* m_pShape{nullptr};

#ifdef _DEBUG
		QuadRendererComponent* m_pDebugRenderer{ nullptr };
#endif // _DEBUG

	};

	class CircleColliderComponent final : public ColliderComponent
	{
	public:
		CircleColliderComponent(GameObject* pOwner)
			: ColliderComponent(pOwner, ColliderType::Circle) {}
		virtual ~CircleColliderComponent() override = default;

		CircleColliderComponent(const CircleColliderComponent& other) = delete;
		CircleColliderComponent(CircleColliderComponent&& other) = delete;
		CircleColliderComponent& operator=(const CircleColliderComponent& other) = delete;
		CircleColliderComponent& operator=(CircleColliderComponent&& other) = delete;

		virtual bool HandleCollision(ColliderComponent* pOther, const glm::vec3& fixedVel, CollisionHit& result) override;
		virtual bool DoRaycast(const GeometryUtils::Ray& ray, CollisionHit& result, CollisionLayer ignoreLayers = CollisionLayer::None) override;

		void SetShape(CircleComponent* pCircle);
		CircleComponent* GetShape() const { return m_pShape; }

	private:
		CircleComponent* m_pShape{ nullptr };
	};
}