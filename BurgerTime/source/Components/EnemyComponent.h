#pragma once
#include "Component/Component.h"
#include "Component/Physics/RigidBody2DComponent.h"

#include "States/Enemy/EnemyState.h"

namespace dae
{
	class BTGameMode;
	class CharacterInfoComponent;
	class StateMachine;
	class EnemyComponent final : public Component
	{
	public:
		struct States
		{
			std::unique_ptr<EnemyState> pGoToPlayerState{ nullptr };
			std::unique_ptr<EnemyState> pClimbLadderState{ nullptr };
			std::unique_ptr<EnemyState> pDieState{ nullptr };
			std::unique_ptr<EnemyState> pFallState{ nullptr };
			std::unique_ptr<EnemyState> pAttackState{ nullptr };
			std::unique_ptr<EnemyState> pStunnedState{ nullptr };
			std::unique_ptr<EnemyState> pWaitState{ nullptr };
		};

		EnemyComponent(GameObject* pOwner, CharacterInfoComponent* pCharacter, uint32_t reward);
		virtual ~EnemyComponent() = default;

		EnemyComponent(const EnemyComponent& other) = delete;
		EnemyComponent(EnemyComponent&& other) = delete;
		EnemyComponent& operator=(const EnemyComponent& other) = delete;
		EnemyComponent& operator=(EnemyComponent&& other) = delete;

		virtual void Awake() override;

		void Stun();

		EnemyState* GetState(size_t) { return nullptr; }
		CharacterInfoComponent* GetCharacter() { return m_pCharacter; }
		CharacterInfoComponent* GetOverlappedPlayer() { return m_pOverlappedPlayer; }
		RigidBody2DComponent* GetOverlappedBurger() const {return m_pOverlappedBurger;}
		States& GetStates() { return m_States; }

	private:
		void OnDeath();
		void OnOverlap(const CollisionHit& hit);
		void OnEndOverlap(const CollisionHit& hit);

		uint32_t m_Reward;
		CharacterInfoComponent* m_pCharacter;
		CharacterInfoComponent* m_pOverlappedPlayer{nullptr};
		BTGameMode* m_pCurrentGameMode{ nullptr };
		RigidBody2DComponent* m_pOverlappedBurger{ nullptr };
		StateMachine* m_pStateMachine{ nullptr };

		States m_States;
	};
}