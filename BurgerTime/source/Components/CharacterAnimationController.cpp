#include "CharacterAnimationController.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"
#include "HealthComponent.h"
#include "Component/SpriteAtlasComponent.h"
#include "State/StateMachine.h"

#include "BurgerTime.h"

#include "States/GameStates/BTGameMode.h"
#include "GameManager.h"

dae::CharacterAnimationController::CharacterAnimationController(GameObject* pOwner, SpriteAnimatorComponent* pAnimator)
	: Component(pOwner), m_pAnimator{pAnimator}
{
	
}

dae::CharacterAnimationController::CharacterAnimationController(GameObject* pOwner)
	: Component(pOwner)
{
	
}

void dae::CharacterAnimationController::Awake()
{
	m_pCurrentGameMode = dynamic_cast<BTGameMode*>(&GameManager::GetInstance().GetState());

	if (!m_pStateMachine)
		m_pStateMachine = GetOwner()->GetScene()->Instantiate(0u, GetOwner())->AddComponent<StateMachine>(m_pStates[CharacterAnim::WalkDown].get());
	else
		m_pStateMachine->SetState(m_pStates[CharacterAnim::WalkDown].get());

	if (!m_pAnimator)
		m_pAnimator = GetOwner()->GetComponent<SpriteAnimatorComponent>();

	if (!m_pHealth)
	{
		m_pHealth = GetOwner()->GetComponent<HealthComponent>();
		m_pHealth->GetOnHealthChanged() += std::bind(&CharacterAnimationController::OnHit, this, std::placeholders::_1);
	}
	if (!m_pRigidBody)
		m_pRigidBody = GetOwner()->GetComponent<RigidBody2DComponent>();
}

void dae::CharacterAnimationController::Sleep()
{
	m_pAnimator->GetClip(static_cast<size_t>(CharacterAnimationController::CharacterAnim::Die)).GetAnimEvent(4).Clear();
}

dae::SpriteAnimClip& dae::CharacterAnimationController::GetClip(CharacterAnim clip)
{
	return m_pStates[clip]->GetClip();
}

dae::SpriteAnimState& dae::CharacterAnimationController::GetState(CharacterAnim clip)
{
	return *m_pStates[clip];
}

void dae::CharacterAnimationController::SetAnimState(CharacterAnim clip)
{
	assert(m_pStates.find(clip) != m_pStates.end() && "no animation clip with this id");

	m_pStateMachine->SetState(m_pStates[clip].get());
}

dae::SpriteAnimatorComponent* dae::CharacterAnimationController::GetAnimator()
{
	return m_pAnimator;
}

dae::HealthComponent* dae::CharacterAnimationController::GetHealth()
{
	return m_pHealth;
}

dae::RigidBody2DComponent* dae::CharacterAnimationController::GetRigidBody()
{
	return m_pRigidBody;
}

void dae::CharacterAnimationController::ResetIsDead()
{
	m_IsDead = false;
}

void dae::CharacterAnimationController::OnHit(uint32_t health)
{
	if (health < m_pHealth->GetMaxValue())
	{
		m_IsDead = true;
	}
}
