#pragma once
#include "Component/Component.h"
#include "Components/MovementComponent.h"
#include "TankCanon.h"
//rendering
#include "Component/TextureComponent.h"
#include "Component/RenderComponent.h"

namespace dae
{
	class HealthComponent;
	class TankComponent : public Component
	{
	public:
		struct TankDesc
		{
			float health{};
		};

		TankComponent(GameObject* pOwner, const TankDesc& desc);
		virtual ~TankComponent();

		virtual void Awake() override;


		void Move(const glm::vec2& dir);
		void RotateCanon(float dir);

		inline HealthComponent& GetHealth() const { return *m_pHealth; }
		inline MovementComponent& GetMovement() const { return *m_pMovement; }
		inline TankCanon& GetCanon() const { return *m_pTankCanon; }
		inline RigidBody2DComponent& GetRigidBody() const { return m_pMovement->GetRigidBody(); }

	private:
		float m_RotationSpeed{ 360.f };
		TankCanon* m_pTankCanon{};
		GameObject* m_pTankBody{};

		MovementComponent* m_pMovement{ nullptr };
		HealthComponent* m_pHealth{ nullptr };
	};
}