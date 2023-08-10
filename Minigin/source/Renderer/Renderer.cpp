#include <stdexcept>
#include "Renderer.h"
#include "Managers/SceneManager.h"
#include "Renderer/Texture2D.h"
#include "Core/Window.h"
#include "Core/BitFlag.h"

#include "Component/RenderComponent.h"
#include "Platform/ImGui/ImGuiComponent.h"
#include "Component/CameraComponent.h"
#include "Managers/ServiceLocator.h"

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
	while (!m_pComponentsToRemove.empty())
	{
		RenderComponent* componentToRemove = m_pComponentsToRemove.front();
		m_pComponentsToRemove.pop();
		m_pRenderComponents.erase(
			std::remove(m_pRenderComponents.begin(), m_pRenderComponents.end(), componentToRemove),
			m_pRenderComponents.end());
	}

	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	if (m_pActiveCamera)
	{
		auto& window{ ServiceLocator::GetWindow() };
		const auto& camSize{ m_pActiveCamera->GetSize() };
		m_CameraScale.x = window.GetWidth() / camSize.x;
		m_CameraScale.y = window.GetHeight() / camSize.y;
	}
	else
		m_CameraScale = { 1.f, 1.f };

	for (auto& pComponent : m_pRenderComponents)
	{
		if (pComponent && pComponent->GetLayer() == 0)
		{
			pComponent->Render();
		}
		else if (pComponent)
		{
			m_pRenderComponentQueue.push(pComponent);
		}
	}

	while (!m_pRenderComponentQueue.empty())
	{
		RenderComponent* pComponent{ m_pRenderComponentQueue.top() };
		pComponent->Render();
		m_pRenderComponentQueue.pop();
	}

	//ImGui
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_pWindow);
	ImGui::NewFrame();

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
	glm::vec3 camPos{ GetCameraPos() };
	dst.x = static_cast<int>(x * m_CameraScale.x + camPos.x);
	dst.y = static_cast<int>(y * m_CameraScale.y + camPos.y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	dst.w = static_cast<int>(dst.w * m_CameraScale.x);
	dst.h = static_cast<int>(dst.h * m_CameraScale.y);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	glm::vec3 camPos{ GetCameraPos() };
	dst.x = static_cast<int>(x * m_CameraScale.x + camPos.x);
	dst.y = static_cast<int>(y * m_CameraScale.y + camPos.y);
	dst.w = static_cast<int>(width * m_CameraScale.x);
	dst.h = static_cast<int>(height * m_CameraScale.y);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, float x, float y, float width, float height, float rotation) const
{
	SDL_Rect dst{};
	glm::vec3 camPos{ GetCameraPos() };
	dst.x = static_cast<int>(x * m_CameraScale.x + camPos.x);
	dst.y = static_cast<int>(y * m_CameraScale.y + camPos.y);
	dst.w = static_cast<int>(width * m_CameraScale.x);
	dst.h = static_cast<int>(height * m_CameraScale.y);
	SDL_Point offset{ dst.x + static_cast<int>(width * 0.5f), dst.y + static_cast<int>(height * 0.5f)};
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, rotation, &offset, SDL_FLIP_NONE);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, float x, float y, float srcX, float srcY, float srcWidth, float srcHeight, float rotation) const
{
	SDL_Rect dst{};
	glm::vec3 camPos{ GetCameraPos() };
	dst.x = static_cast<int>(x * m_CameraScale.x + camPos.x);
	dst.y = static_cast<int>(y * m_CameraScale.y + camPos.y);
	dst.w = static_cast<int>(srcWidth * m_CameraScale.x);
	dst.h = static_cast<int>(srcHeight * m_CameraScale.y);

	SDL_Rect src{};
	src.x = static_cast<int>(srcX);
	src.y = static_cast<int>(srcY);
	src.w = static_cast<int>(srcWidth);
	src.h = static_cast<int>(srcHeight);

	SDL_Point offset{ dst.x + static_cast<int>(srcWidth * 0.5f), dst.y + static_cast<int>(srcHeight * 0.5f) };

	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst, rotation, NULL, SDL_FLIP_NONE);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, float x, float y, float srcX, float srcY, float srcWidth, float srcHeight, float rotation, bool flipHorizontal, bool flipVertical) const
{
	SDL_Rect dst{};
	glm::vec3 camPos{ GetCameraPos() };
	dst.x = static_cast<int>(x * m_CameraScale.x + camPos.x);
	dst.y = static_cast<int>(y * m_CameraScale.y + camPos.y);
	dst.w = static_cast<int>(srcWidth * m_CameraScale.x);
	dst.h = static_cast<int>(srcHeight * m_CameraScale.y);

	SDL_Rect src{};
	src.x = static_cast<int>(srcX);
	src.y = static_cast<int>(srcY);
	src.w = static_cast<int>(srcWidth);
	src.h = static_cast<int>(srcHeight);

	SDL_Point offset{ dst.x + static_cast<int>(srcWidth * 0.5f), dst.y + static_cast<int>(srcHeight * 0.5f) };

	SDL_RendererFlip flip{ SDL_RendererFlip::SDL_FLIP_NONE };
	if (flipHorizontal)
		BitFlag::Set(flip, SDL_RendererFlip::SDL_FLIP_HORIZONTAL, true);
	if (flipVertical)
		BitFlag::Set(flip, SDL_RendererFlip::SDL_FLIP_VERTICAL, true);

	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst, rotation, NULL, flip);
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

	glm::vec3 camPos{ GetCameraPos() };
	int xI{ static_cast<int>(x * m_CameraScale.x + camPos.x) };
	int yI{ static_cast<int>(y * m_CameraScale.y + camPos.y) };
	int widthI{ static_cast<int>(width * m_CameraScale.x) };
	int heightI{ static_cast<int>(height * m_CameraScale.y) };
	
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

	glm::vec3 camPos{ GetCameraPos() };
	int xI{ static_cast<int>(x * m_CameraScale.x + camPos.x) };
	int yI{ static_cast<int>(y * m_CameraScale.y + camPos.y) };
	int widthI{ static_cast<int>(width * m_CameraScale.x) };
	int heightI{ static_cast<int>(height * m_CameraScale.y) };
	
	SDL_Rect rect{ xI, yI, widthI, heightI };
	SDL_RenderFillRect(m_renderer, &rect);
	
}

void dae::Renderer::AddComponent(RenderComponent* pComponent)
{
	m_pRenderComponents.push_back(pComponent);
}

void dae::Renderer::RemoveComponent(RenderComponent* pComponent)
{
	m_pComponentsToRemove.push(pComponent);
}

inline SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }

void dae::Renderer::SetActiveCamera(CameraComponent* pCamera)
{
	if (m_pActiveCamera)
		m_pActiveCamera->m_Current = false;
	m_pActiveCamera = pCamera;
	if (m_pActiveCamera)
		m_pActiveCamera->m_Current = true;
}

dae::CameraComponent* dae::Renderer::GetActiveCamera() const
{
	return m_pActiveCamera;
}

glm::vec3 dae::Renderer::GetCameraPos() const
{
	if (m_pActiveCamera)
		return m_pActiveCamera->GetTransform().GetWorldPosition();
	return glm::vec3();
}

bool dae::Renderer::RenderComponentComparator::operator()(const RenderComponent* a, const RenderComponent* b) const
{
	return a->GetLayer() > b->GetLayer();
}
