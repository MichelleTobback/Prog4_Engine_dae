#pragma once
#include "Component.h"

namespace dae
{
	class GameObject;
	class TextComponent;
	class FPSComponent final : public Component
	{
	public:
		FPSComponent(GameObject* pOwner);
		virtual ~FPSComponent() override = default;

		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;


		virtual void Update() override;

	private:
		int m_FPSCount{};
		int m_FPS{};
		float m_FPSTimer{};

		TextComponent* m_pTextComponent{nullptr};
	};
}