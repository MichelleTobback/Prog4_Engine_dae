#include "SpriteAnimState.h"

dae::SpriteAnimState::SpriteAnimState(size_t clipId, SpriteAnimatorComponent* pAnimator)
    : m_pAnimator{pAnimator}, m_AnimClip{clipId}
{
    
}

dae::SpriteAnimClip& dae::SpriteAnimState::GetClip()
{
    return m_pAnimator->GetClip(m_AnimClip);
}

size_t dae::SpriteAnimState::GetID() const
{
    return m_AnimClip;
}

dae::SpriteAnimatorComponent* dae::SpriteAnimState::GetAnimator()
{
    return m_pAnimator;
}
