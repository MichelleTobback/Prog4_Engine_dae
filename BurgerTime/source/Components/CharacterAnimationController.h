#pragma once
#include "Component/Component.h"

#include "Component/SpriteAnimatorComponent.h"
#include "States/SpriteAnim/SpriteAnimState.h"

#include "Component/Physics/RigidBody2DComponent.h"
#include "Components/HealthComponent.h"

namespace dae
{
	class StateMachine;
	class BTGameMode;
	class SpriteAtlasComponent;
	class CharacterAnimationController final : public Component
	{
	public:
		enum class CharacterAnim
		{
			WalkDown, WalkUp, WalkLeft, WalkRight, Die, ThrowDown, ThrowUp, ThrowLeft, ThrowRight, Stunned
		};

		CharacterAnimationController(GameObject* pOwner, SpriteAnimatorComponent* pAnimator);
		CharacterAnimationController(GameObject* pOwner);
		virtual ~CharacterAnimationController() = default;

		CharacterAnimationController(const CharacterAnimationController& other) = delete;
		CharacterAnimationController(CharacterAnimationController&& other) = delete;
		CharacterAnimationController& operator=(const CharacterAnimationController& other) = delete;
		CharacterAnimationController& operator=(CharacterAnimationController&& other) = delete;

		virtual void Awake() override;

		template <typename State>
		void AddState(CharacterAnim id, size_t clipId);

		SpriteAnimClip& GetClip(CharacterAnim clip);
		SpriteAnimState& GetState(CharacterAnim clip);
		void SetAnimState(CharacterAnim clip);

		SpriteAnimatorComponent* GetAnimator();
		HealthComponent* GetHealth();
		RigidBody2DComponent* GetRigidBody();

		bool IsDead() const { return m_IsDead; }
		void ResetIsDead();
		void OnHit(uint32_t health);

	private:

		SpriteAnimatorComponent* m_pAnimator{};
		BTGameMode* m_pCurrentGameMode{ nullptr };

		StateMachine* m_pStateMachine{ nullptr };
		RigidBody2DComponent* m_pRigidBody{ nullptr };
		HealthComponent* m_pHealth{ nullptr };
		bool m_IsDead{ false };

		std::unordered_map<CharacterAnim, std::unique_ptr<SpriteAnimState>> m_pStates;
	};

	template <typename AnimState>
	void CharacterAnimationController::AddState(CharacterAnim id, size_t clipId)
	{
		if (m_pStates.find(id) == m_pStates.end())
			m_pStates[id] = std::make_unique<AnimState>(clipId, this);
	}
}