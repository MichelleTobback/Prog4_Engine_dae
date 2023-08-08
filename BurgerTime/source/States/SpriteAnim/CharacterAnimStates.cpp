#include "CharacterAnimStates.h"
#include "BurgerTime.h"

#include "Component/Physics/RigidBody2DComponent.h"
#include "Components/HealthComponent.h"

#include "Components/CharacterAnimationController.h"

//===============================================================
// WalkDownAnimState
//===============================================================

dae::WalkDownAnimState::WalkDownAnimState(size_t clipId, CharacterAnimationController* pAnimatorController)
	: SpriteAnimState(clipId, pAnimatorController->GetAnimator()), m_pAnimController{ pAnimatorController }
{
	
}

void dae::WalkDownAnimState::OnEnter()
{
	GetAnimator()->PlayClip(GetID());
}

dae::State::StatePtr dae::WalkDownAnimState::OnUpdate()
{
	if (m_pAnimController->IsDead())
		return &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::Die);

	const float epsilon{ 0.1f };
	RigidBody2DComponent* pRigidBody{ m_pAnimController->GetRigidBody() };
	if (pRigidBody->GetVelocity().x > epsilon)
		return &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::WalkRight);
	if (pRigidBody->GetVelocity().x < -epsilon)
		return &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::WalkLeft);
	if (pRigidBody->GetVelocity().y < -epsilon)
		return &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::WalkUp);

	return this;
}

void dae::WalkDownAnimState::OnExit()
{
	
}

//===============================================================
// WalkUpAnimState
//===============================================================

dae::WalkUpAnimState::WalkUpAnimState(size_t clipId, CharacterAnimationController* pAnimatorController)
	: SpriteAnimState(clipId, pAnimatorController->GetAnimator()), m_pAnimController{ pAnimatorController }
{
	
}

void dae::WalkUpAnimState::OnEnter()
{
	GetAnimator()->PlayClip(GetID());
}

dae::State::StatePtr dae::WalkUpAnimState::OnUpdate()
{
	if (m_pAnimController->IsDead())
		return &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::Die);

	const float epsilon{ 0.1f };
	RigidBody2DComponent* pRigidBody{ m_pAnimController->GetRigidBody() };
	if (pRigidBody->GetVelocity().x > epsilon)
		return &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::WalkRight);
	if (pRigidBody->GetVelocity().x < -epsilon)
		return &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::WalkLeft);
	if (pRigidBody->GetVelocity().y > epsilon)
		return &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::WalkDown);

	return this;
}

void dae::WalkUpAnimState::OnExit()
{
}

//===============================================================
// WalkRightAnimState
//===============================================================

dae::WalkRightAnimState::WalkRightAnimState(size_t clipId, CharacterAnimationController* pAnimatorController)
	: SpriteAnimState(clipId, pAnimatorController->GetAnimator()), m_pAnimController{ pAnimatorController }
{
	
}

void dae::WalkRightAnimState::OnEnter()
{
	GetAnimator()->PlayClip(GetID());
}

dae::State::StatePtr dae::WalkRightAnimState::OnUpdate()
{
	if (m_pAnimController->IsDead())
		return &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::Die);

	const float epsilon{ 0.1f };
	RigidBody2DComponent* pRigidBody{ m_pAnimController->GetRigidBody() };
	if (pRigidBody->GetVelocity().y < -epsilon)
		return &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::WalkUp);
	if (pRigidBody->GetVelocity().y > epsilon)
		return &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::WalkDown);
	if (pRigidBody->GetVelocity().x < -epsilon)
		return &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::WalkLeft);

	return this;
}

void dae::WalkRightAnimState::OnExit()
{
}

//===============================================================
// WalkLeftAnimState
//===============================================================

dae::WalkLeftAnimState::WalkLeftAnimState(size_t clipId, CharacterAnimationController* pAnimatorController)
	: SpriteAnimState(clipId, pAnimatorController->GetAnimator()), m_pAnimController{ pAnimatorController }
{
	
}

void dae::WalkLeftAnimState::OnEnter()
{
	GetAnimator()->PlayClip(GetID());
}

dae::State::StatePtr dae::WalkLeftAnimState::OnUpdate()
{
	if (m_pAnimController->IsDead())
		return &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::Die);

	const float epsilon{ 0.1f };
	RigidBody2DComponent* pRigidBody{ m_pAnimController->GetRigidBody() };
	if (pRigidBody->GetVelocity().y < -epsilon)
		return &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::WalkUp);
	if (pRigidBody->GetVelocity().y > epsilon)
		return &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::WalkDown);
	if (pRigidBody->GetVelocity().x > epsilon)
		return &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::WalkRight);

	return this;
}

void dae::WalkLeftAnimState::OnExit()
{
}

//===============================================================
// DieAnimState
//===============================================================

dae::DieAnimState::DieAnimState(size_t clipId, CharacterAnimationController* pAnimatorController)
	: SpriteAnimState(clipId, pAnimatorController->GetAnimator()), m_pAnimController{ pAnimatorController }
{
	auto& clip{ GetClip() };
	if (clip.frames.size() > 0)
		clip.GetAnimEvent(clip.frames.size() - 1) += std::bind(&DieAnimState::OnAnimEnd, this);
}

void dae::DieAnimState::OnEnter()
{
	GetAnimator()->PlayClip(GetID());
	GetAnimator()->PlayFromStart();
	m_AnimEnd = false;
}

dae::State::StatePtr dae::DieAnimState::OnUpdate()
{
	return !m_AnimEnd ? this : &m_pAnimController->GetState(CharacterAnimationController::CharacterAnim::WalkDown);
}

void dae::DieAnimState::OnExit()
{
	m_pAnimController->ResetIsDead();
}

void dae::DieAnimState::OnAnimEnd()
{
	m_AnimEnd = true;
}

//===============================================================
// StunnedAnimState
//===============================================================

dae::StunnedAnimState::StunnedAnimState(size_t clipId, CharacterAnimationController* pAnimatorController)
	: SpriteAnimState(clipId, pAnimatorController->GetAnimator()), m_pAnimController{ pAnimatorController }
{
}

void dae::StunnedAnimState::OnEnter()
{
	GetAnimator()->PlayClip(GetID());
	GetAnimator()->PlayFromStart();
}

dae::State::StatePtr dae::StunnedAnimState::OnUpdate()
{
	return this;
}

void dae::StunnedAnimState::OnExit()
{
}
