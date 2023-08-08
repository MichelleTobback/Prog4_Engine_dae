#pragma once
#include "States/Enemy/EnemyState.h"

namespace dae
{
	enum class CollisionLayer;
	class EnemyDieState final : public EnemyState
	{
	public:
		EnemyDieState(EnemyComponent* pEnemy);
		virtual ~EnemyDieState() = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	private:
		CollisionLayer m_Layer{};
	};
}