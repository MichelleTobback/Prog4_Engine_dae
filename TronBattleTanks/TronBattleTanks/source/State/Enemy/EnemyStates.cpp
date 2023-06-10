#include "EnemyStates.h"

dae::EnemyIdleState::EnemyIdleState(TankComponent* pTank)
    : TankState(pTank)
{
}

void dae::EnemyIdleState::OnEnter()
{
}

dae::State::StatePtr dae::EnemyIdleState::OnUpdate()
{
    return nullptr;
}

void dae::EnemyIdleState::OnExit()
{
}
