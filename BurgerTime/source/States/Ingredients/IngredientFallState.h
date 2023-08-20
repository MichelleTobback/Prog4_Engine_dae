#pragma once
#include "IngredientState.h"

namespace dae
{
	class AudioClip;
	class IngredientFallState : public IngredientState
	{
	public:
		IngredientFallState(BurgerIngredient* pIngredient);
		virtual ~IngredientFallState() = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

		virtual void OnBeginOverlap(const CollisionHit&) override;
		virtual void OnEndOverlap(const CollisionHit&) override;

	private:
		float m_MinHeight{};
		std::unique_ptr<AudioClip> m_pFallSound;
	};
}