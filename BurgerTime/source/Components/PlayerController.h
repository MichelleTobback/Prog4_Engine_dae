#pragma once
#include "Component/Component.h"
#include "Component/Physics/CharacterController2D.h"

#include "Core/Delegate.h"
#include "Core/Math.h"

#include "Audio/AudioClip.h"

namespace dae
{
	class BTGameMode;
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

		ObservableType<uint32_t>& GetPeppers() { return m_Peppers; }

	private:
		void ThrowPepper();
		void OnHit(uint32_t health);
		void OnDeath();

		CharacterController2D* m_pCharacterController;
		int m_ControllerIndex;
		bool m_Reset{ false };

		ObservableType<uint32_t> m_Peppers{5};

		BTGameMode* m_pCurrentGameMode{nullptr};
		std::unique_ptr<AudioClip> m_pDamageSound;
	};
}