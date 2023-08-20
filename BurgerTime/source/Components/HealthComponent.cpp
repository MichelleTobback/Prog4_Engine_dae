#include "HealthComponent.h"

dae::HealthComponent::HealthComponent(GameObject* pOwner, uint32_t max, uint32_t current)
	: Component(pOwner)
	, m_CurrentValue{ current }
	, m_MaxValue{ max }
{
	m_pOnDeath = std::make_unique<Delegate<void()>>();
	m_pOnHealthChanged = std::make_unique<Delegate<void(uint32_t)>>();
}

void dae::HealthComponent::Sleep()
{
	m_pOnDeath->Clear();
	m_pOnHealthChanged->Clear();
}

uint32_t dae::HealthComponent::DealDamage(uint32_t amount)
{
	if (m_CurrentValue > 0)
	{
		if (amount <= m_CurrentValue)
			m_CurrentValue -= amount;
		else
			m_CurrentValue = 0;

		if (m_CurrentValue == 0)
			m_pOnDeath->Invoke();

		m_pOnHealthChanged->Invoke(m_CurrentValue);
	}

	return m_CurrentValue;
}

void dae::HealthComponent::Heal()
{
	Heal(m_MaxValue);
}

void dae::HealthComponent::Heal(uint32_t amount)
{
	m_CurrentValue += amount;

	if (m_CurrentValue > m_MaxValue)
	{
		m_CurrentValue = m_MaxValue;
	}

	m_pOnHealthChanged->Invoke(m_CurrentValue);
}

void dae::HealthComponent::SetMax(uint32_t value, bool refill)
{
	m_MaxValue = value;
	if (refill)
		Heal(value);
}
