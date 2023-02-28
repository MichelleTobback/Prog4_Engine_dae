#pragma once
#include "Component.h"

#include <memory>
#include <glm/glm.hpp>
#include <string>

namespace dae
{
	class Texture2D;

	class TextureComponent : public Component
	{
	public:
		TextureComponent() = default;
		virtual ~TextureComponent() override = default;

		const std::shared_ptr<Texture2D>& GetTexture() const;
		void SetTexture(const std::string& path);
		void SetTexture(const std::shared_ptr<Texture2D>& pTexture);

		const glm::vec2& GetSize() const;
		void SetSize(const glm::vec2 size);

	private:
		std::shared_ptr<Texture2D> m_pTexture{};

		glm::vec2 m_Size;
	};
}