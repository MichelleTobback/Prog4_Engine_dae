#include "CharacterInfo.h"
#include "Scene/GameObject.h"

#include "BurgerTime.h"

dae::CharacterInfoComponent::CharacterInfoComponent(GameObject* pOwner, HealthComponent* pHealth, CharacterController2D* pController, CharacterAnimationController* pAnimator)
	: Component(pOwner), m_Info{ pHealth, pController, pAnimator }
{
}

void dae::CharacterInfoComponent::Awake()
{
	m_Info.pHealth->Heal();
}
