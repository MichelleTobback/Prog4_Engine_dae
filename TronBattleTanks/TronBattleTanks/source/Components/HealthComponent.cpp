#include "HealthComponent.h"

dae::HealthComponent::HealthComponent(GameObject* pOwner, float max, float current)
	: Component(pOwner)
	, m_CurrentValue{ current }
	, m_MaxValue{ max }
{
	m_pOnDeath = std::make_unique<Subject>();
	m_pOnHealthChanged = std::make_unique<Subject>();
}

float dae::HealthComponent::DealDamage(float amount)
{
	m_CurrentValue -= amount;
	if (m_CurrentValue <= 0.f)
	{
		m_CurrentValue = 0.f;
		m_pOnDeath->Invoke(Event{});
	}

	HealthChangedCallback();

	return m_CurrentValue;
}

void dae::HealthComponent::Heal(float amount)
{
	m_CurrentValue += amount;

	if (m_CurrentValue > m_MaxValue)
	{
		m_CurrentValue = m_MaxValue;
	}

	HealthChangedCallback();
}

void dae::HealthComponent::SetMax(float value, bool refill)
{
	m_MaxValue = value;
	if (refill)
		Heal(value);
}

inline void dae::HealthComponent::HealthChangedCallback()
{
	Event healthChanged{};
	healthChanged.data = reinterpret_cast<char*>(&m_CurrentValue);
	healthChanged.dataSize = sizeof(float);
	m_pOnHealthChanged->Invoke(healthChanged);
}
