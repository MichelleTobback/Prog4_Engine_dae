#pragma once
#include "Component/Component.h"
#include "Component/Physics/RigidBody2DComponent.h"

#include <glm/glm.hpp>
#include <vector>

namespace dae
{
	constexpr CollisionLayer INGREDIENT_COLLISION_LAYER = CollisionLayer::Four;

	class AudioClip;
	class RigidBody2DComponent;
	class SpriteAtlasComponent;
	class BurgerIngredient : public Component
	{
	public:
		enum class IngredientType
		{
			TopBun = 0, BottomBun = 1, Cheese = 2, Patty = 3, Tomato = 4, Lettuce = 5
		};

		BurgerIngredient(GameObject* pOwner, IngredientType type, SpriteAtlasComponent* pSpriteAtlas, RigidBody2DComponent* pRigidBody);

		virtual void Awake() override;

		virtual void PlateEnter();
		void Fall();

		static BurgerIngredient* GetFromCollider(ColliderComponent* pCollider);

	protected:
		int GetWalkedFlags() const;
		void SetWalkedFlags(int flags);
		void Reset();
		void SetAllTilesTriggered();

	private:
		void CacheCurrentYPos();

		IngredientType m_Type;
		RigidBody2DComponent* m_pRigidBody;
		const float m_TileSize{ 8.f };
		const int m_Length{ 4 }; //in tiles
		int m_WalkedOnTilesFlags{ 0 }, m_AllFlags{0};
		float m_CurrentYPos{};
		bool m_OnPlate{ false };

		static std::unique_ptr<AudioClip> m_pOverlapSound;
		static std::unique_ptr<AudioClip> m_pFallSound;
	};
}