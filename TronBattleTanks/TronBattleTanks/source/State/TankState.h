#pragma once

#include "State.h"
#include "Components/TankComponent.h"

namespace dae
{
	class TankState : public State
	{
	public:
		TankState(TankComponent* pTank) : m_pTank{pTank} {}
		virtual ~TankState() = default;

		virtual void OnEnter() override = 0;
		virtual StatePtr OnUpdate() override = 0;
		virtual void OnExit() override = 0;

		inline const TankComponent& GetTank() const { return *m_pTank; }

	protected:
		inline TankComponent& GetTank() { return *m_pTank; }

	private:
		TankComponent* m_pTank;
	};
}
