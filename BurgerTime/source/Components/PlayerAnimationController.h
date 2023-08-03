#pragma once
#include "Component/Component.h"

#include "Component/SpriteAnimatorComponent.h"

namespace dae
{
	class BTGameMode;
	class PlayerAnimationController final : public Component
	{
	public:
		PlayerAnimationController(GameObject* pOwner);
		virtual ~PlayerAnimationController() = default;

		PlayerAnimationController(const PlayerAnimationController& other) = delete;
		PlayerAnimationController(PlayerAnimationController&& other) = delete;
		PlayerAnimationController& operator=(const PlayerAnimationController& other) = delete;
		PlayerAnimationController& operator=(PlayerAnimationController&& other) = delete;

		virtual void Awake() override;

	private:
		void OnHit();

		SpriteAnimatorComponent* m_pAnimator{};
		BTGameMode* m_pCurrentGameMode{ nullptr };
	};
}