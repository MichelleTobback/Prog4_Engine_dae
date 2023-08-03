#pragma once
#include "Component/Physics/ColliderComponent.h"

namespace dae
{
	class BurgerTime final
	{
	public:
		inline static constexpr CollisionLayer LEVEL_COLLISION_LAYER = CollisionLayer::One;
		inline static constexpr CollisionLayer PLAYER_COLLISION_LAYER = CollisionLayer::Two;
		inline static constexpr CollisionLayer INGREDIENT_COLLISION_LAYER = CollisionLayer::Three;
		inline static constexpr CollisionLayer PLATE_COLLISION_LAYER = CollisionLayer::Four;
		inline static constexpr CollisionLayer ENEMY_COLLISION_LAYER = CollisionLayer::Five;

		enum class SpawnID
		{
			Player = 0,
			MrHotDog = 1
		};

	private:
		BurgerTime() = default;
	};
}