#pragma once
#include "Component.h"

namespace dae
{
	class TransformComponent;
	class ControllerCommandTestComponent final : public Component
	{
	public:
		ControllerCommandTestComponent(GameObject* pGameObject, float speed, unsigned int controllerId = 0);
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

		unsigned int m_ControllerId;
	};

	class KeyboardCommandTestComponent final : public Component
	{
	public:
		KeyboardCommandTestComponent(GameObject* pGameObject, float speed);
		virtual ~KeyboardCommandTestComponent() override {}

		KeyboardCommandTestComponent(const KeyboardCommandTestComponent& other) = delete;
		KeyboardCommandTestComponent(KeyboardCommandTestComponent&& other) = delete;
		KeyboardCommandTestComponent& operator=(const KeyboardCommandTestComponent& other) = delete;
		KeyboardCommandTestComponent& operator=(KeyboardCommandTestComponent&& other) = delete;

	private:
		void MoveUp();
		void MoveDown();
		void MoveRight();
		void MoveLeft();

		void Move(float x, float y);

		TransformComponent* m_pTransform{ nullptr };
		float m_MovementSpeed;
	};
}