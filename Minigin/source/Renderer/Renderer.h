#pragma once
#include <SDL.h>
#include "Core/Singleton.h"

#include <glm/glm.hpp>
#include <vector>
#include <queue>

namespace dae
{
	class CameraComponent;
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
		void RenderTexture(const Texture2D& texture, float x, float y, float srcX, float srcY, float srcWidth, float srcHeight, float rotation, bool flipHorizontal, bool flipVertical) const;
		void RenderQuad(float x, float y, float width, float height, const glm::vec4& color);
		void RenderSolidQuad(float x, float y, float width, float height, const glm::vec4& color);

		void AddComponent(RenderComponent* pComponent);
		void RemoveComponent(RenderComponent* pComponent);

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

		void SetActiveCamera(CameraComponent* pCamera);
		CameraComponent* GetActiveCamera() const;

	private:
		struct RenderComponentComparator
		{
			bool operator()(const RenderComponent* a, const RenderComponent* b) const;
		};

		bool m_ShowDemo{ false };
		std::vector<RenderComponent*> m_pRenderComponents{};
		std::vector<ImGuiComponent*> m_pImGuiComponents{};
		std::priority_queue<RenderComponent*, std::vector<RenderComponent*>, RenderComponentComparator> m_pRenderComponentQueue;
		std::queue<RenderComponent*> m_pComponentsToRemove{};
		CameraComponent* m_pActiveCamera{ nullptr };
		glm::vec2 m_CameraScale{ 1.f, 1.f };
		friend class RenderComponent;
		friend class ImGuiComponent;
	};
}

