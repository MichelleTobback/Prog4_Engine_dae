#pragma once
#include "States/Enemy/EnemyState.h"

namespace dae
{
	class EnemyStunnedState final : public EnemyState
	{
	public:
		EnemyStunnedState(EnemyComponent* pEnemy, float duration = 1.f);
		virtual ~EnemyStunnedState() = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	private:
		const float m_Duration;
		float m_Time{};
	};
}