#pragma once
#include "Component/Component.h"

#include "Component/Physics/RigidBody2DComponent.h"
#include "Components/HealthComponent.h"

namespace dae
{
	class BTGameMode;
	class EnemyComponent final : public Component
	{
	public:
		EnemyComponent(GameObject* pOwner, uint32_t reward);
		virtual ~EnemyComponent() = default;

		EnemyComponent(const EnemyComponent& other) = delete;
		EnemyComponent(EnemyComponent&& other) = delete;
		EnemyComponent& operator=(const EnemyComponent& other) = delete;
		EnemyComponent& operator=(EnemyComponent&& other) = delete;

		virtual void Awake() override;

	private:
		void OnDeath();
		void OnOverlap(const CollisionHit& hit);

		uint32_t m_Reward;
		HealthComponent* m_pHealth{};
		BTGameMode* m_pCurrentGameMode{ nullptr };
	};
}