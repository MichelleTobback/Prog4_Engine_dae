#pragma once
#include "IngredientState.h"

namespace dae
{
	class IngredientInPlateState : public IngredientState
	{
	public:
		IngredientInPlateState(BurgerIngredient* pIngredient)
			: IngredientState(pIngredient) {}
		virtual ~IngredientInPlateState() = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;
	};
}