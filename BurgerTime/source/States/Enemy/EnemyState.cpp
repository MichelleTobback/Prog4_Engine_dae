#include "EnemyState.h"
#include "Components/EnemyComponent.h"

dae::EnemyState::EnemyState(EnemyComponent* pEnemy)
    : m_pEnemy{pEnemy}
{
}

dae::EnemyComponent* dae::EnemyState::GetEnemy()
{
    return m_pEnemy;
}

dae::CharacterInfo& dae::EnemyState::GetCharacter()
{
    return m_pEnemy->GetCharacter()->Get();
}
