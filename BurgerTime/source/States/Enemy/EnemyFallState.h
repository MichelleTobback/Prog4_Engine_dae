#pragma once
#include "States/Enemy/EnemyState.h"

namespace dae
{
	class EnemyFallState final : public EnemyState
	{
	public:
		EnemyFallState(EnemyComponent* pEnemy);
		virtual ~EnemyFallState() = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

		virtual void OnEndOverlap(const CollisionHit&) override;

	private:
		float m_OriginalSpeed{};
	};
}