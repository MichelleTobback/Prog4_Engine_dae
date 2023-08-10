#pragma once
#include "Component/Component.h"
#include "Component/Physics/RigidBody2DComponent.h"

#include "States/Ingredients/IngredientState.h"

#include <glm/glm.hpp>
#include <vector>

namespace dae
{
	class StateMachine;
	class BTGameMode;
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

		struct IngredientStates
		{
			std::unique_ptr<IngredientState> pIdle{ nullptr };
			std::unique_ptr<IngredientState> pFall{ nullptr };
			std::unique_ptr<IngredientState> pInPlate{ nullptr };
		};

		BurgerIngredient(GameObject* pOwner, IngredientType type, SpriteAtlasComponent* pSpriteAtlas, RigidBody2DComponent* pRigidBody, uint32_t reward);

		virtual void Awake() override;

		virtual void PlateEnter();
		void Fall();

		inline IngredientStates& GetStates() { return m_pStates; }
		inline RigidBody2DComponent& GetRigidBody() { return *m_pRigidBody; }

		static BurgerIngredient* GetFromCollider(ColliderComponent* pCollider);

		bool IsOnPlate() const { return m_OnPlate; }
		void AddReward();

	protected:
		int GetWalkedFlags() const;
		void SetWalkedFlags(int flags);
		void Reset();
		void SetAllTilesTriggered();

	private:
		void OnBeginOverlap(const CollisionHit& hit);
		void OnEndOverlap(const CollisionHit& hit);

		IngredientType m_Type;
		RigidBody2DComponent* m_pRigidBody;
		const float m_TileSize{ 8.f };
		const int m_Length{ 4 }; //in tiles
		int m_WalkedOnTilesFlags{ 0 }, m_AllFlags{0};
		bool m_OnPlate{ false };
		uint32_t m_Reward;
		BTGameMode* m_pCurrentGameMode{ nullptr };

		IngredientStates m_pStates{};
		StateMachine* m_pStateMachine{ nullptr };

		static std::unique_ptr<AudioClip> m_pOverlapSound;
		static std::unique_ptr<AudioClip> m_pFallSound;
	};
}