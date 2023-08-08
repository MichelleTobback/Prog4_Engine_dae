#include "RigidBody2DComponent.h"
#include "Scene/Scene.h"
#include "Core/Time.h"
#include "Core/BitFlag.h"

dae::RigidBody2DComponent::RigidBody2DComponent(GameObject* pOwner, CollisionMode mode)
    : Component(pOwner), 
    m_pOnBeginOverlap{ std::make_unique<OnOverlapDelegate>() }, 
    m_pOnEndOverlap{ std::make_unique<OnOverlapDelegate>() },
    m_CollisionMode{ mode }
{
}

void dae::RigidBody2DComponent::Awake()
{
    GetScene()->GetPhysics().AddRigidBody(this);
    m_CurrentPos = GetTransform().GetLocalPosition();
}

void dae::RigidBody2DComponent::Sleep()
{
    GetScene()->GetPhysics().RemoveRigidBody(this);
}

void dae::RigidBody2DComponent::Update()
{
    //if (m_PosDirty)
    //{
    //    GetTransform().SetLocalPosition(m_CurrentPos);
    //    m_PosDirty = false;
    //}
    //else
    //{
    //    m_CurrentPos = GetTransform().GetLocalPosition();
    //}
}

void dae::RigidBody2DComponent::FixedUpdate()
{
    float elapsed{ Time::GetInstance().GetFixedTimeStep() };

    if (!IsStatic())
    {
        glm::vec2 acceleration{ m_Force / m_Mass };
        m_Velocity += glm::vec3{ acceleration * elapsed, 0.f };
        glm::vec3 deltaVel{ m_Velocity * elapsed };
        GetTransform().Translate(deltaVel);
    }
    m_Force = {};

    glm::vec3 fixedVelocity{ m_Velocity * elapsed };
    auto& physics{ GetScene()->GetPhysics() };
    CollisionHit result{};
    RigidBody2DComponent* pCollidingBody{ nullptr };
    for (auto pCollider : m_pColliders)
    {
        if (!pCollider || (IsStatic() && !pCollider->IsTrigger()) || pCollider->GetCollisionLayer() == CollisionLayer::None)
            continue;

        physics.ForEachRigidBody([&, this](RigidBody2DComponent* pRigidBody)->void
            {
                if (pRigidBody == this)
                    return;

                auto& otherColliders{ pRigidBody->GetColliders() };
                for (const auto& pOther : otherColliders)
                {
                    const bool blockCollision{ !pOther->IsTrigger() && !pCollider->IsTrigger() };
                    const bool bothTrigger{ pOther->IsTrigger() && pCollider->IsTrigger() };
                    const bool alreadyBlocked{ pCollidingBody != nullptr };
                    const bool noCollisionLayer{ pOther->GetCollisionLayer() == CollisionLayer::None };
                    const bool ignore{ !pOther 
                        || BitFlag::IsSet(pCollider->GetCollisionIgnoreLayer(), pOther->GetCollisionLayer())
                        || BitFlag::IsSet(pOther->GetCollisionIgnoreLayer(), pCollider->GetCollisionLayer()) };
                    if ((blockCollision && alreadyBlocked) || ignore || bothTrigger || noCollisionLayer)
                        continue;

                    CollisionHit currentResult{};
                    pCollider->HandleCollision(pOther, fixedVelocity, currentResult);

                    if (!blockCollision)
                    {
                        CollisionHit otherHit{ currentResult };
                        otherHit.pOther = GetOwner();
                        std::swap(otherHit.pCollider, otherHit.pOtherCollider);
                        //if (pCollider->IsTrigger())
                        //    std::swap(currentResult, otherHit);

                        if (!pCollider->IsOverlapping(pOther) && currentResult.succes)
                        {
                            // Begin overlap
                            m_pOnBeginOverlap->Invoke(currentResult);
                            pRigidBody->GetOnBeginOverlap().Invoke(otherHit);
                        }
                        else if (pCollider->IsOverlapping(pOther) && !currentResult.succes)
                        {
                            // End overlap
                            m_pOnEndOverlap->Invoke(currentResult);
                            pRigidBody->GetOnEndOverlap().Invoke(otherHit);
                        }
                    }
                    else if (currentResult.succes)
                    {
                        result = currentResult;
                        pCollidingBody = pRigidBody;
                    }
                }
                return;
            });
    }

    if (result.succes && pCollidingBody)
    {
        if (pCollidingBody->IsDynamic())
        {
            GetTransform().Translate(-result.normal * result.depth * 0.5f);
            pCollidingBody->GetTransform().Translate(result.normal * result.depth * 0.5f);
        }
        else
        {
            GetTransform().Translate(-result.normal * result.depth);
        }

        glm::vec3 relativeVel{ pCollidingBody->GetVelocity() - m_Velocity };
        float vDotN{ glm::dot(relativeVel, result.normal) };

        if (vDotN > 0.f)
            return;

        float e{ std::min(m_Restitution, pCollidingBody->GetRestitution()) };
        float j{ -(1.f + e) * vDotN };
        j /= (GetMassInverse() + GetMassInverse());

        const glm::vec2 force{ j * result.normal };

        m_Velocity -= glm::vec3{ force * GetMassInverse(), 0.f };
        const glm::vec3 otherVelocity{ pCollidingBody->GetVelocity() + glm::vec3{ force * pCollidingBody->GetMassInverse(), 0.f } };
        pCollidingBody->SetVelociy(otherVelocity);
    }
}

void dae::RigidBody2DComponent::SetDynamic()
{
    m_CollisionMode = CollisionMode::Dynamic;
}

void dae::RigidBody2DComponent::SetStatic()
{
    m_CollisionMode = CollisionMode::Static;
}

bool dae::RigidBody2DComponent::IsStatic() const
{
    return  m_CollisionMode == CollisionMode::Static;
}

bool dae::RigidBody2DComponent::IsDynamic() const
{
    return  m_CollisionMode == CollisionMode::Dynamic;
}

uint32_t dae::RigidBody2DComponent::AddCollider(ColliderComponent* pCollider)
{
    m_pColliders.push_back(pCollider);
    pCollider->SetRigidBody(this);
    return static_cast<uint32_t>(m_pColliders.size() - 1);
}

dae::ColliderComponent* dae::RigidBody2DComponent::GetCollider(uint32_t id)
{
    assert((id >= 0 && id < m_pColliders.size()) && "no valid colliderID");
    return m_pColliders[id];
}

void dae::RigidBody2DComponent::ApplyForce(const glm::vec3& force)
{
    m_Force += force;
}

void dae::RigidBody2DComponent::Move(const glm::vec3& translation)
{
    m_PosDirty = true;
    m_CurrentPos += translation;
}

void dae::RigidBody2DComponent::SetVelociy(const glm::vec3& velocity)
{
    m_Velocity = velocity;
}

const glm::vec3& dae::RigidBody2DComponent::GetVelocity() const
{
    return m_Velocity;
}

float dae::RigidBody2DComponent::GetMassInverse()
{
    if (m_MassDirty)
    {
        m_MassInv = (IsStatic()) ? 0.f : 1.f / m_Mass;
        m_MassDirty = false;
    }
    return m_MassInv;
}

dae::OnOverlapDelegate& dae::RigidBody2DComponent::GetOnBeginOverlap()
{
    return *m_pOnBeginOverlap;
}

dae::OnOverlapDelegate& dae::RigidBody2DComponent::GetOnEndOverlap()
{
    return *m_pOnEndOverlap;
}
