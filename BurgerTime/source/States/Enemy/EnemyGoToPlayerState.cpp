#include "EnemyGoToPlayerState.h"
#include "Scene/Scene.h"
#include "AI/Pathfinding/AStar.h"
#include "States/CharacterState.h"
#include "States/Enemy/EnemyClimbLadderState.h"
#include "Components/CharacterInfo.h"
#include "Core/Random.h"

#include <glm/gtx/norm.hpp>

dae::EnemyGoToPlayerState::EnemyGoToPlayerState(CharacterInfoComponent* pCharacterInfo)
    : CharacterState(pCharacterInfo)
{
}

void dae::EnemyGoToPlayerState::OnEnter()
{
    auto players{ GetCharacter()->pController->GetScene()->GetGameObjectWithTag("Player") };
    if (players.size() > 0)
    {
        SetPlayer(players[0]->GetComponent<CharacterInfoComponent>());
        CalculatePath();
    }
}

dae::State::StatePtr dae::EnemyGoToPlayerState::OnUpdate()
{
    if (m_pPath.size() == 0)
        return this;

    const glm::vec3& pos{ GetCharacter()->pController->GetTransform().GetWorldPosition() };

    //if (IsOnLadder() && (m_NextNodeIndex - 1) > 0)
    //{
    //    EnemyClimbLadderState* pNextState{ reinterpret_cast<EnemyClimbLadderState*>(GetCharacter()->pStates[1].get()) };
    //    NodeComponent* pNode{ m_pPath[m_NextNodeIndex - 1] };
    //    const float epsilon{ 0.001f };
    //    for (auto edge : pNode->GetEdges())
    //    {
    //        glm::vec3 nodePos{ edge.pTo->GetTransform().GetWorldPosition() };
    //        if (std::abs((nodePos - pos).y) > epsilon)
    //        {
    //            pNextState->SetEndTile(edge.pTo);
    //            return pNextState;
    //        }
    //    }
    //}

    if (m_NextNodeIndex != m_pPath.size())
    {
        glm::vec3 nodePos{ m_pPath[m_NextNodeIndex]->GetTransform().GetWorldPosition() };

        glm::vec2 direction{ nodePos.x - pos.x, nodePos.y - pos.y };
        glm::normalize(direction);
        GetCharacter()->pController->Move(direction);

        const float distanceSqrt{ glm::distance2(pos, nodePos) };
        const float epsilonSqrt{ 8.f };

        if (distanceSqrt < epsilonSqrt)
        {
            ++m_NextNodeIndex;
        }
    }
    if (m_NextNodeIndex == m_pPath.size())
    {
        CalculatePath();
    }
    return this;
}

void dae::EnemyGoToPlayerState::OnExit()
{

}

void dae::EnemyGoToPlayerState::SetPlayer(CharacterInfoComponent* pPlayer)
{
    m_pPlayer = pPlayer;
}

dae::NodeComponent* dae::EnemyGoToPlayerState::GetNode(const glm::vec3& pos)
{
    const float tileSize{ 8.f };
    auto pObjects{ GetCharacter()->pController->GetScene()->GetGameObjectsAtPos(pos, tileSize * 0.5f, false) };
    for (auto& pObject : pObjects)
    {
        if (pObject->HasComponent<NodeComponent>())
            return pObject->GetComponent<NodeComponent>();
    }
    return nullptr;
}

void dae::EnemyGoToPlayerState::CalculatePath()
{
    NodeComponent* pStart{ GetNode(GetCharacter()->pController->GetTransform().GetWorldPosition()) };
    NodeComponent* pEnd{ m_pPlayer ? GetNode(m_pPlayer->GetTransform().GetWorldPosition()) : nullptr};

    if (pStart && pEnd)
    {
        AStar pathFinder{};
        m_pPath = pathFinder.FindPath(pStart, pEnd);
        m_NextNodeIndex = 0;
    }
}

bool dae::EnemyGoToPlayerState::IsOnLadder()
{
    size_t current{ m_NextNodeIndex > 1 ? m_NextNodeIndex - 1 : 0 };
    return (m_NextNodeIndex > 0 && m_pPath.size() != 0 && current < m_pPath.size()
        && m_pPath[current]->GetEdges().size() > 2);
}
