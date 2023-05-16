#pragma once
#include <string>
#include <memory>

#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final : public Component
	{
	public:
		TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});
		virtual ~TextComponent() override = default;

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		virtual void Update() override;

		void SetText(const std::string& text);
		inline const std::string& GetText() const { return m_Text; }
		inline const glm::vec4& GetColor() const { return m_Color; }
		inline Font* GetFont() const { return m_Font.get(); }

	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		glm::vec4 m_Color;
	};
}
