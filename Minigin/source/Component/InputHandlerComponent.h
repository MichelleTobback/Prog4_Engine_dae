#pragma once
#include "Component.h"
#include "Input/InputCommand.h"

namespace dae
{
	class InputHandlerComponent final : public Component
	{
	public:
		InputHandlerComponent(GameObject* pOwner);
		virtual ~InputHandlerComponent() override = default;

		InputHandlerComponent(const InputHandlerComponent& other) = delete;
		InputHandlerComponent(InputHandlerComponent&& other) = delete;
		InputHandlerComponent& operator=(const InputHandlerComponent& other) = delete;
		InputHandlerComponent& operator=(InputHandlerComponent&& other) = delete;

		virtual void Update() override;

		InputCommand::ICHandler& GetHandler();

	private:
		std::unique_ptr<InputCommand::ICHandler> m_pCommandHandler;
	};
}