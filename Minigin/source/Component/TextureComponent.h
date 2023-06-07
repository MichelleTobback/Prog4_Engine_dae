#pragma once
#include "Component.h"
#include "Renderer/Texture2D.h"

#include <memory>
#include <glm/glm.hpp>
#include <string>

namespace dae
{
	class Texture2D;

	class TextureComponent : public Component
	{
	public:
		TextureComponent(GameObject* pOwner);
		virtual ~TextureComponent() override = default;

		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

		const std::shared_ptr<Texture2D>& GetTexture() const;
		void SetTexture(const std::string& path);
		void SetTexture(const std::shared_ptr<Texture2D>& pTexture);

		const glm::vec2& GetSize() const;
		void SetSize(const glm::vec2 size);
		bool HasTexture() const { return m_pTexture != nullptr; }

	private:
		std::shared_ptr<Texture2D> m_pTexture{nullptr};

		glm::vec2 m_Size{};
	};
}