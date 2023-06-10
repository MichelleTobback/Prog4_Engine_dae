#pragma once
#include "Component/Component.h"
#include "Core/Subject.h"

namespace dae
{
	class HealthComponent final : public Component
	{
	public:
		HealthComponent(GameObject* pOwner, float max, float current = 0.f);
		virtual ~HealthComponent() override = default;

		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		//returns remaining
		float DealDamage(float amount);
		void Heal(float amount);
		void SetMax(float value, bool refill = false);

		float GetValue() const { return m_CurrentValue; }
		float GetMaxValue() const { return m_MaxValue; }

		inline Subject* GetOnDeath() { return m_pOnDeath.get(); }
		inline Subject* GetOnHealthChanged() { return m_pOnHealthChanged.get(); }

	private:
		float m_CurrentValue;
		float m_MaxValue;

		std::unique_ptr<Subject> m_pOnDeath;
		std::unique_ptr<Subject> m_pOnHealthChanged;

		void HealthChangedCallback();
	};
}