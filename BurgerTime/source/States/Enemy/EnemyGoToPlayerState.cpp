#include "EnemyGoToPlayerState.h"
#include "Scene/Scene.h"
#include "AI/Pathfinding/AStar.h"
#include "States/CharacterState.h"
#include "States/Enemy/EnemyClimbLadderState.h"
#include "Components/CharacterInfo.h"
#include "Components/EnemyComponent.h"
#include "Core/Random.h"
#include "GameManager.h"
#include "States/GameStates/BTGameMode.h"

#include <glm/gtx/norm.hpp>

dae::EnemyGoToPlayerState::EnemyGoToPlayerState(EnemyComponent* pEnemy)
    : EnemyState(pEnemy)
{
}

void dae::EnemyGoToPlayerState::OnEnter()
{
    m_pCurrentGameMode = dynamic_cast<BTGameMode*>(&GameManager::GetInstance().GetState());
    auto players{ GetEnemy()->GetScene()->GetGameObjectWithTag("Player")};
    if (players.size() > 0)
    {
        CalculatePath();
    }
}

dae::State::StatePtr dae::EnemyGoToPlayerState::OnUpdate()
{
    const float epsilon{ Random::GetRandomFloatInRange(0.1f, 32.f)};
    const glm::vec3 currentPos{ GetEnemy()->GetCharacter()->Get().pController->GetTransform().GetWorldPosition() };
    const glm::vec3 currentPlayerPos{ m_pCurrentGameMode->GetClosestPlayerPos(currentPos) };
    if (m_pPath.size() == 0 || glm::length2(currentPlayerPos - m_PlayerPos) > epsilon)
    {
        m_PlayerPos = currentPlayerPos;
        CalculatePath();
    }

    if (GetEnemy()->GetCharacter()->Get().pHealth->GetValue() == 0)
        return GetEnemy()->GetStates().pDieState.get();
    if (GetEnemy()->GetOverlappedBurger() && GetEnemy()->GetOverlappedBurger()->GetVelocity().y > 0.1f)
        return GetEnemy()->GetStates().pFallState.get();
    if (GetEnemy()->GetOverlappedPlayer())
        return GetEnemy()->GetStates().pAttackState.get();

    const glm::vec3& pos{ GetEnemy()->GetCharacter()->Get().pController->GetTransform().GetWorldPosition() };

    //if (IsOnLadder() && (m_NextNodeIndex - 1) > 0)
    //    return GetEnemy()->GetStates().pClimbLadderState.get();

    if (m_NextNodeIndex != m_pPath.size())
    {
        glm::vec3 nodePos{ m_pPath[m_NextNodeIndex]->GetTransform().GetWorldPosition() };

        glm::vec2 direction{ nodePos.x - pos.x, nodePos.y - pos.y };
        glm::normalize(direction);
        GetEnemy()->GetCharacter()->Get().pController->Move(direction);

        const float distanceSqrt{ glm::distance2(pos, nodePos) };
        const float epsilonSqrt{ 8.f };

        if (distanceSqrt < epsilonSqrt)
        {
            ++m_NextNodeIndex;
        }
    }
    return this;
}

void dae::EnemyGoToPlayerState::OnExit()
{
    m_pCurrentGameMode = nullptr;
}

dae::NodeComponent* dae::EnemyGoToPlayerState::GetNode(const glm::vec3& pos)
{
    const float tileSize{ 8.f };
    auto pObjects{ GetEnemy()->GetCharacter()->Get().pController->GetScene()->GetGameObjectsAtPos(pos, tileSize * 0.5f, false) };
    for (auto& pObject : pObjects)
    {
        if (pObject->HasComponent<NodeComponent>())
            return pObject->GetComponent<NodeComponent>();
    }
    return nullptr;
}

void dae::EnemyGoToPlayerState::CalculatePath()
{
    const glm::vec3 currentPos{ GetEnemy()->GetCharacter()->Get().pController->GetTransform().GetWorldPosition() };
    NodeComponent* pStart{ GetNode(currentPos) };
    NodeComponent* pEnd{ (m_pCurrentGameMode) ? GetNode(m_PlayerPos) : nullptr};

    if (pStart && pEnd)
    {
        AStar pathFinder{};
        m_pPath = pathFinder.FindPath(pStart, pEnd);
        if (m_NextNodeIndex >= m_pPath.size())
            m_NextNodeIndex = 0;
    }
}

bool dae::EnemyGoToPlayerState::IsOnLadder()
{
    size_t current{ m_NextNodeIndex > 1 ? m_NextNodeIndex - 1 : 0 };
    return (m_NextNodeIndex > 0 && m_pPath.size() != 0 && current < m_pPath.size()
        && m_pPath[current]->GetEdges().size() == 2
        && std::abs(GetCharacter().pController->GetDirection().y) > 0.001f);
}
