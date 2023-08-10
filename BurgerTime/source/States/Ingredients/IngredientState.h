#pragma once
#include "State/State.h"
#include "Component/Physics/RigidBody2DComponent.h"

namespace dae
{
	class BurgerIngredient;
	class IngredientState : public State
	{
	public:
		IngredientState(BurgerIngredient* pIngredient)
			: m_pIngredient{ pIngredient } {}
		virtual ~IngredientState() = default;

		virtual void OnEnter() = 0;
		virtual StatePtr OnUpdate() = 0;
		virtual void OnExit() = 0;
		virtual void OnBeginOverlap(const CollisionHit&) {}
		virtual void OnEndOverlap(const CollisionHit&) {}

		BurgerIngredient& GetIngredient() { return *m_pIngredient; }

	private:
		BurgerIngredient* m_pIngredient;
	};
}