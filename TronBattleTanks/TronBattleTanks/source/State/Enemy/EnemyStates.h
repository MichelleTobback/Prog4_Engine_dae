#pragma once
#include "State/TankState.h"

namespace dae
{
	class EnemyIdleState final : public TankState
	{
	public:
		EnemyIdleState(TankComponent* pTank);
		virtual ~EnemyIdleState() = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	private:

	};
}