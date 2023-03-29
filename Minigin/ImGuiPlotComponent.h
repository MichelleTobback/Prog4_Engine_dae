#pragma once
#include "ImGuiComponent.h"

#include <imgui_plot.h>
#include <vector>

namespace dae
{
	class ImGuiPlotComponent final : public ImGuiComponent
	{
	public:
		ImGuiPlotComponent(GameObject* pOwner, const std::string& name);
		virtual ~ImGuiPlotComponent() override = default;

		ImGuiPlotComponent(const ImGuiPlotComponent& other) = delete;
		ImGuiPlotComponent(ImGuiPlotComponent&& other) = delete;
		ImGuiPlotComponent& operator=(const ImGuiPlotComponent& other) = delete;
		ImGuiPlotComponent& operator=(ImGuiPlotComponent&& other) = delete;

	protected:
		virtual void OnImGuiRender() override;

	private:
		void NewMeasurements();

		ImGui::PlotConfig m_Config;
		int m_Samples{ 10 };

		std::vector<float> m_Timings;
	};
}