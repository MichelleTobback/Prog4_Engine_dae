#pragma once
#include "Component/Component.h"
#include "Components/CharacterInfo.h"

#include <memory>

namespace dae
{
	class HUDComponent final : public Component
	{
	public:
		HUDComponent(GameObject* pOwner, CharacterInfo* player);
		virtual ~HUDComponent() = default;

		HUDComponent(const HUDComponent& other) = delete;
		HUDComponent(HUDComponent&& other) = delete;
		HUDComponent& operator=(const HUDComponent& other) = delete;
		HUDComponent& operator=(HUDComponent&& other) = delete;

		virtual void Awake() override;
	};
}