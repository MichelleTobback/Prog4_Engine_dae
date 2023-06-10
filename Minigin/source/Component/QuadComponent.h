#pragma once
#include "Component.h"

#include <glm/glm.hpp>

namespace dae
{
	class QuadComponent final : public Component
	{
	public:
		QuadComponent(GameObject* pOwner, const glm::vec2& size, const glm::vec4& color = {1.f, 1.f, 1.f, 1.f}, bool isSolid = false);
		virtual ~QuadComponent() override = default;

		QuadComponent(const QuadComponent& other) = delete;
		QuadComponent(QuadComponent&& other) = delete;
		QuadComponent& operator=(const QuadComponent& other) = delete;
		QuadComponent& operator=(QuadComponent&& other) = delete;

		const glm::vec2& GetSize() const;
		void SetSize(const glm::vec2 size);
		const glm::vec4& GetColor() const;
		void SetColor(const glm::vec4& color);
		bool IsSolid() const { return m_IsSolid; }

		const std::vector<glm::vec2>& GetVertices();
		void GetVerticesWorldPosition(std::vector<glm::vec2>& vertsOut);

		size_t GetVertexCount() const { return m_NumVerts; }

	private:
		glm::vec2 m_Size{};
		glm::vec4 m_Color{};
		bool m_IsSolid{};

		const size_t m_NumVerts{ 4 };
		std::vector<glm::vec2> m_Vertices{};
		bool m_VerticesDirty{ true };

		void RecalculateVertices();
	};
}