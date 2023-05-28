#include <stdexcept>
#include "Renderer.h"
#include "Managers/SceneManager.h"
#include "Renderer/Texture2D.h"
#include "Core/Window.h"

#include "Component/RenderComponent.h"
#include "Platform/ImGui/ImGuiComponent.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(const Window& pWindow)
{
	m_pWindow = SDL_GetWindowFromID(pWindow.GetID());

	if (m_pWindow == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	m_renderer = SDL_CreateRenderer(m_pWindow, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(m_pWindow, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void dae::Renderer::Render()
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	for (auto& pComponent : m_pRenderComponents)
	{
		if (pComponent)
			pComponent->Render();
	}

	//ImGui
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_pWindow);
	ImGui::NewFrame();
	//SceneManager::GetInstance().OnImGuiRender();

	if (m_ShowDemo)
		ImGui::ShowDemoWindow(&m_ShowDemo);

	for (auto& pComponent : m_pImGuiComponents)
	{
		if (pComponent)
			pComponent->ImGuiRender();
	}

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	
	//draw frame
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

//====================================================================================
// TextureRender
//====================================================================================

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, float x, float y, float width, float height, float rotation) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_Point offset{static_cast<int>(width * 0.5f), static_cast<int>(height * 0.5f)};
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, rotation, &offset, SDL_FLIP_NONE);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, float x, float y, float srcX, float srcY, float srcWidth, float srcHeight, float rotation) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(srcWidth);
	dst.h = static_cast<int>(srcHeight);

	SDL_Rect src{};
	src.x = static_cast<int>(srcX);
	src.y = static_cast<int>(srcY);
	src.w = static_cast<int>(srcWidth);
	src.h = static_cast<int>(srcHeight);

	SDL_Point offset{ src.x + static_cast<int>(srcWidth * 0.5f), src.y + static_cast<int>(srcHeight * 0.5f) };

	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst, rotation, &offset, SDL_FLIP_NONE);
}

//====================================================================================
// QuadRender
//====================================================================================

void dae::Renderer::RenderQuad(float x, float y, float width, float height, const glm::vec4& color)
{
	SDL_Color colorSdl{};
	colorSdl.r = static_cast<Uint8>(color.r * 255);
	colorSdl.g = static_cast<Uint8>(color.g * 255);
	colorSdl.b = static_cast<Uint8>(color.b * 255);
	colorSdl.a = static_cast<Uint8>(color.a * 255);
	SDL_SetRenderDrawColor(m_renderer, colorSdl.r, colorSdl.g, colorSdl.b, colorSdl.a);

	int xI{ static_cast<int>(x) };
	int yI{ static_cast<int>(y) };
	int widthI{ static_cast<int>(width) };
	int heightI{ static_cast<int>(height) };
	
	SDL_Rect rect{ xI, yI, widthI, heightI };
	SDL_RenderDrawRect(m_renderer, &rect);
}

void dae::Renderer::RenderSolidQuad(float x, float y, float width, float height, const glm::vec4& color)
{
	SDL_Color colorSdl{};
	colorSdl.r = static_cast<Uint8>(color.r * 255);
	colorSdl.g = static_cast<Uint8>(color.g * 255);
	colorSdl.b = static_cast<Uint8>(color.b * 255);
	colorSdl.a = static_cast<Uint8>(color.a * 255);
	SDL_SetRenderDrawColor(m_renderer, colorSdl.r, colorSdl.g, colorSdl.b, colorSdl.a);

	int xI{ static_cast<int>(x) };
	int yI{ static_cast<int>(y) };
	int widthI{ static_cast<int>(width) };
	int heightI{ static_cast<int>(height) };
	
	SDL_Rect rect{ xI, yI, widthI, heightI };
	SDL_RenderFillRect(m_renderer, &rect);
	
}

inline SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
