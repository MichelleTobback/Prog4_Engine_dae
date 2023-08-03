#pragma once
#include "Component/Component.h"
#include "Component/Physics/CharacterController2D.h"
#include "Component/SpriteAnimatorComponent.h"
#include "Components/HealthComponent.h"

#include "States/CharacterState.h"

#include <memory>

namespace dae
{
	struct CharacterInfo
	{
		CharacterController2D* pController{ nullptr };
		SpriteAnimatorComponent* pAnimator{ nullptr };
		HealthComponent* pHealth{nullptr};

		std::vector<std::unique_ptr<CharacterState>> pStates;
	};

	class CharacterInfoComponent final : public Component
	{
	public:
		CharacterInfoComponent(GameObject* pOwner);
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