#include "IngredientInPlateState.h"

void dae::IngredientInPlateState::OnEnter()
{
    GetIngredient();
}

dae::State::StatePtr dae::IngredientInPlateState::OnUpdate()
{
    return this;
}

void dae::IngredientInPlateState::OnExit()
{
}
