#pragma once
#include "IngredientState.h"

namespace dae
{
	class AudioClip;
	class RigidBody2DComponent;
	class IngredientIdleState : public IngredientState
	{
	public:
		IngredientIdleState(BurgerIngredient* pIngredient);
		virtual ~IngredientIdleState() = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

		virtual void OnBeginOverlap(const CollisionHit& hit) override;
		virtual void OnEndOverlap(const CollisionHit&) override;

	private:

		int m_WalkedOnTilesFlags{ 0 };
		int m_AllFlags{ 0 };
		float m_OverlappedYPos{};

		std::unique_ptr<AudioClip> m_pOverlapSound;
	};
}