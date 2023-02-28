#pragma once
#include "Component.h"

namespace dae
{
	class FPSComponent final : public Component
	{
	public:
		FPSComponent() = default;
		virtual ~FPSComponent() override = default;

		virtual void Update(GameObject* pGameObject) override;

	private:
		int m_FPSCount{};
		int m_FPS{};
		float m_FPSTimer{};
	};
}