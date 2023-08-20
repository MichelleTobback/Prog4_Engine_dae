#pragma once
#include "States/Enemy/EnemyState.h"

namespace dae
{
	class InputHandlerComponent;
	class EnemyControlledState final : public EnemyState
	{
	public:
		EnemyControlledState(EnemyComponent* pEnemy);
		virtual ~EnemyControlledState() = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	private:
		InputHandlerComponent* m_pInput{nullptr};
	};
}