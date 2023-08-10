#pragma once
#include "IngredientState.h"

namespace dae
{
	class IngredientFallState : public IngredientState
	{
	public:
		IngredientFallState(BurgerIngredient* pIngredient)
			: IngredientState(pIngredient) {}
		virtual ~IngredientFallState() = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

		virtual void OnBeginOverlap(const CollisionHit&) override;
		virtual void OnEndOverlap(const CollisionHit&) override;

	private:
		float m_MinHeight{};
	};
}