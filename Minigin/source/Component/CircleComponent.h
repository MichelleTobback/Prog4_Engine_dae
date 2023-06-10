#pragma once
#include "Component.h"

#include <glm/glm.hpp>

namespace dae
{
	class CircleComponent final : public Component
	{
	public:
		CircleComponent(GameObject* pOwner, float radius, const glm::vec4& color = { 1.f, 1.f, 1.f, 1.f }, bool isSolid = false);
		virtual ~CircleComponent() override = default;

		CircleComponent(const CircleComponent& other) = delete;
		CircleComponent(CircleComponent&& other) = delete;
		CircleComponent& operator=(const CircleComponent& other) = delete;
		CircleComponent& operator=(CircleComponent&& other) = delete;

		float GetRadius() const;
		void SetRadius(float radius);
		const glm::vec4& GetColor() const;
		void SetColor(const glm::vec4& color);
		bool IsSolid() const { return m_IsSolid; }

	private:
		float m_Radius{};
		glm::vec4 m_Color{};
		bool m_IsSolid{};
	};
}