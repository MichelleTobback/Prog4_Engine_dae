#pragma once
#include "States/Enemy/EnemyState.h"

namespace dae
{
	class EnemyAttackState final : public EnemyState
	{
	public:
		EnemyAttackState(EnemyComponent* pEnemy, float cooldown = 1.f);
		virtual ~EnemyAttackState() = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	private:
		const float m_Cooldown;
		float m_Time{};
	};
}