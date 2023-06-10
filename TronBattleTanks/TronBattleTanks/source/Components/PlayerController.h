#pragma once
#include "Component/Component.h"

#include "Core/Math.h"

namespace dae
{
	class TankComponent;
	class PlayerController : public Component
	{
	public:
		PlayerController(GameObject* pOwner, int controllerIndex = -1);
		virtual ~PlayerController() override = default;

		PlayerController(const PlayerController& other) = delete;
		PlayerController(PlayerController&& other) = delete;
		PlayerController& operator=(const PlayerController& other) = delete;
		PlayerController& operator=(PlayerController&& other) = delete;

		inline TankComponent& GetTank() const { return *m_pTank; }

	private:
		int m_ControllerIndex;
		TankComponent* m_pTank{ nullptr };
	};
}