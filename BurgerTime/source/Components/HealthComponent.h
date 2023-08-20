#pragma once
#include "Component/Component.h"
#include "Core/Delegate.h"

namespace dae
{
	class HealthComponent final : public Component
	{
	public:
		HealthComponent(GameObject* pOwner, uint32_t max, uint32_t current = 0.f);
		virtual ~HealthComponent() override = default;

		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		virtual void Sleep() override;

		//returns remaining
		uint32_t DealDamage(uint32_t amount);
		void Heal();
		void Heal(uint32_t amount);
		void SetMax(uint32_t value, bool refill = false);

		uint32_t GetValue() const { return m_CurrentValue; }
		uint32_t GetMaxValue() const { return m_MaxValue; }

		inline Delegate<void()>& GetOnDeath() { return *m_pOnDeath; }
		inline Delegate<void(uint32_t)>& GetOnHealthChanged() { return *m_pOnHealthChanged; }

	private:
		uint32_t m_CurrentValue;
		uint32_t m_MaxValue;

		std::unique_ptr<Delegate<void()>> m_pOnDeath;
		std::unique_ptr<Delegate<void(uint32_t)>> m_pOnHealthChanged;
	};
}