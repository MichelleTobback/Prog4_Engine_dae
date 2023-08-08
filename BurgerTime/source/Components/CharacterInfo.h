#pragma once
#include "Component/Component.h"
#include "Component/Physics/CharacterController2D.h"
#include "Components/HealthComponent.h"
#include "Components/CharacterAnimationController.h"

#include "States/CharacterState.h"

#include <memory>

namespace dae
{
	struct CharacterInfo
	{
		HealthComponent* pHealth{ nullptr };
		CharacterController2D* pController{ nullptr };
		CharacterAnimationController* pAnimator{ nullptr };
	};

	class CharacterInfoComponent final : public Component
	{
	public:
		CharacterInfoComponent(GameObject* pOwner, HealthComponent* pHealth, CharacterController2D* pController, CharacterAnimationController* pAnimator);
		virtual ~CharacterInfoComponent() = default;

		CharacterInfoComponent(const CharacterInfoComponent& other) = delete;
		CharacterInfoComponent(CharacterInfoComponent&& other) = delete;
		CharacterInfoComponent& operator=(const CharacterInfoComponent& other) = delete;
		CharacterInfoComponent& operator=(CharacterInfoComponent&& other) = delete;

		virtual void Awake() override;

		CharacterInfo& Get() { return m_Info; }

	private:
		CharacterInfo m_Info{};
	};
}