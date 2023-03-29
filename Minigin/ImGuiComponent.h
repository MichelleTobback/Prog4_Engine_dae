#pragma once
#include "Component.h"

#include <imgui.h>

namespace dae
{
	class ImGuiComponent : public Component
	{
	public:
		ImGuiComponent(GameObject* pOwner, const std::string& name);
		virtual ~ImGuiComponent() override = default;

		ImGuiComponent(const ImGuiComponent& other) = delete;
		ImGuiComponent(ImGuiComponent&& other) = delete;
		ImGuiComponent& operator=(const ImGuiComponent& other) = delete;
		ImGuiComponent& operator=(ImGuiComponent&& other) = delete;

		void ImGuiRender();

	protected:
		virtual void OnImGuiRender(){}

	private:
		const std::string m_Name;
	};
}