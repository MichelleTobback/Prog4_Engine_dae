#include "EnemyWaitState.h"
#include "Core/Time.h"
#include "Components/EnemyComponent.h"

dae::EnemyWaitState::EnemyWaitState(EnemyComponent* pEnemy, float time)
    : EnemyState(pEnemy), m_Time{time}
{
}

void dae::EnemyWaitState::OnEnter()
{
    m_Current = 0.f;
}

dae::State::StatePtr dae::EnemyWaitState::OnUpdate()
{
    m_Current += Time::GetInstance().GetDeltaTime();
    if (m_Current >= m_Time)
        return GetEnemy()->GetStates().pGoToPlayerState.get();

    return this;
}

void dae::EnemyWaitState::OnExit()
{
}
