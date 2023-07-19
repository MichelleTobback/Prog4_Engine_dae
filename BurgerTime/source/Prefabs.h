#pragma once
#include <string>

#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Components/BurgerIngredient.h"
#include "Component/Physics/ColliderComponent.h"

namespace dae
{
	constexpr CollisionLayer LADDER_COLLISION_LAYER = CollisionLayer::Three;

	class Prefabs final
	{
	public:
		static GameObject* CreateBurgerPlate(Scene* pScene, RigidBody2DComponent* pRigidBody);
		static GameObject* CreateBurgerIngredient(Scene* pScene, BurgerIngredient::IngredientType type);
		static GameObject* CreatePeterPepper(Scene* pScene);
		static GameObject* CreateTestLevel(Scene* pScene);

	private:
		Prefabs() = default;
	};
}