#pragma once
#include <SDL.h>
#include "Core/Singleton.h"

#include <glm/glm.hpp>
#include <vector>

namespace dae
{
	class RenderComponent;
	class ImGuiComponent;
	class Texture2D;
	class Window;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_pWindow{};
		SDL_Color m_clearColor{};	
	public:
		void Init(const Window& window);
		void Render();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height, float rotation) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float srcX, float srcY, float srcWidth, float srcHeight, float rotation) const;
		void RenderQuad(float x, float y, float width, float height, const glm::vec4& color);
		void RenderSolidQuad(float x, float y, float width, float height, const glm::vec4& color);

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

	private:
		bool m_ShowDemo{ false };
		std::vector<RenderComponent*> m_pRenderComponents{};
		std::vector<ImGuiComponent*> m_pImGuiComponents{};
		friend class RenderComponent;
		friend class ImGuiComponent;
	};
}

