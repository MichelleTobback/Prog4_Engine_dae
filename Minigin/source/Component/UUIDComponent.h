#pragma once

#include "Component.h"
#include "Core/UUID.h"

namespace dae
{
	class UUIDComponent final : public Component
	{
	public:
		UUIDComponent(GameObject* pOwner, UUID uuid);
		virtual ~UUIDComponent() override = default;

		UUIDComponent(const UUIDComponent& other) = delete;
		UUIDComponent(UUIDComponent&& other) = delete;
		UUIDComponent& operator=(const UUIDComponent& other) = delete;
		UUIDComponent& operator=(UUIDComponent&& other) = delete;

		UUID GetUUID() const { return m_Uuid; }

	private:
		UUID m_Uuid;
	};
}