#pragma once
#include "Component/Physics/ColliderComponent.h"

namespace dae
{
	class SpriteAtlasComponent;
	class SpriteAnimatorComponent;
	class BurgerTime final
	{
	public:
		inline static constexpr float TILE_SIZE = 8.f;
		inline static constexpr float SPRITE_SIZE = TILE_SIZE * 2.f;

		inline static constexpr CollisionLayer LEVEL_COLLISION_LAYER = CollisionLayer::One;
		inline static constexpr CollisionLayer PLAYER_COLLISION_LAYER = CollisionLayer::Two;
		inline static constexpr CollisionLayer INGREDIENT_COLLISION_LAYER = CollisionLayer::Three;
		inline static constexpr CollisionLayer PLATE_COLLISION_LAYER = CollisionLayer::Four;
		inline static constexpr CollisionLayer ENEMY_COLLISION_LAYER = CollisionLayer::Five;
		inline static constexpr CollisionLayer PEPPER_COLLISION_LAYER = CollisionLayer::Six;

		enum class SpawnID
		{
			Player = 0,
			MrHotDog = 1, MrEgg = 2, MrPickle = 3,
			TopBun, BottomBun, Lettuce, Tomato, Patty, Cheese
		};

		enum class LevelID
		{
			Menu,
			Level1, Level2, Level3
		};

		enum class InputID
		{
			Move, Throw, ChangeScene
		};

		// * rect.x, rect.y = spritestart pos
		// * rect.z, rect.w = num cols, num rows
		// * spriteSize	= pixel width/height of sprite
		static size_t CreateAnimClip(SpriteAtlasComponent* pAtlas, SpriteAnimatorComponent* pAnimator, const glm::u32vec4& rect, bool flip = false, float spriteSize = SPRITE_SIZE, float speed = 1.f);
		static bool IsIngredient(SpawnID id);

		static void CreateDebugInput();
		static void LoadScenes();

		static void CreateInputBindings();

	private:
		BurgerTime() = default;
	};
}