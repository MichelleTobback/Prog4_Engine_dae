#include "EnemyClimbLadderState.h"
#include "States/CharacterState.h"
#include "Components/CharacterInfo.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Components/NodeComponent.h"
#include "Components/EnemyComponent.h"
#include "Component/Physics/CharacterController2D.h"

dae::EnemyClimbLadderState::EnemyClimbLadderState(EnemyComponent* pEnemy)
    : EnemyState(pEnemy)
{
}

void dae::EnemyClimbLadderState::OnEnter()
{
    CharacterController2D* pController{ GetCharacter().pController };
    m_MoveDir = pController->GetDirection();
    SetEndTile();
}

dae::State::StatePtr dae::EnemyClimbLadderState::OnUpdate()
{
    auto pController{ GetEnemy()->GetCharacter()->Get().pController };
    pController->Move(m_MoveDir);

    const glm::vec3& pos{ pController->GetTransform().GetWorldPosition() };
    const float distanceSqrt{ glm::distance2(pos, m_EndPos) };
    const float epsilonSqrt{ 8.f };

    if (distanceSqrt < epsilonSqrt || std::abs(glm::length2(pController->GetRigidBody()->GetVelocity())) < epsilonSqrt)
    {
        return GetEnemy()->GetStates().pGoToPlayerState.get();
    }

    return this;
}

void dae::EnemyClimbLadderState::OnExit()
{
    
}

void dae::EnemyClimbLadderState::SetEndTile()
{
    const glm::vec3& pos{ GetEnemy()->GetTransform().GetWorldPosition() };
    NodeComponent* pCurrent{ GetNode(pos) }, * pPrev{nullptr};
    if (!pCurrent)
    {
        m_EndPos = pos;
        return;
    }
    const glm::vec3 up{ 0.f, 1.f, 0.f };
    for (auto& edge : pCurrent->GetEdges())
    {
        glm::vec3 nodePos{ edge.pTo->GetTransform().GetWorldPosition() };
        glm::vec3 direction{ nodePos - pos };
        glm::vec3 moveDir{ m_MoveDir.x, m_MoveDir.y, 0.f };
        if (glm::dot(moveDir, direction) >= 0)
        {
            pPrev = pCurrent;
            pCurrent = edge.pTo;
            break;
        }
    }


    while (pCurrent && pCurrent != pPrev && pCurrent->GetEdges().size() <= 2)
    {
        for (auto& edge : pCurrent->GetEdges())
        {
            if (edge.pTo != pPrev)
            {
                pPrev = pCurrent;
                pCurrent = edge.pTo;
                break;
            }
        }
    }

    m_EndPos = pCurrent->GetTransform().GetWorldPosition();
}

dae::NodeComponent* dae::EnemyClimbLadderState::GetNode(const glm::vec3& pos)
{
    const float tileSize{ 8.f };
    auto pObjects{ GetEnemy()->GetScene()->GetGameObjectsAtPos(pos, tileSize * 0.5f, false) };
    for (auto& pObject : pObjects)
    {
        if (pObject->HasComponent<NodeComponent>())
            return pObject->GetComponent<NodeComponent>();
    }
    return nullptr;
}
