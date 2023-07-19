#pragma once
#include "Component/Component.h"
#include "Component/Physics/CharacterController2D.h"

#include "Core/Math.h"

namespace dae
{
	class BurgerTimeMovementController : public Component
	{
	public:
		BurgerTimeMovementController(GameObject* pOwner, CharacterController2D* pCharactarerController, CollisionLayer layer = CollisionLayer::One);
		virtual ~BurgerTimeMovementController() override = default;

		BurgerTimeMovementController(const BurgerTimeMovementController& other) = delete;
		BurgerTimeMovementController(BurgerTimeMovementController&& other) = delete;
		BurgerTimeMovementController& operator=(const BurgerTimeMovementController& other) = delete;
		BurgerTimeMovementController& operator=(BurgerTimeMovementController&& other) = delete;

		CharacterController2D& GetCharacterController() const;

		void Move(const glm::vec2& dir);

	private:
		void OnOverlap(const CollisionHit& hit);
		void OnEndOverlap(const CollisionHit& hit);

		BoxCollider2DComponent* m_pLadderCollider{ nullptr };
		CharacterController2D* m_pCharacterController;
		bool m_CanMoveVertical{ false };
		float m_VerticalOffset{ 2.f };
	};
}