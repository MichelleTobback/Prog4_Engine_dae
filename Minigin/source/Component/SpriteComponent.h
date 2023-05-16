#pragma once

#include "Component.h"
#include "TextureComponent.h"

#include <glm/glm.hpp>

namespace dae
{
	class Font;
	class Texture2D;
	class SpriteComponent final : public Component
	{
	public:
		SpriteComponent(GameObject* pOwner, TextureComponent* pTexture, const glm::vec4& src);
		SpriteComponent(GameObject* pOwner, TextureComponent* pTexture, float srcX = 0.f, float srcY = 0.f, float srcWidth = 0.f, float srcHeight = 0.f);
		virtual ~SpriteComponent() override = default;

		SpriteComponent(const SpriteComponent& other) = delete;
		SpriteComponent(SpriteComponent&& other) = delete;
		SpriteComponent& operator=(const SpriteComponent& other) = delete;
		SpriteComponent& operator=(SpriteComponent&& other) = delete;

		void SetTexture(TextureComponent* pTexture, const glm::vec4& src);
		void SetTexture(TextureComponent* pTexture, float srcX = 0.f, float srcY = 0.f, float srcWidth = 0.f, float srcHeight = 0.f);
		void SetSource(const glm::vec4& src);
		void SetSource(float srcX = 0.f, float srcY = 0.f, float srcWidth = 0.f, float srcHeight = 0.f);

		inline const glm::vec4& GetSource() const { return m_Source; }
		inline const TextureComponent& GetTexture() const { return *m_pTexture; }

	private:
		TextureComponent* m_pTexture{ nullptr };
		glm::vec4 m_Source{};
	};
}