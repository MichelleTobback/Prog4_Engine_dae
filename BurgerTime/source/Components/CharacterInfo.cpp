#include "CharacterInfo.h"
#include "Scene/GameObject.h"

#include "BurgerTime.h"

dae::CharacterInfoComponent::CharacterInfoComponent(GameObject* pOwner)
	: Component(pOwner)
{
}

void dae::CharacterInfoComponent::Awake()
{
	if (GetOwner()->HasComponent<CharacterController2D>() && !m_Info.pController)
	{
		m_Info.pController = GetOwner()->GetComponent<CharacterController2D>();
	}

	if (GetOwner()->HasComponent<SpriteAnimatorComponent>() && !m_Info.pAnimator)
		m_Info.pAnimator = GetOwner()->GetComponent<SpriteAnimatorComponent>();

	if (GetOwner()->HasComponent<HealthComponent>())
	{
		if (!m_Info.pHealth)
			m_Info.pHealth = GetOwner()->GetComponent<HealthComponent>();
		m_Info.pHealth->Heal();
	}
}
