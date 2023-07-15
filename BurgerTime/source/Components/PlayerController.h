#pragma once
#include "Component/Component.h"
#include "Component/Physics/CharacterController2D.h"

#include "Core/Math.h"

namespace dae
{
	constexpr CollisionLayer PLAYER_COLLISION_LAYER = CollisionLayer::Two;

	class PlayerController : public Component
	{
	public:
		PlayerController(GameObject* pOwner, CharacterController2D* pCharactarerController, int controllerIndex = -1);
		virtual ~PlayerController() override = default;

		PlayerController(const PlayerController& other) = delete;
		PlayerController(PlayerController&& other) = delete;
		PlayerController& operator=(const PlayerController& other) = delete;
		PlayerController& operator=(PlayerController&& other) = delete;

		virtual void Awake() override;
		virtual void LateUpdate() override;

	private:
		void OnDeath();
		void Move(const glm::vec2& dir);
		void OnOverlap(const CollisionHit& hit);
		void OnEndOverlap(const CollisionHit& hit);

		BoxCollider2DComponent* m_pLadderCollider{ nullptr };
		CharacterController2D* m_pCharacterController;
		int m_ControllerIndex;
		bool m_Reset{ false }, m_CanMoveVertical{false};
		float m_VerticalOffset{ 2.f };
	};
}