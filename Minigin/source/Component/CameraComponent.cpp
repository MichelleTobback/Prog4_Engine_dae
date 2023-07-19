#include "CameraComponent.h"
#include "Renderer/Renderer.h"
#include "Managers/ServiceLocator.h"

dae::CameraComponent::CameraComponent(GameObject* pOwner, float aspectRatio)
	: Component(pOwner), m_Size{ 0.f, 0.f }
{
	auto& window{ ServiceLocator::GetWindow() };
	m_Size.x = window.GetWidth() * aspectRatio;
	m_Size.y = window.GetHeight() * aspectRatio;
}

dae::CameraComponent::CameraComponent(GameObject* pOwner, const glm::vec2& size)
	: Component(pOwner), m_Size{size}
{
}

void dae::CameraComponent::SetCurrent()
{
	Renderer::GetInstance().SetActiveCamera(this);
}
