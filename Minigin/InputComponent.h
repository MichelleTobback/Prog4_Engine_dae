#pragma once
#include "Component.h"

namespace dae
{
	class InputComponent final : public Component
	{
	public:
		InputComponent(GameObject* pGameObject);
		virtual ~InputComponent() override;

		InputComponent(const InputComponent& other) = delete;
		InputComponent(InputComponent&& other) = delete;
		InputComponent& operator=(const InputComponent& other) = delete;
		InputComponent& operator=(InputComponent&& other) = delete;

		uint8_t GetID() const { return m_InputId; }

	private:
		uint8_t m_InputId;
		static uint8_t s_InstanceCount;
	};
}