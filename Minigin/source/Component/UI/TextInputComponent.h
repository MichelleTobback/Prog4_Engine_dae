#pragma once
#include "Component/Component.h"
#include "Component/InputHandlerComponent.h"
#include "Component/TextComponent.h"

namespace dae
{
	class TextInputComponent final : public Component
	{
	public:
		TextInputComponent(GameObject* pOwner, TextComponent* pText, InputHandlerComponent* pInput);
		virtual ~TextInputComponent() override = default;

		TextInputComponent(const TextInputComponent& other) = delete;
		TextInputComponent(TextInputComponent&& other) = delete;
		TextInputComponent& operator=(const TextInputComponent& other) = delete;
		TextInputComponent& operator=(TextInputComponent&& other) = delete;

	private:
		void AddChar(float value);
		void RemoveChar();
		void CreateAddCharInputBindings(InputCommand::ICHandler& input);
		TextComponent* m_pTextComponent;

	};
}