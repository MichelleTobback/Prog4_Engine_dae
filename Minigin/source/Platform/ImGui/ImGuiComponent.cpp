#include "ImGuiComponent.h"
#include "Renderer/Renderer.h"

dae::ImGuiComponent::ImGuiComponent(GameObject* pOwner, const std::string& name)
	: Component(pOwner), m_Name{name}
{
	Renderer::GetInstance().m_pImGuiComponents.push_back(this);
}

void dae::ImGuiComponent::ImGuiRender()
{
	ImGui::Begin(m_Name.c_str());

	OnImGuiRender();

	ImGui::End();
}
