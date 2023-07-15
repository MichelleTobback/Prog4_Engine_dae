#pragma once
#include "Component/Component.h"
#include "RigidBody2DComponent.h"
#include "ColliderComponent.h"

namespace dae
{
	class CharacterController2D : public Component
	{
	public:
		struct CharacterController2DDesc
		{
			float movementSpeed{ 200.f };
		};

		CharacterController2D(GameObject* pOwner, const CharacterController2DDesc& desc);
		virtual ~CharacterController2D() override = default;

		CharacterController2D(const CharacterController2D& other) = delete;
		CharacterController2D(CharacterController2D&& other) = delete;
		CharacterController2D& operator=(const CharacterController2D& other) = delete;
		CharacterController2D& operator=(CharacterController2D&& other) = delete;

		virtual void Update() override;

		RigidBody2DComponent* GetRigidBody() const { return m_pRigidBody; }
		void SetRigidBody(RigidBody2DComponent* pRigidbody) { m_pRigidBody = pRigidbody; }
		BoxCollider2DComponent* GetCollider() const { return m_pCollider; }
		void SetCollider(BoxCollider2DComponent* pCollider);

		void Move(const glm::vec2& direction);

	private:
		CharacterController2DDesc m_Desc;
		RigidBody2DComponent* m_pRigidBody{ nullptr };
		BoxCollider2DComponent* m_pCollider{ nullptr };

		glm::vec2 m_CurrentMovementDirection{1.f, 0.f};
	};
}