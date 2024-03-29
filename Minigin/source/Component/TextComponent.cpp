#include <stdexcept>
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer/Renderer.h"
#include "Renderer/Texture2D.h"
#include "TextureComponent.h"

#include "Scene/GameObject.h"

dae::TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, const glm::vec4& color)
	: Component(std::move(pOwner)), m_NeedsUpdate(true), m_Text(text), m_Font(std::move(font)), m_Color{color}
{ 
}

void dae::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		const SDL_Color color = 
		{ 
			static_cast<Uint8>(m_Color.r * 255),
			static_cast<Uint8>(m_Color.g * 255),
			static_cast<Uint8>(m_Color.b * 255) 
		};
		const std::string txt{ (!m_Text.empty()) ? m_Text : " " };
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), txt.c_str(), color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		auto tc{ GetOwner()->GetComponent<TextureComponent>()};
		tc->SetTexture(std::make_shared<Texture2D>(texture));
		m_NeedsUpdate = false;
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_NeedsUpdate = text != m_Text;
	m_Text = text;
}


