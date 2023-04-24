#pragma once
#include "Component.h"

namespace dae
{
	class TransformComponent;
	class ControllerCommandTestComponent final : public Component
	{
	public:
		ControllerCommandTestComponent(GameObject* pGameObject, float speed, int controllerId = -1);
		virtual ~ControllerCommandTestComponent() override {}

		ControllerCommandTestComponent(const ControllerCommandTestComponent& other) = delete;
		ControllerCommandTestComponent(ControllerCommandTestComponent&& other) = delete;
		ControllerCommandTestComponent& operator=(const ControllerCommandTestComponent& other) = delete;
		ControllerCommandTestComponent& operator=(ControllerCommandTestComponent&& other) = delete;

	private:
		void MoveUp();
		void MoveDown();
		void MoveRight();
		void MoveLeft();

		void Move(float x, float y);

		TransformComponent* m_pTransform{ nullptr };
		float m_MovementSpeed;

		int m_ControllerId;
	};
}